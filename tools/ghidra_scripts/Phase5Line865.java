// Phase5Line865.java
// Decompile the three unhooked functions called at WinMain line 865:
//   FUN_14020f000 (RVA 0x20f000)
//   FUN_14028d400 (RVA 0x28d400)
//   FUN_1402718e0 (RVA 0x2718e0)
// These are the primary suspects for the 1000ms episodic stalls confirmed in
// gapE (lines 831-869) with no disk I/O and no WaitMessage.
// Output: logs/phase5_line865.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase5Line865 extends GhidraScript {

    static final long[] RVAS  = { 0x20f000L, 0x28d400L, 0x2718e0L };
    static final String[] NAMES = { "FUN_14020f000", "FUN_14028d400", "FUN_1402718e0" };

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_line865.c"));
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long imageBase = currentProgram.getImageBase().getOffset();

        for (int k = 0; k < RVAS.length; k++) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(imageBase + RVAS[k]);
            Function fn = getFunctionAt(addr);

            out.printf("\n/* =========================================================\n");
            out.printf(" * %s  (RVA 0x%x)\n", NAMES[k], RVAS[k]);
            if (fn == null) {
                out.printf(" * ERROR: no function at this address\n */\n\n");
                continue;
            }
            out.printf(" * size: %d bytes\n", fn.getBody().getNumAddresses());
            out.printf(" * params: %d  return: %s\n",
                fn.getParameterCount(),
                fn.getReturnType().getName());
            out.printf(" * ========================================================= */\n\n");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res != null && res.decompileCompleted()) {
                out.println(res.getDecompiledFunction().getC());
            } else {
                out.println("// decompile failed");
            }
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase5_line865.c");
    }
}
