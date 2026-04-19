// Phase 3aa: Investigate FUN_140288980 (called per-player from slot-3)
// and FUN_1404d9cd0 (called from slot-4 with singleton+mode).
// Also: decompile FUN_140245690 (used in FUN_1404889d0 to check param_1+0x10).
// And: find who calls mode vtable[0x158] (= slot 43, the per-frame game tick).
//
// FUN_140288980(player, 1, 0) from slot-3 = likely player AI dispatch or unit iteration.
// FUN_1404d9cd0 from slot-4 = per-player cleanup/service.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3aaPlayerThink extends GhidraScript {

    static final long RVA_288980 = 0x288980L; // FUN_140288980 — per-player dispatch from slot-3
    static final long RVA_4D9CD0 = 0x4d9cd0L; // FUN_1404d9cd0 — per-player service from slot-4
    static final long RVA_245690 = 0x245690L; // FUN_140245690 — used in FUN_1404889d0
    static final long RVA_4854E0 = 0x4854e0L; // FUN_1404854e0 — GameScoringManager Attach_Lua_Script
    // LandModeClass vtable[43] = slot at offset 0x158
    static final long RVA_LAND_SLOT43_PTR = 0x3b0000L; // placeholder — will scan for mode vtable

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3aa_player_think.c"));

        section(pw, "FUN_140288980 — per-player dispatch (called from slot-3 per player)");
        decompile(decomp, pw, imageBase, RVA_288980, 300);

        section(pw, "FUN_1404d9cd0 — per-player service (called from slot-4)");
        decompile(decomp, pw, imageBase, RVA_4D9CD0, 300);

        section(pw, "FUN_140245690 — used in FUN_1404889d0");
        decompile(decomp, pw, imageBase, RVA_245690, 150);

        // Find callers of FUN_140288980 to confirm it's called per-player
        section(pw, "CALLERS OF FUN_140288980");
        Address addr = currentProgram.getImageBase().add(RVA_288980);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr);
        Set<Long> seen = new HashSet<>();
        while (refs.hasNext()) {
            Reference ref = refs.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    if (seen.add(callerRva)) {
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Find what FUN_140288980 calls (its callees)
        section(pw, "CALLEES OF FUN_140288980 (what it calls)");
        Function fn288980 = getFunctionAt(currentProgram.getImageBase().add(RVA_288980));
        if (fn288980 != null) {
            Set<Long> calleeSeen = new HashSet<>();
            for (Reference ref : currentProgram.getReferenceManager()
                    .getReferencesFrom(fn288980.getBody().getMinAddress())) {
                if (ref.getReferenceType().isCall()) {
                    long calleeOffset = ref.getToAddress().getOffset() - imageBase;
                    if (calleeSeen.add(calleeOffset)) {
                        Function callee = getFunctionAt(ref.getToAddress());
                        String name = (callee != null) ? callee.getName() : "?";
                        long size = (callee != null) ? callee.getBody().getNumAddresses() : 0;
                        pw.println("// Callee: RVA 0x" + Long.toHexString(calleeOffset) +
                            "  Name: " + name + "  Size: " + size + " bytes");
                    }
                }
            }
        }
        pw.println();

        // Also decompile FUN_1404d9cd0 callees
        section(pw, "CALLEES OF FUN_1404d9cd0 (slot-4 per-player service)");
        Function fn4d9cd0 = getFunctionAt(currentProgram.getImageBase().add(RVA_4D9CD0));
        if (fn4d9cd0 != null) {
            Set<Long> calleeSeen = new HashSet<>();
            for (Reference ref : currentProgram.getReferenceManager()
                    .getReferencesFrom(fn4d9cd0.getBody().getMinAddress())) {
                if (ref.getReferenceType().isCall()) {
                    long calleeOffset = ref.getToAddress().getOffset() - imageBase;
                    if (calleeSeen.add(calleeOffset)) {
                        Function callee = getFunctionAt(ref.getToAddress());
                        String name = (callee != null) ? callee.getName() : "?";
                        long size = (callee != null) ? callee.getBody().getNumAddresses() : 0;
                        pw.println("// Callee: RVA 0x" + Long.toHexString(calleeOffset) +
                            "  Name: " + name + "  Size: " + size + " bytes");
                    }
                }
            }
        }
        pw.println();

        pw.close();
        decomp.dispose();
        println("Phase 3aa output written to logs/phase3aa_player_think.c");
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
