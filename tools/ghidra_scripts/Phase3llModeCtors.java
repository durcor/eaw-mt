// Phase 3ll: Identify non-combat mode classes and what they call at vtable[43] (offset 0x158).
//
// Strategy:
// 1. Use Ghidra data-type API (not raw getLong) to read vtable slot 43 for each vtable that
//    contains FUN_14035cc10 — the data model has relocations applied.
// 2. Find functions referencing mode-name strings (GalacticMode, SpaceMode, LandMode).
// 3. Decompile the LoadThread entry fn FUN_14022e400.
// 4. Decompile FUN_14008df00 (LoadThread manager) first 120 lines.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class Phase3llModeCtors extends GhidraScript {

    // Known vtable entry locations for FUN_14035cc10 (from Phase3jj)
    static final long[] VTABLE_ENTRY_RVAS = {
        0x8651e8L, 0x8673c0L, 0x8762e0L, 0x879318L, 0xb6f858L
    };
    // Mode-name string RVAs
    static final long RVA_STR_GALACTIC   = 0x876588L; // "GalacticMode"
    static final long RVA_STR_SPACE      = 0x8795c0L; // "SpaceMode"
    static final long RVA_STR_LAND       = 0x867668L; // "LandMode"
    // GameModeManagerClass::Start_Game string
    static final long RVA_STR_STARTGAME  = 0x85af30L;
    // Thread entry point (called by _beginthreadex in FUN_14022e490)
    static final long RVA_THREAD_ENTRY   = 0x22e400L;
    // LoadThread manager
    static final long RVA_LOAD_MANAGER   = 0x8df00L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3ll_mode_ctors.c"));

        // 1. For each vtable entry containing FUN_14035cc10, read the vtable via data model
        section(pw, "VTABLE SLOT READING — data-model pointer resolution");
        pw.println("// Vtable entry sites for FUN_14035cc10 (combat mode tick):");
        pw.println("// Assumption: it sits at slot 43 (offset 0x158) in each vtable.");
        pw.println("// So vtable_base = entry_rva - 0x158.");
        pw.println("// We then read vtable_base[i] for nearby slots to identify function neighbors.");
        pw.println();
        for (long entryRva : VTABLE_ENTRY_RVAS) {
            long vtableBase = entryRva - 0x158L;
            pw.println("// --- Entry at RVA 0x" + Long.toHexString(entryRva) +
                " → assumed vtable base 0x" + Long.toHexString(vtableBase));
            // Try slots 40-46 (0x140-0x170) around slot 43
            for (int slot = 0; slot <= 50; slot++) {
                long slotRva = vtableBase + slot * 8L;
                Address slotAddr = currentProgram.getImageBase().add(slotRva);
                try {
                    // Use Ghidra data model — reads through relocation table
                    Data data = listing.getDataAt(slotAddr);
                    if (data != null && (data.getDataType() instanceof Pointer)) {
                        Address target = (Address) data.getValue();
                        if (target != null) {
                            long targetRva = target.getOffset() - imageBase;
                            Function fn = getFunctionAt(target);
                            String fnName = (fn != null) ? fn.getName() : "?";
                            long sz = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                            String marker = (slot == 43) ? " *** slot 43 ***" : "";
                            pw.println("//   [" + slot + "] → fn RVA 0x" + Long.toHexString(targetRva) +
                                "  " + fnName + "  " + sz + "b" + marker);
                        } else {
                            pw.println("//   [" + slot + "] → null pointer");
                        }
                    } else if (data != null) {
                        pw.println("//   [" + slot + "] data type: " + data.getDataType().getName() +
                            " value: " + data.getValue());
                    } else {
                        pw.println("//   [" + slot + "] no data defined at RVA 0x" + Long.toHexString(slotRva));
                    }
                } catch (Exception e) {
                    pw.println("//   [" + slot + "] error: " + e.getMessage());
                }
            }
            pw.println();
        }

        // 2. Find references to mode name strings
        section(pw, "REFERENCES TO MODE NAME STRINGS");
        long[] strRvas = {RVA_STR_GALACTIC, RVA_STR_SPACE, RVA_STR_LAND, RVA_STR_STARTGAME};
        String[] strNames = {"GalacticMode", "SpaceMode", "LandMode", "GameModeManagerClass::Start_Game"};
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        for (int i = 0; i < strRvas.length; i++) {
            Address strAddr = currentProgram.getImageBase().add(strRvas[i]);
            pw.println("// String \"" + strNames[i] + "\" at RVA 0x" + Long.toHexString(strRvas[i]) + ":");
            ReferenceIterator refs = refMgr.getReferencesTo(strAddr);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function fn = getFunctionContaining(ref.getFromAddress());
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    pw.println("//   Ref from fn RVA 0x" + Long.toHexString(fnRva) +
                        "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                }
            }
        }

        // 3. Decompile mode-name-referencing functions
        section(pw, "GalacticMode-referencing functions");
        Address galStr = currentProgram.getImageBase().add(RVA_STR_GALACTIC);
        Set<Long> seen = new HashSet<>();
        ReferenceIterator refs = refMgr.getReferencesTo(galStr);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Function fn = getFunctionContaining(ref.getFromAddress());
            if (fn != null) {
                long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                if (seen.add(fnRva)) decompile(decomp, pw, imageBase, fnRva, 80);
            }
        }

        section(pw, "SpaceMode-referencing functions");
        Address spcStr = currentProgram.getImageBase().add(RVA_STR_SPACE);
        seen.clear();
        refs = refMgr.getReferencesTo(spcStr);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Function fn = getFunctionContaining(ref.getFromAddress());
            if (fn != null) {
                long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                if (seen.add(fnRva)) decompile(decomp, pw, imageBase, fnRva, 80);
            }
        }

        // 4. Decompile thread entry + LoadThread manager
        section(pw, "FUN_14022e400 — thread entry fn (called by _beginthreadex)");
        decompile(decomp, pw, imageBase, RVA_THREAD_ENTRY, 120);

        section(pw, "FUN_14008df00 — LoadThread manager (first 120 lines)");
        decompile(decomp, pw, imageBase, RVA_LOAD_MANAGER, 120);

        pw.close();
        decomp.dispose();
        println("Phase 3ll output written.");
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
