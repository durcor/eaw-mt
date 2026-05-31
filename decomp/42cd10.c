/* LocomotorCommonClass::vfunc_66  RVA 0x42cd10  size 27  params 0  ret undefined */


ulonglong LocomotorCommonClass::vfunc_66(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(*(ulonglong *)(param_1 + 0x28) + 0xa8);
  if (lVar1 == 0) {
    return *(ulonglong *)(param_1 + 0x28) & 0xffffffffffffff00;
  }
  return (ulonglong)*(byte *)(lVar1 + 0x260);
}


