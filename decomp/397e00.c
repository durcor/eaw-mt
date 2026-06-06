/* FUN_140397e00  RVA 0x397e00  size 352  params 0  ret undefined */


undefined8 FUN_140397e00(longlong param_1,longlong param_2,int param_3)

{
  int iVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  
  if (param_2 == 0) {
    return 0;
  }
  iVar4 = FUN_140374b50(*(undefined8 *)(param_1 + 0x298),0);
  if (iVar4 == 0) {
    return 0;
  }
  cVar3 = FUN_14043fa90(&DAT_140b2f2e0,iVar4);
  lVar5 = param_1;
  if (cVar3 != '\0') {
    lVar5 = *(longlong *)(param_1 + 0x2b0);
  }
  lVar6 = lVar5;
  if (((iVar4 != 0x22) || (lVar6 = param_1, lVar5 == 0)) && (lVar5 = lVar6, lVar6 == 0)) {
    return 0;
  }
  cVar3 = FUN_1403986b0(lVar5,iVar4);
  if (cVar3 == '\0') {
    return 0;
  }
  cVar3 = FUN_14039b950(lVar5,iVar4,1);
  if (cVar3 == '\0') {
    return 0;
  }
  if (iVar4 == 0x22) {
    lVar6 = *(longlong *)(lVar5 + 0x100);
    if (lVar6 == 0) {
      return 0;
    }
    lVar2 = *(longlong *)(lVar6 + 0x280);
    if (lVar2 == 0) {
      return 0;
    }
    if ((lVar2 != param_2) && (lVar2 != *(longlong *)(param_2 + 0x2b0))) {
      return 0;
    }
    iVar1 = *(int *)(lVar6 + 0x288);
  }
  else {
    if (iVar4 != 0x29) goto LAB_140397f3b;
    cVar3 = FUN_14039b950(param_1,0x29,1);
    if (cVar3 == '\0') {
      return 0;
    }
    lVar6 = *(longlong *)(lVar5 + 0x100);
    if (lVar6 == 0) {
      return 0;
    }
    lVar2 = *(longlong *)(lVar6 + 0x270);
    if (lVar2 == 0) {
      return 0;
    }
    if ((lVar2 != param_2) && (lVar2 != *(longlong *)(param_2 + 0x2b0))) {
      return 0;
    }
    iVar1 = *(int *)(lVar6 + 0x278);
  }
  if ((iVar1 != -1) && (param_3 != iVar1)) {
    return 0;
  }
LAB_140397f3b:
  lVar5 = FUN_140397d90(lVar5,iVar4);
  if ((lVar5 != 0) && (0 < *(int *)(lVar5 + 0xf8))) {
    return *(undefined8 *)(*(longlong *)(lVar5 + 0xf0) + 8);
  }
  return 0;
}


