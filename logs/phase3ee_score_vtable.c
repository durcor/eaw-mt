
// ================================================================
// FUN_14004fd50 — TheGameScoringManagerClass::Get() singleton getter
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4fd50  Name: FUN_14004fd50  Size: 137 bytes
// ------------------------------------------------------------

longlong FUN_14004fd50(void)

{
  longlong lVar1;
  undefined **local_res8 [4];
  
  if (DAT_140a109a0 == 0) {
    lVar1 = FUN_140769c58(0x20);
    if (lVar1 == 0) {
      DAT_140a109a0 = 0;
    }
    else {
      DAT_140a109a0 = FUN_140485340(lVar1);
    }
    local_res8[0] = &PTR_vftable_140a10998;
    if (DAT_140a7bc10 != DAT_140a7bc18) {
      *DAT_140a7bc10 = &PTR_vftable_140a10998;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
      return DAT_140a109a0;
    }
    FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,local_res8);
  }
  return DAT_140a109a0;
}




// ================================================================
// FUN_14046a350 — called after GameScoringManager::System_Initialize
// ================================================================

// ------------------------------------------------------------
// RVA: 0x46a350  Name: FUN_14046a350  Size: 805 bytes
// ------------------------------------------------------------

undefined8 FUN_14046a350(longlong param_1,undefined8 *param_2)

