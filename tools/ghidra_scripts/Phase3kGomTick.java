// Phase 3k: Find the GOM simulation tick.
// Slot 75 = Remove_Game_Object (accesses param_1+0x1e0 stride 0x18 2D vector).
// Slot 73 (FUN_14035eb40) is likely Add_Game_Object — immediately above Remove.
// Base slot 1 (FUN_14035e560) = possible base-class Think, called by galactic slot 1.
// Base slot 2 (FUN_140365300) = called by galactic slot 2 (cleanup).
// FUN_1404b12f0 = iterated in galactic slot 2 loop.
// FUN_140305c40 = large unconditional scene sub-call (prime sim-tick candidate).
// FUN_1402fd330 = flag-gated scene sub-call (2461 bytes, partially read).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3kGomTick extends GhidraScript {

    // GOM management methods (base class, slot 73+75 neighborhood)
    static final long RVA_SLOT73_ADD    = 0x35eb40L; // FUN_14035eb40  likely Add_Game_Object
    static final long RVA_SLOT77       = 0x365150L; // FUN_140365150  base-only, unknown

    // Base class slot 1 Think (called from galactic slot 1)
    static final long RVA_BASE_THINK   = 0x35e560L; // FUN_14035e560

    // Base class slot 2 (called from galactic slot 2 at entry)
    static final long RVA_BASE_SLOT2   = 0x365300L; // FUN_140365300

    // Iterated in galactic slot 2 loop (DAT_140b31ae8 list)
    static final long RVA_B12F0        = 0x4b12f0L; // FUN_1404b12f0

    // Large unconditional scene sub-call — prime sim-tick candidate
    static final long RVA_305C40       = 0x305c40L; // FUN_140305c40

    // Flag-gated scene sub-call (partially read in phase3h)
    static final long RVA_FD330        = 0x2fd330L; // FUN_1402fd330

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3k_gom_tick.c"));

        section(pw, "SLOT 73 — FUN_14035eb40 (likely Add_Game_Object)");
        decompile(decomp, pw, imageBase, RVA_SLOT73_ADD, 300);

        section(pw, "SLOT 77 — FUN_140365150 (base-only, unknown)");
        decompile(decomp, pw, imageBase, RVA_SLOT77, 300);

        section(pw, "BASE SLOT 1 THINK — FUN_14035e560");
        decompile(decomp, pw, imageBase, RVA_BASE_THINK, 400);

        section(pw, "BASE SLOT 2 — FUN_140365300 (called from galactic slot 2)");
        decompile(decomp, pw, imageBase, RVA_BASE_SLOT2, 200);

        section(pw, "FUN_1404b12f0 (iterated in galactic slot 2 cleanup loop)");
        decompile(decomp, pw, imageBase, RVA_B12F0, 300);

        // The big two: unconditional and flag-gated scene sub-calls
        section(pw, "FUN_140305c40 — UNCONDITIONAL SCENE SUB-CALL (primary sim-tick candidate)");
        decompile(decomp, pw, imageBase, RVA_305C40, 5000);

        section(pw, "FUN_1402fd330 — FLAG-GATED SCENE SUB-CALL (2461 bytes)");
        decompile(decomp, pw, imageBase, RVA_FD330, 3000);

        pw.close();
        decomp.dispose();
        println("Phase 3k output written to logs/phase3k_gom_tick.c");
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

        DecompileResults res = decomp.decompileFunction(fn, 180, monitor);
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
