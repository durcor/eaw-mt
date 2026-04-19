// Phase 3h: Find the simulation tick via mode base class and remaining frame calls.
// FUN_14035a5e0 is called first in ALL three mode constructors (base class init).
// FUN_1402fd330 is the flag-gated scene sub-call (when modes are active).
// FUN_140301750 and FUN_14025ca30 are frame-end calls before render.
// FUN_1403b03e0 runs when game active and players are loaded.
// Also check FUN_140207e50 (player registry sub-tick).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3hBaseMode extends GhidraScript {

    // Base mode class constructor — sets vtable for base GameModeClass
    static final long RVA_BASE_CTOR  = 0x35a5e0L;  // FUN_14035a5e0

    // Flag-gated scene sub-call (runs when modes are active)
    static final long RVA_FD330      = 0x2fd330L;  // FUN_1402fd330

    // Frame-end calls (before render)
    static final long RVA_FE_301750  = 0x301750L;  // FUN_140301750
    static final long RVA_FE_25CA30  = 0x25ca30L;  // FUN_14025ca30

    // Called from FUN_1403b08c0 when game active + players loaded
    static final long RVA_B03E0      = 0x3b03e0L;  // FUN_1403b03e0

    // Player registry sub-tick
    static final long RVA_207E50     = 0x207e50L;  // FUN_140207e50

    // Scene unconditional sub-call (already in phase3b, key candidate)
    static final long RVA_305C40     = 0x305c40L;  // FUN_140305c40

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3h_base_mode.c"));

        section(pw, "BASE MODE CTOR — FUN_14035a5e0");
        decompile(decomp, pw, imageBase, RVA_BASE_CTOR, 600);

        section(pw, "SCENE FLAG-GATED SUB-CALL — FUN_1402fd330");
        decompile(decomp, pw, imageBase, RVA_FD330, 3000);

        section(pw, "FRAME-END: FUN_140301750");
        decompile(decomp, pw, imageBase, RVA_FE_301750, 400);

        section(pw, "FRAME-END: FUN_14025ca30");
        decompile(decomp, pw, imageBase, RVA_FE_25CA30, 400);

        section(pw, "COMMAND QUEUE SUB (FUN_1403b03e0)");
        decompile(decomp, pw, imageBase, RVA_B03E0, 400);

        section(pw, "PLAYER REGISTRY SUB-TICK (FUN_140207e50)");
        decompile(decomp, pw, imageBase, RVA_207E50, 400);

        section(pw, "SCENE UNCONDITIONAL SUB-CALL — FUN_140305c40");
        decompile(decomp, pw, imageBase, RVA_305C40, 4000);

        pw.close();
        decomp.dispose();
        println("Phase 3h output written to logs/phase3h_base_mode.c");
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
