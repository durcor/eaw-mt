#!/usr/bin/env python3
"""PreToolUse hook: block Bash commands that write/delete outside the project directory."""
import sys
import json
import re
import shlex

PROJECT = "/home/ty/src/eaw-reverse-engineering"
ALLOWED_WRITE_PREFIXES  = (PROJECT, "/tmp", "/nix")
ALLOWED_DELETE_PREFIXES = (PROJECT, "/tmp")

def allowed_write(path):
    return any(path.startswith(p) for p in ALLOWED_WRITE_PREFIXES)

def allowed_delete(path):
    return any(path.startswith(p) for p in ALLOWED_DELETE_PREFIXES)

def positional_args(tokens):
    """Strip flags and their values from a token list, return bare path args."""
    # Flags that consume the next token as a value
    VALUE_FLAGS = {"-t", "-T", "-S", "-b", "--target-directory",
                   "--suffix", "--backup", "--mode", "-m"}
    out = []
    skip = False
    for t in tokens:
        if skip:
            skip = False
            continue
        if t in VALUE_FLAGS:
            skip = True
            continue
        if t.startswith("-"):
            continue
        out.append(t)
    return out

def check_cmd(tokens, blocked):
    if not tokens:
        return
    bin_name = tokens[0].split("/")[-1]
    args = tokens[1:]
    pos = positional_args(args)

    # Destination-write commands: last positional is the write target
    if bin_name in ("cp", "mv", "install"):
        if len(pos) >= 2:
            dest = pos[-1]
            if dest.startswith("/") and not allowed_write(dest):
                blocked.append(f"{bin_name} → {dest}")

    # Delete commands: every positional is a target
    elif bin_name in ("rm", "unlink", "rmdir"):
        for p in pos:
            if p.startswith("/") and not allowed_delete(p):
                blocked.append(f"{bin_name} → {p}")

    # Create/modify commands: every positional is a write target
    elif bin_name in ("mkdir", "touch", "truncate", "tee"):
        for p in pos:
            if p.startswith("/") and not allowed_write(p):
                blocked.append(f"{bin_name} → {p}")

    # Symlink: last positional is the link location (write)
    elif bin_name == "ln":
        if len(pos) >= 2:
            dest = pos[-1]
            if dest.startswith("/") and not allowed_write(dest):
                blocked.append(f"ln → {dest}")

    # chmod/chown with explicit paths — flag as blocked if outside project
    elif bin_name in ("chmod", "chown", "chattr"):
        # first positional is mode/owner spec, rest are paths
        for p in pos[1:]:
            if p.startswith("/") and not allowed_write(p):
                blocked.append(f"{bin_name} → {p}")


data = json.load(sys.stdin)
cmd = data.get("tool_input", {}).get("command", "")
blocked = []

# 1. Redirect operators and tee (>> > and tee -a)
for t in re.findall(r"(?:>>?|tee(?:\s+-a)?)\s+(/[^\s;|&\"']+)", cmd):
    if not allowed_write(t):
        blocked.append(f"redirect → {t}")

# 2. Explicit command invocations — split on shell separators
try:
    for subcmd in re.split(r"[;&\n]|\|\|?|&&", cmd):
        subcmd = subcmd.strip()
        if not subcmd:
            continue
        try:
            check_cmd(shlex.split(subcmd), blocked)
        except ValueError:
            pass  # unclosed quotes etc — don't block on parse errors
except Exception:
    pass

if blocked:
    print(json.dumps({
        "decision": "block",
        "reason": f"Operation targets path(s) outside project directory: {blocked}"
    }))
    sys.exit(2)
