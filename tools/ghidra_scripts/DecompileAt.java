//Phase 1 infra: reusable batch decompiler. Decompiles each RVA passed as a script arg
//(hex, with or without 0x) to decomp/<rva>.c, using the project's current (RTTI-applied) names.
//Usage via justfile: just decomp 387400   |   just decomp-list logs/simcore_rvas.txt
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class DecompileAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) { println("DecompileAt: no RVA args"); return; }

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        long base = currentProgram.getImageBase().getOffset();
        new File("/home/ty/src/eaw-reverse-engineering/decomp").mkdirs();

        int ok = 0, fail = 0;
        for (String a : args) {
            String h = a.startsWith("0x") || a.startsWith("0X") ? a.substring(2) : a;
            long rva;
            try { rva = Long.parseLong(h, 16); } catch (Exception e) { println("bad rva: " + a); continue; }
            Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(base + rva);
            Function fn = currentProgram.getListing().getFunctionAt(addr);
            File outF = new File(String.format("/home/ty/src/eaw-reverse-engineering/decomp/%x.c", rva));
            PrintWriter out = new PrintWriter(new FileWriter(outF));
            if (fn == null) {
                out.printf("/* RVA 0x%x — no function at address */\n", rva);
                fail++;
            } else {
                out.printf("/* %s  RVA 0x%x  size %d  params %d  ret %s */\n\n",
                    fn.getName(true), rva, fn.getBody().getNumAddresses(),
                    fn.getParameterCount(), fn.getReturnType().getName());
                DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
                if (res.decompileCompleted()) { out.println(res.getDecompiledFunction().getC()); ok++; }
                else { out.printf("/* decompile failed: %s */\n", res.getErrorMessage()); fail++; }
            }
            out.close();
        }
        decomp.dispose();
        println("DecompileAt: " + ok + " ok, " + fail + " failed → decomp/");
    }
}
