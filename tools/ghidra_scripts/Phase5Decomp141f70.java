import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp141f70 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Address addr = currentProgram.getAddressFactory().getAddress("0x140141f70");
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("No function at 0x140141f70"); return; }
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/phase5_141f70.c");
        fw.write("/* FUN_140141f70  RVA 0x141f70 */\n\n");
        fw.write(res.getDecompiledFunction().getC());
        fw.close();
        println("Done — " + fn.getName());
    }
}
