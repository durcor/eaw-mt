// Phase 5 (Model B prereq, step 3): confirm the EaW Lua game-object binding shape.
// FUN_140024bfb0 registers 5 closures (0x24a570/7f0/8a0, 0x24c200/320) — candidate
// metatable + dispatcher. Decompile the registrar and each closure to see whether the
// game API is a generic name-reflecting __index dispatcher (=> write surface is the whole
// game-object method set, not an enumerable closure list).
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5DecompGameBinding extends GhidraScript {
    static final long[] RVAS = {
        0x24bfb0L,  // registrar (builds the metatable)
        0x24a570L, 0x24a7f0L, 0x24a8a0L, 0x24c200L, 0x24c320L, // the 5 registered closures
        0x246940L   // pump error-path closure (also registered)
    };
    @Override
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        DecompInterface d = new DecompInterface();
        d.openProgram(currentProgram);
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5DecompGameBinding.out"));
        for (long rva : RVAS) {
            Function f = getFunctionAt(currentProgram.getImageBase().add(rva));
            pw.printf("%n// ===== rva=0x%x  %s =====%n", rva,
                      f != null ? f.getName() : "<no func>");
            if (f == null) continue;
            DecompileResults r = d.decompileFunction(f, 60, monitor);
            if (r != null && r.decompileCompleted())
                pw.println(r.getDecompiledFunction().getC());
            else
                pw.println("// <decompile failed>");
        }
        pw.close();
        println("Phase5DecompGameBinding: done");
    }
}
