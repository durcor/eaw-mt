// Phase 5 (lever 3): find the sim tick/delta passed to gsvc (FUN_14028d400 param_2 = EDX).
// Disassemble ~20 instructions before each CALL to FUN_14028d400 to see how EDX/RDX is set —
// that's the game-time tick whose advance rate game-speed scales.
//@category EawMT
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;
public class Phase5GsvcDeltaSrc extends GhidraScript {
    @Override public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5GsvcDeltaSrc.out"));
        Address gsvc = currentProgram.getImageBase().add(0x28d400L);
        Listing lst = currentProgram.getListing();
        ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(gsvc);
        while (it.hasNext()) {
            Reference ref = it.next();
            if (!ref.getReferenceType().isCall()) continue;
            Address call = ref.getFromAddress();
            Function caller = getFunctionContaining(call);
            pw.printf("%n=== CALL to gsvc @rva 0x%x  in %s ===%n",
                call.getOffset() - base, caller != null ? caller.getName() : "<none>");
            // walk back 20 instructions
            ArrayList<Instruction> ins = new ArrayList<>();
            Instruction p = lst.getInstructionBefore(call);
            for (int i = 0; i < 20 && p != null; i++) { ins.add(p); p = p.getPrevious(); }
            Collections.reverse(ins);
            for (Instruction in : ins)
                pw.printf("  0x%-8x %s%n", in.getAddress().getOffset() - base, in.toString());
            Instruction ci = lst.getInstructionAt(call);
            if (ci != null) pw.printf("  0x%-8x %s   <-- CALL%n", call.getOffset() - base, ci.toString());
        }
        pw.close();
        println("Phase5GsvcDeltaSrc: done");
    }
}
