import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;

public class Phase5DumpVtableCtx extends GhidraScript {
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        Memory mem = currentProgram.getMemory();
        long[] rvas = { 0xb51d14L, 0x818db8L };
        for (long rva : rvas) {
            println(String.format("=== context around rva=0x%x ===", rva));
            // dump 8 qwords before, the entry itself, and 8 qwords after
            long start = rva - 0x40;
            for (int i = 0; i < 17; i++) {
                long off = start + i * 8;
                Address a = currentProgram.getAddressFactory()
                    .getDefaultAddressSpace().getAddress(base + off);
                try {
                    long val = mem.getLong(a);
                    long valRva = val - base;
                    Function fn = currentProgram.getFunctionManager()
                        .getFunctionContaining(a);
                    String tag = (off == rva) ? " <-- FACTORY REF" : "";
                    Function target = currentProgram.getFunctionManager()
                        .getFunctionAt(currentProgram.getAddressFactory()
                            .getDefaultAddressSpace().getAddress(val));
                    String fnName = (target != null) ? "  [" + target.getName() + "]" : "";
                    println(String.format("  [0x%x] = 0x%x (rva=0x%x)%s%s",
                        off, val & 0xFFFFFFFFFFFFFFFFL, valRva & 0xFFFFFFFFFFFFFFFFL, fnName, tag));
                } catch (Exception e) {
                    println(String.format("  [0x%x] = <unreadable>", off));
                }
            }
        }
    }
}
