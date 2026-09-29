import argparse
import re
import collections

# Define a file ignore list where we dont want iwyu to intervene
blacklist = ['Mock_Files', 'debug_writer', 'debug_interface']

# Define a include mapping which shall be caused by our pragmas.
# However when an include is completly missing, iwyu guides us to include the wrong header.
# Note: pa_pa_reuse.h is intended to be the last one in the dictionary. We are not stupid, just lazy.
inc_to_be_added_mapping = {'reuse.h': 'pa_reuse.h',
                           '<stdint.h>': '"pa_reuse.h"',
                           '<stddef.h>': '"pa_reuse.h"',
                           'pa_pa_reuse.h': 'pa_reuse.h'}

# Define regex for parsing of our console output
should_add_parser = re.compile('([\/\w\-\.]*?) should add.*?\:(.*?)\n\n', re.DOTALL)
should_remove_parser = re.compile('([\/\w\-\.]*?) should remove.*?\:(.*?)\n\n', re.DOTALL)
cycle_parser = re.compile('Cycle in include-mapping:\n(.*?)..\/', re.DOTALL)


# class summarizing elements of iwyu finding
class Iwyu_Finding:
    def __init__(self, filename, datatype, component):
        self.missing_includes = []
        self.unnecessary_includes = []
        self.filename = filename
        self.datatype = datatype
        self.component = component

    def append_missing_include(self, include):
        self.missing_includes.append(include)

    def append_unnecessary_include(self, include):
        self.unnecessary_includes.append(include)


def get_component_by_file(file, feature):
    if feature in file:
        component = ''.join(feature)
    return component


# this function accumulates all iwyu findings from the build log.
def accumulate_findings(build_log, feature):
    all_lines = open(build_log, "r").read()
    list_of_iwyu_findings_internal = []
    should_add_matches = should_add_parser.findall(all_lines)
    should_rem_matches = should_remove_parser.findall(all_lines)

    # at first create objects based on "should add" findings
    for match in should_add_matches:
        file = match[0]  # access match in first group
        if feature in file:
            datatype = file.split(".")[-1]
            filename = file.split(".")[0]
            component = get_component_by_file(file, feature)
            list_of_iwyu_findings_internal.append(Iwyu_Finding(filename, datatype, component))
            list_of_iwyu_findings_internal[-1].append_missing_include(match[1])  # access match in second group

    # secondly create objects or modify object member based on "should remove" findings
    for match in should_rem_matches:
        file = match[0]
        if feature in file:
            filename = file.split(".")[0]
            datatype = file.split(".")[-1]
            component = get_component_by_file(file, feature)
            # check whether an object with for this file was already created
            f_obj_created = False
            for obj in list_of_iwyu_findings_internal:
                if obj.filename == filename and obj.datatype == datatype:
                    obj.append_unnecessary_include(match[1])
                    f_obj_created = True
                    break

            # append an object otherwise
            if not f_obj_created:
                list_of_iwyu_findings_internal.append(Iwyu_Finding(filename, datatype, component))
                list_of_iwyu_findings_internal[-1].append_unnecessary_include(match[1])

    return list_of_iwyu_findings_internal


def write_single_iwyu_to_file(file, iwyu):
    file.write("%s.%s has the following findings:\n" % (iwyu.filename, iwyu.datatype))
    missing_inc_str = "".join(iwyu.missing_includes)
    if missing_inc_str and missing_inc_str.strip():
        file.write("Add the following includes:")
        file.write("%s\n\n" % missing_inc_str)
    unnecessary_inc_str = "".join(iwyu.unnecessary_includes)
    if unnecessary_inc_str and unnecessary_inc_str.strip():
        file.write("Remove the following includes:")
        file.write("%s\n\n" % unnecessary_inc_str)


