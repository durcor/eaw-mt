// Phase 5 (Source-B verify): determine whether the per-probe asset-load cost
// (FUN_140141f70, called 119x in FUN_14025ec10's slow path) bottoms out in
// CreateFile (=> fscache already short-circuits it) or a MEG-internal read
// (=> fscache does not help; Source B needs its own cache).
// Decompiles the probe functions and follows callees a few levels, then we grep
// the output for file-open APIs (CreateFileA/W, NtCreateFile, _open, fopen, ReadFile).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.*;

public class Phase5MegProbeMechanism extends GhidraScript {
    static final long[] ROOTS = { 0x141f70L, 0x142f80L, 0x25ec10L };
    DecompInterface d; PrintWriter pw; long base; Set<Long> done = new HashSet<>();

    void dump(Function f, int depth) throws Exception {
        if (f == null || depth < 0) return;
        long rva = f.getEntryPoint().getOffset() - base;
        if (!done.add(rva)) return;
        pw.printf("%n// ===== %s rva=0x%x =====%n", f.getName(), rva);
        DecompileResults r = d.decompileFunction(f, 45, monitor);
        if (r != null && r.decompileCompleted())
            pw.println(r.getDecompiledFunction().getC());
        else pw.println("// <decompile failed>");
        if (depth > 0)
            for (Function c : f.getCalledFunctions(monitor)) {
                String n = c.getName();
                // follow internal funcs; also always note imported thunks by name
                dump(c, depth - 1);
            }
    }

    @Override public void run() throws Exception {
        base = currentProgram.getImageBase().getOffset();
        d = new DecompInterface(); d.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5MegProbeMechanism.out"));
        for (long rva : ROOTS) {
            Function f = getFunctionAt(currentProgram.getImageBase().add(rva));
            dump(f, 3);
        }
        pw.close();
        println("Phase5MegProbeMechanism: functions dumped=" + done.size());
    }
}
