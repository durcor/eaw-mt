// Phase5DumpCallsIn387010.java
// Dump all CALL and JMP instructions inside FUN_140387010 (344 bytes).
// d12d480 and d12d520 both showed n=0 — the stall must be in a different callee.
// Need to see every branch target inside this function to know what to hook next.
//
// Output: logs/phase5_calls_387010.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5DumpCallsIn387010 extends GhidraScript {

    static final long RVA  = 0x387010L;
    static final String NAME = "FUN_140387010";

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_calls_387010.txt"));
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
        println("Done — logs/phase5_calls_387010.txt");
    }
}
