/* FUN_14001e680  RVA 0x1e680  size 218  params 0  ret undefined */


ulonglong * FUN_14001e680(ulonglong *param_1,ulonglong *param_2)

{
  ulonglong uVar1;
  longlong lVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  
  uVar3 = 0;
  *param_1 = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  uVar1 = param_2[2];
  if (0xf < param_2[3]) {
    param_2 = (ulonglong *)*param_2;
  }
  if (uVar1 < 0x10) {
    uVar3 = param_2[1];
    uVar4 = 0xf;
    *param_1 = *param_2;
    param_1[1] = uVar3;
  }
  else {
    uVar4 = uVar1 | 0xf;
    if (0x7fffffffffffffff < uVar4) {
      uVar4 = 0x7fffffffffffffff;
    }
    if (uVar4 + 1 < 0x1000) {
      if (uVar4 != 0xffffffffffffffff) {
        uVar3 = FUN_140769c58();
      }
    }
    else {
      if (uVar4 + 0x28 <= uVar4 + 1) {
                    /* WARNING: Subroutine does not return */
        FUN_1400225f0();
      }
      lVar2 = FUN_140769c58(uVar4 + 0x28);
      if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
      uVar3 = lVar2 + 0x27U & 0xffffffffffffffe0;
      *(longlong *)(uVar3 - 8) = lVar2;
    }
    *param_1 = uVar3;
    FUN_14076bd80(uVar3,param_2,uVar1 + 1);
  }
  param_1[2] = uVar1;
  param_1[3] = uVar4;
  return param_1;
}


