/* SimpleSpaceLocomotorBehaviorClass::vfunc_6  RVA 0x626420  size 1484  params 0  ret undefined */


undefined4
SimpleSpaceLocomotorBehaviorClass::vfunc_6(longlong *param_1,longlong param_2,int param_3)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  bool bVar8;
  float fVar9;
  float fVar10;
  undefined4 uVar11;
  undefined4 local_res8 [2];
  undefined8 local_a8;
  float local_a0;
  undefined8 local_98;
  float local_90;
  undefined8 local_88;
  float local_80;
  undefined8 local_78;
  float local_70;
  undefined1 local_68 [48];
  
  LocomotorCommonClass::vfunc_6();
  cVar4 = (**(code **)(*param_1 + 0x210))(param_1);
  if (cVar4 != '\0') {
    local_res8[0] = 0x650000;
    if ((param_1[5] == 0) || (lVar2 = *(longlong *)(param_1[5] + 0xa8), lVar2 == 0)) {
      return 0x80650001;
    }
    iVar1 = *(int *)(lVar2 + 0x48);
    if (iVar1 == 4) {
      FUN_140626c80(param_1,lVar2,local_res8);
    }
    else {
      if (iVar1 == 5) {
        FUN_140627590(param_1,lVar2,local_res8);
        return local_res8[0];
      }
      if (iVar1 == 0x28) {
        FUN_1405cd680(param_1,lVar2,local_res8);
        return local_res8[0];
      }
      if (iVar1 == 0x2c) {
        FUN_1406269f0(param_1,lVar2,local_res8);
        return local_res8[0];
      }
    }
    return local_res8[0];
  }
  lVar2 = *(longlong *)(param_1[5] + 0xa8);
  if (lVar2 == 0) {
    return 0x80650001;
  }
  if ((0x16 < *(uint *)(lVar2 + 0x48)) || ((0x480001U >> (*(uint *)(lVar2 + 0x48) & 0x1f) & 1) == 0)
     ) {
    return 0x80650002;
  }
  if (*(longlong *)(param_1[5] + 0x298) == 0) {
    return 0x80650001;
  }
  do {
    while (iVar1 = *(int *)(lVar2 + 0x48), iVar1 == 0) {
      FUN_140559250(*(undefined8 *)(param_1[5] + 0xa8),0x13);
    }
    if (iVar1 == 0x13) {
      lVar5 = *(longlong *)(param_1[5] + 0xa8);
      if (lVar5 == 0) {
        return 0x80650001;
      }
      if (*(longlong *)(param_1[5] + 0x298) == 0) {
        return 0x80650001;
      }
      lVar6 = FUN_1405569b0(lVar5);
      if (lVar6 != 0) {
        *(undefined1 *)(lVar6 + 0x6d) = 1;
        lVar3 = param_1[5];
        local_88 = *(undefined8 *)(lVar3 + 0x78);
        local_a8 = *(undefined8 *)(lVar3 + 0x78);
        local_a0 = *(float *)(lVar3 + 0x80);
        local_78 = *(undefined8 *)(lVar3 + 0x84);
        local_98 = *(undefined8 *)(lVar3 + 0x84);
        local_90 = *(float *)(lVar3 + 0x8c);
        lVar3 = *(longlong *)(lVar3 + 0xa8);
        local_80 = local_a0;
        local_70 = local_90;
        uVar7 = FUN_1405569b0(lVar3);
        local_res8[0] = *(undefined4 *)(lVar3 + 0xec);
        (**(code **)(*param_1 + 0x1d8))
                  (param_1,uVar7,DAT_140b0a320,&local_a8,&local_98,local_res8[0],&local_88,&local_78
                   ,local_res8);
        FUN_1403a8710(param_1[5],&local_78);
        FUN_1403a8f90(param_1[5],&local_88,0);
        uVar7 = (**(code **)(*param_1 + 0x1e8))(param_1,local_68);
        FUN_1403a84f0(param_1[5],uVar7);
        *(undefined4 *)(lVar3 + 0xec) = local_res8[0];
        if ((float)local_a8 == (float)local_88) {
          if ((local_a8._4_4_ == local_88._4_4_) && (local_a0 == local_80)) {
            if ((float)local_98 == (float)local_78) {
              if ((local_98._4_4_ == local_78._4_4_) && (local_90 == local_70)) {
                FUN_140559250(lVar5,0x16);
              }
            }
          }
        }
        cVar4 = FUN_1405c5910(lVar6);
        if (cVar4 == '\0') goto LAB_140626824;
        FUN_140557b10(lVar5,param_1[5]);
      }
      FUN_140559250(lVar5,0x16);
      goto LAB_140626824;
    }
  } while (iVar1 != 0x16);
  lVar5 = param_1[5];
  fVar10 = *(float *)(lVar5 + 0x84);
  local_78 = *(undefined8 *)(lVar5 + 0x84);
  local_70 = *(float *)(lVar5 + 0x8c);
  if (fVar10 != 0.0) {
    fVar9 = (float)FUN_1403724d0(*(undefined8 *)(lVar5 + 0x298));
    if (0.0 <= fVar10) {
      if ((0.0 < fVar10) && (fVar10 = fVar10 - fVar9, fVar10 <= 0.0)) {
        fVar10 = 0.0;
      }
    }
    else {
      fVar10 = fVar9 + fVar10;
      if (0.0 <= fVar10) {
        fVar10 = 0.0;
      }
    }
    local_78 = CONCAT44(local_78._4_4_,fVar10);
    FUN_1403a8710(param_1[5],&local_78);
    lVar5 = param_1[5];
  }
  if (*(longlong *)(lVar5 + 0xa8) != 0) {
    *(undefined4 *)(*(longlong *)(lVar5 + 0xa8) + 0xec) = 0;
  }
