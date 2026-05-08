# CLAUDE.md — EaW Reverse Engineering Project

This file is read at the start of every session. Rules here are mandatory and supplement the operational rules in `EaW_Multithread_Blueprint.md` Section 0. Read both before doing anything.

---

## Agent Model Policy

When spawning subagents via the `Agent` tool, you MUST select the model according to the task type below. Do not use the default (inheriting from parent) unless no category fits.

| Task category | Model | Subagent type | Examples |
|---|---|---|---|
| Log/file scanning, grep, glob, pattern matching | `haiku` | `Explore` | Scanning `logs/*.c` for vtable patterns, finding xrefs, searching function lists |
| Spec writing, decompile interpretation, synthesis | `sonnet` | `general-purpose` | Drafting `openspec/` entries, interpreting decompiled C output |
| Architectural decisions, threading safety analysis, contradictory-finding resolution | `opus` | `general-purpose` | Thread ownership of GOM tick, renderer affinity, multi-subsystem threading invariants |
| Ghidra script execution, git operations | Bash directly — no Agent needed | — | Running `analyzeHeadless`, committing findings |

**Rules:**
- Never spawn an `opus` agent for tasks that fit `haiku` or `sonnet`. If uncertain, use `sonnet`.
- Spawn `haiku` Explore agents aggressively for all log-scanning passes — they are cheap and fast.
- Never spawn an agent just to run a Bash command. Use the Bash tool directly.
- Escalate to `opus` only for sessions where you are stuck on a genuinely ambiguous architectural question. Get user sign-off before doing so (it's also a human validation gate per Blueprint rule 6).

---

## Session Startup Checklist

1. Read `EaW_Multithread_Blueprint.md` Section 0 (Operational Rules).
2. Read the relevant `openspec/` files for the subsystem being worked on this session.
3. Check `git status` — confirm no uncommitted findings from the previous session.
4. State the current phase and what you intend to accomplish before starting any analysis.

---

## Build and Deploy Discipline

- **Always use `just build-winmm` to compile and deploy the hook DLL.** Never roll a manual `gcc` + `cp` sequence — `just build-winmm` does both atomically and uses the correct nix-shell toolchain.
- Do not invoke `nix develop --command gcc ...` directly for hook builds; that bypasses the justfile and risks mismatched flags or a stale copy in the game directory.

---

## Commit Discipline

- Commit after every confirmed finding committed to `openspec/`. Do not batch multiple discoveries into one commit.
- Commit message format: `analysis: <what was confirmed> — <where it's documented>`
- Never commit hypotheses as facts. If a finding is unconfirmed, it goes in a `TODO:` block in the spec, not a committed entry.
