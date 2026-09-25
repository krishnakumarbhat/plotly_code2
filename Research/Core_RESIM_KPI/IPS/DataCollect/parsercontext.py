"""
File Name: parsercontext.py
Author: Bharanidharan Subramani
Email : Bharanidharan.s@aptiv.com
Description:
This class hold address/object of Data collect class,
and it calls collect data strategy/function of respective class
on invocation of execute method
"""

from IPS.DataCollect.icollectdata import IDataCollect


class ParserContext:
    def __init__(self, data_collect_strategy: IDataCollect):
        self.parse_strategy = data_collect_strategy

    def execute(self, infile, outfile):
        self.parse_strategy.collect_data(infile, outfile)