{
  ulonglong uVar1;
  int iVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong *plVar7;
  ulonglong uVar8;
  longlong *plVar9;
  longlong lVar10;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  plVar9 = (longlong *)(param_1 + 0x460);
  if (plVar9 != param_2) {
    puVar3 = param_2;
    if (0xf < (ulonglong)param_2[3]) {
      puVar3 = (undefined8 *)*param_2;
    }
    FUN_140022730(plVar9,puVar3,param_2[2]);
  }
  FUN_1404683c0(param_1);
  plVar7 = plVar9;
  if (0xf < *(ulonglong *)(param_1 + 0x478)) {
    plVar7 = (longlong *)*plVar9;
  }
  FUN_140025760("Loading Weather System Modifiers database file %s...\n",plVar7);
  puVar4 = (undefined8 *)FUN_140769c58(0x58);
  puVar3 = (undefined8 *)0x0;
  if (puVar4 != (undefined8 *)0x0) {
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4[2] = 0;
    puVar4[4] = 0;
    puVar4[5] = 0xf;
    puVar4[6] = 0;
    puVar3 = puVar4;
  }
  plVar7 = plVar9;
  if (0xf < *(ulonglong *)(param_1 + 0x478)) {
    plVar7 = (longlong *)*plVar9;
  }
  local_28[0] = 0;
  lVar10 = -1;
  local_18 = 0;
  local_10 = 0xf;
  do {
    lVar10 = lVar10 + 1;
  } while (*(char *)((longlong)plVar7 + lVar10) != '\0');
  FUN_140022730(local_28);
  iVar2 = FUN_1402397d0(puVar3,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  if (iVar2 < 0) {
    plVar7 = plVar9;
    if (0xf < *(ulonglong *)(param_1 + 0x478)) {
      plVar7 = (longlong *)*plVar9;
    }
    FUN_140025760("Error: Unable to open XML file %s\n",plVar7);
    if (0xf < *(ulonglong *)(param_1 + 0x478)) {
      plVar9 = (longlong *)*plVar9;
    }
    FUN_140025760("Error: Can\'t load Weather System Modifiers from database file %s\n",plVar9);
    if (puVar3 != (undefined8 *)0x0) {
      if (puVar3[1] != 0) {
        FUN_1401c0b50(puVar3[1],1);
      }
      if (puVar3[6] != 0) {
        thunk_FUN_1407864b8();
      }
      uVar8 = puVar3[5];
      if (0xf < uVar8) {
        lVar10 = puVar3[2];
        lVar6 = lVar10;
        if (0xfff < uVar8 + 1) {
          lVar6 = *(longlong *)(lVar10 + -8);
          uVar8 = uVar8 + 0x28;
          lVar10 = lVar10 - lVar6;
          if (0x1f < lVar10 - 8U) {
LAB_14046a66c:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(lVar10,uVar8);
          }
        }
        thunk_FUN_1407864b8(lVar6);
      }
      puVar3[4] = 0;
      puVar3[5] = 0xf;
      *(undefined1 *)(puVar3 + 2) = 0;
      thunk_FUN_1407864b8(puVar3,0x58);
    }
    uVar5 = 0;
  }
  else {
    puVar4 = (undefined8 *)&DAT_00000001;
    do {
      local_28[0] = 0;
      local_18 = 0;
      local_10 = 0xf;
      FUN_140239d90(puVar3,&DAT_140851640,local_28);
      if (puVar3 != (undefined8 *)0x0) {
        FUN_140469480(param_1,puVar3);
      }
      iVar2 = FUN_140239d40(puVar3);
      if (iVar2 != 0) {
        puVar4 = (undefined8 *)0x0;
      }
      if (0xf < local_10) {
        if (0xfff < local_10 + 1) {
          if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
    } while ((char)puVar4 == '\x01');
    if (puVar3 != (undefined8 *)0x0) {
      if (puVar3[1] != 0) {
        FUN_1401c0b50(puVar3[1],1);
      }
      if (puVar3[6] != 0) {
        thunk_FUN_1407864b8();
      }
      uVar1 = puVar3[5];
      if (0xf < uVar1) {
        lVar10 = puVar3[2];
        uVar8 = uVar1 + 1;
        lVar6 = lVar10;
        if (0xfff < uVar8) {
          lVar6 = *(longlong *)(lVar10 + -8);
          uVar8 = uVar1 + 0x28;
          lVar10 = lVar10 - lVar6;
          if (0x1f < lVar10 - 8U) goto LAB_14046a66c;
        }
        thunk_FUN_1407864b8(lVar6,uVar8);
      }
      puVar3[4] = 0;
      puVar3[5] = 0xf;
      *(undefined1 *)(puVar3 + 2) = 0;
      thunk_FUN_1407864b8(puVar3,0x58);
    }
    uVar5 = 1;
  }
  return uVar5;
}




// ================================================================
// FUN_14004da50 — called on system init error
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4da50  Name: FUN_14004da50  Size: 1199 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14004da50(void)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  
  if (DAT_140b30690 != 0) {
    FUN_14044f130(&DAT_140b306dd);
  }
  FUN_1400c6e30();
  FUN_140051ba0(&DAT_140a10a98,0);
  FUN_140050240(&DAT_140a109b0,0);
  FUN_14048aff0(&DAT_140a2aa40);
  uVar2 = FUN_14004fc30();
  FUN_140475540(uVar2);
  FUN_140332920(&DAT_140a172d0);
  FUN_14030c2e0(&DAT_140b27f60);
  FUN_14044cfc0(&DAT_140a28c10);
  FUN_1404724c0(&DAT_140a2a060);
  FUN_14047add0(&DAT_140b31160);
  FUN_1404746a0(&DAT_140a2a0b0);
  FUN_1402d7060(&DAT_140b27e60);
  FUN_140467f40(&DAT_140a29f20);
  FUN_140470d20(&DAT_140a29fa0);
  FUN_140470430(&DAT_140a29f60);
  FUN_14048a180(&DAT_140a2a9c0);
  thunk_FUN_140427fc0(&DAT_140b2f170);
  FUN_140359830(&DAT_140b2bcb0,0);
  _guard_check_icall(&DAT_140b2f2e0);
  uVar2 = FUN_14004fd50();
  FUN_140488ab0(uVar2);
  FUN_1402d2900(&DAT_140b27dc0);
  FUN_140321c80(&DAT_140b2b300);
  FUN_140322730(&DAT_140b2b3a8);
  FUN_1403207a0(&DAT_140b2b2f0);
  thunk_FUN_140477030(&DAT_140a2a120);
  FUN_1401c8070(&DAT_140a78510,0);
  FUN_1401f9aa0(&DAT_140a7bc28);
  FUN_1401f9aa0(&DAT_140a7bc58);
  FUN_14021c940(&DAT_140a7d0a0);
  thunk_FUN_1402c91c0(&DAT_140b15800);
  thunk_FUN_1404456f0(&DAT_140b30160);
  FUN_14048c510(&DAT_140b31330);
  FUN_14046a680(&DAT_140b30b40);
  FUN_140325150(&DAT_140b2b3b0);
  FUN_1404828d0(&DAT_140a2a850);
  FUN_140483180();
  FUN_140489bb0(&DAT_140a2a980);
  uVar2 = FUN_14004fb30();
  FUN_14046f8a0(uVar2);
  uVar2 = FUN_14001fb80();
  FUN_14007d3a0(uVar2);
  FUN_14047f690();
  FUN_14048f1d0();
  FUN_1404809d0(&DAT_140a2a810);
  FUN_1404817f0();
  FUN_140489320();
  FUN_14048f8c0(&PTR_vftable_140a2aa78);
  uVar2 = FUN_14004faa0();
  FUN_140476820(uVar2);
  if (DAT_140a619ed != '\0') {
    FUN_140030770(&DAT_140a60df0);
    FUN_1403b1af0(&DAT_140a286f0);
    uVar2 = FUN_14004fba0();
    FUN_140263300(uVar2);
    if (DAT_1409cf008 != 0) {
      puVar3 = DAT_140a7bc08;
      if (DAT_140a7bc08 != DAT_140a7bc10) {
        do {
          if ((undefined *)*puVar3 == &DAT_1409cf000) break;
          puVar3 = puVar3 + 1;
        } while (puVar3 != DAT_140a7bc10);
        if (puVar3 != DAT_140a7bc10) {
          FUN_14076bd80(puVar3,puVar3 + 1,(longlong)DAT_140a7bc10 - (longlong)(puVar3 + 1));
          DAT_140a7bc10 = DAT_140a7bc10 + -1;
          if (DAT_140a7bc08 == DAT_140a7bc10) {
            DAT_140a7bc10 = DAT_140a7bc08;
          }
        }
      }
      lVar1 = DAT_1409cf008;
      if (DAT_1409cf008 != 0) {
        FUN_14007dd80(DAT_1409cf008);
        thunk_FUN_1407864b8(lVar1,0xd0);
      }
      DAT_1409cf008 = 0;
    }
    if (DAT_140a61a08 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a08 = 0;
    }
    if (DAT_140a61a10 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a10 = 0;
    }
    if (DAT_140a61a18 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a18 = 0;
    }
    if (DAT_140b2eea0 != (undefined8 *)0x0) {
      (**(code **)*DAT_140b2eea0)(DAT_140b2eea0,1);
      DAT_140b2eea0 = (undefined8 *)0x0;
    }
    if (DAT_140a619f8 != (undefined8 *)0x0) {
      (**(code **)*DAT_140a619f8)(DAT_140a619f8,1);
      DAT_140a619f8 = (undefined8 *)0x0;
    }
    _DAT_140a28790 = 0;
    DAT_140b306a0 = 0;
    FUN_1400fc8d0();
    FUN_1400fc9a0();
    if (DAT_140a619f0 != (undefined8 *)0x0) {
      FUN_14025c680();
      if (DAT_140a619f0 != (undefined8 *)0x0) {
        (**(code **)*DAT_140a619f0)(DAT_140a619f0,1);
      }
      DAT_140a619f0 = (undefined8 *)0x0;
    }
    FUN_140362b20();
    FUN_14026dbe0();
    FUN_14048a9c0();
    FUN_14025fd10();
    DAT_140a619ed = '\0';
  }
  FUN_1401d4730();
  FUN_1401d2a00();
  FUN_14002d750();
  FUN_140215e10();
  FUN_140049290(&DAT_1409cf320);
  FUN_140049290(&DAT_1409cf390);
  uVar2 = FUN_14004fcc0();
  FUN_14047fd90(uVar2);
  FUN_140261370(&DAT_140b0a130);
  FUN_140261260(&DAT_140b0a130,DAT_140a61a30,"EmpireAtWar-Bold");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a38,"EmpireAtWar-Light");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a40,"EmpireAtWar-Medium");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a48,"EmpireAtWar-Stencil");
  DAT_140b15668 = 0;
  return;
}




