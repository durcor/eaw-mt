/* FUN_1401ffb40  RVA 0x1ffb40  size 96  params 0  ret undefined */


int FUN_1401ffb40(uint *param_1,int param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  
  iVar3 = param_2;
  if (param_2 <= param_3) {
    iVar3 = param_3;
    param_3 = param_2;
  }
  if (param_3 == iVar3) {
    return param_3;
  }
  uVar4 = iVar3 - param_3;
  uVar1 = 0x4000;
  if (uVar4 < 0x7fff) {
    uVar2 = uVar4 & 0x4000;
    while (uVar2 == 0) {
      uVar1 = uVar1 >> 1;
      uVar2 = uVar4 & uVar1;
    }
  }
  else {
    uVar4 = 0x7ffe;
  }
  do {
    uVar2 = *param_1 * 0x41c64e6d + 0xbdf;
    *param_1 = uVar2;
    uVar2 = uVar2 >> 10 & 0x7fff & uVar1 * 2 - 1;
  } while ((int)uVar4 < (int)uVar2);
  return uVar2 + param_3;
}


