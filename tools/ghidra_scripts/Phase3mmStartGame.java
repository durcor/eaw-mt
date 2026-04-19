// Phase 3mm: Find GalacticMode/SpaceMode/LandMode vtable bases via data refs to GetName fns.
// Decompile GameModeManagerClass::Start_Game to see mode creation.
// Find LoadingThreadClass vtable[1] body (the actual LoadThread work).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class Phase3mmStartGame extends GhidraScript {

    // GetName() functions for each mode class
    static final long RVA_GALACTIC_GETNAME = 0x4b2060L;  // returns "GalacticMode"
    static final long RVA_SPACE_GETNAME    = 0x4d77c0L;  // returns "SpaceMode"
    static final long RVA_LAND_GETNAME     = 0x3b8060L;  // returns "LandMode"
    // GameModeManagerClass::Start_Game
    static final long RVA_START_GAME       = 0x28e890L;
    // FUN_14008df00 LoadThread manager — trace callees at command 0xc path
    static final long RVA_LOAD_MANAGER     = 0x8df00L;
    // FUN_14022e310 — thread name setter (sets "LoadThread" name on thread object)
    static final long RVA_THREAD_NAME_SETTER = 0x22e310L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();
        ReferenceManager refMgr = currentProgram.getReferenceManager();

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3mm_start_game.c"));

        // 1. Data refs to GetName fns → vtable entry locations → vtable bases → slot 43
        section(pw, "VTABLE BASE DISCOVERY VIA DATA REFS TO GetName FUNCTIONS");
        long[] getNameRvas = {RVA_GALACTIC_GETNAME, RVA_SPACE_GETNAME, RVA_LAND_GETNAME};
        String[] modeNames = {"GalacticMode", "SpaceMode", "LandMode"};

        for (int m = 0; m < getNameRvas.length; m++) {
            Address fnAddr = currentProgram.getImageBase().add(getNameRvas[m]);
            pw.println("// Mode: " + modeNames[m] + " GetName=RVA 0x" + Long.toHexString(getNameRvas[m]));
            ReferenceIterator refs = refMgr.getReferencesTo(fnAddr);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                if (ref.getReferenceType().isData()) {
                    long entryRva = ref.getFromAddress().getOffset() - imageBase;
                    pw.println("//   Data ref (vtable entry?) at RVA 0x" + Long.toHexString(entryRva));

                    // The GetName slot is likely small (slot 1 or 2). Try different offsets.
                    // For each candidate slot position, compute vtable base and read slot 43.
                    for (int slotGuess : new int[]{1, 2, 3, 4, 5, 6}) {
                        long vtableBase = entryRva - slotGuess * 8L;
                        // Try to read vtable slot 43 = vtableBase + 0x158
                        Address slot43Addr = currentProgram.getImageBase().add(vtableBase + 0x158L);
                        try {
                            Data data = listing.getDataAt(slot43Addr);
                            if (data != null && (data.getDataType() instanceof Pointer)) {
                                Address target = (Address) data.getValue();
                                if (target != null) {
                                    long targetRva = target.getOffset() - imageBase;
                                    Function fn = getFunctionAt(target);
                                    String fnName = (fn != null) ? fn.getName() : "?";
                                    long sz = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                                    pw.println("//     Slot guess=" + slotGuess +
                                        " vtable_base=0x" + Long.toHexString(vtableBase) +
                                        " → slot[43] = RVA 0x" + Long.toHexString(targetRva) +
                                        "  " + fnName + "  " + sz + "b");
                                }
                            }
                        } catch (Exception ignore) {}
                    }

                    // Also: find what function contains the vtable entry address (if it's in a ctor)
                    Function containingFn = getFunctionContaining(ref.getFromAddress());
                    if (containingFn != null) {
                        long fnRva = containingFn.getEntryPoint().getOffset() - imageBase;
                        pw.println("//   Entry at 0x" + Long.toHexString(entryRva) +
                            " is INSIDE function: " + containingFn.getName() +
                            " RVA 0x" + Long.toHexString(fnRva));
                    } else {
                        pw.println("//   Entry at 0x" + Long.toHexString(entryRva) +
                            " is in static data (vtable in .rdata)");
                    }

                    // Read the first few slots around this entry to identify the vtable
                    pw.println("//   Nearby vtable entries (slots -2 to +5 relative to this entry):");
                    for (int d = -2; d <= 5; d++) {
                        long nearRva = entryRva + d * 8L;
                        Address nearAddr = currentProgram.getImageBase().add(nearRva);
                        try {
                            Data data = listing.getDataAt(nearAddr);
                            if (data != null && (data.getDataType() instanceof Pointer)) {
                                Address target = (Address) data.getValue();
                                if (target != null) {
                                    long targetRva = target.getOffset() - imageBase;
                                    Function fn = getFunctionAt(target);
                                    String fnName = (fn != null) ? fn.getName() : "?";
                                    long sz = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                                    pw.println("//     [d=" + d + "] → fn RVA 0x" + Long.toHexString(targetRva) +
                                        "  " + fnName + "  " + sz + "b");
                                }
                            }
                        } catch (Exception ignore) {}
                    }
                }
            }
            pw.println();
        }

        // 2. Decompile GameModeManagerClass::Start_Game
        section(pw, "GameModeManagerClass::Start_Game (FUN_14028e890)");
        decompile(decomp, pw, imageBase, RVA_START_GAME, 150);

        // 3. Find LoadingThreadClass vtable by looking for its name string reference
        section(pw, "LoadingThreadClass — find and decompile thread body");
        // Find "LoadThread" string again and look at what uses it as a thread constructor
        byte[] needle = "LoadingThreadClass".getBytes("US-ASCII");
        Address scanStart = currentProgram.getImageBase().add(0x800000L);
        Address scanEnd   = currentProgram.getImageBase().add(0xa00000L);
        Address found = currentProgram.getMemory().findBytes(scanStart, scanEnd, needle, null, true, monitor);
        while (found != null) {
            long foundRva = found.getOffset() - imageBase;
            byte[] buf = new byte[48];
            try { currentProgram.getMemory().getBytes(found, buf); } catch (Exception ignore) {}
            String s = new String(buf).replaceAll("[^\\x20-\\x7e].*","");
            pw.println("// String at RVA 0x" + Long.toHexString(foundRva) + ": \"" + s + "\"");
            ReferenceIterator refs = refMgr.getReferencesTo(found);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function fn = getFunctionContaining(ref.getFromAddress());
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    pw.println("//   Ref from fn RVA 0x" + Long.toHexString(fnRva) +
                        "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                }
            }
            try { found = currentProgram.getMemory().findBytes(found.add(1), scanEnd, needle, null, true, monitor); }
            catch (Exception e) { break; }
        }

        // 4. Decompile FUN_14022e310 (thread name setter) — helps understand thread structure
        section(pw, "FUN_14022e310 — thread name setter (sets 'LoadThread' etc)");
        decompile(decomp, pw, imageBase, RVA_THREAD_NAME_SETTER, 80);

        // 5. Callers of RVA_LOAD_MANAGER — who triggers loading?
        section(pw, "Callers of FUN_14008df00 (LoadThread command handler)");
        Address loadMgrAddr = currentProgram.getImageBase().add(RVA_LOAD_MANAGER);
        Function loadMgrFn = getFunctionAt(loadMgrAddr);
        if (loadMgrFn != null) {
            for (Function caller : loadMgrFn.getCallingFunctions(monitor)) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                pw.println("// Caller: RVA 0x" + Long.toHexString(callerRva) +
                    "  " + caller.getName() + "  " + caller.getBody().getNumAddresses() + "b");
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3mm output written.");
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
            pw.println("// No function at RVA 0x" + Long.toHexString(rva)); return;
        }
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() + "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        DecompileResults res = decomp.decompileFunction(fn, 240, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage()); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        for (int i = 0; i < Math.min(maxLines, lines.length); i++) pw.println(lines[i]);
        if (lines.length > maxLines)
            pw.println("// ... [truncated at " + maxLines + " / " + lines.length + " lines]");
        pw.println();
    }
}
