"""This arxml parser is used to generate the wireshark config files."""
# !/usr/bin/python3

import argparse
import xml.etree.ElementTree as ET
import os
import datetime

ns = {"ns": "http://autosar.org/schema/r4.0"}
services = {}
interfaces = {}
structs = {}
structs_dict = {}
typerefs = {}
arrays = {}
lists = {}
enums = {}
unions = {}
strings = {}
basetypes_typedef = {}
basetypes2 = {}
methods = {}
eventgroups = {}
clients = {}

someip_ports = [30490, 30501]

DEBUG = False


class WiresharkParameterTypes:
    """Wireshark Parameter Types."""

    basetype = 1
    string = 2
    array = 3
    struct = 4
    union = 5
    typedef = 6
    enum = 7


class Arxml_Parser:
    """Read in the given arxml file and create create a html summary and config files for Wiresharks."""

    def __init__(self, arxmlFile):
        """
        Read arxml and output html and Wireshark config files.
        """
        self.arxmlFile = arxmlFile
        if not os.path.exists(self.arxmlFile):
            print("ERROR - Arxml does not exist:\n    {0}".format(self.arxmlFile))
            exit(1)

        tree = ET.parse(self.arxmlFile)
        self.root = tree.getroot()

        self.__parse_enums()
        self.__parse_services()
        self.__parse_interfaces()
        self.__parse_methods()
        # TODO: self.__parse_eventgroups()

        # Parse Ethernet clusters and connectors
        self.ethernet_clusters = self.__parse_ethernet_clusters()

        # Parse Signal Triggerings
        # signal_triggerings = self.__parse_signal_triggerings()

        # Connect services to interfaces
        # connected_services = connect_services_to_interfaces(ethernet_clusters, signal_triggerings)
        self.connected_services = self.__connect_services_to_interfaces()

        for service_id, _service_data in services.items():
            self.__populate_parameter_list((int(service_id)))

    def __extract_parameter_list(self, struct_name, prefix=""):
        result = []

        if DEBUG:
            print(f"Inspecting struct: {struct_name}")
            if struct_name in structs:
                print(f"Members: {structs[struct_name]}")

        if struct_name in basetypes_typedef:
            basetype_info = basetypes_typedef[struct_name]
            param_type, id_ref = self.__getParam_IDType(basetype_info[1])

            if DEBUG:
                print(f"struct_name={struct_name} - param_type={param_type} - id_ref={id_ref}")

            result.append((struct_name, basetype_info[1], id_ref))  # basetype_info[0]))
            return result

        if struct_name not in structs:
            if DEBUG:
                print(f"struct_name not in struct : {struct_name}")

            if struct_name in typerefs:

                param_type, _id_ref = self.__getParam_IDType(typerefs[struct_name][1])

                if param_type == 4:  # Struct
                    idCntr = 1
                    # lengthOfLengthField = 0
                    # padTo = 0
                    # WTLVExt = "FALSE"

                    for struct_name, struct_info in structs.items():
                        cntr = 0
                        for struct_val_name, _struct_type in struct_info[1:]:
                            param_type, id_ref = self.__getParam_IDType(struct_val_name)
                            if DEBUG:
                                print(f"{struct_val_name} - {_struct_type}")
                            if DEBUG:
                                print(f"idCntr:{idCntr} - id_ref:{id_ref}")
                            member_name = "test"
                            if idCntr == _id_ref:
                                if DEBUG:
                                    print(
                                        f"struct_name = {struct_name}: struct_val_name = {struct_val_name}"
                                    )

                                s = self.__extract_parameter_list(
                                    struct_val_name,
                                    f"{struct_name}.{struct_val_name}" if prefix else member_name,
                                )

                                result.extend(s)
                            cntr += 1
                        idCntr += 1

            return result

        for member in structs[struct_name][1:]:
            member_name, type_or_kind = member

            # Handle based on type
            if type_or_kind == "VALUE":
                if member_name in basetypes_typedef:
                    base_type = basetypes_typedef[member_name][1]
                    param_type, id_ref = self.__getParam_IDType(base_type)
                    result.append((member_name, base_type, id_ref))
                else:
                    result.append((member_name, "uint8", "3"))  # fallback

            # elif type_or_kind == "TYPE_REFERENCE":
            #     # Recurse into nested struct
            #     print(f"Recursing into REFERENCE: {member_name}")
            #     # nested_type = typerefs.get(member_name, member_name)
            #     # print(f"member_name : {member_name}")
            #     # print(f"nested_type : {nested_type}")
            #     result.extend(
            #         self.__extract_parameter_list(
            #             member_name, f"{prefix}.{member_name}" if prefix else member_name
            #         )
            #     )

            elif type_or_kind in ["TYPE_REFERENCE", "STRUCTURE"]:
                # Resolve actual struct name from typerefs if needed
                resolved_name = member_name
                if member_name in typerefs:
                    resolved_name = typerefs[member_name][1].split("/")[-1]

                if resolved_name in structs:
                    result.extend(
                        self.__extract_parameter_list(
                            resolved_name, f"{prefix}.{member_name}" if prefix else member_name
                        )
                    )
                else:
                    print(f"Struct not found for: {resolved_name}")

            elif type_or_kind == "ARRAY":
                if DEBUG:
                    print(f"Array: {member_name}")
                # Get array metadata
                if member_name in arrays:
                    array_info = arrays[member_name]
                    array_size = array_info[0]
                    array_type = array_info[3]
                    element_type = array_info[2]

                    # Add array container itself
                    result.append((member_name, "array", str(array_size)))

                    # If array elements are structs or typedefs, recurse
                    if array_type in ["STRUCTURE", "TYPE_REFERENCE"]:
                        nested = self.__extract_parameter_list(
                            element_type, f"{prefix}.{member_name}" if prefix else member_name
                        )
                        result.extend(nested)
                else:
                    result.append((member_name, "array", "3"))  # fallback

            else:
                result.append((member_name, "UNKNOWN", "3"))
        return result

    def __populate_parameter_list(self, service_filter=None):
        # param_index = 0
        message_type = 2
        version = 1
        wtlv = "FALSE"

        for service_id, service_info in self.connected_services.items():
            if DEBUG:
                print("-------------------")
                print(f"service_id   : {service_id}")
                print(f"service_info : {service_info}")

            if service_filter and int(service_id, 10) != service_filter:
                continue

            # service_data = service_info["service_data"]
            _interfaces = service_info["interfaces"]

            for method_id in methods.get(service_id, []):
                for interface in interfaces:
                    if interface not in interfaces:
                        continue
                    if interface in _interfaces and interfaces[interface]:
                        struct_tuple = interfaces[interface][0]
                        if len(struct_tuple) > 1:
                            tref_path = struct_tuple[1]
                            struct_name = (
                                tref_path.rpartition("/")[2] if tref_path else struct_tuple[0]
                            )

                            params = self.__extract_parameter_list(struct_name)
                            lists_key = (service_id, method_id)
                            if lists_key not in lists:
                                lists[lists_key] = []

                            for idx, (param_name, _param_type, id_ref) in enumerate(params):
                                lists[lists_key].append(
                                    (
                                        version,
                                        message_type,
                                        wtlv,
                                        len(params),
                                        idx,
                                        param_name,
                                        1,
                                        id_ref,
                                        param_name,
                                    )
                                )
                        else:
                            continue  # Skip if struct name not available
                    else:
                        continue  # Skip if interface not in dictionary or has no data
                    # struct_name = interfaces[interface][0][1]
                    # print(struct_name)

    def get_all_struct_elements(self):
        """Return list of elements in all the structs."""
        elements = []
        for struct_info in structs.values():
            for struct_val_name, val_type in struct_info[1:]:
                if val_type == "VALUE":
                    elements.append(struct_val_name)
                elif val_type == "ARRAY" and arrays[struct_val_name][2] in basetypes_typedef:
                    elements.append(struct_val_name)
        return elements

    def save_html_report(self, htmlFile):
        """Save the HTML content to a file."""
        html_content = self.__generate_html_content(self.connected_services, interfaces)
        with open(htmlFile, "w") as file:
            file.write(html_content)

    def save_configs(self, configs):
        """Save the config files."""
        self.__generate_SOMEIP_Services_config(self.connected_services, configs[0])  # DONE
        self.__generate_SOMEIP_Methods_config(methods, self.connected_services, configs[1])
        self.__generate_SOMEIP_Eventgroups_config(eventgroups, configs[2])
        self.__generate_SOMEIP_Clients_config(clients, configs[3])

        # -------------
        # generate_SOMEIP_ParameterBaseTypeList_config(basetypes_typedef)   # DONE
        self.__generate_SOMEIP_ParameterBaseTypeList2_config(basetypes2, configs[4])
        self.__generate_SOMEIP_ParameterLists_config(lists, configs[5])
        self.__generate_SOMEIP_ParameterArrays_config(arrays, configs[6])
        self.__generate_SOMEIP_ParameterStructs_config(structs, configs[7])
        self.__generate_SOMEIP_ParameterTypdefList_config(typerefs, basetypes_typedef, configs[8])
        self.__generate_SOMEIP_ParameterString_config(strings, configs[9])
        self.__generate_SOMEIP_ParameterUnions_config(unions, configs[10])
        self.__generate_SOMEIP_ParameterEnums_config(enums, configs[11])

    def save_lua(self, lua_file, version_file, variant, udp_ports=someip_ports):
        """Save a lua script to help dissect SOME/IP messages in Wireshark."""
        type_map = {
            "uint8": ("uint8", 1),
            "uint16": ("uint16", 2),
            "uint32": ("uint32", 4),
            "uint64": ("uint64", 8),
            "float32": ("float", 4),
            "float": ("float", 4),
            "double": ("float", 8),
            "unsigned char": ("uint8", 1),
            "unsigned short": ("uint16", 2),
            "unsigned int": ("uint32", 4),
            "unsigned long": ("uint64", 8),
            "signed char": ("int8", 1),
            "signed short": ("int16", 2),
            "signed int": ("int32", 4),
            "int": ("int32", 4),
        }

        def resolve_type(param_name, base_id):
            cntr = 1
            for basetype_name, _basetype_info in basetypes2.items():
                if base_id == cntr:
                    if basetype_name in type_map:
                        return type_map[basetype_name]
                cntr = cntr + 1
            return ("uint8", 1)  # default fallback

        now = datetime.datetime.now()

        lua_lines = []
        lua_lines.append(
            "------------------------------------------------------------------------------"
        )
        lua_lines.append("-- Filename     : SOMEIP.lua")
        lua_lines.append("-- Date Created : {0}-{1}-{2}".format(now.month, now.day, now.year))
        lua_lines.append("-- Description  : ")
        lua_lines.append("--\n-- Wireshark dissector for Aptiv's SOME/IP protocol.\n--")
        lua_lines.append("-- VARIANT: {0}".format(variant))
        lua_lines.append("-- SW VERSION: v{0}".format(self.__getSWVersion(version_file)))
        lua_lines.append("-- Supported Services:")
        for service_id, service_data in services.items():
            lua_lines.append(f"--   0x{int(service_id):0x}({service_data[1]})")

        lua_lines.append(
            "------------------------------------------------------------------------------\n"
        )

        lua_lines.append("-- Auto-generated SOME/IP Lua Dissector")
        lua_lines.append(
            'local someip_proto = Proto("generated_someip", "Generated SOME/IP Protocol")'
        )
        lua_lines.append("local f = someip_proto.fields")

        # Header fields
        header_fields = [
            ("service_id", "uint16", "Service ID", "HEX"),
            ("method_id", "uint16", "Method ID", "HEX"),
            ("length", "uint32", "Length", "DEC"),
            ("client_id", "uint16", "Client ID", "HEX"),
            ("session_id", "uint16", "Session ID", "HEX"),
            ("interface_version", "uint8", "Interface Version", "DEC"),
            ("message_type", "uint8", "Message Type", "HEX"),
            ("return_code", "uint8", "Return Code", "HEX"),
        ]
        for field, lua_type, label, base in header_fields:
            lua_lines.append(
                f'f.{field} = ProtoField.{lua_type}("generated_someip.{field}", "{label}", base.{base})'
            )

        # Payload fields
        payload_fields = {}

        for (service_id, _method_id), params in lists.items():
            prefix = f"s{int(service_id):x}_"
            if DEBUG:
                print(f"DEBUG: {service_id} - {params}")
            for _, _, _, _, _, param_name, _, param_id, _ in params:
                if DEBUG:
                    print(f"DEBUG: {service_id} - {param_name}")
                lua_type, size = resolve_type(param_name, param_id)

                full_name = f"{prefix}{param_name}"
                payload_fields[full_name] = (lua_type, size)

        for name, (lua_type, _) in payload_fields.items():
            lua_lines.append(
                f'f.{name} = ProtoField.{lua_type}("generated_someip.{name}", "{name}", base.DEC)'
            )

        # Dissector function
        lua_lines.append("\nfunction someip_proto.dissector(buffer, pinfo, tree)")
        lua_lines.append("  if buffer:len() < 16 then return end\n")
        lua_lines.append('  pinfo.cols.protocol = "Aptiv SOME/IP"')

        lua_lines.append("  local service_id = buffer(0,2):uint()")
        lua_lines.append("  local method_id = buffer(2,2):uint()")
        lua_lines.append(
            '  pinfo.cols.info = string.format("Aptiv SOME/IP Service 0x%04X Method 0x%04X", service_id, method_id)'
        )
        lua_lines.append(
            '  local subtree = tree:add(someip_proto, buffer(), "SOME/IP Protocol Data")'
        )

        lua_lines.append("  subtree:add(f.service_id, buffer(0,2))")
        lua_lines.append("  subtree:add(f.method_id, buffer(2,2))")
        lua_lines.append("  subtree:add(f.length, buffer(4,4))")
        lua_lines.append("  subtree:add(f.client_id, buffer(8,2))")
        lua_lines.append("  subtree:add(f.session_id, buffer(10,2))")
        lua_lines.append("  subtree:add(f.interface_version, buffer(13,1))")
        lua_lines.append("  subtree:add(f.message_type, buffer(14,1))")
        lua_lines.append("  subtree:add(f.return_code, buffer(15,1))")
        lua_lines.append("  local offset = 16")
        lua_lines.append('  local payload_tree = subtree:add(buffer(offset), "Aptiv Payload")')

        for (service_id, _method_id), params in lists.items():
            lua_lines.append(f"  if service_id == 0x{int(service_id):04X} then")
            for _1, _2, _3, _4, _5, param_name, _6, param_id, _ in params:

                lua_type, size = resolve_type(param_name, param_id)
                if DEBUG:
                    print(f"{param_name} - {param_id} - {lua_type} - {size}")
                if DEBUG:
                    print(f"{_1} - {_2} - {_3} - {_4} - {_5} - {_6}")
                try:
                    array_size, is_array = self._get_array_total_size(param_name)
                    if DEBUG:
                        print(f"{param_name} - {self._get_array_total_size(param_name)}")
                    if is_array:
                        size = array_size
                except Exception as e:
                    print(e.message)
                full_name = f"s{int(service_id):x}_{param_name}"
                lua_lines.append(f"    payload_tree:add(f.{full_name}, buffer(offset, {size}))")
                lua_lines.append(f"    offset = offset + {size}")
            lua_lines.append("  end")

        lua_lines.append("end")
        lua_lines.append('local udp_port = DissectorTable.get("udp.port")')

        lua_lines.append(
            "-- Register Aptiv UDP protocol to handle messages on expected UDP ports."
        )
        locaUdpPorts = "local udp_ports = {"
        first = True

        for port in udp_ports:
            if first:
                locaUdpPorts += '"{0}"'.format(port)
                first = False
            else:
                locaUdpPorts += ',"{0}"'.format(port)

        locaUdpPorts += "}\n"

        lua_lines.append(locaUdpPorts)

        lua_lines.append("for i,port in ipairs(udp_ports) do")
        lua_lines.append("\tudp_port:add(port, someip_proto)")
        lua_lines.append("end\n")

        with open(lua_file, "w") as f:
            f.write("\n".join(lua_lines))

    def _get_array_total_size(self, array_name):
        """Return the size of the given array."""
        is_array = False
        if DEBUG:
            print(f"DEBUG00: {array_name}")
        if array_name not in arrays:
            return 0, is_array
        if DEBUG:
            print(f"DEBUG0: {arrays}")
        is_array = True

        if len(arrays[array_name]) < 5:
            element_type = arrays[array_name][2]
            array_size = int(arrays[array_name][0])
        else:
            element_type = arrays[array_name][3]
            array_size = int(arrays[array_name][1])
        if DEBUG:
            print(f"DEBUG1: {array_size}")
        if DEBUG:
            print(f"DEBUG2: {element_type}")
        struct_info = self.__drill_down_structure(element_type)
        total_bits = 0

        for field, info in struct_info.items():

            if isinstance(info, dict) and "size" in info:

                type_str = info.get("type")

                if type_str.isdigit():
                    size = int(type_str)
                else:
                    size_str = info.get("size")
                    try:
                        size = int(size_str)
                    except (ValueError, TypeError):
                        print(f"Could not parse size from: {info}")
                        size = None

                # size = int(info["size"])

                # type_str = info["type"]
                # try:
                #     type_val = int(type_str)
                # except ValueError:
                #     print(f"Skipping non-integer type: {type_str}")
                #     type_val = None  # or handle it however you need

                array_count = int(info.get("array size", 1))
                total_bits += size * array_count
                if DEBUG:
                    print(f"Field = {field} - {size} - {array_count} - {total_bits}")

        total_bytes = (total_bits // 8) * array_size
        if DEBUG:
            print(f"DEBUG3: {total_bytes}")
        return total_bytes, is_array

    def __getSWVersion(self, streamFile):
        """Pull the sw version from the history file."""
        sw_version = ""

        if os.path.exists(streamFile):
            with open(streamFile) as file_:
                for line in file_:
                    if "#define MAJOR_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define MINOR_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define PATCH_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip() + "."
                    if "#define PRE_RELEASE_REVISION" in line:
                        versionLine = line.split("(uint8_t)")
                        sw_version += versionLine[1].replace(")", "").strip()
                        # print(sw_version)
                        break

        return (
            sw_version.replace(";", "")
            .replace("/", "")
            .replace("*", "")
            .replace("U", "")
            .replace(",", "")
        )

    def __get_child_attribute(self, element, childtag, attribkey):
        """Find the child attrubute of a given element."""
        if childtag is None or attribkey is None:
            return "First"

        c = element.find(childtag, ns)
        if c is None:
            # xml.etree.ElementTree.dump(element)
            return "Second"
        if attribkey in c.attrib:
            return c.attrib[attribkey]
        return "Third"

    def __get_child_attribute_text(self, element, childtag, attribkey):
        """Get the given element's child attribute text."""
        if childtag is None or attribkey is None:
            return "First"

        c = element.find(childtag, ns)
        if c is None:
            # xml.etree.ElementTree.dump(element)
            return "Second"
        if attribkey in c.attrib:
            return c.attrib[attribkey].text
        return "Third"

    def __get_ref(self, element, attribute_tag, attribute_value):
        """Get the reference of the given element."""
        c = element.find(attribute_tag, ns)
        if c is not None:
            if "DEST" in c.attrib:
                if c.attrib["DEST"] == attribute_value:
                    return c.text.rpartition("/")[2]
        return ""

    def __get_ref_path(self, element, attribute_tag, attribute_value):
        """Get the reference path of the given element (ignore DEST for v2.0)."""
        c = element.find(attribute_tag, ns)
        if c is not None and c.text:
            return c.text
        return ""

    def __get_name_imp_dtype_ref(self, element):
        """Get the name of the given element's implementation data type reference."""
        reference = element.find(".//ns:SW-DATA-DEF-PROPS-CONDITIONAL", ns)
        dtype_ref = self.__get_ref_path(
            reference, "ns:IMPLEMENTATION-DATA-TYPE-REF", "IMPLEMENTATION-DATA-TYPE"
        )
        return dtype_ref.rpartition("/")[2]

    def __resolve_ref_path(self, ref_path):
        """Resolve the given reference path."""
        if "/" not in ref_path:
            return None
        dtype_name = ref_path.rpartition("/")[2]
        element = self.root.find(f".//*[ns:SHORT-NAME={dtype_name!r}]", ns)
        return element

    def __parse_services(self):
        """Find all services and add them to the services dictionary."""
        # find the services
        for psis in self.root.findall(".//ns:PROVIDED-SERVICE-INSTANCE", ns):
            # switch = {}
            name = psis.find("ns:SHORT-NAME", ns).text.partition("_")[2]
            instance_id = psis.find("ns:INSTANCE-IDENTIFIER", ns).text
            service_id = psis.find("ns:SERVICE-IDENTIFIER", ns).text

            if service_id not in services:
                services[service_id] = (instance_id, name)
            # psis = soad.find('.//ns:APPLICATION-ENDPOINT/ns:PROVIDED-SERVICE-INSTANCES',ns)
            # if psis is not None:
            # print(psis)
            #   for psi in psis.find('.//ns:PROVIDED-SERVICE-INSTANCE',ns):
            #      print(psi.findall('.//ns:SHORT-NAME',ns)[0].text)

            # switch["id"] = sw.findall('.//ns:SHORT-NAME', ns)[0].text
            # conn_ref = get_child_attribute_text(sw, "ns:CONNECTOR-REF", "DEST")
            # cluster_ref_name = get_child_attribute_text(sw, "ns:COMMUNICATION-CLUSTER-REF", "DEST")
            # print(conn_ref)
            # print(cluster_ref_name)

    def __parse_interfaces(self):
        """Find all interfaces and add them to the interface dictionary."""
        # find parameters
        for sri in self.root.findall(".//ns:SENDER-RECEIVER-INTERFACE", ns):
            name = sri.find("ns:SHORT-NAME", ns).text
            variables = []
            for var in sri.findall(".//ns:VARIABLE-DATA-PROTOTYPE", ns):
                var_name = var.find("ns:SHORT-NAME", ns).text
                tref = var.find("ns:TYPE-TREF", ns)
                dtype_ref = tref.text if (tref is not None and tref.text) else ""
                variables.append((var_name, dtype_ref))
            interfaces[name] = variables
            # print("INTERFACE:", name)
        for par_list in interfaces.values():
            for var in par_list:
                tref_path = var[1]
                target = tref_path.rpartition("/")[2] if tref_path else var[0]
                self.__parse_datatype(target)
                tref_path = var[1]
                target = tref_path.rpartition("/")[2] if tref_path else var[0]
                self.__parse_datatype(target)

    def __parse_eventgroups(self):
        """Find all event groups and add them to the eventGroups dictionary."""
        print("Not Finished")
        # for csi in self.root.findall(".//ns:CONSUMED-SERVICE-INSTANCE", ns):
        # service_path = self.__get_ref_path(
        #     csi, "ns:PROVIDED-SERVICE-INSTANCE-REF", "PROVIDED-SERVICE-INSTANCE"
        # )
        # service_element = self.__resolve_ref_path(service_path)
        # service_id = service_element.find("ns:SERVICE-IDENTIFIER", ns).text
        # print(service_id)
        # print(service_path)
        # print(service_element.find(".//ns:SERVICE-IDENTIFIER").text)
        # for ceg in csi.findall(".//ns:CONSUMED-EVENT-GROUP", ns):
        #    print("EVENT NAME:"+ceg.find("ns:SHORT-NAME", ns).text)
        #    print("EVENT ID:  "+ceg.find("ns:EVENT-GROUP-IDENTIFIER", ns).text)

    def __parse_signal_triggerings(self):
        """Find all triggers and add them to the signal_triggerings dictionary."""
        signal_triggerings = {}
        for triggering in self.root.findall(".//ns:I-SIGNAL-TRIGGERING", ns):
            triggering_name = triggering.find("ns:SHORT-NAME", ns).text
            port_refs = []
            for port_ref in triggering.findall(".//ns:I-SIGNAL-PORT-REF", ns):
                port_refs.append(port_ref.text)
            signal_triggerings[triggering_name] = port_refs
        return signal_triggerings

    def __parse_ethernet_clusters(self):
        """Search for all ethernet clusters and populate the ethernet_clusters dictionary."""
        ethernet_clusters = {}
        for cluster in self.root.findall(".//ns:ETHERNET-CLUSTER", ns):
            cluster_name = cluster.find("ns:SHORT-NAME", ns).text
            connectors = []
            for connector_ref in cluster.findall(".//ns:COMMUNICATION-CONNECTOR-REF", ns):
                connectors.append(connector_ref.text)
            ethernet_clusters[cluster_name] = connectors
        return ethernet_clusters

    def __parse_methods(self):
        """Search for all methods and populate the methods dictionary."""
        for scipdu in self.root.findall(".//ns:SOCKET-CONNECTION-IPDU-IDENTIFIER", ns):
            header = scipdu.find("ns:HEADER-ID", ns).text
            value = int(header)
            h_service = hex(value)[2:-4]
            d_service = str(int(h_service, 16))
            method = hex(value)[-4:]

            if d_service != "65535":  # TODO check this....
                if d_service not in methods:
                    s_methods = []
                    s_methods.append(method)
                    methods[d_service] = s_methods

                else:
                    methods.update({d_service: method})

                # print('\nHEADER:'+header+' hex_service:'+h_service+' hex_event:'+method+' dec service:'+d_service)

    def __parse_enums(self):
        enum_index_cntr = 1
        # Iterate through each COMPU-METHOD
        for compu_method in self.root.findall(".//ns:COMPU-METHOD", ns):
            category = compu_method.find("ns:CATEGORY", ns).text
            if category == "TEXTTABLE":
                enum_name = compu_method.find("ns:SHORT-NAME", ns).text
                compu_scales = compu_method.findall(".//ns:COMPU-SCALES/ns:COMPU-SCALE", ns)

                enum_values = []

                for scale in compu_scales:
                    lower_limit = scale.find("ns:LOWER-LIMIT", ns).text
                    vt = scale.find(".//ns:COMPU-CONST/ns:VT", ns).text
                    enum_values.append({"ID": enum_index_cntr, "value": lower_limit, "name": vt})
                enum_index_cntr += 1
                enums[enum_name] = enum_values

    def __add_basetype(self, name, element):
        """Search for all base types and populate the basetypes_typedef dictionary."""
        if name not in basetypes_typedef:
            reference = element.find(".//ns:SW-DATA-DEF-PROPS-CONDITIONAL", ns)
            dtype_ref = self.__get_ref_path(reference, "ns:BASE-TYPE-REF", "SW-BASE-TYPE")
            compu_ref = self.__get_ref_path(reference, "ns:COMPU-METHOD-REF", "COMPU-METHOD")

            n_element = self.__resolve_ref_path(dtype_ref)
            n_cat = n_element.find("./ns:CATEGORY", ns).text

            n_bitsize = n_element.find("./ns:BASE-TYPE-SIZE", ns).text
            n_native = n_element.find("./ns:NATIVE-DECLARATION", ns).text
            n_encode = n_element.find("./ns:BASE-TYPE-ENCODING", ns).text

            basetypes_typedef[name] = (
                n_bitsize,
                n_native,
                n_encode,
                n_cat,
            )  # , len(basetypes_typedef))
            if n_native not in basetypes2:
                basetypes2[n_native] = (len(basetypes2) + 1, n_bitsize)

            comp_element = self.__resolve_ref_path(compu_ref)
            if comp_element is not None:
                c_sname = comp_element.find("./ns:SHORT-NAME", ns).text

                ptype, idref = self.__getParam_IDType(name)
                # print(f"{n_native}:{ptype}:{idref}:{name}")
                if c_sname in enums:
                    enums[c_sname] = [
                        {**item, "Parameter Type": ptype, "ID Reference": idref, "Ref Name": name}
                        for index, item in enumerate(enums[c_sname])
                    ]

    def __add_array(self, name, element):
        """Search for all arrays and populate the arrays dictionary."""
        if name not in arrays:
            sub = element.find("./ns:SUB-ELEMENTS", ns)
            array_meta = sub.find("./ns:IMPLEMENTATION-DATA-TYPE-ELEMENT", ns)
            array_size = array_meta.find("./ns:ARRAY-SIZE", ns).text
            array_type = array_meta.find("./ns:ARRAY-SIZE-SEMANTICS", ns).text
            dtype_cat = array_meta.find("./ns:CATEGORY", ns).text
            dtype_ind = array_meta.find("./ns:SHORT-NAME", ns).text
            dtype_name = self.__get_name_imp_dtype_ref(array_meta)
            self.__add_datatype(dtype_ind, dtype_cat, array_meta)

            arrays[name] = (array_size, array_type, dtype_name, dtype_cat)

    def __add_typeref(self, name, element):
        """Search for all typerefs and populate the typerefs dictionary."""
        if name not in typerefs:
            reference = element.find(".//ns:SW-DATA-DEF-PROPS-CONDITIONAL", ns)
            # compu_ref = self.__get_ref_path(reference, "ns:COMPU-METHOD-REF", "COMPU-METHOD")

            dtype_ref = self.__get_ref_path(
                reference, "ns:IMPLEMENTATION-DATA-TYPE-REF", "IMPLEMENTATION-DATA-TYPE"
            )
            # print(dtype_ref)
            n_element = self.__resolve_ref_path(dtype_ref)
            category = n_element.find("./ns:CATEGORY", ns).text
            n_name = n_element.find("./ns:SHORT-NAME", ns).text

            if name != "Dimension":
                typerefs[name] = (len(typerefs) + 1, dtype_ref, category, n_element)
            self.__add_datatype(n_name, category, n_element)

    def __add_struct(self, name, element):
        """Add the given name to the struct dictionary."""
        if name not in structs:
            subelements = [len(structs) + 1]
            # print(element)
            sub = element.find("./ns:SUB-ELEMENTS", ns)
            # print(sub)
            for dtype_subelement in sub.findall("./ns:IMPLEMENTATION-DATA-TYPE-ELEMENT", ns):
                name_sub = dtype_subelement.find("ns:SHORT-NAME", ns).text
                cat_sub = dtype_subelement.find("ns:CATEGORY", ns).text
                subelements.append((name_sub, cat_sub))
                self.__add_datatype(name_sub, cat_sub, dtype_subelement)
            structs[name] = subelements

    def __add_datatype(self, name, category, element):
        """Add the datatype to a specific catetory."""
        if category == "STRUCTURE":
            self.__add_struct(name, element)
        elif category == "TYPE_REFERENCE":
            self.__add_typeref(name, element)
        elif category == "ARRAY":
            self.__add_array(name, element)
        elif category == "VALUE":
            self.__add_basetype(name, element)
        # VALUE?

    def __find_related_interfaces(self, service_id, service_data):
        """Find the related interface for the given service id."""
        related_interfaces = []

        # Example logic to find related interfaces
        for interface_name, interface_details in interfaces.items():
            # print(f"Interface Name: {interface_name}")
            # print(f"Interface Details: {interface_details}")
            if self.__service_related_to_interface(
                service_id, service_data, interface_name, interface_details
            ):
                related_interfaces.append(interface_name)

        return related_interfaces

    def __find_related_interfaces_v2(self, service_id, service_data):
        """Find the related interface for the given service id."""
        related_interfaces = []

        # Check if the service is linked to any specific Ethernet cluster
        for cluster_name, connectors in self.ethernet_clusters.items():
            if self.__service_related_to_cluster(service_id, service_data, cluster_name):
                for connector in connectors:
                    # Assuming connectors are related to interfaces
                    interfaces = self.__find_interfaces_related_to_connector(connector)
                    related_interfaces.extend(interfaces)

        return related_interfaces

    def __service_related_to_cluster(self, service_id, service_data, cluster_name):
        """Define logic to determine if a service is related to an Ethernet cluster."""
        pass

    def __find_interfaces_related_to_connector(self, connector):
        """Define logic to find interfaces related to a given connector."""
        # This might involve parsing other parts of the ARXML or certain naming conventions
        return []  # /* list of interfaces */]

    def __service_related_to_interface(
        self, service_id, service_data, interface_name, interface_details
    ):
        """Hardcoded lookup table for the service to interface link."""
        # Define the criteria for linking a service to an interface
        lookupTable = {
            "RDR_1_Status": "Interface_RDR_Status_Event",
            "RDR_1_SW_Info": "Interface_RDR_SW_Info_Event",
            "ADAS_Vehicle_Status": "Interface_Vechicle_Status_Event",
            "RDR_1_Detections": "Interface_DetectionList_Event",
        }

        # if service_data[1] in interface_name:  # Example condition

        if service_data[1] in lookupTable:
            if lookupTable[service_data[1]] == interface_name:  # Example condition
                return True
        return False

    def __get_last_segment_of_path(self, type_key):
        """Parse out the last segment of a path of a type."""
        if type_key in typerefs:
            path = typerefs[type_key][0]  # Get the full path
            # Split the path by '/' and return the last element
            return path.split("/")[-1]
        else:
            return "Type key not found"

    def __connect_services_to_interfaces(self):
        """Find the interfaces that are connected to the services."""
        connected_services = {}

        for service_id, service_data in services.items():
            # print("--------")
            # print(f"Service ID: {service_id}")
            # print(f"Service Data: {service_data}")
            related_interfaces = self.__find_related_interfaces(service_id, service_data)
            connected_services[service_id] = {
                "service_data": service_data,
                "interfaces": related_interfaces,
            }

        # print(connected_services)
        return connected_services

    def __parse_datatype(self, name):
        """Parse down to the data type."""
        for dtype in self.root.findall(".//ns:IMPLEMENTATION-DATA-TYPE", ns):
            if name == dtype.find("ns:SHORT-NAME", ns).text:
                category = dtype.find("ns:CATEGORY", ns).text
                # print("FOUND", "category:", category)
                # use if-elif-else or 3.10 pattern matching?
                self.__add_datatype(name, category, dtype)

    def __add_app_record(self, name, element):
        """v2.0: Map APPLICATION-RECORD-DATA-TYPE to our structs dict."""
        if name in structs:
            return
        subelements = [len(structs) + 1]
        for rec_el in element.findall("./ns:ELEMENTS/ns:APPLICATION-RECORD-ELEMENT", ns):
            el_name = rec_el.find("ns:SHORT-NAME", ns).text
            el_cat_el = rec_el.find("ns:CATEGORY", ns)
            el_cat = el_cat_el.text if el_cat_el is not None else "VALUE"
            if el_cat == "BOOLEAN":
                el_cat = "VALUE"
            subelements.append((el_name, el_cat))
            # Recurse into referenced types (TYPE-TREF), if present
            el_tref = rec_el.find("ns:TYPE-TREF", ns)
            if el_tref is not None and el_tref.text:
                target = el_tref.text.rpartition("/")[2]
                self.__parse_datatype(target)
        structs[name] = subelements

    def __add_app_array(self, name, element):
        """v2.0: Map APPLICATION-ARRAY-DATA-TYPE to our arrays dict."""
        if name in arrays:
            return
        el = element.find("./ns:ELEMENT", ns)
        if el is None:
            return
        el_cat_el = el.find("ns:CATEGORY", ns)
        el_cat = el_cat_el.text if el_cat_el is not None else "VALUE"
        size_sem_el = el.find("ns:ARRAY-SIZE-SEMANTICS", ns)
        max_num_el = el.find("ns:MAX-NUMBER-OF-ELEMENTS", ns)
        tref = el.find("ns:TYPE-TREF", ns)
        dtype_name = tref.text.rpartition("/")[2] if (tref is not None and tref.text) else None
        if dtype_name:
            self.__parse_datatype(dtype_name)
        arrays[name] = (
            max_num_el.text if max_num_el is not None else "0",
            size_sem_el.text if size_sem_el is not None else "FIXED-SIZE",
            dtype_name,
            el_cat,
        )

    def __add_app_primitive(self, name, element):
        """v2.0: Map APPLICATION-PRIMITIVE-DATA-TYPE to basetypes_typedef/basetypes2."""
        if name in basetypes_typedef:
            return
        # Try to infer width from COMPU-METHOD
        bits = None
        cm_ref = element.find(".//ns:SW-DATA-DEF-PROPS-CONDITIONAL/ns:COMPU-METHOD-REF", ns)
        if cm_ref is not None and cm_ref.text:
            cm_name = cm_ref.text.rpartition("/")[2]
            cm = self.root.find(f".//ns:COMPU-METHOD[ns:SHORT-NAME='{cm_name}']", ns)
            if cm is not None:
                scale = cm.find(".//ns:COMPU-SCALE", ns)
                if scale is not None:
                    up = scale.find("ns:UPPER-LIMIT", ns)
                    try:
                        if up is not None and up.text is not None:
                            upper = int(float(up.text))
                            if upper <= 0xFF:
                                bits = 8
                            elif upper <= 0xFFFF:
                                bits = 16
                            elif upper <= 0xFFFFFFFF:
                                bits = 32
                            else:
                                bits = 64
                    except Exception as e:
                        print(f"An unexpected program error occurred: {e}")
                        pass
        if bits is None:
            bits = 32
        native = {8: "uint8", 16: "uint16", 32: "uint32", 64: "uint64"}[bits]
        basetypes_typedef[name] = (str(bits), native, "BIGENDIAN", "VALUE")
        if native not in basetypes2:
            basetypes2[native] = (len(basetypes2) + 1, str(bits))

    def __drill_down_structure(self, struct_name):
        """Drill down to the base level of each value from a struct."""
        # Normalize to SHORT-NAME if a full path was passed
        if struct_name and "/" in str(struct_name):
            struct_name = str(struct_name).split("/")[-1]

        # Check if the structure is in the base types first
        if struct_name in basetypes_typedef:
            base_type_info = basetypes_typedef[struct_name]
            return {"type": base_type_info[1], "size": base_type_info[0]}

        # Guard if missing
        if struct_name not in structs:
            return {}

        structure = structs[struct_name][1:]
        result = {}

        for property_name, property_type in structure:
            if property_type == "VALUE":
                if property_name in basetypes_typedef:
                    base_type_info = basetypes_typedef[property_name]
                    result[property_name] = {"type": base_type_info[1], "size": base_type_info[0]}
                else:
                    result[property_name] = {"type": "-", "size": "-"}
            elif property_type == "TYPE_REFERENCE":
                ref_struct_name = (
                    typerefs[property_name][1].split("/")[-1]
                    if property_name in typerefs
                    else property_name
                )
                result[property_name] = self.__drill_down_structure(ref_struct_name)
            elif property_type == "ARRAY":
                array_info = arrays.get(property_name)
                if array_info:
                    array_size = array_info[0]
                    elem_type = array_info[2]
                    elem_kind = array_info[3] if len(array_info) > 3 else "VALUE"
                    if elem_type and "/" in str(elem_type):
                        elem_type = str(elem_type).split("/")[-1]
                    if elem_kind in ["STRUCTURE", "TYPE_REFERENCE", "STRUCTURE_REFERENCE"]:
                        result[property_name] = {
                            "type": self.__drill_down_structure(elem_type),
                            "array size": array_size,
                        }
                    else:
                        if elem_type in basetypes_typedef:
                            base_info = basetypes_typedef[elem_type]
                            result[property_name] = {
                                "type": base_info[1],
                                "size": base_info[0],
                                "array size": array_size,
                            }
                        else:
                            result[property_name] = {"type": elem_type, "array size": array_size}
                else:
                    result[property_name] = {"type": "array", "array size": "-"}
            else:
                # Treat as nested struct by name
                result[property_name] = self.__drill_down_structure(property_name)
        return result

    def __getParam_IDType(self, val, debug=0):
        """Get the parameter and ID type for each value."""
        paramType = "?"
        idRef = "?"

        # Strip out any /blabla/bla/strucName
        val = [part for part in val.split("/") if part][-1] if val else None

        if val in structs:
            paramType = WiresharkParameterTypes.struct
            idRef = structs[val][0]

        elif val in arrays:
            paramType = WiresharkParameterTypes.array
            idRef = arrays[val][0]
        elif val in basetypes2:
            paramType = WiresharkParameterTypes.basetype
            idRef = basetypes2[val][0]
        elif val in typerefs:
            paramType = WiresharkParameterTypes.typedef
            idRef = typerefs[val][0]
            if debug:
                # print(typerefs)
                print(typerefs)
                print(f"1: {val}:{paramType}:{idRef}")
        elif val in basetypes_typedef:
            paramType = WiresharkParameterTypes.typedef
            idRef = basetypes_typedef[val][0]  # + len(typerefs)
            if debug:
                print("------------------------------------------")
                print(basetypes_typedef)
                print(f"2: {val}:{paramType}:{idRef}")
                exit()

        # print(f"{val}:{paramType}:{idRef}")
        return paramType, idRef

    def __generate_SOMEIP_Services_config(self, connected_services, config_file):
        """Generate a Wireshark config file for Service."""
        # config_file = "SOMEIP_service_identifiers"
        with open(config_file, "w") as file:
            file.write("# This Services config file is automatically generated, DO NOT MODIFY\n")
            for service_id, info in connected_services.items():
                service_version, service_name = info["service_data"]
                file.write(f'"{int(service_id):x}","{service_name}"\n')

    def __generate_SOMEIP_Methods_config(self, methods, connected_services, config_file):
        """Generate a Wireshark config file for Methods."""
        # config_file = "temp/SOMEIP_method_event_identifiers.txt"
        with open(config_file, "w") as file:
            file.write("# This Methods config file is automatically generated, DO NOT MODIFY\n")
            file.write("# Service ID | Methods ID | Method Name\n")

            for service_id, method_id in methods.items():
                method_name = connected_services[service_id]["interfaces"][0]
                file.write(f'"{int(service_id):x}","{method_id[0]}","{method_name}"\n')

    def __generate_SOMEIP_Eventgroups_config(self, eventgroups, config_file):
        """Generate a Wireshark config file for Event Groups."""
        # config_file = "SOMEIP_eventgroup_identifiers"
        with open(config_file, "w") as file:
            file.write(
                "# This EventGroups config file is automatically generated, DO NOT MODIFY\n"
            )
            file.write("# Service ID | Eventgroup ID | Eventgroup Name\n")
            for method_id, method_name in eventgroups.items():
                file.write(f'"{int(method_id):x}","{method_name}"\n')

    def __generate_SOMEIP_Clients_config(self, clients, config_file):
        """Generate a Wireshark config file for Clients."""
        # config_file = "SOMEIP_client_identifiers"
        with open(config_file, "w") as file:
            file.write("# This Client config file is automatically generated, DO NOT MODIFY\n")
            file.write("# Service ID | Client ID | Client Name\n")
            # for method_id, method_name in methods.items():
            #   file.write(f'"{int(method_id):x}","{method_name}"\n')

    def __generate_SOMEIP_ParameterLists_config(self, lists, config_file):
        """Generate a Wireshark config file for Parameter Lists."""
        # config_file = "SOMEIP_parameter_list"
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Lists config file is automatically generated, DO NOT MODIFY\n"
            )
            file.write(
                "# Service ID | Method ID | Version | Message Type | WTLV Extension? | # of Parameters | Parameter Position/ID | Parameter Name | Parameter Type | ID Reference | Filter String\n"
            )
            for list_id, lists_info in lists.items():
                for row in lists_info:
                    file.write(
                        f'"{int(list_id[0]):x}","{list_id[1]}","{row[0]}","{row[1]}", "{row[2]}", "{row[3]}", "{row[4]}", "{row[5]}", "{row[6]}", "{row[7]}", "{row[8]}"\n'
                    )

    def __generate_SOMEIP_ParameterArrays_config(self, arrays, config_file):
        """Generate a Wireshark config file for Parameter Arrays."""
        # config_file = "SOMEIP_parameter_arrays"
        idCntr = 1
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Arrays config file is automatically generated, DO NOT MODIFY\n"
            )
            file.write(
                "# ID | Array Name | Parameter Type | ID Ref | # of Items | Filter String | Dimension | Lower Limit | Upper Limit | Length of Length Field | Pad to\n"
            )
            for array_name, array_info in arrays.items():
                param_type, id_ref = self.__getParam_IDType(array_info[2])

                num_items = 1
                filterString = "array"
                dimension = 0
                lowerLimit = array_info[0]
                upperLimit = array_info[0]
                lengthField = 0
                padTo = 0

                file.write(
                    f'"{idCntr}","{array_name}","{param_type}","{id_ref}","{num_items}","{filterString}","{dimension}","{lowerLimit}","{upperLimit}","{lengthField}","{padTo}"\n'
                )
                newTuple = (idCntr,) + array_info
                arrays[array_name] = newTuple
                idCntr += 1

    def __generate_SOMEIP_ParameterStructs_config(self, structs, config_file):
        """Generate a Wireshark config file for Parameter Structs."""
        # config_file = "7_SOMEIP_parameter_structs_DONE"

        idCntr = 1
        lengthOfLengthField = 0
        padTo = 0
        WTLVExt = "FALSE"

        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Structs config file is automatically generated, DO NOT MODIFY\n"
                "# ID | Struct Name | Length Field | Pad to | WTLV Ext? | # of items | Parameter ID | Parameter Name | Parameter Type | ID ref | Filter String\n"
            )

            for struct_name, struct_info in structs.items():
                cntr = 0
                for struct_val_name, _struct_type in struct_info[1:]:
                    param_type, id_ref = self.__getParam_IDType(struct_val_name)
                    file.write(
                        f'"{idCntr}","{struct_name}","{lengthOfLengthField}","{padTo}","{WTLVExt}","{len(struct_info)}","{cntr}","{struct_val_name}","{param_type}","{id_ref}","{struct_val_name}"\n'
                    )
                    cntr += 1
                idCntr += 1

    def __generate_SOMEIP_ParameterUnions_config(self, unions, config_file):
        """Generate a Wireshark config file for Parameter Unions."""
        # config_file = "8_SOMEIP_parameter_unions"
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Unions config file is automatically generated, DO NOT MODIFY\n"
            )

    def __generate_SOMEIP_ParameterEnums_config(self, enums, config_file):
        """Generate a Wireshark config file for Parameter Enums."""
        # config_file = "9_SOMEIP_parameter_enums"
        idCntr = 1
        # print(enums)
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Enums config file is automatically generated, DO NOT MODIFY\n"
                "# ID | Name | Parameter Type | ID Reference | Number of Items | Value | Value Name\n"
            )

            for enum_name, enum_info in enums.items():
                for enum_value in enum_info:
                    if "Parameter Type" in enum_value:
                        param_type, id_ref = self.__getParam_IDType(
                            enum_value["Ref Name"]
                        )  # , debug=1)
                        file.write(
                            f'"{enum_value["ID"]}","{enum_name}","{param_type}", "{id_ref}", "{len(enum_info)}","{enum_value["value"]}", "{enum_value["name"]}"\n'
                        )
                    else:
                        file.write(
                            f'"{enum_value["ID"]}","{enum_name}","?", "?", "{len(enum_info)}","{enum_value["value"]}", "{enum_value["name"]}"\n'
                        )

                idCntr += 1
                # print(enum_name)
                # print(enum_info)
                # newTuple = (idCntr,) + enum_info
                # enums[enum_name] = newTuple

    def __generate_SOMEIP_ParameterBaseTypeList_config(self, basetypes, config_file):
        """Generate a Wireshark config file for Parameter Base Type List."""
        # config_file = "10_SOMEIP_parameter_base_types_DONE"
        # ID | Name | Data Type | Big Engian | Bitlength base type | Bitlength enc. type
        idCntr = 1
        bigEndian = "TRUE"
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Base Type List config file is automatically generated, DO NOT MODIFY\n"
            )
            for basetype_name, basetype_info in basetypes.items():
                file.write(
                    f'"{idCntr}","{basetype_name}","{basetype_info[1]}","{bigEndian}","{basetype_info[0]}","{basetype_info[0]}"\n'
                )
                newTuple = (idCntr,) + basetype_info
                basetypes[basetype_name] = newTuple
                idCntr += 1

    def __generate_SOMEIP_ParameterBaseTypeList2_config(self, basetypes2, config_file):
        """Generate a Wireshark config file for Parameter Base Types."""
        # config_file = "10_SOMEIP_parameter_base_types_DONE"
        # ID | Name | Data Type | Big Engian | Bitlength base type | Bitlength enc. type
        idCntr = 1
        bigEndian = "TRUE"
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Base Type List config file is automatically generated, DO NOT MODIFY\n"
            )
            for basetype_name, basetype_info in basetypes2.items():
                file.write(
                    f'"{idCntr}","{basetype_name}","{basetype_name}","{bigEndian}","{basetype_info[1]}","{basetype_info[1]}"\n'
                )
                idCntr += 1

    def __generate_SOMEIP_ParameterString_config(self, unions, config_file):
        """Generate a Wireshark config file for Parameter Strings."""
        # config_file = "11_SOMEIP_parameter_strings"
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Strings config file is automatically generated, DO NOT MODIFY\n"
            )

    def __generate_SOMEIP_ParameterTypdefList_config(
        self, typerefs, basetypes_typedefs, config_file
    ):
        """Generate a Wireshark config file for Parameter Typedef.

        Parameters:
        - typerefs: dictionary of typerefs found in the arxml
        - basetypes_typedefs dictionary of typerefs that are basetypes as well from the arxml.
        """
        # config_file = "12_SOMEIP_parameter_typedefs_DONE"
        idCntr = 1
        with open(config_file, "w") as file:
            file.write(
                "# This Parameter Type Ref config file is automatically generated, DO NOT MODIFY\n"
            )
            file.write("# ID | Name | Data Type | ID Reference\n")
            for typeref_name, typeref_info in typerefs.items():
                param_type, id_ref = self.__getParam_IDType(typeref_info[1])
                file.write(f'"{idCntr}","{typeref_name}","{param_type}","{id_ref}"\n')
                idCntr += 1
            for basetype_name, basetype_info in basetypes_typedefs.items():
                param_type, id_ref = self.__getParam_IDType(basetype_info[1])
                file.write(f'"{idCntr}","{basetype_name}","{param_type}","{id_ref}"\n')
                newTuple = (idCntr,) + basetype_info
                basetypes_typedefs[basetype_name] = newTuple
                idCntr += 1

    def __generate_html_content(self, connected_services, interfaces):
        """Generate HTML content from the given data and options.

        This function transforms the provided data into HTML format, allowing for optional customization based on the provided options.
        The generated HTML can be used for web pages, reports, or any other context where HTML content is required.
        The specifics of the HTML generation, such as the structure, styling, and inclusion of dynamic content, depend on the nature
        of the input data and the options specified.

        Parameters:
        data (DataType): The primary data from which to generate the HTML content.
        The type and structure of this data (e.g., dictionary, list, custom object) will dictate the format of the resulting HTML.

        options (dict, optional): A dictionary of options to customize the HTML output.
        Possible options could include styling preferences, data filtering criteria, or layout configurations.
        Defaults to None, indicating no custom options are applied. Returns: str: A string containing the generated HTML content.

        Raises: ValueError: If any issues are encountered with the input data that prevent HTML generation.
        """
        # html = "<html><head><style>"
        # html += "table {border: 1px solid black; border-collapse: collapse; width: 80%; table-layout: fixed;}"
        # html += "th, td {border: 1px solid black; padding: 5px; text-align: left;}"
        # html += "th.property, td.property {width: 50%;}"  # Set width for property column
        # html += "th.center, td.center {width: 16%; text-align: center;}"  # Set width for type, size, and array size columns
        # html += "</style></head><body>"

        html = """
        <html>
        <head>
        <style>
            body {
                font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
                margin: 20px;
                background-color: #f9f9f9;
                color: #333;
            }
            h1, h2, h3 {
                color: #2c3e50;
            }
            table {
                width: 100%;
                border-collapse: collapse;
                margin-bottom: 30px;
                font-size: 14px;
            }
            th, td {
                border: 1px solid #ccc;
                padding: 10px;
                text-align: left;
            }
            th {
                background-color: #34495e;
                color: white;
            }
            tr:nth-child(even) {
                background-color: #f2f2f2;
            }
            tr:hover {
                background-color: #e0f7fa;
            }
            ul {
                padding-left: 20px;
            }
            p {
                margin: 5px 0;
            }
        </style>
        </head>
        <body>
        """

        html += "<h1>SOME/IP Service Summary</h1>"
        html += "<ul>"
        html += "<table>"
        html += "<tr><th>Service Name</th><th>Service ID</th><th>Payload Size (bytes)</th></tr>"

        for service_id, info in connected_services.items():
            service_version, service_name = info["service_data"]
            total_bits = 0
            # print(service_id)
            # print(service_name)
            for interface in info["interfaces"]:
                if interface in interfaces:
                    tref_path = interfaces[interface][0][1]
                    struct_name = (
                        tref_path.rpartition("/")[-1] if tref_path else interfaces[interface][0][0]
                    )
                    struct_breakdown = self.__drill_down_structure(struct_name)

                    for field_name, field_info in struct_breakdown.items():
                        main_array_size = 1
                        if isinstance(field_info, dict) and "array size" in field_info:
                            main_array_size = int(field_info.get("array size", 1)) or 1
                            field_info = field_info["type"]

                        for var_name, var_info in field_info.items():
                            if isinstance(var_info, dict) and "size" in var_info:
                                try:
                                    size = int(var_info.get("size", 0))
                                    array_size = int(var_info.get("array size", 1)) or 1

                                except ValueError:
                                    print(var_info)
                                    print("Not a valid integer, skipping.")

                                total_bits += size * array_size * main_array_size
                            else:
                                for var_name, var_info2 in var_info.items():
                                    if isinstance(var_info2, dict) and "size" in var_info2:
                                        try:
                                            size = int(var_info2.get("size", 0))
                                            array_size = int(var_info2.get("array size", 1)) or 1
                                        except ValueError:
                                            print(field_name)
                                            print(var_name)
                                            print("Not a valid integer, skipping.")

                                        total_bits += size * array_size * main_array_size

            total_bytes = total_bits // 8
            html += f"<tr><td>{service_name}</td><td>{hex(int(service_id))}</td><td>{total_bytes}</td></tr>"

        html += "</table>"
        html += "</ul>"
        html += "<h1>SOME/IP Service Details</h1>"

        # html = "<html><head><style>table, th, td {border: 1px solid black; border-collapse: collapse;} th, td {padding: 5px;}</style></head><body>"
        # html += "<h1>Service Details</h1>"

        for service_id, info in connected_services.items():
            service_version, service_name = info["service_data"]
            html += f"<h2>Service ID: {hex(int(service_id))}</h2>"
            html += f"<p>Service Name: {service_name}<br>Service Version: {service_version}</p>"
            html += "<h3>Linked Interfaces:</h3>"
            html += "<ul>"
            for interface in info["interfaces"]:
                html += f"<li>{interface}</li>"
                if interface in interfaces:
                    tref_path = interfaces[interface][0][1]
                    struct_name = (
                        tref_path.rpartition("/")[-1] if tref_path else interfaces[interface][0][1]
                    )  # type name
                    html += f"<p>Struct: {struct_name}</p>"
                    struct_breakdown = self.__drill_down_structure(struct_name)
                    html += "<table>"
                    html += "<tr ><th>Property</th><th class='center'>Type</th><th class='center'>Size</th><th class='center'>Array Size</th><th class='center'>Status</th></tr>"
                    html += self.__convert_structure_to_html_table(struct_breakdown)
                    html += "</table>"
            html += "</ul>"

        html += "</body></html>"
        return html

    def __convert_structure_to_html_table(self, structure, indent=0, array_size=0):
        """Convert a given structure into an HTML table format."""
        html = ""
        # If structure is not a dict, render a simple placeholder row
        if not isinstance(structure, dict):
            return "<tr><td colspan='5'>N/A</td></tr>"
        for key, value in structure.items():
            if key == "type" or key == "array size" or key.isdigit():
                if isinstance(value, dict):  # It's a nested structure
                    # html += "<tr>"
                    # html += "<td style='padding-left: " + str(indent * 20) + "px;'>" + str(key) + "</td>"

                    sub_html = self.__convert_structure_to_html_table(value, indent)

                    html += f"<tr class='center'>{sub_html}</tr>"
                    # html += "</tr>"
                # else:
                # It's a direct value
                #   # html += f"<td>{value.get('type', '-')}</td>"
                #   # html += f"<td>{value.get('size', '-')}</td>"
                #   # html += f"<td>{value.get('array size', '-')}</td>" ## Main base type
                #   print(f"TEST1: {key}:{value}")
                #   if (key == "type"):
                #       print (value)

                #   html += f"<td colspan='3'>FRED{value}</td>"
            elif key == "size":
                continue  # do nothing
            else:
                html += "<tr>"
                html += (
                    "<td style='padding-left: " + str(indent * 20) + "px;'>" + str(key) + "</td>"
                )

                if isinstance(value, dict) and "type" in value:  # It's a base type
                    if isinstance(value["type"], dict):

                        # print(array_size, value.get('array size', 'N/A'))
                        # if value.get('array size'):
                        #   html += f"<td colspan='3'>FRED1_{value.get('array size')}</td>"

                        sub_html = self.__convert_structure_to_html_table(
                            value, indent + 1, value.get("array size", "-")
                        )

                        if "type" in value["type"]:
                            html += f"<td class='center'>{value['type'].get('type', '-')}</td>"
                            html += f"<td class='center'>{value['type'].get('size', '-')}</td>"
                            # Array row with size
                            html += f"<td class='center'>{value.get('array size', '-')}</td>"
                            html += f"<td class='center'>TODO</td>{sub_html}"

                        else:
                            html += "<td class='center'>-</td>"
                            html += "<td class='center'>-</td>"
                            # Array row with size
                            html += f"<td class='center'>{value.get('array size', '-')}</td>"
                            html += f"<td class='center'>TODO</td>{sub_html}"
                    else:
                        html += f"<td class='center'>{value.get('type', '-')}</td>"
                        html += f"<td class='center'>{value.get('size', '-')}</td>"
                        # Main base type
                        html += f"<td class='center'>{value.get('array size', '-')}</td>"
                        html += "<td class='center'>TODO</td>"
                elif isinstance(value, dict):
                    # It's a nested structure
                    # create a sub-table for nested structures
                    sub_html = self.__convert_structure_to_html_table(
                        value, indent + 1, array_size
                    )

                    # Structure name row
                    html += f"<td colspan='4'>{sub_html}</td>"
                else:
                    # It's a direct value
                    html += f"<td colspan='4'>{value}</td>"
                html += "</tr>"
        return html


if __name__ == "__main__":

    arg_parser = argparse.ArgumentParser(
        description="Parse ARXML file to create SOME-IP Wireshark configuration files.",
        epilog="APTIV 2024",
    )

    arg_parser.add_argument("-a", "--arxml", type=str, help=".arxml input file", required=True)
    arg_parser.add_argument("-o", "--html", type=str, help=".html output file", required=True)
    arg_parser.add_argument("-l", "--lua", type=str, help=".lua output file", required=True)
    arg_parser.add_argument(
        "-c",
        "--configs",
        type=str,
        nargs="+",
        help="The configs used by Wireshark for SOME/IP",
        required=True,
    )
    arg_parser.add_argument("-s", "--swversionFile", type=str, help="Version .c file")
    arg_parser.add_argument("-v", "--variant", type=str, help="Variant name")

    arguments = arg_parser.parse_args()
    arxml_parser = Arxml_Parser(arguments.arxml)
    arxml_parser.save_html_report(arguments.html)
    arxml_parser.save_configs(arguments.configs)
    arxml_parser.save_lua(arguments.lua, arguments.swversionFile, arguments.variant)
