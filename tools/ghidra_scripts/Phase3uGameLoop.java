// Phase 3u: Find the main game loop that calls the per-frame mode Service functions.
// Strategy:
//   1. Find all callers of FUN_14035cc10 (mode slot 4, per-frame for ALL modes) via xrefs
//   2. Find all callers of FUN_14035cb10 (mode slot 3 target, per-frame) via xrefs
//   3. The caller of both is the game loop / mode service dispatcher
//   4. Also look at FUN_14008ddc0 (called from FUN_14008df00 when mode ends)
//   5. And FUN_14028a5d0 result is used in slot 3; FUN_1404d9c80 = per-frame service alloc
//
// The main game loop orchestrates: input → think → render each frame.
// Once found, look for what calls GOM.Think() or unit.Think().
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3uGameLoop extends GhidraScript {

    // Per-frame slot-4 function (shared by ALL modes)
    static final long RVA_SLOT4   = 0x35cc10L; // FUN_14035cc10
    // Per-frame slot-3 function (shared by Space and Land)
    static final long RVA_SLOT3   = 0x35cb10L; // FUN_14035cb10
    // Post-mode-exit helper
    static final long RVA_8DDC0   = 0x8ddc0L;  // FUN_14008ddc0 — called after mode clear

    // FUN_1404d9c80 — the object allocated by FUN_14028a5d0 (size 0x10, service per player)
    static final long RVA_D9C80   = 0x4d9c80L; // FUN_1404d9c80 — per-player service ctor

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3u_game_loop.c"));

        // Find callers of slot-4 function
        section(pw, "CALLERS OF FUN_14035cc10 (slot-4 per-frame, ALL modes)");
        printCallers(pw, imageBase, RVA_SLOT4);

        // Find callers of slot-3 function
        section(pw, "CALLERS OF FUN_14035cb10 (slot-3 per-frame, Space+Land)");
        printCallers(pw, imageBase, RVA_SLOT3);

        // Post-mode helper
        section(pw, "FUN_14008ddc0 — post-mode-exit (called when mode ends/transitions)");
        decompile(decomp, pw, imageBase, RVA_8DDC0, 300);

        // Per-player service object ctor
        section(pw, "FUN_1404d9c80 — per-player service object ctor");
        decompile(decomp, pw, imageBase, RVA_D9C80, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3u output written to logs/phase3u_game_loop.c");
    }

    private void printCallers(PrintWriter pw, long imageBase, long rva) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr);
        Set<Long> seen = new HashSet<>();
        int count = 0;
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Function caller = getFunctionContaining(ref.getFromAddress());
            if (caller != null) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                if (seen.add(callerRva)) {
                    pw.println("// RVA 0x" + Long.toHexString(callerRva) +
                        "  RefType: " + ref.getReferenceType() +
                        "  Name: " + caller.getName() +
                        "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    count++;
                }
            }
        }
        if (count == 0) pw.println("// (no callers found via direct references)");
        pw.println();
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
