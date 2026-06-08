/* FUN_140396cb0  RVA 0x396cb0  size 36  params 0  ret undefined */


undefined8 FUN_140396cb0(longlong param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(param_1 + 0x2d0);
  if (((lVar1 != 0) && (-1 < param_2)) && (param_2 < *(int *)(lVar1 + 0x10))) {
    return *(undefined8 *)(*(longlong *)(lVar1 + 8) + (longlong)param_2 * 8);
  }
  return 0;
}


