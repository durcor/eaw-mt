# Ghidra post-analysis script: dump all defined strings to stdout
from ghidra.program.model.data import StringDataType, UnicodeDataType
from ghidra.program.model.listing import Data

listing = currentProgram.getListing()
data_iter = listing.getDefinedData(True)

count = 0
while data_iter.hasNext():
    d = data_iter.next()
    dt = d.getDataType()
    if isinstance(dt, (StringDataType, UnicodeDataType)):
        try:
            val = d.getValue()
            if val and len(str(val)) > 3:
                addr = d.getAddress()
                print("0x{}\t{}".format(addr, repr(str(val))))
                count += 1
        except Exception:
            pass

print("# Total strings: {}".format(count), file=java.lang.System.err)
