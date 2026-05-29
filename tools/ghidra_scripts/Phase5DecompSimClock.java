// Phase 5 (lever 3 scoping): find the sim-tick / game-speed scheduler.
// WinMain:865 trio — FUN_14020f000, FUN_14028d400 (gsvc), FUN_1402718e0 — plus the
// clock advance. Goal: locate where game speed scales the sim time-step / delta-ticks,
// to assess whether a clean per-frame tick-rate cap can be hooked.
//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.*;
public class Phase5DecompSimClock extends GhidraScript {
    static final long[] RVAS = { 0x20f000L, 0x28d400L, 0x2718e0L };
    DecompInterface d; PrintWriter pw; long base; Set<Long> done = new HashSet<>();
    void dump(Function f, int depth) throws Exception {
        if (f == null || depth < 0) return;
        long rva = f.getEntryPoint().getOffset() - base;
        if (!done.add(rva)) return;
        pw.printf("%n// ===== %s rva=0x%x =====%n", f.getName(), rva);
        DecompileResults r = d.decompileFunction(f, 45, monitor);
        pw.println(r != null && r.decompileCompleted() ? r.getDecompiledFunction().getC() : "// <fail>");
        if (depth > 0) for (Function c : f.getCalledFunctions(monitor)) dump(c, depth - 1);
    }
    @Override public void run() throws Exception {
        base = currentProgram.getImageBase().getOffset();
        d = new DecompInterface(); d.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5DecompSimClock.out"));
        for (long rva : RVAS) dump(getFunctionAt(currentProgram.getImageBase().add(rva)), 1);
        pw.close();
        println("Phase5DecompSimClock: dumped=" + done.size());
    }
}
