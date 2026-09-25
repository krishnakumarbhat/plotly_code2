"""This arxml parser is used to generate the wireshark config files."""
# !/usr/bin/python3

import argparse
import xml.etree.ElementTree as ET
import os
from someip_signals_parser import SignalParser

ns = {"ns": "http://autosar.org/schema/r4.0"}
services = {}
interfaces = {}
structs = {}
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

        print("Parsing...", self.arxmlFile)

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
        # # Note: You need to implement the logic for this function based on your ARXML file
        # connected_services = connect_services_to_interfaces(ethernet_clusters, signal_triggerings)
        self.connected_services = self.__connect_services_to_interfaces()

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

    def __get_child_attribute(self, element, childtag, attribkey):
        """Find the child attrubute of a given element."""
        if childtag is None or attribkey is None:
            return "First"

        c = element.find(childtag, ns)
        # print(c)
        # print(c.text)
        if c is None:
            # xml.etree.ElementTree.dump(element)
            return "Second"
        if attribkey in c.attrib:
            return c.attrib[attribkey]
        # print(c.attrib)
        return "Third"

    def __get_child_attribute_text(self, element, childtag, attribkey):
        """Get the given element's child attribute text."""
        if childtag is None or attribkey is None:
            return "First"

        c = element.find(childtag, ns)
        # print(c)
        # print(c.text)
        if c is None:
            # xml.etree.ElementTree.dump(element)
            return "Second"
        if attribkey in c.attrib:
            return c.attrib[attribkey].text
        # print(c.attrib)
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
        """Get the reference path of the given element."""
        c = element.find(attribute_tag, ns)
        if c is not None:
            if "DEST" in c.attrib:
                if c.attrib["DEST"] == attribute_value:
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
            # print(psis)
            # print(name, instance_id, service_id)

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
            for var in sri.findall(".//ns:VARIABLE-DATA-PROTOTYPE", ns):
                # print(var)
                variables = []
                var_name = var.find("ns:SHORT-NAME", ns).text
                # print(var_name)
                dtype_ref = self.__get_ref(var, "ns:TYPE-TREF", "IMPLEMENTATION-DATA-TYPE")
                variables.append((var_name, dtype_ref))
            interfaces[name] = variables
            # print("INTERFACE:", name)
        for par_list in interfaces.values():
            for var in par_list:
                # print(vars[1])
                self.__parse_datatype(var[1])

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

    # <CONSUMED-SERVICE-INSTANCE>
    # 														<SHORT-NAME>consService_RDR_2_Detections_from_RDR_1</SHORT-NAME>
    # 														<CONSUMED-EVENT-GROUPS>
    # 															<CONSUMED-EVENT-GROUP>
    # 																<SHORT-NAME>consServiceGrp_RDR_2_Detections_from_RDR_1</SHORT-NAME>
    # 																<EVENT-GROUP-IDENTIFIER>0x1</EVENT-GROUP-IDENTIFIER>

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
            "ASDM_Vehicle_Status": "Interface_Vechicle_Status_Event",
            "RDR_1_Detections": "Interface_DetectionList_Event",
            "RDR_1_SW_Info": "Interface_RDR_SW_Info_Event",
            "RDR_1_Status": "Interface_RDR_Status_Event",
            "RDR_1_Tracker_Data": "Interface_ObjectList_Event",
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
                #  print("FOUND", "category:", category)
                # use if-elif-else or 3.10 pattern matching?
                self.__add_datatype(name, category, dtype)

    def __drill_down_structure(self, struct_name):
        """Drill down to the base level of each value from a struct."""
        # Check if the structure is in the base types first
        if struct_name in basetypes_typedef:
            base_type_info = basetypes_typedef[struct_name]
            return {
                "type": base_type_info[2],
                "size": base_type_info[1],
            }  # Return only type and size

        # If not, proceed to check in structures
        if struct_name not in structs:
            return f"Structure {struct_name} not found"

        structure = structs[struct_name][1:]
        result = {}

        for property_name, property_type in structure:
            if property_type == "VALUE":
                if property_name in basetypes_typedef:
                    base_type_info = basetypes_typedef[property_name]
                    result[property_name] = {"type": base_type_info[2], "size": base_type_info[1]}
                else:
                    result[property_name] = "VALUE"
            elif property_type == "TYPE_REFERENCE":
                ref_struct_name = typerefs[property_name][1].split("/")[-1]
                result[property_name] = self.__drill_down_structure(ref_struct_name)
            elif property_type == "ARRAY":
                array_info = arrays[property_name]
                array_size = array_info[0]  # Extracting the size of the array
                array_type = array_info[3]

                if array_type == "TYPE_REFERENCE":
                    ref_struct_name = array_info[2]
                    result[property_name] = {
                        "type": self.__drill_down_structure(ref_struct_name),
                        "array size": array_size,
                    }
                else:
                    result[property_name] = {"type": array_type, "array size": array_size}
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
        # elif val in enums:
        #     print("!!!!!!!!!!!!!!!!!!!!!")
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
                file.write(f'"{int(service_id):x}","{method_id}","{method_name}"\n')

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
            # for method_id, method_name in methods.items():
            #     file.write(f'"{int(service_id):x}","{service_name}"\n')

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
                "ID | Name | Parameter Type | ID Reference | Number of Items | Value | Value Name\n"
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
        html = "<html><head><style>"
        html += "table {border: 1px solid black; border-collapse: collapse; width: 80%; table-layout: fixed;}"
        html += "th, td {border: 1px solid black; padding: 5px; text-align: left;}"
        html += "th.property, td.property {width: 50%;}"  # Set width for property column
        html += "th.center, td.center {width: 16%; text-align: center;}"  # Set width for type, size, and array size columns
        html += "</style></head><body>"
        html += "<h1>Service Details</h1>"

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
                    struct_name = interfaces[interface][0][1]  # Accessing the desired string
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
        """Convert a given structure into an HTML table format.

        This function takes a data structure (like a list, dictionary, or custom object) and converts it into an HTML table representation.
        The conversion process depends on the structure of the input.
        The function aims to represent the data in a tabular format, suitable for displaying in a web browser or other HTML-supporting environments.

        Parameters:
        structure (type): A description of the input parameter 'structure', including its type and what it represents.

        Returns:
        str: An HTML string representing the input structure formatted as a table.
        """
        html = ""
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
                            html += f"<td class='center'>{signalParser.getSignalStatus(key)}</td>{sub_html}"

                        else:
                            html += "<td class='center'>-</td>"
                            html += "<td class='center'>-</td>"
                            # Array row with size
                            html += f"<td class='center'>{value.get('array size', '-')}</td>"
                            html += f"<td class='center'>{signalParser.getSignalStatus(key)}</td>{sub_html}"
                    else:
                        html += f"<td class='center'>{value.get('type', '-')}</td>"
                        html += f"<td class='center'>{value.get('size', '-')}</td>"
                        # Main base type
                        html += f"<td class='center'>{value.get('array size', '-')}</td>"
                        html += f"<td class='center'>{signalParser.getSignalStatus(key)}</td>"
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
    arg_parser.add_argument(
        "-s",
        "--source",
        type=str,
        action="append",
        help=".source input file for parsing implemented signals",
        required=True,
    )
    arg_parser.add_argument("-o", "--html", type=str, help=".html output file", required=True)
    arg_parser.add_argument(
        "-c",
        "--configs",
        type=str,
        nargs="+",
        help="The configs used by Wireshark for SOME/IP",
    )

    arguments = arg_parser.parse_args()
    signalParser = SignalParser()
    for file in arguments.source:
        signalParser.parseAssignedSignals(file)
    arxml_parser = Arxml_Parser(arguments.arxml)
    arxml_parser.save_html_report(arguments.html)
    arxml_parser.save_configs(arguments.configs)
