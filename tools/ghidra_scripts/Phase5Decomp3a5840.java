import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp3a5840 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Address addr = currentProgram.getAddressFactory().getAddress("0x1403a5840");
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("No function at 0x1403a5840"); return; }
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/phase5_3a5840.c");
        fw.write("/* FUN_1403a5840  RVA 0x3a5840 */\n\n");
        fw.write(res.getDecompiledFunction().getC());
        fw.close();
        println("Done — " + fn.getName());
    }
}
