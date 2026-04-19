// Phase 3: GOM tick hunt + galactic/space/land mode sub-tick mapping.
// Decompiles mode-1 galactic map ticks (GOM candidate), mode-2/3 battle ticks,
// LoadThread entry, and timer subsystem candidates.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;
import java.util.*;

public class Phase3GomAndModes extends GhidraScript {

    // Mode-1 (galactic map) sub-ticks dispatched from FUN_14030c3b0
    static final long[] GALACTIC_MAP = {
        0x3e8a80L,  // FUN_1403e8a80
        0x3e83a0L,  // FUN_1403e83a0
        0x3145c0L,  // FUN_1403145c0
        0x3edd00L,  // FUN_1403edd00
        0x3e5aa0L,  // FUN_1403e5aa0
        0x310e40L,  // FUN_140310e40
        0x3e4cb0L,  // FUN_1403e4cb0
        0x3e6ff0L,  // FUN_1403e6ff0
        0x3e6630L,  // FUN_1403e6630
        0x3e79b0L,  // FUN_1403e79b0
    };

    // Mode-2 (space battle) sub-ticks
    static final long[] SPACE_BATTLE = {
        0x317480L,  // FUN_140317480 — shared with land
        0x317d00L,  // FUN_140317d00 — space-only
        0x316260L,  // FUN_140316260 — shared ending step
    };

    // Mode-3 (land battle) sub-ticks
    static final long[] LAND_BATTLE = {
        0x312570L,  // FUN_140312570 — land-only
        // 0x317480 and 0x316260 shared with space; already in SPACE_BATTLE
    };

    // Timer subsystem candidates
    static final long[] TIMER = {
        0x215b90L,  // FUN_140215b90 — frame profiler or timer tick?
        0x1a7370L,  // QPC timer cluster start
        0x222e20L,  // QPC timer cluster
        0x222fa0L,  // QPC timer cluster end
    };

    // LoadThread entry
    static final long[] LOAD_THREAD = {
        0x8df00L,   // LoadThread entry point (FUN_14008df00)
    };

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3_gom_modes.c"));

        section(pw, "GALACTIC MAP SUB-TICKS (MODE-1) — GOM CANDIDATES");
        for (long rva : GALACTIC_MAP) {
            decompile(decomp, pw, imageBase, rva, 3000);
        }

        section(pw, "SPACE BATTLE SUB-TICKS (MODE-2)");
        for (long rva : SPACE_BATTLE) {
            decompile(decomp, pw, imageBase, rva, 2048);
        }

        section(pw, "LAND BATTLE SUB-TICKS (MODE-3)");
        for (long rva : LAND_BATTLE) {
            decompile(decomp, pw, imageBase, rva, 2048);
        }

        section(pw, "TIMER SUBSYSTEM CANDIDATES");
        for (long rva : TIMER) {
            decompile(decomp, pw, imageBase, rva, 1024);
        }

        section(pw, "LOAD THREAD ENTRY POINT");
        for (long rva : LOAD_THREAD) {
            decompile(decomp, pw, imageBase, rva, 3000);
        }

        pw.close();
        decomp.dispose();
        println("Phase 3 output written to logs/phase3_gom_modes.c");
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
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(rva) +
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
