// Phase 3j: Decompile GalacticModeClass-specific vtable overrides.
// From vtable @ RVA 0x8762c0 we know the per-class overrides not yet analyzed.
// Slot 2 (FUN_1404b3350, +0x1160 bytes) is prime candidate for Think/Service.
// Also checking slots 1,5,6,9-13,18,21-24,40-44 for simulation dispatch.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3jGalacticMethods extends GhidraScript {

    // GalacticModeClass vtable @ 0x8762c0 — undecompiled override slots
    // Slot 2: prime candidate for Think/Update (large function)
    static final long RVA_SLOT2   = 0x4b3350L; // FUN_1404b3350
    static final long RVA_SLOT1   = 0x4b21f0L; // FUN_1404b21f0
    static final long RVA_SLOT5   = 0x4b1fb0L; // FUN_1404b1fb0
    static final long RVA_SLOT6   = 0x3b5a40L; // FUN_1403b5a40
    static final long RVA_SLOT9   = 0x4b2360L; // FUN_1404b2360
    static final long RVA_SLOT10  = 0x4b25f0L; // FUN_1404b25f0
    static final long RVA_SLOT11  = 0x4b2c70L; // FUN_1404b2c70
    static final long RVA_SLOT12  = 0x4b2d60L; // FUN_1404b2d60
    static final long RVA_SLOT13  = 0x4b23c0L; // FUN_1404b23c0
    static final long RVA_SLOT18  = 0x360970L; // FUN_140360970
    static final long RVA_SLOT21  = 0x4b2bc0L; // FUN_1404b2bc0
    static final long RVA_SLOT22  = 0x4b2ef0L; // FUN_1404b2ef0
    static final long RVA_SLOT23  = 0x4b2c00L; // FUN_1404b2c00
    static final long RVA_SLOT24  = 0x4b32e0L; // FUN_1404b32e0
    static final long RVA_SLOT40  = 0x47940L;  // FUN_140047940
    static final long RVA_SLOT41  = 0x4b2330L; // FUN_1404b2330
    static final long RVA_SLOT42  = 0x4b2020L; // FUN_1404b2020
    static final long RVA_SLOT43  = 0x4b1600L; // FUN_1404b1600
    static final long RVA_SLOT44  = 0x361470L; // FUN_140361470
    static final long RVA_SLOT45  = 0x3613a0L; // FUN_1403613a0
    static final long RVA_SLOT56  = 0x364ef0L; // FUN_140364ef0
    static final long RVA_SLOT74  = 0x365540L; // FUN_140365540
    static final long RVA_SLOT75  = 0x35cf20L; // FUN_14035cf20
    static final long RVA_SLOT78  = 0x364f30L; // FUN_140364f30

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3j_galactic_methods.c"));

        // Priority 1: Slot 2 — most likely Think/Update (large function)
        section(pw, "SLOT 2 — FUN_1404b3350 (prime Think candidate)");
        decompile(decomp, pw, imageBase, RVA_SLOT2, 600);

        // Priority 2: Slot 1 — destructor variant or second-frame method
        section(pw, "SLOT 1 — FUN_1404b21f0");
        decompile(decomp, pw, imageBase, RVA_SLOT1, 300);

        // Priority 3: Slot 43 — GalacticModeClass-specific early init area
        section(pw, "SLOT 43 — FUN_1404b1600");
        decompile(decomp, pw, imageBase, RVA_SLOT43, 300);

        // Slots 9-13: GalacticMode-specific state methods
        section(pw, "SLOT 9 — FUN_1404b2360");
        decompile(decomp, pw, imageBase, RVA_SLOT9, 200);

        section(pw, "SLOT 10 — FUN_1404b25f0");
        decompile(decomp, pw, imageBase, RVA_SLOT10, 200);

        section(pw, "SLOT 11 — FUN_1404b2c70");
        decompile(decomp, pw, imageBase, RVA_SLOT11, 200);

        section(pw, "SLOT 12 — FUN_1404b2d60");
        decompile(decomp, pw, imageBase, RVA_SLOT12, 200);

        section(pw, "SLOT 13 — FUN_1404b23c0");
        decompile(decomp, pw, imageBase, RVA_SLOT13, 200);

        // Slots 18,21-24: middle vtable range
        section(pw, "SLOT 18 — FUN_140360970");
        decompile(decomp, pw, imageBase, RVA_SLOT18, 200);

        section(pw, "SLOT 21 — FUN_1404b2bc0");
        decompile(decomp, pw, imageBase, RVA_SLOT21, 200);

        section(pw, "SLOT 22 — FUN_1404b2ef0");
        decompile(decomp, pw, imageBase, RVA_SLOT22, 200);

        section(pw, "SLOT 23 — FUN_1404b2c00");
        decompile(decomp, pw, imageBase, RVA_SLOT23, 200);

        section(pw, "SLOT 24 — FUN_1404b32e0");
        decompile(decomp, pw, imageBase, RVA_SLOT24, 200);

        // Slot 5,6: early override range
        section(pw, "SLOT 5 — FUN_1404b1fb0");
        decompile(decomp, pw, imageBase, RVA_SLOT5, 200);

        section(pw, "SLOT 6 — FUN_1403b5a40");
        decompile(decomp, pw, imageBase, RVA_SLOT6, 200);

        // Slots 40-42: late galactic overrides
        section(pw, "SLOT 40 — FUN_140047940");
        decompile(decomp, pw, imageBase, RVA_SLOT40, 200);

        section(pw, "SLOT 41 — FUN_1404b2330");
        decompile(decomp, pw, imageBase, RVA_SLOT41, 200);

        section(pw, "SLOT 42 — FUN_1404b2020");
        decompile(decomp, pw, imageBase, RVA_SLOT42, 200);

        // Slots 44,45,56,74,75,78: remaining unique overrides
        section(pw, "SLOT 44 — FUN_140361470");
        decompile(decomp, pw, imageBase, RVA_SLOT44, 300);

        section(pw, "SLOT 45 — FUN_1403613a0");
        decompile(decomp, pw, imageBase, RVA_SLOT45, 300);

        section(pw, "SLOT 56 — FUN_140364ef0");
        decompile(decomp, pw, imageBase, RVA_SLOT56, 200);

        section(pw, "SLOT 74 — FUN_140365540");
        decompile(decomp, pw, imageBase, RVA_SLOT74, 200);

        section(pw, "SLOT 75 — FUN_14035cf20");
        decompile(decomp, pw, imageBase, RVA_SLOT75, 200);

        section(pw, "SLOT 78 — FUN_140364f30");
        decompile(decomp, pw, imageBase, RVA_SLOT78, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3j output written to logs/phase3j_galactic_methods.c");
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
