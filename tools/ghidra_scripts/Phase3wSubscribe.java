// Phase 3w: Decompile FUN_1402215b0 (602 bytes) — the ONLY substantive caller of
// FUN_1402406c0 (signal subscribe). This is the registration function that connects
// listeners to signal events, including event 0x23 (per-frame Think trigger).
// Also decompile FUN_14020aa60 (connects listener to signal list) and
// check the mode vtable slot 43 (0x158) — the per-frame game loop dispatch.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3wSubscribe extends GhidraScript {

    static final long RVA_2215B0  = 0x2215b0L; // FUN_1402215b0 — main signal subscriber (602 bytes)
    static final long RVA_20AA60  = 0x20aa60L; // FUN_14020aa60 — connect listener to signal
    static final long RVA_221410  = 0x221410L; // FUN_140221410 — called from FUN_1402406c0 on first subscribe
    static final long RVA_240F50  = 0x240f50L; // FUN_140240f50 — lookup/find signal slot in FUN_1402406c0

    // SpaceModeClass vtable slot 43 (offset 0x158) — per-frame game loop dispatch
    // Find by looking at SpaceModeClass or LandModeClass vtable at index 43
    // Space vtable slot-4 target was FUN_14035cc10, which fires event 0x23
    // The game loop calls mode->vtable[43] each frame
    static final long RVA_SPACE_SLOT43 = 0x4d7ce0L; // SpaceModeClass slot 41 from phase3p
    static final long RVA_LAND_SLOT43  = 0x3b86c0L; // LandModeClass slot 41 from phase3p

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3w_subscribe.c"));

        section(pw, "FUN_1402215b0 — main signal subscriber (only real caller of FUN_1402406c0)");
        decompile(decomp, pw, imageBase, RVA_2215B0, 400);

        section(pw, "FUN_14020aa60 — connect listener to signal list");
        decompile(decomp, pw, imageBase, RVA_20AA60, 200);

        section(pw, "FUN_140221410 — called from FUN_1402406c0 on first subscribe for signal source");
        decompile(decomp, pw, imageBase, RVA_221410, 200);

        section(pw, "FUN_140240f50 — lookup signal slot (called in FUN_1402406c0)");
        decompile(decomp, pw, imageBase, RVA_240F50, 200);

        // Find callers of FUN_1402215b0 to understand what calls the subscription setup
        section(pw, "CALLERS OF FUN_1402215b0 (signal subscription setup)");
        Address addr2215b0 = currentProgram.getImageBase().add(RVA_2215B0);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(addr2215b0);
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

        // Decompile callers that are small/medium
        for (long callerRva : seen) {
            Function fn = getFunctionAt(currentProgram.getImageBase().add(callerRva));
            if (fn == null) fn = getFunctionContaining(currentProgram.getImageBase().add(callerRva));
            if (fn != null && fn.getBody().getNumAddresses() < 600) {
                section(pw, "Caller of FUN_1402215b0: RVA 0x" + Long.toHexString(callerRva) +
                    " (" + fn.getBody().getNumAddresses() + " bytes)");
                decompile(decomp, pw, imageBase, callerRva, 300);
            }
        }

        // Also find callers of FUN_14020aa60 to understand what else connects listeners
        section(pw, "CALLERS OF FUN_14020aa60 (connect listener to signal) — sample");
        Address addr20aa60 = currentProgram.getImageBase().add(RVA_20AA60);
        ReferenceIterator refs2 = refMgr.getReferencesTo(addr20aa60);
        Set<Long> seen2 = new HashSet<>();
        while (refs2.hasNext()) {
            Reference ref = refs2.next();
            if (ref.getReferenceType().isCall()) {
                Function caller = getFunctionContaining(ref.getFromAddress());
                if (caller != null) {
                    long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                    if (seen2.add(callerRva)) {
                        pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                            "  Name: " + caller.getName() +
                            "  Size: " + caller.getBody().getNumAddresses() + " bytes");
                    }
                }
            }
        }
        pw.println();

        pw.close();
        decomp.dispose();
        println("Phase 3w output written to logs/phase3w_subscribe.c");
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
