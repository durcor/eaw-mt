// Phase 5 (Model B prereq, step 2): enumerate the OO Lua game-API closures.
// lua_pushcclosure = FUN_1407b9340 (rva 0x7b9340), signature (L /*rcx*/, fn /*rdx*/, n /*r8d*/).
// Direct (OO) registrations load the fn with `LEA RDX,[fn]` (an immediate code address) right
// before the call; the luaL_register stdlib loop loads fn from memory (no code reference) and
// is excluded automatically. For each CALL site we walk backward to the nearest write of RDX
// that carries a reference into executable memory, and report that fn as a registered closure.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.lang.Register;
import java.io.*;
import java.util.*;

public class Phase5EnumGameApi extends GhidraScript {

    static final long PUSHCCLOSURE_RVA = 0x7b9340L;
    static final long LUAL_REGISTER_RVA = 0x7ce7a0L; // exclude calls originating here

    boolean isExec(Address a) {
        MemoryBlock b = currentProgram.getMemory().getBlock(a);
        return b != null && b.isExecute();
    }

    @Override
    public void run() throws Exception {
        long base = currentProgram.getImageBase().getOffset();
        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/Phase5EnumGameApi.out"));

        Address pcc = currentProgram.getImageBase().add(PUSHCCLOSURE_RVA);
        Listing listing = currentProgram.getListing();
        Register rdx = currentProgram.getRegister("RDX");

        TreeSet<Long> gameFns = new TreeSet<>();
        int callSites = 0, resolved = 0, fromRegister = 0;

        ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(pcc);
        while (it.hasNext()) {
            Reference ref = it.next();
            if (!ref.getReferenceType().isCall()) continue;
            callSites++;
            Address callAddr = ref.getFromAddress();
            Function caller = getFunctionContaining(callAddr);
            long callerRva = caller != null ? caller.getEntryPoint().getOffset() - base : -1;
            if (callerRva == LUAL_REGISTER_RVA) { fromRegister++; continue; }

            // walk backward for the nearest RDX-write that references executable memory
            Instruction ins = listing.getInstructionBefore(callAddr);
            long fnRva = -1;
            for (int steps = 0; steps < 16 && ins != null; steps++) {
                if (caller != null && !caller.getBody().contains(ins.getAddress())) break;
                Register dst = ins.getRegister(0);
                if (dst != null && dst.equals(rdx)) {
                    for (Reference r : ins.getReferencesFrom()) {
                        Address t = r.getToAddress();
                        if (t != null && isExec(t)) { fnRva = t.getOffset() - base; break; }
                    }
                    break; // first RDX writer decides (whether or not it had a code ref)
                }
                ins = ins.getPrevious();
            }
            if (fnRva >= 0) {
                resolved++;
                gameFns.add(fnRva);
                pw.printf("call@0x%-8x caller=0x%-8x  registers fn_rva=0x%x%n",
                          callAddr.getOffset() - base, callerRva, fnRva);
            } else {
                pw.printf("call@0x%-8x caller=0x%-8x  (fn from register/memory — unresolved)%n",
                          callAddr.getOffset() - base, callerRva);
            }
        }

        pw.printf("%n=== distinct game-API closures (fn rvas) ===%n");
        for (Long r : gameFns) pw.printf("0x%x%n", r);
        pw.printf("%n=== SUMMARY: %d call sites, %d via luaL_register(excluded), "
                + "%d resolved, %d distinct fns ===%n",
                callSites, fromRegister, resolved, gameFns.size());
        pw.close();
        println("Phase5EnumGameApi: callSites=" + callSites + " distinctFns=" + gameFns.size());
    }
}
