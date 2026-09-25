#!/usr/bin/env python
"""
This tool assists with determining the maximum stack usage via static analysis.

One of the major problems with this type of analysis is that the compiler is
often not able to determine what functions might be called when function
pointers or longjmps are used.
To support these use cases, this tool takes a configuration ini file as input.
This configuration file can describe the heirarchy of indirect function calls.
The tool will then evaluate each indirect call as its own entry point and use
the maximal stack size of the child branches as the value to propagate upwards.
"""
import argparse
import configparser
import sys
from stack_parser import Configstate
from loghelper import Logger
from pathlib import Path

# Note: additional imports below. See the match statement in script_main.


# helper functions
def parse_args(inargs) -> argparse.Namespace:
    """Parse command line input options using argparse."""
    parser = argparse.ArgumentParser(
        description="""This tool assists with determining the maximum stack usage via static analysis."""
    )
    parser.add_argument("-r", "--exec_root", help="specify the bazel exec root path")
    parser.add_argument(
        "-o", "--output", type=argparse.FileType("w"), help="specify an output file"
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="extra output for debug")
    parser.add_argument("-t", "--tool_path", help="specify the path to the parsing tool")
    parser.add_argument("-s", "--system_path", help="specify the path to the xtensa system")
    parser.add_argument("configfile", help="specify the configuration .ini file")
    parser.add_argument("inputfile", help="specify the input .elf/.map file")
    return parser.parse_args(inargs)


# processing functions
def parse_config(configstate: Configstate) -> str:
    """Parse the config file and return the tool string."""
    verbose = configstate.log.verbose
    config = configstate.config
    configfile = Path(__file__).parent.joinpath(configstate.args.configfile).resolve()

    config.read(configfile)
    verbose(config.sections())

    # sanity check the config
    if not config.has_section("config"):
        raise KeyError("Section [config] not found in {}".format(configfile))
    if not config.has_option("config", "tool"):
        raise KeyError("Option 'tool' not found in {} section [config]".format(configfile))

    return config["config"]["tool"]


def script_main(inargs) -> int:
    """Execute main logic for the script."""
    log = Logger()
    verbose = log.verbose

    args = parse_args(inargs)
    log.set_verbose(args.verbose)
    verbose(args)

    # parse the config file
    verbose("Parsing the config file...")
    config = configparser.ConfigParser(interpolation=configparser.ExtendedInterpolation())
    configstate = Configstate(args, config, log)

    tool = parse_config(configstate)
    verbose("tool={}".format(tool))

    match tool:
        case "xt-stack-usage":
            from xt_stack_usage import XT_Stack_Parser

            stackparser = XT_Stack_Parser(configstate)
        case _:
            raise NotImplementedError("Support for tool {} is not available".format(tool))
    verbose(stackparser)

    # parse the call graph from the config file
    verbose("Building the call graph...")
    stackparser.parse()

    # evaluate the stack size for each branch in the call graph and recursively
    # aggregate them into a top-level stack size number
    verbose("Evaluating stack sizes...")
    stack_size = stackparser.evaluate(stackparser.stack_evaluator)

    # determine top-level stack size "add ons"
    extra_stack = int(config["config"].get("extra_stack", "0"), base=0)
    max_intr_size = 0
    if len(stackparser.interrupts) > 0:
        max_intr_size = max(i.aggregated_stack_usage for i in stackparser.interrupts)
    verbose("  adding extra stack {} and interrupt stack {}".format(extra_stack, max_intr_size))
    stack_size += extra_stack
    stack_size += max_intr_size

    max_stack = int(config["config"].get("max_stack", "0x3fffffff"), base=0)

    log.stdprint(
        "Overall stack size for {}:{}: {}".format(
            args.inputfile, stackparser.get_entry_point_name(), stack_size
        )
    )

    # write report
    if args.output is not None:
        verbose("Writing report to {}...".format(args.output.name))
        print(
            "Overall stack size for {}:{}: {}".format(
                args.inputfile, stackparser.get_entry_point_name(), stack_size
            ),
            file=args.output,
        )
        stackparser.evaluate(stackparser.print_report, depth_first=False)

    if stack_size > max_stack:
        log.stdprint("Error: stack size exceeds maximum stack size allowed ({})".format(max_stack))
        return 1
    else:
        return 0


# executable entry point
if __name__ == "__main__":
    sys.exit(script_main(sys.argv[1:]))
