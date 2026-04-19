// Phase 3q: Follow FUN_14027ff80 (per-player per-frame tick from FUN_1404d9cd0).
// FUN_1404d9cd0 iterates all players and calls FUN_14027ff80(player, mode) each frame.
// FUN_14027ff80 is the per-player tick — it likely calls GOM.Think_All_Objects for that player.
// Also check FUN_14035cb10 (real function behind slot-3 thunk in both Space/Land vtables).
// And FUN_1403bd080 (allocator of mode+0x1f0, size 0x148 — could be the sim manager).
// Also look at functions near GOM dtor (RVA 0x29c800) — Think/Service likely nearby.
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import java.io.*;

public class Phase3qPlayerTick extends GhidraScript {

    // Per-player per-frame tick
    static final long RVA_27FF80  = 0x27ff80L; // FUN_14027ff80 — per-player frame tick

    // Slot-3 real function (thunk target)
    static final long RVA_35CB10  = 0x35cb10L; // FUN_14035cb10 — thunk target for slot 3

    // Allocator of mode+0x1f0 (size 0x148) — conditional combat sim manager
    static final long RVA_3BD080  = 0x3bd080L; // FUN_1403bd080 — alloc mode+0x1f0 subsystem

    // Functions in GOM neighborhood — potential Think/Service candidates
    // GOM ctor at 0x29a0c0 (1753 bytes) → ends ~0x29a7a9
    // GOM dtor at 0x29c800 (1971 bytes) → ends ~0x29cff3
    // Check: 0x29a800-0x29b3c0 range (between ctor end and 0x29b3c0)
    static final long RVA_29B020  = 0x29b020L; // GOM function candidate A
    static final long RVA_29C500  = 0x29c500L; // GOM function candidate B (pre-dtor)
    static final long RVA_29D000  = 0x29d000L; // GOM function candidate C (post-dtor)
    static final long RVA_29E000  = 0x29e000L; // GOM function candidate D

    @Override
    public void run() throws Exception {
        long imageBase = currentProgram.getImageBase().getOffset();
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new FileWriter(
            "/home/ty/src/eaw-reverse-engineering/logs/phase3q_player_tick.c"));

        section(pw, "FUN_14027ff80 — per-player per-frame tick (MAIN CANDIDATE)");
        decompile(decomp, pw, imageBase, RVA_27FF80, 600);

        section(pw, "FUN_14035cb10 — slot-3 real function (both Space+Land modes)");
        decompile(decomp, pw, imageBase, RVA_35CB10, 300);

        section(pw, "FUN_1403bd080 — alloc mode+0x1f0 subsystem (size 0x148)");
        decompile(decomp, pw, imageBase, RVA_3BD080, 300);

        section(pw, "GOM neighborhood A — RVA ~0x29b020 (between GOM ctor end and b3c0)");
        decompileNeighbor(decomp, pw, imageBase, RVA_29B020, 200);

        section(pw, "GOM neighborhood B — RVA ~0x29c500 (just before GOM dtor)");
        decompileNeighbor(decomp, pw, imageBase, RVA_29C500, 200);

        section(pw, "GOM neighborhood C — RVA ~0x29d000 (just after GOM dtor end)");
        decompileNeighbor(decomp, pw, imageBase, RVA_29D000, 200);

        section(pw, "GOM neighborhood D — RVA ~0x29e000");
        decompileNeighbor(decomp, pw, imageBase, RVA_29E000, 200);

        pw.close();
        decomp.dispose();
        println("Phase 3q output written to logs/phase3q_player_tick.c");
    }

    private void section(PrintWriter pw, String title) {
        pw.println("\n// ================================================================");
        pw.println("// " + title);
        pw.println("// ================================================================\n");
    }

    // decompileNeighbor: if exact address has no function, try getFunctionContaining
    private void decompileNeighbor(DecompInterface decomp, PrintWriter pw,
                                    long imageBase, long rva, int maxLines) throws Exception {
        Address addr = currentProgram.getImageBase().add(rva);
        Function fn = getFunctionAt(addr);
        if (fn == null) fn = getFunctionContaining(addr);
        if (fn == null) {
            pw.println("// No function found near RVA 0x" + Long.toHexString(rva));
            // Try scanning forward up to 0x100 bytes
            for (long delta = 8; delta <= 0x100; delta += 8) {
                Address a2 = currentProgram.getImageBase().add(rva + delta);
                fn = getFunctionAt(a2);
                if (fn != null) break;
            }
            if (fn == null) { pw.println("// (scan found nothing)\n"); return; }
        }
        long actualRva = fn.getEntryPoint().getOffset() - imageBase;
        pw.println("// Found function at RVA 0x" + Long.toHexString(actualRva));
        decompile(decomp, pw, imageBase, actualRva, maxLines);
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

        DecompileResults res = decomp.decompileFunction(fn, 180, monitor);
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
