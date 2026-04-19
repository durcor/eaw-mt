// Phase 3s: Find the unit Think loop via cross-references.
// FUN_1403b08c0 (command queue processor, confirmed per-frame) — find its callers.
// FUN_140285760 (per-player tick from slot 3's FUN_1404d9df0) — likely per-player AI tick.
// FUN_140240940 (event dispatcher called with event 0x23) — might be the think dispatch.
// Also check: FUN_140285760 size and what it calls.
// If the command processor caller iterates game objects, that IS the Think loop.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3sXrefThink extends GhidraScript {

    static final long RVA_CMD_Q   = 0x3b08c0L; // FUN_1403b08c0 — command queue processor
    static final long RVA_285760  = 0x285760L; // FUN_140285760 — per-player tick (from d9df0)
    static final long RVA_240940  = 0x240940L; // FUN_140240940 — event dispatcher (event 0x23)

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3s_xref_think.c"));

        // 1. Find callers of the command queue processor
        section(pw, "CALLERS OF FUN_1403b08c0 (command queue processor)");
        Address cmdqAddr = currentProgram.getImageBase().add(RVA_CMD_Q);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(cmdqAddr);
        List<Long> callerRvas = new ArrayList<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    if (!callerRvas.contains(callerRva)) {
                        callerRvas.add(callerRva);
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Decompile each caller
        for (long callerRva : callerRvas) {
            section(pw, "CALLER OF CMD_Q: RVA 0x" + Long.toHexString(callerRva));
            decompile(decomp, pw, imageBase, callerRva, 400);
        }

        // 2. Per-player per-frame tick
        section(pw, "FUN_140285760 — per-player per-frame tick (from FUN_1404d9df0)");
        decompile(decomp, pw, imageBase, RVA_285760, 500);

        // 3. Event dispatcher
        section(pw, "FUN_140240940 — event dispatcher (event 0x23 = think signal)");
        decompile(decomp, pw, imageBase, RVA_240940, 300);

        pw.close();
        decomp.dispose();
        println("Phase 3s output written to logs/phase3s_xref_think.c");
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
