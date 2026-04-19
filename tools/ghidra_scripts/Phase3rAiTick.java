// Phase 3r: Find the AI/simulation tick called from FUN_14035cb10 (mode slot 3).
// Slot 3 calls:
//   FUN_14044b500(&DAT_140a28c10, mode)  — global AI singleton tick?
//   FUN_1404d9df0(singleton, mode)       — another per-frame update
//   FUN_140288980(player, 1, 0)          — per-player service call
// Also check large LandMode-specific vtable slots (5, 20, 22) for the sim tick.
// And look at FUN_140220ed0 — the signal/event dispatcher fired in slot 4 (event 0x23).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3rAiTick extends GhidraScript {

    // Called from slot-3 with global AI singleton
    static final long RVA_44B500  = 0x44b500L; // FUN_14044b500(singleton, mode)
    // Per-frame update via singleton (from slot 3)
    static final long RVA_D9DF0   = 0x4d9df0L; // FUN_1404d9df0(singleton, mode)
    // Per-player service call (from slot 3)
    static final long RVA_288980  = 0x288980L; // FUN_140288980(player, 1, 0)
    // Signal/event dispatcher fired in slot 4 with event 0x23
    static final long RVA_220ED0  = 0x220ed0L; // FUN_140220ed0 — fire event 0x23

    // LandModeClass mode-specific slots (different from Space)
    static final long RVA_LAND5   = 0x3b7aa0L; // slot 5  FUN_1403b7aa0
    static final long RVA_LAND20  = 0x3ba400L; // slot 20 FUN_1403ba400
    static final long RVA_LAND22  = 0x3bb440L; // slot 22 FUN_1403bb440
    static final long RVA_LAND13  = 0x3b8950L; // slot 13 FUN_1403b8950
    static final long RVA_LAND14  = 0x3bafd0L; // slot 14 FUN_1403bafd0

    // SpaceModeClass mode-specific slots
    static final long RVA_SPACE13 = 0x4d7da0L; // slot 13 FUN_1404d7da0
    static final long RVA_SPACE20 = 0x4d8de0L; // slot 20 FUN_1404d8de0

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3r_ai_tick.c"));

        section(pw, "FUN_14044b500 — global AI singleton tick from slot 3 (KEY CANDIDATE)");
        decompile(decomp, pw, imageBase, RVA_44B500, 400);

        section(pw, "FUN_1404d9df0 — per-frame update from slot 3");
        decompile(decomp, pw, imageBase, RVA_D9DF0, 300);

        section(pw, "FUN_140288980 — per-player service from slot 3");
        decompile(decomp, pw, imageBase, RVA_288980, 400);

        section(pw, "FUN_140220ed0 — signal dispatcher (event 0x23 = unit Think trigger?)");
        decompile(decomp, pw, imageBase, RVA_220ED0, 200);

        section(pw, "LandModeClass slot 5 — FUN_1403b7aa0");
        decompile(decomp, pw, imageBase, RVA_LAND5, 200);

        section(pw, "LandModeClass slot 13 — FUN_1403b8950");
        decompile(decomp, pw, imageBase, RVA_LAND13, 200);

        section(pw, "LandModeClass slot 14 — FUN_1403bafd0");
        decompile(decomp, pw, imageBase, RVA_LAND14, 200);

        section(pw, "LandModeClass slot 20 — FUN_1403ba400");
        decompile(decomp, pw, imageBase, RVA_LAND20, 200);

        section(pw, "LandModeClass slot 22 — FUN_1403bb440");
        decompile(decomp, pw, imageBase, RVA_LAND22, 200);

        section(pw, "SpaceModeClass slot 13 — FUN_1404d7da0");
        decompile(decomp, pw, imageBase, RVA_SPACE13, 200);

        section(pw, "SpaceModeClass slot 20 — FUN_1404d8de0");
        decompile(decomp, pw, imageBase, RVA_SPACE20, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3r output written to logs/phase3r_ai_tick.c");
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
