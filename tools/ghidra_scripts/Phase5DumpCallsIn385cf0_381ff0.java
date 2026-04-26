// Phase5DumpCallsIn385cf0_381ff0.java
// Dump all CALL and JMP instructions in FUN_140385cf0 (376 bytes) and FUN_140381ff0 (746 bytes).
// The E8 scan for d12d520 (12d520) and d12d480 (12d480) returned n=0 — need to find the
// actual call encoding used (E8 direct, FF 15 indirect, FF D0 register, inlined, etc.).
//
// Output: logs/phase5_calls_385cf0_381ff0.txt

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.lang.OperandType;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase5DumpCallsIn385cf0_381ff0 extends GhidraScript {

    static final long[] RVAS  = { 0x385cf0L, 0x381ff0L };
    static final String[] NAMES = { "FUN_140385cf0", "FUN_140381ff0" };

    @Override
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("logs/phase5_calls_385cf0_381ff0.txt"));
        long base = currentProgram.getImageBase().getOffset();
        Listing listing = currentProgram.getListing();

        for (int k = 0; k < RVAS.length; k++) {
            Address fnAddr = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(base + RVAS[k]);
            Function fn = listing.getFunctionAt(fnAddr);
            if (fn == null) {
                out.printf("%s (RVA 0x%x) — no function\n\n", NAMES[k], RVAS[k]);
                continue;
            }

            out.printf("=== %s (RVA 0x%x, %d bytes) ===\n",
                NAMES[k], RVAS[k], fn.getBody().getNumAddresses());

            InstructionIterator it = listing.getInstructions(fn.getBody(), true);
            while (it.hasNext()) {
                Instruction insn = it.next();
                String mnem = insn.getMnemonicString().toUpperCase();
                if (!mnem.startsWith("CALL") && !mnem.startsWith("JMP")) continue;

                long rva = insn.getAddress().getOffset() - base;
                byte[] bytes = insn.getBytes();
                StringBuilder hex = new StringBuilder();
                for (byte b : bytes) hex.append(String.format("%02X ", b));

                // Try to resolve the target
                String target = "?";
                Address[] flows = insn.getFlows();
                if (flows != null && flows.length > 0) {
                    long tgt = flows[0].getOffset() - base;
                    Function tf = listing.getFunctionAt(flows[0]);
                    if (tf != null)
                        target = String.format("RVA 0x%x  [%s]", tgt, tf.getName());
                    else
                        target = String.format("RVA 0x%x", tgt);
                } else {
                    // Indirect — no static flow, show operand
                    target = "indirect (" + insn.getDefaultOperandRepresentation(0) + ")";
                }

                out.printf("  +0x%03x  %-30s %s -> %s\n",
                    rva - RVAS[k], hex.toString().trim(), mnem, target);
            }
            out.println();
        }

        out.close();
        println("Done — logs/phase5_calls_385cf0_381ff0.txt");
    }
}
