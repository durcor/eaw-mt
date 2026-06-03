import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import java.io.FileWriter;

public class DumpTelekinesisPrologue extends GhidraScript {
    public void run() throws Exception {
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/DumpTelekinesisPrologue.log");
        long[] starts = { 0x14063f210L };
        String[] names = { "63f210 TelekinesisTarget::vfunc_6" };
        for (int i = 0; i < starts.length; ++i) {
            long start = starts[i];
            long end   = start + 0x40;   // ~64 bytes
            fw.write(String.format("=== %s @ 0x%x ===\n", names[i], start));
            for (long off = start; off < end; ) {
                Address addr = currentProgram.getAddressFactory().getAddress(String.format("0x%x", off));
                Instruction insn = currentProgram.getListing().getInstructionAt(addr);
                if (insn == null) { fw.write(String.format("  0x%x: <no insn>\n", off)); off += 1; continue; }
                StringBuilder sb = new StringBuilder();
                for (byte b : insn.getBytes()) sb.append(String.format("%02x ", b));
                fw.write(String.format("  0x%x (+0x%03x): %-40s  %s\n", off, off - start, sb.toString().trim(), insn.toString()));
                off += insn.getLength();
            }
            fw.write("\n");
        }
        fw.close();
        println("Done");
    }
}
