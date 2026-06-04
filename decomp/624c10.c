/* SimpleSpaceLocomotorBehaviorClass::FUN_140624c10  RVA 0x624c10  size 420  params 0  ret undefined */


void SimpleSpaceLocomotorBehaviorClass::FUN_140624c10
               (longlong param_1,undefined4 *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  double dVar5;
  float fVar6;
  float fVar7;
  undefined1 local_78 [8];
  undefined4 local_70;
  
  FUN_14020b100(param_3,local_78,param_2);
  fVar1 = (float)FUN_14020b6d0(local_70);
  fVar2 = (float)FUN_140372560(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
  dVar5 = (double)FUN_1407760bc((double)(fVar1 / fVar2));
  fVar2 = (float)dVar5;
  if (0.0 <= fVar2) {
    fVar7 = DAT_1407ffaf8;
    if (fVar2 <= DAT_1407ffaf8) {
      fVar7 = fVar2;
    }
  }
  else {
    fVar7 = 0.0;
  }
  fVar3 = (float)FUN_14020b6d0(*param_2);
  fVar4 = (float)FUN_1403724d0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
  fVar2 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298) + 0x39c);
  fVar6 = fVar2;
  if (0.0 <= fVar1) {
    fVar6 = (float)((uint)fVar2 ^ DAT_140800860);
  }
  if (fVar7 < DAT_1408007c0) {
    fVar6 = fVar6 * (DAT_1407ffaf8 - (DAT_1408007c0 - fVar7) * DAT_1408007d4);
  }
  dVar5 = (double)FUN_1407760bc((double)(fVar6 - fVar3));
  fVar2 = (float)dVar5 / fVar2;
  fVar1 = DAT_1408007b4;
  if ((DAT_1408007b4 <= fVar2) && (fVar1 = DAT_1407ffaf8, fVar2 <= DAT_1407ffaf8)) {
    fVar1 = fVar2;
  }
  if (fVar6 <= fVar3) {
    if (fVar3 <= fVar6) {
      *param_3 = fVar3;
    }
    else {
      fVar3 = fVar3 - fVar1 * fVar4;
      if (fVar6 <= fVar3) {
        fVar6 = fVar3;
      }
      *param_3 = fVar6;
    }
  }
  else {
    fVar3 = fVar1 * fVar4 + fVar3;
    if (fVar3 <= fVar6) {
      fVar6 = fVar3;
    }
    *param_3 = fVar6;
  }
  return;
}


