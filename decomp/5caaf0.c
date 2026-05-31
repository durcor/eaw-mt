/* FighterLocomotorBehaviorClass::FUN_1405caaf0  RVA 0x5caaf0  size 1092  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FighterLocomotorBehaviorClass::FUN_1405caaf0
               (longlong param_1,float *param_2,undefined4 param_3,float *param_4,float *param_5,
               undefined8 param_6)

{
  longlong lVar1;
  bool bVar2;
  float fVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined8 uVar6;
  undefined8 *puVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  undefined8 in_stack_fffffffffffffef8;
  undefined4 uVar13;
  float local_d8;
  float local_d4;
  float local_d0;
  undefined8 local_c8;
  undefined4 local_c0;
  undefined1 local_b8 [4];
  undefined4 local_b4;
  undefined4 local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  float local_88;
  float fStack_84;
  float fStack_80;
  float fStack_7c;
  
  fVar10 = DAT_1408007dc;
  fVar3 = DAT_1408007d4;
  uVar13 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
  if (*param_5 <= 0.0) {
    return;
  }
  if (*param_4 <= 0.0) {
    return;
  }
  lVar5 = *(longlong *)(param_1 + 0x28);
  local_98 = _DAT_140800830;
  uStack_90 = _UNK_140800838;
  local_a8 = _DAT_140800820;
  uStack_a0 = _UNK_140800828;
  local_88 = _DAT_140800840;
  fStack_84 = _UNK_140800844;
  fStack_80 = _UNK_140800848;
  fStack_7c = _UNK_14080084c;
  ProjectileBehaviorClass::FUN_1402cf9e0
            (&local_a8,
             (uint)(((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * *(float *)(lVar5 + 0x88)) ^
             DAT_140800860);
  FUN_1400480f0(&local_a8,
                (uint)(((fVar10 * fVar3) / DAT_1408007f4) * *(float *)(lVar5 + 0x8c)) ^
                DAT_140800860);
  lVar1 = *(longlong *)(param_1 + 0x28);
  fVar11 = param_2[1] - *(float *)(lVar1 + 0x7c);
  fVar10 = *param_2 - *(float *)(lVar1 + 0x78);
  fVar12 = param_2[2] - *(float *)(lVar1 + 0x80);
  local_c8 = 0;
  local_c0 = 0;
  local_d8 = (float)local_a8 * fVar10 + local_a8._4_4_ * fVar11 + (float)uStack_a0 * fVar12 +
             uStack_a0._4_4_;
  local_d4 = (float)local_98 * fVar10 + local_98._4_4_ * fVar11 + (float)uStack_90 * fVar12 +
             uStack_90._4_4_;
  local_d0 = local_88 * fVar10 + fStack_84 * fVar11 + fStack_80 * fVar12 + fStack_7c;
  FUN_14020acd0(local_b8,&local_c8,&local_d8);
  fVar11 = (float)FUN_14020b6d0(local_b0);
  fVar10 = (float)FUN_14020b6d0(local_b4);
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
  if (DAT_1408007ec <= ABS(fVar11)) {
    if (*(char *)(lVar1 + 0x1d4) == '\0') goto LAB_1405cad0c;
  }
  else {
    *(undefined1 *)(lVar1 + 0x1d4) = 0;
LAB_1405cad0c:
    if (((*(int *)(lVar1 + 0x48) == 0x1c) || (ABS(fVar11) <= _DAT_1408a650c)) ||
       (fVar12 = (float)FUN_140372440(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298)),
       fVar12 <= 0.0)) goto LAB_1405cad66;
  }
  bVar2 = 0.0 < fVar10;
  fVar10 = DAT_1408524fc;
  if (bVar2) {
    fVar10 = DAT_1408524f8;
  }
  fVar11 = 0.0;
  *(undefined1 *)(lVar1 + 0x1d4) = 1;
LAB_1405cad66:
  fVar12 = (float)FUN_14020b6d0(*(undefined4 *)(lVar5 + 0x84));
  fVar8 = (float)FUN_1403724d0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
  fVar9 = (float)FUN_140372560(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
  if (DAT_1408524f8 < ABS(fVar9 * (fVar12 / fVar8) + fVar11)) {
    fVar12 = DAT_1407ffaf8;
    if (fVar11 < 0.0) {
      fVar12 = DAT_14080080c;
    }
    fVar11 = fVar11 - fVar12 * DAT_1408007f4;
  }
  FUN_1405c95a0(param_1,fVar11,param_4);
  FUN_1405c8fb0(param_1,fVar10,param_6);
  uVar6 = 0;
  if (*(longlong *)(param_1 + 0x58) != 0) {
    FUN_140046f10(*(longlong *)(param_1 + 0x58),1);
    *(undefined8 *)(param_1 + 0x58) = 0;
  }
  if (DAT_140b395e4 != '\0') {
    uVar4 = (**(code **)(*DAT_140b15418 + 0x100))();
    FUN_1401f93e0(&local_c8,0xff,0xff,0xff,CONCAT44(uVar13,0xff));
    lVar5 = FUN_140769c58(0x20);
    if (lVar5 != 0) {
      uVar6 = FUN_140046350(lVar5,0);
    }
    *(undefined8 *)(param_1 + 0x58) = uVar6;
    puVar7 = &DAT_140b15870;
    if (0xf < DAT_140b15888) {
      puVar7 = DAT_140b15870;
    }
    FUN_140047370(uVar6,uVar4,*(longlong *)(param_1 + 0x28) + 0x78,param_2,puVar7,&local_c8,fVar3,0,
                  1);
    FUN_1401f9440(&local_c8);
  }
  FUN_1405c9360(param_1,param_3,param_5);
  return;
}


