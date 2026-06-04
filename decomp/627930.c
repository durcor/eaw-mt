/* SimpleSpaceLocomotorBehaviorClass::FUN_140627930  RVA 0x627930  size 809  params 0  ret undefined */


undefined8
SimpleSpaceLocomotorBehaviorClass::FUN_140627930
          (undefined8 param_1,longlong param_2,uint param_3,undefined4 *param_4,undefined4 *param_5,
          undefined8 param_6,undefined4 *param_7,undefined4 *param_8,undefined4 *param_9)

{
  longlong lVar1;
  longlong lVar2;
  undefined4 *puVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float local_c8;
  float local_c4;
  undefined4 local_c0;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined8 local_a8;
  undefined4 local_a0;
  undefined1 local_98 [16];
  undefined1 local_88 [8];
  undefined4 local_80;
  undefined1 local_78 [64];
  
  if (param_2 == 0) {
    return 0;
  }
  *param_7 = *param_4;
  param_7[1] = param_4[1];
  param_7[2] = param_4[2];
  *param_8 = *param_5;
  param_8[1] = param_5[1];
  param_8[2] = param_5[2];
  *param_9 = 0;
  if ((uint)((*(longlong *)(param_2 + 0x20) - *(longlong *)(param_2 + 0x18)) / 0x34) < 2) {
    FUN_1405c5dc0(param_2);
  }
  else {
    lVar1 = FUN_14054fc40(param_2 + 0x18,0);
    lVar2 = FUN_14054fc40(param_2 + 0x18,1);
    local_a8 = 0;
    local_a0 = 0;
    FUN_14020acd0(local_98,&local_a8,lVar1 + 0xc);
    local_a8 = 0;
    local_a0 = 0;
    FUN_14020acd0(&local_b8,&local_a8,lVar2 + 0xc);
    fVar5 = DAT_1407ffaf8;
    fVar4 = *(float *)(lVar2 + 0x30) - *(float *)(lVar1 + 0x30);
    if (fVar4 == 0.0) {
      *param_8 = local_b8;
      param_8[2] = local_b0;
      param_8[1] = local_b4;
      FUN_1405c5dc0(param_2);
    }
    else {
      fVar4 = ((float)param_3 - *(float *)(lVar1 + 0x30)) / fVar4;
      if (DAT_1407ffaf8 <= fVar4) {
        FUN_1405c5dc0(param_2);
      }
      fVar6 = 0.0;
      if ((0.0 <= fVar4) && (fVar6 = fVar5, fVar4 <= fVar5)) {
        fVar6 = fVar4;
      }
      FUN_14020b100(&local_b8,local_88,local_98);
      fVar4 = (float)FUN_14020b6d0(local_80);
      local_c8 = *(float *)(lVar1 + 0xc);
      local_c4 = *(float *)(lVar1 + 0x10);
      fVar4 = ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * fVar6 * fVar4;
      local_c0 = *(undefined4 *)(lVar1 + 0x14);
      if (0x1d000000 < ((uint)fVar4 & 0x7f800000)) {
        fVar5 = (float)FUN_140776150(fVar4);
      }
      if (0x1d000000 < ((uint)fVar4 & 0x7f800000)) {
        fVar4 = (float)FUN_140776650(fVar4);
      }
      local_a8 = 0;
      local_a0 = 0;
      fVar6 = local_c8 * fVar4;
      local_c8 = local_c8 * fVar5 - local_c4 * fVar4;
      local_c4 = fVar6 + local_c4 * fVar5;
      puVar3 = (undefined4 *)FUN_14020acd0(local_78,&local_a8,&local_c8);
      *param_8 = *puVar3;
      param_8[1] = puVar3[1];
      param_8[2] = puVar3[2];
      FUN_140624c10(param_1,param_5,param_8);
    }
  }
  return 1;
}


