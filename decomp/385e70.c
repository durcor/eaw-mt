/* FUN_140385e70  RVA 0x385e70  size 688  params 0  ret undefined */


undefined8 FUN_140385e70(longlong param_1,float *param_2,undefined4 *param_3,undefined4 *param_4)

{
  char cVar1;
  longlong lVar2;
  float fVar3;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  float local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  float local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  float local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  float local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  float local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  float local_5c;
  
  lVar2 = FUN_1403858b0();
  if ((lVar2 == 0) ||
     (cVar1 = FUN_14012d2c0(lVar2,*(undefined4 *)(param_1 + 0x38),&local_b8), cVar1 == '\0')) {
    return 0;
  }
  if (*(int *)(param_1 + 0x3c) < 0) {
    if (param_2 != (float *)0x0) {
      *param_2 = local_ac;
      param_2[1] = local_9c;
      param_2[2] = local_8c;
    }
    if (param_3 != (undefined4 *)0x0) {
      *param_3 = local_b8;
      param_3[2] = local_b0;
      param_3[4] = local_a8;
      param_3[6] = local_a0;
      param_3[1] = local_b4;
      param_3[8] = local_98;
      param_3[5] = local_a4;
      param_3[10] = local_90;
      param_3[3] = local_ac;
      param_3[7] = local_9c;
      param_3[9] = local_94;
      param_3[0xb] = local_8c;
    }
  }
  else {
    cVar1 = FUN_14012d2c0(lVar2,*(int *)(param_1 + 0x3c),&local_88);
    if (cVar1 == '\0') {
      return 0;
    }
    fVar3 = DAT_1407ffaf8 / DAT_1408007d4;
    if (param_2 != (float *)0x0) {
      *param_2 = (local_7c + local_ac) * fVar3;
      param_2[1] = (local_6c + local_9c) * fVar3;
      param_2[2] = (local_5c + local_8c) * fVar3;
    }
    if (param_3 != (undefined4 *)0x0) {
      *param_3 = local_b8;
      param_3[2] = local_b0;
      param_3[4] = local_a8;
      param_3[6] = local_a0;
      param_3[1] = local_b4;
      param_3[8] = local_98;
      param_3[5] = local_a4;
      param_3[10] = local_90;
      param_3[3] = local_ac;
      param_3[7] = local_9c;
      param_3[9] = local_94;
      param_3[0xb] = local_8c;
    }
    if (param_4 != (undefined4 *)0x0) {
      *param_4 = local_88;
      param_4[2] = local_80;
      param_4[4] = local_78;
      param_4[6] = local_70;
      param_4[1] = local_84;
      param_4[8] = local_68;
      param_4[5] = local_74;
      param_4[10] = local_60;
      param_4[3] = local_7c;
      param_4[7] = local_6c;
      param_4[9] = local_64;
      param_4[0xb] = local_5c;
      return 1;
    }
  }
  return 1;
}


