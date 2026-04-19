// Phase 3d: Decompile the missed game-active callees and GOM-string function.
// Key targets found from full WinMain read:
//   - Two _guard_check_icall indirect calls (DAT_140b2b3a8 / DAT_140b2b2f0)
//   - FUN_14002ee40: post-scene-manager game-active callee
//   - FUN_140305890: another post-scene-manager game-active callee
//   - FUN_0x29efe0: contains "GOM" debug string
//   - FUN_14002ffb0: frame-ending game-active call
//   - FUN_140060330: final frame call (render/present?)
//   - FUN_1402af7c0, FUN_1402b0520: use "GameObjectManager" string
//   - Find where DAT_140b2b3a8 / DAT_140b2b2f0 are initialized
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Phase3dGomFinal extends GhidraScript {

    // Missed game-active callees from WinMain
    static final long RVA_POST_SCENE_A = 0x2ee40L;   // FUN_14002ee40 — game-active after scene mgr
    static final long RVA_POST_SCENE_B = 0x305890L;  // FUN_140305890 — game-active after scene mgr
    // GOM string holder
    static final long RVA_GOM_STR      = 0x29efe0L;  // FUN_14029efe0 — has "GOM" string
    // Frame-ending calls
    static final long RVA_FRAME_END_A  = 0x2ffb0L;   // FUN_14002ffb0
    static final long RVA_FRAME_END_B  = 0x60330L;   // FUN_140060330 (render/present?)
    // GameObjectManager string holders
    static final long RVA_GOM_MGR_A    = 0x2af7c0L;  // FUN_1402af7c0
    static final long RVA_GOM_MGR_B    = 0x2b0520L;  // FUN_1402b0520
    // GameObjectClass string holder
    static final long RVA_GOC          = 0x3a9e30L;  // FUN_1403a9e30
    // Indirect call targets: find who writes DAT_140b2b3a8 and DAT_140b2b2f0
    static final long DAT_ICALL_A      = 0xb2b3a8L;  // abs: 0x140b2b3a8
    static final long DAT_ICALL_B      = 0xb2b2f0L;  // abs: 0x140b2b2f0

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3d_gom_final.c"));

        // 1. Find who writes to the guard_check_icall targets
        section(pw, "XREFS TO DAT_140b2b3a8 and DAT_140b2b2f0 (guard_check_icall targets)");
        findWriters(pw, imageBase, decomp, new long[]{DAT_ICALL_A, DAT_ICALL_B});

        // 2. Post-scene-manager game-active callees
        section(pw, "POST-SCENE-MANAGER GAME-ACTIVE CALLEES");
        decompile(decomp, pw, imageBase, RVA_POST_SCENE_A, 3000);
        decompile(decomp, pw, imageBase, RVA_POST_SCENE_B, 3000);

        // 3. GOM string holder
        section(pw, "GOM STRING HOLDER — FUN_14029efe0");
        decompile(decomp, pw, imageBase, RVA_GOM_STR, 3000);

        // 4. Frame-ending calls
        section(pw, "FRAME-ENDING CALLS");
        decompile(decomp, pw, imageBase, RVA_FRAME_END_A, 2048);
        decompile(decomp, pw, imageBase, RVA_FRAME_END_B, 2048);

        // 5. GameObjectManager string holders
        section(pw, "GameObjectManager STRING HOLDERS");
        decompile(decomp, pw, imageBase, RVA_GOM_MGR_A, 2048);
        decompile(decomp, pw, imageBase, RVA_GOM_MGR_B, 2048);

        // 6. GameObjectClass string holder
        section(pw, "GameObjectClass STRING HOLDER");
        decompile(decomp, pw, imageBase, RVA_GOC, 2048);

        pw.close();
        decomp.dispose();
        println("Phase 3d output written to logs/phase3d_gom_final.c");
    }

    private void findWriters(PrintWriter pw, long imageBase,
                              DecompInterface decomp, long[] rvas) throws Exception {
        for (long rva : rvas) {
            Address dat = currentProgram.getImageBase().add(rva);
            pw.println("// --- xrefs to DAT_0x" + Long.toHexString(rva) + " ---");
            ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(dat);
            Set<Long> seen = new LinkedHashSet<>();
            while (refs.hasNext()) {
                Reference ref = refs.next();
                Function fn = getFunctionContaining(ref.getFromAddress());
                if (fn == null) continue;
                long fnRva = fn.getEntryPoint().getOffset() - imageBase;
                pw.println("//   " + ref.getReferenceType() +
                    " from FUN_0x" + Long.toHexString(fnRva) +
                    " @ site 0x" + Long.toHexString(ref.getFromAddress().getOffset() - imageBase));
                seen.add(fnRva);
            }
            // Decompile unique writers
            for (long fnRva : seen) {
                decompile(decomp, pw, imageBase, fnRva, 2048);
            }
        }
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
        // Dedup — skip if we already printed this RVA
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// ------------------------------------------------------------");
        pw.println("// RVA: 0x" + Long.toHexString(actualRva) +
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
