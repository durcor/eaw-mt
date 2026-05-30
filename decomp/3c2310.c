/* SelectBehaviorClass::vfunc_6  RVA 0x3c2310  size 300  params 0  ret undefined */


undefined8 SelectBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  
  LocomotorCommonClass::vfunc_6();
  if (*(char *)(*(longlong *)(param_2 + 0xb0) + 0x60) == '\0') {
    *(undefined1 *)(*(longlong *)(param_2 + 0xb0) + 0x60) = 1;
    FUN_1403c2440(param_1,param_2);
  }
  if (*(char *)(param_1 + 0x40) == '\0') {
    if (*(char *)(param_2 + 0x3a7) == '\0') {
      FUN_1403c19c0(param_1,param_2);
      goto LAB_1403c2360;
    }
  }
  else {
LAB_1403c2360:
    if (*(char *)(param_2 + 0x3a7) == '\0') goto LAB_1403c237e;
  }
  if (*(char *)(param_1 + 0x40) != '\0') {
    FUN_1403c1d80(param_1,param_2);
    *(undefined1 *)(param_1 + 0x40) = 0;
  }
LAB_1403c237e:
  FUN_1403c2710(param_1,param_2);
  fVar4 = *(float *)(param_1 + 0x54) * DAT_1408007d4;
  fVar1 = fVar4 * DAT_140b0a344;
  fVar2 = *(float *)(param_1 + 0x44) - *(float *)(param_1 + 0x48);
  fVar3 = DAT_1407ffaf8 /
          (fVar1 * DAT_140819c54 * fVar1 + fVar1 + DAT_1407ffaf8 +
          fVar1 * DAT_140819c50 * fVar1 * fVar1);
  fVar1 = (fVar2 * fVar4 + *(float *)(param_1 + 0x4c)) * DAT_140b0a344;
  *(float *)(param_1 + 0x44) = (fVar1 + fVar2) * fVar3 + *(float *)(param_1 + 0x48);
  *(float *)(param_1 + 0x4c) = (*(float *)(param_1 + 0x4c) - fVar1 * fVar4) * fVar3;
  return 0x650000;
}


