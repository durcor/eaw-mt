// Phase 3p: Follow the per-frame battle tick from FUN_14035cc10 (slot 4 of all modes).
// FUN_14035cc10 calls:
//   (A) FUN_1403c05d0(param_1[0x3e])  — when battle active; mode+0x1f0 subsystem UNKNOWN
//   (B) FUN_1404d9cd0(singleton, param_1) — every frame via service singleton
//   (C) FUN_14035b2e0() — from slot 6 before dispatching to slot 41
// Slot 6 (FUN_1403b5a40) calls FUN_14035b2e0 then (*param_1+0x148)(param_1) = slot 41.
// The GOM destructor FUN_14029c800 (1971 bytes) iterates gom+0x38 list; its iteration
// pattern will let us recognize the Think loop elsewhere.
// Also check slot 41 of SpaceModeClass (RVA 0x4d7ce0) and LandModeClass (RVA 0x3b86c0).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3pSimTick extends GhidraScript {

    // From FUN_14035cc10: the battle-active subsystem tick
    static final long RVA_C05D0   = 0x3c05d0L; // FUN_1403c05d0 — param_1[0x3e] tick
    // Service tick called every frame
    static final long RVA_D9CD0   = 0x4d9cd0L; // FUN_1404d9cd0 — singleton service tick
    // Slot 6 helper
    static final long RVA_5B2E0   = 0x35b2e0L; // FUN_14035b2e0 — called before slot 41

    // Mode vtable slot 41 overrides (dispatch from slot 6)
    static final long RVA_SPACE41 = 0x4d7ce0L; // SpaceModeClass slot 41
    static final long RVA_LAND41  = 0x3b86c0L; // LandModeClass slot 41

    // Base class mode initializer — look for who sets param_1[0x3e] = mode+0x1f0
    static final long RVA_BASE_INIT = 0x35e560L; // FUN_14035e560 (base mode init)

    // GOM destructor for comparison (iteration pattern reference)
    static final long RVA_GOM_DTOR = 0x29c800L; // FUN_14029c800 (1971 bytes)

    // From SpaceModeClass vtable, slot 1 (large init) for context
    static final long RVA_SPACE_INIT = 0x4d7820L; // FUN_1404d7820 (1208 bytes) — space init

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3p_sim_tick.c"));

        section(pw, "FUN_1403c05d0 — tick of mode+0x1f0 subsystem (BATTLE-ACTIVE, MAIN CANDIDATE)");
        decompile(decomp, pw, imageBase, RVA_C05D0, 600);

        section(pw, "FUN_1404d9cd0 — singleton service tick (every frame)");
        decompile(decomp, pw, imageBase, RVA_D9CD0, 300);

        section(pw, "FUN_14035b2e0 — slot-6 helper before slot-41 dispatch");
        decompile(decomp, pw, imageBase, RVA_5B2E0, 200);

        section(pw, "SpaceModeClass slot 41 — FUN_1404d7ce0");
        decompile(decomp, pw, imageBase, RVA_SPACE41, 300);

        section(pw, "LandModeClass slot 41 — FUN_1403b86c0");
        decompile(decomp, pw, imageBase, RVA_LAND41, 300);

        section(pw, "FUN_14035e560 — base mode initializer (who sets mode+0x1f0?)");
        decompile(decomp, pw, imageBase, RVA_BASE_INIT, 400);

        section(pw, "FUN_14029c800 — GOM destructor (gom+0x38 iteration reference)");
        decompile(decomp, pw, imageBase, RVA_GOM_DTOR, 400);

        pw.close();
        decomp.dispose();
        println("Phase 3p output written to logs/phase3p_sim_tick.c");
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
