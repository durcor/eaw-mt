/* FUN_14012d6f0  RVA 0x12d6f0  size 44  params 0  ret undefined */


undefined8 FUN_14012d6f0(longlong param_1,int param_2)

{
  if ((-1 < param_2) &&
     (param_2 < (int)(*(longlong *)(param_1 + 0xf8) - *(longlong *)(param_1 + 0xf0) >> 4))) {
    return *(undefined8 *)(*(longlong *)(param_1 + 0xf0) + 8 + (longlong)param_2 * 0x10);
  }
  return 0;
}


