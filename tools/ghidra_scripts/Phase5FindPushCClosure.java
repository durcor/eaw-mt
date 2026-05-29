// Phase 5 (Model B prereq, step 1): locate lua_pushcclosure.
// The base-lib luaL_Reg table is at rva 0x8fe120. The function that references it
// passes it to luaL_register, whose registration loop calls lua_pushcclosure with
// each entry's func field. We find the referencing function, decompile it and its
// callees (2 levels), so the decompile reveals luaL_register and lua_pushcclosure.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase5FindPushCClosure extends GhidraScript {

    static final long BASE_TABLE_RVA = 0x8fe120L;

    DecompInterface decomp;
    PrintWriter pw;
    long imageBase;
    Set<Long> done = new HashSet<>();

    void dump(Function f, int depth) throws Exception {
        if (f == null || depth < 0) return;
        long rva = f.getEntryPoint().getOffset() - imageBase;
        if (!done.add(rva)) return;
        pw.printf("%n// ===== %s  rva=0x%x  =====%n", f.getName(), rva);
        DecompileResults r = decomp.decompileFunction(f, 60, monitor);
        if (r != null && r.decompileCompleted())
            pw.println(r.getDecompiledFunction().getC());
        else
            pw.println("// <decompile failed>");
        if (depth > 0)
            for (Function c : f.getCalledFunctions(monitor))
                dump(c, depth - 1);
    }

    @Override
    public void run() throws Exception {
        imageBase = currentProgram.getImageBase().getOffset();
        decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5FindPushCClosure.out"));

        Address tbl = currentProgram.getImageBase().add(BASE_TABLE_RVA);
        pw.printf("// references to base-lib table @rva 0x%x:%n", BASE_TABLE_RVA);
        ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(tbl);
        Set<Function> openers = new LinkedHashSet<>();
        while (it.hasNext()) {
            Reference ref = it.next();
            Address from = ref.getFromAddress();
            Function f = getFunctionContaining(from);
            pw.printf("//   from 0x%x  in %s%n", from.getOffset() - imageBase,
                      f != null ? f.getName() : "<none>");
            if (f != null) openers.add(f);
        }
        // Decompile each opener and its callees two levels deep (opener -> luaL_register
        // -> lua_pushcclosure). Cap output by the `done` dedupe set.
        for (Function o : openers) dump(o, 2);

        pw.close();
        println("Phase5FindPushCClosure: openers=" + openers.size());
    }
}
