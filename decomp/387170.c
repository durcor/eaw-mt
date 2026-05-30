/* FUN_140387170  RVA 0x387170  size 650  params 0  ret undefined */


void FUN_140387170(longlong param_1)

{
  int iVar1;
  float fVar2;
  uint uVar3;
  longlong lVar4;
  ulonglong uVar5;
  int iVar6;
  ulonglong uVar7;
  longlong lVar8;
  ulonglong uVar9;
  uint uVar10;
  ulonglong uVar11;
  float fVar12;
  float fVar13;
  
  uVar3 = DAT_140800860;
  fVar2 = DAT_1407ffaf8;
  if (*(int *)(param_1 + 0x80) < 1) {
    *(undefined1 *)(param_1 + 0x6f) = 0;
    return;
  }
  if (((*(longlong *)(param_1 + 0x20) != 0) &&
      (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4d) == '\x01')) &&
     (*(float *)(param_1 + 0x28) <= 0.0)) {
    FUN_14001f910(param_1 + 0x70);
    *(undefined1 *)(param_1 + 0x6f) = 0;
    return;
  }
  uVar5 = 0;
  uVar7 = uVar5;
  do {
    lVar4 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(uVar5 + *(longlong *)(param_1 + 0x78)));
    iVar6 = (int)uVar7;
    if (lVar4 == 0) {
      if ((-1 < iVar6) && (iVar1 = *(int *)(param_1 + 0x80), iVar6 < iVar1)) {
        uVar10 = (iVar1 - iVar6) - 1;
        if (uVar10 != 0) {
          if ((*(uint *)(param_1 + 0x84) & 0x80000000) != 0) {
            FUN_14076bd80(*(longlong *)(param_1 + 0x78) + (longlong)iVar6 * 4,
                          *(longlong *)(param_1 + 0x78) + (longlong)(iVar6 + 1) * 4);
            *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x80) + -1;
            goto LAB_1403873aa;
          }
          if (0 < (int)uVar10) {
            if (iVar6 + 1 < iVar6) {
              uVar11 = (ulonglong)uVar10;
              lVar4 = (longlong)(iVar1 + -1) * 4;
              lVar8 = (longlong)(int)(iVar6 + -1 + uVar10) * 4;
              do {
                *(undefined4 *)(lVar8 + *(longlong *)(param_1 + 0x78)) =
                     *(undefined4 *)(lVar4 + *(longlong *)(param_1 + 0x78));
                uVar11 = uVar11 - 1;
                lVar4 = lVar4 + -4;
                lVar8 = lVar8 + -4;
              } while (uVar11 != 0);
              *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x80) + -1;
              goto LAB_1403873aa;
            }
            uVar9 = (ulonglong)uVar10;
            uVar11 = uVar5;
            do {
              *(undefined4 *)(uVar11 + *(longlong *)(param_1 + 0x78)) =
                   *(undefined4 *)(uVar11 + 4 + *(longlong *)(param_1 + 0x78));
              uVar9 = uVar9 - 1;
              uVar11 = uVar11 + 4;
            } while (uVar9 != 0);
          }
        }
        *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x80) + -1;
      }
    }
    else {
      fVar13 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x470);
      if (fVar13 < *(float *)(lVar4 + 0x70) || fVar13 == *(float *)(lVar4 + 0x70)) {
        FUN_14027f370(lVar4,(uint)fVar13 ^ uVar3,0);
        fVar13 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x474);
        fVar12 = (float)FUN_140385ae0(param_1);
        fVar13 = fVar13 + *(float *)(param_1 + 0x28);
        if (0.0 <= fVar13) {
          if (fVar13 <= fVar12) {
            fVar12 = fVar13;
          }
        }
        else {
          fVar12 = 0.0;
        }
        *(float *)(param_1 + 0x28) = fVar12;
        fVar13 = (float)FUN_140396df0(*(undefined8 *)(param_1 + 0x10));
        fVar12 = (float)FUN_140396070(*(undefined8 *)(param_1 + 0x10));
        if (fVar13 < fVar12) {
          FUN_1403a89d0(*(longlong *)(param_1 + 0x10),
                        *(float *)(*(longlong *)(param_1 + 0x10) + 0x5c) *
                        ((fVar12 - fVar13) + fVar2));
        }
        fVar13 = (float)FUN_140385ae0(param_1);
        if (*(float *)(param_1 + 0x28) == fVar13) {
          FUN_14001f910(param_1 + 0x70);
          *(undefined2 *)(param_1 + 0x6e) = 0;
          FUN_140384d00(param_1,1,0);
          return;
        }
        uVar7 = (ulonglong)(iVar6 + 1);
        uVar5 = uVar5 + 4;
      }
      else {
        FUN_1402052a0(param_1 + 0x70,uVar7);
      }
    }
LAB_1403873aa:
    if (*(int *)(param_1 + 0x80) <= (int)uVar7) {
      return;
    }
  } while( true );
}


