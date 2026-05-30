//Phase 2 struct recovery: for each vtable RVA passed as an arg, list the functions that reference
//its address (constructor/destructor/RTTI candidates) — the ctor is where field layout is set up.
//Usage: just ghidra-script won't pass args; invoke via the decomp recipe pattern. Args = hex vtable RVAs.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

public class Phase2VtableXrefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) { println("no vtable RVA args"); return; }
        long base = currentProgram.getImageBase().getOffset();
        FunctionManager fm = currentProgram.getFunctionManager();
        ReferenceManager rm = currentProgram.getReferenceManager();

        for (String a : args) {
            String h = a.startsWith("0x") ? a.substring(2) : a;
            long rva = Long.parseLong(h, 16);
            Address vt = toAddr(base + rva);
            println("=== vtable RVA 0x" + Long.toHexString(rva) + " (VA " + vt + ") ===");
            ReferenceIterator it = rm.getReferencesTo(vt);
            int n = 0;
            while (it.hasNext()) {
                Reference r = it.next();
                Address from = r.getFromAddress();
                Function f = fm.getFunctionContaining(from);
                String fn = (f != null)
                    ? f.getName(true) + " (RVA 0x" + Long.toHexString(f.getEntryPoint().getOffset() - base) + ")"
                    : "<no function>";
                println(String.format("  ref @0x%x  type=%s  in %s",
                    from.getOffset() - base, r.getReferenceType(), fn));
                n++;
            }
            if (n == 0) println("  (no references found — vtable may be reached only via RTTI COL)");
        }
    }
}
