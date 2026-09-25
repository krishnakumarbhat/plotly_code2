"""A2l Reader.

This library reads a given elf file and returns addresses for a given variable name.
"""

from elftools.elf.elffile import ELFFile
import os
import errno


class A2lReader:
    """Al2 reader structure."""

    def __init__(self, elfFile=None) -> None:
        """Initialize the A2l Reader."""
        self.elfFile = elfFile
        self.symbols = {}
        if self.elfFile:
            if not os.path.isfile(self.elfFile):
                raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), elfFile)
            else:
                self.elf = ELFFile(open(self.elfFile, "rb"))
                self._dump_elf()
        else:
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), "")

    def _dump_elf(self):
        """
        Dump the symbol table of an ELF file.

        Needs pyelftools (https://github.com/eliben/pyelftools)
        """
        for sec in self.elf.iter_sections():
            if sec["sh_type"] == "SHT_SYMTAB":
                symbols = sorted(sec.iter_symbols(), key=lambda sym: sym.name)
                # print("    symbols:")
                for sym in symbols:
                    if not sym.name:
                        continue
                    self.symbols[sym.name] = [sym["st_value"], sym["st_size"]]

        print()

    def getAddress(self, var):
        """Get the address of the given variable."""
        addr = 0
        size = 0
        try:
            addr, size = self.symbols[var]

            print("A2L: %s = 0x%X, %d" % (var, addr, size))

        except Exception as e:
            print("Variable does not exist - " + str(e))
        return addr, size
