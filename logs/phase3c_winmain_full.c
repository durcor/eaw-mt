
// ================================================================
// WINMAIN FULL DECOMPILE (FUN_14005d990)
// ================================================================

// RVA: 0x5d990  Size: 7440 bytes

/* WARNING: Removing unreachable block (ram,0x00014005f137) */
/* WARNING: Removing unreachable block (ram,0x00014005f161) */
/* WARNING: Removing unreachable block (ram,0x00014005f166) */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14005d990(HINSTANCE param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  byte *pbVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  uint uVar5;
  uint uVar6;
  longlong *plVar7;
  char cVar8;
  char cVar9;
  ATOM AVar10;
  DWORD DVar11;
  BOOL BVar12;
  undefined4 uVar13;
  LONG X;
  int iVar14;
  int iVar15;
  undefined8 uVar16;
  longlong lVar17;
  undefined8 *puVar18;
  ulonglong uVar19;
  longlong lVar20;
  HWND hWnd;
  undefined8 uVar21;
  FILE *_File;
  char *pcVar22;
  HANDLE hProcess;
  LONG LVar23;
  undefined1 *puVar24;
  ulonglong uVar25;
  undefined8 *puVar26;
  LONG LVar27;
  HINSTANCE pHVar28;
  ulonglong uVar29;
  ulonglong uVar30;
  LONG Y;
  uint uVar31;
  float fVar32;
  ulonglong in_stack_fffffffffffff9f8;
  undefined8 in_stack_fffffffffffffa00;
  undefined4 uVar33;
  tagRECT local_5b8;
  undefined1 local_5a8 [16];
  ulonglong local_598;
  undefined4 local_590;
  char local_58c;
  undefined1 local_58b;
  undefined8 local_588;
  ulonglong local_580;
  DWORD local_578;
  undefined8 local_574;
  ulonglong local_568 [2];
  undefined8 local_558;
  ulonglong local_550;
  ulonglong local_548 [2];
  undefined8 local_538;
  ulonglong local_530;
  longlong local_528;
  undefined **local_520;
  WNDCLASSEXA local_518;
  undefined8 local_4c8;
  undefined4 local_4c0;
  tagMSG local_4b8;
  char local_488 [271];
  undefined1 uStack_379;
  byte local_378 [512];
  byte local_178 [320];
  
  uVar33 = (undefined4)((ulonglong)in_stack_fffffffffffffa00 >> 0x20);
  puVar18 = (undefined8 *)0x0;
  local_4b8.wParam = 0;
  DAT_140a619d8 = param_1;
  strncpy((char *)&local_518,"952F41D039DB4507A84B40488E76722E 0123456789ABCDEIPA",0x33);
  FUN_14021db70();
  cVar8 = FUN_14007f880();
  if (cVar8 == '\0') {
    return 0;
  }
  uVar16 = FUN_140067c80();
  cVar8 = FUN_14006a050(uVar16);
  if (cVar8 == '\0') {
    return 0;
  }
  DAT_140a13e20 = FUN_140222fa0();
  _DAT_140a2ab18 = FUN_140222fa0();
  cVar8 = FUN_140067d70(param_3);
  if ((DAT_140a61aaa == '\0') && (DAT_140a61aab == '\0')) {
    puVar26 = puVar18;
    if ((DAT_140a10d70 != 0) && (puVar26 = &DAT_140a10d60, 0xf < DAT_140a10d78)) {
      puVar26 = DAT_140a10d60;
    }
    cVar9 = FUN_14021dce0(puVar26);
    if (cVar9 == '\0') {
      return 0;
    }
  }
  DVar11 = GetModuleFileNameA((HMODULE)0x0,(LPSTR)local_378,0x104);
  if (DVar11 == 0) {
LAB_14005db21:
    GetLastError();
  }
  else {
    in_stack_fffffffffffff9f8 = 0;
    _splitpath((char *)local_378,(char *)local_178,local_488,(char *)0x0,(char *)0x0);
    puVar26 = puVar18;
    do {
      pbVar1 = local_178 + (longlong)puVar26;
      local_378[(longlong)puVar26] = *pbVar1;
      puVar26 = (undefined8 *)((longlong)puVar26 + 1);
    } while (*pbVar1 != 0);
    puVar24 = &uStack_379;
    do {
      pcVar22 = puVar24 + 1;
      puVar24 = puVar24 + 1;
    } while (*pcVar22 != '\0');
    lVar17 = 0;
    do {
      cVar9 = local_488[lVar17];
      puVar24[lVar17] = cVar9;
      lVar17 = lVar17 + 1;
    } while (cVar9 != '\0');
    BVar12 = SetCurrentDirectoryA((LPCSTR)local_378);
    if (BVar12 == 0) goto LAB_14005db21;
  }
  lVar17 = FUN_140769c58(0x38);
  if (lVar17 != 0) {
    puVar18 = (undefined8 *)FUN_140216a50(lVar17);
  }
  DAT_140a7d080 = puVar18;
  if (DAT_140a15330 != 0) {
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    FUN_140217740(puVar18,puVar26);
    lVar17 = DAT_140a15330;
    if (0x7fffffffffffffffU - DAT_140a15330 < 0xe) {
                    /* WARNING: Subroutine does not return */
      FUN_140022610();
    }
    local_5b8._0_8_ = (undefined8 *)0x0;
    puVar18 = (undefined8 *)&local_5b8;
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    uVar30 = 0xf;
    local_5a8 = ZEXT816(0);
    uVar29 = DAT_140a15330 + 0xe;
    if (0xf < uVar29) {
      uVar30 = uVar29 | 0xf;
      if (uVar30 < 0x8000000000000000) {
        if (uVar30 < 0x16) {
          uVar30 = 0x16;
        }
        if (uVar30 + 1 < 0x1000) {
          if (uVar30 == 0xffffffffffffffff) {
            puVar18 = (undefined8 *)0x0;
            local_5b8._0_8_ = puVar18;
          }
          else {
            puVar18 = (undefined8 *)FUN_140769c58();
            local_5b8._0_8_ = puVar18;
          }
          goto LAB_14005dc48;
        }
        uVar19 = uVar30 + 0x28;
        if (uVar19 <= uVar30 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
      }
      else {
        uVar30 = 0x7fffffffffffffff;
        uVar19 = 0x8000000000000027;
      }
      lVar20 = FUN_140769c58(uVar19);
      if (lVar20 == 0) goto LAB_14005f660;
      puVar18 = (undefined8 *)(lVar20 + 0x27U & 0xffffffffffffffe0);
      puVar18[0xffffffffffffffff] = lVar20;
      local_5b8._0_8_ = puVar18;
    }
LAB_14005dc48:
    local_5a8._8_8_ = uVar30;
    local_5a8._0_8_ = uVar29;
    FUN_14076bd80(puVar18,puVar26,lVar17);
    *(undefined8 *)((longlong)puVar18 + lVar17) = s_Data_Patch_meg_140802db8._0_8_;
    *(undefined4 *)((longlong)puVar18 + lVar17 + 8) = s_Data_Patch_meg_140802db8._8_4_;
    *(undefined2 *)((longlong)puVar18 + lVar17 + 0xc) = s_Data_Patch_meg_140802db8._12_2_;
    *(undefined1 *)((longlong)puVar18 + lVar17 + 0xeU) = 0;
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\patch2.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f654;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f654:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\64Patch.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f65a;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f65a:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    if (0xf < (ulonglong)local_5a8._8_8_) {
      uVar29 = local_5a8._8_8_ + 1;
      puVar18 = (undefined8 *)local_5b8._0_8_;
      if (0xfff < uVar29) {
        puVar18 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
        uVar29 = local_5a8._8_8_ + 0x28;
        if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar18))) {
LAB_14005f660:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(puVar18,uVar29);
    }
  }
  FUN_140217740(DAT_140a7d080,&DAT_140802dec);
  FUN_140216d20(DAT_140a7d080,"Data\\Patch.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\patch2.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\64Patch.meg",0,0);
  if (DAT_140a15330 != 0) {
    FUN_140214110(&DAT_140a15320);
  }
  DAT_140a7c090 = FUN_140200090();
  FUN_140067590();
  local_518.cbSize = 0;
  local_518.style = 0;
  local_518.cbClsExtra = _DAT_140802a40;
  local_518.cbWndExtra = _UNK_140802a44;
  local_518.hInstance = _UNK_140802a48;
  FUN_140022730(&local_518,"Arial Unicode MS",0x10);
  FUN_140270020(&local_518);
  local_580 = 0xf;
  local_588 = 0xb;
  local_598._0_1_ = s_EmpireAtWar_140802e08[0];
  local_598._1_1_ = s_EmpireAtWar_140802e08[1];
  local_598._2_1_ = s_EmpireAtWar_140802e08[2];
  local_598._3_1_ = s_EmpireAtWar_140802e08[3];
  local_598._4_1_ = s_EmpireAtWar_140802e08[4];
  local_598._5_1_ = s_EmpireAtWar_140802e08[5];
  local_598._6_1_ = s_EmpireAtWar_140802e08[6];
  local_598._7_1_ = s_EmpireAtWar_140802e08[7];
  local_590 = (uint)CONCAT12(s_EmpireAtWar_140802e08[10],s_EmpireAtWar_140802e08._8_2_);
  FUN_140270020(&local_598);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_520,"Software\\Petroglyph",1,1,in_stack_fffffffffffff9f8);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_528,"Software\\Petroglyph\\StarWars FOC",1,1,in_stack_fffffffffffff9f8);
  FUN_14021f740(&local_578,"Software\\Petroglyph\\StarWars FOC\\Profiles",1,1,
                in_stack_fffffffffffff9f8 & 0xffffffffffffff00);
  FUN_14021f810(&local_578);
  FUN_14021f810(&local_528);
  FUN_14021f810(&local_520);
  if (((DAT_140a61b0f == '\0') &&
      (DAT_140a10cb8 = CreateMutexA((LPSECURITY_ATTRIBUTES)0x0,1,"EAWIsRunning"),
      DAT_140a10cb8 != (HANDLE)0xffffffffffffffff)) && (DVar11 = GetLastError(), DVar11 == 0xb7)) {
    ReleaseMutex(DAT_140a10cb8);
    DAT_140a10cb8 = (HANDLE)0xffffffffffffffff;
    hWnd = FindWindowA("TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
    if (hWnd == (HWND)0x0) {
      return 0;
    }
    SetForegroundWindow(hWnd);
    return 0;
  }
  if ((0xb < DAT_140a10bb0 + 1U) || ((0x9c1U >> (DAT_140a10bb0 + 1U & 0x1f) & 1) == 0)) {
    DAT_140a7c090 = DAT_140a10bb0;
  }
  if (cVar8 == '\0') {
    if (DAT_140a10cb8 == (HANDLE)0xffffffffffffffff) {
      return 0;
    }
    ReleaseMutex(DAT_140a10cb8);
    return 0;
  }
  local_578 = 0x103;
  GetUserNameA(local_488,&local_578);
  local_578 = 0x103;
  GetComputerNameA((LPSTR)local_178,&local_578);
  FUN_140025760("     %s\n\n",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
  uVar16 = FUN_14021dae0();
  FUN_140025760("     Version : %s\n",uVar16);
  uVar16 = FUN_14021d7e0(0,0);
  uVar21 = FUN_14021d740();
  uVar13 = FUN_14021d780();
  FUN_140025760("     Build   : %d by %s at %s\n",uVar13,uVar21,uVar16);
  CoInitialize((LPVOID)0x0);
  lVar17 = -1;
  do {
    lVar17 = lVar17 + 1;
  } while (*(short *)(PTR_u_Star_Wars_R___Empire_at_War_TM___140a10ca8 + lVar17 * 2) != 0);
  FUN_1400228a0(&DAT_140a15560);
  SetCursor((HCURSOR)0x0);
  timeGetDevCaps((LPTIMECAPS)&DAT_140a61b10,8);
  timeBeginPeriod(DAT_140a61b10);
  FUN_140499340();
  FUN_1402029c0(&DAT_140a7c230);
  FUN_1400517b0(&PTR_vftable_140a10a20);
  FUN_14021eef0(&PTR_vftable_140a15360);
  if (DAT_140a61a9b == '\0') {
    DAT_140a61aac = DAT_140a7c441;
  }
  DAT_1409cf308 = DAT_140a61ae8;
  if ((DAT_140a61ae8 == 0) && (DAT_1409cf308 = DAT_140a7c350, DAT_140a61aac != '\0')) {
    DAT_1409cf308 = DAT_140a7c35c;
  }
  DAT_1409cf30c = DAT_140a61aec;
  if ((DAT_140a61aec == 0) && (DAT_1409cf30c = DAT_140a7c354, DAT_140a61aac != '\0')) {
    DAT_1409cf30c = DAT_140a7c360;
  }
  DAT_1409cf310 = DAT_140a7c358;
  if (DAT_140a61af0 != 0) {
    DAT_1409cf310 = DAT_140a61af0;
  }
  FUN_140307830(&DAT_140b27f60);
  if ((DAT_140a7c43c != '\0') || (DAT_140a61aac == '\0')) {
    param_4 = 3;
  }
  if (DAT_140a61a9e == '\0') {
    FUN_140036d60(&DAT_140a60df0,DAT_140a7c46d);
  }
  FUN_1404913a0(FUN_14004a220,FUN_140038150,&LAB_1400381e0,FUN_140072170);
  DAT_140a61a28 = FUN_14004e480();
  DAT_140a60e28 = DAT_140a7c43f;
  FUN_140307830(&DAT_140b27f60,DAT_1409cf308,DAT_1409cf30c);
  DAT_140a61af4 = 0;
  DAT_140a619d8 = param_1;
  EnumDisplayMonitors((HDC)0x0,(LPCRECT)0x0,FUN_140065ad0,0);
  local_518.lpfnWndProc = FUN_140060c60;
  local_518.cbSize = 0x50;
  local_518.style = 0x200b;
  local_518.cbClsExtra = 0;
  local_518.cbWndExtra = 0;
  local_518.hInstance = param_1;
  local_518.hIcon = LoadIconA(param_1,(LPCSTR)0x83);
  local_518.hbrBackground = _DAT_140803530;
  local_518.lpszMenuName = _UNK_140803538;
  local_518.hCursor = (HCURSOR)0x0;
  local_518.lpszClassName = "TESTWINCLASS";
  local_518.hIconSm = LoadIconA(local_518.hInstance,(LPCSTR)0x84);
  AVar10 = RegisterClassExA(&local_518);
  if (AVar10 == 0) goto LAB_14005f55c;
  local_5b8.bottom = DAT_1409cf30c;
  local_5b8.right = DAT_1409cf308;
  local_5b8._0_8_ = (undefined8 *)0x0;
  iVar15 = 0;
  iVar14 = 0;
  if (-1 < DAT_140a10bb4) {
    local_5b8.top = DAT_140a61afc;
    local_5b8.left = DAT_140a61af8;
    local_5b8.bottom = DAT_1409cf30c + DAT_140a61afc;
    local_5b8.right = DAT_1409cf308 + DAT_140a61af8;
    iVar15 = DAT_140a61afc;
    iVar14 = DAT_140a61af8;
  }
  AdjustWindowRect(&local_5b8,0xcf0000,0);
  Y = local_5b8.top;
  LVar27 = local_5b8.bottom;
  if (local_5b8.top < iVar15) {
    Y = local_5b8.top + (iVar15 - local_5b8.top);
    LVar27 = local_5b8.bottom + (iVar15 - local_5b8.top);
    local_5b8.top = Y;
    local_5b8.bottom = LVar27;
  }
  X = local_5b8.left;
  LVar23 = local_5b8.right;
  if (local_5b8.left < iVar14) {
    X = local_5b8.left + (iVar14 - local_5b8.left);
    LVar23 = local_5b8.right + (iVar14 - local_5b8.left);
    local_5b8.left = X;
    local_5b8.right = LVar23;
  }
  uVar30 = 0;
  uVar29 = CONCAT44(uVar33,Y);
  DAT_140a619d0 =
       CreateWindowExA(0,"TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0,0xcf0000,X,
                       Y,LVar23 - X,LVar27 - Y,(HWND)0x0,(HMENU)0x0,param_1,(LPVOID)0x0);
  if (DAT_140a619d0 == (HWND)0x0) goto LAB_14005f55c;
  ShowWindow(DAT_140a619d0,param_4);
  UpdateWindow(DAT_140a619d0);
  FUN_140214a00(DAT_140a619d0);
  cVar8 = FUN_14004b250();
  if (cVar8 != '\x01') goto LAB_14005f55c;
  puVar18 = &DAT_140a10d40;
  if (0xf < DAT_140a10d58) {
    puVar18 = DAT_140a10d40;
  }
  _File = (FILE *)FUN_14078013c(puVar18,&DAT_140803298);
  if (_File != (FILE *)0x0) {
    local_378[0] = 0;
    pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    while (pcVar22 != (char *)0x0) {
      if ((0x2d < local_378[0]) ||
         ((0x200000002401U >> ((longlong)(char)local_378[0] & 0x3fU) & 1) == 0)) {
        uVar16 = FUN_14001fb80();
        FUN_14007e040(uVar16,local_378);
      }
      pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    }
    fclose(_File);
  }
  if (DAT_140a61a50 != (undefined8 *)0x0) {
    (**(code **)*DAT_140a61a50)(DAT_140a61a50,1);
    DAT_140a61a50 = (undefined8 *)0x0;
  }
  if (DAT_140a61a99 == '\0') {
    if (DAT_140a61a9a == '\0') {
      if (DAT_140a10d10 == 0) {
        DAT_140a61aa8 = DAT_140a61ab7;
        _guard_check_icall(&DAT_140b27f60);
        if ((DAT_140a61ace == '\0') && (DAT_140a619ef == '\0')) {
          if (DAT_140a61a9f == '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,0);
            uVar29 = uVar29 & 0xffffffffffffff00;
            FUN_14044d920(&DAT_140b306dd,"Logo1",0,FUN_140065780,1,uVar29);
          }
          else {
            FUN_1400c58e0();
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
        }
      }
      else {
        FUN_140053c10(&PTR_vftable_140a10b18);
        cVar8 = FUN_140055250(&PTR_vftable_140a10b18,&DAT_140a10d00);
        if (cVar8 == '\0') {
          FUN_140033850(&DAT_140a60df0);
          puVar18 = &DAT_140a10d00;
          if (0xf < DAT_140a10d18) {
            puVar18 = DAT_140a10d00;
          }
          cVar8 = FUN_140052d10(&PTR_vftable_140a10b18,0,puVar18,0);
          pcVar22 = (char *)FUN_14004fba0();
          if (*pcVar22 != '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
          if (cVar8 != '\0') goto LAB_14005e71d;
        }
        DAT_140a619ef = '\x01';
      }
    }
    else {
      cVar8 = FUN_140069040();
      if (cVar8 == '\0') {
        DAT_140a619ef = '\x01';
      }
    }
  }
  else {
    cVar8 = FUN_140068b20();
    if (cVar8 == '\0') {
      DAT_140a619ef = '\x01';
    }
  }
LAB_14005e71d:
  fVar4 = DAT_14080350c;
  fVar3 = DAT_1408007f8;
  if (DAT_140a61b0e != '\0') {
    DAT_140a14d01 = 1;
  }
  if (DAT_140a619ef == '\0') {
    iVar15 = 0;
    local_528 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    do {
      lVar17 = local_528;
      if ((((DAT_140b2f1d8 == '\x01') && (DAT_140b2f1d8 = '\0', DAT_140a61a98 == '\0')) &&
          (DAT_140a61a98 = '\x01', DAT_140a61ae0 == 0)) && (DAT_140a61acc == '\0')) {
        FUN_14025fb10(2);
        DAT_140a61acc = '\x01';
      }
      local_5b8._0_8_ = (undefined8 *)0x0;
      local_5b8._8_8_ = local_5b8._8_8_ & 0xffffffff00000000;
      local_4c8 = 0;
      local_4c0 = 0x3f800000;
      local_578 = 0;
      local_574 = 0x3f800000;
      if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x158))(DAT_140b15418,&local_5b8,&local_4c8,&local_578);
      }
      iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      while (iVar14 != 0) {
        if (local_4b8.message == 0x12) {
          DAT_140a619ef = '\x01';
          break;
        }
        iVar14 = TranslateAcceleratorA(local_4b8.hwnd,(HACCEL)0x0,&local_4b8);
        if (iVar14 == 0) {
          TranslateMessage(&local_4b8);
          DispatchMessageA(&local_4b8);
        }
        iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      }
      cVar8 = thunk_FUN_1401768f0();
      if (((cVar8 != '\0') && (DAT_1409cf314 != '\0')) &&
         ((DAT_140a61b08 != 0 &&
          (DVar11 = timeGetTime(), 1000 < (DVar11 - DAT_140a61b08) - _DAT_140a7bec0)))) {
        FUN_140066980();
        uVar16 = FUN_14004fba0();
        FUN_140262af0(uVar16);
        DAT_140a61b08 = 0;
      }
      thunk_FUN_14024bb80();
      FUN_1402505c0();
      uVar5 = DAT_140b0a320;
      FUN_1402089e0();
      FUN_1403b08c0(&DAT_140b2ed18);
      FUN_1402c2910(&DAT_140b15690,uVar5);
      if (((DAT_140b15418 != (longlong *)0x0) &&
          (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0')) && (DAT_140b1574e == '\0')) {
        FUN_1402c0c70(&DAT_140b15690,uVar5);
      }
      FUN_1404908c0(&PTR_vftable_140a2aaa0);
      FUN_1400caa60();
      if (((byte)uVar5 & 0x3f) == 0x3f) {
        DVar11 = timeGetTime();
        FUN_14021e7b0(DVar11 - _DAT_140a7bec0);
        FUN_1400503d0(&DAT_140a109b0);
      }
      FUN_140051d10(&DAT_140a10a98);
      if ((*(int *)(lVar17 + 0xd4) < DAT_140a61b1c) &&
         (_Init_thread_header(&DAT_140a61b1c), DAT_140a61b1c == -1)) {
        DVar11 = timeGetTime();
        _DAT_140a61b18 = DVar11 - _DAT_140a7bec0;
        _Init_thread_footer(&DAT_140a61b1c);
      }
      DVar11 = timeGetTime();
      iVar14 = DVar11 - _DAT_140a7bec0;
      uVar31 = iVar14 - _DAT_140a61b18;
      _DAT_140a61b18 = iVar14;
      FUN_140215b90((float)uVar31 * fVar4);
      FUN_140339bc0(&DAT_140b2ba20,iVar14);
      FUN_140022c50(&DAT_1409cf080,iVar14);
      uVar16 = FUN_140067c80();
      FUN_14006a1e0(uVar16);
      uVar16 = FUN_140025770();
      FUN_14006d1d0(uVar16);
      if (DAT_1409cf314 == '\x01') {
        uVar29 = uVar29 & 0xffffffffffffff00;
        FUN_14021caf0(&DAT_140a7d0a0,iVar14,&local_5b8,&local_4c8,&local_578,uVar29);
        FUN_1402d72c0(&DAT_140b27e60,iVar14,uVar5);
        FUN_1402d2ab0(&DAT_140b27dc0,iVar14,uVar5);
        FUN_140321dc0(&DAT_140b2b300,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b3a8,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b2f0,iVar14,uVar5);
        FUN_140325190(&DAT_140b2b3b0,iVar14);
      }
      _guard_check_icall(&DAT_140a2a120,iVar14);
      FUN_1401c8420(&DAT_140a78510);
      cVar8 = thunk_FUN_1401768f0();
      if (cVar8 != '\0') {
        cVar8 = FUN_14044d840(&DAT_140b306dd);
        if (((cVar8 != '\0') && (cVar8 = FUN_14044d780(&DAT_140b306dd), cVar8 != '\0')) &&
           (FUN_14044f130(&DAT_140b306dd), DAT_140a111e0 != 0)) {
          uVar29 = uVar29 & 0xffffffffffffff00;
          puVar18 = &DAT_140a111d0;
          if (0xf < DAT_140a111e8) {
            puVar18 = DAT_140a111d0;
          }
          FUN_14044d920(&DAT_140b306dd,puVar18,0,0,1,uVar29);
        }
        FUN_14030c3b0(&DAT_140b27f60,iVar14);
        if (DAT_1409cf314 != '\0') {
          FUN_14002ee40(&DAT_140a60df0,iVar14);
        }
        if (DAT_1409cf314 != '\0') {
          FUN_140305890(&DAT_140b27f60);
        }
        FUN_1400493f0(&DAT_1409cf320,iVar14);
        FUN_1400493f0(&DAT_1409cf390);
      }
      if ((DAT_140b0a355 != '\0' || DAT_140b1574c != '\0') && (DAT_140a61b0d == '\0')) {
        if (DAT_140a61e60 != '\0') {
          FUN_14009aa80();
        }
        if ((DAT_140b1574e == '\0') || (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,&PTR_1407ff7e8,0);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,&local_598,local_548,local_568,&local_518,0,0,0,0,uVar30);
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f6a2:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            goto LAB_14005efe5;
          }
        }
        else {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,L"TEXT_BUTTON_CONTINUE_AFTER_SYNC",0x1f);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,local_568,local_548,&local_598,&local_518,0,0,0,0,uVar30);
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f648:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
LAB_14005efe5:
            thunk_FUN_1407864b8();
          }
        }
        DAT_140a61b0d = '\x01';
        FUN_14027c9c0(&DAT_140b0a320);
        if (DAT_140a61ab2 != '\0') {
          local_590._0_1_ = s_oosevents_log_140802f20[8];
          local_590._1_1_ = s_oosevents_log_140802f20[9];
          local_590._2_1_ = s_oosevents_log_140802f20[10];
          local_590._3_1_ = s_oosevents_log_140802f20[0xb];
          local_58c = s_oosevents_log_140802f20[0xc];
          local_580 = 0xf;
          local_588 = 0xd;
          local_598._0_1_ = s_oosevents_log_140802f20[0];
          local_598._1_1_ = s_oosevents_log_140802f20[1];
          local_598._2_1_ = s_oosevents_log_140802f20[2];
          local_598._3_1_ = s_oosevents_log_140802f20[3];
          local_598._4_1_ = s_oosevents_log_140802f20[4];
          local_598._5_1_ = s_oosevents_log_140802f20[5];
          local_598._6_1_ = s_oosevents_log_140802f20[6];
          local_598._7_1_ = s_oosevents_log_140802f20[7];
          local_58b = 0;
          FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320);
          DAT_140a61ab2 = '\0';
          if (0xf < local_580) {
            if ((0xfff < local_580 + 1) && (0x1f < (local_598 - *(longlong *)(local_598 - 8)) - 8))
            {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            thunk_FUN_1407864b8();
          }
        }
        FUN_140047860(&DAT_140b153e0);
      }
      uVar16 = FUN_14004fba0();
      FUN_140262b80(uVar16);
      FUN_1403b1b50(&DAT_140a286f0);
      if ((DAT_1409cf314 == '\0') &&
         (((cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0' || (DAT_140b155c4 != '\0')) &&
          (cVar8 = FUN_14024cef0(), cVar8 == '\0')))) {
        WaitMessage();
      }
      else {
        cVar8 = FUN_14027c5f0(&DAT_140b0a320);
        if (((((cVar8 == '\0') || (DAT_140b155c4 == '\0')) || (DAT_140a785a8 == 0)) ||
            (cVar9 = FUN_1401c0c70(&DAT_140a78510), cVar9 == '\0')) && (cVar8 != '\0')) {
          if (DAT_140a61e70 != '\0') {
            FUN_14009b9f0();
          }
          FUN_14027c360(&DAT_140b0a320,iVar14);
          uVar6 = DAT_140b0a320;
          FUN_1402c0c70(&DAT_140b15690,DAT_140b0a320);
          if (uVar6 == ((int)uVar6 / 100) * 100) {
            if (DAT_140a10970 == 0) {
              lVar20 = FUN_140769c58(0xc0);
              lVar17 = 0;
              if (lVar20 != 0) {
                lVar17 = FUN_140475660(lVar20);
              }
              local_520 = &PTR_vftable_140a10968;
              DAT_140a10970 = lVar17;
              if (DAT_140a7bc10 == DAT_140a7bc18) {
                FUN_14001ded0(&DAT_140a7bc08);
              }
              else {
                *DAT_140a7bc10 = &PTR_vftable_140a10968;
                DAT_140a7bc10 = DAT_140a7bc10 + 1;
              }
            }
            FUN_1404767c0(DAT_140a10970);
          }
          FUN_14020f000((float)DAT_140b0a320 / (float)DAT_140b0a340);
          FUN_14028d400(&DAT_140b153e0);
          if (DAT_140a61d38 == '\0') {
            FUN_1402718e0((float)DAT_140b0a320 / (float)DAT_140b0a340);
          }
          FUN_140297e30(&DAT_140a16fd0);
          FUN_14001dc60();
          plVar7 = DAT_140b15418;
          if (DAT_140b15418 != (longlong *)0x0) {
            FUN_1403271d0(DAT_140b15418 + 0x14);
            FUN_140490480((longlong)plVar7 + 0x9c);
            FUN_1403268c0(plVar7 + 0x14);
          }
          FUN_14044a3f0(&DAT_140b30350);
          cVar8 = FUN_14028af60(&DAT_140b153e0);
          if ((((cVar8 != '\0') || ((DAT_140a61e60 == '\0' && (DAT_140b30690 == 0)))) &&
              (DAT_140b15418 != (longlong *)0x0)) &&
             (iVar15 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar15 == 0)) {
            lVar17 = DAT_140b15418[5];
            cVar8 = FUN_140342dc0(lVar17);
            if ((cVar8 == '\x01') && (cVar8 = FUN_140342d80(lVar17), cVar8 == '\x01')) {
              FUN_140092bb0(&DAT_140a10d88);
            }
          }
          FUN_14028a4c0(&DAT_140b153e0);
          FUN_14027cb70(&DAT_140b0a320);
          iVar15 = iVar14;
          if (0 < (int)DAT_140a2ab00) {
            FUN_14005f6b0(1);
          }
        }
        else {
          FUN_14009ba60(iVar14);
          FUN_1404908c0(&DAT_140b15560);
          Sleep(0);
          if (((500 < (uint)(iVar14 - iVar15)) &&
              (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) && (DAT_140b155c4 == '\0')) {
            FUN_14027d6a0(&DAT_140b0a320);
            iVar15 = iVar14;
          }
        }
      }
      if ((DAT_140a61aac != '\0') || (DAT_1409cf314 != '\0')) {
        DVar11 = timeGetTime();
        uVar19 = (ulonglong)DAT_140b0a320;
        iVar14 = DVar11 - _DAT_140a7bec0;
        if (DAT_140a61d38 != '\0') {
          FUN_1402718e0((float)(uint)(iVar14 - _DAT_140a61d3c) / fVar3);
        }
        fVar32 = (((float)uVar19 - (float)(int)uVar5) * (float)DAT_140b0a340) / fVar3;
        fVar2 = 0.0;
        if (0.0 <= fVar32) {
          fVar2 = fVar32;
        }
        cVar8 = FUN_14027d800(&DAT_140b0a320);
        if (cVar8 != '\0') {
          FUN_14028c580(&DAT_140b153e0);
          FUN_140271840();
          FUN_140301750(&DAT_140b27f60);
          FUN_14002ffb0(&DAT_140a60df0);
          FUN_14025ca30(DAT_140a619f0,(float)uVar31 / fVar3);
          FUN_140060330(fVar2 != 0.0);
        }
        FUN_14027c2f0(&DAT_140b0a320,iVar14);
        timeGetTime();
        FUN_14027c330(&DAT_140b0a320);
      }
    } while (DAT_140a619ef == '\0');
  }