def create_iwyu_report(iwyu_findings, circular_dep_list, iwyu_number_of_occurences):
    iwyu_findings.sort(key=lambda x: (x.filename, x.datatype))

    sw_iwyus = []
    for obj in iwyu_findings:
        sw_iwyus.append(obj)

    with open("iwyu_report.txt", "a") as f:

        # Note down all production findings
        f.write("Iwyu findings in Software:\n\n")
        for obj in sw_iwyus:
            write_single_iwyu_to_file(f, obj)

        f.write("Circular dependencies:\n")

        # Note occurence of a circular dependency
        if circular_dep_list:
            for dict_element in circular_dep_list:
                f.write("Circular dependency start at %s\n" % (dict_element["start"]))
                f.write("Path of the dependency is as follows:\n%s\n" % (dict_element["path"]))
        else:
            f.write("No circular dependencies found. Hoooray\n\n")

        # Note amount of findings for every interesting feature
        f.write("Overall number of findings per component:\n")
        for key, value in iwyu_number_of_occurences.items():
            f.write("Component %s has %s findings\n" % (key, value))


def accumulate_cycles(build_log):
    all_lines = open(build_log, "r").read()

    list_of_cylces = []
    cycle_matches = cycle_parser.findall(all_lines)
    for match in cycle_matches:
        include_path = match
        starting_point = include_path.split("->")[0]
        list_of_cylces.append({"start": starting_point, "path": include_path})

    return list_of_cylces


def count_iwyu_findings(list_of_iwyu_findings, feature_under_investigation):
    seen = set()

    # Get unique elements which are interesting for us
    for finding in list_of_iwyu_findings:
        if finding not in seen:
            seen.add(finding.component)

    # Count the findings which are given in the objects
    occurences = {el: 0 for el in seen}
    for finding in list_of_iwyu_findings:
        missed_inc = "".join(finding.missing_includes)
        unn_inc = "".join(finding.unnecessary_includes)
        missing_includes_count = 0
        unn_includes_count = 0
        if missed_inc and missed_inc.strip():
            missing_includes_count = missed_inc.count('#include')
        if unn_inc and unn_inc.strip():
            unn_includes_count = unn_inc.count('#include')
        occurences[finding.component] += unn_includes_count + missing_includes_count

    # Check whether our components are available. In case they are not available,
    # the parsers were not able to find iwyu issues here.
    if feature_under_investigation not in occurences:
        occurences[feature_under_investigation] = 0

    # Give everything a consistent order
    occurences = collections.OrderedDict(sorted(occurences.items()))

    return occurences


def filter_finding_list_by_filename(list_of_iwyu_findings):
    filtered_list_of_iwyu_findings = []
    for finding in list_of_iwyu_findings:
        if any(blacklist_entry in finding.filename for blacklist_entry in blacklist):
            print("File ", finding.filename, " was filtered due to blacklist")
        else:
            filtered_list_of_iwyu_findings.append(finding)
            print(finding.filename)
    return filtered_list_of_iwyu_findings


def adapt_missing_includes_by_mapping(list_of_iwyu_findings):
    for idx, finding in enumerate(list_of_iwyu_findings):
        for missing_inc in finding.missing_includes:
            for key, val in inc_to_be_added_mapping.items():
                missing_inc = re.sub(key, val, missing_inc)
            list_of_iwyu_findings[idx].missing_includes = missing_inc
    return list_of_iwyu_findings


# main function for creation of iwyu report
if __name__ == '__main__':
    # define command line options
    # this also generates --help and error handling
    CLI = argparse.ArgumentParser(description='Parse build logs and create an include-what-you-use report.')
    CLI.add_argument("build_log_path", help='Path pointing to a build log file')

    feature_under_investigation = "Calibration_Tool"

    # parse the command line
    args = CLI.parse_args()

    # parse build log for iwyu elements
    list_of_iwyu_findings = accumulate_findings(args.build_log_path, feature_under_investigation)

    # parse build log for cycle dependencies
    list_of_circular_includes = accumulate_cycles(args.build_log_path)

    # parse build log for cycle dependencies
    list_of_iwyu_findings = filter_finding_list_by_filename(list_of_iwyu_findings)

    # do a string replacement of includes to be added to fulfill our needs
    list_of_iwyu_findings = adapt_missing_includes_by_mapping(list_of_iwyu_findings)

    # count interesting iwyu findings
    iwyu_number_of_occurences = count_iwyu_findings(list_of_iwyu_findings, feature_under_investigation)

    # create report for iwyu findings
    create_iwyu_report(list_of_iwyu_findings, list_of_circular_includes,
                       iwyu_number_of_occurences)
