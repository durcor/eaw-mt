// Phase4FlushBody.java
// Decompile the first 120 lines of FUN_140180dc0 (render flush body, 2078 bytes)
// to find where it reads the render task list from the driver object.
// Output: logs/phase4_flush_body.c

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.decompiler.*;
import java.io.*;

public class Phase4FlushBody extends GhidraScript {

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase4_flush_body.c"));
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long imageBase = currentProgram.getImageBase().getOffset();
        Address addr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(imageBase + 0x180dc0L);

        Function fn = getFunctionAt(addr);
        if (fn == null) { out.println("// no function"); out.close(); return; }

        out.printf("// FUN_140180dc0 — render flush body (%d bytes)\n\n",
            fn.getBody().getNumAddresses());

        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
        if (res != null && res.decompileCompleted()) {
            // Print only the first 120 lines — the list-reading setup is near the top
            String[] lines = res.getDecompiledFunction().getC().split("\n");
            int limit = Math.min(lines.length, 120);
            for (int i = 0; i < limit; i++)
                out.println(lines[i]);
            if (lines.length > limit)
                out.printf("\n// ... (%d more lines)\n", lines.length - limit);
        } else {
            out.println("// decompile failed");
        }

        out.close();
        decomp.dispose();
        println("Done — logs/phase4_flush_body.c");
    }
}
