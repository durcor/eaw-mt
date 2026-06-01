/* FUN_140294a70  RVA 0x294a70  size 42  params 0  ret undefined */


undefined4 FUN_140294a70(longlong *param_1)

{
  longlong lVar1;
  
  if ((param_1[1] - *param_1 >> 3 != 0) &&
     (lVar1 = *(longlong *)(*param_1 + (longlong)(int)param_1[6] * 8), lVar1 != 0)) {
    return *(undefined4 *)(lVar1 + 0x4c);
  }
  return 0xffffffff;
}


