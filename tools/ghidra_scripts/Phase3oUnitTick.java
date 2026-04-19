// Phase 3o: Find the per-unit Think loop via the mode slot-4 subsystem tickers.
// FUN_14035cc10 (slot 4 of ALL modes) ticks:
//   param_1[7] via FUN_140493d50(param_1[7], mode)
//   param_1[8] via FUN_1405284b0(param_1[8], mode)
// For land mode, param_1[7] is alloc'd by FUN_1404922d0 and param_1[8] by FUN_140527f30.
// Also: FUN_1402af130 registers players with GOM — peek to understand GOM→mode linkage.
// FUN_1403a8380 = per-object destructor (called from GOM dtor), reveals object structure.
// FUN_1402a6a00 = Remove_Object (called from GOM dtor).
// The per-unit Think loop is likely inside FUN_140493d50 or one of its callees.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3oUnitTick extends GhidraScript {

    static final long RVA_493D50  = 0x493d50L; // FUN_140493d50 (ticks param_1[7])
    static final long RVA_5284B0  = 0x5284b0L; // FUN_1405284b0 (ticks param_1[8])
    static final long RVA_A8380   = 0x3a8380L; // FUN_1403a8380 (per-object dtor)
    static final long RVA_A6A00   = 0x2a6a00L; // FUN_1402a6a00 (remove object)
    static final long RVA_AF130   = 0x2af130L; // FUN_1402af130 (register players with GOM)
    static final long RVA_922D0   = 0x4922d0L; // FUN_1404922d0 (allocs land subsystem 7)
    static final long RVA_527F30  = 0x527f30L; // FUN_140527f30 (allocs land subsystem 8)
    // Space mode subsystem (param_1[6] for space)
    static final long RVA_532B70  = 0x532b70L; // FUN_140532b70 (space battle AI/renderer)
    static final long RVA_534BD0  = 0x534bd0L; // FUN_140534bd0 (land battle AI/renderer)

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3o_unit_tick.c"));

        section(pw, "FUN_140493d50 — tick subsystem param_1[7] (UNIT THINK CANDIDATE)");
        decompile(decomp, pw, imageBase, RVA_493D50, 600);

        section(pw, "FUN_1405284b0 — tick subsystem param_1[8]");
        decompile(decomp, pw, imageBase, RVA_5284B0, 400);

        section(pw, "FUN_1403a8380 — per-object destructor (object structure reference)");
        decompile(decomp, pw, imageBase, RVA_A8380, 200);

        section(pw, "FUN_1402a6a00 — Remove_Object (remove from GOM ReferenceList)");
        decompile(decomp, pw, imageBase, RVA_A6A00, 200);

        section(pw, "FUN_1402af130 — register players with GOM");
        decompile(decomp, pw, imageBase, RVA_AF130, 200);

        section(pw, "FUN_1404922d0 — alloc land battle subsystem 7");
        decompile(decomp, pw, imageBase, RVA_922D0, 200);

        section(pw, "FUN_140527f30 — alloc land battle subsystem 8");
        decompile(decomp, pw, imageBase, RVA_527F30, 200);

        section(pw, "FUN_140532b70 — space battle AI/render system (param_1[6] for space)");
        decompile(decomp, pw, imageBase, RVA_532B70, 200);

        section(pw, "FUN_140534bd0 — land battle AI/render system (param_1[6] for land)");
        decompile(decomp, pw, imageBase, RVA_534BD0, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3o output written to logs/phase3o_unit_tick.c");
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
