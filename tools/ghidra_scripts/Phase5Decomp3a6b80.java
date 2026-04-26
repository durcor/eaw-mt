// Phase5Decomp3a6b80.java
// Decompile FUN_1403a6b80 (RVA 0x3a6b80) — confirmed per-entity stall carrier.
// Called N times per FUN_1402be640 invocation; ONE call takes 22,310ms
// while all others return in <0.01ms.  Not O(N) — one entity hits a
// catastrophic code path (blocking I/O, Lua, mutex, or resource load).
//
// Output: logs/phase5_3a6b80.c

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5Decomp3a6b80 extends GhidraScript {

    static final long[] RVAS  = { 0x3a6b80L };
    static final String[] NAMES = { "FUN_1403a6b80" };

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_3a6b80.c"));
        long base = currentProgram.getImageBase().getOffset();

        for (int k = 0; k < RVAS.length; k++) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + RVAS[k]);
            Function fn = currentProgram.getListing().getFunctionAt(addr);
            if (fn == null) {
                out.printf("/* %s (RVA 0x%x) — no function at address */\n\n", NAMES[k], RVAS[k]);
                continue;
            }
            out.printf("/* =========================================================\n");
            out.printf(" * %s  (RVA 0x%x)\n", NAMES[k], RVAS[k]);
            out.printf(" * size: %d bytes  params: %d  return: %s\n",
                fn.getBody().getNumAddresses(),
                fn.getParameterCount(),
                fn.getReturnType().getName());
            out.printf(" * ========================================================= */\n\n");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.printf("/* decompile failed: %s */\n\n", res.getErrorMessage());
            }
            out.println();
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase5_3a6b80.c");
    }
}
