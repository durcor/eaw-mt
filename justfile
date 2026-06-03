wine         := "/nix/store/dp1hc4npi0520afxz7ama2h048ssi2y3-proton-ge-bin-GE-Proton10-34-steamcompattool/files/bin/wine64"
proton       := "/nix/store/dp1hc4npi0520afxz7ama2h048ssi2y3-proton-ge-bin-GE-Proton10-34-steamcompattool/proton"
steam-client := env('HOME') + "/.local/share/Steam"

# NOTE: claude doesn't like escaped whitespace in paths
# game-base := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'"
game-base := env('HOME') + "/gam/steam/steamapps/common/sweaw"

game-dir  := env('HOME') + "/gam/steam/steamapps/common/sweaw/corruption"
compat-data := env('HOME') + "/gam/steam/steamapps/compatdata/32470"
log-file  := env('HOME') + "/gam/steam/steamapps/common/sweaw/corruption/eaw-mt.log"
# Virtual-desktop resolution for launch-foc-desktop. Override: just desktop-res=2560x1440 launch-foc-desktop
desktop-res := "1920x1080"
# Differential-test golden-trace capture (profile build only). Override: just difftrace=1 launch-foc-desktop
difftrace := ""
# Runtime transform-offset finder (profile build only). Override: just offwatch=1 launch-foc-desktop
offwatch := ""
save-dir  := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx/drive_c/users/steamuser/Saved Games/Petroglyph/Empire At War - Forces of Corruption/Save"

export WINEPREFIX := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx"

# RELEASE build (default): shipped fixes only, profiler compiled out.
build-winmm:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

# PROFILE build: adds -DEAW_PROFILE → full per-function timing profiler + stats dump.
build-winmm-profile:
  nix develop --command x86_64-w64-mingw32-gcc -DEAW_PROFILE -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

# ORACLE build: adds -DEAW_ORACLE → the DT* differential-oracle inline-trampoline hooks ONLY (no
# per-function timing profiler, no body-patching subcallee hooks). Use this for oracle captures: it is
# far less perturbative and avoids the body-patcher battle-load crash (c0000005 in b375380). Still
# runtime-gated by EAW_DIFFTRACE=1. Shares the winmm.dll output path, so rebuild release after.
build-winmm-oracle:
  nix develop --command x86_64-w64-mingw32-gcc -DEAW_ORACLE -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

# Compile + run the lifted sim-core host validation tests (sim/). No game needed.
sim-test:
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/tick_clock_test.cpp sim/tick_clock.cpp -o /tmp/eaw_tick_clock_test
  /tmp/eaw_tick_clock_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/entity_spine_test.cpp sim/entity_spine.cpp -o /tmp/eaw_entity_spine_test
  /tmp/eaw_entity_spine_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/locomotor_test.cpp sim/locomotor.cpp -o /tmp/eaw_locomotor_test
  /tmp/eaw_locomotor_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/hardpoint_test.cpp sim/hardpoint.cpp sim/sim_rng.cpp -o /tmp/eaw_hardpoint_test
  /tmp/eaw_hardpoint_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/sim_rng_test.cpp sim/sim_rng.cpp -o /tmp/eaw_sim_rng_test
  /tmp/eaw_sim_rng_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/signal_dispatch_test.cpp sim/signal_dispatch.cpp -o /tmp/eaw_signal_dispatch_test
  /tmp/eaw_signal_dispatch_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/sfx_channel_test.cpp sim/sfx_channel.cpp sim/sim_rng.cpp -o /tmp/eaw_sfx_channel_test
  /tmp/eaw_sfx_channel_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/damage_tracking_test.cpp sim/damage_tracking.cpp -o /tmp/eaw_damage_tracking_test
  /tmp/eaw_damage_tracking_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/energy_pool_test.cpp sim/energy_pool.cpp -o /tmp/eaw_energy_pool_test
  /tmp/eaw_energy_pool_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/ability_countdown_test.cpp sim/ability_countdown.cpp -o /tmp/eaw_ability_countdown_test
  /tmp/eaw_ability_countdown_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/unit_ai_test.cpp sim/unit_ai.cpp -o /tmp/eaw_unit_ai_test
  /tmp/eaw_unit_ai_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/nebula_test.cpp sim/nebula.cpp -o /tmp/eaw_nebula_test
  /tmp/eaw_nebula_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/asteroid_field_damage_test.cpp sim/asteroid_field_damage.cpp sim/sim_rng.cpp -o /tmp/eaw_asteroid_field_damage_test
  /tmp/eaw_asteroid_field_damage_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/targeting_dispatch_test.cpp sim/targeting_dispatch.cpp -o /tmp/eaw_targeting_dispatch_test
  /tmp/eaw_targeting_dispatch_test
  nix develop --command g++ -std=c++17 -O2 -Wall -Wextra -Isim \
    sim/tests/telekinesis_target_test.cpp sim/telekinesis_target.cpp -o /tmp/eaw_telekinesis_target_test
  /tmp/eaw_telekinesis_target_test

