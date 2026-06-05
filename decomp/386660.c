/* FUN_140386660  RVA 0x386660  size 1057  params 0  ret undefined */


void FUN_140386660(longlong param_1,int param_2,char param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 *puVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  float fVar13;
  ulonglong in_stack_ffffffffffffff68;
  undefined8 local_78;
  undefined4 local_70;
  undefined8 local_68;
  undefined4 local_60;
  undefined1 local_58 [12];
  undefined4 local_4c;
  undefined4 local_3c;
  undefined4 local_2c;
  
  lVar5 = *(longlong *)(param_1 + 0x10);
  if (lVar5 == 0) {
    return;
  }
  if (*(int *)(*(longlong *)(param_1 + 0x20) + 0x48) == 0xc) {
    return;
  }
  if (*(int *)(lVar5 + 0x50) == DAT_140a286f0) {
    *(byte *)(lVar5 + 0x3a0) = *(byte *)(lVar5 + 0x3a0) | 1;
  }
  if (0.0 < *(float *)(param_1 + 0x28)) {
    return;
  }
  iVar12 = 0;
  *(undefined4 *)(param_1 + 0x28) = 0;
  if ((param_2 == -1) || (param_2 == 6)) goto LAB_1403867a1;
  lVar5 = *(longlong *)(param_1 + 0x10);
  iVar2 = FUN_140294a70(&DAT_140a16fd0);
  if ((*(int *)(lVar5 + 0x58) != iVar2) || (fVar13 = (float)FUN_140396df0(lVar5), fVar13 <= 0.0))
  goto LAB_1403867a1;
  iVar2 = *(int *)(*(longlong *)(param_1 + 0x20) + 0x48);
  if (iVar2 - 5U < 6) {
    iVar10 = 5;
    iVar11 = iVar12;
    do {
      iVar3 = FUN_140397c10(*(undefined8 *)(param_1 + 0x10),iVar10);
      iVar11 = iVar11 + iVar3;
      iVar10 = iVar10 + 1;
    } while (iVar10 < 0xb);
    if ((iVar11 != 0) ||
       (lVar5 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298) + 0x1070),
       lVar5 == 0)) goto LAB_140386760;
  }
  else {
LAB_140386760:
    iVar11 = FUN_140397c10(*(undefined8 *)(param_1 + 0x10),iVar2);
    if ((iVar11 != 0) ||
       (lVar5 = FUN_140373aa0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),iVar2),
       lVar5 == 0)) goto LAB_1403867a1;
  }
  in_stack_ffffffffffffff68 = in_stack_ffffffffffffff68 & 0xffffffff00000000;
  FUN_1402d5290(&DAT_140b27e60,lVar5,0,0,in_stack_ffffffffffffff68);
LAB_1403867a1:
  FUN_140384d00(param_1,0,1);
  uVar6 = FUN_140331cc0(&DAT_140a172d0);
  lVar5 = FUN_140294d30(&DAT_140a16fd0,uVar6);
  iVar2 = *(int *)(param_1 + 0x2c);
  cVar1 = '\0';
  if ((-1 < iVar2) || (iVar2 = *(int *)(param_1 + 0x38), -1 < iVar2)) {
    uVar6 = FUN_1402648b0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2a0));
    cVar1 = FUN_14012d2c0(uVar6,iVar2,local_58);
  }
  local_78 = 0;
  local_70 = 0;
  if (cVar1 == '\x01') {
    local_70 = local_2c;
    local_78 = CONCAT44(local_3c,local_4c);
  }
  lVar7 = *(longlong *)(param_1 + 0x20);
  lVar8 = *(longlong *)(lVar7 + 0x430);
  if (lVar8 != 0) {
    in_stack_ffffffffffffff68 = *(longlong *)(param_1 + 0x10) + 0x84;
    lVar7 = FUN_140294aa0(&DAT_140a16fd0);
    FUN_14029f810(*(undefined8 *)(DAT_140b15418 + 0x18),lVar8,*(undefined4 *)(lVar7 + 0x4c),
                  &local_78,in_stack_ffffffffffffff68,1,0);
    local_68 = local_78;
    local_60 = local_70;
    FUN_1402cdc00(&DAT_140b27a10,&local_68,lVar8,0);
    lVar7 = *(longlong *)(param_1 + 0x20);
  }
  lVar8 = *(longlong *)(lVar7 + 0x248);
  if ((lVar8 != 0) ||
     ((*(longlong *)(lVar7 + 0x440) != 0 &&
      (lVar8 = *(longlong *)(*(longlong *)(lVar7 + 0x440) + 0x12d0), lVar8 != 0)))) {
    FUN_1402d5240(&DAT_140b27e60,lVar8,&local_78,*(undefined8 *)(param_1 + 0x10),
                  in_stack_ffffffffffffff68 & 0xffffffff00000000,0);
  }
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x440);
  if ((lVar7 != 0) && (param_3 == '\0')) {
    lVar8 = *(longlong *)(param_1 + 0x10);
    uVar6 = FUN_140385c70(param_1,&local_68);
    uVar6 = FUN_14029f810(*(undefined8 *)(DAT_140b15418 + 0x18),lVar7,*(undefined4 *)(lVar5 + 0x4c),
                          uVar6,lVar8 + 0x84,1,0);
    FUN_14029bfe0(*(undefined8 *)(DAT_140b15418 + 0x18),uVar6);
  }
  FUN_1403a5700(*(undefined8 *)(param_1 + 0x10),*(undefined4 *)(param_1 + 0x18));
  uVar6 = FUN_1402648b0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2a0));
  lVar5 = *(longlong *)(param_1 + 0x20);
  if (*(longlong *)(lVar5 + 0xd0) != 0) {
    puVar9 = (undefined8 *)(lVar5 + 0xc0);
    if (0xf < *(ulonglong *)(lVar5 + 0xd8)) {
      puVar9 = (undefined8 *)*puVar9;
    }
    uVar4 = FUN_14012d520(uVar6,puVar9);
    lVar5 = FUN_14012d6f0(uVar6,uVar4);
    if (lVar5 != 0) {
      *(byte *)(lVar5 + 0x30) = *(byte *)(lVar5 + 0x30) & 0xfe;
    }
  }
  if ((-1 < *(int *)(param_1 + 0x30)) && (iVar11 = FUN_14012d6d0(uVar6), iVar2 = iVar12, 0 < iVar11)
     ) {
    do {
      cVar1 = FUN_14012dae0(uVar6,iVar2,*(undefined4 *)(param_1 + 0x30));
      if (cVar1 == '\x01') {
        lVar5 = FUN_14012d6f0(uVar6,iVar2);
        *(byte *)(lVar5 + 0x30) = *(byte *)(lVar5 + 0x30) & 0xfe;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < iVar11);
  }
  if ((((*(longlong *)(param_1 + 0x20) != 0) &&
       (*(char *)(*(longlong *)(param_1 + 0x20) + 0x3d7) == '\x01')) &&
      (-1 < *(int *)(param_1 + 0x34))) && (iVar2 = FUN_14012d6d0(uVar6), 0 < iVar2)) {
    do {
      cVar1 = FUN_14012dae0(uVar6,iVar12,*(undefined4 *)(param_1 + 0x34));
      if (cVar1 == '\x01') {
        lVar5 = FUN_14012d6f0(uVar6,iVar12);
        *(byte *)(lVar5 + 0x30) = *(byte *)(lVar5 + 0x30) | 1;
      }
      iVar12 = iVar12 + 1;
    } while (iVar12 < iVar2);
  }
  return;
}


