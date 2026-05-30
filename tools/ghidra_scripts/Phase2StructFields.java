//Phase 2 struct recovery workhorse: for a class namespace (arg), decompile every method in it and
//accumulate all `this`-relative field accesses (param_1 is `this` for thiscall methods). Outputs an
//empirical struct field map: byte-offset -> {access count, observed access widths}. The union across
//a class's methods is the struct's used-field set. Cap to avoid runaway on huge classes.
//Usage: just gs Phase2StructFields GameObjectClass
//@category EawMT

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.util.*;
import java.util.regex.*;
import java.io.*;

public class Phase2StructFields extends GhidraScript {

    // width inference from a cast token preceding the access
    static String widthOf(String cast) {
        if (cast == null) return "?";
        if (cast.contains("char") || cast.contains("undefined1") || cast.contains("byte") || cast.contains("int8")) return "1";
        if (cast.contains("short") || cast.contains("undefined2") || cast.contains("int16")) return "2";
        if (cast.contains("float")) return "4f";
        if (cast.contains("double")) return "8d";
        if (cast.contains("undefined8") || cast.contains("longlong") || cast.contains("int64") || cast.contains("code *") || cast.contains(" *")) return "8";
        if (cast.contains("int") || cast.contains("undefined4") || cast.contains("uint") || cast.contains("int32")) return "4";
        return "?";
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) { println("need a class namespace name"); return; }
        String cls = args[0];
        int cap = args.length > 1 ? Integer.parseInt(args[1]) : 80;

        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface dc = new DecompInterface();
        dc.openProgram(currentProgram);

        // collect functions whose parent namespace name == cls
        List<Function> methods = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) {
            Namespace ns = f.getParentNamespace();
            if (ns != null && !ns.isGlobal() && ns.getName().equals(cls)) methods.add(f);
        }
        println("class " + cls + ": " + methods.size() + " methods (decompiling up to " + cap + ")");

        // offset -> width -> count
        TreeMap<Long,Map<String,Integer>> fields = new TreeMap<>();
        // patterns over decompiled C, all relative to param_1
        //  *(TYPE *)(param_1 + 0xNN)   |  *(TYPE *)((longlong)param_1 + 0xNN)  | param_1[k] | param_1 + 0xNN
        Pattern pCast = Pattern.compile("\\*\\(([^)]+?) \\*\\)\\((?:\\(longlong\\))?param_1 \\+ (0x[0-9a-f]+)\\)");
        Pattern pIdx  = Pattern.compile("param_1\\[(0x[0-9a-f]+|\\d+)\\]");
        Pattern pBare = Pattern.compile("(?:\\(longlong\\))?param_1 \\+ (0x[0-9a-f]+)");

        int done = 0;
        for (Function f : methods) {
            if (done >= cap) break;
            DecompileResults r = dc.decompileFunction(f, 60, monitor);
            if (r == null || !r.decompileCompleted()) continue;
            String c = r.getDecompiledFunction().getC();
            done++;
            Matcher m;
            m = pCast.matcher(c);
            while (m.find()) addField(fields, Long.parseLong(m.group(2).substring(2), 16), widthOf(m.group(1)));
            m = pIdx.matcher(c);
            while (m.find()) {
                String g = m.group(1);
                long k = g.startsWith("0x") ? Long.parseLong(g.substring(2),16) : Long.parseLong(g);
                addField(fields, k * 8, "8");   // undefined8* indexing
            }
            m = pBare.matcher(c);
            while (m.find()) addField(fields, Long.parseLong(m.group(1).substring(2), 16), "&");
        }
        dc.dispose();

        File of = new File("/home/ty/src/eaw-reverse-engineering/logs/Phase2Fields_" + cls + ".tsv");
        PrintWriter pw = new PrintWriter(new FileWriter(of));
        pw.println("offset\ttotal\twidths");
        for (Map.Entry<Long,Map<String,Integer>> e : fields.entrySet()) {
            int tot = 0; StringBuilder w = new StringBuilder();
            for (Map.Entry<String,Integer> we : e.getValue().entrySet()) {
                tot += we.getValue();
                if (w.length()>0) w.append(",");
                w.append(we.getKey()).append(":").append(we.getValue());
            }
            pw.println(String.format("0x%x\t%d\t%s", e.getKey(), tot, w.toString()));
        }
        pw.close();
        println("methods decompiled: " + done + "  distinct field offsets: " + fields.size());
        println("wrote logs/Phase2Fields_" + cls + ".tsv");
    }

    static void addField(TreeMap<Long,Map<String,Integer>> fields, long off, String w) {
        if (off < 0 || off > 0x100000) return;
        fields.computeIfAbsent(off, k -> new TreeMap<>()).merge(w, 1, Integer::sum);
    }
}
