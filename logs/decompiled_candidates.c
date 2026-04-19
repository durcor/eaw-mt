// ============================================================
// RVA: 0x14fa0  Size: 164 bytes
// ============================================================

void FUN_140014fa0(void)

{
  undefined8 *puVar1;
  int iVar2;
  undefined *puVar3;
  DWORD DVar4;
  int iVar5;
  undefined4 uStack_c;
  
  DVar4 = GetCurrentThreadId();
  puVar3 = PTR_s_Main_Thread_140a155c0;
  iVar5 = DAT_140a155ac * 2 >> 1;
  DAT_140a155d0 = DVar4;
  if (iVar5 <= DAT_140a155a8) {
    iVar2 = iVar5 / 10;
    if (iVar5 / 10 < 10) {
      iVar2 = 10;
    }
    FUN_14022eb30(&PTR_vftable_140a15598,iVar2 + iVar5,0,1);
  }
  puVar1 = (undefined8 *)(DAT_140a155a0 + (longlong)DAT_140a155a8 * 0x10);
  *puVar1 = puVar3;
  puVar1[1] = CONCAT44(uStack_c,DVar4);
  DAT_140a155a8 = DAT_140a155a8 + 1;
  PTR_vftable_140a155b0 = (undefined *)MainThreadClass::vftable;
  atexit((_func_5014 *)&LAB_1407edbc0);
  return;
}



// ============================================================
// RVA: 0x8df00  Size: 2340 bytes
// ============================================================

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



// ============================================================
// RVA: 0x1a7370  Size: 891 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_1401a7370(int param_1)

{
  DWORD DVar1;
  LARGE_INTEGER LVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  LARGE_INTEGER local_res10 [3];
  
  if (DAT_140a705e0 == 0) {
    DAT_140a705e0 = 1;
    DAT_140a12bb0 = QueryPerformanceFrequency(local_res10);
    if (DAT_140a12bb0 == 0) goto LAB_1401a7577;
    DAT_140a705e8.s = local_res10[0].s;
  }
  if (DAT_140a12bb0 != 0) {
    if (((DAT_140a705f0.QuadPart == 0) || (param_1 == 1)) || (LVar2 = DAT_140a705f0, param_1 == 4))
    {
      QueryPerformanceCounter(local_res10);
      LVar2 = local_res10[0];
    }
    if (param_1 == 6) {
      lVar3 = LVar2.QuadPart - _DAT_140a705f8;
      _DAT_140a705f8 = LVar2.s;
      return (float)lVar3 / (float)DAT_140a705e8.QuadPart;
    }
    if (param_1 == 5) {
      return (float)(LVar2.QuadPart - _DAT_140a70600) / (float)DAT_140a705e8.QuadPart;
    }
    if (param_1 == 0) {
      _DAT_140a70600 = LVar2.s;
      DAT_140a705f0 = (LARGE_INTEGER)0x0;
      DAT_140a12bb4 = 0;
      _DAT_140a705f8 = LVar2.s;
      return 0.0;
    }
    if (param_1 == 1) {
      if (DAT_140a12bb4 != 0) {
        _DAT_140a70600 = _DAT_140a70600 + (LVar2.QuadPart - (longlong)DAT_140a705f0);
      }
      _DAT_140a705f8 = LVar2.s;
      DAT_140a705f0 = (LARGE_INTEGER)0x0;
      DAT_140a12bb4 = 0;
      return 0.0;
    }
    if (param_1 == 2) {
      if (DAT_140a12bb4 != 0) {
        return 0.0;
      }
      DAT_140a705f0.s = LVar2.s;
      _DAT_140a705f8 = LVar2.s;
      DAT_140a12bb4 = 1;
      return 0.0;
    }
    if (param_1 == 3) {
      DAT_140a705f0.QuadPart = DAT_140a705f0.QuadPart + DAT_140a705e8.QuadPart / 10;
      return 0.0;
    }
    if (param_1 != 4) {
      return DAT_14080080c;
    }
    return (float)LVar2.QuadPart / (float)DAT_140a705e8.QuadPart;
  }
LAB_1401a7577:
  if (((DAT_140a70610 == 0.0) || (param_1 == 1)) || (fVar5 = DAT_140a70610, param_1 == 4)) {
    DVar1 = timeGetTime();
    fVar5 = (float)((double)DVar1 * _DAT_14081c078);
  }
  if (param_1 == 6) {
    fVar4 = fVar5 - _DAT_140a70608;
    _DAT_140a70608 = fVar5;
    return fVar4;
  }
  if (param_1 == 5) {
    fVar5 = fVar5 - DAT_140a7060c;
  }
  else {
    if (param_1 == 0) {
      DAT_140a7060c = fVar5;
      DAT_140a12bb4 = 0;
      _DAT_140a70608 = fVar5;
      DAT_140a70610 = (float)param_1;
      return 0.0;
    }
    if (param_1 == 1) {
      if (DAT_140a12bb4 != 0) {
        DAT_140a7060c = DAT_140a7060c + (fVar5 - DAT_140a70610);
      }
      _DAT_140a70608 = fVar5;
      DAT_140a12bb4 = 0;
      DAT_140a70610 = 0.0;
      return 0.0;
    }
    if (param_1 == 2) {
      if (DAT_140a12bb4 == 0) {
        DAT_140a12bb4 = 1;
        _DAT_140a70608 = fVar5;
        DAT_140a70610 = fVar5;
      }
      return 0.0;
    }
    if (param_1 == 3) {
      DAT_140a70610 = DAT_140a70610 + DAT_1408007b4;
      return 0.0;
    }
    if (param_1 != 4) {
      return DAT_14080080c;
    }
  }
  return fVar5;
}



