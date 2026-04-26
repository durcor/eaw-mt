// Phase5Decomp387400.java
// Decompile FUN_140387400 (RVA 0x387400) — confirmed terminal stall carrier.
// Called from FUN_140387010 (+0x074) when entity motion state (entity+0x48) is in [5,10].
// b387400_max == b87010_max (Δ<1ms) in all stall windows. b381ff0 n=0 always.
//
// Output: logs/phase5_387400.c

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5Decomp387400 extends GhidraScript {

    static final long RVA   = 0x387400L;
    static final String NAME = "FUN_140387400";

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_387400.c"));
        long base = currentProgram.getImageBase().getOffset();

        Address addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + RVA);
        Function fn = currentProgram.getListing().getFunctionAt(addr);
        if (fn == null) {
            out.printf("/* %s (RVA 0x%x) — no function at address */\n", NAME, RVA);
        } else {
            out.printf("/* =========================================================\n");
            out.printf(" * %s  (RVA 0x%x)\n", NAME, RVA);
            out.printf(" * size: %d bytes  params: %d  return: %s\n",
                fn.getBody().getNumAddresses(),
                fn.getParameterCount(),
                fn.getReturnType().getName());
            out.printf(" * ========================================================= */\n\n");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res.decompileCompleted())
                out.println(res.getDecompiledFunction().getC());
            else
                out.printf("/* decompile failed: %s */\n", res.getErrorMessage());
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase5_387400.c");
    }
}
