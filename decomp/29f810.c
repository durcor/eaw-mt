/* FUN_14029f810  RVA 0x29f810  size 2201  params 0  ret undefined */


longlong *
FUN_14029f810(longlong param_1,longlong param_2,int param_3,undefined4 *param_4,undefined8 param_5,
             undefined1 param_6,undefined1 param_7)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  ulonglong uVar5;
  longlong lVar6;
  longlong *plVar7;
  longlong *plVar8;
  undefined8 uVar9;
  int iVar10;
  longlong *plVar11;
  uint uVar12;
  longlong *plVar13;
  undefined4 uVar14;
  longlong local_98;
  longlong *local_90;
  undefined1 local_88 [16];
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined8 local_64;
  undefined4 local_5c;
  undefined8 local_58;
  undefined4 local_50;
  undefined4 local_4c;
  
  if (param_2 == 0) {
    return (longlong *)0x0;
  }
  lVar3 = FUN_140769c58(0x8a0);
  plVar13 = (longlong *)0x0;
  plVar4 = plVar13;
  if (lVar3 != 0) {
    plVar4 = (longlong *)
             FUN_140388b60(lVar3,param_1,param_2,param_3,param_4,param_5,param_6,param_7);
  }
  plVar7 = plVar4 + 3;
  if (plVar4 == (longlong *)0x0) {
    plVar7 = plVar13;
  }
  cVar1 = FUN_14020a9b0(param_1 + 0x38,plVar7,1);
  if (cVar1 == '\x01') {
    (**(code **)*plVar4)(plVar4);
  }
  local_98 = CONCAT44(local_98._4_4_,(int)plVar4[10]);
  local_90 = plVar4;
  FUN_140241df0(param_1 + 0x80,local_88,&local_98);
  cVar1 = FUN_140370320(param_2,0x4a,0xffffffff);
  if (cVar1 != '\0') {
    FUN_14020a9b0(param_1 + 0x408,plVar7,1);
  }
  cVar1 = FUN_14020a9b0(*(longlong *)(param_1 + 200) + (longlong)param_3 * 0x48,plVar7,1);
  if (cVar1 == '\x01') {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(byte *)(plVar4 + 0x74) & 0x40) == 0) {
    cVar1 = *(char *)(plVar4[0x53] + 0x41);
  }
  else {
    cVar1 = *(char *)(plVar4[0x53] + 0x42);
  }
  if (cVar1 != '\0') {
    plVar11 = (longlong *)((longlong)(int)plVar4[0xb] * 0x38 + *(longlong *)(param_1 + 0xd0));
    (**(code **)(*plVar11 + 8))(plVar11,plVar4 + 5);
    (**(code **)(**(longlong **)(param_1 + 0xd8) + 8))(*(longlong **)(param_1 + 0xd8),plVar4 + 5);
  }
  uVar5 = FUN_140397900(plVar4,1);
  plVar11 = *(longlong **)(param_1 + 0x558);
  if ((((plVar11 != (longlong *)0x0) && (lVar3 = plVar4[0x53], *(char *)(lVar3 + 0x54) == '\0')) &&
      (*(char *)(lVar3 + 0x56) == '\0')) &&
     ((*(char *)(lVar3 + 0x57) == '\0' && ((uVar5 & 0xffffffef) != 0)))) {
    (**(code **)(*plVar11 + 8))(plVar11,plVar4 + 5);
  }
  cVar1 = FUN_14039ac80(plVar4);
  if (cVar1 != '\0') {
    (**(code **)(**(longlong **)(param_1 + 0xe0) + 8))(*(longlong **)(param_1 + 0xe0),plVar4 + 5);
  }
  cVar1 = FUN_140374da0(param_2,plVar4);
  if (cVar1 != '\0') {
    cVar1 = FUN_14020a9b0(param_1 + 0x1c8,plVar7,1);
    if (cVar1 == '\x01') {
      (**(code **)*plVar4)(plVar4);
    }
    if ((DAT_140b15490 == '\0') && (plVar4[0x20] != 0)) {
      lVar6 = plVar4[0x20] + 8;
      cVar1 = *(char *)(plVar4[0x53] + 0xe8);
      local_98 = lVar6;
      for (lVar3 = *(longlong *)(param_1 + 0x48); lVar3 != param_1 + 0x40;
          lVar3 = *(longlong *)(lVar3 + 8)) {
        plVar11 = (longlong *)(*(longlong *)(lVar3 + 0x18) + -0x18);
        if (*(longlong *)(lVar3 + 0x18) == 0) {
          plVar11 = plVar13;
        }
        if (((*(longlong *)(plVar11[0x53] + 0x1648) != 0) && (plVar11 != (longlong *)0x0)) &&
           ((plVar11 != plVar4 || (cVar1 != '\0')))) {
          FUN_14037cab0(lVar6);
        }
      }
    }
  }
  if ((0.0 < *(float *)(param_2 + 0x64c)) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x210,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(char *)(param_2 + 0x52) != '\0') &&
     (cVar1 = FUN_14020a9b0(param_1 + 600,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(char *)((longlong)plVar4 + 0x334) != -1) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x2a0,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if (*(char *)((longlong)plVar4 + 0x35e) != -1) {
    iVar2 = *(int *)(param_1 + 0x3d0);
    iVar10 = *(int *)(param_1 + 0x3d4) * 2 >> 1;
    if (iVar10 <= iVar2) {
      iVar2 = iVar10 / 10;
      if (iVar10 / 10 < 10) {
        iVar2 = 10;
      }
      FUN_140047aa0(param_1 + 0x3c0,iVar2 + iVar10,0,1);
      iVar2 = *(int *)(param_1 + 0x3d0);
    }
    *(longlong **)(*(longlong *)(param_1 + 0x3c8) + (longlong)iVar2 * 8) = plVar4;
    *(int *)(param_1 + 0x3d0) = *(int *)(param_1 + 0x3d0) + 1;
  }
  if ((((0.0 < *(float *)(param_2 + 0x1e20)) || (0.0 < *(float *)(param_2 + 0x1df8))) ||
      (0.0 < *(float *)(param_2 + 0x21f4))) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x2e8,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if (((char)plVar4[0x6d] != -1) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x330,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(char *)((longlong)plVar4 + 0x397) != -1) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x378,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(char *)((longlong)plVar4 + 0x335) != -1) &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x560,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if ((*(char *)(param_2 + 0x13) != '\0') &&
     (cVar1 = FUN_14020a9b0(param_1 + 0x5a8,plVar7,1), cVar1 == '\x01')) {
    (**(code **)*plVar4)(plVar4);
  }
  if (*(char *)((longlong)plVar4 + 0x34c) != -1) {
    *(undefined1 *)((longlong)plVar4 + 0x3cb) = 1;
  }
  if (plVar4[0x54] != 0) {
    FUN_140265280(plVar4[0x54],(float)DAT_140b0a320 / (float)DAT_140b0a340,0);
  }
  FUN_1402b38c0(param_1,plVar4);
  plVar7 = plVar13;
  if (((*(byte *)(plVar4 + 0x74) & 0x40) == 0) && ((char)plVar4[0x69] == -1)) {
    FUN_1402b3670(param_1,plVar4);
    FUN_1403a82a0(plVar4);
  }
  do {
    iVar2 = FUN_140374b50(plVar4[0x53],plVar7);
    if (iVar2 == 0x20) {
      FUN_1404cce30(plVar4);
      break;
    }
    uVar12 = (int)plVar7 + 1;
    plVar7 = (longlong *)(ulonglong)uVar12;
  } while ((int)uVar12 < 2);
  if (0.0 < *(float *)(plVar4[0x53] + 0x23e8)) {
    FUN_1404cce30(plVar4);
    if (plVar4[0x54] != 0) {
      FUN_140265ae0(plVar4[0x54],7,1);
    }
  }
  FUN_1403935b0(plVar4,*(undefined1 *)(param_1 + 0x63c),0,1);
  if ((*(longlong *)(param_2 + 0x600) != 0) && (cVar1 = FUN_14013ecf0(), cVar1 != '\0')) {
    plVar7 = (longlong *)(param_2 + 0x5f0);
    if (0xf < *(ulonglong *)(param_2 + 0x608)) {
      plVar7 = (longlong *)*plVar7;
    }
    plVar7 = (longlong *)FUN_140488e00(plVar7);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)*plVar7)(plVar7);
    }
    plVar11 = (longlong *)(**(code **)(*plVar7 + 0x28))(plVar7);
    if (plVar11 != (longlong *)0x0) {
      (**(code **)*plVar11)(plVar11);
    }
    local_6c = *param_4;
    local_5c = param_4[1];
    local_4c = param_4[2];
    local_78 = 0x3f800000;
    local_70 = 0;
    local_68 = 0;
    local_64 = 0x3f800000;
    local_58 = 0;
    local_50 = 0x3f800000;
    (**(code **)(*plVar11 + 0x58))(plVar11,&local_78);
    plVar8 = DAT_140b15418;
    if ((DAT_140b15418 != (longlong *)0x0) &&
       (lVar3 = (**(code **)(*DAT_140b15418 + 0x100))(DAT_140b15418), lVar3 != 0)) {
      plVar8 = (longlong *)(**(code **)(*plVar8 + 0x100))(plVar8);
      plVar8 = (longlong *)(**(code **)(*plVar8 + 0x78))(plVar8);
      if (plVar8 != (longlong *)0x0) {
        (**(code **)(*plVar8 + 0xa8))(plVar8,plVar11);
      }
    }
    (**(code **)(*plVar11 + 8))(plVar11);
    (**(code **)(*plVar7 + 8))(plVar7);
  }
  if (*(int *)(param_2 + 0x2138) != -1) {
    FUN_140470700(&DAT_140a29fa0,*(int *)(param_2 + 0x2138),plVar4 + 0x49);
  }
  if (((*(char *)(plVar4[0x53] + 0x3b) == '\0') &&
      (lVar3 = (**(code **)(*plVar4 + 0x10))(plVar4,8), lVar3 == 0)) &&
     (lVar3 = (**(code **)(*plVar4 + 0x10))(plVar4,0xc), lVar3 == 0)) {
    uVar9 = FUN_140058570();
    FUN_140220ed0(uVar9,param_1,0x1c,plVar4);
  }
  if (((*(longlong **)(param_1 + 0x20) != (longlong *)0x0) &&
      (cVar1 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0xf0))(), cVar1 != '\0')) &&
     ((0 < *(int *)(plVar4[0x53] + 0x2120) &&
      ((lVar3 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar4[0xb]), lVar3 != 0 &&
       (plVar7 = plVar13, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))))))) {
    do {
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,plVar7);
      if ((lVar6 != 0) && (cVar1 = FUN_1402823e0(lVar3,lVar6), cVar1 != '\0')) {
        FUN_140289010(lVar6);
      }
      uVar12 = (int)plVar7 + 1;
      plVar7 = (longlong *)(ulonglong)uVar12;
    } while ((int)uVar12 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  if ((0.0 < *(float *)(plVar4[0x53] + 0x23ec)) &&
     (lVar3 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar4[0xb]), lVar3 != 0)) {
    FUN_140282180(lVar3,1);
  }
  iVar2 = FUN_140374b50(plVar4[0x53],0);
  if ((iVar2 == 0x40) && (lVar3 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar4[0xb]), lVar3 != 0)) {
    FUN_1402820b0(lVar3,1);
  }
  FUN_1403a8ad0(plVar4,DAT_1407ffaf8);
  uVar14 = FUN_1403725f0(plVar4[0x53],plVar4);
  FUN_1403a91e0(plVar4,uVar14);
  iVar2 = FUN_140294df0(plVar4);
  if (0 < iVar2) {
    do {
      lVar3 = FUN_140396cb0(plVar4,plVar13);
      if (lVar3 != 0) {
        uVar14 = FUN_140385ae0(lVar3);
        FUN_140387ca0(lVar3,uVar14);
      }
      uVar12 = (int)plVar13 + 1;
      plVar13 = (longlong *)(ulonglong)uVar12;
      iVar2 = FUN_140294df0(plVar4);
    } while ((int)uVar12 < iVar2);
  }
  return plVar4;
}


