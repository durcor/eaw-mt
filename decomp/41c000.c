/* FUN_14041c000  RVA 0x41c000  size 154  params 0  ret undefined */


undefined4 * FUN_14041c000(longlong param_1,undefined4 *param_2)

{
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  
  fVar3 = ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * *(float *)(param_1 + 8);
  fVar1 = fVar3;
  if (0x1d000000 < ((uint)fVar3 & 0x7f800000)) {
    fVar1 = (float)FUN_140776650(fVar3);
  }
  uVar2 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)fVar3 & 0x7f800000)) {
    uVar2 = FUN_140776150(fVar3);
  }
  param_2[1] = fVar1;
  *param_2 = uVar2;
  return param_2;
}


