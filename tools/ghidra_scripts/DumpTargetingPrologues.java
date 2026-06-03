import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import java.io.FileWriter;

public class DumpTargetingPrologues extends GhidraScript {
    public void run() throws Exception {
        FileWriter fw = new FileWriter("/home/ty/src/eaw-reverse-engineering/logs/DumpTargetingPrologues.log");
        long[] starts = { 0x140633a30L, 0x140634810L, 0x140633ae0L };
        String[] names = { "633a30 dispatcher", "634810 mode1", "633ae0 mode2" };
        for (int i = 0; i < starts.length; ++i) {
            long start = starts[i];
            long end   = start + 0x30;   // ~48 bytes
            fw.write(String.format("=== %s @ 0x%x ===\n", names[i], start));
            for (long off = start; off < end; ) {
                Address addr = currentProgram.getAddressFactory().getAddress(String.format("0x%x", off));
                Instruction insn = currentProgram.getListing().getInstructionAt(addr);
                if (insn == null) { fw.write(String.format("  0x%x: <no insn>\n", off)); off += 1; continue; }
                StringBuilder sb = new StringBuilder();
                for (byte b : insn.getBytes()) sb.append(String.format("%02x ", b));
                fw.write(String.format("  0x%x (+0x%03x): %-34s  %s\n", off, off - start, sb.toString().trim(), insn.toString()));
                off += insn.getLength();
            }
            fw.write("\n");
        }
        fw.close();
        println("Done");
    }
}
