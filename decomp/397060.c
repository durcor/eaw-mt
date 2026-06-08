/* FUN_140397060  RVA 0x397060  size 82  params 0  ret undefined */


ulonglong FUN_140397060(longlong param_1)

{
  longlong lVar1;
  uint uVar2;
  ulonglong in_RAX;
  longlong lVar3;
  longlong *plVar4;
  
  if ((*(longlong *)(param_1 + 0x2b0) == 0) ||
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x2b0) + 200), lVar1 == 0)) {
    return in_RAX & 0xffffffffffffff00;
  }
  plVar4 = *(longlong **)(lVar1 + 0x10);
  if (plVar4 != (longlong *)0x0) {
    uVar2 = 0;
    if (0 < *(int *)(lVar1 + 0x18)) {
      lVar3 = 0;
      do {
        if (*plVar4 == param_1) goto LAB_1403970a9;
        uVar2 = uVar2 + 1;
        lVar3 = lVar3 + 1;
        plVar4 = plVar4 + 1;
      } while (lVar3 < *(int *)(lVar1 + 0x18));
    }
  }
  uVar2 = 0xffffffff;
LAB_1403970a9:
  return (ulonglong)(uVar2 >> 0x1f) ^ 1;
}


