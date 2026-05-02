// Phase5EntityAllocChain.java
// Trace FUN_14029f810 (GameObjectClass allocator, 2201 bytes) up to the
// loading/battle-mode boundary to confirm whether entities (and their
// pending coroutines) are created during loading or at battle start.
//
// Also decompile the XML batch-load callers of FUN_1402b0a20 (FUN_1402b4d70
// and FUN_14035f970) to determine if they run during the loading screen.
//
// If entity construction is confirmed during loading:
//   → coroutines are idle in thread tables from loading onward
//   → pre-warm by triggering Pump_Threads before SpaceMode activates
//   → hook point: SpaceMode ctor or vtable[22] first call

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class Phase5EntityAllocChain extends GhidraScript {

    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5EntityAllocChain.log"));

        Address base = currentProgram.getImageBase();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        // 1. FUN_14029f810 — entity allocator (caller of GameObjectClass::ctor)
        decompileWithCallers(out, di, base, 0x29f810L, "entity_alloc(29f810)", 80, 2);

        // 2. FUN_1402b4d70 — caller of XML batch loader (FUN_1402b0a20)
        decompileWithCallers(out, di, base, 0x2b4d70L, "xml_batch_load_caller(2b4d70)", 60, 2);

        // 3. FUN_14035f970 — other caller of FUN_1402b0a20 (3899 bytes — likely major load fn)
        decompileWithCallers(out, di, base, 0x35f970L, "load_fn(35f970)", 60, 2);

        // 4. FUN_1402b3960 — caller of FUN_14039fc90 (respawn/re-init path)
        decompileWithCallers(out, di, base, 0x2b3960L, "reinit_caller(2b3960)", 60, 2);

        // 5. Callers of FUN_14029f810 (entity_alloc) — who allocates entities?
        // Already shown above, but let's also go one level up from its callers
        out.println("=== SECOND-LEVEL CALLERS OF entity_alloc (0x29f810) ===\n");
        Address allocAddr = base.add(0x29f810L);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(allocAddr)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            if (caller == null) continue;
            long cRva = caller.getEntryPoint().getOffset() - base.getOffset();

            out.printf("  -- Callers of (caller 0x%x) --\n", cRva);
            for (ghidra.program.model.symbol.Reference r2 :
                    currentProgram.getReferenceManager()
                        .getReferencesTo(caller.getEntryPoint())) {
                if (!r2.getReferenceType().isCall()) continue;
                Address cs2 = r2.getFromAddress();
                Function c2 = getFunctionContaining(cs2);
                out.printf("    RVA 0x%x  in  %s  (fn 0x%x, %d bytes)\n",
                    cs2.getOffset() - base.getOffset(),
                    c2 != null ? c2.getName() : "???",
                    c2 != null ? c2.getEntryPoint().getOffset() - base.getOffset() : 0L,
                    c2 != null ? (int)c2.getBody().getNumAddresses() : -1);
            }
            out.println();
        }

        di.dispose();
        out.close();
        println("Done -> logs/Phase5EntityAllocChain.log");
    }

    void decompileWithCallers(PrintWriter out, DecompInterface di, Address base,
                              long rva, String label, int maxLines, int callerDepth)
        throws Exception
    {
        Address addr = base.add(rva);
        Function fn = getFunctionAt(addr);

        out.printf("=== %s (RVA 0x%x) ===\n", label, rva);
        if (fn == null) { out.printf("  [no function]\n\n"); return; }

        int sz = (int) fn.getBody().getNumAddresses();
        out.printf("  Name: %s  Size: %d bytes\n\n", fn.getName(), sz);

        DecompileResults dr = di.decompileFunction(fn, 60, monitor);
        if (dr.decompileCompleted()) {
            String[] lines = dr.getDecompiledFunction().getC().split("\n");
            int lim = Math.min(lines.length, maxLines);
            for (int i = 0; i < lim; i++) out.println(lines[i]);
            if (lines.length > lim) out.printf("  ... (%d more lines)\n", lines.length - lim);
        } else {
            out.println("  [decompile failed]");
        }
        out.println();

        out.printf("  -- Callers of RVA 0x%x --\n", rva);
        for (ghidra.program.model.symbol.Reference ref :
                currentProgram.getReferenceManager().getReferencesTo(addr)) {
            if (!ref.getReferenceType().isCall()) continue;
            Address cs = ref.getFromAddress();
            Function caller = getFunctionContaining(cs);
            long cRva = cs.getOffset() - base.getOffset();
            out.printf("    RVA 0x%x  in  %s  (fn RVA 0x%x, %d bytes)\n",
                cRva,
                caller != null ? caller.getName() : "???",
                caller != null ? caller.getEntryPoint().getOffset() - base.getOffset() : 0L,
                caller != null ? (int)caller.getBody().getNumAddresses() : -1);
        }
        out.println();
    }
}
