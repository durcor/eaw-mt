/* FUN_1401ffdb0  RVA 0x1ffdb0  size 50  params 0  ret undefined */


undefined8 FUN_1401ffdb0(uint *param_1,int param_2)

{
  uint uVar1;
  
  if (param_2 == 0) {
    return 0;
  }
  if (param_2 != 100) {
    do {
      uVar1 = *param_1 * 0x41c64e6d + 0xbdf;
      *param_1 = uVar1;
      uVar1 = uVar1 >> 10 & 0x7f;
    } while (100 < uVar1);
    if (param_2 <= (int)uVar1) {
      return 0;
    }
  }
  return 1;
}


