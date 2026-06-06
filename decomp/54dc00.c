/* TeamTargetingBehaviorClass::FUN_14054dc00  RVA 0x54dc00  size 384  params 0  ret undefined */


ulonglong TeamTargetingBehaviorClass::FUN_14054dc00(longlong param_1,longlong param_2)

{
  ulonglong uVar1;
  ulonglong uVar2;
  bool bVar3;
  int iVar4;
  longlong lVar5;
  ulonglong uVar6;
  longlong *plVar7;
  uint uVar8;
  ulonglong uVar10;
  double dVar11;
  undefined4 uVar12;
  float fVar13;
  ulonglong uVar9;
  
  fVar13 = DAT_140800804;
  if ((*(longlong *)(param_1 + 0x20) == 0) || (param_2 == 0)) {
    return (ulonglong)(uint)DAT_140800804;
  }
  bVar3 = false;
  uVar10 = 0;
  uVar1 = *(ulonglong *)(param_2 + 0x1648);
  uVar2 = *(ulonglong *)(param_2 + 0x1650);
  iVar4 = FUN_14068dca0();
  uVar9 = uVar10;
  if (0 < iVar4) {
    do {
      lVar5 = FUN_14068dbb0(*(undefined8 *)(param_1 + 0x20),uVar9);
      if (lVar5 == param_2) {
        dVar11 = (double)FUN_14068dc80(*(undefined8 *)(param_1 + 0x20),uVar9);
        uVar12 = (undefined4)((ulonglong)dVar11 >> 0x20);
        fVar13 = (float)dVar11;
        goto LAB_14054dd3a;
      }
      uVar6 = FUN_14068db60();
      if ((uVar6 & uVar1) == 0) {
        uVar6 = FUN_14068db80(*(undefined8 *)(param_1 + 0x20),uVar9);
        if ((uVar2 & uVar6) != 0) {
          dVar11 = (double)FUN_14068dc80(*(undefined8 *)(param_1 + 0x20),uVar9);
          if (bVar3) {
            if ((float)dVar11 <= fVar13) {
              fVar13 = (float)dVar11;
            }
          }
          else {
            fVar13 = (float)dVar11;
          }
          bVar3 = true;
        }
      }
      else {
        dVar11 = (double)FUN_14068dc80();
        if ((float)dVar11 <= fVar13) {
          fVar13 = (float)dVar11;
        }
      }
      uVar8 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar8;
      iVar4 = FUN_14068dca0(*(undefined8 *)(param_1 + 0x20));
    } while ((int)uVar8 < iVar4);
  }
  uVar12 = 0;
  if (0 < *(int *)(param_1 + 0x38)) {
    plVar7 = (longlong *)(*(longlong *)(param_1 + 0x30) + 8);
    do {
      if (*plVar7 == param_2) goto LAB_14054dd32;
      uVar10 = uVar10 + 1;
      plVar7 = plVar7 + 2;
    } while ((longlong)uVar10 < (longlong)*(int *)(param_1 + 0x38));
  }
  if (((!bVar3) && ((*(ulonglong *)(param_1 + 0x48) & uVar1) != 0)) ||
     ((*(ulonglong *)(param_1 + 0x50) & uVar2) != 0)) {
LAB_14054dd32:
    uVar12 = 0;
    fVar13 = DAT_14080080c;
  }
LAB_14054dd3a:
  return CONCAT44(uVar12,fVar13);
}


