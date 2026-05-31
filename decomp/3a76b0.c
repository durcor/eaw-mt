/* FUN_1403a76b0  RVA 0x3a76b0  size 499  params 2  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Unknown calling convention */

void FUN_1403a76b0(GameObjectClass *param_1,int param_2)

{
  int iVar1;
  void *pvVar2;
  HardPointClass *pHVar3;
  longlong lVar4;
  longlong *plVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  longlong lVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  pvVar2 = param_1->hardpoints;
  if ((pvVar2 != (void *)0x0) && ((param_1->flags & 0x40) == 0)) {
    iVar1 = *(int *)((longlong)pvVar2 + 0x10);
    lVar4 = (longlong)iVar1;
    if ((iVar1 != 0) && (uVar6 = 0, 0 < iVar1)) {
      plVar5 = *(longlong **)((longlong)pvVar2 + 8);
      uVar7 = uVar6;
      lVar8 = lVar4;
      do {
        if ((*plVar5 != 0) && (*(char *)(*(longlong *)(*plVar5 + 0x20) + 0x4d) == '\x01')) {
          uVar7 = (ulonglong)((int)uVar7 + 1);
        }
        plVar5 = plVar5 + 1;
        lVar8 = lVar8 + -1;
      } while (lVar8 != 0);
      if (0 < (int)uVar7) {
        fVar11 = 0.0;
        uVar7 = uVar6;
        do {
          lVar8 = *(longlong *)(*(longlong *)((longlong)param_1->hardpoints + 8) + uVar7 * 8);
          if ((lVar8 != 0) && (*(char *)(*(longlong *)(lVar8 + 0x20) + 0x4d) == '\x01')) {
            fVar9 = (float)FUN_140540070();
            fVar11 = fVar11 + fVar9;
          }
          uVar7 = uVar7 + 1;
        } while ((longlong)uVar7 < lVar4);
        fVar10 = (float)FUN_140396df0(param_1);
        fVar9 = fVar10 + _DAT_140b16d78;
        if (DAT_1407ffaf8 <= fVar10 + _DAT_140b16d78) {
          fVar9 = DAT_1407ffaf8;
        }
        fVar10 = (float)FUN_140396070(param_1);
        fVar9 = (fVar10 - fVar9) * fVar11;
        if (fVar9 <= 0.0) {
          fVar9 = 0.0;
        }
        uVar7 = uVar6;
        if (0.0 < fVar9) {
          do {
            lVar8 = *(longlong *)(*(longlong *)((longlong)param_1->hardpoints + 8) + uVar7 * 8);
            if ((((lVar8 != 0) && (*(char *)(*(longlong *)(lVar8 + 0x20) + 0x4d) == '\x01')) &&
                ((*(longlong *)(lVar8 + 0x20) == 0 || (0.0 < *(float *)(lVar8 + 0x28))))) &&
               (fVar10 = (*(float *)(lVar8 + 0x28) / fVar11) * fVar9, 0.0 < fVar10)) {
              FUN_140387f50(lVar8,param_1,1,fVar10,0);
            }
            uVar7 = uVar7 + 1;
          } while ((longlong)uVar7 < lVar4);
        }
      }
      do {
        pHVar3 = *(HardPointClass **)(*(longlong *)((longlong)param_1->hardpoints + 8) + uVar6 * 8);
        if (pHVar3 != (HardPointClass *)0x0) {
          FUN_140387010(pHVar3,param_2);
        }
        uVar6 = uVar6 + 1;
      } while ((longlong)uVar6 < lVar4);
    }
  }
  return;
}


