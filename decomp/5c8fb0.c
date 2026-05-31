/* FighterLocomotorBehaviorClass::FUN_1405c8fb0  RVA 0x5c8fb0  size 214  params 0  ret undefined */


void FighterLocomotorBehaviorClass::FUN_1405c8fb0(longlong param_1,float param_2,float *param_3)

{
  float fVar1;
  longlong lVar2;
  float fVar3;
  double dVar4;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  
  lVar2 = *(longlong *)(param_1 + 0x28);
  local_38 = *(undefined4 *)(lVar2 + 0x84);
  local_30 = *(undefined4 *)(lVar2 + 0x8c);
  fVar3 = (float)FUN_14020b6d0(*(undefined4 *)(lVar2 + 0x88));
  fVar1 = *param_3;
  if (fVar1 < ABS(param_2)) {
    if (param_2 <= 0.0) {
      if (0.0 <= param_2) goto LAB_1405c9055;
      fVar3 = fVar3 - fVar1;
    }
    else {
      fVar3 = fVar3 + fVar1;
    }
    *param_3 = 0.0;
  }
  else {
    fVar3 = fVar3 + param_2;
    dVar4 = (double)FUN_1407760bc((double)param_2);
    *param_3 = *param_3 - (float)dVar4;
  }
LAB_1405c9055:
  local_34 = FUN_14020b710(fVar3);
  FUN_1403a8710(*(undefined8 *)(param_1 + 0x28),&local_38);
  return;
}


