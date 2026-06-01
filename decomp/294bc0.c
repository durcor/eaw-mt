/* FUN_140294bc0  RVA 0x294bc0  size 24  params 0  ret undefined */


undefined8 FUN_140294bc0(longlong param_1,int param_2)

{
  if ((-1 < param_2) && (param_2 < *(int *)(param_1 + 0x28))) {
    return *(undefined8 *)(*(longlong *)(param_1 + 0x20) + (longlong)param_2 * 8);
  }
  return 0;
}


