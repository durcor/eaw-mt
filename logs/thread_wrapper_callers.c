// thread wrapper called from FUN_0x2056d0 (site 0x205807)
// thread wrapper called from FUN_0x205de0 (site 0x20603d)
// thread wrapper called from FUN_0x8df00 (site 0x8e1c1)
// thread wrapper called from FUN_0x225bf0 (site 0x225d41)
// thread wrapper called from FUN_0x2059f0 (site 0x205cbe)
// Total thread-creation callers: 5

// ------------------------------------------------------------
// RVA: 0x2056d0  Name: FUN_1402056d0  Size: 404 bytes
// ------------------------------------------------------------

bool FUN_1402056d0(longlong param_1,short param_2,char param_3)

{
  longlong *plVar1;
  longlong lVar2;
  DWORD DVar3;
  int iVar4;
  BOOL BVar5;
  longlong *plVar6;
  longlong *plVar7;
  longlong *plVar8;
  char *pcVar9;
  bool bVar10;
  longlong *local_res8;
  int local_28;
  short local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_18;
  
  plVar1 = (longlong *)(param_1 + 0xd0);
  plVar8 = local_res8;
  if (plVar1 != (longlong *)0x0) {
    DVar3 = WaitForSingleObject((HANDLE)*plVar1,10000);
    plVar8 = plVar1;
    if (DVar3 == 0) {
      *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Init";
    }
    else {
      pcVar9 = "Unknown";
      if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
        pcVar9 = *(char **)(param_1 + 0xd8);
      }
      FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                    ,"PacketHandlerClass::Init",pcVar9);
    }
  }
  FUN_140202370(&local_28,0,0);
  local_28 = 0;
  local_18 = 0x101;
  if (param_2 == 0) {
    param_2 = 0x4d2;
  }
  local_20 = 0x8000;
  *(short *)(param_1 + 0xa0) = param_2;
  local_1c = 0x2000;
  local_24 = param_2;
  if ((param_3 != '\0') && (iVar4 = FUN_140208b10(), iVar4 != 0)) {
    local_28 = iVar4;
  }
  plVar6 = (longlong *)FUN_140227030(&local_res8,&local_28);
  plVar1 = *(longlong **)(param_1 + 0x98);
  plVar7 = (longlong *)*plVar6;
  if (plVar1 != plVar7) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))(plVar1);
      plVar7 = (longlong *)*plVar6;
    }
    *(longlong **)(param_1 + 0x98) = plVar7;
    if (plVar7 != (longlong *)0x0) {
      (**(code **)*plVar7)();
    }
  }
  if (local_res8 != (longlong *)0x0) {
    (**(code **)(*local_res8 + 8))();
  }
  bVar10 = *(longlong *)(param_1 + 0x98) != 0;
  if (bVar10) {
    FUN_14022e490(param_1);
  }
  if (plVar8 != (longlong *)0x0) {
    lVar2 = plVar8[1];
    plVar8[1] = 0;
    BVar5 = ReleaseMutex((HANDLE)*plVar8);
    if (BVar5 == 0) {
      DVar3 = GetLastError();
      FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                    "RunDebugBuild",lVar2);
    }
  }
  return bVar10;
}



// ------------------------------------------------------------
// RVA: 0x205de0  Name: FUN_140205de0  Size: 707 bytes
// ------------------------------------------------------------

undefined8 FUN_140205de0(longlong param_1,longlong *param_2)

