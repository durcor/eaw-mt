/* GameObjectClass::vfunc_2  RVA 0x395ac0  size 43  params 0  ret undefined */


longlong GameObjectClass::vfunc_2(longlong param_1,int param_2)

{
  char cVar1;
  
  if (param_2 == 0) {
    return param_1;
  }
  cVar1 = *(char *)((longlong)param_2 + 0x332 + param_1);
  if (cVar1 == -1) {
    return 0;
  }
  return *(longlong *)(*(longlong *)(param_1 + 0x278) + (longlong)cVar1 * 8);
}