// ============================================================
// RVA: 0x222e20  Size: 99 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140222e20(void)

{
  LARGE_INTEGER local_res8;
  LARGE_INTEGER local_res10 [3];
  
  QueryPerformanceFrequency(&local_res8);
  _DAT_140a7e2f0 = local_res8.s;
  DAT_140a7e2f8 = local_res8.QuadPart / 1000;
  DAT_140a7e2e8 = 1;
  QueryPerformanceCounter(local_res10);
  _DAT_140a7e300 = local_res10[0];
  return;
}



// ============================================================
// RVA: 0x222e90  Size: 55 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_140222e90(void)

{
  LARGE_INTEGER local_res8 [4];
  
  if (DAT_140a7e2e8 == 0) {
    FUN_140222e20();
  }
  QueryPerformanceCounter(local_res8);
  return (local_res8[0].QuadPart - _DAT_140a7e300) / DAT_140a7e2f8;
}



// ============================================================
// RVA: 0x222ed0  Size: 113 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140222ed0(void)

{
  LARGE_INTEGER local_res8;
  LARGE_INTEGER local_res10 [3];
  
  if (DAT_140a7e2e8 == 0) {
    QueryPerformanceFrequency(&local_res8);
    _DAT_140a7e2f0 = local_res8.s;
    DAT_140a7e2f8 = local_res8.QuadPart / 1000;
    DAT_140a7e2e8 = 1;
    FUN_140222ed0();
  }
  QueryPerformanceCounter(local_res10);
  _DAT_140a7e300 = local_res10[0];
  return;
}



// ============================================================
// RVA: 0x76ab1c  Size: 172 bytes
// ============================================================

/* Library Function - Single Match
    __security_init_cookie
   
   Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release */

void __cdecl __security_init_cookie(void)

{
  DWORD DVar1;
  _FILETIME local_res8;
  _FILETIME local_res10;
  LARGE_INTEGER local_res18;
  
  if (DAT_140a44b98 == 0x2b992ddfa232) {
    local_res10.dwLowDateTime = 0;
    local_res10.dwHighDateTime = 0;
    GetSystemTimeAsFileTime(&local_res10);
    local_res8 = local_res10;
    DVar1 = GetCurrentThreadId();
    local_res8 = (_FILETIME)((ulonglong)local_res8 ^ (ulonglong)DVar1);
    DVar1 = GetCurrentProcessId();
    local_res8 = (_FILETIME)((ulonglong)local_res8 ^ (ulonglong)DVar1);
    QueryPerformanceCounter(&local_res18);
    DAT_140a44b98 =
         ((ulonglong)local_res18.s.LowPart << 0x20 ^
          CONCAT44(local_res18.s.HighPart,local_res18.s.LowPart) ^ (ulonglong)local_res8 ^
         (ulonglong)&local_res8) & 0xffffffffffff;
    if (DAT_140a44b98 == 0x2b992ddfa232) {
      DAT_140a44b98 = 0x2b992ddfa233;
    }
  }
  DAT_140a44b90 = ~DAT_140a44b98;
  return;
}