LAB_14005f55c:
  timeEndPeriod(DAT_140a61b10);
  if (DAT_140a10cb8 != (HANDLE)0xffffffffffffffff) {
    ReleaseMutex(DAT_140a10cb8);
  }
  if ((((DAT_140a61acd != '\0') && (iVar15 = GetSystemMetrics(0x57), iVar15 != 0)) &&
      (DVar11 = ExpandEnvironmentStringsA("%SystemRoot%\\ehome\\ehshell.exe",local_488,0x104),
      DVar11 != 0)) && (DVar11 = GetFileAttributesA(local_488), DVar11 != 0xffffffff)) {
    ShellExecuteA((HWND)0x0,"open",local_488,(LPCSTR)0x0,(LPCSTR)0x0,1);
  }
  hProcess = GetCurrentProcess();
  TerminateProcess(hProcess,0);
  return 0;
}



// ================================================================
// STRING SEARCH: GameObjectManager / Think / GOM_Update
// ================================================================


// --- Searching for: "GameObjectManager" ---
//   String at RVA 0x85ba68
//     xref from FUN_0x2af7c0 @ site 0x2af80f
//   String at RVA 0x85bc58
//     xref from FUN_0x2b0520 @ site 0x2b0592
//   String at RVA 0xa4beec
//   String at RVA 0xa4c11f

