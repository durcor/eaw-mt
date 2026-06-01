/* FUN_140657db0  RVA 0x657db0  size 513  params 0  ret undefined */


void FUN_140657db0(longlong param_1,longlong *param_2,longlong param_3)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  int iVar16;
  int local_res10;
  
  if (param_2 != (longlong *)0x0) {
    lVar13 = (longlong)*(int *)(param_3 + 0x48);
    lVar9 = (**(code **)(*param_2 + 0x10))(param_2,0x12);
    if (lVar9 != 0) {
      local_res10 = 0;
      iVar4 = FUN_1404f5900(lVar9);
      if (0 < iVar4) {
        do {
          lVar10 = FUN_1404f58e0(lVar9,local_res10);
          pfVar1 = (float *)(*(longlong *)(lVar10 + 0x40) + lVar13 * 4);
          if (*pfVar1 <= 0.0 && *pfVar1 != 0.0) {
            return;
          }
          lVar11 = *(longlong *)(lVar10 + 0x58);
          iVar4 = *(int *)(param_1 + 0xb0);
          fVar2 = *(float *)(lVar11 + lVar13 * 8);
          fVar3 = *(float *)(*(longlong *)(lVar10 + 0x40) + (longlong)*(int *)(param_3 + 0x48) * 4);
          iVar5 = (int)((fVar2 - fVar3) / *(float *)(param_1 + 0xb8));
          iVar16 = 0;
          if (0 < iVar5) {
            iVar16 = iVar5;
          }
          iVar6 = (int)((fVar2 + fVar3) / *(float *)(param_1 + 0xb8));
          iVar5 = iVar4 + -1;
          if (iVar6 < iVar4 + -1) {
            iVar5 = iVar6;
          }
          iVar7 = (int)((*(float *)(lVar11 + 4 + lVar13 * 8) - fVar3) / *(float *)(param_1 + 0xbc));
          iVar6 = 0;
          if (0 < iVar7) {
            iVar6 = iVar7;
          }
          iVar7 = *(int *)(param_1 + 0xb4) + -1;
          iVar8 = (int)((fVar3 + *(float *)(lVar11 + 4 + lVar13 * 8)) / *(float *)(param_1 + 0xbc));
          if (iVar8 < iVar7) {
            iVar7 = iVar8;
          }
          if (iVar6 <= iVar7) {
            lVar11 = (longlong)(iVar4 * iVar6 + iVar16);
            uVar15 = (ulonglong)((iVar7 - iVar6) + 1);
            do {
              if (iVar16 <= iVar5) {
                uVar14 = (ulonglong)((iVar5 - iVar16) + 1);
                lVar12 = lVar11;
                do {
                  lVar11 = lVar12 + 1;
                  FUN_14020abe0(*(undefined8 *)
                                 (*(longlong *)
                                   (*(longlong *)(param_1 + 0x40) +
                                   (longlong)*(int *)(param_3 + 0x48) * 0x18) + lVar12 * 8),lVar10);
                  uVar14 = uVar14 - 1;
                  lVar12 = lVar11;
                } while (uVar14 != 0);
              }
              lVar11 = lVar11 + (iVar16 + -1 + (iVar4 - iVar5));
              uVar15 = uVar15 - 1;
            } while (uVar15 != 0);
          }
          local_res10 = local_res10 + 1;
          iVar4 = FUN_1404f5900(lVar9);
        } while (local_res10 < iVar4);
      }
    }
  }
  return;
}


