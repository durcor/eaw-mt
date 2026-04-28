import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp3718f0 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Address addr = currentProgram.getAddressFactory().getAddress("0x1403718f0");
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("No function at 0x1403718f0"); return; }
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/phase5_3718f0.c");
        fw.write("/* FUN_1403718f0  RVA 0x3718f0 */\n\n");
        fw.write(res.getDecompiledFunction().getC());
        fw.close();
        println("Done — " + fn.getName());
    }
}