// --- Searching for: "GameObjectClass" ---
//   String at RVA 0x866330
//     xref from FUN_0x3a9e30 @ site 0x3aae3b
//   String at RVA 0x8a951d
//   String at RVA 0x8c03c5
//   String at RVA 0xa46b9d
//   String at RVA 0xa4bdd1
//   String at RVA 0xa4bec2
//   String at RVA 0xa4bfec
//   String at RVA 0xa4c070
//   String at RVA 0xa4cfd3
//   String at RVA 0xa4d6f4
//   String at RVA 0xa4d739

// --- Searching for: "GOM" ---
//   String at RVA 0x85abdf
//   String at RVA 0x85c028
//     xref from FUN_0x29efe0 @ site 0x29f1f4

// --- Searching for: "Think" ---
//   (not found)

// --- Searching for: "Update_All" ---
//   String at RVA 0x8835f0

// --- Searching for: "Sim_Object" ---
//   (not found)

// --- Searching for: "SimObject" ---
//   (not found)

// --- Searching for: "game_object" ---
//   (not found)

// --- Searching for: "unit_update" ---
//   (not found)

// --- Searching for: "PhysicsClass" ---
//   (not found)

// --- Searching for: "AIClass" ---
//   String at RVA 0xa4bbfc
//   String at RVA 0xa52147
//   String at RVA 0xa5217c

// --- Searching for: "TacticalAI" ---
//   String at RVA 0xa586fc
//   String at RVA 0xa5872c

// --- Searching for: "BehaviorClass" ---
//   String at RVA 0x85c2ef
//   String at RVA 0x867886
//   String at RVA 0x868589
//   String at RVA 0x868d02
//   String at RVA 0x8695a4
//   String at RVA 0x869e83
//   String at RVA 0x86a36c
//   String at RVA 0x86a7d8
//   String at RVA 0x86aa75
//   String at RVA 0x86ae8b
//   String at RVA 0x86afc6
//   String at RVA 0x86b984
//   String at RVA 0x875805
//   String at RVA 0x876d8e
//   String at RVA 0x876ec6
//   String at RVA 0x877124
//   String at RVA 0x877159
//   String at RVA 0x8779ed
//   String at RVA 0x877b2f
//   String at RVA 0x877c6d
