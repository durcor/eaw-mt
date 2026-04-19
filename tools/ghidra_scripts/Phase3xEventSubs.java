// Phase 3x: Find who subscribes to event 0x23 (the per-frame Think signal).
// FUN_140220e90 is the subscribe wrapper: (source, event_id, listener).
// Find all callers of FUN_140220e90 and decompile small/medium ones.
// Also decompile key callers of FUN_14020aa60 (connect listener) including
// FUN_14018ada0 (611b), FUN_140189fa0 (360b), FUN_14053cd60 (1984b head).
// Also search for cross-references to event 0x23 constant.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3xEventSubs extends GhidraScript {

    static final long RVA_220E90  = 0x220e90L; // FUN_140220e90 — subscribe wrapper
    static final long RVA_20AA60  = 0x20aa60L; // FUN_14020aa60 — connect listener to signal
    static final long RVA_18ADA0  = 0x18ada0L; // FUN_14018ada0 — medium caller of 20aa60
    static final long RVA_189FA0  = 0x189fa0L; // FUN_140189fa0 — medium caller of 20aa60
    static final long RVA_2FB150  = 0x2fb150L; // FUN_1402fb150 — 2839b caller of 20aa60
    static final long RVA_53CD60  = 0x53cd60L; // FUN_14053cd60 — 1984b caller of 20aa60
    static final long RVA_4F97D0  = 0x4f97d0L; // FUN_1404f97d0 — 207b caller of 20aa60
    static final long RVA_524EE0  = 0x524ee0L; // FUN_140524ee0 — 169b caller of 20aa60
    static final long RVA_725910  = 0x725910L; // FUN_140725910 — 43b caller of 20aa60
    static final long RVA_725940  = 0x725940L; // FUN_140725940 — 372b caller of 20aa60
    static final long RVA_149490  = 0x149490L; // FUN_140149490 — 26b caller of 20aa60

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3x_event_subs.c"));

        // Find all callers of FUN_140220e90 (subscribe wrapper) — these show event_id usage
        section(pw, "ALL CALLERS OF FUN_140220e90 (subscribe wrapper)");
        Address addr220e90 = currentProgram.getImageBase().add(RVA_220E90);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr220e90);
        List<long[]> callers220e90 = new ArrayList<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    boolean found = false;
                    for (long[] e : callers220e90) if (e[0] == callerRva) { found = true; break; }
                    if (!found) {
                        callers220e90.add(new long[]{callerRva, caller.getBody().getNumAddresses()});
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Decompile small-medium callers of FUN_140220e90
        for (long[] entry : callers220e90) {
            if (entry[1] <= 600) {
                section(pw, "Caller of FUN_140220e90 subscribe: RVA 0x" + Long.toHexString(entry[0]) +
                    " (" + entry[1] + " bytes)");
                decompile(decomp, pw, imageBase, entry[0], 300);
            }
        }

        // Decompile the large FUN_1402fb150 (head only — look for 0x23)
        section(pw, "FUN_1402fb150 — 2839b caller of FUN_14020aa60 (head 200 lines)");
        decompile(decomp, pw, imageBase, RVA_2FB150, 200);

        // Medium callers of FUN_14020aa60
        section(pw, "FUN_14018ada0 — 611b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_18ADA0, 300);

        section(pw, "FUN_140189fa0 — 360b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_189FA0, 200);

        // Small callers of FUN_14020aa60
        section(pw, "FUN_140149490 — 26b caller of FUN_14020aa60 (tiny)");
        decompile(decomp, pw, imageBase, RVA_149490, 60);

        section(pw, "FUN_140725910 — 43b caller of FUN_14020aa60 (tiny)");
        decompile(decomp, pw, imageBase, RVA_725910, 60);

        section(pw, "FUN_140725940 — 372b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_725940, 200);

        section(pw, "FUN_1404f97d0 — 207b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_4F97D0, 150);

        section(pw, "FUN_140524ee0 — 169b caller of FUN_14020aa60");
        decompile(decomp, pw, imageBase, RVA_524EE0, 150);

        pw.close();
        decomp.dispose();
        println("Phase 3x output written to logs/phase3x_event_subs.c");
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

        DecompileResults res = decomp.decompileFunction(fn, 240, monitor);
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