// ============================================================
// RVA: 0x224b90  Size: 2216 bytes
// ============================================================

void FUN_140224b90(longlong param_1,longlong *param_2)

{
  char *pcVar1;
  longlong lVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong *plVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  undefined8 uVar9;
  longlong lVar10;
  undefined2 *puVar11;
  longlong lVar12;
  undefined *puVar13;
  longlong lVar14;
  longlong *local_res8;
  wchar_t *local_res10;
  longlong *local_res18;
  longlong *local_res20;
  undefined8 *local_88;
  undefined1 local_80 [8];
  short local_78;
  undefined6 uStack_76;
  undefined1 local_68 [8];
  undefined8 *local_60;
  
  lVar14 = param_1 + 0x139;
  puVar11 = (undefined2 *)(param_1 + 0x138);
  do {
    pcVar1 = (char *)((longlong)puVar11 + 1);
    puVar11 = (undefined2 *)((longlong)puVar11 + 1);
  } while (*pcVar1 != '\0');
  *puVar11 = DAT_14085387c;
  local_res20 = (longlong *)0x0;
  iVar7 = (**(code **)(*param_2 + 200))(param_2,&local_res20);
  lVar2 = -1;
  lVar10 = lVar2;
  if ((iVar7 == 0) && (local_res20 != (longlong *)0x0)) {
    local_88 = (undefined8 *)0x0;
    iVar7 = (**(code **)(*local_res20 + 0x40))(local_res20,&local_88);
    if (iVar7 == 0) {
      local_res18 = (longlong *)0x0;
      iVar7 = (**(code **)*local_88)(local_88,&DAT_1408fc8e8,&local_res18);
      if (iVar7 == 0) {
        Ordinal_8(local_68);
        (**(code **)(*local_res18 + 0x28))();
        iVar7 = (**(code **)(*local_res18 + 0x18))(local_res18,1,local_68,0);
        while (iVar7 == 0) {
          local_res8 = (longlong *)0x0;
          if (((local_60 != (undefined8 *)0x0) &&
              (iVar7 = (**(code **)*local_60)(local_60,&DAT_1408fc908,&local_res8), iVar7 == 0)) &&
             (local_res8 != (longlong *)0x0)) {
            local_res10 = (wchar_t *)0x0;
            iVar7 = (**(code **)(*local_res8 + 0x48))(local_res8,&local_res10);
            if ((iVar7 == 0) && (local_res10 != (wchar_t *)0x0)) {
              FUN_140025760("%sFound UPnP service \'%ls\'\n",lVar14);
              iVar7 = _wcsicmp(local_res10,
                               L"urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1");
              if (iVar7 == 0) {
                uVar9 = Ordinal_2(L"WANAccessType");
                plVar5 = local_res8;
                Ordinal_8(local_80);
                iVar7 = 10;
LAB_140224d30:
                iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80);
                if (iVar8 == 0) {
                  lVar3 = CONCAT62(uStack_76,local_78);
                  FUN_140025760("%sWAN Access Type: \'%ls\'\n",lVar14,lVar3);
                  lVar12 = lVar2;
                  do {
                    lVar12 = lVar12 + 1;
                  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
                  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 +
                                *(longlong *)(param_1 + 0x118) + 0x188,lVar3);
                }
                else if ((iVar8 == -0x7ffbfdee) && (iVar7 != 0)) goto code_r0x000140224d50;
                goto LAB_140224db0;
              }
LAB_140224f8f:
              iVar7 = _wcsicmp(local_res10,L"urn:schemas-upnp-org:service:WANIPConnection:1");
              if (iVar7 == 0) {
                uVar9 = Ordinal_2(L"ConnectionType");
                plVar5 = local_res8;
                Ordinal_8(local_80);
                iVar7 = 10;
LAB_140224fd0:
                iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80);
                if (iVar8 == 0) {
                  lVar3 = CONCAT62(uStack_76,local_78);
                  FUN_140025760("%sConnection Type: \'%ls\'\n",lVar14,lVar3);
                  lVar12 = lVar2;
                  do {
                    lVar12 = lVar12 + 1;
                  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
                  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 +
                                *(longlong *)(param_1 + 0x118) + 0x100,lVar3);
                }
                else if ((iVar8 == -0x7ffbfdee) && (iVar7 != 0)) goto code_r0x000140224ff0;
                goto LAB_140225050;
              }
LAB_1402253c4:
              Ordinal_6(local_res10);
            }
          }
          iVar7 = (**(code **)(*local_res18 + 0x18))(local_res18,1,local_68);
        }
      }
    }
  }
  do {
    lVar10 = lVar10 + 1;
  } while (*(char *)(lVar14 + lVar10) != '\0');
  if (lVar10 != 0) {
    do {
      lVar14 = lVar2;
      lVar2 = lVar14 + 1;
    } while (*(char *)(param_1 + 0x13a + lVar14) != '\0');
    *(undefined1 *)(lVar14 + 0x139 + param_1) = 0;
  }
  return;
