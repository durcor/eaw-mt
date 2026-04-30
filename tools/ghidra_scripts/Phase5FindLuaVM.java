// Phase5FindLuaVM.java
// Decompile FUN_140247a90 (Pump_Threads, RVA 0x247a90) and list every function
// it calls, so we can identify the Lua VM entry points (lua_resume, lua_status,
// lua_sethook, lua_error) needed for coroutine time-bounding.

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class Phase5FindLuaVM extends GhidraScript {

    static final long PUMPE_RVA = 0x247a90L;

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5FindLuaVM.log"));

        Address base   = currentProgram.getImageBase();
        Address pumpe  = base.add(PUMPE_RVA);

        Function fn = getFunctionAt(pumpe);
        if (fn == null) {
            out.println("ERROR: no function at " + pumpe);
            out.close();
            return;
        }
        out.printf("=== %s  RVA 0x%x ===\n\n", fn.getName(), PUMPE_RVA);

        // Decompile
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        DecompileResults dr = di.decompileFunction(fn, 60, monitor);
        if (dr.decompileCompleted()) {
            out.println("--- decompile ---");
            out.println(dr.getDecompiledFunction().getC());
        } else {
            out.println("Decompile failed: " + dr.getErrorMessage());
        }

        // All CALL targets in the function body
        out.println("--- callees ---");
        Listing listing = currentProgram.getListing();
        InstructionIterator it = listing.getInstructions(fn.getBody(), true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            if (!ins.getMnemonicString().equals("CALL")) continue;
            for (Reference ref : ins.getReferencesFrom()) {
                if (!ref.getReferenceType().isCall()) continue;
                Address tgt = ref.getToAddress();
                long rva = tgt.getOffset() - base.getOffset();
                Function tf = getFunctionAt(tgt);
                int sz = tf != null ? (int) tf.getBody().getNumAddresses() : -1;
                out.printf("  0x%x  (RVA 0x%x, %d bytes)  %s\n",
                    tgt.getOffset(), rva, sz,
                    tf != null ? tf.getName() : "???");

                // Decompile each callee so we can read its signature
                if (tf != null && sz > 0 && sz < 4000) {
                    DecompileResults cr = di.decompileFunction(tf, 30, monitor);
                    if (cr.decompileCompleted()) {
                        String c = cr.getDecompiledFunction().getC();
                        // Print just the first ~20 lines (signature + top of body)
                        String[] lines = c.split("\n");
                        int lim = Math.min(lines.length, 20);
                        for (int i = 0; i < lim; i++) out.println("    " + lines[i]);
                        if (lines.length > lim) out.println("    ...");
                    }
                    out.println();
                }
            }
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5FindLuaVM.log");
    }
}
