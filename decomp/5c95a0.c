/* FighterLocomotorBehaviorClass::FUN_1405c95a0  RVA 0x5c95a0  size 603  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405c95a0
               (longlong param_1,undefined8 param_2,float *param_3)

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  double dVar7;
  float fVar8;
  undefined4 local_68;
  undefined4 local_64;
  float local_60;
  
  fVar2 = (float)FUN_14020b6d0(param_2);
  lVar1 = *(longlong *)(param_1 + 0x28);
  fVar8 = (float)(*(uint *)(*(longlong *)(lVar1 + 0x298) + 0x39c) ^ DAT_140800860);
  if ((fVar8 <= fVar2) && (fVar8 = *(float *)(*(longlong *)(lVar1 + 0x298) + 0x39c), fVar2 <= fVar8)
     ) {
    fVar8 = fVar2;
  }
  fVar8 = (float)((uint)fVar8 ^ DAT_140800860);
  local_68 = *(undefined4 *)(lVar1 + 0x84);
  local_64 = *(undefined4 *)(lVar1 + 0x88);
  local_60 = *(float *)(lVar1 + 0x8c);
  fVar3 = (float)FUN_140372560(*(undefined8 *)(lVar1 + 0x298));
  fVar4 = (float)FUN_14020b6d0(local_68);
  fVar5 = (float)FUN_14020b6d0(fVar8 - fVar4);
  fVar6 = (float)FUN_1403724d0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
  fVar6 = fVar6 * (*param_3 / fVar3);
  if (0.0 <= fVar6) {
    fVar3 = DAT_1408524f8;
    if (fVar6 <= DAT_1408524f8) {
      fVar3 = fVar6;
    }
  }
  else {
    fVar3 = 0.0;
  }
  if (fVar3 < ABS(fVar5)) {
    if (fVar5 <= 0.0) {
      if (fVar5 < 0.0) {
        fVar4 = fVar4 - fVar3;
      }
    }
    else {
      fVar4 = fVar4 + fVar3;
    }
  }
  else {
    fVar4 = fVar4 + fVar5;
  }
  if (fVar4 != 0.0) {
    fVar3 = DAT_1407ffaf8;
    if (fVar8 < 0.0) {
      fVar3 = DAT_14080080c;
    }
    fVar8 = DAT_14080080c;
    if (0.0 <= fVar4) {
      fVar8 = DAT_1407ffaf8;
    }
    if (fVar3 != fVar8) {
      *param_3 = 0.0;
      goto LAB_1405c97a1;
    }
  }
  fVar8 = *param_3;
  if (fVar8 < ABS(fVar2)) {
    *param_3 = 0.0;
    if (fVar2 <= 0.0) {
      local_60 = local_60 - fVar8;
    }
    else {
      local_60 = local_60 + fVar8;
    }
  }
  else {
    local_60 = local_60 + fVar2;
    dVar7 = (double)FUN_1407760bc((double)fVar2);
    *param_3 = *param_3 - (float)dVar7;
  }
LAB_1405c97a1:
  local_68 = FUN_14020b710(fVar4);
  local_60 = (float)FUN_14020b710(local_60);
  FUN_1403a8710(*(undefined8 *)(param_1 + 0x28),&local_68);
  return;
}


