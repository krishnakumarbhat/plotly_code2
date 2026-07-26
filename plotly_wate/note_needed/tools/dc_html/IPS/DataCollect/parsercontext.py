from IPS.DataCollect.icollectdata import IDataCollect

class ParserContext:
    def __init__(self, strategy: IDataCollect):
        self.strategy = strategy

    def execute(self, infile, outfile):
        self.strategy.collect_data(infile, outfile)