code_r0x000140224d50:
  iVar7 = iVar7 + -1;
  Sleep(100);
  if (iVar7 < 1) goto LAB_140224db0;
  goto LAB_140224d30;
LAB_140224db0:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"PhysicalLinkStatus");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_140224e60;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_140224e60;
  }
  lVar3 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sPhysical link status: \'%ls\'\n",lVar14,lVar3);
  lVar12 = lVar2;
  do {
    lVar12 = lVar12 + 1;
  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 + *(longlong *)(param_1 + 0x118) + 0x1a8,
                lVar3);
LAB_140224e60:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"Layer1UpstreamMaxBitrate");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_140224ef5;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_140224ef5;
  }
  uVar4 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sUpstream max bitrate: \'%d\'\n",lVar14,uVar4 & 0xffffffff);
  *(int *)((longlong)*(int *)(param_1 + 300) * 0x1d0 + 0x1c8 + *(longlong *)(param_1 + 0x118)) =
       (int)uVar4;
LAB_140224ef5:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"Layer1DownstreamMaxBitrate");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_140224f86;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_140224f86;
  }
  uVar4 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sDownstream max bitrate: \'%d\'\n",lVar14,uVar4 & 0xffffffff);
  *(int *)((longlong)*(int *)(param_1 + 300) * 0x1d0 + 0x1cc + *(longlong *)(param_1 + 0x118)) =
       (int)uVar4;
LAB_140224f86:
  Ordinal_6(uVar9);
  goto LAB_140224f8f;
code_r0x000140224ff0:
  iVar7 = iVar7 + -1;
  Sleep(100);
  if (iVar7 < 1) goto LAB_140225050;
  goto LAB_140224fd0;
LAB_140225050:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"ConnectionStatus");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_140225100;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_140225100;
  }
  lVar3 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sConnection Status: \'%ls\'\n",lVar14,lVar3);
  lVar12 = lVar2;
  do {
    lVar12 = lVar12 + 1;
  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 + *(longlong *)(param_1 + 0x118) + 0x120,
                lVar3);
LAB_140225100:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"ExternalIPAddress");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_1402251b0;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_1402251b0;
  }
  lVar3 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sExternal IP Address: \'%ls\'\n",lVar14,lVar3);
  lVar12 = lVar2;
  do {
    lVar12 = lVar12 + 1;
  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 + *(longlong *)(param_1 + 0x118) + 0x140,
                lVar3);
LAB_1402251b0:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"RemoteHost");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), iVar8 != 0) {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_140225260;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_140225260;
  }
  lVar3 = CONCAT62(uStack_76,local_78);
  FUN_140025760("%sRemote Host: \'%ls\'\n",lVar14,lVar3);
  lVar12 = lVar2;
  do {
    lVar12 = lVar12 + 1;
  } while (*(short *)(lVar3 + lVar12 * 2) != 0);
  FUN_1400228a0((longlong)*(int *)(param_1 + 300) * 0x1d0 + *(longlong *)(param_1 + 0x118) + 0x160,
                lVar3);
