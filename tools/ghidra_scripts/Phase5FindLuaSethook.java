// Phase5FindLuaSethook.java
// Decompile the Lua API function cluster found near RVA 0x7b8000-0x7bd000
// to identify lua_resume, lua_sethook, lua_error, and lua_State layout.
// lua_resume candidate: FUN_1407bc310 (called in Pump_Threads, checked ==0)
// All Lua API functions appear to be in the 0x7b8000-0x7bd000 band.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase5FindLuaSethook extends GhidraScript {

    // Known Lua API candidates from Phase5FindLuaVM output
    static final long[] KNOWN = {
        0x7b94a0L,  // FUN_1407b94a0 — called with (L, "LuaThreadTable", 14) → lua_getglobal?
        0x7b8e90L,  // FUN_1407b8e90 — called with (L, LUA_GLOBALSINDEX)
        0x7b9e00L,  // FUN_1407b9e00 — returns type, checked ==5 → lua_type?
        0x7b95e0L,  // FUN_1407b95e0 — called with (L, param_2)
        0x7b9510L,  // FUN_1407b9510 — called with (L) only
        0x7b9a60L,  // FUN_1407b9a60 — called with (L, -3)
        0x7b9ab0L,  // FUN_1407b9ab0 — called with (L, -2)
        0x7bc310L,  // FUN_1407bc310 — lua_resume candidate (result checked ==0)
    };

    // Scan this range for all functions; decompile each, look for hook-write pattern
    static final long RANGE_START = 0x7b8000L;
    static final long RANGE_END   = 0x7bdfffL;

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5FindLuaSethook.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        Listing listing = currentProgram.getListing();

        // 1. Decompile the known candidates
        out.println("=== KNOWN LUA API CANDIDATES ===\n");
        for (long rva : KNOWN) {
            Address addr = base.add(rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) { out.printf("RVA 0x%x — no function\n\n", rva); continue; }
            out.printf("--- RVA 0x%x  %s  (%d bytes) ---\n",
                rva, fn.getName(), (int)fn.getBody().getNumAddresses());
            DecompileResults dr = di.decompileFunction(fn, 30, monitor);
            if (dr.decompileCompleted()) {
                String[] lines = dr.getDecompiledFunction().getC().split("\n");
                for (int i = 0; i < Math.min(lines.length, 40); i++)
                    out.println(lines[i]);
                if (lines.length > 40) out.println("  ...");
            } else {
                out.println("  [decompile failed: " + dr.getErrorMessage() + "]");
            }
            out.println();
        }

        // 2. Scan the full range — collect all functions by walking the function manager
        out.println("\n=== RANGE SCAN 0x7b8000-0x7bdfff ===\n");
        Address rangeStart = base.add(RANGE_START);
        Address rangeEnd   = base.add(RANGE_END);
        List<Function> inRange = new ArrayList<>();
        FunctionIterator allFns = listing.getFunctions(true);
        while (allFns.hasNext()) {
            Function fn = allFns.next();
            Address ep = fn.getEntryPoint();
            if (ep.compareTo(rangeStart) >= 0 && ep.compareTo(rangeEnd) <= 0)
                inRange.add(fn);
        }
        out.printf("Functions in range: %d\n\n", inRange.size());
        for (Function fn : inRange) {
            long rva = fn.getEntryPoint().getOffset() - base.getOffset();
            int sz = (int) fn.getBody().getNumAddresses();
            out.printf("  RVA 0x%x  %d bytes  %s\n", rva, sz, fn.getName());
        }

        // 3. Decompile every small function in the range (≤200 bytes)
        out.println("\n=== SMALL FUNCTIONS IN RANGE (≤200 bytes) — full decompile ===\n");
        for (Function fn : inRange) {
            int sz = (int) fn.getBody().getNumAddresses();
            if (sz > 200) continue;
            long rva = fn.getEntryPoint().getOffset() - base.getOffset();
            out.printf("--- RVA 0x%x  %d bytes  %s ---\n", rva, sz, fn.getName());
            DecompileResults dr = di.decompileFunction(fn, 20, monitor);
            if (dr.decompileCompleted()) {
                out.println(dr.getDecompiledFunction().getC());
            } else {
                out.println("  [failed]");
            }
            out.println();
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5FindLuaSethook.log");
    }
}
