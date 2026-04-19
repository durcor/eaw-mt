// Phase 3v: Understand the signal subscription system to find handlers for event 0x23.
// FUN_140220e90 calls FUN_1402406c0(signal_source, event_id) — signal registration?
// Event 0x23 fired per-frame from slot 4 on &DAT_140b153e0.
// Looking at FUN_1402406c0 and also callers that register for event 0x23.
// Also: search WinMain (FUN_14005d990) for the main per-frame loop section.
// The WinMain is 7440 bytes; read offset 3000+ to find the game loop.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3vSignalReg extends GhidraScript {

    static final long RVA_2406C0  = 0x2406c0L; // FUN_1402406c0 — signal subscribe/register
    static final long RVA_WINMAIN = 0x5d990L;   // FUN_14005d990 — WinMain (7440 bytes)

    // Other signal-related functions to understand the system
    static final long RVA_1CACB0  = 0x1cacb0L; // FUN_1401cacb0 — used in FUN_140240940

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3v_signal_reg.c"));

        section(pw, "FUN_1402406c0 — signal subscribe/register");
        decompile(decomp, pw, imageBase, RVA_2406C0, 300);

        section(pw, "FUN_1401cacb0 — signal lookup (used in FUN_140240940 dispatcher)");
        decompile(decomp, pw, imageBase, RVA_1CACB0, 200);

        // Callers of FUN_1402406c0 with event_id = 0x23
        section(pw, "ALL CALLERS OF FUN_1402406c0 (signal subscription)");
        Address subscribeAddr = currentProgram.getImageBase().add(RVA_2406C0);
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        ReferenceIterator refs = refMgr.getReferencesTo(subscribeAddr);
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

        // WinMain tail section — read the last 400 lines of decompile to find game loop
        section(pw, "WinMain FUN_14005d990 — last section (should contain game loop)");
        decompileFromOffset(decomp, pw, imageBase, RVA_WINMAIN, 400, 400);

        pw.close();
        decomp.dispose();
        println("Phase 3v output written to logs/phase3v_signal_reg.c");
    }

    // Decompile and output only lines [skipLines, skipLines+maxLines)
    private void decompileFromOffset(DecompInterface decomp, PrintWriter pw,
                                      long imageBase, long rva,
                                      int skipLines, int maxLines) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function at RVA 0x" + Long.toHexString(rva)); return;
        }
        pw.println("// Function size: " + fn.getBody().getNumAddresses() + " bytes, showing lines " +
            skipLines + " to " + (skipLines + maxLines));

        DecompileResults res = decomp.decompileFunction(fn, 300, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage()); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        int total = lines.length;
        int start = Math.min(skipLines, total);
        int end = Math.min(skipLines + maxLines, total);
        pw.println("// Total lines: " + total + ", outputting " + start + " to " + end);
        for (int i = start; i < end; i++) pw.println(lines[i]);
        pw.println("// ... [end of window]");
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
