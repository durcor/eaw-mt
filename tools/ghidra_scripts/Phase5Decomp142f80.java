import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp142f80 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Address addr = currentProgram.getAddressFactory().getAddress("0x140142f80");
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("No function at 0x140142f80"); return; }
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/phase5_142f80.c");
        fw.write("/* FUN_140142f80  RVA 0x142f80 */\n\n");
        fw.write(res.getDecompiledFunction().getC());
        fw.close();
        println("Done — " + fn.getName());
    }
}
