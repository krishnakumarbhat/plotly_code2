import clang.cindex
import argparse
import os
from typing import Tuple, Optional

class StreamDefinitionGenerator():
   """
   A class to generate stream definitions from C++ header files.

   This class uses the Clang library to parse C++ header files and generate
   stream definitions based on specified structs.
   """
   def generate_stream_definition(self) -> None:
      """
         Generate the stream definition and write it to the output file.
      """
      with open(self.output_file, "w") as self.streamdef_file:
         self.parse()
         self.find_structs_and_fields(self.translation_unit.cursor)

   def __init__(self, package_install_dir : str, file_to_parse : str, struct_to_parse : str, output_file : str):
      """
      Initialize the StreamDefinitionGenerator.

      Args:
         package_install_dir (str): Path to the SG installed folder.
         file_to_parse (str): Path to the header file to parse.
         struct_to_parse (str): Name of the struct to be parsed.
         output_file (str): Name of the output file for stream definitions.
      """
      clang.cindex.Config.set_library_file("/usr/lib/llvm-15/lib/libclang.so.1")
      self.index = clang.cindex.Index.create()

      self.package_install_dir = package_install_dir
      self.file_to_parse = file_to_parse
      self.struct_to_parse = struct_to_parse
      self.output_file = output_file
      self.file_path = file_to_parse

      self.struct_found = False

   def clean_type_name(self, type_name : str) -> str:
      """
        Remove 'std::' prefix from type names.

        Args:
            type_name (str): The original type name.

        Returns:
            str: The cleaned type name.
        """
      return type_name.removeprefix("std::")

   def get_exact_type(self, ctype : clang.cindex.Type) -> str:
      """
      Get the exact type name for a given Clang type.

      Args:
         ctype: Clang type object.

      Returns:
         str: The exact type name.
      """
      typedef_name = ctype.get_typedef_name()

      if typedef_name == "float32_t":
         return "float"

      decl_cursor = ctype.get_declaration()
      if decl_cursor.kind == clang.cindex.CursorKind.ENUM_DECL:
         underlying_type = decl_cursor.enum_type
         element_type_name = underlying_type.spelling
         return self.clean_type_name(element_type_name)

      if typedef_name:
         return typedef_name
      return ctype.spelling

   def parse(self) -> None:
      """
      Parse the specified header file using Clang.
      """
      self.translation_unit = self.index.parse(self.file_path, args=["-x",
                                    "c++",
                                    "-I/usr/include",
                                    "-std=c++14",
                                    f"-I{self.package_install_dir}/sg/iface",
                                    f"-I{self.package_install_dir}/rspp/iface"])

   def is_array_type(self, ctype: clang.cindex.Type) -> Tuple[bool, Optional[int], Optional[str]]:
      """
      Check if a given type is an array type.

      Args:
         ctype: Clang type object.

      Returns:
         tuple: (is_array, array_size, element_type)
      """
      if ctype.kind == clang.cindex.TypeKind.CONSTANTARRAY:
         element_type = ctype.element_type
         array_size = ctype.element_count

         decl_cursor = element_type.get_declaration()

         # handle array of enum types
         if decl_cursor.kind == clang.cindex.CursorKind.ENUM_DECL:
            underlying_type = decl_cursor.enum_type
            element_type_name = underlying_type.spelling
         else:
            element_type_name = element_type.spelling

         if "float32_t" in element_type_name:
            return True, array_size, "float"
         return True, array_size, self.clean_type_name(element_type_name)
      return False, None, None

   def parse_array_type(self, child : clang.cindex.Cursor, array_size : int, element_type : str, full_var_name : str) -> None:
      """
      Parse and write the stream definition for an array type.

      Args:
         child: Clang cursor for the array field.
         array_size (int): Size of the array.
         element_type (str): Type of the array elements.
         full_var_name (str): Full variable name including parent struct names.
      """
      if child.type.get_array_element_type().get_declaration().kind == clang.cindex.CursorKind.STRUCT_DECL:
         self.streamdef_file.write(f"REPEAT{array_size}\n")
         self.find_structs_and_fields(child.type.get_array_element_type().get_declaration(), full_var_name)
         self.streamdef_file.write("END_REPEAT\n")
      else:
         self.streamdef_file.write(f"REPEAT{array_size}\n")
         self.streamdef_file.write(f"{element_type} {full_var_name}\n")
         self.streamdef_file.write("END_REPEAT\n")

   def calculate_offset(self, node : clang.cindex.Cursor, child : clang.cindex.Cursor, offset : int) -> int:
      """
      Calculate the offset for a field and add padding if necessary.

      Args:
         node: Clang cursor for the parent struct.
         child: Clang cursor for the field.
         offset (int): Current offset.

      Returns:
         int: Updated offset.
      """
      field_size = child.type.get_size()
      field_offset = node.type.get_offset(child.spelling) // 8

      if field_offset > offset:
         padding_size = field_offset - offset
         self.streamdef_file.write(f"PADDING{padding_size}\n")
         offset += padding_size

      offset = field_offset + field_size
      return offset

   def parse_struct_def(self, node : clang.cindex.Cursor, parent_var_name : str, offset : int, struct_size : int) -> None:
      """
      Parse and write the stream definition for a struct.

      Args:
         node: Clang cursor for the struct.
         parent_var_name (str): Name of the parent variable (for nested structs).
         offset (int): Current offset within the struct.
         struct_size (int): Total size of the struct.
      """
      for child in node.get_children():
         if child.kind == clang.cindex.CursorKind.FIELD_DECL:
            field_name = child.spelling
            field_type = self.get_exact_type(child.type)
            full_var_name = f"{parent_var_name}.{field_name}" if parent_var_name else field_name

            offset = self.calculate_offset(node, child, offset)

            if field_name.startswith("__"):
               continue

            is_array, array_size, element_type = self.is_array_type(child.type)

            if is_array:
               self.parse_array_type(child, array_size, element_type, full_var_name)
            else:
               if child.type.get_declaration().kind == clang.cindex.CursorKind.STRUCT_DECL:
                  self.find_structs_and_fields(child.type.get_declaration(), full_var_name)
               else:
                  self.streamdef_file.write(f"{field_type} {full_var_name}\n")
      self.add_padding(offset, struct_size)

   def add_padding(self, offset : int, struct_size : int) -> None:
      """
      Add padding to the stream definition if necessary.

      This method checks if there's any remaining space in the struct after
      all fields have been processed. If there is, it adds padding to fill
      the remaining space.

      Args:
         offset (int): The current offset after processing all fields.
         struct_size (int): The total size of the struct.

    """
      if offset < struct_size:
         padding_size = struct_size - offset
         self.streamdef_file.write(f"PADDING{padding_size}\n")

   def find_structs_and_fields(self, node : clang.cindex.Cursor, parent_var_name = "", current_offset=0) -> None:
      """
      Recursively find and process structs and their fields in the AST.

      This method traverses the Abstract Syntax Tree (AST) to find the specified struct
      and its fields. It handles nested structs, namespaces, and writes the stream
      definition for each field encountered.

      Args:
         node (clang.cindex.Cursor): The current node in the AST being processed.
         parent_var_name (str, optional): The name of the parent variable for nested structs.
                                          Defaults to an empty string.
         current_offset (int, optional): The current byte offset within the struct.
                                          Defaults to 0.

      Returns:
         None

      Side effects:
         - Updates self.struct_found when the target struct is found.
         - Writes to self.streamdef_file with the stream definition entries.
      """
      struct_type = node.type
      struct_size = struct_type.get_size()
      offset = current_offset

      if node.kind == clang.cindex.CursorKind.NAMESPACE:
         for child in node.get_children():
            if child.spelling == self.struct_to_parse:
               total_size = child.type.get_size()
               self.streamdef_file.write(str(total_size)+"\n")
               self.struct_found = True
               self.find_structs_and_fields(child, parent_var_name)
         return
      if node.kind == clang.cindex.CursorKind.STRUCT_DECL:
         if node.location.file is None:
            return
         if node.spelling != self.struct_to_parse and not self.struct_found:
            return

      if node.kind == clang.cindex.CursorKind.STRUCT_DECL and node.is_definition():
         self.parse_struct_def(node, parent_var_name, offset, struct_size)

      if node.kind != clang.cindex.CursorKind.STRUCT_DECL:
         for child in node.get_children():
            self.find_structs_and_fields(child, parent_var_name)

"""
NOTE: This script depends on Linux packeges. Windows is NOT supported
"""
if __name__ == "__main__":
   parser = argparse.ArgumentParser()
   parser.add_argument("-sg", "--package_install_dir", help="Path to the SG installation (via cmake) folder", required=True)
   parser.add_argument("-f", "--header_file", help="Path to the header file to parse", required=True)
   parser.add_argument("-s", "--struct", help="Name of struct to be parsed", required=True)
   parser.add_argument("-o", "--output_file", default="strdef.txt", help="Name of struct to be parsed")

   args = parser.parse_args()

   generator = StreamDefinitionGenerator(args.package_install_dir, args.header_file, args.struct, args.output_file)

   generator.generate_stream_definition()