// ================================================================
// ALL CALLERS OF FUN_14004fd50 (GameScoringManagerClass::Get)
// ================================================================

// Caller: RVA 0x4b250  Name: FUN_14004b250  Size: 10192 bytes
// Caller: RVA 0x4da50  Name: FUN_14004da50  Size: 1199 bytes
// Caller: RVA 0x88fe0  Name: FUN_140088fe0  Size: 10281 bytes
// Caller: RVA 0x92bb0  Name: FUN_140092bb0  Size: 4897 bytes
// Caller: RVA 0x28d400  Name: FUN_14028d400  Size: 654 bytes
// Caller: RVA 0x28c900  Name: FUN_14028c900  Size: 2798 bytes
// Caller: RVA 0x4994c0  Name: FUN_1404994c0  Size: 158 bytes
// Caller: RVA 0x499420  Name: FUN_140499420  Size: 158 bytes
// Caller: RVA 0x5752a0  Name: FUN_1405752a0  Size: 361 bytes
// Caller: RVA 0x616740  Name: FUN_140616740  Size: 2708 bytes
// Caller: RVA 0x6171e0  Name: FUN_1406171e0  Size: 3463 bytes
// Caller: RVA 0x28b080  Name: FUN_14028b080  Size: 2249 bytes
// Caller: RVA 0x35f970  Name: FUN_14035f970  Size: 3899 bytes


