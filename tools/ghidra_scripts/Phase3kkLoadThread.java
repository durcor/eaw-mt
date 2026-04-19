// Phase 3kk: Map LoadThread scope — what asset types, sync primitives with main thread,
// whether load stalls block the simulation tick.
// Strategy:
//   1. Find "LoadThread" string → thread entry fn → decompile.
//   2. Find all xrefs to LoadThread thread-creation site.
//   3. Identify EnterCriticalSection / WaitForSingleObject / mutex calls inside LoadThread.
//   4. Decompile FUN_14022e490 (thread wrapper) and its callees.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3kkLoadThread extends GhidraScript {

    // Thread wrapper (5 call sites confirmed in Phase 1)
    static final long RVA_THREAD_WRAPPER = 0x22e490L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3kk_load_thread.c"));

        // 1. Find "LoadThread" string reference
        section(pw, "STRING SEARCH: LoadThread / PacketHandler / NATUtils thread names");
        String[] threadStrings = {"LoadThread", "PacketHandler Thread", "NATUtilsThread",
                                   "LuaScriptThread", "Main Thread"};
        ReferenceManager refMgr = currentProgram.getReferenceManager();
        SymbolTable symTable = currentProgram.getSymbolTable();

        Map<String, List<Long>> threadFnRvas = new LinkedHashMap<>();
        for (String ts : threadStrings) threadFnRvas.put(ts, new ArrayList<>());

        SymbolIterator si = symTable.getAllSymbols(true);
        while (si.hasNext()) {
            Symbol sym = si.next();
            String name = sym.getName();
            for (String ts : threadStrings) {
                if (name.contains(ts.replace(" ", ""))) {
                    long sRva = sym.getAddress().getOffset() - imageBase;
                    pw.println("// Sym: \"" + name + "\" at RVA 0x" + Long.toHexString(sRva));
                }
            }
        }

        // Scan for string data in .rdata
        section(pw, "MEMORY SCAN: thread name strings → callers");
        long[] scanRanges = {0x860000L, 0x8d0000L}; // typical string range
        Memory mem = currentProgram.getMemory();
        for (String target : new String[]{"LoadThread", "PacketHandler"}) {
            byte[] needle = target.getBytes("US-ASCII");
            Address start = currentProgram.getImageBase().add(scanRanges[0]);
            Address end   = currentProgram.getImageBase().add(scanRanges[1]);
            Address found = mem.findBytes(start, end, needle, null, true, monitor);
            if (found != null) {
                long foundRva = found.getOffset() - imageBase;
                pw.println("// String \"" + target + "\" at RVA 0x" + Long.toHexString(foundRva));
                ReferenceIterator refs = refMgr.getReferencesTo(found);
                while (refs.hasNext()) {
                    Reference ref = refs.next();
                    Function fn = getFunctionContaining(ref.getFromAddress());
                    if (fn != null) {
                        long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                        pw.println("//   Referenced by fn RVA 0x" + Long.toHexString(fnRva) +
                            "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                        threadFnRvas.get(target.equals("LoadThread") ? "LoadThread" : "PacketHandler Thread")
                            .add(fnRva);
                    }
                }
            } else {
                pw.println("// String \"" + target + "\" not found in range 0x" +
                    Long.toHexString(scanRanges[0]) + "-0x" + Long.toHexString(scanRanges[1]));
                // Try wider range
                Address start2 = currentProgram.getImageBase().add(0x800000L);
                Address end2   = currentProgram.getImageBase().add(0xa00000L);
                found = mem.findBytes(start2, end2, needle, null, true, monitor);
                if (found != null) {
                    long foundRva = found.getOffset() - imageBase;
                    pw.println("// (wider range) String \"" + target + "\" at RVA 0x" + Long.toHexString(foundRva));
                    ReferenceIterator refs = refMgr.getReferencesTo(found);
                    while (refs.hasNext()) {
                        Reference ref = refs.next();
                        Function fn = getFunctionContaining(ref.getFromAddress());
                        if (fn != null) {
                            long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                            pw.println("//   Referenced by fn RVA 0x" + Long.toHexString(fnRva) +
                                "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                        }
                    }
                }
            }
        }

        // 2. Decompile thread wrapper and all its call sites
        section(pw, "FUN_14022e490 — thread creation wrapper (5 call sites)");
        decompile(decomp, pw, imageBase, RVA_THREAD_WRAPPER, 80);

        section(pw, "CALL SITES OF FUN_14022e490");
        Address wrapAddr = currentProgram.getImageBase().add(RVA_THREAD_WRAPPER);
        Function wrapFn = getFunctionAt(wrapAddr);
        if (wrapFn != null) {
            for (Function caller : wrapFn.getCallingFunctions(monitor)) {
                long callerRva = caller.getEntryPoint().getOffset() - imageBase;
                pw.println("// Call site fn: RVA 0x" + Long.toHexString(callerRva) +
                    "  " + caller.getName() + "  " + caller.getBody().getNumAddresses() + "b");
                decompile(decomp, pw, imageBase, callerRva, 60);
            }
        }

        // 3. Decompile functions that reference the LoadThread string
        section(pw, "LoadThread-referencing functions — decompiled");
        List<Long> ltRvas = threadFnRvas.get("LoadThread");
        for (long rva : ltRvas) {
            decompile(decomp, pw, imageBase, rva, 120);
        }

        // 4. Find ThreadLockMutexClass usage — search for the mutex class strings
        section(pw, "ThreadLockMutexClass — string search + callers");
        byte[] mutexNeedle = "ThreadLockMutex".getBytes("US-ASCII");
        Address mStart = currentProgram.getImageBase().add(0x800000L);
        Address mEnd   = currentProgram.getImageBase().add(0xa00000L);
        Address mFound = mem.findBytes(mStart, mEnd, mutexNeedle, null, true, monitor);
        while (mFound != null) {
            long mRva = mFound.getOffset() - imageBase;
            // read the full string
            byte[] buf = new byte[64];
            try { mem.getBytes(mFound, buf); } catch (Exception ignore) {}
            String s = new String(buf).replaceAll("[^\\x20-\\x7e].*", "");
            pw.println("// Mutex string at RVA 0x" + Long.toHexString(mRva) + ": \"" + s + "\"");
            ReferenceIterator refs = refMgr.getReferencesTo(mFound);
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function fn = getFunctionContaining(ref.getFromAddress());
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    pw.println("//   Ref from RVA 0x" + Long.toHexString(fnRva) +
                        "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                }
            }
            try { mFound = mem.findBytes(mFound.add(1), mEnd, mutexNeedle, null, true, monitor); }
            catch (Exception e) { break; }
        }

        // 5. Find EnterCriticalSection / WaitForSingleObject call sites
        section(pw, "IMPORT CALLS: EnterCriticalSection / WaitForSingleObject");
        String[] importNames = {"EnterCriticalSection", "WaitForSingleObject",
                                "LeaveCriticalSection", "ReleaseSemaphore", "SetEvent"};
        for (String imp : importNames) {
            SymbolIterator impSi = symTable.getAllSymbols(true);
            while (impSi.hasNext()) {
                Symbol sym = impSi.next();
                if (sym.getName().equals(imp) || sym.getName().endsWith("::" + imp)) {
                    long sRva = sym.getAddress().getOffset() - imageBase;
                    pw.println("// Import \"" + imp + "\" at RVA 0x" + Long.toHexString(sRva));
                    ReferenceIterator refs = refMgr.getReferencesTo(sym.getAddress());
                    int count = 0;
                    while (refs.hasNext() && count < 20) {
                        Reference ref = refs.next();
                        Function fn = getFunctionContaining(ref.getFromAddress());
                        if (fn != null) {
                            long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                            pw.println("//   Called by RVA 0x" + Long.toHexString(fnRva) +
                                "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + "b");
                            count++;
                        }
                    }
                    if (count == 20) pw.println("//   ... (truncated at 20)");
                }
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3kk output written.");
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
