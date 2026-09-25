"""This module defines a subclass of Stack_Parser that can parse xtensa BBE32 elf files using xt-stack-usage."""
import sys
import os
from configparser import SectionProxy
from stack_parser import Stack_Parser, Stack_Node, Configstate
from subprocess import Popen, PIPE, DEVNULL
from pathlib import Path


class XT_Stack_Node(Stack_Node):
    """Subclass of Stack_Node that adds additional xtensa-specific fields."""

    def __init__(self, name: str = "main", section: SectionProxy = None):
        """Extend the base class init with additional report field."""
        # call base class init
        Stack_Node.__init__(self, name, section)

        # add xt-specific fields
        self.xtreport = ""


class XT_Stack_Parser(Stack_Parser):
    """Derived class of Stack_Parser that implements stack usage reporting for xtensa BBE32 by way of the xt-stack-usage utility."""

    parser_type = "xt-stack-usage"

    def __init__(self, configstate: Configstate):
        """Extend the base class init with xtensa-specific configuration."""
        # call base class init
        Stack_Parser.__init__(self, configstate)

        # get the xt-specific configuration options
        config = self.config
        configfile = configstate.args.configfile

        if not config.has_option("config", "xtensa_core"):
            raise KeyError(
                "Option 'xtensa_core' not found in {} section [config]".format(configfile)
            )

        self.xtensa_core = config["config"]["xtensa_core"]
        self.xtensa_system = config["config"].get("xtensa_system_" + sys.platform, None)
        if self.xtensa_system is None:
            self.xtensa_system = config["config"].get("xtensa_system", "")

        # determine the exec root
        if self.args.exec_root is None:
            bazelexec = Popen(["bazel", "info", "execution_root"], stderr=DEVNULL, stdout=PIPE)
            exec_root, _ = bazelexec.communicate(timeout=15)
            self.args.exec_root = exec_root.decode().strip()

        # set up some helper class variables
        self.__env = os.environ.copy()
        self.__env["XTENSA_CORE"] = self.xtensa_core
        if self.args.system_path:
            self.__env["XTENSA_SYSTEM"] = self.args.system_path
        else:
            p = Path(self.args.exec_root, self.xtensa_system)
            self.__env["XTENSA_SYSTEM"] = str(p.resolve())
        xtbin = "xt-stack-usage"
        if self.args.tool_path:
            # args.tool_path may be a full path to the tool executable
            self.__xtbin = self.args.tool_path
        else:
            if (sys.platform == "win32") or (sys.platform == "cygwin"):
                xtbin += ".exe"
            # construct a reasonable default path under the exec_root using the configured xtensa_system
            p = Path(self.args.exec_root, self.xtensa_system, xtbin)
            self.__xtbin = str(p.resolve())

    def __str__(self):
        # call base class __str__
        result = Stack_Parser.__str__(self)

        # append xt-specific details
        if self.args.verbose:
            result += """
  xtensa_core: {}
  xtensa_system: {}""".format(
                self.xtensa_core, self.xtensa_system
            )

        return result

    # override __parse_node__ to provide our custom XT_Stack_Node class
    def __parse_node__(self, entry_point: str = "main", node_class=XT_Stack_Node) -> Stack_Node:
        # just call the parent implementation
        return Stack_Parser.__parse_node__(self, entry_point, XT_Stack_Node)

    def stack_evaluator(self, node: Stack_Node) -> int:
        """
        Extend base class stack_evaluator with xtensa toolchain implementation.

        This evaluator function populates the stack_usage field of the
        Stack_Node using the xt-stack-usage utility.
        It is suitable to be passed as a parameter to Stack_Parser.evaluate().

        The return value is an integer representing the stack usage for this
        and the aggregated child nodes.
        """
        error = self.log.error
        entry_point = node.entry_point

        xtexec = Popen(
            [self.__xtbin, "-CesadRUFlxM", "--entry=" + entry_point, self.args.inputfile],
            env=self.__env,
            stdout=PIPE,
        )
        while xtexec.returncode is None:
            output, _ = xtexec.communicate(timeout=15)
            node.xtreport += output.decode()

        if not node.xtreport.startswith("Stack usage: "):
            raise RuntimeError(
                "Unable to parse stack size from xt-stack-usage report.\nOutput from tool:\n"
                + node.xtreport
            )

        # take the first line, which we just confirmed is "Stack usage: "
        # split on whitespace and grab the 3rd token which is the stack size
        stack_string = node.xtreport.splitlines()[0].split()[2]

        # check whether this ends with +, as that indicates that there should
        # be child nodes that we have to aggregate
        if stack_string.endswith("+"):
            stack_string = stack_string[:-1]  # strip it
            if node.num_children == 0:
                error(
                    "Warning: xt-stack-usage reports that it can't determine the stack size for {}, but the configuration does not indicate any child branches to check".format(
                        entry_point
                    )
                )
        node.stack_usage = int(stack_string)

        # perform some additional sanity checks while we're here
        if node.xtreport.find("No recursions detected.") < 0:
            error(
                "Warning: xt-stack-usage reports that recursion cycles were detected for {}".format(
                    entry_point
                )
            )
        if node.xtreport.find("No undefined references found.") < 0:
            error(
                "Warning: xt-stack-usage reports that undefined references were found for {}".format(
                    entry_point
                )
            )

        return Stack_Parser.stack_evaluator(self, node)

    def print_report(self, node: Stack_Node):
        """
        Extend the base class print_report with additional report details from the xtensa toolchain.

        This evaluator function writes the stack_usage report from
        xt-stack-usage for the given node to the output report file requested
        on the command line.
        It is suitable to be passed as a parameter to Stack_Parser.evaluate().
        """
        output = self.args.output

        # print delimiter
        print(
            "\n###############################################################################",
            file=output,
        )
        # call base class print_report, which has a useful tree output with an
        # asterisk identifying the largest subtree
        Stack_Parser.print_report(self, node)

        # print delimiter and append xt-stack-usage report output
        print(
            "\n############################ xt-stack-usage report ############################",
            file=output,
        )
        print(node.xtreport, end="", file=output)
