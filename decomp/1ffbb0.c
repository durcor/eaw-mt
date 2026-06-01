/* FUN_1401ffbb0  RVA 0x1ffbb0  size 67  params 0  ret undefined */


undefined8 FUN_1401ffbb0(uint *param_1,undefined8 param_2,undefined8 param_3)

{
  uint uVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined4 uVar5;
  
  fVar4 = (float)param_3;
  fVar2 = (float)param_2;
  fVar3 = fVar2;
  uVar5 = (int)((ulonglong)param_3 >> 0x20);
  if (fVar4 < fVar2) {
    fVar3 = fVar4;
    fVar4 = fVar2;
    uVar5 = (int)((ulonglong)param_2 >> 0x20);
  }
  uVar1 = *param_1 * 0x41c64e6d + 0xbdf;
  *param_1 = uVar1;
  return CONCAT44(uVar5,(fVar4 - fVar3) * ((float)(uVar1 >> 10 & 0x7fff) / DAT_1408511e4) + fVar3);
}


