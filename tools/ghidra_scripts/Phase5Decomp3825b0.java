import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import java.io.*;

public class Phase5Decomp3825b0 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long[] rvas = { 0x3825b0L };
        String[] names = { "FUN_1403825b0" };
        long base = currentProgram.getImageBase().getOffset();

        PrintWriter out = new PrintWriter(new FileWriter(
            System.getProperty("user.home") +
            "/src/eaw-reverse-engineering/logs/phase5_3825b0.c"));

        for (int i = 0; i < rvas.length; i++) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + rvas[i]);
            Function fn = getFunctionAt(addr);
            if (fn == null) { println("NOT FOUND: " + names[i]); continue; }
            DecompileResults r = decomp.decompileFunction(fn, 60, monitor);
            out.println("/* " + names[i] + "  RVA 0x" +
                Long.toHexString(rvas[i]) + "  size: " +
                fn.getBody().getNumAddresses() + " bytes */");
            out.println(r.getDecompiledFunction().getC());
            out.println();
        }
        out.close();
        println("Done.");
    }
}