{
  undefined8 *puVar1;
  longlong lVar2;
  DWORD DVar3;
  BOOL BVar4;
  undefined8 uVar5;
  longlong *plVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  char *pcVar9;
  undefined8 *local_res8;
  
  if (*(longlong *)(param_1 + 0x98) != 0) {
    puVar1 = (undefined8 *)(param_1 + 0xd0);
    puVar8 = local_res8;
    if (puVar1 != (undefined8 *)0x0) {
      DVar3 = WaitForSingleObject((HANDLE)*puVar1,10000);
      puVar8 = puVar1;
      if (DVar3 == 0) {
        *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Rebind";
      }
      else {
        pcVar9 = "Unknown";
        if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
          pcVar9 = *(char **)(param_1 + 0xd8);
        }
        FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                      ,"PacketHandlerClass::Rebind",pcVar9);
      }
    }
    if (*(longlong **)(param_1 + 0x98) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x98) + 8))();
      *(undefined8 *)(param_1 + 0x98) = 0;
    }
    if (puVar8 != (undefined8 *)0x0) {
      uVar5 = puVar8[1];
      puVar8[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*puVar8);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",uVar5);
      }
    }
  }
  FUN_14022e560(param_1);
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  lVar2 = *(longlong *)(param_1 + 0x90);
  while (lVar2 != 0) {
    FUN_140209540(&DAT_140a14b80,*(undefined8 *)(**(longlong **)(param_1 + 0x88) + 0x10));
    plVar6 = (longlong *)**(longlong **)(param_1 + 0x88);
    lVar2 = *plVar6;
    *(longlong *)(param_1 + 0x90) = *(longlong *)(param_1 + 0x90) + -1;
    *(longlong *)plVar6[1] = lVar2;
    *(longlong *)(lVar2 + 8) = plVar6[1];
    lVar2 = plVar6[2];
    if (lVar2 != 0) {
      (**(code **)(*(longlong *)(lVar2 + 0x60) + 8))(lVar2 + 0x60);
    }
    thunk_FUN_1407864b8(plVar6,0x18);
    lVar2 = *(longlong *)(param_1 + 0x90);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  puVar1 = (undefined8 *)(param_1 + 0xd0);
  if (puVar1 != (undefined8 *)0x0) {
    DVar3 = WaitForSingleObject((HANDLE)*puVar1,10000);
    local_res8 = puVar1;
    if (DVar3 == 0) {
      *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Init";
    }
    else {
      pcVar9 = "Unknown";
      if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
        pcVar9 = *(char **)(param_1 + 0xd8);
      }
      FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                    ,"PacketHandlerClass::Init",pcVar9);
    }
  }
  plVar6 = *(longlong **)(param_1 + 0x98);
  plVar7 = (longlong *)*param_2;
  if (plVar6 != plVar7) {
    if (plVar6 != (longlong *)0x0) {
      (**(code **)(*plVar6 + 8))();
      plVar7 = (longlong *)*param_2;
    }
    *(longlong **)(param_1 + 0x98) = plVar7;
    plVar6 = (longlong *)0x0;
    if (plVar7 != (longlong *)0x0) {
      (**(code **)*plVar7)();
      plVar6 = *(longlong **)(param_1 + 0x98);
    }
  }
  if (plVar6 == (longlong *)0x0) {
    if (local_res8 != (undefined8 *)0x0) {
      uVar5 = local_res8[1];
      local_res8[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*local_res8);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",uVar5);
      }
    }
    if ((longlong *)*param_2 != (longlong *)0x0) {
      (**(code **)(*(longlong *)*param_2 + 8))();
    }
    uVar5 = 0;
  }
  else {
    *(undefined2 *)(param_1 + 0xa0) = 1;
    FUN_14022e490(param_1);
    if (local_res8 != (undefined8 *)0x0) {
      uVar5 = local_res8[1];
      local_res8[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*local_res8);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",uVar5);
      }
    }
    if ((longlong *)*param_2 != (longlong *)0x0) {
      (**(code **)(*(longlong *)*param_2 + 8))();
    }
    uVar5 = 1;
  }
  return uVar5;
}



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



// ------------------------------------------------------------
// RVA: 0x225bf0  Name: FUN_140225bf0  Size: 386 bytes
// ------------------------------------------------------------

void FUN_140225bf0(longlong param_1,undefined4 param_2,undefined4 param_3,undefined8 param_4)

