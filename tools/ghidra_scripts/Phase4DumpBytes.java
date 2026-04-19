// Phase4DumpBytes.java
// Dump the raw bytes of FUN_140180d90 (RVA 0x180d90, 32 bytes)
// and FUN_140149650 (RVA 0x149650) for trampoline construction.
// Output: logs/phase4_bytes.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.app.plugin.core.analysis.AutoAnalysisManager;
import java.io.*;

public class Phase4DumpBytes extends GhidraScript {

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase4_bytes.txt"));
        long imageBase = currentProgram.getImageBase().getOffset();
        Listing listing = currentProgram.getListing();

        long[] targets = { 0x180d90L, 0x13D3F0L };

        for (long rva : targets) {
            Address addr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(imageBase + rva);

            out.printf("// RVA 0x%x  VA 0x%x\n", rva, imageBase + rva);
            out.printf("// Instructions:\n");

            InstructionIterator iter = listing.getInstructions(addr, true);
            int byteCount = 0;
            while (iter.hasNext() && byteCount < 64) {
                Instruction insn = iter.next();
                // Stop if we've left the function
                if (insn.getAddress().getOffset() - (imageBase + rva) >= 64) break;

                long offset = insn.getAddress().getOffset() - imageBase;
                byte[] bytes = insn.getBytes();
                StringBuilder hex = new StringBuilder();
                for (byte b : bytes) hex.append(String.format("%02x ", b));

                out.printf("  +%02d  %06x  %-30s  %s\n",
                    byteCount, offset,
                    hex.toString().trim(),
                    insn.toString());
                byteCount += bytes.length;
                if (byteCount >= 32) break;
            }
            out.printf("\n");
        }

        out.close();
        println("Done — logs/phase4_bytes.txt");
    }
}
