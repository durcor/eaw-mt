// Phase 3e: Decompile tick-slot initializers and game-object dispatch.
// FUN_140322780 and FUN_1403207c0 initialize DAT_140b2b3a8 / DAT_140b2b2f0 (the
// _guard_check_icall indirect-call targets in WinMain's game-active block).
// FUN_140265280 is called from FUN_140305890 (scene manager post-call) with individual
// game objects — likely the per-object tick dispatch.
// Also decompile FUN_140321830, FUN_140321170, FUN_140323660 (tick slot helpers),
// and the cluster of 0x10xxxx functions that appear in the icall-slot DATA xrefs.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3eTickInit extends GhidraScript {

    // Tick-slot initializers
    static final long RVA_SLOT_INIT_A  = 0x322780L;  // FUN_140322780 — init DAT_140b2b3a8
    static final long RVA_SLOT_INIT_B  = 0x3207c0L;  // FUN_1403207c0 — init DAT_140b2b2f0
    static final long RVA_SLOT_SET     = 0x321830L;  // FUN_140321830 — helper (flag param)
    static final long RVA_SLOT_START   = 0x321170L;  // FUN_140321170 — start tick with object
    static final long RVA_SLOT_CLEAR   = 0x323660L;  // FUN_140323660 — clear DAT_140b2b3b0
    // Per-object dispatch from FUN_140305890
    static final long RVA_OBJ_DISPATCH = 0x265280L;  // FUN_140265280 — per-object tick?
    // 0x10xxxx cluster — appear in both icall-slot DATA xrefs
    static final long RVA_10_A = 0x105190L;  // FUN_140105190
    static final long RVA_10_B = 0x1053c0L;  // FUN_1401053c0
    static final long RVA_10_C = 0x105d30L;  // FUN_140105d30
    static final long RVA_10_D = 0x107900L;  // FUN_140107900
    static final long RVA_10_E = 0x1089e0L;  // FUN_1401089e0
    static final long RVA_10_F = 0x1098e0L;  // FUN_1401098e0
    // Also check FUN_1402b09b0 called in Start_Game with (DAT_140b15418+0x18, map_name)
    static final long RVA_B09B0 = 0x2b09b0L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3e_tick_init.c"));

        section(pw, "TICK-SLOT INITIALIZERS");
        decompile(decomp, pw, imageBase, RVA_SLOT_INIT_A, 300);
        decompile(decomp, pw, imageBase, RVA_SLOT_INIT_B, 300);
        decompile(decomp, pw, imageBase, RVA_SLOT_SET, 300);
        decompile(decomp, pw, imageBase, RVA_SLOT_START, 300);
        decompile(decomp, pw, imageBase, RVA_SLOT_CLEAR, 300);

        section(pw, "PER-OBJECT DISPATCH (FUN_140265280)");
        decompile(decomp, pw, imageBase, RVA_OBJ_DISPATCH, 2000);

        section(pw, "0x10xxxx CLUSTER (appear in icall-slot xrefs)");
        decompile(decomp, pw, imageBase, RVA_10_A, 1024);
        decompile(decomp, pw, imageBase, RVA_10_B, 1024);
        decompile(decomp, pw, imageBase, RVA_10_C, 1024);
        decompile(decomp, pw, imageBase, RVA_10_D, 1024);
        decompile(decomp, pw, imageBase, RVA_10_E, 1024);
        decompile(decomp, pw, imageBase, RVA_10_F, 1024);

        section(pw, "Start_Game helper (FUN_1402b09b0)");
        decompile(decomp, pw, imageBase, RVA_B09B0, 500);

        pw.close();
        decomp.dispose();
        println("Phase 3e output written to logs/phase3e_tick_init.c");
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }

    private void decompile(DecompInterface decomp, PrintWriter pw,
                            long imageBase, long rva, int maxLines) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function at RVA 0x" + Long.toHexString(rva) + "\n"); return;
        }
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");

        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage() + "\n"); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        if (lines.length > maxLines) {
            for (int i = 0; i < maxLines; i++) pw.println(lines[i]);
            pw.println("// ... [truncated at " + maxLines + " lines, total " + lines.length + "]");
        } else {
            pw.println(code);
        }
        pw.println();
    }
}
