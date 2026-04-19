// Phase 3gg: Find TheGameScoringManagerClass::Service (the per-frame Think) by:
// 1. Finding references to the debug string at RVA 0x874358 ("::Serv...")
// 2. Finding TheGameScoringManagerClass::vftable address and reading its slots
// 3. Decompiling the Service function (vftable[1] of GameScoringManagerClass)
//
// Also: find TheGameScoringManagerClass::vftable by scanning .rdata section for
// the address of FUN_140485340 (ctor) or reading RTTI structures.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;

public class Phase3ggServiceFn extends GhidraScript {

    static final long RVA_STR_SERV  = 0x874358L; // "TheGameScoringManagerClass::Serv..."
    static final long RVA_STR_ATTA  = 0x874398L; // "TheGameScoringManagerClass::Atta..." (Attach)
    static final long RVA_STR_RESET = 0x874450L; // "TheGameScoringManagerClass::Rese..." (Reset)
    static final long RVA_STR_GET2  = 0x8743d8L; // "TheGameScoringManagerClass::Get_..."

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3gg_service_fn.c"));

        // Find functions referencing the debug strings
        section(pw, "REFERENCES TO DEBUG STRINGS");
        long[] strAddrs = {RVA_STR_SERV, RVA_STR_ATTA, RVA_STR_RESET, RVA_STR_GET2};
        String[] strNames = {"::Service", "::Attach_Lua_Script", "::Reset", "::Get2"};
        Set<Long> serviceFnRvas = new HashSet<>();

        ReferenceManager refMgr = currentProgram.getReferenceManager();
        for (int k = 0; k < strAddrs.length; k++) {
            Address strAddr = currentProgram.getImageBase().add(strAddrs[k]);
            ReferenceIterator refs = refMgr.getReferencesTo(strAddr);
            pw.println("// String " + strNames[k] + " at RVA 0x" + Long.toHexString(strAddrs[k]) + ":");
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function fn = getFunctionContaining(ref.getFromAddress());
                if (fn != null) {
                    long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                    pw.println("//   Ref from fn RVA 0x" + Long.toHexString(fnRva) +
                        "  " + fn.getName() + "  " + fn.getBody().getNumAddresses() + " bytes");
                    if (k == 0) serviceFnRvas.add(fnRva); // Service string refs
                } else {
                    long refRva = ref.getFromAddress().getOffset() - imageBase;
                    pw.println("//   Ref from non-fn addr RVA 0x" + Long.toHexString(refRva));
                }
            }
        }
        pw.println();

        // Decompile all functions that reference the Service string
        for (long fnRva : serviceFnRvas) {
            section(pw, "Function referencing ::Service string: RVA 0x" + Long.toHexString(fnRva));
            decompile(decomp, pw, imageBase, fnRva, 300);
        }

        // Directly search symbol table for TheGameScoringManagerClass::vftable
        section(pw, "SYMBOL TABLE: TheGameScoringManagerClass vtable entries");
        SymbolTable symTable = currentProgram.getSymbolTable();
        SymbolIterator allSyms = symTable.getAllSymbols(true);
        List<Symbol> vtableSyms = new ArrayList<>();
        while (allSyms.hasNext()) {
            Symbol sym = allSyms.next();
            String name = sym.getName();
            long symRva = sym.getAddress().getOffset() - imageBase;
            if (symRva > 0 && symRva < 0x900000L &&
                name.contains("GameScoringManager") &&
                (name.contains("vtable") || name.contains("vftable"))) {
                vtableSyms.add(sym);
                pw.println("// VtableSym: " + name + " at RVA 0x" + Long.toHexString(symRva));
                // Read the first 8 vtable slots
                for (int i = 0; i < 8; i++) {
                    long slotRva = symRva + i * 8;
                    Address slotAddr = currentProgram.getImageBase().add(slotRva);
                    try {
                        long ptr = currentProgram.getMemory().getLong(slotAddr, true);
                        long fnRva2 = ptr - imageBase;
                        Function fn = getFunctionAt(currentProgram.getImageBase().add(fnRva2));
                        String fnName = (fn != null) ? fn.getName() : "?";
                        long sz = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                        pw.println("//   [" + i + "] RVA 0x" + Long.toHexString(fnRva2) +
                            "  " + fnName + "  " + sz + "b");
                    } catch (Exception e) {
                        pw.println("//   [" + i + "] unreadable");
                    }
                }
                pw.println();
            }
        }

        // If no named vtable symbol found, search via RTTI: look for
        // addresses near known fn in .rdata section
        if (vtableSyms.isEmpty()) {
            section(pw, "VTABLE SEARCH VIA RTTI: scanning .rdata for fn addresses");
            // TheGameScoringManagerClass is 0x20 bytes. Its ctor is FUN_140485340.
            // The vtable is referenced as the FIRST qword written in that ctor.
            // Let's look at the assembly of FUN_140485340 to find the immediate vtable address
            Function ctorFn = getFunctionAt(currentProgram.getImageBase().add(0x485340L));
            if (ctorFn != null) {
                pw.println("// Scanning FUN_140485340 for data references (vtable addr):");
                Address ctorStart = ctorFn.getBody().getMinAddress();
                Address ctorEnd = ctorFn.getBody().getMaxAddress();
                Address cur = ctorStart;
                while (cur != null && cur.compareTo(ctorEnd) <= 0) {
                    for (Reference ref : refMgr.getReferencesFrom(cur)) {
                        if (ref.getReferenceType().isData()) {
                            long refRva = ref.getToAddress().getOffset() - imageBase;
                            Symbol sym = getSymbolAt(ref.getToAddress());
                            String symName = (sym != null) ? sym.getName() : "?";
                            pw.println("//   DataRef to RVA 0x" + Long.toHexString(refRva) +
                                "  (" + symName + ")");
                            // This is likely the vtable — read its entries
                            for (int i = 0; i < 6; i++) {
                                long slotRva = refRva + i * 8;
                                Address slotAddr = currentProgram.getImageBase().add(slotRva);
                                try {
                                    long ptr = currentProgram.getMemory().getLong(slotAddr, true);
                                    long fnRva2 = ptr - imageBase;
                                    Function fn = getFunctionAt(currentProgram.getImageBase().add(fnRva2));
                                    String fnName = (fn != null) ? fn.getName() : "?";
                                    long sz = (fn != null) ? fn.getBody().getNumAddresses() : 0;
                                    pw.println("//   vtable[" + i + "] RVA 0x" +
                                        Long.toHexString(fnRva2) + "  " + fnName + "  " + sz + "b");
                                } catch (Exception e) {
                                    pw.println("//   vtable[" + i + "] unreadable");
                                }
                            }
                        }
                    }
                    try { cur = cur.add(1); } catch (Exception e) { break; }
                }
            }
        }

        pw.close();
        decomp.dispose();
        println("Phase 3gg output written to logs/phase3gg_service_fn.c");
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
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
            "  Name: " + fn.getName() +
            "  Size: " + fn.getBody().getNumAddresses() + " bytes");
        pw.println("// ------------------------------------------------------------");

        DecompileResults res = decomp.decompileFunction(fn, 240, monitor);
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
