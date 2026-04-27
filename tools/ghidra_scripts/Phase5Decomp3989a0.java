import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp3989a0 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long rva = 0x3989a0L;
        long imageBase = currentProgram.getImageBase().getOffset();
        Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace()
            .getAddress(imageBase + rva);

        Function fn = getFunctionAt(addr);
        if (fn == null) {
            println("No function at 0x3989a0");
            return;
        }
        println("Decompiling: " + fn.getName() + "  size=" + fn.getBody().getNumAddresses() + " bytes");

        DecompileResults res = decomp.decompileFunction(fn, 120, new ConsoleTaskMonitor());
        String code = res.getDecompiledFunction().getC();

        FileWriter fw = new FileWriter(
            System.getProperty("user.home") +
            "/src/eaw-reverse-engineering/logs/phase5_3989a0.c");
        fw.write("/* FUN_1403989a0  RVA 0x3989a0  size: " +
            fn.getBody().getNumAddresses() + " bytes */\n\n");
        fw.write(code);
        fw.close();
        println("Written to logs/phase5_3989a0.c");
    }
}
