//Decompile specific candidate functions for game loop analysis
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import java.util.*;

public class DecompileCandidates extends GhidraScript {

    // RVAs of interest from game_loop_candidates.txt analysis
    static final long[] CANDIDATES = {
        0x14fa0L,    // "Main Thread" naming — startup/thread creation
        0x8df00L,    // "LoadThread" creation
        0x1a7370L,   // Timer init (calls QPC + QPF)
        0x222e20L,   // Timer tick cluster #1
        0x222e90L,   // Timer tick cluster #2
        0x222ed0L,   // Timer tick cluster #3
        0x76ab1cL,   // Near entry point, calls QPC
        0x224b90L,   // AI sleep loop (Sleep called 10x)
        0x5d990L,    // Sleep caller — unknown
        0xc9900L,    // Sleep caller x2 — possible worker thread loop
        0x8e9a0L,    // Sleep caller — unknown
    };

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/decompiled_candidates.c"));

        for (long rva : CANDIDATES) {
            Address addr = currentProgram.getImageBase().add(rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) fn = getFunctionContaining(addr);
            if (fn == null) {
                pw.println("// FUN_0x" + Long.toHexString(rva) + " — no function found at this RVA\n");
                continue;
            }

            pw.println("// ============================================================");
            pw.println("// RVA: 0x" + Long.toHexString(rva)
                + "  Size: " + fn.getBody().getNumAddresses() + " bytes");
            pw.println("// ============================================================");

            DecompileResults res = decomp.decompileFunction(fn, 60, monitor);
            if (res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
            } else {
                pw.println("// DECOMPILE FAILED: " + res.getErrorMessage());
            }
            pw.println();
        }

        pw.close();
        decomp.dispose();
        println("Written to logs/decompiled_candidates.c");
    }
}
