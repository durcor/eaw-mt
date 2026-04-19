//Find all callers of _beginthreadex — maps all OS thread creation sites
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class FindBeginthreadexCallers extends GhidraScript {

    static final long RVA_BEGINTHREADEX = 0x786138L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/beginthreadex_callers.c"));

        Address beginthreadex = currentProgram.getImageBase().add(RVA_BEGINTHREADEX);
        Set<Long> callerRVAs = new LinkedHashSet<>();

        ReferenceIterator refs = currentProgram.getReferenceManager()
            .getReferencesTo(beginthreadex);
        while (refs.hasNext()) {
            Reference ref = refs.next();
            Address fromAddr = ref.getFromAddress();
            Function fn = getFunctionContaining(fromAddr);
            if (fn == null) continue;
            long rva = fn.getEntryPoint().getOffset() - imageBase;
            pw.println("// _beginthreadex called from FUN_0x" + Long.toHexString(rva) +
                " (site 0x" + Long.toHexString(fromAddr.getOffset() - imageBase) + ")");
            callerRVAs.add(rva);
        }

        pw.println("\n// Total callers: " + callerRVAs.size() + "\n");

        for (long rva : callerRVAs) {
            Address addr = currentProgram.getImageBase().add(rva);
            Function fn = getFunctionAt(addr);
            if (fn == null) fn = getFunctionContaining(addr);
            if (fn == null) continue;

            pw.println("// ============================================================");
            pw.println("// RVA: 0x" + Long.toHexString(rva) +
                "  Size: " + fn.getBody().getNumAddresses() + " bytes");
            pw.println("// ============================================================");

            DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
            if (res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
            } else {
                pw.println("// DECOMPILE FAILED: " + res.getErrorMessage());
            }
            pw.println();
        }

        pw.close();
        decomp.dispose();
        println("_beginthreadex callers written to logs/beginthreadex_callers.c — " +
            callerRVAs.size() + " callers");
    }
}
