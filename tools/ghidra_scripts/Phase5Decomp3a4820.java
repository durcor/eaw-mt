import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp3a4820 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        Address addr = currentProgram.getAddressFactory().getAddress("0x1403a4820");
        ghidra.program.model.listing.Function fn =
            currentProgram.getFunctionManager().getFunctionAt(addr);
        if (fn == null) { println("No function at 0x1403a4820"); return; }
        DecompileResults res = decomp.decompileFunction(fn, 60, new ConsoleTaskMonitor());
        String code = res.getDecompiledFunction().getC();
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/phase5_3a4820.c");
        fw.write("/* FUN_1403a4820  RVA 0x3a4820  size unknown */\n\n");
        fw.write(code);
        fw.close();
        println("Done — " + fn.getName() + " body decompiled");
    }
}
