// Phase 3dd: Read the tail of FUN_14004b250 (lines 1200-1509) to find FUN_1404889d0 call.
// Also try lines 60-200 (missed by phase3y).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3ddInitTail extends GhidraScript {

    static final long RVA_4B250 = 0x4b250L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3dd_init_tail.c"));

        Address addr = currentProgram.getImageBase().add(RVA_4B250);
        Function fn = getFunctionAt(addr);
        DecompileResults res = decomp.decompileFunction(fn, 400, monitor);
        String[] lines = res.getDecompiledFunction().getC().split("\n");
        int total = lines.length;
        pw.println("// Total lines: " + total);

        pw.println("\n// === lines 60-200 ===");
        for (int i = 60; i < Math.min(200, total); i++) pw.println(lines[i]);

        pw.println("\n// === lines 1200-end ===");
        for (int i = 1200; i < total; i++) pw.println(lines[i]);

        pw.close();
        decomp.dispose();
        println("Phase 3dd done");
    }
}
