/* DynamicTransformBehaviorClass::FUN_1403ac530  RVA 0x3ac530  size 812  params 0  ret undefined */


void DynamicTransformBehaviorClass::FUN_1403ac530(longlong *param_1,undefined1 param_2)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined8 *puVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  undefined8 uVar9;
  longlong *plVar10;
  float fVar11;
  float fVar12;
  undefined4 uVar13;
  float fVar14;
  
  fVar7 = DAT_1408007dc;
  fVar6 = DAT_1408007d4;
  pfVar1 = (float *)(param_1 + 0x49);
  pfVar1[0] = 1.0;
  pfVar1[1] = 0.0;
  *(undefined8 *)((longlong)param_1 + 0x25c) = 0x3f800000;
  param_1[0x4e] = 0x3f800000;
  fVar8 = DAT_1408007f4;
  param_1[0x4a] = 0;
  *(undefined4 *)(param_1 + 0x4b) = 0;
  *(undefined8 *)((longlong)param_1 + 0x264) = 0;
  *(undefined4 *)((longlong)param_1 + 0x26c) = 0;
  fVar2 = *(float *)(param_1 + 0x13);
  fVar3 = *(float *)(param_1 + 0x12);
  fVar14 = *(float *)((longlong)param_1 + 0x94);
  *(int *)((longlong)param_1 + 0x254) = (int)param_1[0xf];
  *(undefined4 *)((longlong)param_1 + 0x264) = *(undefined4 *)((longlong)param_1 + 0x7c);
  *(int *)((longlong)param_1 + 0x274) = (int)param_1[0x10];
  FUN_1400480f0(pfVar1,((fVar7 * fVar6) / fVar8) * fVar2);
  fVar14 = ((fVar7 * fVar6) / fVar8) * fVar14;
  fVar11 = fVar14;
  if (0x1d000000 < ((uint)fVar14 & 0x7f800000)) {
    fVar11 = (float)FUN_140776650(fVar14);
  }
  fVar12 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)fVar14 & 0x7f800000)) {
    fVar12 = (float)FUN_140776150(fVar14);
  }
  fVar14 = *(float *)(param_1 + 0x4a);
  *(float *)(param_1 + 0x4a) = fVar14 * fVar12 + *pfVar1 * fVar11;
  fVar4 = *(float *)(param_1 + 0x4c);
  *pfVar1 = *pfVar1 * fVar12 - fVar14 * fVar11;
  fVar14 = *(float *)(param_1 + 0x4d);
  *(float *)(param_1 + 0x4c) = fVar4 * fVar12 + *(float *)(param_1 + 0x4b) * fVar11;
  *(float *)(param_1 + 0x4b) = *(float *)(param_1 + 0x4b) * fVar12 - fVar4 * fVar11;
  *(float *)(param_1 + 0x4d) = fVar14 * fVar12 - *(float *)(param_1 + 0x4e) * fVar11;
  *(float *)(param_1 + 0x4e) = *(float *)(param_1 + 0x4e) * fVar12 + fVar14 * fVar11;
  FUN_1402cf8d0(pfVar1,((fVar7 * fVar6) / fVar8) * fVar3);
  FUN_1400480f0(pfVar1,((fVar7 * fVar6) / fVar8) * DAT_1408007ec);
  plVar10 = (longlong *)param_1[0x54];
  if (plVar10 != (longlong *)0x0) {
    if (param_1[0x5c] == 0) {
      (**(code **)(*plVar10 + 0x28))(plVar10,pfVar1);
      plVar10 = (longlong *)param_1[0x54];
    }
    uVar13 = *(undefined4 *)(param_1[0x53] + 0x658);
    if (param_1[0x48] != 0) {
      uVar13 = *(undefined4 *)(param_1[0x48] + 0x40);
    }
    FUN_140265d80(plVar10,uVar13);
    FUN_140266340(param_1[0x54],param_1 + 0xf,((fVar7 * fVar6) / fVar8) * fVar2,DAT_140b0a320,
                  param_2);
    if (*(char *)((longlong)param_1 + 0x335) != -1) {
      uVar9 = (**(code **)(*param_1 + 0x10))(param_1,3);
      _guard_check_icall(uVar9,param_1);
    }
    uVar9 = FUN_1403982b0(param_1);
    for (puVar5 = (undefined8 *)param_1[6]; puVar5 != (undefined8 *)0x0;
        puVar5 = (undefined8 *)puVar5[2]) {
      (**(code **)(*(longlong *)*puVar5 + 0x20))((longlong *)*puVar5,puVar5,uVar9);
    }
  }
  *(byte *)((longlong)param_1 + 0x3a1) = *(byte *)((longlong)param_1 + 0x3a1) & 0xfe;
  return;
}


