// Phase 5 (Model B prereq): enumerate the Lua C-API surface.
// EaW registers Lua-callable C functions via luaL_Reg-style tables:
//   struct luaL_Reg { const char *name; lua_CFunction func; }  // 16 bytes on x64
//   arrays terminated by {NULL, NULL}.
// We scan initialized, non-executable, readable blocks for runs of {name*, fn*}
// pairs where name* -> a C identifier string and fn* -> an executable address.
// Each run >= MIN_RUN is reported as a registration table: (name, fn RVA).
// This is the over-approximation of "C closures reachable from lua_resume" — the
// write-surface classification (pure vs sim-write) is the next step.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.*;

public class Phase5LuaApiSurface extends GhidraScript {

    static final int MIN_RUN = 3;   // a real table has several entries

    boolean isExec(long va) {
        try {
            MemoryBlock b = currentProgram.getMemory().getBlock(toAddr(va));
            return b != null && b.isExecute() && b.isInitialized();
        } catch (Exception e) { return false; }
    }

    // read up to max bytes as a C string; return null unless it's a plausible
    // Lua identifier / dotted name (starts alpha/_, body [A-Za-z0-9_.]).
    String readIdent(long va, int max) {
        try {
            Memory mem = currentProgram.getMemory();
            MemoryBlock b = mem.getBlock(toAddr(va));
            if (b == null || !b.isInitialized() || b.isExecute()) return null;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < max; i++) {
                int c = mem.getByte(toAddr(va + i)) & 0xff;
                if (c == 0) break;
                boolean ok = (c=='_') || (c>='A'&&c<='Z') || (c>='a'&&c<='z') ||
                             (c>='0'&&c<='9') || c=='.';
                if (!ok) return null;
                sb.append((char)c);
            }
            if (sb.length() < 2) return null;
            char f = sb.charAt(0);
            if (!((f=='_') || (f>='A'&&f<='Z') || (f>='a'&&f<='z'))) return null;
            return sb.toString();
        } catch (Exception e) { return null; }
    }

    long rd64(long va) throws Exception {
        return currentProgram.getMemory().getLong(toAddr(va));   // little-endian
    }

    @Override
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5LuaApiSurface.out"));

        int tables = 0, entries = 0;
        for (MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
            if (!blk.isInitialized() || blk.isExecute() || !blk.isRead()) continue;
            long start = blk.getStart().getOffset();
            long end   = blk.getEnd().getOffset() - 16;
            for (long a = start; a <= end; a += 8) {
                // try to start a run at `a`
                long namep = rd64(a), fnp = rd64(a + 8);
                String nm = readIdent(namep, 96);
                if (nm == null || !isExec(fnp)) continue;
                // walk the run forward in 16-byte strides
                ArrayList<String> names = new ArrayList<>();
                ArrayList<Long>   fns   = new ArrayList<>();
                long p = a;
                while (true) {
                    long np = rd64(p), fp = rd64(p + 8);
                    String n = readIdent(np, 96);
                    if (n == null || !isExec(fp)) break;
                    names.add(n); fns.add(fp);
                    p += 16;
                }
                if (names.size() >= MIN_RUN) {
                    tables++;
                    pw.printf("=== table @rva 0x%x  (%d entries) ===%n", a - base, names.size());
                    for (int i = 0; i < names.size(); i++) {
                        long rva = fns.get(i) - base;
                        Function fn = getFunctionAt(toAddr(fns.get(i)));
                        pw.printf("  %-40s fn_rva=0x%-8x %s%n",
                            names.get(i), rva, fn != null ? "[func]" : "[no-func]");
                        entries++;
                    }
                    a = p; // skip past consumed run
                }
            }
        }
        pw.printf("%n=== SUMMARY: %d tables, %d registered Lua C functions ===%n",
                  tables, entries);
        pw.close();
        println("Phase5LuaApiSurface: " + tables + " tables, " + entries + " entries");
    }
}