{
  int iVar1;
  undefined8 *puVar2;
  int iVar3;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa0));
  iVar1 = *(int *)(param_1 + 0xd8);
  iVar3 = *(int *)(param_1 + 0xdc) * 2 >> 1;
  if (iVar3 <= iVar1) {
    iVar1 = iVar3 / 10;
    if (iVar3 / 10 < 10) {
      iVar1 = 10;
    }
    FUN_140021ec0(param_1 + 200,iVar1 + iVar3,0,1);
    iVar1 = *(int *)(param_1 + 0xd8);
  }
  *(undefined4 *)(*(longlong *)(param_1 + 0xd0) + (longlong)iVar1 * 4) = param_2;
  *(int *)(param_1 + 0xd8) = *(int *)(param_1 + 0xd8) + 1;
  iVar1 = *(int *)(param_1 + 0xf0);
  iVar3 = *(int *)(param_1 + 0xf4) * 2 >> 1;
  if (iVar3 <= iVar1) {
    iVar1 = iVar3 / 10;
    if (iVar3 / 10 < 10) {
      iVar1 = 10;
    }
    FUN_140021ec0(param_1 + 0xe0,iVar1 + iVar3,0,1);
    iVar1 = *(int *)(param_1 + 0xf0);
  }
  *(undefined4 *)(*(longlong *)(param_1 + 0xe8) + (longlong)iVar1 * 4) = param_3;
  *(int *)(param_1 + 0xf0) = *(int *)(param_1 + 0xf0) + 1;
  FUN_1402268e0(param_1 + 0xf8,param_4);
  if (*(longlong *)(param_1 + 0x130) == 0) {
    puVar2 = (undefined8 *)FUN_140769c58(0x28);
    if (puVar2 == (undefined8 *)0x0) {
      puVar2 = (undefined8 *)0x0;
      *(undefined8 *)(param_1 + 0x130) = 0;
    }
    else {
      FUN_14022e310(puVar2,"NATUtilsThread");
      *puVar2 = NATUtilsClass::NATUtilsThreadClass::vftable;
      *(undefined8 **)(param_1 + 0x130) = puVar2;
    }
  }
  else {
    if (*(char *)(*(longlong *)(param_1 + 0x130) + 9) == '\0') goto LAB_140225d46;
    FUN_14022e560(*(undefined8 *)(param_1 + 0x130));
    puVar2 = *(undefined8 **)(param_1 + 0x130);
  }
  FUN_14022e490(puVar2);