ghidra-script script:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis \
    -scriptPath tools/ghidra_scripts \
    -postScript {{script}}.java \
    -log logs/{{script}}.log

# Run a Ghidra script with trailing args (read-only). e.g. just gs Phase2VtableXrefs 8661b8 869ea0
gs script +args:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis -readOnly \
    -scriptPath tools/ghidra_scripts \
    -postScript {{script}}.java {{args}} \
    -log logs/{{script}}.log

# Decompile one or more RVAs (hex) to decomp/<rva>.c using current RTTI-applied names.
# e.g. just decomp 387400 3a76b0 28d400
decomp +rvas:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis -readOnly \
    -scriptPath tools/ghidra_scripts \
    -postScript DecompileAt.java {{rvas}} \
    -log logs/DecompileAt.log

# Decompile every RVA listed (one hex RVA per line) in the given file.
decomp-list file:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis -readOnly \
    -scriptPath tools/ghidra_scripts \
    -postScript DecompileAt.java $(tr '\n' ' ' < {{file}}) \
    -log logs/DecompileAt.log

build-hook:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/eaw-mt-hook.dll hooks/eaw-mt-hook.c -lkernel32

  cp patches/experimental/eaw-mt-hook.dll {{game-dir}}
  cp patches/experimental/eaw-mt-hook.dll $WINEPREFIX/drive_c/windows/system32

  {{wine}} reg add \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v AppInit_DLLs /t REG_SZ /d "eaw-mt-hook.dll" /f
  {{wine}} reg add \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v LoadAppInit_DLLs /t REG_DWORD /d 1 /f

remove-hook:
  {{wine}} reg delete \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v AppInit_DLLs /f

# Launch FoC+TR directly via wine64 inside steam-run's FHS env.
# Requires patched ntdll.so (fixes wine-staging 10.0 server_get_name_info NULL memcpy crash).
# unshare --mount shadows the Nix store ntdll.so with our patched copy before bwrap's --bind /nix.
# SteamAppId/SteamGameId/STEAM_COMPAT_CLIENT_INSTALL_PATH required for lsteamclient.dll IPC handshake.
# d3d9=n,b loads the DXVK d3d9.dll from the Wine prefix (drive_c/windows/system32/d3d9.dll) instead
# of wined3d's built-in software renderer, which is unstable and crashes during D3D resource teardown.
launch-foc:
  unshare --mount --map-root-user bash -c " \
    mount --bind {{justfile_directory()}}/patches/wine/ntdll.so \
      /nix/store/jckwa9yy4aj2yq6r3c9x4rk0g95cvwm9-source/files/lib/wine/x86_64-unix/ntdll.so && \
    capsh --inh='' -- -c ' \
      WINEPREFIX={{compat-data}}/pfx \
      WINEDLLOVERRIDES=winmm=n,b \
      SteamAppId=32470 \
      SteamGameId=32470 \
      STEAM_COMPAT_CLIENT_INSTALL_PATH=/home/ty/.local/share/Steam \
      steam-run {{wine}} \
      {{game-dir}}/StarWarsG.exe MODPATH=Mods/Imperial_Civil_War \
    ' \
  "

# Launch FoC+TR inside a Wine virtual desktop ("eaw" window at {{desktop-res}}).
# Running under explorer /desktop keeps the game in a contained top-level window that
# holds focus reliably and believes it is always foreground, so the game no longer
# pauses/halts when the pointer leaves the window (focus-follows-mouse WMs). Identical
# env to launch-foc; only difference is the `explorer /desktop=` wrapper before the exe.
launch-foc-desktop:
  unshare --mount --map-root-user bash -c " \
    mount --bind {{justfile_directory()}}/patches/wine/ntdll.so \
      /nix/store/jckwa9yy4aj2yq6r3c9x4rk0g95cvwm9-source/files/lib/wine/x86_64-unix/ntdll.so && \
    capsh --inh='' -- -c ' \
      WINEPREFIX={{compat-data}}/pfx \
      WINEDLLOVERRIDES=winmm=n,b \
      EAW_DIFFTRACE={{difftrace}} \
      EAW_OFFWATCH={{offwatch}} \
      SteamAppId=32470 \
      SteamGameId=32470 \
      STEAM_COMPAT_CLIENT_INSTALL_PATH=/home/ty/.local/share/Steam \
      steam-run {{wine}} \
      explorer /desktop=eaw,{{desktop-res}} \
      {{game-dir}}/StarWarsG.exe MODPATH=Mods/Imperial_Civil_War \
    ' \
  "

# Kill game, rebuild hook, then launch FoC.
cycle: kill-game build-winmm launch-foc

# Wait for StarWarsG.exe to appear (up to 60s), then print its PID.
wait-game:
  timeout 60 sh -c 'until pgrep -x StarWarsG.exe > /dev/null; do sleep 1; done && pgrep -x StarWarsG.exe'

# Kill a running game instance.
kill-game:
  pkill -x StarWarsG.exe || true

# List save files sorted by modification time (newest first).
show-saves:
  ls -lt "{{save-dir}}" | grep -v settings.sav
