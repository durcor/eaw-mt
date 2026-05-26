'use strict';

// Trace alHModel constructor (FUN_14012b330) calls where param_2's
// render-pass container has a null +8 field — the exact crash precondition
// at RVA 0x12b6bf.
//
// Usage:
//   frida -n StarWarsG.exe -l trace_alHModel_crash.js
//
// What to look for in output:
//   has_null=true  -> this invocation is the bad one
//   Backtrace containing rva~0x247a90 (Pump_Threads) or rva~0x7bc310
//   (lua_resume) -> called from inside a Lua coroutine (prewarm side-effect)
//   Backtrace without those -> indirect corruption (game code post-prewarm)

const base = Module.getBaseAddress('StarWarsG.exe');
if (!base) {
    console.error('[trace] ERROR: StarWarsG.exe not found');
} else {
    console.log('[trace] StarWarsG.exe base: ' + base);
}

// Known RVA ranges for context labelling
const LUA_VM_LO  = 0x7b0000;
const LUA_VM_HI  = 0x7d0000;
const PUMP_RVA   = 0x247a90;
const RESUME_RVA = 0x7bc310;

function labelRva(rva) {
    if (rva >= LUA_VM_LO && rva < LUA_VM_HI) return ' [LuaVM]';
    if (Math.abs(rva - PUMP_RVA)   < 0x200)  return ' [Pump_Threads]';
    if (Math.abs(rva - RESUME_RVA) < 0x100)  return ' [lua_resume]';
    return '';
}

let hitCount = 0;

Interceptor.attach(base.add(0x12b330), {
    onEnter(args) {
        const param2 = args[1];

        // Scan the render-pass container at param2+0xf0..+0xf8 for null +8 fields.
        let hasNull   = false;
        let nullIdx   = -1;
        let elemCount = 0;
        try {
            const arrStart = param2.add(0xf0).readPointer();
            const arrEnd   = param2.add(0xf8).readPointer();
            if (!arrStart.isNull() && !arrEnd.isNull()) {
                const byteSpan = arrEnd.sub(arrStart).toInt32();
                if (byteSpan >= 0 && byteSpan < 0x10000) {
                    elemCount = byteSpan / 16;
                    for (let i = 0; i < elemCount; i++) {
                        const field8 = arrStart.add(i * 16 + 8).readPointer();
                        if (field8.isNull()) {
                            hasNull = true;
                            nullIdx = i;
                            break;
                        }
                    }
                }
            }
        } catch (e) {
            hasNull = true;
        }

        // Always log first 2 calls (sanity) + every null hit.
        if (!hasNull && hitCount >= 2) return;
        hitCount++;

        const bt = Thread.backtrace(this.context, Backtracer.FUZZY)
            .map(addr => {
                const rva = addr.sub(base).toInt32();
                return `  rva=0x${(rva >>> 0).toString(16).padStart(8,'0')}${labelRva(rva >>> 0)}`;
            })
            .join('\n');

        console.log(
            `\n[alHModel#${hitCount}] param1=${args[0]} param2=${param2}` +
            ` elems=${elemCount} has_null=${hasNull}` +
            (hasNull ? ` null_idx=${nullIdx}` : '') +
            `\n${bt}`
        );
    }
});

console.log('[trace] alHModel ctor hook active at rva=0x12b330');
console.log('[trace] Waiting for model construction with null render-pass element...');
