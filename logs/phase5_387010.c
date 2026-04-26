/* =========================================================
 * FUN_140387010  (RVA 0x387010)
 * size: 344 bytes  params: 0  return: undefined
 * ========================================================= */


void FUN_140387010(longlong param_1,int param_2)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  longlong lVar5;
  undefined8 *puVar6;
  int iVar7;
  
  if (*(longlong *)(param_1 + 0x20) == 0) {
    return;
  }
  if (*(longlong *)(param_1 + 0x10) == 0) {
    return;
  }
  if ((*(byte *)(*(longlong *)(param_1 + 0x10) + 0x3a0) & 2) != 0) {
    return;
  }
  if ((DAT_140b15418 != 0) && (cVar2 = FUN_14035f790(), cVar2 != '\0')) {
    return;
  }
  lVar5 = *(longlong *)(param_1 + 0x20);
  iVar7 = param_2 - *(int *)(param_1 + 0x60);
  *(int *)(param_1 + 0x60) = param_2;
  if ((*(int *)(lVar5 + 0x48) - 5U < 6) && (*(char *)(param_1 + 0x6c) == '\x01')) {
    FUN_140387400(param_1,iVar7);
    lVar5 = *(longlong *)(param_1 + 0x20);
  }
  if ((((*(char *)(lVar5 + 0x4e) != '\x01') || (*(char *)(param_1 + 0x6c) != '\x01')) ||
      (*(longlong *)(param_1 + 0x10) == 0)) || (*(float *)(param_1 + 0x28) <= 0.0))
  goto LAB_140387148;
  if (*(int *)(param_1 + 0x94) < 0) {
    lVar5 = FUN_140385cf0(param_1);
    if (lVar5 == 0) goto LAB_140387148;
    puVar6 = (undefined8 *)(*(longlong *)(param_1 + 0x20) + 0x1c0);
    if (0xf < *(ulonglong *)(*(longlong *)(param_1 + 0x20) + 0x1d8)) {
      puVar6 = (undefined8 *)*puVar6;
    }
    iVar3 = FUN_14012d2a0(lVar5,puVar6);
    if (iVar3 < 0) goto LAB_140387148;
    *(int *)(param_1 + 0x94) = iVar3;
    FUN_14012d430(lVar5,iVar3,1);
    lVar1 = *(longlong *)(param_1 + 0x20);
    if (*(longlong *)(lVar1 + 0x1f0) != 0) {
      puVar6 = (undefined8 *)(lVar1 + 0x1e0);
      if (0xf < *(ulonglong *)(lVar1 + 0x1f8)) {
        puVar6 = (undefined8 *)*puVar6;
      }
      uVar4 = FUN_14012d2a0(lVar5,puVar6);
      *(undefined4 *)(param_1 + 0x98) = uVar4;
      FUN_14012d430(lVar5,uVar4,1);
    }
  }
  FUN_140381ff0(param_1);
LAB_140387148:
  if (*(char *)(param_1 + 0x6f) == '\x01') {
    FUN_140387170(param_1,iVar7);
  }
  return;
}



