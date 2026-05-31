//Dump vtable slots: for each RVA arg, read N qword slots and name the target function.
//Usage: just gs DumpVtable 8a6198   (optionally a second arg = slot count, default 32)
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;

public class DumpVtable extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) { println("DumpVtable: need a vtable RVA arg"); return; }
        long base = currentProgram.getImageBase().getOffset();
        Memory mem = currentProgram.getMemory();
        int slots = 32;
        if (args.length > 1) { try { slots = Integer.parseInt(args[1]); } catch (Exception e) {} }
        for (int ai = 0; ai < (args.length > 1 ? 1 : args.length); ai++) {
            String h = args[ai].startsWith("0x") ? args[ai].substring(2) : args[ai];
            long vt = Long.parseLong(h, 16);
            println(String.format("=== vtable rva=0x%x ===", vt));
            for (int i = 0; i < slots; i++) {
                long off = vt + (long)i * 8;
                Address a = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + off);
                try {
                    long val = mem.getLong(a);
                    long valRva = val - base;
                    Function target = currentProgram.getFunctionManager().getFunctionAt(
                        currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(val));
                    String fnName = (target != null) ? "  [" + target.getName() + "]" : "  [no-func]";
                    println(String.format("  slot %2d (+0x%02x): rva=0x%x%s", i, i*8, valRva & 0xFFFFFFFFL, fnName));
                } catch (Exception e) {
                    println(String.format("  slot %2d (+0x%02x): <unreadable>", i, i*8));
                }
            }
        }
    }
}
