/* RevealBehaviorClass::FUN_14035d1b0  RVA 0x35d1b0  size 826  params 0  ret undefined */


void RevealBehaviorClass::FUN_14035d1b0
               (longlong *param_1,int param_2,longlong param_3,undefined8 param_4,char param_5)

{
  longlong *plVar1;
  undefined8 uVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  undefined **local_res18;
  float local_1d8;
  float local_1d4;
  float local_1d0;
  undefined **local_1c8;
  float local_1c0;
  float local_1bc;
  float local_1b8;
  float local_1b4;
  float local_1b0;
  undefined1 local_1a8 [224];
  undefined1 local_c8 [176];
  
  if (param_3 == 0) {
    return;
  }
  cVar3 = (**(code **)(*param_1 + 0x240))();
  fVar9 = DAT_140805898;
  fVar8 = DAT_1407ffaf8;
  if (cVar3 == '\0') {
    return;
  }
  if ((param_5 == '\0') && (cVar3 = FUN_14039a230(param_3,param_2,0), cVar3 != '\x01')) {
    plVar1 = *(longlong **)(param_3 + 0x2a0);
    fVar7 = fVar9;
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x58))(plVar1,&local_1c0);
      local_1d0 = local_1b8 + *(float *)(param_3 + 0x80);
      local_1d4 = local_1bc + *(float *)(param_3 + 0x7c);
      local_1d8 = local_1c0 + *(float *)(param_3 + 0x78);
      fVar7 = local_1b4;
      fVar9 = local_1b4;
      if (local_1b4 <= local_1b0) {
        fVar7 = local_1b0;
        fVar9 = local_1b0;
      }
    }
  }
  else {
    fVar7 = 0.0;
    iVar4 = (**(code **)(*param_1 + 0xe0))(param_1);
    if (iVar4 == 1) {
      fVar7 = (float)FUN_1403722a0(*(undefined8 *)(param_3 + 0x298),param_3);
    }
    else {
      iVar4 = (**(code **)(*param_1 + 0xe0))(param_1);
      if (iVar4 == 2) {
        fVar7 = *(float *)(*(longlong *)(param_3 + 0x298) + 0x1d6c);
      }
    }
    lVar5 = (**(code **)(*param_1 + 0x220))(param_1,*(undefined4 *)(param_3 + 0x58));
    if (lVar5 != 0) {
      local_1c8 = Plus<float>::vftable;
      local_res18 = MostExtreme<float>::vftable;
      fVar6 = (float)FUN_14033fb70(lVar5 + 0x1f8,&local_1c8,&local_res18);
      fVar7 = fVar7 * (fVar6 + fVar8);
    }
    FUN_140535cb0(local_1a8,0);
    cVar3 = FUN_140395c70(param_3,local_1a8);
    if (cVar3 != '\0') {
      local_1c8 = Plus<float>::vftable;
      local_res18 = GreaterThan<float>::vftable;
      fVar6 = (float)FUN_14033fb70(local_c8,&local_1c8,&local_res18);
      fVar7 = fVar7 * (fVar6 + fVar8);
    }
    if (fVar9 <= fVar7) {
      fVar9 = fVar7;
    }
    local_1d8 = *(float *)(param_3 + 0x78);
    local_1d4 = *(float *)(param_3 + 0x7c);
    fVar7 = *(float *)(*(longlong *)(param_3 + 0x298) + 0x1d70);
    local_1d0 = *(float *)(param_3 + 0x80);
    FUN_140535fb0(local_1a8);
    fVar7 = fVar9 * fVar7;
  }
  cVar3 = (**(code **)(*param_1 + 0x240))(param_1);
  if (cVar3 == '\0') {
    return;
  }
  lVar5 = FUN_14028aa00(&DAT_140b153e0,param_1);
  if (lVar5 != 0) {
    uVar2 = *(undefined8 *)(lVar5 + 0x18);
    iVar4 = (**(code **)(*param_1 + 0xe0))(param_1);
    if (iVar4 == 1) {
      lVar5 = FUN_1402ac5a0(uVar2);
    }
    else {
      iVar4 = (**(code **)(*param_1 + 0xe0))(param_1);
      if (iVar4 != 2) goto LAB_14035d47a;
      lVar5 = FUN_1402ad0e0(uVar2);
    }
    if ((lVar5 != 0) &&
       (((param_2 == *(int *)(lVar5 + 8) &&
         (iVar4 = (**(code **)(*param_1 + 0xe0))(param_1), iVar4 == 1)) ||
        ((param_2 == *(int *)(lVar5 + 4) &&
         (iVar4 = (**(code **)(*param_1 + 0xe0))(param_1), iVar4 == 2)))))) {
      fVar8 = *(float *)((longlong)param_1 + 0x14);
    }
  }
LAB_14035d47a:
  if ((((param_1[0x33] != 0) && (-1 < param_2)) && (param_2 < (int)param_1[0x32])) &&
     (lVar5 = *(longlong *)(param_1[0x33] + (longlong)param_2 * 8), lVar5 != 0)) {
    FUN_1404c0ec0(lVar5,&local_1d8,fVar8 * fVar9,fVar8 * fVar7,param_4);
  }
  return;
}


