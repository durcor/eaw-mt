/* FUN_140383f70  RVA 0x383f70  size 1858  params 0  ret undefined */


float * FUN_140383f70(longlong param_1,float *param_2,longlong param_3,undefined1 *param_4)

{
  undefined4 uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  bool bVar8;
  int iVar9;
  int iVar10;
  longlong lVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float local_res20;
  float local_178;
  float local_174;
  float local_170;
  float local_168;
  float local_164;
  float local_160;
  undefined1 local_158 [12];
  float local_14c;
  float local_13c;
  float local_12c;
  undefined1 local_128 [12];
  float local_11c;
  float local_10c;
  float local_fc;
  undefined1 local_f8 [12];
  float local_ec;
  float local_dc;
  float local_cc;
  
  *param_4 = 0;
  FUN_140398440(param_3,&local_168);
  if ((*(longlong *)(param_3 + 0x2a0) == 0) ||
     (lVar5 = FUN_1402648b0(), fVar13 = DAT_1408007d4, fVar22 = DAT_1407ffaf8, lVar5 == 0)) {
    *param_2 = local_168;
    param_2[2] = local_160;
    param_2[1] = local_164;
    return param_2;
  }
  lVar5 = *(longlong *)(param_3 + 0x298);
  local_res20 = SUB84(param_4,0);
  if (0 < *(int *)(lVar5 + 0xe70)) {
    iVar9 = 0;
    lVar11 = 0;
    fVar12 = local_res20;
    fVar14 = local_res20;
    fVar15 = local_res20;
    do {
      uVar1 = *(undefined4 *)(lVar11 + *(longlong *)(lVar5 + 0xe68));
      uVar6 = FUN_1402648b0(*(undefined8 *)(param_3 + 0x2a0));
      cVar2 = FUN_14012d2c0(uVar6,uVar1,local_f8);
      fVar21 = local_cc;
      fVar20 = local_dc;
      fVar19 = local_ec;
      fVar16 = fVar12;
      fVar17 = fVar14;
      fVar18 = fVar15;
      if (cVar2 == '\x01') {
        local_178 = local_ec;
        local_174 = local_dc;
        local_170 = local_cc;
        lVar7 = FUN_1403858b0(param_1);
        if ((((lVar7 != 0) &&
             (cVar2 = FUN_14012d2c0(lVar7,*(undefined4 *)(param_1 + 0x38),local_158), cVar2 != '\0')
             ) && (fVar16 = local_13c, fVar17 = local_14c, fVar18 = local_12c,
                  -1 < *(int *)(param_1 + 0x3c))) &&
           (cVar2 = FUN_14012d2c0(lVar7,*(int *)(param_1 + 0x3c),local_128), fVar16 = fVar12,
           fVar17 = fVar14, fVar18 = fVar15, cVar2 != '\0')) {
          fVar12 = fVar22 / fVar13;
          fVar16 = (local_10c + local_13c) * fVar12;
          fVar17 = (local_11c + local_14c) * fVar12;
          fVar18 = (local_fc + local_12c) * fVar12;
        }
        fVar12 = fVar17 - fVar19;
        fVar14 = fVar16 - fVar20;
        fVar15 = fVar18 - fVar21;
        fVar12 = (float)FUN_140776d48(fVar12 * fVar12 + fVar14 * fVar14 + fVar15 * fVar15);
        fVar14 = (float)FUN_1403857d0(param_1);
        lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298);
        if ((0 < *(int *)(lVar7 + 0xc48)) &&
           (lVar7 = FUN_140373670(lVar7,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x324)),
           lVar7 != 0)) {
          fVar15 = (float)FUN_140397780(param_3);
          fVar14 = fVar14 + fVar15;
        }
        if ((fVar12 <= fVar14) && (cVar2 = FUN_140383ba0(param_1,&local_178), cVar2 == '\x01')) {
          *param_2 = fVar19;
          param_2[1] = fVar20;
          param_2[2] = fVar21;
          *param_4 = 1;
          return param_2;
        }
      }
      iVar9 = iVar9 + 1;
      lVar11 = lVar11 + 4;
      fVar12 = fVar16;
      fVar14 = fVar17;
      fVar15 = fVar18;
    } while (iVar9 < *(int *)(lVar5 + 0xe70));
  }
  if ((*(longlong *)(param_3 + 0x2d0) != 0) &&
     (iVar9 = *(int *)(*(longlong *)(param_3 + 0x2d0) + 0x10), 0 < iVar9)) {
    iVar3 = FUN_1401ffb40(&DAT_140a13e24,0,iVar9 + -1);
    iVar10 = 0;
    fVar12 = local_res20;
    fVar14 = local_res20;
    fVar15 = local_res20;
    do {
      lVar5 = FUN_140396cb0(param_3,iVar3);
      if ((*(longlong *)(lVar5 + 0x20) == 0) ||
         (*(char *)(*(longlong *)(lVar5 + 0x20) + 0x4d) != '\x01')) {
        bVar8 = false;
      }
      else {
        bVar8 = *(float *)(lVar5 + 0x28) <= 0.0;
      }
      fVar19 = fVar12;
      fVar20 = fVar14;
      fVar21 = fVar15;
      if (!bVar8) {
        iVar4 = *(int *)(lVar5 + 0x2c);
        if (iVar4 < 0) {
          iVar4 = *(int *)(lVar5 + 0x38);
          cVar2 = '\0';
          if (-1 < iVar4) goto LAB_1403842bc;
        }
        else {
LAB_1403842bc:
          uVar6 = FUN_1402648b0(*(undefined8 *)(*(longlong *)(lVar5 + 0x10) + 0x2a0));
          cVar2 = FUN_14012d2c0(uVar6,iVar4,local_158);
        }
        local_170 = 0.0;
        local_178 = 0.0;
        local_174 = 0.0;
        if (cVar2 == '\x01') {
          local_178 = local_14c;
          local_174 = local_13c;
          local_170 = local_12c;
        }
        fVar18 = local_170;
        fVar17 = local_174;
        fVar16 = local_178;
        lVar5 = FUN_1403858b0(param_1);
        if ((((lVar5 != 0) &&
             (cVar2 = FUN_14012d2c0(lVar5,*(undefined4 *)(param_1 + 0x38),local_158), cVar2 != '\0')
             ) && (fVar19 = local_14c, fVar20 = local_13c, fVar21 = local_12c,
                  -1 < *(int *)(param_1 + 0x3c))) &&
           (cVar2 = FUN_14012d2c0(lVar5,*(int *)(param_1 + 0x3c),local_128), fVar19 = fVar12,
           fVar20 = fVar14, fVar21 = fVar15, cVar2 != '\0')) {
          fVar13 = fVar22 / fVar13;
          fVar19 = (local_11c + local_14c) * fVar13;
          fVar20 = (local_10c + local_13c) * fVar13;
          fVar21 = (local_fc + local_12c) * fVar13;
        }
        fVar13 = fVar19 - fVar16;
        fVar12 = fVar20 - fVar17;
        fVar14 = fVar21 - fVar18;
        fVar12 = (float)FUN_140776d48(fVar13 * fVar13 + fVar12 * fVar12 + fVar14 * fVar14);
        fVar14 = (float)FUN_1403857d0(param_1);
        lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298);
        if ((0 < *(int *)(lVar5 + 0xc48)) &&
           (lVar5 = FUN_140373670(lVar5,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x324)),
           lVar5 != 0)) {
          fVar13 = (float)FUN_140397780(param_3);
          fVar14 = fVar14 + fVar13;
        }
        fVar13 = DAT_1408007d4;
        if ((fVar12 <= fVar14) &&
           (cVar2 = FUN_140383ba0(param_1,&local_178), fVar13 = DAT_1408007d4, cVar2 == '\x01')) {
          *param_2 = fVar16;
          param_2[1] = fVar17;
          param_2[2] = fVar18;
          *param_4 = 1;
          return param_2;
        }
      }
      iVar4 = iVar3 + 1;
      iVar10 = iVar10 + 1;
      iVar3 = 0;
      if (iVar4 < iVar9) {
        iVar3 = iVar4;
      }
      fVar12 = fVar19;
      fVar14 = fVar20;
      fVar15 = fVar21;
    } while (iVar10 < iVar9);
  }
  FUN_140398440(param_3,&local_178);
  lVar5 = FUN_1403858b0(param_1);
  fVar12 = local_res20;
  fVar14 = local_res20;
  fVar15 = local_res20;
  if ((((lVar5 != 0) &&
       (cVar2 = FUN_14012d2c0(lVar5,*(undefined4 *)(param_1 + 0x38),local_158), cVar2 != '\0')) &&
      (fVar12 = local_14c, fVar14 = local_12c, fVar15 = local_13c, -1 < *(int *)(param_1 + 0x3c)))
     && (cVar2 = FUN_14012d2c0(lVar5,*(int *)(param_1 + 0x3c),local_128), fVar12 = local_res20,
        fVar14 = local_res20, fVar15 = local_res20, cVar2 != '\0')) {
    fVar22 = fVar22 / fVar13;
    fVar12 = (local_11c + local_14c) * fVar22;
    fVar14 = (local_fc + local_12c) * fVar22;
    fVar15 = (local_10c + local_13c) * fVar22;
  }
  fVar22 = (float)FUN_140776d48((fVar12 - local_178) * (fVar12 - local_178) +
                                (fVar15 - local_174) * (fVar15 - local_174) +
                                (fVar14 - local_170) * (fVar14 - local_170));
  fVar13 = (float)FUN_1403857d0(param_1);
  fVar12 = (float)FUN_140397780(param_3);
  if ((fVar13 + fVar12 < fVar22) || (cVar2 = FUN_140383ba0(param_1,&local_178), cVar2 != '\x01')) {
    *param_2 = local_168;
    *param_4 = 0;
    param_2[1] = local_164;
    local_170 = local_160;
  }
  else {
    *param_2 = local_178;
    *param_4 = 1;
    param_2[1] = local_174;
  }
  param_2[2] = local_170;
  return param_2;
}