LAB_140626824:
  lVar5 = *(longlong *)(param_2 + 0x2a0);
  if (lVar5 == 0) goto LAB_14062696e;
  cVar4 = FUN_14039b950(param_2,9,1);
  bVar8 = cVar4 != '\0';
  if (*(char *)(lVar2 + 0xd0) == '\0') {
    if (0 < *(int *)(lVar2 + 0xd4)) {
      local_70 = (float)(uint)(*(int *)(lVar2 + 0xd4) - param_3) * DAT_1408007d4;
      local_78 = CONCAT44(local_70,local_70);
      fVar10 = (float)FUN_1407d8310(&local_78);
      fVar10 = (fVar10 * DAT_1408007c0 + DAT_1408007c0) * DAT_1408007b8;
      uVar11 = DAT_14080350c;
      goto LAB_14062693d;
    }
    cVar4 = FUN_140264890(lVar5,bVar8);
    if (cVar4 != '\0') {
      FUN_140265ae0(lVar5,bVar8,0);
    }
  }
  else {
    cVar4 = FUN_140264890(lVar5,bVar8);
    if (cVar4 == '\0') {
      FUN_140265ae0(lVar5,bVar8,1);
    }
    fVar9 = (float)FUN_140370f00(*(undefined8 *)(param_1[5] + 0x298),param_1[5]);
    fVar10 = 0.0;
    if (((0.0 < fVar9) &&
        (fVar9 = (*(float *)(lVar2 + 0xec) * DAT_14080090c) / fVar9 + DAT_1408007b8, 0.0 <= fVar9))
       && (fVar10 = DAT_1407ffaf8, fVar9 <= DAT_1407ffaf8)) {
      fVar10 = fVar9;
    }
    uVar11 = 0;
LAB_14062693d:
    FUN_140265a60(lVar5,bVar8,fVar10,uVar11);
  }
  if (param_3 + 1 == *(int *)(lVar2 + 0xd4)) {
    FUN_140265a60(lVar5,bVar8,DAT_1407ffaf8,DAT_1408007b4);
  }
LAB_14062696e:
  fVar10 = *(float *)(lVar2 + 0xec);
  fVar9 = DAT_140b16d54 * DAT_140819b38;
  FUN_1403a7e30(param_2,fVar9 <= fVar10);
  FUN_1403a7cf0(param_2,fVar10 < fVar9);
  return 0x650000;
}


