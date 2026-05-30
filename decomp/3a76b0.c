/* FUN_1403a76b0  RVA 0x3a76b0  size 499  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403a76b0(longlong param_1,undefined4 param_2)

{
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  longlong lVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  lVar6 = *(longlong *)(param_1 + 0x2d0);
  if ((lVar6 != 0) && ((*(byte *)(param_1 + 0x3a0) & 0x40) == 0)) {
    iVar1 = *(int *)(lVar6 + 0x10);
    lVar2 = (longlong)iVar1;
    if ((iVar1 != 0) && (uVar4 = 0, 0 < iVar1)) {
      plVar3 = *(longlong **)(lVar6 + 8);
      uVar5 = uVar4;
      lVar6 = lVar2;
      do {
        if ((*plVar3 != 0) && (*(char *)(*(longlong *)(*plVar3 + 0x20) + 0x4d) == '\x01')) {
          uVar5 = (ulonglong)((int)uVar5 + 1);
        }
        plVar3 = plVar3 + 1;
        lVar6 = lVar6 + -1;
      } while (lVar6 != 0);
      if (0 < (int)uVar5) {
        fVar9 = 0.0;
        uVar5 = uVar4;
        do {
          lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x2d0) + 8) + uVar5 * 8);
          if ((lVar6 != 0) && (*(char *)(*(longlong *)(lVar6 + 0x20) + 0x4d) == '\x01')) {
            fVar7 = (float)FUN_140540070();
            fVar9 = fVar9 + fVar7;
          }
          uVar5 = uVar5 + 1;
        } while ((longlong)uVar5 < lVar2);
        fVar8 = (float)FUN_140396df0(param_1);
        fVar7 = fVar8 + _DAT_140b16d78;
        if (DAT_1407ffaf8 <= fVar8 + _DAT_140b16d78) {
          fVar7 = DAT_1407ffaf8;
        }
        fVar8 = (float)FUN_140396070(param_1);
        fVar7 = (fVar8 - fVar7) * fVar9;
        if (fVar7 <= 0.0) {
          fVar7 = 0.0;
        }
        uVar5 = uVar4;
        if (0.0 < fVar7) {
          do {
            lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x2d0) + 8) + uVar5 * 8);
            if ((((lVar6 != 0) && (*(char *)(*(longlong *)(lVar6 + 0x20) + 0x4d) == '\x01')) &&
                ((*(longlong *)(lVar6 + 0x20) == 0 || (0.0 < *(float *)(lVar6 + 0x28))))) &&
               (fVar8 = (*(float *)(lVar6 + 0x28) / fVar9) * fVar7, 0.0 < fVar8)) {
              FUN_140387f50(lVar6,param_1,1,fVar8,0);
            }
            uVar5 = uVar5 + 1;
          } while ((longlong)uVar5 < lVar2);
        }
      }
      do {
        lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x2d0) + 8) + uVar4 * 8);
        if (lVar6 != 0) {
          FUN_140387010(lVar6,param_2);
        }
        uVar4 = uVar4 + 1;
      } while ((longlong)uVar4 < lVar2);
    }
  }
  return;
}


