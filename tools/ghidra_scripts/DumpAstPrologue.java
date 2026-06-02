import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import java.io.FileWriter;

public class DumpAstPrologue extends GhidraScript {
    public void run() throws Exception {
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/DumpAstPrologue.log");
        long start = 0x140437310L;
        long end   = 0x140437340L;   // ~48 bytes of prologue instructions
        for (long off = start; off < end; ) {
            Address addr = currentProgram.getAddressFactory().getAddress(String.format("0x%x", off));
            Instruction insn = currentProgram.getListing().getInstructionAt(addr);
            if (insn == null) { fw.write(String.format("  0x%x: <no insn>\n", off)); off += 1; continue; }
            StringBuilder sb = new StringBuilder();
            for (byte b : insn.getBytes()) sb.append(String.format("%02x ", b));
            fw.write(String.format("  0x%x (+0x%03x): %-32s  %s\n", off, off - start, sb.toString().trim(), insn.toString()));
            off += insn.getLength();
        }
        fw.close();
        println("Done");
    }
}
