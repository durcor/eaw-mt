// Phase 3jj: Identify what mode vtable[43] (offset 0x158) calls in non-combat modes.
// Strategy:
//   1. Find all functions that call (*ptr + 0x158) — these are mode dispatchers.
//   2. For each mode class, find its ctor via debug string (e.g. "GalacticConquestMode::").
//   3. For each ctor, scan data references to locate the vftable in .rdata.
//   4. Find what function Ghidra placed at vftable+0x158 (via references FROM that address).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3jjModeVtable extends GhidraScript {

    // Known mode-related string RVAs (will also search symbol table)
    // These are likely near the ::Service / ::Reset strings found in Phase 3gg
    static final String[] MODE_KEYWORDS = {
        "GalacticConquest", "GalacticMode", "Galactic",
        "SpaceMode", "LandMode", "MainMenu", "FrontEnd",
        "CampaignMode", "SkirmishMode", "MultiplLayer",
        "GameMode", "PlanetaryInvasion"
    };

    // FUN_14035cc10 = combat mode vtable[43] — confirmed by Phase 3
    static final long RVA_COMBAT_TICK = 0x35cc10L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3jj_mode_vtable.c"));

        // 1. Find all call sites of (*X + 0x158) in Ghidra — search for pattern
        //    by finding callers of FUN_14035cc10 through non-direct refs
        section(pw, "DATA REFERENCES TO FUN_14035cc10 (combat vtable[43])");
        Address combatTick = currentProgram.getImageBase().add(RVA_COMBAT_TICK);
        Function combatFn = getFunctionAt(combatTick);
        if (combatFn != null) {
            pw.println("// Combat mode tick: " + combatFn.getName() +
                " at RVA 0x" + Long.toHexString(RVA_COMBAT_TICK));
            // Data refs = vtable entries pointing to this fn
            ReferenceManager refMgr = currentProgram.getReferenceManager();
            ReferenceIterator refs = refMgr.getReferencesTo(combatTick);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                if (ref.getReferenceType().isData()) {
                    long refRva = ref.getFromAddress().getOffset() - imageBase;
                    pw.println("// DataRef (vtable entry?) at RVA 0x" + Long.toHexString(refRva));
                }
            }
        }

        // 2. Search symbol table for mode-class strings
        section(pw, "SYMBOL TABLE: mode-class name search");
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator allSyms = symTable.getAllSymbols(true);
        Map<String, Long> modeCtors = new LinkedHashMap<>();
        while (allSyms.hasNext()) {
            Symbol sym = allSyms.next();
            String name = sym.getName();
            long symRva = sym.getAddress().getOffset() - imageBase;
            if (symRva <= 0 || symRva >= 0x900000L) continue;
            for (String kw : MODE_KEYWORDS) {
                if (name.contains(kw)) {
                    pw.println("// Sym: " + name + "  RVA 0x" + Long.toHexString(symRva));
                    break;
                }
            }
        }

        // 3. Search for mode debug strings by scanning known string areas
        section(pw, "STRING REFERENCES: mode class names");
        String[] strKeywords = {"GalacticConquest", "FrontEnd", "CampaignMode",
                                "SkirmishMode", "PlanetaryInvasion", "GameMode::"};
        for (String kw : strKeywords) {
            pw.println("// Searching for string: \"" + kw + "\"");
            SymbolIterator si = symTable.getAllSymbols(true);
            while (si.hasNext()) {
                Symbol sym = si.next();
                if (sym.getName().contains(kw)) {
                    long sRva = sym.getAddress().getOffset() - imageBase;
                    pw.println("//   Found sym: " + sym.getName() + " RVA 0x" + Long.toHexString(sRva));
                    // Find refs to this symbol address
                    ReferenceManager refMgr = currentProgram.getReferenceManager();
                    ReferenceIterator refs = refMgr.getReferencesTo(sym.getAddress());
                    while (refs.hasNext()) {
                        Reference ref = refs.next();
                        Function fn = getFunctionContaining(ref.getFromAddress());
                        if (fn != null) {
                            long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                            pw.println("//     Ref from fn RVA 0x" + Long.toHexString(fnRva) +
                                " " + fn.getName() + " " + fn.getBody().getNumAddresses() + "b");
                        }
                    }
                }
            }
        }

        // 4. Find functions that call through +0x158 offset (indirect calls to mode vtable[43])
        //    Decompile WinMain range and look for calls to (*ptr + 0x158)
        //    We know the WinMain does: (*(*DAT_140b15418 + 0x158))(DAT_140b15418, ...)
        //    Find all callers of pattern: look for any fn that has xref to 0x158-offset vftable call
        //    Best approach: look for functions that CALL FUN_14035cc10 indirectly via vtable
        //    Since we can't easily search for indirect calls, instead find callers of the dispatch loop
        section(pw, "CALLERS OF KNOWN MODE TICK FUNCTIONS");
        long[] modeTickRvas = {RVA_COMBAT_TICK};
        for (long rva : modeTickRvas) {
            Address addr = currentProgram.getImageBase().add(rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) continue;
            for (Function caller : fn.getCallingFunctions(monitor)) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                pw.println("// Caller of 0x" + Long.toHexString(rva) + ": RVA 0x" +
                    Long.toHexString(callerRva) + "  " + caller.getName() +
                    "  " + caller.getBody().getNumAddresses() + "b");
            }
        }

        // 5. Decompile FUN_14035cc10 callers to see if there's a dispatcher that also
        //    calls other mode ticks
        section(pw, "CALLERS OF FUN_14035cc10 (full decompile)");
        if (combatFn != null) {
            for (Function caller : combatFn.getCallingFunctions(monitor)) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                pw.println("// --- Caller: " + caller.getName() + " RVA 0x" + Long.toHexString(callerRva));
                decompile(decomp, pw, imageBase, callerRva, 60);
            }
        }

        // 6. Find strings like "GalacticConquestMode::Service" or "::Think" in string table
        section(pw, "FUNCTION SEARCH: galactic mode per-frame fn via debug strings");
        // scan functions for ones that contain debug strings with mode names
        FunctionIterator fnIter = currentProgram.getListing().getFunctions(true);
        int checked = 0;
        List<long[]> candidates = new ArrayList<>();
        while (fnIter.hasNext() && checked < 50000) {
            Function fn = fnIter.next();
            long fnRva = fn.getEntryPoint().getOffset() - imageBase;
            if (fnRva < 0x400000L || fnRva > 0x600000L) { checked++; continue; }
            // Look for data refs from this fn to string table
            ReferenceManager refMgr = currentProgram.getReferenceManager();
            AddressSetView body = fn.getBody();
            Address cur = body.getMinAddress();
            Address end = body.getMaxAddress();
            boolean found = false;
            while (cur != null && !found && cur.compareTo(end) <= 0) {
                for (Reference ref : refMgr.getReferencesFrom(cur)) {
                    long toRva = ref.getToAddress().getOffset() - imageBase;
                    if (toRva >= 0x860000L && toRva < 0x8c0000L) {
                        // in string data range — check data at address
                        try {
                            Memory mem = currentProgram.getMemory();
                            byte[] buf = new byte[32];
                            mem.getBytes(ref.getToAddress(), buf);
                            String s = new String(buf).replaceAll("[^\\x20-\\x7e].*", "");
                            for (String kw : new String[]{"GalacticConquest", "FrontEnd",
                                    "Campaign", "Skirmish", "PlanetaryInvasion"}) {
                                if (s.contains(kw)) {
                                    pw.println("// FN RVA 0x" + Long.toHexString(fnRva) +
                                        "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() +
                                        "b  →  string: \"" + s + "\"");
                                    candidates.add(new long[]{fnRva});
                                    found = true;
                                    break;
                                }
                            }
                        } catch (Exception ignore) {}
                    }
                    if (found) break;
                }
                try { cur = cur.add(1); } catch (Exception e) { break; }
                checked++;
            }
        }

        // Decompile any candidates found
        for (long[] c : candidates) {
            decompile(decomp, pw, imageBase, c[0], 80);
        }

        pw.close();
        decomp.dispose();
        println("Phase 3jj output written.");
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
