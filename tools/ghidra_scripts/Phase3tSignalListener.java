// Phase 3t: Find the per-unit Think via the signal listener embedded at GameObjectClass+8.
// GameObjectClass dtor (slot 0) calls FUN_1401f92b0(param_1+8) — destroys embedded sub-obj.
// GameObjectClass dtor (slot 1) calls FUN_1401f92c0(param_1+8).
// Signal dispatcher calls (*plVar11->vftable[1])(plVar11, source, event_id, data).
// If plVar11 = game_object+8, then game_object+8->vftable[1] IS the per-unit Think.
//
// Strategy:
//   1. Decompile FUN_1401f92b0 and FUN_1401f92c0 — understand the sub-object type
//   2. Find who registers game objects for signal &DAT_140b153e0 event 0x23
//      (search for calls to a registration fn with event_id=0x23)
//   3. Look for the sub-object's constructor to find its vtable
//   4. Decompile vtable slot 1 of that vtable = per-unit Think
//
// Also: find callers of FUN_1401f92b0 to understand who creates/destroys this sub-type.
// And: look at FUN_140220e90 (signal registration call from FUN_1403bd080/SetupPhaseManager).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3tSignalListener extends GhidraScript {

    // Sub-object destructors called from GameObjectClass dtor on param_1+8
    static final long RVA_F92B0   = 0x1f92b0L; // FUN_1401f92b0 — sub-obj dtor A
    static final long RVA_F92C0   = 0x1f92c0L; // FUN_1401f92c0 — sub-obj dtor B

    // Signal registration — called from SetupPhaseManager ctor with event_id=0x1c,0x1d,0x24
    static final long RVA_220E90  = 0x220e90L; // FUN_140220e90 — signal register/connect

    // The add_object function registers objects into GOM; likely also subscribes to signals
    static final long RVA_AACAC0  = 0x2acac0L; // FUN_1402acac0 — add object (register in GOM)

    // FUN_14028a5d0 — the singleton getter used to obtain the combat/signal manager
    static final long RVA_28A5D0  = 0x28a5d0L; // FUN_14028a5d0 — singleton getter

    // FUN_140058570 — another singleton getter (appears in Remove_Object & SetupPhaseManager)
    static final long RVA_058570  = 0x058570L; // FUN_140058570 — another singleton

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3t_signal_listener.c"));

        section(pw, "FUN_1401f92b0 — sub-object destructor A (GameObjectClass+8)");
        decompile(decomp, pw, imageBase, RVA_F92B0, 200);

        section(pw, "FUN_1401f92c0 — sub-object destructor B (GameObjectClass+8)");
        decompile(decomp, pw, imageBase, RVA_F92C0, 200);

        section(pw, "FUN_140220e90 — signal register/connect (SetupPhaseManager subscribes)");
        decompile(decomp, pw, imageBase, RVA_220E90, 200);

        section(pw, "FUN_1402acac0 — add_object (registers object into GOM, likely subscribes)");
        decompile(decomp, pw, imageBase, RVA_AACAC0, 400);

        section(pw, "FUN_14028a5d0 — singleton getter (for combat/signal manager)");
        decompile(decomp, pw, imageBase, RVA_28A5D0, 100);

        section(pw, "FUN_140058570 — singleton getter B");
        decompile(decomp, pw, imageBase, RVA_058570, 100);

        // Find callers of FUN_1401f92b0 to understand what creates the sub-object
        section(pw, "CALLERS OF FUN_1401f92b0 (sub-obj destructor A)");
        Address f92b0Addr = currentProgram.getImageBase().add(RVA_F92B0);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(f92b0Addr);
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

        // Decompile the first few callers that are small or medium-sized
        for (long callerRva : callerRvas) {
            Function fn = getFunctionAt(currentProgram.getImageBase().add(callerRva));
            if (fn != null && fn.getBody().getNumAddresses() < 300) {
                section(pw, "Small caller of sub-obj-dtor-A: RVA 0x" + Long.toHexString(callerRva));
                decompile(decomp, pw, imageBase, callerRva, 200);
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3t output written to logs/phase3t_signal_listener.c");
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
