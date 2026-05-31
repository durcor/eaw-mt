/* FighterLocomotorBehaviorClass::FUN_1405ce010  RVA 0x5ce010  size 1244  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405ce010(longlong *param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  undefined4 *puVar10;
  longlong *plVar11;
  uint uVar12;
  ulonglong uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 local_res8 [2];
  longlong *local_res10;
  float local_a8;
  float local_a4;
  float local_a0;
  undefined1 local_9c [12];
  float local_90;
  float local_8c;
  float local_88;
  float local_84;
  
  uVar13 = 0;
  lVar9 = param_1[5];
  plVar11 = *(longlong **)(lVar9 + 0x2b0);
  lVar2 = *(longlong *)(lVar9 + 0xa8);
  uVar5 = uVar13;
  uVar6 = uVar13;
  local_res10 = plVar11;
  if (plVar11 != (longlong *)0x0) {
    uVar5 = (**(code **)(*plVar11 + 0x10))(plVar11,0x16);
    uVar6 = FUN_1404ec820(plVar11[0x22]);
    lVar9 = param_1[5];
  }
  lVar9 = *(longlong *)(lVar9 + 0xf0);
  if (lVar9 != 0) {
    lVar7 = FUN_1403973b0();
    local_res8[0] = *(undefined4 *)(lVar7 + 0xde0);
    uVar8 = FUN_1404ad4c0(&local_90,param_1[5],0xffffffffffffffff);
    FUN_1403fe810(*(undefined8 *)(lVar9 + 0x138),local_res8,&DAT_1408a618c,uVar8,&PTR_1407ff8a8,
                  DAT_140891814);
    FUN_1404ad530(&local_90);
  }
  lVar9 = FUN_1405569b0(lVar2);
  if (lVar9 == 0) {
    lVar9 = param_1[5];
    local_a0 = *(float *)(lVar2 + 0x1c) + *(float *)(lVar9 + 0x80);
    local_a4 = *(float *)(lVar2 + 0x18) + *(float *)(lVar9 + 0x7c);
    local_a8 = *(float *)(lVar2 + 0x14) + *(float *)(lVar9 + 0x78);
    uVar8 = FUN_1405c9090(param_1,local_9c,&local_a8);
    FUN_1403a8f90(param_1[5],uVar8,0);
    return;
  }
  if (*(char *)(lVar9 + 0x6d) == '\0') {
    *(undefined1 *)(lVar9 + 0x6d) = 1;
  }
  if (uVar6 == param_1[5]) {
    FUN_1405c4ec0(lVar9,&local_90,plVar11);
    if ((DAT_140869970 <=
         (double)(local_84 * (*(float *)(param_1[5] + 0x7c) - local_8c) +
                 local_88 * (*(float *)(param_1[5] + 0x78) - local_90))) &&
       (iVar4 = FUN_140405300(uVar5), uVar6 = uVar13, plVar11 = local_res10, 0 < iVar4)) {
      do {
        lVar7 = FUN_1404052d0(uVar5,uVar6);
        uVar8 = *(undefined8 *)(lVar7 + 0xa8);
        lVar7 = FUN_1405569b0(uVar8);
        if (lVar7 != 0) {
          uVar8 = FUN_1405569b0(uVar8);
          FUN_1405c4920(uVar8);
        }
        uVar12 = (int)uVar6 + 1;
        uVar6 = (ulonglong)uVar12;
        iVar4 = FUN_140405300(uVar5);
        plVar11 = local_res10;
      } while ((int)uVar12 < iVar4);
    }
    cVar3 = FUN_1405c5910(lVar9);
    if (cVar3 == '\0') goto LAB_1405ce41f;
    if (*(int *)(lVar2 + 0x48) != 0x1e) {
      puVar10 = (undefined4 *)
                FUN_14054fe50((longlong *)(lVar9 + 0x18),
                              (int)((*(longlong *)(lVar9 + 0x20) - *(longlong *)(lVar9 + 0x18)) /
                                   0x34) + -1);
      uVar14 = *puVar10;
      uVar15 = puVar10[1];
      uVar1 = *(undefined4 *)(*(longlong *)(param_1[5] + 0x298) + 0xdfc);
      iVar4 = FUN_140405300(uVar5);
      if (0 < iVar4) {
        do {
          plVar11 = (longlong *)FUN_1404052d0(uVar5,uVar13);
          lVar9 = plVar11[0x15];
          *(undefined4 *)(lVar9 + 8) = uVar14;
          *(undefined4 *)(lVar9 + 0xc) = uVar15;
          *(undefined4 *)(lVar9 + 0x10) = uVar1;
          *(undefined4 *)(lVar9 + 0x38) = *(undefined4 *)(lVar2 + 0x38);
          *(undefined4 *)(lVar9 + 0x3c) = *(undefined4 *)(lVar2 + 0x3c);
          *(undefined4 *)(lVar9 + 0x40) = *(undefined4 *)(lVar2 + 0x40);
          lVar7 = FUN_1405569b0(lVar9);
          if (lVar7 != 0) {
            uVar8 = FUN_1405569b0(lVar9);
            FUN_1405c5dc0(uVar8);
            uVar8 = (**(code **)(*plVar11 + 0x10))(plVar11,1);
            FUN_1405cb750(uVar8);
          }
          uVar12 = (int)uVar13 + 1;
          uVar13 = (ulonglong)uVar12;
          iVar4 = FUN_140405300(uVar5);
        } while ((int)uVar12 < iVar4);
      }
    }
    lVar9 = param_1[5];
    local_a8 = *(float *)(lVar2 + 0x14) + *(float *)(lVar9 + 0x78);
  }
  else {
    if (uVar5 == 0) {
      FUN_1405c4ec0(lVar9,&local_90);
      if (DAT_140869970 <=
          (double)(local_84 * (*(float *)(param_1[5] + 0x7c) - local_8c) +
                  local_88 * (*(float *)(param_1[5] + 0x78) - local_90))) {
        FUN_1405c4920(lVar9);
      }
      cVar3 = FUN_1405c5910(lVar9);
      if (cVar3 != '\0') {
        puVar10 = (undefined4 *)
                  FUN_14054fe50((longlong *)(lVar9 + 0x18),
                                (int)((*(longlong *)(lVar9 + 0x20) - *(longlong *)(lVar9 + 0x18)) /
                                     0x34) + -1);
        uVar14 = *puVar10;
        uVar15 = puVar10[1];
        *(undefined4 *)(lVar2 + 0x10) = *(undefined4 *)(*(longlong *)(param_1[5] + 0x298) + 0xdfc);
        *(undefined4 *)(lVar2 + 8) = uVar14;
        *(undefined4 *)(lVar2 + 0xc) = uVar15;
        FUN_1405cb750(param_1);
        lVar9 = param_1[5];
        local_a0 = *(float *)(lVar9 + 0x80) + *(float *)(lVar2 + 0x1c);
        local_a4 = *(float *)(lVar9 + 0x7c) + *(float *)(lVar2 + 0x18);
        local_a8 = *(float *)(lVar2 + 0x14) + *(float *)(lVar9 + 0x78);
        goto LAB_1405ce49b;
      }
    }
LAB_1405ce41f:
    uVar8 = FUN_1405ca390(param_1);
    uVar14 = (undefined4)uVar8;
    uVar15 = (undefined4)((ulonglong)uVar8 >> 0x20);
    cVar3 = (**(code **)(*param_1 + 0x1c8))(param_1);
    if (cVar3 != '\0') {
      if (plVar11 == (longlong *)0x0) {
        uVar14 = *(undefined4 *)(lVar2 + 0x158);
        uVar15 = 0;
      }
      else {
        uVar14 = *(undefined4 *)(plVar11[0x15] + 0x158);
        uVar15 = 0;
      }
    }
    FUN_1405c9e90(param_1,CONCAT44(uVar15,uVar14),
                  *(undefined4 *)(*(longlong *)(param_1[5] + 0x298) + 0xdfc));
    lVar9 = param_1[5];
    local_a8 = *(float *)(lVar9 + 0x78) + *(float *)(lVar2 + 0x14);
  }
  local_a4 = *(float *)(lVar2 + 0x18) + *(float *)(lVar9 + 0x7c);
  local_a0 = *(float *)(lVar2 + 0x1c) + *(float *)(lVar9 + 0x80);
LAB_1405ce49b:
  uVar8 = FUN_1405c9090(param_1,local_9c,&local_a8);
  FUN_1403a8f90(param_1[5],uVar8,0);
  return;
}


