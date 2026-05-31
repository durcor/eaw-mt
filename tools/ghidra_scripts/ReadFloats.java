//Read float (and raw dword) values at the given RVAs. Usage: just gs ReadFloats 8007dc 8007d4 ...
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryAccessException;

public class ReadFloats extends GhidraScript {
    @Override
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        String[] args = getScriptArgs();
        for (String a : args) {
            long rva = Long.parseLong(a.trim(), 16);
            Address addr = toAddr(base + rva);
            try {
                int bits = getInt(addr);               // little-endian dword
                float f = Float.intBitsToFloat(bits);
                println(String.format("DAT_%08x  raw=0x%08x  float=%.9g", rva, bits, f));
            } catch (MemoryAccessException e) {
                println(String.format("DAT_%08x  <unreadable: %s>", rva, e.getMessage()));
            }
        }
    }
}