LAB_140225260:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"NATEnabled");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), sVar6 = local_78, iVar8 != 0)
  {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_14022530b;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_14022530b;
  }
  puVar13 = &DAT_140853b28;
  if (local_78 != 0) {
    puVar13 = &DAT_14084ec54;
  }
  FUN_140025760("%sNAT Enabled: \'%s\'\n",lVar14,puVar13);
  *(bool *)((longlong)*(int *)(param_1 + 300) * 0x1d0 + 0x180 + *(longlong *)(param_1 + 0x118)) =
       sVar6 != 0;
LAB_14022530b:
  Ordinal_6(uVar9);
  uVar9 = Ordinal_2(L"PortMappingEnabled");
  plVar5 = local_res8;
  Ordinal_8(local_80);
  iVar7 = 10;
  while (iVar8 = (**(code **)(*plVar5 + 0x38))(plVar5,uVar9,local_80), sVar6 = local_78, iVar8 != 0)
  {
    if ((iVar8 != -0x7ffbfdee) || (iVar7 == 0)) goto LAB_1402253bb;
    iVar7 = iVar7 + -1;
    Sleep(100);
    if (iVar7 < 1) goto LAB_1402253bb;
  }
  puVar13 = &DAT_140853b28;
  if (local_78 != 0) {
    puVar13 = &DAT_14084ec54;
  }
  FUN_140025760("%sPort Mapping Enabled: \'%s\'\n",lVar14,puVar13);
  *(bool *)((longlong)*(int *)(param_1 + 300) * 0x1d0 + 0x181 + *(longlong *)(param_1 + 0x118)) =
       sVar6 != 0;
LAB_1402253bb:
  Ordinal_6(uVar9);
  goto LAB_1402253c4;
}



// ============================================================
// RVA: 0x5d990  Size: 7440 bytes
// ============================================================

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



