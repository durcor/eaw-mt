// Phase 3g: Hunt the simulation tick.
// The scene manager is confirmed UI-only. The GOM tick must be in:
//   - WinMain's "get+tick" pairs: FUN_140025770/FUN_14006d1d0, FUN_140067c80/FUN_14006a1e0
//   - FUN_140051d10 (new per-frame tick on DAT_140a10a98)
//   - FUN_1402e7430 (called from camera/input with scene manager)
//   - FUN_1402089e0 (player registry tick in WinMain)
//   - FUN_1403b08c0 (per-frame tick on DAT_140b2ed18)
// Also: read bytes at DAT_140b2b3a8/2f0 to see if they hold static function pointers.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import java.io.*;

public class Phase3gSimTick extends GhidraScript {

    // WinMain "get + tick" pairs
    static final long RVA_GET_A     = 0x25770L;   // FUN_140025770 — returns something
    static final long RVA_TICK_A    = 0x6d1d0L;   // FUN_14006d1d0 — ticks it
    static final long RVA_GET_B     = 0x67c80L;   // FUN_140067c80
    static final long RVA_TICK_B    = 0x6a1e0L;   // FUN_14006a1e0

    // New per-frame tick
    static final long RVA_TICK_C    = 0x51d10L;   // FUN_140051d10(&DAT_140a10a98)

    // Called from camera/input with scene manager arg
    static final long RVA_E7430     = 0x2e7430L;  // FUN_1402e7430

    // Player registry tick (unconditional in WinMain)
    static final long RVA_PLAYER    = 0x2089e0L;  // FUN_1402089e0

    // Per-frame tick on DAT_140b2ed18
    static final long RVA_B08C0     = 0x3b08c0L;  // FUN_1403b08c0

    // GameModeManagerClass::Service/Think candidates
    static final long RVA_GOM_SVC   = 0x2bf840L;  // area near 0x28xxxx GOM methods — try

    // The indirect call target addresses — read raw bytes
    static final long DAT_ICALL_A   = 0xb2b3a8L;
    static final long DAT_ICALL_B   = 0xb2b2f0L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3g_sim_tick.c"));

        // 0. Read raw bytes at indirect call target addresses
        section(pw, "RAW BYTES AT DAT_140b2b3a8 AND DAT_140b2b2f0");
        readPointer(pw, imageBase, DAT_ICALL_A, "DAT_140b2b3a8");
        readPointer(pw, imageBase, DAT_ICALL_B, "DAT_140b2b2f0");

        // 1. WinMain get+tick pair A
        section(pw, "GET+TICK PAIR A");
        decompile(decomp, pw, imageBase, RVA_GET_A, 200);
        decompile(decomp, pw, imageBase, RVA_TICK_A, 300);

        // 2. WinMain get+tick pair B
        section(pw, "GET+TICK PAIR B");
        decompile(decomp, pw, imageBase, RVA_GET_B, 200);
        decompile(decomp, pw, imageBase, RVA_TICK_B, 300);

        // 3. New per-frame tick
        section(pw, "NEW PER-FRAME TICK (FUN_140051d10)");
        decompile(decomp, pw, imageBase, RVA_TICK_C, 400);

        // 4. FUN_1402e7430 — called from camera/input with scene mgr
        section(pw, "SCENE-MGR CALL FROM CAMERA (FUN_1402e7430)");
        decompile(decomp, pw, imageBase, RVA_E7430, 400);

        // 5. Player registry tick
        section(pw, "PLAYER REGISTRY TICK (FUN_1402089e0)");
        decompile(decomp, pw, imageBase, RVA_PLAYER, 300);

        // 6. Per-frame tick on DAT_140b2ed18
        section(pw, "PER-FRAME TICK FUN_1403b08c0");
        decompile(decomp, pw, imageBase, RVA_B08C0, 300);

        pw.close();
        decomp.dispose();
        println("Phase 3g output written to logs/phase3g_sim_tick.c");
    }

    private void readPointer(PrintWriter pw, long imageBase, long rva, String label)
            throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Memory mem = currentProgram.getMemory();
        try {
            long val = mem.getLong(addr);
            long ptrRva = val - imageBase;
            pw.println("// " + label + " @ RVA 0x" + Long.toHexString(rva));
            pw.println("//   raw value = 0x" + Long.toHexString(val));
            pw.println("//   as RVA    = 0x" + Long.toHexString(ptrRva));
            // Check if the pointed-to RVA is a function
            if (val != 0) {
                Address targetAddr = toAddr(val);
                Function fn = getFunctionAt(targetAddr);
                if (fn == null) fn = getFunctionContaining(targetAddr);
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    pw.println("//   -> function: " + fn.getName() +
                        " @ RVA 0x" + Long.toHexString(fnRva));
                } else {
                    pw.println("//   -> no function found at target");
                }
            }
        } catch (Exception e) {
            pw.println("// " + label + ": read error: " + e.getMessage());
        }
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
