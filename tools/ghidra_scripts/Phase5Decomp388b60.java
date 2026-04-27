import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;

public class Phase5Decomp388b60 extends GhidraScript {
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        long rva = 0x388b60L;
        long imageBase = currentProgram.getImageBase().getOffset();
        Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace()
            .getAddress(imageBase + rva);

        Function fn = getFunctionAt(addr);
        if (fn == null) {
            println("No function at 0x388b60");
            return;
        }
        println("Decompiling: " + fn.getName() + "  size=" + fn.getBody().getNumAddresses() + " bytes");

        DecompileResults res = decomp.decompileFunction(fn, 120, new ConsoleTaskMonitor());
        String code = res.getDecompiledFunction().getC();

        FileWriter fw = new FileWriter(
            System.getProperty("user.home") +
            "/src/eaw-reverse-engineering/logs/phase5_388b60.c");
        fw.write("/* FUN_140388b60  RVA 0x388b60  size: " +
            fn.getBody().getNumAddresses() + " bytes */\n\n");
        fw.write(code);
        fw.close();
        println("Written to logs/phase5_388b60.c");
    }
}
