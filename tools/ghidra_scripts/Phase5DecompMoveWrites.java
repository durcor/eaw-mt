// Phase 5 (Model C scoping): verify the shared-write callees in FUN_140387400's work path.
// FUN_1403846c0 = "release previous target" (suspected cross-entity refcount write)
// FUN_1402d5290 = global signal/event dispatch (writes DAT_140b27e60)
// Decompile both to confirm whether the per-component movement work touches shared state
// (=> Model C fork-join not cleanly viable).
//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
public class Phase5DecompMoveWrites extends GhidraScript {
    static final long[] RVAS = { 0x3846c0L, 0x2d5290L, 0x396fe0L, 0x39a400L, 0x39b2a0L };
    @Override public void run() throws Exception {
        DecompInterface d = new DecompInterface(); d.openProgram(currentProgram);
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5DecompMoveWrites.out"));
        for (long rva : RVAS) {
            Function f = getFunctionAt(currentProgram.getImageBase().add(rva));
            pw.printf("%n// ===== rva=0x%x %s =====%n", rva, f != null ? f.getName() : "<none>");
            if (f == null) continue;
            DecompileResults r = d.decompileFunction(f, 45, monitor);
            pw.println(r != null && r.decompileCompleted()
                ? r.getDecompiledFunction().getC() : "// <decompile failed>");
        }
        pw.close();
        println("Phase5DecompMoveWrites: done");
    }
}
