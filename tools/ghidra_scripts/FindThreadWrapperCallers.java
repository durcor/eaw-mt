//Find callers of FUN_0x22e490 (sole thread creation wrapper) and decompile FUN_14022e400 (thread proc)
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class FindThreadWrapperCallers extends GhidraScript {

    static final long RVA_THREAD_WRAPPER = 0x22e490L;
    static final long RVA_THREAD_PROC    = 0x22e400L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/thread_wrapper_callers.c"));

        // Callers of the thread-creation wrapper
        Address wrapperAddr = currentProgram.getImageBase().add(RVA_THREAD_WRAPPER);
        Set<Long> callerRVAs = new LinkedHashSet<>();

        ReferenceIterator refs = currentProgram.getReferenceManager()
            .getReferencesTo(wrapperAddr);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Address fromAddr = ref.getFromAddress();
            Function fn = getFunctionContaining(fromAddr);
            if (fn == null) continue;
            long rva = fn.getEntryPoint().getOffset() - imageBase;
            pw.println("// thread wrapper called from FUN_0x" + Long.toHexString(rva) +
                " (site 0x" + Long.toHexString(fromAddr.getOffset() - imageBase) + ")");
            callerRVAs.add(rva);
        }
        pw.println("// Total thread-creation callers: " + callerRVAs.size() + "\n");

        // Decompile each caller
        for (long rva : callerRVAs) {
            decompileRVA(decomp, pw, imageBase, rva);
        }

        // Also decompile the thread proc itself to see what it dispatches
        pw.println("\n// ================================================================");
        pw.println("// Thread proc: FUN_0x22e400 (launched by every _beginthreadex call)");
        pw.println("// ================================================================");
        decompileRVA(decomp, pw, imageBase, RVA_THREAD_PROC);

        pw.close();
        decomp.dispose();
        println("Thread wrapper callers written to logs/thread_wrapper_callers.c — " +
            callerRVAs.size() + " callers");
    }

    private void decompileRVA(DecompInterface decomp, PrintWriter pw,
                               long imageBase, long rva) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function at RVA 0x" + Long.toHexString(rva)); return;
        }
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(rva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");
        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
        if (res.decompileCompleted()) {
            pw.println(res.getDecompiledFunction().getC());
        } else {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage());
        }
        pw.println();
    }
}
