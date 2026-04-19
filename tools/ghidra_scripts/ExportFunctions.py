# Ghidra post-analysis script: dump all functions with RVA and name
from ghidra.program.model.symbol import SourceType

IMAGE_BASE = currentProgram.getImageBase().getOffset()
fm = currentProgram.getFunctionManager()
funcs = fm.getFunctions(True)

count = 0
while funcs.hasNext():
    fn = funcs.next()
    va = fn.getEntryPoint().getOffset()
    rva = va - IMAGE_BASE
    name = fn.getName()
    source = fn.getSymbol().getSource()
    print("0x{:08X}\t{}\t{}".format(rva, name, source))
    count += 1

print("# Total functions: {}".format(count), file=java.lang.System.err)