// ================================================================
// Small caller: RVA 0x4994c0 (158 bytes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4994c0  Name: FUN_1404994c0  Size: 158 bytes
// ------------------------------------------------------------

void FUN_1404994c0(undefined4 param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  
  lVar1 = FUN_140294bc0(&DAT_140a16fd0,param_1);
  *(undefined1 *)(lVar1 + 100) = 1;
  uVar2 = FUN_14004fd50();
  FUN_140488190(uVar2,lVar1);
  iVar4 = DAT_140a2ab00._4_4_ * 2 >> 1;
  if (iVar4 <= (int)DAT_140a2ab00) {
    iVar3 = iVar4 / 10;
    if (iVar3 < 10) {
      iVar3 = 10;
    }
    FUN_140021ec0(&PTR_vftable_140a2aaf0,iVar3 + iVar4,0,1);
  }
  *(undefined4 *)(DAT_140a2aaf8 + (longlong)(int)DAT_140a2ab00 * 4) = param_1;
  DAT_140a2ab00._0_4_ = (int)DAT_140a2ab00 + 1;
  return;
}




// ================================================================
// Small caller: RVA 0x499420 (158 bytes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x499420  Name: FUN_140499420  Size: 158 bytes
// ------------------------------------------------------------

void FUN_140499420(longlong param_1)

{
  undefined4 uVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  int iVar5;
  
  lVar3 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x24));
  *(undefined1 *)(lVar3 + 99) = 1;
  uVar4 = FUN_14004fd50();
  FUN_140488190(uVar4,lVar3);
  uVar1 = *(undefined4 *)(param_1 + 0x24);
  iVar5 = DAT_140a2ab00._4_4_ * 2 >> 1;
  if (iVar5 <= (int)DAT_140a2ab00) {
    iVar2 = iVar5 / 10;
    if (iVar5 / 10 < 10) {
      iVar2 = 10;
    }
    FUN_140021ec0(&PTR_vftable_140a2aaf0,iVar2 + iVar5,0,1);
  }
  *(undefined4 *)(DAT_140a2aaf8 + (longlong)(int)DAT_140a2ab00 * 4) = uVar1;
  DAT_140a2ab00._0_4_ = (int)DAT_140a2ab00 + 1;
  return;
}



