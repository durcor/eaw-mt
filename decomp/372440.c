/* FUN_140372440  RVA 0x372440  size 136  params 0  ret undefined */


float FUN_140372440(longlong param_1)

{
  float fVar1;
  int iVar2;
  float fVar3;
  
  fVar1 = DAT_1407ffaf8;
  fVar3 = DAT_1407ffaf8;
  if ((*(byte *)(*(longlong *)(param_1 + 0x790) + 1) & 1) == 0) {
    iVar2 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar2 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    fVar3 = DAT_140b16dc0;
    if (((iVar2 != 2) && (fVar3 = DAT_140b16dc4, iVar2 != 1)) && (fVar3 = fVar1, iVar2 == 0)) {
      fVar3 = DAT_140b16dbc;
    }
    fVar3 = fVar3 * DAT_140a28568;
  }
  return fVar3 * *(float *)(param_1 + 0x3a0);
}


