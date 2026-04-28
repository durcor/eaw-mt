// Phase5DumpCallsIn25ec10.java
// Dump all CALL/JMP instructions inside FUN_14025ec10 to get byte size
// and confirm call sites for FUN_140142f80 / FUN_140141f70.

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5DumpCallsIn25ec10 extends GhidraScript {

    static final long RVA  = 0x25ec10L;
    static final String NAME = "FUN_14025ec10";

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_calls_25ec10.txt"));
        long base = currentProgram.getImageBase().getOffset();
        Listing listing = currentProgram.getListing();

        Address fnAddr = currentProgram.getAddressFactory()
            .getDefaultAddressSpace().getAddress(base + RVA);
        Function fn = listing.getFunctionAt(fnAddr);
        if (fn == null) {
            out.printf("%s (RVA 0x%x) — no function\n", NAME, RVA);
            out.close();
            return;
        }

        out.printf("=== %s (RVA 0x%x, %d bytes) ===\n",
            NAME, RVA, fn.getBody().getNumAddresses());

        InstructionIterator it = listing.getInstructions(fn.getBody(), true);
        while (it.hasNext()) {
            Instruction insn = it.next();
            String mnem = insn.getMnemonicString().toUpperCase();
            if (!mnem.startsWith("CALL") && !mnem.startsWith("JMP")) continue;

            long rva = insn.getAddress().getOffset() - base;
            byte[] bytes = insn.getBytes();
            StringBuilder hex = new StringBuilder();
            for (byte b : bytes) hex.append(String.format("%02X ", b));

            String target;
            Address[] flows = insn.getFlows();
            if (flows != null && flows.length > 0) {
                long tgt = flows[0].getOffset() - base;
                Function tf = listing.getFunctionAt(flows[0]);
                if (tf != null)
                    target = String.format("RVA 0x%x  [%s]", tgt, tf.getName());
                else
                    target = String.format("RVA 0x%x", tgt);
            } else {
                target = "indirect (" + insn.getDefaultOperandRepresentation(0) + ")";
            }

            out.printf("  +0x%03x  %-30s %s -> %s\n",
                rva - RVA, hex.toString().trim(), mnem, target);
        }

        out.close();
        println("Done — logs/phase5_calls_25ec10.txt");
    }
}
