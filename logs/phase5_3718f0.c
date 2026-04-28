/* FUN_1403718f0  RVA 0x3718f0 */


undefined8 FUN_1403718f0(longlong param_1,uint param_2,int param_3,int param_4)

{
  longlong lVar1;
  int iVar2;
  longlong *plVar3;
  uint uVar4;
  int iVar5;
  longlong lVar6;
  
  if ((param_2 == 1) && (param_3 != -1)) {
    param_2 = param_3 + 0x18;
  }
  uVar4 = (param_2 ^ 0xdeadbeef) & 0x7fffffff;
  iVar5 = (uVar4 % 0x1f31d) * 0x41a7 + (uVar4 / 0x1f31d) * -0xb14;
  iVar2 = iVar5 + 0x7fffffff;
  if (-1 < iVar5) {
    iVar2 = iVar5;
  }
  plVar3 = (longlong *)
           (*(longlong *)(param_1 + 0xd70) +
           ((longlong)iVar2 & *(ulonglong *)(param_1 + 0xd88)) * 0x10);
  lVar1 = *(longlong *)(param_1 + 0xd60);
  lVar6 = plVar3[1];
  if (lVar6 != lVar1) {
    iVar2 = *(int *)(lVar6 + 0x10);
    iVar5 = iVar2 - param_2;
    if ((int)param_2 < iVar2) {
      do {
        if (lVar6 == *plVar3) {
          return 0;
        }
        lVar6 = *(longlong *)(lVar6 + 8);
      } while ((int)param_2 < *(int *)(lVar6 + 0x10));
      iVar2 = *(int *)(lVar6 + 0x10);
      iVar5 = iVar2 - param_2;
    }
    if (SBORROW4(iVar2,param_2) == iVar5 < 0) {
      if (lVar6 == 0) {
        lVar6 = lVar1;
      }
      if (((lVar6 != lVar1) && (iVar2 = *(int *)(lVar6 + 0x28), 0 < iVar2)) && (param_4 < iVar2)) {
        if (-1 < param_4) {
          return *(undefined8 *)(*(longlong *)(lVar6 + 0x20) + 8 + (longlong)param_4 * 0x10);
        }
        if (iVar2 != 1) {
          iVar2 = FUN_1401ffb40(&DAT_140a13e24,0,iVar2 + -1);
          return *(undefined8 *)(*(longlong *)(lVar6 + 0x20) + 8 + (longlong)iVar2 * 0x10);
        }
        return *(undefined8 *)(*(longlong *)(lVar6 + 0x20) + 8);
      }
    }
  }
  return 0;
}

