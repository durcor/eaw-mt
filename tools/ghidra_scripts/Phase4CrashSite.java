// Phase4CrashSite.java
// Disassemble 64 bytes around RVA 0x180e80 (FUN_140180dc0 + 0xC0, the crash site).
// Also dump the full decompile of FUN_14013d820 (first callee in the flush body).
// Output: logs/phase4_crash_site.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase4CrashSite extends GhidraScript {

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase4_crash_site.txt"));
        long base = currentProgram.getImageBase().getOffset();
        Listing listing = currentProgram.getListing();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        // 1. Disassemble 80 bytes around the crash site (+0xB0 to +0xD0 from FUN_140180dc0)
        long flushBodyRva = 0x180dc0L;
        out.printf("// FUN_140180dc0 at RVA 0x%x\n", flushBodyRva);
        out.printf("// Disassembly from +0xA0 to +0xE0 (crash is at +0xC0 and +0xC9)\n\n");

        Address startAddr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + flushBodyRva + 0xA0L);
        Address endAddr   = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + flushBodyRva + 0xE0L);

        InstructionIterator instrs = listing.getInstructions(startAddr, true);
        while (instrs.hasNext()) {
            Instruction inst = instrs.next();
            if (inst.getAddress().compareTo(endAddr) > 0) break;
            long rva = inst.getAddress().getOffset() - base;
            out.printf("  +0x%03x  RVA=0x%x  %s\n",
                rva - flushBodyRva, rva, inst.toString());
        }

        // 2. Decompile FUN_14013d820 (first call in flush body)
        out.printf("\n\n// FUN_14013d820 decompile (first callee in FUN_140180dc0)\n\n");
        Address fn2addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + 0x13d820L);
        Function fn2 = getFunctionAt(fn2addr);
        if (fn2 == null) {
            out.println("// no function at 0x13d820");
        } else {
            out.printf("// Size: %d bytes\n", fn2.getBody().getNumAddresses());
            DecompileResults res = decomp.decompileFunction(fn2, 60, monitor);
            if (res != null && res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed");
            }
        }

        // 3. Also dump what's at driver+0x6c2f0 offsets in the decompile output
        // (looking for which pass list is being accessed at +0xC0)
        out.printf("\n\n// FUN_140180dc0 decompile lines 40-80 (loop entry area)\n\n");
        Address fn1addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + flushBodyRva);
        Function fn1 = getFunctionAt(fn1addr);
        if (fn1 != null) {
            DecompileResults res = decomp.decompileFunction(fn1, 120, monitor);
            if (res != null && res.decompileCompleted()) {
                String[] lines = res.getDecompiledFunction().getC().split("\n");
                for (int i = 39; i < Math.min(lines.length, 80); i++)
                    out.println(lines[i]);
            }
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase4_crash_site.txt");
    }
}
