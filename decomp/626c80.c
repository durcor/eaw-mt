/* SimpleSpaceLocomotorBehaviorClass::FUN_140626c80  RVA 0x626c80  size 2316  params 0  ret undefined */


undefined8
SimpleSpaceLocomotorBehaviorClass::FUN_140626c80
          (longlong param_1,longlong param_2,undefined4 *param_3)

{
  byte *pbVar1;
  float fVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  char cVar6;
  int iVar7;
  undefined4 uVar8;
  longlong *plVar9;
  longlong lVar10;
  undefined8 uVar11;
  float fVar12;
  float fVar13;
  float extraout_XMM0_Da;
  double dVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  double in_stack_fffffffffffffe08;
  undefined8 local_108;
  float local_100;
  float local_f8;
  float local_f4;
  float local_f0;
  float local_e8;
  float local_e4;
  float local_e0;
  float local_d8;
  float local_d4;
  float local_d0;
  float local_cc;
  float local_c8;
  float local_c4;
  
  *param_3 = 0x650000;
  fVar4 = DAT_140b27bc0;
  fVar13 = DAT_140b27bbc;
  fVar17 = DAT_140b27bb0;
  fVar15 = DAT_140b27b7c;
  fVar25 = DAT_1407ffaf8;
  iVar7 = *(int *)(param_2 + 0x5c);
  if (iVar7 == -1) {
    local_cc = DAT_140b27bbc;
    local_d0 = DAT_140b27bc0;
    fVar22 = DAT_1408007c0;
    if (DAT_140b27bc0 - DAT_140b27bbc <= DAT_1407ffaf8) {
LAB_14062718d:
      if (fVar22 <= fVar25) {
        fVar25 = fVar22;
      }
    }
    else {
      lVar10 = *(longlong *)(param_1 + 0x28);
      lVar3 = *(longlong *)(lVar10 + 0x2b8);
      fVar22 = *(float *)(lVar10 + 0x78);
      local_c8 = DAT_140b27b7c;
      fVar20 = *(float *)(lVar10 + 0x7c);
      local_100 = DAT_140b27bb0;
      local_f4 = *(float *)(param_2 + 0x30);
      fVar24 = *(float *)(lVar10 + 0x80);
      fVar18 = (DAT_140b27b7c * *(float *)(param_2 + 0x18) + *(float *)(lVar3 + 0x62c)) *
               DAT_140b27bac +
               (DAT_140b27b7c * *(float *)(param_2 + 0x14) + *(float *)(lVar3 + 0x628)) *
               DAT_140b27ba8 +
               (DAT_140b27b7c * *(float *)(param_2 + 0x1c) + *(float *)(lVar3 + 0x630)) *
               DAT_140b27bb0;
      fVar2 = *(float *)(param_2 + 0x2c);
      fVar12 = *(float *)(param_2 + 0x34);
      local_d4 = DAT_140b27ba8;
      fVar16 = (fVar18 * DAT_140b27bb0 - fVar24) * DAT_140b27bb0;
      fVar21 = (fVar18 * DAT_140b27bac - fVar20) * DAT_140b27bac;
      fVar23 = (local_f4 - fVar20) * DAT_140b27bac + (fVar2 - fVar22) * DAT_140b27ba8 +
               (fVar12 - fVar24) * DAT_140b27bb0;
      fVar19 = (fVar18 * DAT_140b27ba8 - fVar22) * DAT_140b27ba8;
      local_d8 = DAT_140b27bac;
      local_108 = CONCAT44(DAT_140b27bac,DAT_140b27ba8);
      local_f8 = fVar2;
      local_f0 = fVar12;
      dVar14 = (double)FUN_1407760bc((double)fVar23);
      fVar5 = local_f0;
      fVar18 = local_f4;
      if ((float)dVar14 < DAT_140815de4) {
        fVar22 = 0.0;
        fVar20 = 0.0;
        fVar24 = 0.0;
      }
      else {
        fVar23 = (fVar21 + fVar19 + fVar16) / fVar23;
        fVar24 = (fVar12 - fVar24) * fVar23 + fVar24;
        fVar20 = (local_f4 - fVar20) * fVar23 + fVar20;
        fVar22 = fVar22 + fVar23 * (fVar2 - fVar22);
      }
      fVar12 = (float)FUN_140776d48((fVar2 - fVar22) * (fVar2 - fVar22) +
                                    (local_f4 - fVar20) * (local_f4 - fVar20) +
                                    (local_f0 - fVar24) * (local_f0 - fVar24));
      local_c4 = fVar25 - (fVar12 - fVar13) / (fVar4 - fVar13);
      in_stack_fffffffffffffe08 = (double)fVar24;
      LocomotorCommonClass::FUN_14027d160
                (0x10,
                 "CLD: %f, %f, %f (%08X, %08X, %08X)\nCV: %f, %f, %f (%08X, %08X, %08X)\nCDD: %f, %f, %f (%08X, %08X, %08X)\nHEP: %f, %f, %f (%08X, %08X, %08X)\nIHyperD: %f (%08X)\nMinD: %f (%08X)\nMaxD: %f (%08X)\n"
                 ,(double)fVar22,(double)fVar20,in_stack_fffffffffffffe08,fVar22,fVar20,fVar24,
                 (double)*(float *)(param_2 + 0x14),(double)*(float *)(param_2 + 0x18),
                 (double)*(float *)(param_2 + 0x1c),*(undefined4 *)(param_2 + 0x14),
                 *(undefined4 *)(param_2 + 0x18),*(undefined4 *)(param_2 + 0x1c),(double)local_d4,
                 (double)local_d8,(double)fVar17,(undefined4)local_108,local_108._4_4_,local_100,
                 (double)fVar2,(double)fVar18,(double)fVar5,local_f8,local_f4,local_f0,
                 (double)fVar15,local_c8,(double)fVar13,local_cc,(double)fVar4,local_d0);
      fVar22 = local_c4;
      if (0.0 <= local_c4) goto LAB_14062718d;
      fVar25 = 0.0;
    }
    fVar25 = fVar25 * DAT_1408007e0;
    iVar7 = FUN_1401ffb40(&DAT_140a13e24,1,4);
    iVar7 = iVar7 + (int)fVar25 * 5;
    if (iVar7 < 0) {
      iVar7 = 0;
    }
    else if (0x18 < iVar7) {
      iVar7 = 0x18;
    }
    *(int *)(param_2 + 0x5c) = iVar7;
  }
  if (DAT_140b27d5f != '\0') {
    if (DAT_140b1574e != '\0') {
      return 1;
    }
    FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),1);
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
    *pbVar1 = *pbVar1 | 1;
    return 1;
  }
  iVar7 = iVar7 + 1;
  if (0x95 < iVar7) {
    if (*(char *)(param_2 + 0x260) != '\x01') {
      return 1;
    }
    FUN_140624dc0(param_1,param_2);
    return 1;
  }
  *(int *)(param_2 + 0x5c) = iVar7;
  if (iVar7 < 0x19) {
    if (DAT_140b1574e == '\0') {
      FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),1);
      pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
      *pbVar1 = *pbVar1 | 1;
    }
    plVar9 = (longlong *)
             (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                       (*(longlong **)(param_1 + 0x28),0x18);
    if (plVar9 == (longlong *)0x0) {
      return 1;
    }
    cVar6 = FUN_1404c36f0(plVar9);
    if (cVar6 != '\0') {
      return 1;
    }
    (**(code **)(*plVar9 + 0xc0))(plVar9);
    return 1;
  }
  if (((*(char *)(param_2 + 0x2b2) != '\0') && (iVar7 < 0x23)) && (DAT_140b1574e == '\0')) {
    FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),1);
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
    *pbVar1 = *pbVar1 | 1;
  }
  if (iVar7 != 0x23) {
    if ((iVar7 == 0x78) &&
       (plVar9 = (longlong *)
                 (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                           (*(longlong **)(param_1 + 0x28),0x18), plVar9 != (longlong *)0x0)) {
      (**(code **)(*plVar9 + 0xb8))(plVar9);
    }
    goto LAB_140627444;
  }
  lVar10 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58));
  lVar10 = *(longlong *)(*(longlong *)(lVar10 + 0x68) + 0x6b0);
  if (lVar10 != 0) {
    FUN_1402d5320(&DAT_140b27e60,lVar10,*(undefined8 *)(param_1 + 0x28),0,
                  (ulonglong)in_stack_fffffffffffffe08 & 0xffffffffffffff00);
  }
  uVar11 = FUN_140294a40(&DAT_140a16fd0);
  cVar6 = FUN_14039a2c0(*(undefined8 *)(param_1 + 0x28),uVar11,0);
  if ((cVar6 == '\x01') || (lVar10 = *(longlong *)(param_1 + 0x28), *(char *)(lVar10 + 0x34b) == -1)
     ) {
LAB_140627372:
    FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),0);
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
    *pbVar1 = *pbVar1 & 0xfe;
  }
  else {
    uVar8 = FUN_140294a70(&DAT_140a16fd0);
    cVar6 = FUN_14035f470(DAT_140b15418,uVar8,lVar10,0);
    if ((cVar6 == '\0') || (DAT_140a284c4 == '\0')) goto LAB_140627372;
  }
  if (*(char *)(param_2 + 0x2b2) != '\0') {
    FUN_1403a8bd0(*(undefined8 *)(param_1 + 0x28),6);
    local_108 = 0;
    local_100 = 0.0;
    FUN_1403a8b70(*(undefined8 *)(param_1 + 0x28),&local_108);
    FUN_1403a8ba0(*(undefined8 *)(param_1 + 0x28),DAT_1408795e0 / (float)(DAT_140b0a340 * 4));
    FUN_14039f2a0(*(undefined8 *)(param_1 + 0x28));
    uVar11 = (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                       (*(longlong **)(param_1 + 0x28),0x4b);
    FUN_1403a54c0(*(undefined8 *)(param_1 + 0x28),uVar11);
  }
LAB_140627444:
  fVar25 = *(float *)(param_2 + 0x14);
  fVar15 = *(float *)(param_2 + 0x18);
  fVar17 = *(float *)(param_2 + 0x1c);
  fVar13 = (float)FUN_14049d400(&DAT_140b27a10,iVar7);
  lVar10 = *(longlong *)(param_1 + 0x28);
  local_e8 = *(float *)(lVar10 + 0x78) + fVar25 * fVar13;
  local_e4 = *(float *)(lVar10 + 0x7c) + fVar15 * fVar13;
  local_e0 = *(float *)(lVar10 + 0x80) + fVar17 * fVar13;
  FUN_1403a8f90(lVar10,&local_e8,0);
  cVar6 = SceneClass::FUN_140264cb0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0));
  if (((cVar6 != '\0') && (*(char *)(param_2 + 0x2b2) == '\0')) &&
     (fVar25 = local_e8 - *(float *)(param_2 + 0x2c), fVar15 = local_e4 - *(float *)(param_2 + 0x30)
     , fVar17 = local_e0 - *(float *)(param_2 + 0x34),
     FUN_140776d48(fVar25 * fVar25 + fVar15 * fVar15 + fVar17 * fVar17),
     extraout_XMM0_Da < DAT_140867698)) {
    FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),0);
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
    *pbVar1 = *pbVar1 & 0xfe;
    FUN_1403a8bd0(*(undefined8 *)(param_1 + 0x28),0);
  }
  return 1;
}


