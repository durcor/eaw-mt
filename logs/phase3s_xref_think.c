
// ================================================================
// CALLERS OF FUN_1403b08c0 (command queue processor)
// ================================================================

// Caller: RVA 0x5d990  Name: FUN_14005d990  Size: 7440 bytes
// Caller: RVA 0x8df00  Name: FUN_14008df00  Size: 2340 bytes


// ================================================================
// CALLER OF CMD_Q: RVA 0x5d990
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5d990  Name: FUN_14005d990  Size: 7440 bytes
// ------------------------------------------------------------

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
      DAT_140a10cb8 != &DAT_ffffffffffffffff)) && (DVar11 = GetLastError(), DVar11 == 0xb7)) {
    ReleaseMutex(DAT_140a10cb8);
    DAT_140a10cb8 = &DAT_ffffffffffffffff;
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
    if (DAT_140a10cb8 == &DAT_ffffffffffffffff) {
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
  local_518.hIcon = LoadIconA(param_1,&DAT_00000083);
  local_518.hbrBackground = _DAT_140803530;
  local_518.lpszMenuName = _UNK_140803538;
// ... [truncated at 400 lines, total 941]


// ================================================================
// CALLER OF CMD_Q: RVA 0x8df00
// ================================================================

// ------------------------------------------------------------
// RVA: 0x8df00  Name: FUN_14008df00  Size: 2340 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14008df00(int *param_1)

{
  float fVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong *plVar4;
  char cVar5;
  int iVar6;
  DWORD DVar7;
  ulonglong uVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  uint uVar14;
  longlong lVar15;
  longlong lVar16;
  int iVar17;
  undefined **local_res10;
  char *local_c8 [2];
  undefined8 local_b8;
  ulonglong local_b0;
  undefined **local_a8;
  char local_9f;
  longlong *local_80;
  undefined4 local_78;
  undefined4 local_74;
  char local_70;
  ulonglong local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  int local_48;
  undefined8 local_40;
  
  cVar5 = FUN_1401c1e90(&DAT_140a78510,DAT_140a110d4);
  plVar4 = DAT_140b15418;
  if (cVar5 == '\0') {
    return;
  }
  iVar6 = *param_1;
  if (iVar6 == 0) {
LAB_14008df5a:
    if (DAT_140b15418 != (longlong *)0x0) {
      (**(code **)(*DAT_140b15418 + 0x40))();
    }
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 == 1) {
    plVar11 = (longlong *)(param_1 + 2);
    if (0xf < *(ulonglong *)(param_1 + 8)) {
      plVar11 = *(longlong **)(param_1 + 2);
    }
    if (*(longlong *)(param_1 + 6) != 0x10) {
      return;
    }
    if (*plVar11 != 0x494745425f434449) {
      return;
    }
    if (plVar11[1] != 0x4e4f545455425f4e) {
      return;
    }
    if (DAT_140a28c20 == '\0') {
      if (DAT_140b15418 == (longlong *)0x0) goto LAB_14008e800;
    }
    else {
      FUN_1402d2790(&DAT_140b27dc0);
      FUN_14021cf60(&DAT_140a7d0a0,0);
      FUN_14021ce30(&DAT_140a7d0a0,0,0,0);
      if (plVar4 == (longlong *)0x0) goto LAB_14008e800;
      *(undefined1 *)((longlong)plVar4 + 0x9a) = 0;
    }
    (**(code **)(*plVar4 + 0x40))(plVar4);
LAB_14008e800:
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 != 0xc) {
    if (iVar6 != 0xe) {
      return;
    }
    if ((param_1[0xb] != 0x20) && (param_1[0xb] != 0xd)) {
      return;
    }
    goto LAB_14008df5a;
  }
  if (DAT_140a61d39 == '\0') {
    return;
  }
  DAT_140a61a68 = &LAB_14008e8c0;
  DAT_140a110d0 = 0;
  FUN_14021d0f0(&DAT_140a7d0a0,DAT_140b30328,DAT_140b3032c,DAT_140b30330,DAT_140b30334);
  if (DAT_140a61df8 != (longlong *)0x0) {
    iVar6 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar6 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar6 == 0) {
      FUN_14007fa80();
    }
  }
  plVar4 = DAT_140a61df8;
  uVar9 = DAT_140a61d58;
  iVar6 = DAT_140a61d54;
  uVar3 = DAT_140a110dc;
  uVar2 = DAT_140a110d8;
  puVar10 = &DAT_140a110e8;
  if (0xf < DAT_140a11100) {
    puVar10 = DAT_140a110e8;
  }
  cVar5 = DAT_140a61d3a;
  FUN_14022e310(&local_a8,"LoadThread");
  local_a8 = LoadingThreadClass::vftable;
  local_80 = (longlong *)0x0;
  local_68[0] = 0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  if (plVar4 != (longlong *)0x0) {
    local_80 = plVar4;
    (**(code **)*plVar4)(plVar4);
  }
  lVar15 = -1;
  local_78 = uVar3;
  lVar16 = -1;
  local_74 = uVar2;
  do {
    lVar16 = lVar16 + 1;
  } while (*(char *)((longlong)puVar10 + lVar16) != '\0');
  local_70 = cVar5;
  FUN_140022730(local_68,puVar10);
  local_48 = iVar6;
  local_40 = uVar9;
  local_c8[0] = (char *)FUN_140769c58(0x20);
  uVar9 = s_IDC_BEGIN_BUTTON_1408052c8._8_8_;
  local_b8 = 0x10;
  local_b0 = 0x1f;
  *(undefined8 *)local_c8[0] = s_IDC_BEGIN_BUTTON_1408052c8._0_8_;
  *(undefined8 *)(local_c8[0] + 8) = uVar9;
  local_c8[0][0x10] = '\0';
  FUN_1401c30d0(&DAT_140a78510,DAT_140a110d4,local_c8,0);
  if (0xf < local_b0) {
    if (0xfff < local_b0 + 1) {
      if ((char *)0x1f < local_c8[0] + (-8 - *(longlong *)(local_c8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_c8[0] + -8),local_b0 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DAT_140a61d50 = DAT_140a61dd4;
  DAT_140a61d4c = 0;
  DAT_140a61d39 = 1;
  _DAT_140a61d88 = 0;
  uRam0000000140a61d90 = 0;
  _DAT_140a61d80 = 0;
  _DAT_140a61d98 = 0;
  uRam0000000140a61da0 = 0;
  _DAT_140a61da8 = 0;
  _DAT_140a61d60 = 0;
  uRam0000000140a61d68 = 0;
  _DAT_140a61d70 = 0;
  uRam0000000140a61d78 = 0;
  FUN_14022e490(&local_a8);
  while (local_9f == '\0') {
    FUN_14008e9a0();
    FUN_1400ca620();
  }
  FUN_14022e560(&local_a8);
  DVar7 = timeGetTime();
  fVar1 = DAT_1407ffaf8;
  uVar13 = 0;
  iVar17 = DVar7 - _DAT_140a7bec0;
  uVar12 = uVar13;
  iVar6 = iVar17;
  if ((DAT_140a61d3a != '\0') || (((DAT_140a61d54 - 1U & 0xfffffffc) == 0 && (DAT_140a61d54 != 3))))
  {
    while ((DVar7 = timeGetTime(), (DVar7 - _DAT_140a7bec0) - iVar17 < 0xea61 ||
           (DVar7 = timeGetTime(), (DVar7 - _DAT_140a7bec0) - iVar6 < 0xafc9))) {
      FUN_14008e9a0();
      uVar8 = uVar13;
      while ((*(int *)((longlong)&DAT_140a61db0 + uVar8) == -1 ||
             (*(float *)((longlong)&DAT_140a61d88 + uVar8) == fVar1))) {
        uVar8 = uVar8 + 4;
        if (0x23 < (longlong)uVar8) {
          if ((DAT_140a61d3a == '\0') &&
             (((DAT_140a61d54 - 1U & 0xfffffffc) != 0 || (DAT_140a61d54 == 3)))) {
            DAT_140a61d50 = (DAT_140a110e0 - DAT_140a61dd4) + DAT_140a61dd4;
          }
          else {
            _DAT_140a61d88 = CONCAT44(DAT_140a61d8c,0x3f800000);
            if (((DAT_140a61d54 - 1U & 0xfffffffc) == 0) && (DAT_140a61d54 != 3)) {
              FUN_1400ca410();
            }
          }
          goto LAB_14008e46b;
        }
      }
      DVar7 = timeGetTime();
      uVar14 = DVar7 - _DAT_140a7bec0;
      if (5000 < uVar14 - (int)uVar12) {
        if ((DAT_140a61d3a == '\0') &&
           (((DAT_140a61d54 - 1U & 0xfffffffc) != 0 || (DAT_140a61d54 == 3)))) {
          DAT_140a61d50 = (DAT_140a110e0 - DAT_140a61dd4) + DAT_140a61dd4;
        }
        else {
          _DAT_140a61d88 = CONCAT44(DAT_140a61d8c,0x3f800000);
          if (((DAT_140a61d54 - 1U & 0xfffffffc) == 0) && (DAT_140a61d54 != 3)) {
            FUN_1400ca410();
          }
        }
        uVar12 = (ulonglong)uVar14;
      }
      FUN_1402089e0();
      uVar2 = DAT_140b0a320;
      FUN_1403b08c0(&DAT_140b2ed18);
      FUN_1402c2910(&DAT_140b15690,uVar2);
      FUN_1404908c0(&PTR_vftable_140a2aaa0);
      if (DAT_1409cf0a0 == 0) {
        lVar16 = FUN_140769c58(0x18);
        if (lVar16 == 0) {
          DAT_1409cf0a0 = 0;
        }
        else {
          DAT_1409cf0a0 = FUN_14006a000(lVar16);
        }
        local_res10 = &PTR_vftable_1409cf098;
        if (DAT_140a7bc10 == DAT_140a7bc18) {
          FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res10);
        }
        else {
          *DAT_140a7bc10 = &PTR_vftable_1409cf098;
          DAT_140a7bc10 = DAT_140a7bc10 + 1;
        }
      }
      FUN_14006a1e0(DAT_1409cf0a0);
      uVar9 = FUN_140025770();
      FUN_14006d1d0(uVar9);
      cVar5 = FUN_1400ca620();
      if (cVar5 != '\0') {
        DVar7 = timeGetTime();
        iVar6 = DVar7 - _DAT_140a7bec0;
      }
    }
  }
LAB_14008e46b:
  FUN_14008eff0();
  FUN_14013cb90();
  if (DAT_140b15418 != (longlong *)0x0) {
    if (DAT_140a11118 == 0) {
      puVar10 = &DAT_140a110e8;
      if (0xf < DAT_140a11100) {
        puVar10 = DAT_140a110e8;
      }
      do {
        lVar15 = lVar15 + 1;
      } while (*(char *)((longlong)puVar10 + lVar15) != '\0');
    }
    else {
      puVar10 = &DAT_140a11108;
      if (0xf < DAT_140a11120) {
        puVar10 = DAT_140a11108;
      }
      do {
        lVar15 = lVar15 + 1;
      } while (*(char *)((longlong)puVar10 + lVar15) != '\0');
    }
    FUN_140022730(DAT_140b15418 + 0xe,puVar10,lVar15);
  }
  FUN_1402a8730(DAT_140b15418[3]);
  FUN_14029dc30(DAT_140b15418[4],0);
  FUN_14028a4d0(&DAT_140b153e0);
  FUN_14021d130(&DAT_140a7d0a0,DAT_140b30328,DAT_140b3032c,DAT_140b30330,DAT_140b30334);
  if (DAT_140a61d3a == '\0') {
LAB_14008e558:
    cVar5 = FUN_14028af60(&DAT_140b153e0);
    if (cVar5 == '\0') {
      local_c8[0] = (char *)FUN_140769c58(0x20);
      uVar9 = s_IDC_BEGIN_BUTTON_1408052c8._8_8_;
      local_b8 = 0x10;
      local_b0 = 0x1f;
      *(undefined8 *)local_c8[0] = s_IDC_BEGIN_BUTTON_1408052c8._0_8_;
      *(undefined8 *)(local_c8[0] + 8) = uVar9;
      local_c8[0][0x10] = '\0';
      lVar15 = FUN_1401c29a0(&DAT_140a78510,DAT_140a110d4,local_c8);
      if (0xf < local_b0) {
        if (0xfff < local_b0 + 1) {
          if ((char *)0x1f < local_c8[0] + (-8 - *(longlong *)(local_c8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_c8[0] + -8),local_b0 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      if (lVar15 == 0) {
        if (DAT_140a61d3a == '\0') {
          if (DAT_140b15418 != (longlong *)0x0) {
            (**(code **)(*DAT_140b15418 + 0x40))();
          }
          DAT_140b0a354 = 0;
        }
        FUN_14008ddc0();
      }
      DAT_140a61a68 = (undefined1 *)0x0;
      local_a8 = LoadingThreadClass::vftable;
      if (local_80 != (longlong *)0x0) {
        (**(code **)(*local_80 + 8))();
        local_80 = (longlong *)0x0;
      }
      if (0xf < uStack_50) {
        uVar13 = uStack_50 + 1;
        uVar12 = local_68[0];
        if (0xfff < uVar13) {
          uVar12 = *(ulonglong *)(local_68[0] - 8);
          uVar13 = uStack_50 + 0x28;
          if (0x1f < (local_68[0] - uVar12) - 8) goto LAB_14008e815;
        }
        thunk_FUN_1407864b8(uVar12,uVar13);
      }
      goto LAB_14008e5ec;
    }
  }
  else {
    cVar5 = FUN_14028af60(&DAT_140b153e0);
    if (cVar5 == '\0') {
      FUN_1400eed80();
      goto LAB_14008e558;
    }
    FUN_1400c9e10();
  }
  FUN_14008ddc0();
  local_a8 = LoadingThreadClass::vftable;
  DAT_140a61a68 = (undefined1 *)0x0;
  if (local_80 != (longlong *)0x0) {
    (**(code **)(*local_80 + 8))();
    local_80 = (longlong *)0x0;
  }
  if (0xf < uStack_50) {
    if (0xfff < uStack_50 + 1) {
      uVar12 = *(ulonglong *)(local_68[0] - 8);
      uVar13 = uStack_50 + 0x28;
      if (0x1f < (local_68[0] - uVar12) - 8) {
LAB_14008e815:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar12,uVar13);
      }
    }
    thunk_FUN_1407864b8();
  }
LAB_14008e5ec:
  local_68[0] = local_68[0] & 0xffffffffffffff00;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  if (local_80 != (longlong *)0x0) {
    (**(code **)(*local_80 + 8))();
  }
  FUN_14022e3e0(&local_a8);
  return;
}




// ================================================================
// FUN_140285760 — per-player per-frame tick (from FUN_1404d9df0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x285760  Name: FUN_140285760  Size: 136 bytes
// ------------------------------------------------------------

void FUN_140285760(longlong param_1,longlong *param_2)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  if (*(longlong *)(param_1 + 0x360) != 0) {
    FUN_1404b0950();
  }
  cVar1 = (**(code **)(*param_2 + 0xf0))(param_2);
  if (cVar1 != '\0') {
    lVar3 = FUN_140769c58(0xb8);
    uVar4 = 0;
    if (lVar3 != 0) {
      uVar4 = FUN_1404967a0(lVar3,*(undefined4 *)(param_1 + 0x4c));
    }
    *(undefined8 *)(param_1 + 0x3a0) = uVar4;
  }
  iVar2 = (**(code **)(*param_2 + 0xe0))(param_2);
  *(undefined4 *)(param_1 + 0x3e0 + (longlong)iVar2 * 4) = 0x5a;
  *(undefined4 *)(param_1 + 0x3fc) = 0;
  *(undefined1 *)(param_1 + 0x400) = 1;
  return;
}




// ================================================================
// FUN_140240940 — event dispatcher (event 0x23 = think signal)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x240940  Name: FUN_140240940  Size: 516 bytes
// ------------------------------------------------------------

void FUN_140240940(longlong param_1,undefined4 param_2,undefined8 param_3)

{
  longlong *plVar1;
  int iVar2;
  __uint64 _Var3;
  longlong lVar4;
  ulonglong uVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  undefined1 uVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong *local_res8;
  undefined4 local_res10 [2];
  undefined8 local_res18;
  longlong local_58;
  undefined8 *puStack_50;
  undefined8 *local_48;
  
  lVar4 = *(longlong *)(param_1 + 0x10);
  if ((lVar4 != 0) &&
     (local_res10[0] = param_2, local_res18 = param_3, FUN_1401cacb0(lVar4,&local_res8,local_res10),
     plVar1 = local_res8, local_res8 != *(longlong **)(lVar4 + 8))) {
    local_48 = (undefined8 *)0x0;
    local_58 = 0;
    puStack_50 = (undefined8 *)0x0;
    _Var3 = (__uint64)*(int *)(local_res8[3] + 0x30);
    if (_Var3 != 0) {
      if (0x1fffffffffffffff < _Var3) {
                    /* WARNING: Subroutine does not return */
        FUN_140022630();
      }
      std::vector<unsigned___int64,class_std::allocator<unsigned___int64>_>::_Reallocate_exactly
                ((vector<unsigned___int64,class_std::allocator<unsigned___int64>_> *)&local_58,_Var3
                );
    }
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
    iVar6 = 0;
    uVar9 = *(undefined1 *)(param_1 + 8);
    *(undefined1 *)(param_1 + 8) = 0;
    lVar10 = plVar1[3];
    lVar8 = *(longlong *)(lVar10 + 0x10);
    lVar4 = lVar10 + 8;
    if (lVar8 != lVar4) {
      lVar7 = 0;
      do {
        plVar11 = *(longlong **)(lVar8 + 0x18);
        iVar2 = (int)((longlong)puStack_50 - local_58 >> 3);
        if (iVar6 < iVar2) {
          do {
            if (*(longlong **)(local_58 + lVar7 * 8) == plVar11) {
              lVar8 = *(longlong *)(lVar8 + 8);
              if (lVar8 == lVar4) break;
              plVar11 = *(longlong **)(lVar8 + 0x18);
            }
            iVar6 = iVar6 + 1;
            lVar7 = lVar7 + 1;
          } while (lVar7 < iVar2);
        }
        local_res8 = plVar11;
        if (lVar8 == lVar4) break;
        if (puStack_50 == local_48) {
          FUN_140045580(&local_58,puStack_50,&local_res8);
        }
        else {
          *puStack_50 = plVar11;
          puStack_50 = puStack_50 + 1;
        }
        (**(code **)(*plVar11 + 8))(plVar11,param_1,local_res10[0],local_res18);
        if (*(char *)(param_1 + 8) == '\0') {
          lVar8 = *(longlong *)(lVar8 + 8);
        }
        else {
          lVar10 = plVar1[3];
          iVar6 = 0;
          lVar7 = 0;
          uVar9 = 1;
          lVar8 = *(longlong *)(lVar10 + 0x10);
          *(undefined1 *)(param_1 + 8) = 0;
        }
        lVar4 = lVar10 + 8;
      } while (lVar8 != lVar4);
    }
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + -1;
    *(undefined1 *)(param_1 + 8) = uVar9;
    if (local_58 != 0) {
      uVar5 = (longlong)local_48 - local_58 & 0xfffffffffffffff8;
      if (0xfff < uVar5) {
        if (0x1f < (local_58 - *(longlong *)(local_58 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_58 + -8),uVar5 + 0x27);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}



