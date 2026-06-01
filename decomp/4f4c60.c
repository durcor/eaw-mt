/* FUN_1404f4c60  RVA 0x4f4c60  size 1673  params 0  ret undefined */


void FUN_1404f4c60(longlong param_1)

{
  longlong lVar1;
  float fVar2;
  uint uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  int iVar7;
  undefined8 *puVar8;
  int iVar9;
  longlong *plVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  undefined8 local_ec;
  undefined8 uStack_e4;
  undefined8 local_dc;
  undefined8 uStack_d4;
  undefined8 local_cc;
  undefined8 uStack_c4;
  
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x108);
  *(undefined8 *)(lVar1 + 0x68) = local_ec;
  *(undefined8 *)(lVar1 + 0x70) = uStack_e4;
  *(undefined8 *)(lVar1 + 0x78) = local_dc;
  *(undefined8 *)(lVar1 + 0x80) = uStack_d4;
  *(undefined8 *)(lVar1 + 0x88) = local_cc;
  *(undefined8 *)(lVar1 + 0x90) = uStack_c4;
  *(undefined8 *)(lVar1 + 0x3c) = 0;
  *(undefined8 *)(lVar1 + 0x44) = 0;
  fVar21 = DAT_1408007f4;
  fVar13 = DAT_1408007dc;
  fVar20 = DAT_1408007d4;
  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2d0);
  if ((lVar4 != 0) && (*(int *)(lVar4 + 0x10) != 0)) {
    fVar17 = ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) *
             *(float *)(*(longlong *)(param_1 + 0x28) + 0x8c);
    fVar11 = fVar17;
    if (0x1d000000 < ((uint)fVar17 & 0x7f800000)) {
      fVar11 = (float)FUN_140776650(fVar17);
    }
    fVar2 = DAT_1407ffaf8;
    fVar12 = DAT_1407ffaf8;
    if (0x1d000000 < ((uint)fVar17 & 0x7f800000)) {
      fVar12 = (float)FUN_140776150(fVar17);
    }
    uVar3 = DAT_140800860;
    fVar17 = (float)((uint)fVar12 ^ DAT_140800860);
    iVar9 = 0;
    fVar21 = (fVar13 * fVar20) / fVar21;
    while( true ) {
      lVar4 = *(longlong *)(param_1 + 0x28);
      if (*(longlong *)(lVar4 + 0x2d0) == 0) {
        iVar7 = 0;
      }
      else {
        iVar7 = *(int *)(*(longlong *)(lVar4 + 0x2d0) + 0x10);
      }
      if (iVar7 <= iVar9) break;
      lVar4 = FUN_140396cb0(lVar4);
      if (lVar4 != 0) {
        lVar5 = *(longlong *)(lVar4 + 0x20);
        if (((((*(char *)(lVar5 + 0x4d) == '\0') || (lVar5 == 0)) ||
             ((*(char *)(lVar5 + 0x4d) != '\x01' || (0.0 < *(float *)(lVar4 + 0x28))))) &&
            ((*(char *)(lVar4 + 0x6e) == '\0' && (*(int *)(lVar5 + 0x48) - 5U < 6)))) &&
           ((*(int *)(lVar5 + 0x48) != 10 &&
            ((*(longlong *)(lVar5 + 0x210) != 0 &&
             (*(char *)(*(longlong *)(lVar5 + 0x210) + 0x47) != '\0')))))) {
          lVar5 = FUN_140383af0(lVar4);
          fVar18 = *(float *)(lVar5 + 8) * fVar21;
          fVar13 = fVar18;
          if (0x1d000000 < ((uint)fVar18 & 0x7f800000)) {
            fVar13 = (float)FUN_140776650(fVar18);
          }
          fVar14 = fVar2;
          if (0x1d000000 < ((uint)fVar18 & 0x7f800000)) {
            fVar14 = (float)FUN_140776150(fVar18);
          }
          fVar18 = *(float *)(*(longlong *)(lVar4 + 0x20) + 0x200);
          fVar19 = fVar14 * fVar12 + fVar13 * fVar11;
          if (0.0 <= fVar18) {
            fVar15 = DAT_1408524f8;
            if (fVar18 <= DAT_1408524f8) {
              fVar15 = fVar18;
            }
          }
          else {
            fVar15 = 0.0;
          }
          fVar18 = fVar2;
          if (0x1d000000 < ((uint)((fVar15 / fVar20) * fVar21) & 0x7f800000)) {
            fVar18 = (float)FUN_140776150();
          }
          if (fVar18 <= fVar19) {
            plVar10 = *(longlong **)(lVar4 + 0x20);
            if (0xf < (ulonglong)plVar10[3]) {
              plVar10 = (longlong *)*plVar10;
            }
            lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
            puVar8 = (undefined8 *)(lVar5 + 0xf8);
            if (0xf < *(ulonglong *)(lVar5 + 0x110)) {
              puVar8 = (undefined8 *)*puVar8;
            }
            FUN_140025760("*** Type %s ID %d hard point %s can fire FORWARD***\n",puVar8,
                          *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),plVar10);
            *(float *)(lVar1 + 0x3c) = *(float *)(lVar4 + 0x88) + *(float *)(lVar1 + 0x3c);
          }
          if (fVar18 <= (float)((uint)fVar19 ^ uVar3)) {
            plVar10 = *(longlong **)(lVar4 + 0x20);
            if (0xf < (ulonglong)plVar10[3]) {
              plVar10 = (longlong *)*plVar10;
            }
            lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
            puVar8 = (undefined8 *)(lVar5 + 0xf8);
            if (0xf < *(ulonglong *)(lVar5 + 0x110)) {
              puVar8 = (undefined8 *)*puVar8;
            }
            FUN_140025760("*** Type %s ID %d hard point %s can fire BACKWARD***\n",puVar8,
                          *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),plVar10);
            *(float *)(lVar1 + 0x48) = *(float *)(lVar4 + 0x88) + *(float *)(lVar1 + 0x48);
          }
          fVar13 = fVar13 * fVar17 + fVar14 * fVar11;
          if (fVar18 <= fVar13) {
            plVar10 = *(longlong **)(lVar4 + 0x20);
            if (0xf < (ulonglong)plVar10[3]) {
              plVar10 = (longlong *)*plVar10;
            }
            lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
            puVar8 = (undefined8 *)(lVar5 + 0xf8);
            if (0xf < *(ulonglong *)(lVar5 + 0x110)) {
              puVar8 = (undefined8 *)*puVar8;
            }
            FUN_140025760("*** Type %s ID %d hard point %s can fire RIGHT***\n",puVar8,
                          *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),plVar10);
            *(float *)(lVar1 + 0x44) = *(float *)(lVar4 + 0x88) + *(float *)(lVar1 + 0x44);
          }
          fVar20 = DAT_1408007d4;
          if (fVar18 <= (float)((uint)fVar13 ^ uVar3)) {
            plVar10 = *(longlong **)(lVar4 + 0x20);
            if (0xf < (ulonglong)plVar10[3]) {
              plVar10 = (longlong *)*plVar10;
            }
            lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
            puVar8 = (undefined8 *)(lVar5 + 0xf8);
            if (0xf < *(ulonglong *)(lVar5 + 0x110)) {
              puVar8 = (undefined8 *)*puVar8;
            }
            FUN_140025760("*** Type %s ID %d hard point %s can fire LEFT***\n",puVar8,
                          *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),plVar10);
            *(float *)(lVar1 + 0x40) = *(float *)(lVar4 + 0x88) + *(float *)(lVar1 + 0x40);
            fVar20 = DAT_1408007d4;
          }
        }
      }
      iVar9 = iVar9 + 1;
    }
    puVar8 = (undefined8 *)(*(longlong *)(lVar4 + 0x298) + 0xf8);
    if (0xf < *(ulonglong *)(*(longlong *)(lVar4 + 0x298) + 0x110)) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140025760("*** Type %s ID %d FORWARD firepower %.2f ***\n",puVar8,
                  *(undefined4 *)(lVar4 + 0x50),(double)*(float *)(lVar1 + 0x3c));
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
    puVar8 = (undefined8 *)(lVar4 + 0xf8);
    if (0xf < *(ulonglong *)(lVar4 + 0x110)) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140025760("*** Type %s ID %d RIGHT firepower %.2f ***\n",puVar8,
                  *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),
                  (double)*(float *)(lVar1 + 0x44));
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
    puVar8 = (undefined8 *)(lVar4 + 0xf8);
    if (0xf < *(ulonglong *)(lVar4 + 0x110)) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140025760("*** Type %s ID %d LEFT firepower %.2f ***\n",puVar8,
                  *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),
                  (double)*(float *)(lVar1 + 0x40));
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298);
    puVar8 = (undefined8 *)(lVar4 + 0xf8);
    if (0xf < *(ulonglong *)(lVar4 + 0x110)) {
      puVar8 = (undefined8 *)*puVar8;
    }
    FUN_140025760("*** Type %s ID %d BACKWARD firepower %.2f ***\n",puVar8,
                  *(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x50),
                  (double)*(float *)(lVar1 + 0x48));
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2d0);
    if ((lVar4 == 0) || (*(int *)(lVar4 + 0x10) == 0)) {
      fVar20 = 0.0;
    }
    else {
      lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x108);
      fVar21 = (float)FUN_14020b6d0(0);
      fVar13 = *(float *)(lVar4 + 0x3c);
      fVar20 = 0.0;
      if (*(float *)(lVar4 + 0x3c) < *(float *)(lVar4 + 0x44)) {
        fVar13 = *(float *)(lVar4 + 0x44);
        fVar20 = DAT_1408007ec;
      }
      if ((fVar13 < *(float *)(lVar4 + 0x40)) ||
         (((*(float *)(lVar4 + 0x40) == fVar13 && (fVar20 != 0.0)) &&
          (fVar13 = (float)FUN_14020b6d0(DAT_1408007ec - fVar21), ABS(fVar13) < ABS(fVar21))))) {
        fVar20 = DAT_14087b268;
      }
    }
    *(float *)(lVar1 + 0x4c) = fVar20;
    return;
  }
  uVar6 = FUN_1403973b0();
  uVar16 = FUN_140370e60(uVar6);
  *(undefined4 *)(lVar1 + 0x3c) = uVar16;
  return;
}


