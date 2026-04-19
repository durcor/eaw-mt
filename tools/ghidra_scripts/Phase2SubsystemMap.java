//Phase 2: Map subsystem call chains from main loop. Renderer call chain,
//game-active callees, unconditional per-frame callees, scene manager.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase2SubsystemMap extends GhidraScript {

    // Game-active update callees (from WinMain game loop, DAT_1409cf314==1 branch)
    static final long[] GAME_ACTIVE = {
        0x2d72c0L, 0x2d2ab0L, 0x321dc0L, 0x325190L,
    };

    // Unconditional per-frame callees (run every frame regardless of game state)
    static final long[] UNCONDITIONAL = {
        0x24bb80L,   // thunk_FUN_14024bb80
        0x2505c0L,   // FUN_1402505c0
        0x2089e0L,   // FUN_1402089e0
        0x3b08c0L,   // FUN_1403b08c0
        0x2c2910L,   // FUN_1402c2910
        0x4908c0L,   // FUN_1404908c0
        0xcaa60L,    // FUN_1400caa60
        0x215b90L,   // Timer subsystem tick (delta * scale)
        0x339bc0L,   // Unknown subsystem, abs time arg
        0x22c50L,    // Unknown subsystem, abs time arg
    };

    // Scene/mode manager callees (always path after game-active branch)
    static final long[] SCENE_MANAGER = {
        0x30c3b0L,   // FUN_14030c3b0 — scene manager / mode stack tick
        0x493f0L,    // FUN_1400493f0 — called twice with different args
        0x1c8420L,   // FUN_1401c8420 — called unconditionally after game branches
    };

    // Render flush and one level of callers
    static final long RVA_RENDER_FLUSH = 0x180dc0L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase2_subsystem_map.c"));

        // -----------------------------------------------------------------------
        // 1. Render flush call chain — find what calls FUN_0x180dc0
        // -----------------------------------------------------------------------
        pw.println("// ================================================================");
        pw.println("// RENDER FLUSH CALL CHAIN");
        pw.println("// ================================================================\n");

        Set<Long> renderCallers = new LinkedHashSet<>();
        Address flushAddr = currentProgram.getImageBase().add(RVA_RENDER_FLUSH);
        ReferenceIterator refs = currentProgram.getReferenceManager()
            .getReferencesTo(flushAddr);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Function fn = getFunctionContaining(ref.getFromAddress());
            if (fn == null) continue;
            long rva = fn.getEntryPoint().getOffset() - imageBase;
            pw.println("// Render flush called from FUN_0x" + Long.toHexString(rva) +
                " (site 0x" + Long.toHexString(ref.getFromAddress().getOffset() - imageBase) + ")");
            renderCallers.add(rva);
        }
        pw.println();
        for (long rva : renderCallers) {
            decompile(decomp, pw, imageBase, rva, 2048);
        }

        // -----------------------------------------------------------------------
        // 2. Game-active callees (gated on DAT_1409cf314 == 1)
        // -----------------------------------------------------------------------
        pw.println("\n// ================================================================");
        pw.println("// GAME-ACTIVE CALLEES");
        pw.println("// ================================================================\n");
        for (long rva : GAME_ACTIVE) {
            decompile(decomp, pw, imageBase, rva, 3000);
        }

        // -----------------------------------------------------------------------
        // 3. Unconditional per-frame callees
        // -----------------------------------------------------------------------
        pw.println("\n// ================================================================");
        pw.println("// UNCONDITIONAL PER-FRAME CALLEES");
        pw.println("// ================================================================\n");
        for (long rva : UNCONDITIONAL) {
            decompile(decomp, pw, imageBase, rva, 2048);
        }

        // -----------------------------------------------------------------------
        // 4. Scene/mode manager and always-path callees
        // -----------------------------------------------------------------------
        pw.println("\n// ================================================================");
        pw.println("// SCENE MANAGER / ALWAYS-PATH CALLEES");
        pw.println("// ================================================================\n");
        for (long rva : SCENE_MANAGER) {
            decompile(decomp, pw, imageBase, rva, 2048);
        }

        pw.close();
        decomp.dispose();
        println("Phase 2 subsystem map written to logs/phase2_subsystem_map.c");
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
