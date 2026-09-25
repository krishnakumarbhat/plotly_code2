import argparse
from generate_stream_definitions import StreamDefinitionGenerator
import os
import re
from typing import Tuple, Optional, TextIO
import difflib


class StreamDefinitionComparator():
   """
   A class to compare stream definitions.

   This class generates a new stream definition and compares it with an existing reference stream definition.
   """

   def compare_streams(self) -> int:
      """
      Compare the generated stream definition with the reference stream definition.

      Returns:
         int: 0 if streams are identical, 1 otherwise.
      """
      self.generator.generate_stream_definition()

      reference_stream_path = self._find_reference_stream_file()
      if reference_stream_path:
         with open(self.tmp_stream_file ,'r') as new_stream, open(reference_stream_path, 'r') as reference_stream:
            comparison_result, cmp_message = self._compare_stream_definition_files(new_stream, reference_stream)
            if comparison_result:
               print("Streams are identical")
               return 0
            else:
               self._diff(reference_stream_path, self.tmp_stream_file)
               print(f"stream version {self.stream_version}")
               print(f"Streams are not identical{': ' if cmp_message else ''}{cmp_message}")
               return 1
      else:
         print(f"No definition file found for stream {self.stream_number} version {self.stream_version}")
         return 1

   def __init__(self, sg_repo_path: str, file_to_parse: str, struct_to_parse: str, output_file: Optional[str] = None, stream_definitions_folder: Optional[str] = None) -> None:
      """
      Initialize the StreamDefinitionComparator.

      Args:
         sg_repo_path (str): Path to the SG repo main folder.
         file_to_parse (str): Path to the header file to parse.
         struct_to_parse (str): Name of the struct to be parsed.
         output_file (Optional[str]): Name of the output file. Defaults to None.
         stream_definitions_folder (Optional[str]): Path to folder containing stream definitions. Defaults to None.
      """
      self.file_to_parse = file_to_parse
      self.tmp_stream_file = output_file if not output_file else "tmp_stream.txt"
      self.stream_definitions_folder = stream_definitions_folder

      self.generator = StreamDefinitionGenerator(sg_repo_path, file_to_parse, struct_to_parse, self.tmp_stream_file)

   @staticmethod
   def _diff(stream_ref: str, stream_gen: str):
      """
      Compare two stream files.

      Args:
         stream_ref (str): Path to a reference stream file
         stream_gen (str): Path to a generated stream file
      """
      with open(stream_ref) as f_ref, open(stream_gen) as f_gen:
          result_diff = difflib.unified_diff(
              f_ref.readlines(),
              f_gen.readlines(),
              fromfile=stream_ref,
              tofile=stream_gen, n=2)
          print("".join(result_diff))

   def _find_stream_details(self) -> Tuple[Optional[int], Optional[int]]:
      """
      Find the stream ID and stream version from the parsed file.

      Returns:
         Tuple[Optional[int], Optional[int]]: A tuple containing the stream ID and version, or (None, None) if not found.
      """
      with open(self.file_to_parse, 'r') as file:
         content = file.read()

      # Regular expression to find numbers in curly braces - stream number and version
      numbers_in_curly_braces_pattern = r'\{(\d+)U,\s*(\d+)U\}'

      match = re.search(numbers_in_curly_braces_pattern, content)

      if match:
         stream_id = int(match.group(1))
         stream_version = int(match.group(2))
         return stream_id, stream_version
      else:
         return None, None

   def _find_reference_stream_file(self) -> Optional[str]:
      """
      Find the reference stream definition file.

      Returns:
         Optional[str]: Path to the reference stream definition file, or None if not found.
      """
      self.stream_number, self.stream_version = self._find_stream_details()
      formatted_stream_number = f"{self.stream_number:03d}"
      formatted_stream_version = f"{self.stream_version:03d}"
      stream_definition_filename = f"strdef_src035_str{formatted_stream_number}_ver{formatted_stream_version}.txt"

      for root, _, files in os.walk(self.stream_definitions_folder):
         if stream_definition_filename in files:
            return os.path.join(root, stream_definition_filename)
      return None

   @staticmethod
   def _split_padding_expression(expression: str) -> Tuple[Optional[str], Optional[int]]:
      """
      Split a padding expression into its string and number parts.

      Args:
         expression (str): The padding expression to split.

      Returns:
         Tuple[Optional[str], Optional[int]]: A tuple containing the string part and number part, or (None, None) if invalid.
      """
      match = re.match(r'([A-Za-z]+)(\d+)', expression)

      if match:
         string_part = match.group(1)
         number_part = int(match.group(2))
         return string_part, number_part
      else:
         return None, None

   @staticmethod
   def _compare_stream_definition_files(lhs: TextIO, rhs: TextIO) -> Tuple[bool, str]:
      """
      Compare two stream definition files.

      Args:
         lhs (TextIO): File object for the first stream definition file.
         rhs (TextIO): File object for the second stream definition file.

      Returns:
         Tuple[bool, str]: A tuple containing a boolean indicating if the files are identical and an error message if applicable.
      """
      lhs_lines = [line.strip() for line in lhs.readlines()]
      rhs_lines = [line.strip() for line in rhs.readlines()]

      lhs_line_counter = 0
      rhs_line_counter = 0
      while lhs_line_counter < len(lhs_lines) and rhs_line_counter < len(rhs_lines):
         if lhs_lines[lhs_line_counter] == rhs_lines[rhs_line_counter]:
            lhs_line_counter += 1
            rhs_line_counter += 1
            continue

         # check for padding pattern
         elif lhs_lines[lhs_line_counter].startswith("PADDING"):
            try:
               _, padding_num = StreamDefinitionComparator._split_padding_expression(lhs_lines[lhs_line_counter])

               # Check if the next three lines in the second file match the expected pattern
               if StreamDefinitionComparator._check_padding_pattern(rhs_lines, rhs_line_counter, padding_num):
                  lhs_line_counter += 1
                  rhs_line_counter += 3
                  continue
               else:
                  return False, ""
            except:
               return False, ""

         # check for REPEAT pattern
         elif lhs_line_counter + 2 < len(lhs_lines) and lhs_lines[lhs_line_counter].startswith("REPEAT"):
            try:
               _, repeat_num = StreamDefinitionComparator._split_padding_expression(lhs_lines[lhs_line_counter])

               if StreamDefinitionComparator._check_repeat_pattern(lhs_lines, lhs_line_counter, rhs_lines, rhs_line_counter, repeat_num):
                     lhs_line_counter += 3
                     rhs_line_counter += 1

               else:
                  return False, ""
            except:
               return False, ""

         else:
            return False, ""

      # check if both files were fullt processed
      f_both_files_fully_processed = lhs_line_counter == len(lhs_lines) and rhs_line_counter == len(rhs_lines)
      if not f_both_files_fully_processed:
         return False, "At least one of the streams was not fully processed"

      return True, ""
   
   @staticmethod
   def _check_padding_pattern(rhs_lines : str, rhs_line_counter : int, padding_num : int) -> bool:
      """
      Check if the given lines match the expected padding pattern.

      This method verifies if the lines starting from the given line counter
      follow the expected padding pattern: a REPEAT line, followed by a uint8_t
      padding line, and an END_REPEAT line.

      Args:
         rhs_lines (str): The lines of the right-hand side stream definition.
         rhs_line_counter (int): The current line counter in the right-hand side stream.
         padding_num (int): The expected number of padding bytes.

      Returns:
         bool: True if the pattern matches, False otherwise.
      """
      return rhs_line_counter + 2 < len(rhs_lines) and \
               rhs_lines[rhs_line_counter] == f"REPEAT{padding_num}" and \
               rhs_lines[rhs_line_counter + 1].startswith("uint8_t") and \
               rhs_lines[rhs_line_counter + 1].endswith("padding") and \
               rhs_lines[rhs_line_counter + 2] == "END_REPEAT"
   
   @staticmethod
   def _check_repeat_pattern(lhs_lines : str, lhs_line_counter : int, rhs_lines : str, rhs_line_counter : int, repeat_num : int) -> bool:
      """
      Check if the given lines match the expected repeat pattern.

      This method verifies if the lines in both left-hand side and right-hand side
      stream definitions match the expected repeat pattern. It checks for a REPEAT
      block in the left-hand side and a corresponding PADDING line in the right-hand side.

      Args:
         lhs_lines (str): The lines of the left-hand side stream definition.
         lhs_line_counter (int): The current line counter in the left-hand side stream.
         rhs_lines (str): The lines of the right-hand side stream definition.
         rhs_line_counter (int): The current line counter in the right-hand side stream.
         repeat_num (int): The expected number of repeated elements.

      Returns:
         bool: True if the pattern matches in both streams, False otherwise.
      """
      return lhs_lines[lhs_line_counter + 1].startswith("uint8_t") and \
               lhs_lines[lhs_line_counter + 1].endswith("padding") and \
               lhs_lines[lhs_line_counter + 2] == "END_REPEAT" and \
               rhs_line_counter < len(rhs_lines) and \
               rhs_lines[rhs_line_counter] == f"PADDING{repeat_num}"

if __name__ == "__main__":
   parser = argparse.ArgumentParser()
   parser.add_argument("-sg", "--sg_repo_path", help="Path to the SG repo main folder", required=True)
   parser.add_argument("-f", "--header_file", help="Path to the header file to parse", required=True)
   parser.add_argument("-s", "--struct", help="Name of struct to be parsed", required=True)
   parser.add_argument("-o", "--output_file", default="strdef.txt", help="Name of struct to be parsed")
   parser.add_argument("-d", "--stream_definitions_folder", help="Path to folder containing stream definitions", required=True)

   args = parser.parse_args()

   comparator = StreamDefinitionComparator(args.sg_repo_path, args.header_file, args.struct, args.output_file, args.stream_definitions_folder)
   result = comparator.compare_streams()

   exit(result)
