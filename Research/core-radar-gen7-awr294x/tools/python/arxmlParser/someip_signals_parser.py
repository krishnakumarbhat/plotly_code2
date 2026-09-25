"""This source code parser is used to generate report for Unimplemented SomeIP Signals."""

import pycparser
from pycparser import c_ast


class SignalParser:
    """parse preprocessed Source code."""

    def __init__(self):
        """Initialize Parser and generate AST."""
        self.ast = None
        self.SIGNAL_UNIMPLEMENTED = "0"
        # list of all signals which are assigned any value
        self.assignedSignals = []
        # list of signals which are assigned with value SIGNAL_UNIMPLEMENTED
        self.notImplementedSignals = []

    def parseAssignedSignals(self, preprocessedfile):
        """Parse source code ast and collect signals which are assigned."""
        self.ast = pycparser.parse_file(preprocessedfile, use_cpp=False)
        for function in filter(lambda x: isinstance(x, c_ast.FuncDef), self.ast.ext):
            self.__parseBlocks(function.body.block_items)

    def getSignalStatus(self, signaName):
        """Return Status of given signal."""
        if signaName in self.notImplementedSignals:
            return "Not Implemented"
        elif signaName in self.assignedSignals:
            return "Implemented"
        else:
            return "Not Assigned"

    def getAssignedsignals(self):
        """Return list of signals Assigned in source code."""
        return self.assignedSignals

    def __parseBlocks(self, block_items):
        """Parse block function block to list signal assigned signals."""
        for expression in block_items:
            if isinstance(expression, c_ast.Assignment):
                lvalue = expression.lvalue
                if isinstance(lvalue, c_ast.StructRef):
                    rvalue = expression.rvalue
                    self.assignedSignals.append(lvalue.field.name)
                    if self.__isRvalueUnimplemented(self.__getCastValue(rvalue)):
                        self.notImplementedSignals.append(lvalue.field.name)
            if isinstance(expression, c_ast.For):
                self.__parseBlocks(expression.stmt.block_items)
            if isinstance(expression, c_ast.While):
                self.__parseBlocks(expression.stmt.block_items)

    def __getCastValue(self, cast):
        """Resolve Casting of expression and return token."""
        if isinstance(cast, c_ast.Cast):
            self.__getCastValue(cast.expr)
        return cast

    def __isRvalueUnimplemented(self, rvalue):
        """Check if value is assigned with unimplemented signal value."""
        result = isinstance(rvalue, c_ast.Constant) and rvalue.value == self.SIGNAL_UNIMPLEMENTED
        return result
