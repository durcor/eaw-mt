/* FUN_140373670  RVA 0x373670  size 34  params 0  ret undefined */


undefined8 FUN_140373670(longlong param_1,int param_2)

{
  if ((-1 < param_2) && (param_2 < *(int *)(param_1 + 0xc48))) {
    return *(undefined8 *)(*(longlong *)(param_1 + 0xc40) + 8 + (longlong)param_2 * 0x10);
  }
  return 0;
}


