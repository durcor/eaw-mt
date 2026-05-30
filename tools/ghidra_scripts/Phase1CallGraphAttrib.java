//Phase 1: call-graph attribution. Assign still-DEFAULT (FUN_) functions to a class namespace
//when their callers unambiguously belong to a single class — i.e. private/non-virtual helpers.
//Conservative rule (per round): among a function's callers, consider only those already in a class
//namespace; attribute iff they ALL belong to ONE class AND (>=2 such callers, OR that caller is the
//function's sole caller). Keeps the FUN_ name (provenance: inferred owner, not an RTTI method name);
//only sets the parent namespace. Iterates to fixpoint so attribution propagates down the call tree.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.util.*;

public class Phase1CallGraphAttrib extends GhidraScript {

    boolean isClassNs(Namespace ns) {
        return ns != null && !ns.isGlobal() && (ns instanceof GhidraClass || ns.getSymbol().getSymbolType() == SymbolType.NAMESPACE);
    }

    @Override
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        Namespace global = currentProgram.getGlobalNamespace();

        // class-namespace owner per function address (null = unowned)
        Map<Function,Namespace> owner = new HashMap<>();
        List<Function> unowned = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) {
            Namespace ns = f.getParentNamespace();
            if (isClassNs(ns) && !ns.isGlobal()) owner.put(f, ns);
            else if (f.getName().startsWith("FUN_")) unowned.add(f);
        }
        println("seed class-owned functions: " + owner.size());
        println("unowned FUN_ candidates: " + unowned.size());

        int totalAttributed = 0, round = 0;
        boolean changed = true;
        while (changed && round < 12) {
            changed = false; round++;
            int roundAttr = 0;
            for (Iterator<Function> it = unowned.iterator(); it.hasNext();) {
                Function f = it.next();
                Set<Function> callers;
                try { callers = f.getCallingFunctions(monitor); } catch (Exception e) { continue; }
                if (callers.isEmpty()) continue;
                Namespace single = null; boolean conflict = false; int classCallers = 0;
                for (Function c : callers) {
                    Namespace cns = owner.get(c);
                    if (cns == null) continue;
                    classCallers++;
                    if (single == null) single = cns;
                    else if (!single.equals(cns)) { conflict = true; break; }
                }
                if (conflict || single == null) continue;
                boolean strong = (classCallers >= 2) || (callers.size() == 1);
                if (!strong) continue;
                try {
                    f.setParentNamespace(single);
                    owner.put(f, single);
                    it.remove();
                    roundAttr++; changed = true;
                } catch (Exception e) {}
            }
            totalAttributed += roundAttr;
            println("round " + round + ": attributed " + roundAttr);
        }
        println("TOTAL call-graph attributed: " + totalAttributed);
        println("remaining unowned FUN_: " + unowned.size());
    }
}
