
/* =========================================================
 * FUN_1404d95a0 — SpaceModeClass vtable slot 22
 * RVA 0x4d95a0
 * size: 358 bytes  params: 0  return: undefined
 * ========================================================= */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1404d95a0(longlong param_1,undefined4 param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong *plVar5;
  
  cVar1 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar1 != '\0') {
    FUN_1402cd870(&DAT_140b27a10);
  }
  plVar5 = (longlong *)**(longlong **)(param_1 + 0x348);
  if (plVar5 != *(longlong **)(param_1 + 0x348)) {
    do {
      if (plVar5[3] != 0) {
        FUN_140527dd0(plVar5[3],param_2);
        iVar2 = FUN_140294a70(&DAT_140a16fd0);
        if ((int)plVar5[2] == iVar2) {
          cVar1 = FUN_1402cd0c0(&DAT_140b27a10);
          if (cVar1 == '\0') {
            DAT_140b2c19c = FUN_1405275f0(plVar5[3]);
          }
        }
      }
      plVar5 = (longlong *)*plVar5;
    } while (plVar5 != (longlong *)*(longlong *)(param_1 + 0x348));
  }
  uVar3 = FUN_14041a240();
  FUN_14059d270(uVar3);
  iVar2 = *(int *)(param_1 + 0x3c0) + 1;
  *(int *)(param_1 + 0x3c0) = iVar2;
  if (DAT_140b27dcc == 7) {
    if ((float)DAT_140b0a340 * _DAT_140b301fc <= (float)iVar2) {
      FUN_14028ee00(&DAT_140b153e0,0);
    }
  }
  if (0 < *(int *)(param_1 + 0x3c4)) {
    *(int *)(param_1 + 0x3c4) = *(int *)(param_1 + 0x3c4) + -1;
  }
  if ((*(longlong *)(param_1 + 0x28) != 0) &&
     (iVar2 = 0, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
    do {
      lVar4 = FUN_140294bc0(&DAT_140a16fd0,iVar2);
      if (lVar4 != 0) {
        FUN_140346460(*(undefined8 *)(param_1 + 0x28),0x11,*(undefined4 *)(lVar4 + 0x4c));
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  FUN_1403639d0(param_1,param_2);
  return;
}



/* =========================================================
 * FUN_1403bb440 — LandModeClass  vtable slot 22
 * RVA 0x3bb440
 * size: 598 bytes  params: 0  return: undefined
 * ========================================================= */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403bb440(longlong param_1,undefined4 param_2)

{
  char *pcVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  longlong lVar6;
  longlong *plVar7;
  undefined **local_res8;
  
  cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar2 != '\0') {
    FUN_1402cd870(&DAT_140b27a10);
  }
  plVar7 = (longlong *)**(longlong **)(param_1 + 0x350);
  if (plVar7 != *(longlong **)(param_1 + 0x350)) {
    do {
      if (plVar7[3] != 0) {
        FUN_140527dd0(plVar7[3],param_2);
        iVar3 = FUN_140294a70(&DAT_140a16fd0);
        if ((int)plVar7[2] == iVar3) {
          cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
          if (cVar2 == '\0') {
            DAT_140b2c19c = FUN_1405275f0(plVar7[3]);
          }
        }
      }
      plVar7 = (longlong *)*plVar7;
    } while (plVar7 != (longlong *)*(longlong *)(param_1 + 0x350));
  }
  iVar3 = 0;
  if (DAT_140a286e0 == 0) {
    lVar6 = FUN_140769c58(0x200);
    if (lVar6 == 0) {
      DAT_140a286e0 = 0;
    }
    else {
      DAT_140a286e0 = FUN_140596850(lVar6);
    }
    local_res8 = &PTR_vftable_140a286d8;
    if (DAT_140a7bc10 == DAT_140a7bc18) {
      FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
    }
    else {
      *DAT_140a7bc10 = &PTR_vftable_140a286d8;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
    }
  }
  FUN_14059d270(DAT_140a286e0);
  if (*(longlong *)(param_1 + 0x328) != 0) {
    FUN_14046a0c0(&DAT_140b30b40,param_2);
  }
  FUN_14056ce20(param_1 + 0x3f8,param_2);
  iVar4 = *(int *)(param_1 + 0x410) + 1;
  *(int *)(param_1 + 0x410) = iVar4;
  if (DAT_140b27dcc == 7) {
    if ((float)DAT_140b0a340 * _DAT_140b301f8 <= (float)iVar4) {
      FUN_14028ee00(&DAT_140b153e0,0);
    }
  }
  if (0 < *(int *)(param_1 + 0x414)) {
    *(int *)(param_1 + 0x414) = *(int *)(param_1 + 0x414) + -1;
  }
  if (*(longlong *)(param_1 + 0x38) != 0) {
    iVar4 = FUN_140294a70(&DAT_140a16fd0);
    pcVar1 = *(char **)(param_1 + 0x38);
    if ((((*pcVar1 != '\0') && (-1 < iVar4)) && (iVar4 < *(int *)(pcVar1 + 0x28))) &&
       (*(char *)((longlong)iVar4 * 0x78 + 0x28 + *(longlong *)(pcVar1 + 0x20)) == '\x01')) {
      *(undefined4 *)(param_1 + 0x410) = 0;
    }
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    uVar5 = FUN_140294a70(&DAT_140a16fd0);
    cVar2 = FUN_1405289e0(*(undefined8 *)(param_1 + 0x40),uVar5);
    if (cVar2 == '\x01') {
      *(undefined4 *)(param_1 + 0x410) = 0;
    }
  }
  if ((*(longlong *)(param_1 + 0x28) != 0) && (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
    do {
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,iVar3);
      if (lVar6 != 0) {
        FUN_140346460(*(undefined8 *)(param_1 + 0x28),0x11,*(undefined4 *)(lVar6 + 0x4c));
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  FUN_1403639d0(param_1,param_2);
  return;
}