// ============================================================
// RVA: 0xc9900  Size: 1274 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1400c9900(int *param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  float fVar3;
  bool bVar4;
  char cVar5;
  DWORD DVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  longlong lVar10;
  uint uVar11;
  int *_Buf1;
  undefined8 *puVar12;
  bool bVar13;
  float fVar14;
  char *local_48 [2];
  undefined8 local_38;
  ulonglong local_30;
  
  iVar7 = *param_1;
  if (iVar7 == 0) {
    if (DAT_140a62014 == -1) {
      FUN_1400cab70();
      return;
    }
  }
  else if (iVar7 == 1) {
    _Buf1 = param_1 + 2;
    if (0xf < *(ulonglong *)(param_1 + 8)) {
      _Buf1 = *(int **)_Buf1;
    }
    if ((*(longlong *)(param_1 + 6) == 0x15) &&
       (iVar7 = memcmp(_Buf1,"IDC_LAN_CANCEL_BUTTON",0x15), iVar7 == 0)) {
      FUN_1400cab70();
      return;
    }
  }
  else if (iVar7 == 0xc) {
    if (DAT_140a62030 == 0) {
      if ((DAT_140a62014 != -1) &&
         (DVar6 = timeGetTime(), 20000 < (DVar6 - _DAT_140a7bec0) - DAT_140a62014)) {
        DAT_140a62014 = -1;
        local_48[0] = (char *)FUN_140769c58(0x20);
        uVar2 = s_IDC_LAN_CANCEL_BUTTON_140800258._8_8_;
        local_38 = 0x15;
        local_30 = 0x1f;
        *(undefined8 *)local_48[0] = s_IDC_LAN_CANCEL_BUTTON_140800258._0_8_;
        *(undefined8 *)(local_48[0] + 8) = uVar2;
        *(undefined4 *)(local_48[0] + 0x10) = s_IDC_LAN_CANCEL_BUTTON_140800258._16_4_;
        local_48[0][0x14] = s_IDC_LAN_CANCEL_BUTTON_140800258[0x14];
        local_48[0][0x15] = '\0';
        FUN_1401c2a40(&DAT_140a78510,DAT_140a118f8,local_48);
        if (0xf < local_30) {
          if (0xfff < local_30 + 1) {
            if ((char *)0x1f < local_48[0] + (-8 - *(longlong *)(local_48[0] + -8))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
      }
      if (DAT_140a62011 == '\0') {
        FUN_14002d9d0();
      }
      else {
        DAT_140a62011 = '\0';
        FUN_14002d790(DAT_140a62012);
      }
      iVar7 = FUN_140294b80(&DAT_140a16fd0);
      bVar13 = false;
      if (DAT_140a62018 < iVar7 + -1) {
        iVar8 = FUN_1400c91e0();
        bVar13 = iVar8 != DAT_140a62018;
        DAT_140a62018 = iVar8;
      }
      iVar8 = FUN_140294b80(&DAT_140a16fd0);
      fVar14 = (float)DAT_140a62018 / (float)(iVar8 + -1) + DAT_140815dec;
      fVar3 = fVar14;
      if (((DAT_140815de4 <= fVar14 - DAT_140a62028) || (DAT_140815de4 <= DAT_140a62028 - fVar14))
         || (fVar3 = DAT_140a62028, DAT_140a6202c == 0)) {
        DAT_140a62028 = fVar3;
        DVar6 = timeGetTime();
        DAT_140a6202c = DVar6 - _DAT_140a7bec0;
      }
      else {
        DVar6 = timeGetTime();
        uVar9 = (DVar6 - DAT_140a6202c) - _DAT_140a7bec0;
        uVar11 = 10000;
        if (uVar9 < 10000) {
          uVar11 = uVar9;
        }
        fVar14 = fVar14 + ((float)uVar11 * DAT_140815de8) / (DAT_1408007f8 * DAT_140805898);
      }
      local_48[0] = (char *)FUN_140769c58(0x20);
      uVar2 = s_IDC_NAT_PROGRESS_BAR_140815a08._8_8_;
      local_38 = 0x14;
      local_30 = 0x1f;
      *(undefined8 *)local_48[0] = s_IDC_NAT_PROGRESS_BAR_140815a08._0_8_;
      *(undefined8 *)(local_48[0] + 8) = uVar2;
      *(undefined4 *)(local_48[0] + 0x10) = s_IDC_NAT_PROGRESS_BAR_140815a08._16_4_;
      local_48[0][0x14] = '\0';
      FUN_1401c73d0(&DAT_140a78510,DAT_140a118f8,local_48,fVar14);
      if (0xf < local_30) {
        if (0xfff < local_30 + 1) {
          if ((char *)0x1f < local_48[0] + (-8 - *(longlong *)(local_48[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      if (DAT_140a62018 == iVar7 + -1) {
        if (bVar13) {
          DAT_140a7c479 = 0;
          lVar10 = FUN_140294a40(&DAT_140a16fd0);
          if (lVar10 != 0) {
            FUN_140288880(lVar10,&PTR_vftable_140a11da8);
            *(undefined1 *)(lVar10 + 0x183) = 1;
          }
          FUN_1400c8d80();
        }
        else {
          if (DAT_140a62013 == '\0') {
            cVar5 = FUN_1400c8690();
            if (cVar5 == '\0') {
              return;
            }
            DAT_140a7c478 = 1;
            Sleep(0x14);
            DAT_140a62013 = FUN_1400cb330();
            if (DAT_140a62013 != '\0') {
              DAT_140a7c478 = 0;
              return;
            }
          }
          else {
            bVar13 = true;
            DAT_140a7c478 = 1;
            Sleep(0);
            iVar7 = 0;
            bVar4 = true;
            if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
              do {
                bVar13 = bVar4;
                lVar10 = FUN_140294bc0(&DAT_140a16fd0,iVar7);
                if (((lVar10 != 0) && (*(char *)(lVar10 + 0x62) != '\0')) &&
                   (lVar1 = *(longlong *)(lVar10 + 0x428), lVar1 != 0)) {
                  FUN_14022ef80(lVar1);
                  cVar5 = FUN_14022ef00(lVar1);
                  if (cVar5 == '\0') {
                    bVar13 = false;
                  }
                  else {
                    if ((*(char *)(lVar1 + 0x35) != '\0') &&
                       (puVar12 = (undefined8 *)(lVar1 + 0x38), puVar12 != &DAT_140a11928)) {
                      if (0xf < *(ulonglong *)(lVar1 + 0x50)) {
                        puVar12 = (undefined8 *)*puVar12;
                      }
                      FUN_140022730(&DAT_140a11928,puVar12,*(undefined8 *)(lVar1 + 0x48));
                    }
                    FUN_1400ccf10(lVar1,1);
                    *(undefined8 *)(lVar10 + 0x428) = 0;
                  }
                }
                iVar7 = iVar7 + 1;
                bVar4 = bVar13;
              } while (iVar7 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
            }
            DAT_140a7c478 = 0;
            if (!bVar13) {
              FUN_1400cc2d0();
              return;
            }
            DAT_140a62013 = '\0';
          }
          DAT_140a7c478 = 0;
          FUN_1400c7ca0();
        }
      }
    }
    else {
      DVar6 = timeGetTime();
      if (3000 < (DVar6 - _DAT_140a7bec0) - DAT_140a62030) {
        FUN_1400c7c50();
        return;
      }
    }
  }
  return;
}



// ============================================================
// RVA: 0x8e9a0  Size: 1359 bytes
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14008e9a0(void)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  float fVar5;
  char cVar6;
  DWORD DVar7;
  int iVar8;
  uint uVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined **ppuVar12;
  longlong lVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  char *local_a8 [2];
  undefined8 local_98;
  ulonglong local_90;
  tagMSG local_88 [2];
  
  DVar7 = timeGetTime();
  fVar5 = DAT_1408054cc;
  fVar14 = DAT_1407ffaf8;
  iVar8 = DVar7 - _DAT_140a7bec0;
  lVar11 = 0;
  fVar15 = (float)(uint)(iVar8 - _DAT_140a61d48) / DAT_1408007f8;
  fVar16 = (float)(uint)(iVar8 - _DAT_140a61d3c) / DAT_1408007f8;
  _DAT_140a61d48 = iVar8;
  if ((DAT_140a61d3a == '\0') && (((DAT_140a61d54 - 1U & 0xfffffffc) != 0 || (DAT_140a61d54 == 3))))
  {
    fVar14 = (DAT_1407ffaf8 - DAT_1408054cc) * DAT_140a61d4c + DAT_140a61d50 * DAT_1408054cc;
    if (DAT_140a61d4c < fVar14) {
      DAT_140a61d4c = fVar14;
    }
    if (DAT_140a61de8 == 0) {
      local_a8[0] = (char *)FUN_140769c58(0x20);
      uVar4 = s_IDC_PROGRESS_SOLO_140805270._12_4_;
      uVar3 = s_IDC_PROGRESS_SOLO_140805270._8_4_;
      uVar2 = s_IDC_PROGRESS_SOLO_140805270._4_4_;
      local_98 = 0x11;
      local_90 = 0x1f;
      *(undefined4 *)local_a8[0] = s_IDC_PROGRESS_SOLO_140805270._0_4_;
      *(undefined4 *)(local_a8[0] + 4) = uVar2;
      *(undefined4 *)(local_a8[0] + 8) = uVar3;
      *(undefined4 *)(local_a8[0] + 0xc) = uVar4;
      local_a8[0][0x10] = s_IDC_PROGRESS_SOLO_140805270[0x10];
      local_a8[0][0x11] = '\0';
      FUN_1401c73d0(&DAT_140a78510,DAT_140a110d4,local_a8,DAT_140a61d4c);
      if (local_90 < 0x10) goto LAB_14008ec83;
      if (0xfff < local_90 + 1) {
        if ((char *)0x1f < local_a8[0] + (-8 - *(longlong *)(local_a8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_a8[0] + -8),local_90 + 0x28);
        }
      }
    }
    else {
      local_a8[0] = (char *)0x0;
      lVar11 = -1;
      local_98 = 0;
      local_90 = 0xf;
      do {
        lVar11 = lVar11 + 1;
      } while (*(char *)(DAT_140a61de8 + lVar11) != '\0');
      FUN_140022730(local_a8,DAT_140a61de8,lVar11);
      FUN_1401c73d0(&DAT_140a78510,DAT_140a110d4,local_a8,DAT_140a61d4c);
      if (local_90 < 0x10) goto LAB_14008ec83;
      if (0xfff < local_90 + 1) {
        if ((char *)0x1f < local_a8[0] + (-8 - *(longlong *)(local_a8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_a8[0] + -8),local_90 + 0x28);
        }
      }
    }
  }
  else {
    if (DAT_140a61d84 != 0) {
      ppuVar12 = &PTR_s_IDC_PROGRESS0_140a11130;
      do {
        if (*(int *)((longlong)&DAT_140a61db0 + lVar11) != -1) {
          puVar1 = *ppuVar12;
          lVar13 = -1;
          local_a8[0] = (char *)0x0;
          local_98 = 0;
          local_90 = 0xf;
          *(float *)((longlong)&DAT_140a61d60 + lVar11) =
               *(float *)((longlong)&DAT_140a61d88 + lVar11) * fVar5 +
               (fVar14 - fVar5) * *(float *)((longlong)&DAT_140a61d60 + lVar11);
          do {
            lVar13 = lVar13 + 1;
          } while (puVar1[lVar13] != '\0');
          FUN_140022730(local_a8);
          FUN_1401c73d0(&DAT_140a78510,DAT_140a110d4,local_a8,
                        *(undefined4 *)((longlong)&DAT_140a61d60 + lVar11));
          if (0xf < local_90) {
            if (0xfff < local_90 + 1) {
              if ((char *)0x1f < local_a8[0] + (-8 - *(longlong *)(local_a8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(*(longlong *)(local_a8[0] + -8),local_90 + 0x28);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        ppuVar12 = ppuVar12 + 1;
        lVar11 = lVar11 + 4;
      } while ((longlong)ppuVar12 < 0x140a11178);
      goto LAB_14008ec83;
    }
    DAT_140a61d60 = (DAT_1407ffaf8 - DAT_1408054cc) * DAT_140a61d60 + DAT_140a61d88 * DAT_1408054cc;
    local_a8[0] = (char *)FUN_140769c58(0x20);
    uVar4 = s_IDC_PROGRESS_SOLO_140805270._12_4_;
    uVar3 = s_IDC_PROGRESS_SOLO_140805270._8_4_;
    uVar2 = s_IDC_PROGRESS_SOLO_140805270._4_4_;
    local_98 = 0x11;
    local_90 = 0x1f;
    *(undefined4 *)local_a8[0] = s_IDC_PROGRESS_SOLO_140805270._0_4_;
    *(undefined4 *)(local_a8[0] + 4) = uVar2;
    *(undefined4 *)(local_a8[0] + 8) = uVar3;
    *(undefined4 *)(local_a8[0] + 0xc) = uVar4;
    local_a8[0][0x10] = s_IDC_PROGRESS_SOLO_140805270[0x10];
    local_a8[0][0x11] = '\0';
    FUN_1401c73d0(&DAT_140a78510,DAT_140a110d4,local_a8,DAT_140a61d60);
    if (local_90 < 0x10) goto LAB_14008ec83;
    if (0xfff < local_90 + 1) {
      if ((char *)0x1f < local_a8[0] + (-8 - *(longlong *)(local_a8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_a8[0] + -8),local_90 + 0x28);
      }
    }
  }
  thunk_FUN_1407864b8();
LAB_14008ec83:
  DVar7 = timeGetTime();
  iVar8 = DVar7 - _DAT_140a7bec0;
  if ((uint)(iVar8 - DAT_140a61dac) < 0x32) {
    uVar9 = (DAT_140a61dac - iVar8) + 0x30;
    DVar7 = 0;
    if (uVar9 < 0x3e9) {
      DVar7 = uVar9;
    }
    if (10 < DVar7) {
      DVar7 = 10;
    }
    Sleep(DVar7);
  }
  else {
    DAT_140a61dac = iVar8;
    FUN_1402718e0(fVar16);
    FUN_14025ca30(DAT_140a619f0,fVar15);
    FUN_140271840(fVar15);
    uVar10 = FUN_1401fe0d0(&DAT_140a7bca0,0);
    FUN_14025fc80(uVar10);
    cVar6 = FUN_14025de30();
    if (cVar6 != '\0') {
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
    }
    local_88[0].wParam = 0;
    iVar8 = PeekMessageA(local_88,(HWND)0x0,0,0,3);
    while (iVar8 != 0) {
      iVar8 = TranslateAcceleratorA(local_88[0].hwnd,(HACCEL)0x0,local_88);
      if (iVar8 == 0) {
        TranslateMessage(local_88);
        DispatchMessageA(local_88);
      }
      iVar8 = PeekMessageA(local_88,(HWND)0x0,0,0,3);
    }
  }
  return;
}



