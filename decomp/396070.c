/* FUN_140396070  RVA 0x396070  size 273  params 0  ret undefined */


float FUN_140396070(longlong param_1)

{
  int iVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  lVar4 = *(longlong *)(param_1 + 0x2d0);
  if (lVar4 != 0) {
    iVar1 = *(int *)(lVar4 + 0x10);
    if ((iVar1 != 0) && (uVar5 = 0, 0 < iVar1)) {
      plVar3 = *(longlong **)(lVar4 + 8);
      lVar4 = (longlong)iVar1;
      uVar6 = uVar5;
      do {
        if ((*plVar3 != 0) && (*(char *)(*(longlong *)(*plVar3 + 0x20) + 0x4d) == '\x01')) {
          uVar6 = (ulonglong)((int)uVar6 + 1);
        }
        plVar3 = plVar3 + 1;
        lVar4 = lVar4 + -1;
      } while (lVar4 != 0);
      if ((0 < (int)uVar6) && (iVar2 = FUN_14038e030(param_1), (int)uVar6 != iVar2)) {
        fVar8 = 0.0;
        fVar9 = 0.0;
        do {
          lVar4 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x2d0) + 8) + uVar5 * 8);
          if (*(char *)(*(longlong *)(lVar4 + 0x20) + 0x4d) == '\x01') {
            fVar8 = fVar8 + *(float *)(lVar4 + 0x28);
            fVar7 = (float)FUN_140385ae0();
            fVar9 = fVar9 + fVar7;
          }
          uVar5 = uVar5 + 1;
        } while ((longlong)uVar5 < (longlong)iVar1);
        if ((fVar8 != 0.0) && (fVar9 != 0.0)) {
          return fVar8 / fVar9;
        }
      }
    }
  }
  return 0.0;
}


