wine         := "/nix/store/dp1hc4npi0520afxz7ama2h048ssi2y3-proton-ge-bin-GE-Proton10-34-steamcompattool/files/bin/wine64"
proton       := "/nix/store/dp1hc4npi0520afxz7ama2h048ssi2y3-proton-ge-bin-GE-Proton10-34-steamcompattool/proton"
steam-client := env('HOME') + "/.local/share/Steam"

# NOTE: claude doesn't like escaped whitespace in paths
# game-base := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'"
game-base := env('HOME') + "/gam/steam/steamapps/common/sweaw"

game-dir  := env('HOME') + "/gam/steam/steamapps/common/sweaw/corruption"
compat-data := env('HOME') + "/gam/steam/steamapps/compatdata/32470"
log-file  := env('HOME') + "/gam/steam/steamapps/common/sweaw/corruption/eaw-mt.log"
save-dir  := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx/drive_c/users/steamuser/Saved Games/Petroglyph/Empire At War - Forces of Corruption/Save"

export WINEPREFIX := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx"

build-winmm:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

ghidra-script script:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis \
    -scriptPath tools/ghidra_scripts \
    -postScript {{script}}.java \
    -log logs/{{script}}.log

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
