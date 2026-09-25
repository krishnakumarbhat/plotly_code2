"""
This module defines a parent class to handle basic stack parsing use-cases.
"""
import sys
from argparse import Namespace
from configparser import ConfigParser, SectionProxy
from dataclasses import dataclass
from loghelper import Logger


@dataclass
class Configstate:
    """Structure (dataclass) for marshalling the configuration state more conveniently."""

    args: Namespace
    config: ConfigParser
    log: Logger


class Stack_Node:
    """Represent a node in the call graph."""

    def __init__(self, name: str = "main", section: SectionProxy = None):
        """Initialize the stack node from the provided configparser section."""
        self.entry_point = name
        self.num_children = 0
        self.stack_usage = None
        self.aggregated_stack_usage = 0
        self.extra_stack = 0
        self.__children = []
        if section is not None:
            self.extra_stack = int(section.get("extra_stack", "0"), base=0)

    def __str__(self):
        return "Node {}: children={}, extra_stack={}, stack_usage={}, aggregated_stack_usage={}".format(
            self.entry_point,
            self.num_children,
            self.extra_stack,
            self.stack_usage,
            self.aggregated_stack_usage,
        )

    def __iter__(self):
        return self.__children.__iter__()

    def add_child(self, child):
        """
        Append a child node.

        The child parameter should be a Stack_Node, but this can't be annotated
        until python 3.11.
        """
        self.__children.append(child)
        self.num_children += 1

    def evaluate(self, evaluator, depth_first=True):
        """
        Iterate the child Stack_Nodes and execute the evaluator function on each.

        The depth_first parameter determines the order that nodes are visited.
        The return value is the final return from evalutor(self).

        Prototype for evaluator functions should be:
        ```
        def foo(self, node: Stack_Node) -> int:
        ```
        """
        if not depth_first:
            ret = evaluator(self)

        for child in self.__children:
            child.evaluate(evaluator, depth_first)

        if not depth_first:
            return ret
        else:
            return evaluator(self)


class Stack_Parser:
    """
    Parent class for stack parsers.

    Individual parsers would extend this class with the relevant
    implementation.

    The stack_evaluator() function must be overriden or extended by subclasses.
    It can also be useful to override or extend print_report(), __init__(), and
    __str__().
    Other member functions can also be freely overriden, but this is mostly
    unnecessary. An exception might be __parse_node__(), in case the
    implementation is also extending the Stack_Node class.
    """

    parser_type = "unknown"

    def __init__(self, configstate: Configstate):
        """Initialize the class with fields from configstate."""
        config = configstate.config
        self.log = configstate.log
        self.args = configstate.args
        self.config = config
        self.top = None
        self.interrupts = []

        self.tool_path = config["config"].get("tool_path_" + sys.platform, None)
        if self.tool_path is None:
            self.tool_path = config["config"].get("tool_path", "")

    def __str__(self):
        if self.args.verbose:
            return """Parser Type: {}
  config file: {}
  input file: {}
  exec root: {}
  tool path: {}
  aggregated stack size: {}
  entry point: {}""".format(
                self.parser_type,
                self.args.configfile,
                self.args.inputfile,
                self.args.exec_root,
                self.tool_path,
                self.get_stack_usage(),
                self.get_entry_point_name(),
            )
        else:
            return "Parser Type: {}\n  aggregated stack size: {}".format(
                self.parser_type, self.get_stack_usage()
            )

    def get_stack_usage(self) -> int:
        """Return the aggregated stack usage result for the top-level entry point."""
        if self.top is None:
            return 0
        else:
            return self.top.aggregated_stack_usage

    def get_entry_point_name(self) -> str:
        """Return the name of the top-level entry point."""
        if self.top is None:
            return "__unknown__"
        else:
            return self.top.entry_point

    def __parse_node__(self, entry_point: str = "main", node_class=Stack_Node) -> Stack_Node:
        """
        Iteratively parse each node in the config file.

        The node_class parameter is provided to allow it to be easily overriden
        by derived class implementations.
        """
        entry_section = None
        if self.config.has_section(entry_point):
            entry_section = self.config[entry_point]

        # create a new node of the desired type (Stack_Node or subclass of it)
        new_node = node_class(entry_point, entry_section)

        if entry_section is not None:
            children_list = entry_section.get("indirect_calls", fallback="").split()
            for c in children_list:
                # recursively create the child nodes
                new_node.add_child(self.__parse_node__(c))

        return new_node

    def parse(self):
        """Parse the config file to create the call tree starting with the entry_point node."""
        # this basic implementation doesn't support multiple entry points
        # but that might be a good extension for some sub class implementations
        entry = self.config["config"].get("entry_point", "main")
        self.top = self.__parse_node__(entry)

        # parse the list of interrupts as independent trees
        interrupt_list = self.config["config"].get("cat1_interrupts", fallback="").split()
        for i in interrupt_list:
            self.interrupts.append(self.__parse_node__(i))

        if self.args.verbose:
            self.evaluate(self.log.verbose)

    def evaluate(self, evaluator, depth_first=True):
        """Recursively iterate the Stack_Nodes and execute the evaluator function."""
        if not depth_first:
            ret = self.top.evaluate(evaluator, depth_first)

        for i in self.interrupts:
            i.evaluate(evaluator, depth_first)

        if not depth_first:
            return ret
        else:
            return self.top.evaluate(evaluator, depth_first)

    def stack_evaluator(self, node: Stack_Node) -> int:
        """Evaluate the stack usage for the given node.

        This is a virtual prototype for an evaluator function suitable to pass
        to Stack_Node.evaluate().

        Derived classes should override this with their implementation to
        actually calculate the stack size for the node passed as an argument.
        The derived class can safely call this base class implementation once
        it has populated node.stack_usage.

        The function should return an integer representing the stack usage for
        this node and the aggregated child nodes.
        """
        if node.stack_usage is None:
            raise NotImplementedError(
                "Missing implementation of stack_evaluator in {}".format(type(self))
            )

        for child in node:
            if child.aggregated_stack_usage > node.aggregated_stack_usage:
                node.aggregated_stack_usage = child.aggregated_stack_usage

        node.aggregated_stack_usage += node.extra_stack + node.stack_usage
        self.log.verbose(node)

        return node.aggregated_stack_usage

    def print_report(self, node: Stack_Node):
        """Write out the detailed stack usage report for the given node.

        This is a prototype for an evaluator function suitable to pass to
        Stack_Node.evaluate().

        Derived classes will likely want to override this with their own
        implementation that provides a more detailed or useful report.
        Derived classes can write output directly to the report file using:
        ```
        print(..., file=self.args.output)
        ```
        """
        output = self.args.output
        print(
            """
Function entry point {}:
    Stack Size (including direct calls)           : {}
    Stack Size including direct and indirect calls: {}
    List of indirect branches:""".format(
                node.entry_point, node.extra_stack + node.stack_usage, node.aggregated_stack_usage
            ),
            file=output,
        )

        # identify largest child
        lchildsize = 0
        for child in node:
            if child.aggregated_stack_usage > lchildsize:
                lchildsize = child.aggregated_stack_usage
        for child in node:
            asterisk = " "
            if (lchildsize > 0) and (child.aggregated_stack_usage == lchildsize):
                asterisk = "*"

            print("      {} {}".format(asterisk, child.entry_point), file=output)

        return node.aggregated_stack_usage