LAB_140225d46:
                    /* WARNING: Could not recover jumptable at 0x000140225d6b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa0));
  return;
}



// ------------------------------------------------------------
// RVA: 0x2059f0  Name: FUN_1402059f0  Size: 807 bytes
// ------------------------------------------------------------

undefined8 FUN_1402059f0(longlong param_1,short param_2)

{
  longlong *plVar1;
  longlong lVar2;
  DWORD DVar3;
  BOOL BVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  char *pcVar8;
  longlong *local_res8;
  undefined4 local_38;
  short local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined2 local_28;
  
  if (param_2 == *(short *)(param_1 + 0xa0)) {
    return 1;
  }
  if (*(longlong *)(param_1 + 0x98) != 0) {
    plVar1 = (longlong *)(param_1 + 0xd0);
    plVar7 = local_res8;
    if (plVar1 != (longlong *)0x0) {
      DVar3 = WaitForSingleObject((HANDLE)*plVar1,10000);
      plVar7 = plVar1;
      if (DVar3 == 0) {
        *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Rebind";
      }
      else {
        pcVar8 = "Unknown";
        if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
          pcVar8 = *(char **)(param_1 + 0xd8);
        }
        FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                      ,"PacketHandlerClass::Rebind",pcVar8);
      }
    }
    if (*(longlong **)(param_1 + 0x98) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x98) + 8))();
      *(undefined8 *)(param_1 + 0x98) = 0;
    }
    if (plVar7 != (longlong *)0x0) {
      lVar2 = plVar7[1];
      plVar7[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*plVar7);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",lVar2);
      }
    }
  }
  FUN_14022e560(param_1);
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  lVar2 = *(longlong *)(param_1 + 0x90);
  while (lVar2 != 0) {
    FUN_140209540(&DAT_140a14b80,*(undefined8 *)(**(longlong **)(param_1 + 0x88) + 0x10));
    plVar1 = (longlong *)**(longlong **)(param_1 + 0x88);
    lVar2 = *plVar1;
    *(longlong *)(param_1 + 0x90) = *(longlong *)(param_1 + 0x90) + -1;
    *(longlong *)plVar1[1] = lVar2;
    *(longlong *)(lVar2 + 8) = plVar1[1];
    lVar2 = plVar1[2];
    if (lVar2 != 0) {
      (**(code **)(*(longlong *)(lVar2 + 0x60) + 8))(lVar2 + 0x60);
    }
    thunk_FUN_1407864b8(plVar1,0x18);
    lVar2 = *(longlong *)(param_1 + 0x90);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  plVar1 = (longlong *)(param_1 + 0xd0);
  plVar7 = local_res8;
  if (plVar1 != (longlong *)0x0) {
    DVar3 = WaitForSingleObject((HANDLE)*plVar1,10000);
    plVar7 = plVar1;
    if (DVar3 == 0) {
      *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Init";
    }
    else {
      pcVar8 = "Unknown";
      if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
        pcVar8 = *(char **)(param_1 + 0xd8);
      }
      FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                    ,"PacketHandlerClass::Init",pcVar8);
    }
  }
  FUN_140202370(&local_38,0,0);
  local_38 = 0;
  local_28 = 0x101;
  local_30 = 0x8000;
  if (param_2 == 0) {
    param_2 = 0x4d2;
  }
  local_2c = 0x2000;
  *(short *)(param_1 + 0xa0) = param_2;
  local_34 = param_2;
  plVar5 = (longlong *)FUN_140227030(&local_res8,&local_38);
  plVar1 = *(longlong **)(param_1 + 0x98);
  plVar6 = (longlong *)*plVar5;
  if (plVar1 != plVar6) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))(plVar1);
      plVar6 = (longlong *)*plVar5;
    }
    *(longlong **)(param_1 + 0x98) = plVar6;
    if (plVar6 != (longlong *)0x0) {
      (**(code **)*plVar6)();
    }
  }
  if (local_res8 != (longlong *)0x0) {
    (**(code **)(*local_res8 + 8))();
  }
  if (*(longlong *)(param_1 + 0x98) == 0) {
    if (plVar7 != (longlong *)0x0) {
      lVar2 = plVar7[1];
      plVar7[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*plVar7);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",lVar2);
      }
    }
    return 0;
  }
  FUN_14022e490(param_1);
  if (plVar7 != (longlong *)0x0) {
    lVar2 = plVar7[1];
    plVar7[1] = 0;
    BVar4 = ReleaseMutex((HANDLE)*plVar7);
    if (BVar4 == 0) {
      DVar3 = GetLastError();
      FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                    "RunDebugBuild",lVar2);
    }
  }
  return 1;
}




// ================================================================
// Thread proc: FUN_0x22e400 (launched by every _beginthreadex call)
// ================================================================
// ------------------------------------------------------------
// RVA: 0x22e400  Name: FUN_14022e400  Size: 58 bytes
// ------------------------------------------------------------

undefined8 FUN_14022e400(longlong *param_1)

{
  DWORD DVar1;
  HANDLE pvVar2;
  
  param_1 = (longlong *)*param_1;
  if (param_1 != (longlong *)0x0) {
    pvVar2 = GetCurrentThread();
    param_1[3] = (longlong)pvVar2;
    DVar1 = GetCurrentThreadId();
    *(DWORD *)(param_1 + 4) = DVar1;
    *(undefined1 *)(param_1 + 1) = 1;
    (**(code **)(*param_1 + 8))(param_1);
    *(undefined1 *)((longlong)param_1 + 9) = 1;
  }
  return 0;
}



