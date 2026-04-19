//Phase 1.4: render flush RVA, GOM tick confirmation, Lua thread model
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class Phase14Analysis extends GhidraScript {

    static final long RVA_CANDIDATE_GOM = 0x21caf0L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase14_analysis.c"));

        // ---------------------------------------------------------------
        // 1.4B — Decompile FUN_14021caf0 to confirm GOM tick
        // ---------------------------------------------------------------
        pw.println("// ================================================================");
        pw.println("// 1.4B — FUN_14021caf0 — GOM tick candidate");
        pw.println("// ================================================================");
        decompileRVA(decomp, pw, RVA_CANDIDATE_GOM);

        // ---------------------------------------------------------------
        // 1.4A — Find render task flush via geometry limit error string
        // The string "render task for %s exceeded the dynamic geometry limits"
        // lives in the function that executes (flushes) render tasks.
        // ---------------------------------------------------------------
        pw.println("\n// ================================================================");
        pw.println("// 1.4A — Render flush candidates");
        pw.println("// (via xref to geometry-limit error string)");
        pw.println("// ================================================================\n");

        Set<Long> renderRVAs = new LinkedHashSet<>();
        findStringXrefs(pw, "exceeded the dynamic geometry limits", renderRVAs);
        findStringXrefs(pw, "alRenderTask", renderRVAs);
        findStringXrefs(pw, "alGraphicsDriver", renderRVAs);

        for (long rva : renderRVAs) {
            decompileRVA(decomp, pw, rva);
        }

        // ---------------------------------------------------------------
        // 1.4C — Find Lua thread creation: xref "LuaScriptThread:" string
        // If the function (or its callees) call CreateThread, these are OS
        // threads. If not, they are coroutines managed on one OS thread.
        // ---------------------------------------------------------------
        pw.println("\n// ================================================================");
        pw.println("// 1.4C — Lua thread model");
        pw.println("// (via xref to 'LuaScriptThread:' string)");
        pw.println("// ================================================================\n");

        Set<Long> luaRVAs = new LinkedHashSet<>();
        findStringXrefs(pw, "LuaScriptThread:", luaRVAs);
        findStringXrefs(pw, "LuaScriptThread", luaRVAs);

        for (long rva : luaRVAs) {
            decompileRVA(decomp, pw, rva);
        }

        pw.close();
        decomp.dispose();
        println("Phase 1.4 analysis written to logs/phase14_analysis.c");
    }

    private void findStringXrefs(PrintWriter pw, String fragment, Set<Long> outRVAs) {
        long imageBase = currentProgram.getImageBase().getOffset();
        DataIterator dataIt = currentProgram.getListing().getDefinedData(true);
        while (dataIt.hasNext()) {
            Data d = dataIt.next();
            DataType dt = d.getDataType();
            if (!(dt instanceof StringDataType) &&
                !(dt instanceof UnicodeDataType) &&
                !(dt instanceof TerminatedStringDataType) &&
                !(dt instanceof TerminatedUnicodeDataType)) continue;

            Object val = d.getValue();
            if (val == null) continue;
            String s = val.toString();
            if (!s.contains(fragment)) continue;

            pw.println("// String \"" + s.replace("\n","\\n") + "\" @ 0x" +
                Long.toHexString(d.getAddress().getOffset()));

            ReferenceIterator refs = currentProgram.getReferenceManager()
                .getReferencesTo(d.getAddress());
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Address fromAddr = ref.getFromAddress();
                Function fn = getFunctionContaining(fromAddr);
                if (fn == null) continue;
                long rva = fn.getEntryPoint().getOffset() - imageBase;
                pw.println("//   xref from FUN_0x" + Long.toHexString(rva) +
                    " (call site 0x" + Long.toHexString(fromAddr.getOffset() - imageBase) + ")");
                outRVAs.add(rva);
            }
        }
    }

    private void decompileRVA(DecompInterface decomp, PrintWriter pw, long rva) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// FUN_0x" + Long.toHexString(rva) + " — no function at this RVA\n");
            return;
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
