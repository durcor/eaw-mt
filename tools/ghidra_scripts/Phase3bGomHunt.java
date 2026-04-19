// Phase 3b: GOM hunt — trace scene manager sub-calls and re-examine audio tick.
// FUN_140305c40 is called unconditionally from scene manager every frame.
// FUN_14021caf0 may contain GOM update beyond the audio calls.
// Also trace FUN_1402fd330 and callers of GameObjectClass::Think/Update via RTTI xrefs.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3bGomHunt extends GhidraScript {

    // Unconditional scene manager sub-call (called from FUN_14030c3b0 line ~1799)
    static final long RVA_SCENE_SUB       = 0x305c40L;
    // Scene manager flag-gated sub-call (when +0x48 != 0)
    static final long RVA_SCENE_FD330     = 0x2fd330L;
    // "Primary game-state update" / audio tick — re-examine full body
    static final long RVA_AUDIO_OR_GOM    = 0x21caf0L;
    // Sub-calls of FUN_1402d72c0 that may do entity work
    static final long RVA_D7450           = 0x2d7450L;
    static final long RVA_D7A40           = 0x2d7a40L;

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3b_gom_hunt.c"));

        // 1. Scene manager unconditional sub-call
        section(pw, "SCENE MANAGER UNCONDITIONAL SUB-CALL (FUN_140305c40)");
        decompile(decomp, pw, imageBase, RVA_SCENE_SUB, 4000);

        // 2. Scene manager flag-gated sub-call
        section(pw, "SCENE MANAGER FLAG-GATED SUB-CALL (FUN_1402fd330)");
        decompile(decomp, pw, imageBase, RVA_SCENE_FD330, 2048);

        // 3. Re-examine the so-called audio tick — full decompile
        section(pw, "FULL DECOMPILE OF FUN_14021caf0 (audio / GOM?)");
        decompile(decomp, pw, imageBase, RVA_AUDIO_OR_GOM, 5000);

        // 4. Sub-calls from FUN_1402d72c0
        section(pw, "FUN_1402d72c0 SUB-CALLS");
        decompile(decomp, pw, imageBase, RVA_D7450, 1024);
        decompile(decomp, pw, imageBase, RVA_D7A40, 1024);

        // 5. Scan xrefs to GameObjectClass vtable entries — find who calls Think/Update
        section(pw, "XREFS TO GameObjectClass VTABLE ENTRIES");
        scanGameObjectXrefs(pw, imageBase);

        pw.close();
        decomp.dispose();
        println("Phase 3b output written to logs/phase3b_gom_hunt.c");
    }

    private void scanGameObjectXrefs(PrintWriter pw, long imageBase) throws Exception {
        // Search for the RTTI label by name
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator syms = symTable.getSymbols("GameObjectClass");
        while (syms.hasNext()) {
            Symbol sym = syms.next();
            pw.println("// Symbol: " + sym.getName() + " @ " +
                Long.toHexString(sym.getAddress().getOffset() - imageBase) +
                " (type: " + sym.getSymbolType() + ")");
        }

        // Also try the vtable symbol
        String[] searchNames = {
            "GameObjectClass::vftable",
            "GameObjectManagerClass::vftable",
            "GameObjectClass::Think",
            "GameObjectClass::Update",
            "GameObjectManagerClass",
        };
        for (String name : searchNames) {
            SymbolIterator it = symTable.getSymbols(name);
            while (it.hasNext()) {
                Symbol sym = it.next();
                long rva = sym.getAddress().getOffset() - imageBase;
                pw.println("// Found symbol: " + sym.getName() + " @ RVA 0x" + Long.toHexString(rva));
                ReferenceIterator refs = currentProgram.getReferenceManager()
                    .getReferencesTo(sym.getAddress());
                int count = 0;
                while (refs.hasNext() && count < 20) {
                    Reference ref = refs.next();
                    Function fn = getFunctionContaining(ref.getFromAddress());
                    if (fn != null) {
                        long fromRva = fn.getEntryPoint().getOffset() - imageBase;
                        pw.println("//   xref from FUN_0x" + Long.toHexString(fromRva) +
                            " (site 0x" + Long.toHexString(ref.getFromAddress().getOffset() - imageBase) + ")");
                    }
                    count++;
                }
            }
        }
        pw.println();
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }

    private void decompile(DecompInterface decomp, PrintWriter pw,
                            long imageBase, long rva, int maxLines) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function at RVA 0x" + Long.toHexString(rva) + "\n"); return;
        }
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(rva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");

        DecompileResults res = decomp.decompileFunction(fn, 120, monitor);
        if (!res.decompileCompleted()) {
            pw.println("// DECOMPILE FAILED: " + res.getErrorMessage() + "\n"); return;
        }
        String code = res.getDecompiledFunction().getC();
        String[] lines = code.split("\n");
        if (lines.length > maxLines) {
            for (int i = 0; i < maxLines; i++) pw.println(lines[i]);
            pw.println("// ... [truncated at " + maxLines + " lines, total " + lines.length + "]");
        } else {
            pw.println(code);
        }
        pw.println();
    }
}
