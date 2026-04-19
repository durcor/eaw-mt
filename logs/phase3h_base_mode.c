
// ================================================================
// BASE MODE CTOR — FUN_14035a5e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35a5e0  Name: FUN_14035a5e0  Size: 1642 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x00014035aa0e) */
/* WARNING: Removing unreachable block (ram,0x00014035aa5a) */

undefined8 * FUN_14035a5e0(undefined8 *param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  longlong lVar5;
  uint uVar6;
  int iVar7;
  longlong lVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  undefined4 *puVar11;
  ulonglong uVar12;
  uint uVar13;
  undefined4 *puVar14;
  ulonglong uVar15;
  longlong lVar16;
  longlong lVar17;
  
  uVar12 = 0;
  param_1[0xd] = 0xf;
  param_1[10] = 0;
  param_1[0xc] = 0;
  param_1[0xe] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0xf;
  *param_1 = GameModeClass::vftable;
  iVar7 = 0;
  *(undefined4 *)(param_1 + 2) = 0;
  *(undefined4 *)((longlong)param_1 + 0x14) = 0x3f800000;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[1] = 0xffffffffffffffff;
  param_1[0x12] = 0;
  *(undefined2 *)(param_1 + 0x13) = 0;
  *(undefined1 *)((longlong)param_1 + 0x9a) = 1;
  FUN_140490460((longlong)param_1 + 0x9c);
  param_1[0x14] = 0;
  param_1[0x16] = 0;
  param_1[0x17] = 0xf;
  param_1[0x18] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0xf;
  param_1[0x1c] = 0;
  param_1[0x1d] = 0;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  *(undefined1 *)(param_1 + 0x20) = 0;
  *(undefined4 *)((longlong)param_1 + 0x104) = 0;
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  lVar8 = FUN_140769c58(0x20);
  *(longlong *)lVar8 = lVar8;
  *(longlong *)(lVar8 + 8) = lVar8;
  param_1[0x21] = lVar8;
  uVar10 = param_1[0x21];
  param_1[0x23] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 7;
  param_1[0x27] = 8;
  *(undefined4 *)((longlong)param_1 + 0x104) = 0x3f800000;
  puVar9 = (undefined8 *)FUN_140769c58(0x80);
  param_1[0x23] = puVar9;
  puVar1 = puVar9 + 0x10;
  param_1[0x24] = puVar1;
  param_1[0x25] = puVar1;
  for (; puVar9 != puVar1; puVar9 = puVar9 + 1) {
    *puVar9 = uVar10;
  }
  param_1[0x29] = 0;
  param_1[0x28] = DynamicVectorClass<struct_DelayedEventStruct*___ptr64>::vftable;
  param_1[0x2a] = 0;
  param_1[0x2b] = DynamicVectorClass<struct_ObjectiveStruct>::vftable;
  param_1[0x2c] = 0;
  param_1[0x2d] = 0;
  *(undefined4 *)(param_1 + 0x2e) = 1;
  param_1[0x2f] = 0;
  *(undefined1 *)(param_1 + 0x31) = 0;
  param_1[0x37] = DynamicVectorClass<int>::vftable;
  param_1[0x38] = 0;
  *(undefined4 *)(param_1 + 0x39) = 0;
  *(undefined8 *)((longlong)param_1 + 0x1cc) = 0xffffffff80000000;
  *(undefined4 *)(param_1 + 0x32) = 0;
  param_1[0x33] = 0;
  param_1[0x34] = 0;
  param_1[0x35] = 0;
  param_1[0x36] = 0;
  param_1[0x3b] =
       DynamicVectorClass<class_DynamicVectorClass<class_GameObjectClass*___ptr64>_>::vftable;
  param_1[0x3c] = 0;
  param_1[0x3d] = 0;
  *(undefined4 *)((longlong)param_1 + 0x1d4) = 0xffffffff;
  param_1[0x3e] = 0;
  *(undefined4 *)(param_1 + 0x3f) = 0xffffffff;
  param_1[0x40] = 0;
  param_1[0x41] = 0;
  param_1[0x42] = 0;
  param_1[0x43] = 0;
  param_1[0x44] = 0;
  *(undefined4 *)(param_1 + 0x45) = 0x461c4000;
  *(undefined4 *)((longlong)param_1 + 0x22c) = 0x461c4000;
  *(undefined4 *)(param_1 + 0x46) = 0x3f800000;
  *(undefined4 *)((longlong)param_1 + 0x234) = 0x3f800000;
  *(undefined1 *)(param_1 + 0x47) = 0;
  *(undefined4 *)((longlong)param_1 + 0x23c) = 0;
  param_1[0x48] = 0;
  param_1[0x49] = 0;
  lVar8 = FUN_140769c58(0x18);
  *(longlong *)lVar8 = lVar8;
  *(longlong *)(lVar8 + 8) = lVar8;
  param_1[0x48] = lVar8;
  param_1[0x4a] = 0;
  param_1[0x4b] = 0;
  param_1[0x4c] = 0;
  param_1[0x4d] = 7;
  param_1[0x4e] = 8;
  *(undefined4 *)((longlong)param_1 + 0x23c) = 0x3f800000;
  FUN_140048250(param_1 + 0x4a,0x10,param_1[0x48]);
  param_1[0x51] = 0;
  param_1[0x50] = DynamicVectorClass<class_Vector3>::vftable;
  param_1[0x52] = 0;
  *(undefined1 *)(param_1 + 0x4f) = 0;
  param_1[0x53] = 0;
  param_1[0x54] = 0;
  param_1[0x55] = 0;
  *(undefined1 *)(param_1 + 0x56) = 0;
  *(undefined8 *)((longlong)param_1 + 0x2b4) = 0xffffffffffffffff;
  *(undefined8 *)((longlong)param_1 + 700) = 0xffffffffffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2c4) = 0x3fffff;
  *(undefined4 *)(param_1 + 0x59) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x2cc) = 0x1000000;
  *(undefined2 *)(param_1 + 0x5a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x2d4) = 0;
  param_1[0x5b] = 0;
  param_1[0x5c] = 0;
  param_1[0x5e] = 0;
  param_1[0x5f] = 0xf;
  param_1[0x60] = 0;
  param_1[0x62] = 0;
  param_1[99] = 0xf;
  uVar13 = DAT_140a16fb0;
  lVar8 = param_1[0x51];
  if (DAT_140a16fb0 == 0) {
    if (lVar8 == 0) goto LAB_14035abee;
    thunk_FUN_1407864b8(lVar8);
    *(uint *)((longlong)param_1 + 0x294) = *(uint *)((longlong)param_1 + 0x294) & 0x80000000;
    param_1[0x51] = 0;
  }
  else {
    if (lVar8 == 0) {
      auVar3._8_8_ = 0;
      auVar3._0_8_ = (longlong)(int)DAT_140a16fb0;
      uVar10 = SUB168(ZEXT816(0xc) * auVar3,0);
      if (SUB168(ZEXT816(0xc) * auVar3,8) != 0) {
        uVar10 = 0xffffffffffffffff;
      }
      uVar10 = thunk_FUN_140769c58(uVar10);
      param_1[0x51] = uVar10;
    }
    else {
      auVar4._8_8_ = 0;
      auVar4._0_8_ = (longlong)(int)DAT_140a16fb0;
      uVar10 = SUB168(ZEXT816(0xc) * auVar4,0);
      if (SUB168(ZEXT816(0xc) * auVar4,8) != 0) {
        uVar10 = 0xffffffffffffffff;
      }
      lVar8 = thunk_FUN_140769c58(uVar10);
      uVar6 = *(int *)((longlong)param_1 + 0x294) * 2 >> 1;
      if ((int)uVar13 < (int)uVar6) {
        uVar6 = uVar13;
      }
      lVar16 = (longlong)(int)uVar6;
      uVar15 = uVar12;
      if (3 < lVar16) {
        lVar5 = -lVar8;
        lVar17 = (lVar16 - 4U >> 2) + 1;
        uVar15 = lVar17 * 4;
        puVar14 = (undefined4 *)(lVar8 + 0x14);
        do {
          lVar2 = param_1[0x51];
          puVar14[-5] = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5 + -0x14);
          puVar14[-4] = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5 + -0x10);
          puVar14[-3] = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5 + -0xc);
          lVar2 = param_1[0x51];
          puVar14[-2] = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5 + -8);
          puVar14[-1] = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5 + -4);
          *puVar14 = *(undefined4 *)((longlong)puVar14 + lVar2 + lVar5);
          puVar11 = (undefined4 *)((longlong)puVar14 + param_1[0x51] + (4 - lVar8));
          puVar14[1] = *puVar11;
          puVar14[2] = puVar11[1];
          puVar14[3] = puVar11[2];
          puVar11 = (undefined4 *)((longlong)puVar14 + param_1[0x51] + (0x10 - lVar8));
          puVar14[4] = *puVar11;
          puVar14[5] = puVar11[1];
          puVar14[6] = puVar11[2];
          lVar17 = lVar17 + -1;
          puVar14 = puVar14 + 0xc;
        } while (lVar17 != 0);
      }
      if ((longlong)uVar15 < lVar16) {
        lVar16 = lVar16 - uVar15;
        puVar14 = (undefined4 *)(lVar8 + uVar15 * 0xc + 8);
        do {
          puVar11 = (undefined4 *)((longlong)puVar14 + param_1[0x51] + (-8 - lVar8));
          puVar14[-2] = *puVar11;
          puVar14[-1] = puVar11[1];
          *puVar14 = puVar11[2];
          lVar16 = lVar16 + -1;
          puVar14 = puVar14 + 3;
        } while (lVar16 != 0);
      }
      thunk_FUN_1407864b8(param_1[0x51]);
      param_1[0x51] = lVar8;
    }
    *(uint *)((longlong)param_1 + 0x294) =
         *(uint *)((longlong)param_1 + 0x294) ^
         (uVar13 ^ *(uint *)((longlong)param_1 + 0x294)) & 0x7fffffff;
  }
  iVar7 = *(int *)((longlong)param_1 + 0x294);
LAB_14035abee:
  iVar7 = iVar7 * 2 >> 1;
  *(int *)(param_1 + 0x52) = iVar7;
  uVar15 = uVar12;
  if (0 < iVar7) {
    do {
      lVar8 = param_1[0x51];
      uVar13 = (int)uVar15 + 1;
      *(undefined8 *)(uVar12 + lVar8) = 0;
      *(undefined4 *)(uVar12 + 8 + lVar8) = 0;
      uVar12 = uVar12 + 0xc;
      uVar15 = (ulonglong)uVar13;
    } while ((int)uVar13 < *(int *)(param_1 + 0x52));
  }
  return param_1;
}




// ================================================================
// SCENE FLAG-GATED SUB-CALL — FUN_1402fd330
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2fd330  Name: FUN_1402fd330  Size: 2461 bytes
// ------------------------------------------------------------

void FUN_1402fd330(longlong param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  float fVar5;
  char cVar6;
  int iVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  ulonglong uVar11;
  char ****ppppcVar12;
  int iVar13;
  char *pcVar14;
  char *****pppppcVar15;
  undefined8 *puVar16;
  ulonglong *puVar17;
  undefined8 uVar18;
  undefined8 *puVar19;
  ulonglong uVar20;
  ulonglong *puVar21;
  ulonglong *puVar22;
  ulonglong uVar23;
  char *****pppppcVar24;
  undefined8 *puVar25;
  longlong lVar26;
  uint uVar27;
  char *pcVar28;
  char *****pppppcVar29;
  longlong lVar30;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  char ****local_78 [2];
  undefined8 local_68;
  ulonglong local_60;
  ulonglong local_58 [2];
  undefined8 local_48;
  ulonglong local_40;
  
  *(undefined1 *)(param_1 + 0x2118) = *(undefined1 *)(param_1 + 0x49);
  *(undefined1 *)(param_1 + 0x48) = 0;
  if ((undefined8 **)(param_1 + 0x2708) != &DAT_140b16ae0) {
    puVar16 = &DAT_140b16ae0;
    if (0xf < DAT_140b16af8) {
      puVar16 = DAT_140b16ae0;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2708),puVar16,DAT_140b16af0);
  }
  if ((undefined8 **)(param_1 + 0x2728) != &DAT_140b16b20) {
    puVar16 = &DAT_140b16b20;
    if (0xf < DAT_140b16b38) {
      puVar16 = DAT_140b16b20;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2728),puVar16,DAT_140b16b30);
  }
  if ((undefined8 **)(param_1 + 0x2748) != &DAT_140b16b60) {
    puVar16 = &DAT_140b16b60;
    if (0xf < DAT_140b16b78) {
      puVar16 = DAT_140b16b60;
    }
    FUN_140022730((undefined8 *)(param_1 + 0x2748),puVar16,DAT_140b16b70);
  }
  if (*(char *)(param_1 + 0x2798) == '\0') {
    FUN_1402fa890(param_1);
  }
  if (*(longlong *)(param_1 + 0x27a0) == 0) {
    FUN_1402fa810(param_1);
  }
  if (*(char *)(param_1 + 0x2698) == '\0') {
    FUN_1402fab80(param_1,"data/art/models/i_tutorial_arrow.alo");
  }
  puVar22 = (ulonglong *)(param_1 + 0x2298);
  lVar26 = 10;
  pcVar28 = (char *)(param_1 + 0xb0);
  lVar30 = param_1 + 0x22b0;
  puVar21 = (ulonglong *)(param_1 + 0xa0);
  do {
    if ((char)puVar21[-5] != '\0') {
      iVar7 = (int)puVar21[1];
      if (iVar7 == 0) {
        *(undefined1 *)(puVar22 + -1) = 1;
        *puVar22 = puVar21[-4];
        *(undefined4 *)((longlong)puVar22 + -4) = *(undefined4 *)((longlong)puVar21 + -0x24);
        *(undefined1 *)((longlong)puVar22 + -7) = *(undefined1 *)((longlong)puVar21 + -0x27);
        *(undefined1 *)((longlong)puVar22 + -6) = *(undefined1 *)((longlong)puVar21 + -0x26);
        if (puVar21[-1] != 0) {
          puVar17 = puVar21 + -3;
          if (0xf < *puVar21) {
            puVar17 = (ulonglong *)*puVar17;
          }
          lVar8 = FUN_140407e00(&DAT_140b2f020,puVar17);
          if ((lVar8 != 0) &&
             (lVar8 = FUN_1402a87d0(*(undefined8 *)(DAT_140b15418 + 0x18),lVar8,0),
             0 < *(int *)(lVar8 + 0x10))) {
            puVar22[1] = **(ulonglong **)(lVar8 + 8);
          }
        }
        pcVar14 = pcVar28;
        do {
          cVar6 = *pcVar14;
          pcVar14[lVar30 - (longlong)pcVar28] = cVar6;
          pcVar14 = pcVar14 + 1;
        } while (cVar6 != '\0');
      }
      else {
        if (*(int *)(param_1 + 0xde8) == 0) {
          FUN_1403d3790(param_1);
          iVar7 = (int)puVar21[1];
        }
        puVar17 = puVar21 + -3;
        if (0xf < *puVar21) {
          puVar17 = (ulonglong *)*puVar17;
        }
        FUN_1402f55a0(param_1,puVar17,iVar7,*(undefined4 *)((longlong)puVar21 + 0xc),puVar21 + 2,1,
                      *(undefined1 *)((longlong)puVar21 + -0x26));
      }
    }
    lVar30 = lVar30 + 0x68;
    pcVar28 = pcVar28 + 0x78;
    puVar21 = puVar21 + 0xf;
    puVar22 = puVar22 + 0xd;
    lVar26 = lVar26 + -1;
  } while (lVar26 != 0);
  if (*(longlong *)(param_1 + 0x60) != 0) {
    plVar9 = (longlong *)(param_1 + 0x50);
    if (0xf < *(ulonglong *)(param_1 + 0x68)) {
      plVar9 = (longlong *)*plVar9;
    }
    FUN_1402fc7a0(param_1,plVar9,*(undefined1 *)(param_1 + 0x70));
  }
  plVar9 = (longlong *)FUN_140347b50(0x3f);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 1) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x40);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 2) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x41);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 4) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  plVar9 = (longlong *)FUN_140347b50(0x42);
  uVar18 = 3;
  if ((*(byte *)(param_1 + 0x21c4) & 8) == 0) {
    uVar18 = 1;
  }
  (**(code **)(*plVar9 + 0x1d0))(plVar9,uVar18);
  fVar5 = DAT_1408054e8;
  uVar23 = 0;
  if (0 < *(int *)(param_1 + 0x538)) {
    uVar20 = uVar23;
    do {
      puVar16 = (undefined8 *)0x0;
      puVar10 = (undefined8 *)FUN_140769c58(0x70);
      if (puVar10 != (undefined8 *)0x0) {
        puVar10[2] = 0;
        puVar10[4] = 0;
        puVar10[5] = 0xf;
        puVar10[6] = 0;
        puVar10[8] = 0;
        puVar10[9] = 7;
        FUN_1401f92f0(puVar10 + 0xc);
        puVar16 = puVar10;
      }
      lVar30 = *(longlong *)(param_1 + 0x530);
      uVar18 = FUN_140347b50(0x20d);
      *puVar16 = *(undefined8 *)(uVar23 + lVar30);
      puVar10 = (undefined8 *)(lVar30 + 0x10 + uVar23);
      *(undefined4 *)(puVar16 + 1) = *(undefined4 *)(uVar23 + 8 + lVar30);
      if (puVar16 + 2 != puVar10) {
        puVar25 = puVar10;
        if (0xf < (ulonglong)puVar10[3]) {
          puVar25 = (undefined8 *)*puVar10;
        }
        FUN_140022730(puVar16 + 2,puVar25,puVar10[2]);
      }
      puVar25 = (undefined8 *)(lVar30 + 0x30 + uVar23);
      puVar10 = puVar16 + 6;
      if (puVar10 != puVar25) {
        puVar19 = puVar25;
        if (7 < (ulonglong)puVar25[3]) {
          puVar19 = (undefined8 *)*puVar25;
        }
        FUN_1400228a0(puVar10,puVar19,puVar25[2]);
      }
      *(undefined4 *)(puVar16 + 10) = *(undefined4 *)(uVar23 + 0x50 + lVar30);
      *(undefined4 *)((longlong)puVar16 + 0x54) = *(undefined4 *)(uVar23 + 0x54 + lVar30);
      *(undefined4 *)(puVar16 + 0xb) = *(undefined4 *)(uVar23 + 0x58 + lVar30);
      *(undefined1 *)((longlong)puVar16 + 0x5c) = *(undefined1 *)(uVar23 + 0x5c + lVar30);
      *(undefined1 *)((longlong)puVar16 + 0x5d) = *(undefined1 *)(uVar23 + 0x5d + lVar30);
      puVar1 = (undefined4 *)(uVar23 + 0x60 + lVar30);
      uVar2 = puVar1[1];
      uVar3 = puVar1[2];
      uVar4 = puVar1[3];
      *(undefined4 *)(puVar16 + 0xc) = *puVar1;
      *(undefined4 *)((longlong)puVar16 + 100) = uVar2;
      *(undefined4 *)(puVar16 + 0xd) = uVar3;
      *(undefined4 *)((longlong)puVar16 + 0x6c) = uVar4;
      plVar9 = (longlong *)FUN_14034b350(uVar18,0,0,0);
      *puVar16 = plVar9;
      (**(code **)(*plVar9 + 0x128))
                (plVar9,(int)(*(float *)(puVar16 + 0xc) * fVar5),
                 (int)(*(float *)((longlong)puVar16 + 100) * fVar5) & 0xff,
                 (int)(*(float *)(puVar16 + 0xd) * fVar5) & 0xff,
                 (char)(int)(*(float *)((longlong)puVar16 + 0x6c) * fVar5),1);
      if ((*(char *)((longlong)puVar16 + 0x5d) == '\0') ||
         (*(float *)(puVar16 + 10) == (float)(ulonglong)puVar16[8])) {
        (**(code **)(*(longlong *)*puVar16 + 0x148))((longlong *)*puVar16,puVar10,1);
      }
      iVar7 = *(int *)(param_1 + 0x1178);
      iVar13 = *(int *)(param_1 + 0x117c) * 2 >> 1;
      if (iVar13 <= iVar7) {
        iVar7 = iVar13 / 10;
        if (iVar13 / 10 < 10) {
          iVar7 = 10;
        }
        FUN_140047aa0(param_1 + 0x1168,iVar7 + iVar13,0);
        iVar7 = *(int *)(param_1 + 0x1178);
      }
      uVar27 = (int)uVar20 + 1;
      uVar20 = (ulonglong)uVar27;
      uVar23 = uVar23 + 0x70;
      *(undefined8 **)(*(longlong *)(param_1 + 0x1170) + (longlong)iVar7 * 8) = puVar16;
      *(int *)(param_1 + 0x1178) = *(int *)(param_1 + 0x1178) + 1;
    } while ((int)uVar27 < *(int *)(param_1 + 0x538));
  }
  pppppcVar24 = (char *****)0x0;
  *(undefined4 *)(param_1 + 0x2238) = 0;
  pppppcVar15 = pppppcVar24;
  pppppcVar29 = pppppcVar24;
  if (0 < *(int *)(param_1 + 0x550)) {
    do {
      pcVar28 = (char *)((longlong)pppppcVar29 + *(longlong *)(param_1 + 0x548));
      if (*(longlong *)(pcVar28 + 8) != 0) {
        FUN_1402dd5a0(param_1 + 0x2228,pcVar28);
        lVar30 = *(longlong *)(pcVar28 + 8);
        local_88 = *(undefined4 *)(lVar30 + 0x78);
        local_84 = *(undefined4 *)(lVar30 + 0x7c);
        local_80 = *(undefined4 *)(lVar30 + 0x80);
        FUN_1402fccb0(param_1,&local_88,
                      *(longlong *)(param_1 + 0x2230) + (longlong)(int)pppppcVar15 * 0x28);
      }
      uVar27 = (int)pppppcVar15 + 1;
      pppppcVar15 = (char *****)(ulonglong)uVar27;
      pppppcVar29 = pppppcVar29 + 5;
    } while ((int)uVar27 < *(int *)(param_1 + 0x550));
  }
  lVar30 = *(longlong *)(param_1 + 0x2150);
  if (lVar30 != 0) {
    *(undefined8 *)(param_1 + 0x2150) = 0;
    FUN_1403de570(param_1,lVar30,0);
  }
  local_78[0] = (char ****)0x0;
  local_60 = 0xf;
  local_58[0] = 0;
  local_48 = 0;
  local_40 = 0xf;
  if (*(char *)(param_1 + 0x21ac) == '\0') {
    plVar9 = (longlong *)FUN_140347b50(0x46);
    uVar23 = local_60;
    if (0xb < local_60) {
      local_68 = 0xc;
      pppppcVar15 = local_78;
      if (0xf < local_60) {
        pppppcVar15 = (char *****)local_78[0];
      }
      FUN_14076bd80(pppppcVar15,"b_planet_tab",0xc);
      *(char *)((longlong)pppppcVar15 + 0xc) = '\0';
      pppppcVar24 = (char *****)local_78[0];
      goto LAB_1402fdb6f;
    }
    uVar20 = 0x7fffffffffffffff;
    if (0x7fffffffffffffff - (local_60 >> 1) < local_60) {
      uVar11 = 0x8000000000000027;
LAB_1402fdab1:
      ppppcVar12 = (char ****)FUN_140769c58(uVar11);
      if (ppppcVar12 == (char ****)0x0) goto LAB_1402fdcc3;
      pppppcVar24 = (char *****)((longlong)ppppcVar12 + 0x27U & 0xffffffffffffffe0);
      pppppcVar24[-1] = ppppcVar12;
    }
    else {
      uVar11 = local_60 + (local_60 >> 1);
      uVar20 = 0xf;
      if (0xf < uVar11) {
        uVar20 = uVar11;
      }
      if (0xfff < uVar20 + 1) {
        uVar11 = uVar20 + 0x28;
        if (uVar11 <= uVar20 + 1) goto LAB_1402fdcc9;
        goto LAB_1402fdab1;
      }
      if (uVar20 != 0xffffffffffffffff) {
        pppppcVar24 = (char *****)FUN_140769c58();
      }
    }
    local_68 = 0xc;
    *pppppcVar24 = (char ****)s_b_planet_tab_14085d660._0_8_;
    *(undefined4 *)(pppppcVar24 + 1) = s_b_planet_tab_14085d660._8_4_;
    *(char *)((longlong)pppppcVar24 + 0xc) = '\0';
    local_60 = uVar20;
LAB_1402fdb33:
    if (0xf < uVar23) {
      uVar20 = uVar23 + 1;
      pppppcVar15 = (char *****)local_78[0];
      if (0xfff < uVar20) {
        pppppcVar15 = (char *****)local_78[0][-1];
        uVar20 = uVar23 + 0x28;
        if ((char *)0x1f < (char *)((longlong)local_78[0] + (-8 - (longlong)pppppcVar15)))
        goto LAB_1402fdcc3;
      }
      thunk_FUN_1407864b8(pppppcVar15,uVar20);
    }
  }
  else {
    plVar9 = (longlong *)FUN_140347b50(0x45);
    uVar23 = local_60;
    if (local_60 < 0xb) {
      uVar20 = 0x7fffffffffffffff;
      if (0x7fffffffffffffff - (local_60 >> 1) < local_60) {
        uVar11 = 0x8000000000000027;
LAB_1402fd9d7:
        ppppcVar12 = (char ****)FUN_140769c58(uVar11);
        if (ppppcVar12 == (char ****)0x0) goto LAB_1402fdcc3;
        pppppcVar24 = (char *****)((longlong)ppppcVar12 + 0x27U & 0xffffffffffffffe0);
        pppppcVar24[-1] = ppppcVar12;
      }
      else {
        uVar11 = local_60 + (local_60 >> 1);
        uVar20 = 0xf;
        if (0xf < uVar11) {
          uVar20 = uVar11;
        }
        if (0xfff < uVar20 + 1) {
          uVar11 = uVar20 + 0x28;
          if (uVar11 <= uVar20 + 1) {
LAB_1402fdcc9:
                    /* WARNING: Subroutine does not return */
            FUN_1400225f0();
          }
          goto LAB_1402fd9d7;
        }
        if (uVar20 != 0xffffffffffffffff) {
          pppppcVar24 = (char *****)FUN_140769c58();
        }
      }
      local_68 = 0xb;
      *pppppcVar24 = (char ****)s_b_space_tab_14085d650._0_8_;
      *(undefined2 *)(pppppcVar24 + 1) = s_b_space_tab_14085d650._8_2_;
      *(char *)((longlong)pppppcVar24 + 10) = s_b_space_tab_14085d650[10];
      *(char *)((longlong)pppppcVar24 + 0xb) = '\0';
      local_60 = uVar20;
      goto LAB_1402fdb33;
    }
    local_68 = 0xb;
    pppppcVar15 = local_78;
    if (0xf < local_60) {
      pppppcVar15 = (char *****)local_78[0];
    }
    FUN_14076bd80(pppppcVar15,"b_space_tab",0xb);
    *(char *)((longlong)pppppcVar15 + 0xb) = '\0';
    pppppcVar24 = (char *****)local_78[0];
  }
LAB_1402fdb6f:
  local_78[0] = (char ****)pppppcVar24;
  cVar6 = FUN_140446220(&DAT_140b30350,1,local_78,local_58);
  if (cVar6 == '\0') {
    FUN_140446c90(&DAT_140b30350,1,local_78,local_58);
  }
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x208))(plVar9,0);
  }
  if (cVar6 == '\0') {
    FUN_1404468e0(&DAT_140b30350,1,local_78,local_58);
  }
  DAT_140b30392 = 0;
  if (*(int *)(param_1 + 0x558) == 3) {
    if (*(int *)(param_1 + 0x211c) == 1) {
      uVar18 = 0x26a;
    }
    else {
      if (*(int *)(param_1 + 0x211c) == 0) goto LAB_1402fdc29;
      uVar18 = 0x26d;
    }
    plVar9 = (longlong *)FUN_140347b50(uVar18);
    if (plVar9 != (longlong *)0x0) {
      (**(code **)(*plVar9 + 0x1d0))(plVar9,3);
      FUN_14031f870(param_1);
    }
  }
LAB_1402fdc29:
  if (0xf < local_40) {
    uVar20 = local_40 + 1;
    uVar23 = local_58[0];
    if (0xfff < uVar20) {
      uVar23 = *(ulonglong *)(local_58[0] - 8);
      uVar20 = local_40 + 0x28;
      if (0x1f < (local_58[0] - uVar23) - 8) goto LAB_1402fdcc3;
    }
    thunk_FUN_1407864b8(uVar23,uVar20);
  }
  local_48 = 0;
  local_40 = 0xf;
  local_58[0] = local_58[0] & 0xffffffffffffff00;
  if (0xf < local_60) {
    uVar23 = local_60 + 1;
    pppppcVar15 = (char *****)local_78[0];
    if (0xfff < uVar23) {
      pppppcVar15 = (char *****)local_78[0][-1];
      uVar23 = local_60 + 0x28;
      if ((char *)0x1f < (char *)((longlong)local_78[0] + (-8 - (longlong)pppppcVar15))) {
LAB_1402fdcc3:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc();
      }
    }
    thunk_FUN_1407864b8(pppppcVar15,uVar23);
  }
  return;
}




// ================================================================
// FRAME-END: FUN_140301750
// ================================================================

// ------------------------------------------------------------
// RVA: 0x301750  Name: FUN_140301750  Size: 1450 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140301750(longlong param_1,undefined4 param_2)

{
  char cVar1;
  longlong *plVar2;
  DWORD DVar3;
  undefined8 *puVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  longlong *plVar7;
  longlong *plVar8;
  longlong lVar9;
  longlong lVar10;
  ulonglong uVar11;
  uint uVar12;
  float fVar13;
  float fVar14;
  
  if ((*(int *)(param_1 + 0x211c) != 0) && (FUN_14031d750(), *(char *)(param_1 + 0x2699) == '\0')) {
    DVar3 = timeGetTime();
    fVar13 = (float)(DVar3 - _DAT_140a7bec0) / DAT_1408007f8;
    fVar14 = 0.0;
    if (0.0 <= fVar13 - DAT_140b2bc80) {
      fVar14 = fVar13 - DAT_140b2bc80;
    }
    plVar7 = (longlong *)**(longlong **)(param_1 + 0x640);
    DAT_140b2bc80 = fVar13;
    if (plVar7 != *(longlong **)(param_1 + 0x640)) {
      do {
        (**(code **)(*(longlong *)plVar7[5] + 0x58))((longlong *)plVar7[5],fVar14);
        plVar8 = (longlong *)plVar7[2];
        if (*(char *)((longlong)plVar8 + 0x19) == '\0') {
          cVar1 = *(char *)(*plVar8 + 0x19);
          plVar7 = plVar8;
          plVar8 = (longlong *)*plVar8;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*plVar8 + 0x19);
            plVar7 = plVar8;
            plVar8 = (longlong *)*plVar8;
          }
        }
        else {
          cVar1 = *(char *)(plVar7[1] + 0x19);
          plVar2 = (longlong *)plVar7[1];
          plVar8 = plVar7;
          while ((plVar7 = plVar2, cVar1 == '\0' && (plVar8 == (longlong *)plVar7[2]))) {
            cVar1 = *(char *)(plVar7[1] + 0x19);
            plVar2 = (longlong *)plVar7[1];
            plVar8 = plVar7;
          }
        }
      } while (plVar7 != *(longlong **)(param_1 + 0x640));
    }
    uVar6 = 0;
    uVar5 = uVar6;
    uVar11 = uVar6;
    if (0 < *(int *)(param_1 + 0xde8)) {
      do {
        lVar10 = *(longlong *)(param_1 + 0xde0);
        lVar9 = 3;
        puVar4 = (undefined8 *)(lVar10 + 0x20 + uVar5);
        do {
          (**(code **)(*(longlong *)*puVar4 + 0x58))((longlong *)*puVar4,param_2);
          puVar4 = puVar4 + 1;
          lVar9 = lVar9 + -1;
        } while (lVar9 != 0);
        plVar7 = *(longlong **)(lVar10 + 0xe8 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x118 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0xf0 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0xf8 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x100 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x108 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x110 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x130 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x90 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0xe0 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x58 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x88 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x48 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x50 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x138 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x140 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x158 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x160 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        plVar7 = *(longlong **)(lVar10 + 0x168 + uVar5);
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        lVar9 = 2;
        puVar4 = (undefined8 *)(lVar10 + 0x148 + uVar5);
        do {
          (**(code **)(*(longlong *)*puVar4 + 0x58))((longlong *)*puVar4,param_2);
          puVar4 = puVar4 + 1;
          lVar9 = lVar9 + -1;
        } while (lVar9 != 0);
        uVar12 = (int)uVar11 + 1;
        uVar5 = uVar5 + 0x188;
        uVar11 = (ulonglong)uVar12;
      } while ((int)uVar12 < *(int *)(param_1 + 0xde8));
    }
    plVar7 = (longlong *)**(longlong **)(param_1 + 0x1ff8);
    if (plVar7 != *(longlong **)(param_1 + 0x1ff8)) {
      do {
        plVar8 = (longlong *)plVar7[5];
        plVar2 = (longlong *)*plVar8;
        if (plVar2 != (longlong *)0x0) {
          (**(code **)(*plVar2 + 0x58))(plVar2,param_2);
        }
        plVar8 = (longlong *)plVar8[1];
        if (plVar8 != (longlong *)0x0) {
          (**(code **)(*plVar8 + 0x58))(plVar8,param_2);
        }
        plVar8 = (longlong *)plVar7[2];
        if (*(char *)((longlong)plVar8 + 0x19) == '\0') {
          cVar1 = *(char *)(*plVar8 + 0x19);
          plVar7 = plVar8;
          plVar8 = (longlong *)*plVar8;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*plVar8 + 0x19);
            plVar7 = plVar8;
            plVar8 = (longlong *)*plVar8;
          }
        }
        else {
          cVar1 = *(char *)(plVar7[1] + 0x19);
          plVar2 = (longlong *)plVar7[1];
          plVar8 = plVar7;
          while ((plVar7 = plVar2, cVar1 == '\0' && (plVar8 == (longlong *)plVar7[2]))) {
            cVar1 = *(char *)(plVar7[1] + 0x19);
            plVar2 = (longlong *)plVar7[1];
            plVar8 = plVar7;
          }
        }
      } while (plVar7 != *(longlong **)(param_1 + 0x1ff8));
    }
    plVar7 = (longlong *)**(longlong **)(param_1 + 0x2008);
    if (plVar7 != *(longlong **)(param_1 + 0x2008)) {
      do {
        plVar8 = (longlong *)plVar7[5];
        plVar2 = (longlong *)*plVar8;
        if (plVar2 != (longlong *)0x0) {
          (**(code **)(*plVar2 + 0x58))(plVar2,param_2);
        }
        plVar8 = (longlong *)plVar8[1];
        if (plVar8 != (longlong *)0x0) {
          (**(code **)(*plVar8 + 0x58))(plVar8,param_2);
        }
        plVar8 = (longlong *)plVar7[2];
        if (*(char *)((longlong)plVar8 + 0x19) == '\0') {
          cVar1 = *(char *)(*plVar8 + 0x19);
          plVar7 = plVar8;
          plVar8 = (longlong *)*plVar8;
          while (cVar1 == '\0') {
            cVar1 = *(char *)(*plVar8 + 0x19);
            plVar7 = plVar8;
            plVar8 = (longlong *)*plVar8;
          }
        }
        else {
          cVar1 = *(char *)(plVar7[1] + 0x19);
          plVar2 = (longlong *)plVar7[1];
          plVar8 = plVar7;
          while ((plVar7 = plVar2, cVar1 == '\0' && (plVar8 == (longlong *)plVar7[2]))) {
            cVar1 = *(char *)(plVar7[1] + 0x19);
            plVar2 = (longlong *)plVar7[1];
            plVar8 = plVar7;
          }
        }
      } while (plVar7 != *(longlong **)(param_1 + 0x2008));
    }
    plVar7 = (longlong *)(param_1 + 0x21c8);
    lVar10 = 10;
    do {
      plVar8 = (longlong *)*plVar7;
      if (plVar8 != (longlong *)0x0) {
        (**(code **)(*plVar8 + 0x58))(plVar8,param_2);
      }
      plVar7 = plVar7 + 1;
      lVar10 = lVar10 + -1;
    } while (lVar10 != 0);
    plVar7 = *(longlong **)(param_1 + 0x20d0);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
    }
    plVar7 = *(longlong **)(param_1 + 0x20d8);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
    }
    plVar7 = *(longlong **)(param_1 + 0xfb8);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x38))(plVar7,param_2);
    }
    plVar7 = *(longlong **)(param_1 + 0xfc8);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x38))(plVar7,param_2);
    }
    plVar7 = *(longlong **)(param_1 + 0xfd8);
    if (plVar7 != (longlong *)0x0) {
      (**(code **)(*plVar7 + 0x38))(plVar7,param_2);
    }
    uVar5 = uVar6;
    uVar11 = uVar6;
    if (0 < *(int *)(param_1 + 0x1140)) {
      do {
        plVar7 = *(longlong **)(uVar11 + *(longlong *)(param_1 + 0x1138));
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        uVar12 = (int)uVar5 + 1;
        uVar5 = (ulonglong)uVar12;
        uVar11 = uVar11 + 8;
      } while ((int)uVar12 < *(int *)(param_1 + 0x1140));
    }
    uVar5 = uVar6;
    if (0 < *(int *)(param_1 + 0x1158)) {
      do {
        plVar7 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x1150));
        (**(code **)(*plVar7 + 0x58))(plVar7,param_2);
        uVar12 = (int)uVar5 + 1;
        uVar6 = uVar6 + 8;
        uVar5 = (ulonglong)uVar12;
      } while ((int)uVar12 < *(int *)(param_1 + 0x1158));
    }
    FUN_140358700(&DAT_140b2bcb0,param_2);
    FUN_140315c40(param_1,param_2);
    FUN_1403e7840(param_1,param_2,0);
    plVar7 = (longlong *)**(longlong **)(param_1 + 11000);
    if (plVar7 != *(longlong **)(param_1 + 11000)) {
      do {
        plVar8 = (longlong *)plVar7[9];
        if (plVar8 == (longlong *)0x0) {
          plVar8 = (longlong *)*plVar7;
          *(longlong **)plVar7[1] = plVar8;
          *(longlong *)(*plVar7 + 8) = plVar7[1];
          *(longlong *)(param_1 + 0x2b00) = *(longlong *)(param_1 + 0x2b00) + -1;
          FID_conflict__Tidy_deallocate(plVar7 + 2);
          thunk_FUN_1407864b8(plVar7,0x50);
        }
        else {
          (**(code **)(*plVar8 + 0x58))(plVar8,param_2);
          plVar8 = (longlong *)*plVar7;
        }
        plVar7 = plVar8;
      } while (plVar8 != *(longlong **)(param_1 + 11000));
    }
  }
  return;
}




// ================================================================
// FRAME-END: FUN_14025ca30
// ================================================================

// ------------------------------------------------------------
// RVA: 0x25ca30  Name: FUN_14025ca30  Size: 396 bytes
// ------------------------------------------------------------

void FUN_14025ca30(longlong param_1,float param_2)

{
  longlong *plVar1;
  float fVar2;
  uint uVar3;
  ulonglong uVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  float fVar7;
  
  fVar2 = DAT_14081bd5c;
  fVar7 = param_2 + *(float *)(param_1 + 8);
  *(float *)(param_1 + 8) = fVar7;
  if (fVar2 < fVar7) {
    *(float *)(param_1 + 8) = fVar7 - fVar2;
  }
  uVar5 = 0;
  uVar4 = uVar5;
  uVar6 = uVar5;
  if (0 < *(int *)(param_1 + 0x20)) {
    do {
      plVar1 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x18));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
      uVar6 = uVar6 + 8;
    } while ((int)uVar3 < *(int *)(param_1 + 0x20));
  }
  uVar4 = uVar5;
  uVar6 = uVar5;
  if (0 < *(int *)(param_1 + 0x38)) {
    do {
      plVar1 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x30));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
      uVar6 = uVar6 + 8;
    } while ((int)uVar3 < *(int *)(param_1 + 0x38));
  }
  uVar4 = uVar5;
  uVar6 = uVar5;
  if (0 < *(int *)(param_1 + 0x50)) {
    do {
      plVar1 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x48));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
      uVar6 = uVar6 + 8;
    } while ((int)uVar3 < *(int *)(param_1 + 0x50));
  }
  uVar4 = uVar5;
  uVar6 = uVar5;
  if (0 < *(int *)(param_1 + 0x68)) {
    do {
      plVar1 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x60));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
      uVar6 = uVar6 + 8;
    } while ((int)uVar3 < *(int *)(param_1 + 0x68));
  }
  uVar4 = uVar5;
  uVar6 = uVar5;
  if (0 < *(int *)(param_1 + 0x80)) {
    do {
      plVar1 = *(longlong **)(uVar6 + *(longlong *)(param_1 + 0x78));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar4 = (ulonglong)uVar3;
      uVar6 = uVar6 + 8;
    } while ((int)uVar3 < *(int *)(param_1 + 0x80));
  }
  uVar4 = uVar5;
  if (0 < *(int *)(param_1 + 0x98)) {
    do {
      plVar1 = *(longlong **)(uVar5 + *(longlong *)(param_1 + 0x90));
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x50))(plVar1,param_2);
      }
      uVar3 = (int)uVar4 + 1;
      uVar5 = uVar5 + 8;
      uVar4 = (ulonglong)uVar3;
    } while ((int)uVar3 < *(int *)(param_1 + 0x98));
  }
  return;
}




// ================================================================
// COMMAND QUEUE SUB (FUN_1403b03e0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b03e0  Name: FUN_1403b03e0  Size: 940 bytes
// ------------------------------------------------------------

void FUN_1403b03e0(longlong param_1)

{
  int iVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  char cVar5;
  undefined4 uVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  char cVar11;
  undefined8 *puVar12;
  int *piVar13;
  int *piVar14;
  bool bVar15;
  int local_res10 [2];
  int local_res18 [2];
  int local_res20 [2];
  int local_68 [2];
  undefined8 *local_60;
  undefined8 *local_58;
  undefined8 *local_50;
  longlong local_48;
  
  puVar10 = (undefined8 *)0x0;
  local_50 = (undefined8 *)0x0;
  local_60 = (undefined8 *)0x0;
  local_res10[0] = 0;
  local_res18[0] = 0;
  cVar11 = '\0';
  local_res20[0] = 0;
  local_68[0] = 0;
  lVar7 = FUN_140294a40(&DAT_140a16fd0);
  if (lVar7 == 0) {
    return;
  }
  DAT_140a7c478 = 1;
  local_48 = lVar7;
  local_58 = (undefined8 *)FUN_1402093c0(&DAT_140a14b80,0);
  puVar12 = puVar10;
  if (0 < *(int *)(param_1 + 0x18)) {
LAB_1403b0470:
    plVar3 = *(longlong **)(param_1 + 8);
    if (plVar3 != (longlong *)0x0) {
      plVar8 = plVar3;
      if (plVar3[2] != 0) {
        *(undefined8 *)(plVar3[2] + 0x18) = 0;
        plVar8 = *(longlong **)(param_1 + 8);
      }
      lVar4 = plVar8[2];
      *(longlong *)(param_1 + 8) = lVar4;
      if (lVar4 == 0) {
        *(undefined8 *)(param_1 + 0x10) = 0;
      }
      plVar3[2] = 0;
      plVar3[3] = 0;
      *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + -1;
      cVar5 = (**(code **)(*plVar3 + 0x38))(plVar3);
      if (cVar5 != '\0') {
        uVar6 = FUN_14027cf30(&DAT_140b0a320);
        *(undefined4 *)(plVar3 + 5) = uVar6;
      }
    }
    if (*(int *)((longlong)plVar3 + 0x24) == *(int *)(lVar7 + 0x4c)) {
LAB_1403b04f5:
      cVar5 = (**(code **)(*plVar3 + 0x38))(plVar3);
      bVar15 = cVar5 != '\0';
      if (cVar5 == '\0') {
        piVar13 = local_res18;
        piVar14 = local_68;
        puVar9 = puVar10;
      }
      else {
        piVar13 = local_res10;
        piVar14 = local_res20;
        puVar9 = puVar12;
      }
      puVar10 = local_50;
      if (puVar9 == (undefined8 *)0x0) goto code_r0x0001403b052f;
      goto LAB_1403b058b;
    }
    FUN_1404b3a80(&DAT_140b15690);
    goto LAB_1403b06aa;
  }
LAB_1403b0770:
  FUN_140209540(&DAT_140a14b80,local_58);
  DAT_140a7c478 = 0;
  return;
code_r0x0001403b052f:
  puVar9 = (undefined8 *)FUN_1402093c0(&DAT_140a14b80,0);
  puVar10 = local_60;
  if (puVar9 == (undefined8 *)0x0) goto LAB_1403b04f5;
  (**(code **)*puVar9)(puVar9);
  FUN_1402075b0(&DAT_140b31738,puVar9);
  *piVar14 = *(int *)(puVar9 + 2);
  *piVar13 = 0;
  FUN_1402046f0(puVar9,0,5);
  puVar10 = puVar9;
  if (!bVar15) {
    cVar11 = '\0';
    local_60 = puVar9;
    puVar10 = local_50;
  }
LAB_1403b058b:
  local_50 = puVar10;
  puVar10 = local_58;
  (**(code **)*local_58)(local_58);
  iVar1 = *(int *)(puVar10 + 2);
  (**(code **)(*plVar3 + 0x28))(plVar3,local_58);
  cVar5 = FUN_1402051e0(puVar9,(*(int *)(local_58 + 2) - iVar1) + 5);
  if (cVar5 == '\0') {
    iVar1 = *piVar13;
    if (puVar9 != (undefined8 *)0x0) {
      iVar2 = *piVar14;
      if (iVar2 == 0) {
        FUN_1402046f0(puVar9,iVar1,5);
      }
      else {
        FUN_140205260(puVar9);
        *(int *)(puVar9 + 2) = iVar2;
        FUN_1402046f0(puVar9,iVar1,5);
        FUN_140205280(puVar9);
      }
    }
    if ((bVar15) || (cVar11 != '\0')) {
      FUN_1403b07a0(param_1,puVar9,1);
      cVar11 = '\0';
      if (bVar15) {
        puVar12 = (undefined8 *)0x0;
        local_50 = (undefined8 *)0x0;
        puVar10 = local_60;
        goto LAB_1403b04f5;
      }
    }
    else {
      FUN_1403b07a0(param_1,puVar9,0);
    }
    local_60 = (undefined8 *)0x0;
    cVar11 = '\0';
    puVar10 = (undefined8 *)0x0;
    puVar12 = local_50;
    goto LAB_1403b04f5;
  }
  (**(code **)(*plVar3 + 0x28))(plVar3,puVar9);
  *piVar13 = *piVar13 + 1;
  FUN_1404b3a80(&DAT_140b15690);
  if ((!bVar15) && (cVar5 = (**(code **)(*plVar3 + 0x40))(plVar3), cVar5 != '\0')) {
    cVar11 = '\x01';
  }
  (**(code **)(*plVar3 + 8))(plVar3);
  puVar10 = local_60;
  puVar12 = local_50;
  lVar7 = local_48;
LAB_1403b06aa:
  iVar1 = local_res20[0];
  if (*(int *)(param_1 + 0x18) < 1) goto code_r0x0001403b06b5;
  goto LAB_1403b0470;
code_r0x0001403b06b5:
  if (puVar12 != (undefined8 *)0x0) {
    if (local_res20[0] == 0) {
      FUN_1402046f0(puVar12,local_res10[0],5);
    }
    else {
      FUN_140205260(puVar12);
      *(int *)(puVar12 + 2) = iVar1;
      FUN_1402046f0(puVar12,local_res10[0],5);
      FUN_140205280(puVar12);
    }
    FUN_1403b07a0(param_1,puVar12,1);
  }
  iVar1 = local_68[0];
  if (puVar10 != (undefined8 *)0x0) {
    if (local_68[0] == 0) {
      FUN_1402046f0(puVar10,local_res18[0],5);
    }
    else {
      FUN_140205260(puVar10);
      *(int *)(puVar10 + 2) = iVar1;
      FUN_1402046f0(puVar10,local_res18[0],5);
      FUN_140205280(puVar10);
    }
    FUN_1403b07a0(param_1,puVar10,cVar11);
  }
  goto LAB_1403b0770;
}




// ================================================================
// PLAYER REGISTRY SUB-TICK (FUN_140207e50)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x207e50  Name: FUN_140207e50  Size: 125 bytes
// ------------------------------------------------------------

void FUN_140207e50(longlong param_1)

{
  uint uVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  
  uVar2 = 0;
  if ((*(longlong *)(param_1 + 0x30) != 0) && (FUN_140209ac0(), **(int **)(param_1 + 0x30) == 4)) {
    FUN_140047170(param_1,param_1 + 0x30);
    *(undefined8 *)(param_1 + 0x30) = 0;
    FUN_140207d40(param_1);
  }
  uVar3 = uVar2;
  if (0 < *(int *)(param_1 + 0x10)) {
    do {
      if (*(longlong *)(uVar3 + *(longlong *)(param_1 + 8)) != 0) {
        FUN_140209ac0();
      }
      uVar1 = (int)uVar2 + 1;
      uVar2 = (ulonglong)uVar1;
      uVar3 = uVar3 + 8;
    } while ((int)uVar1 < *(int *)(param_1 + 0x10));
  }
  return;
}




// ================================================================
// SCENE UNCONDITIONAL SUB-CALL — FUN_140305c40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x305c40  Name: FUN_140305c40  Size: 875 bytes
// ------------------------------------------------------------

void FUN_140305c40(longlong param_1)

{
  longlong *plVar1;
  byte bVar2;
  byte bVar3;
  ulonglong uVar4;
  char cVar5;
  int iVar6;
  longlong *plVar7;
  undefined8 uVar8;
  longlong lVar9;
  uint uVar10;
  byte *pbVar11;
  byte *pbVar12;
  uint uVar13;
  undefined1 local_18 [16];
  
  if (DAT_140b15418 == (longlong *)0x0) {
LAB_140305ddb:
    cVar5 = *(char *)(param_1 + 0x26b1);
    if ((cVar5 == '\0') &&
       (((*(char *)(param_1 + 0x26b2) != '\0' || (*(char *)(param_1 + 0x26b3) != '\0')) &&
        (DAT_140b30690 == 0)))) {
      FUN_1402fc7a0(param_1,"Light_Loop",1);
      return;
    }
    if ((*(char *)(param_1 + 0x26b2) == '\0') && (*(char *)(param_1 + 0x26b3) == '\0')) {
      if (cVar5 == '\0') {
        return;
      }
      plVar1 = (longlong *)(param_1 + 0x26b8);
      uVar4 = *(ulonglong *)(param_1 + 0x26d0);
      plVar7 = plVar1;
      if (0xf < uVar4) {
        plVar7 = (longlong *)*plVar1;
      }
      if (((*(longlong *)(param_1 + 0x26c8) == 10) && (*plVar7 == 0x6f4c5f746867694c)) &&
         ((short)plVar7[1] == 0x706f)) {
        pbVar12 = (byte *)&DAT_140a28ca0;
        if (0xf < DAT_140a28cb8) {
          pbVar12 = DAT_140a28ca0;
        }
        iVar6 = strcmp((char *)pbVar12,"Light_Loop");
        if (iVar6 != 0) {
          plVar7 = plVar1;
          if (0xf < uVar4) {
            plVar7 = (longlong *)*plVar1;
          }
          pbVar12 = (byte *)&DAT_140a28ca0;
          if (0xf < DAT_140a28cb8) {
            pbVar12 = DAT_140a28ca0;
          }
          lVar9 = (longlong)plVar7 - (longlong)pbVar12;
          do {
            bVar2 = *pbVar12;
            bVar3 = pbVar12[lVar9];
            if (bVar2 != bVar3) break;
            pbVar12 = pbVar12 + 1;
          } while (bVar3 != 0);
          if (bVar2 == bVar3) {
            return;
          }
          *(undefined1 *)(param_1 + 0x26b1) = 0;
          FUN_140201910(plVar1,0,0);
          return;
        }
        goto LAB_140305cc2;
      }
    }
    else if (cVar5 == '\0') {
      return;
    }
    pbVar12 = (byte *)&DAT_140a28ca0;
    if (0xf < DAT_140a28cb8) {
      pbVar12 = DAT_140a28ca0;
    }
    iVar6 = strcmp((char *)pbVar12,"Light_Loop");
    if (iVar6 == 0) {
      bVar2 = *(byte *)(param_1 + 0x2a30);
      uVar13 = (uint)(*(float *)(param_1 + 0x5c0) * DAT_1408054e8);
      uVar8 = FUN_1401ce750(*(undefined8 *)(param_1 + 0xdd0));
      FUN_1401f9320(local_18,uVar8);
      uVar10 = uVar13 & 0xff;
      if ((byte)uVar13 <= bVar2) {
        uVar10 = (uint)bVar2;
      }
      FUN_1401f9590(local_18,uVar10);
      (**(code **)(**(longlong **)(param_1 + 0xdd0) + 0x28))
                (*(longlong **)(param_1 + 0xdd0),local_18);
      FUN_1401f9440(local_18);
    }
  }
  else {
    cVar5 = (**(code **)(*DAT_140b15418 + 0xf8))();
    uVar4 = DAT_140a28cb8;
    pbVar12 = DAT_140a28ca0;
    if ((cVar5 == '\0') || (*(char *)(param_1 + 0x26a4) != '\0')) {
      cVar5 = (**(code **)(*DAT_140b15418 + 0xf0))();
      uVar4 = DAT_140a28cb8;
      pbVar12 = DAT_140a28ca0;
      if ((cVar5 == '\0') || (*(int *)(param_1 + 0x26a0) < 1)) goto LAB_140305ddb;
      if (*(char *)(param_1 + 0x26b1) == '\0') {
        return;
      }
      pbVar11 = (byte *)&DAT_140a28ca0;
      if (0xf < DAT_140a28cb8) {
        pbVar11 = DAT_140a28ca0;
      }
      iVar6 = strcmp((char *)pbVar11,"Light_Loop");
      if (iVar6 != 0) {
        lVar9 = param_1 + 0x26b8;
        if (0xf < *(ulonglong *)(param_1 + 0x26d0)) {
          lVar9 = *(longlong *)(param_1 + 0x26b8);
        }
        pbVar11 = (byte *)&DAT_140a28ca0;
        if (0xf < uVar4) {
          pbVar11 = pbVar12;
        }
        lVar9 = lVar9 - (longlong)pbVar11;
        do {
          pbVar12 = pbVar11 + lVar9;
          iVar6 = (uint)*pbVar11 - (uint)*pbVar12;
          if (iVar6 != 0) break;
          pbVar11 = pbVar11 + 1;
        } while (*pbVar12 != 0);
        goto LAB_140305d04;
      }
    }
    else {
      if (*(char *)(param_1 + 0x26b1) == '\0') {
        return;
      }
      pbVar11 = (byte *)&DAT_140a28ca0;
      if (0xf < DAT_140a28cb8) {
        pbVar11 = DAT_140a28ca0;
      }
      iVar6 = strcmp((char *)pbVar11,"Light_Loop");
      if (iVar6 != 0) {
        lVar9 = param_1 + 0x26b8;
        if (0xf < *(ulonglong *)(param_1 + 0x26d0)) {
          lVar9 = *(longlong *)(param_1 + 0x26b8);
        }
        pbVar11 = (byte *)&DAT_140a28ca0;
        if (0xf < uVar4) {
          pbVar11 = pbVar12;
        }
        lVar9 = lVar9 - (longlong)pbVar11;
        do {
          pbVar12 = pbVar11 + lVar9;
          iVar6 = (uint)*pbVar11 - (uint)*pbVar12;
          if (iVar6 != 0) break;
          pbVar11 = pbVar11 + 1;
        } while (*pbVar12 != 0);
LAB_140305d04:
        if (iVar6 == 0) {
          return;
        }
        *(undefined1 *)(param_1 + 0x26b1) = 0;
        FUN_140201910(param_1 + 0x26b8,0,0);
        return;
      }
    }
LAB_140305cc2:
    FUN_14030ac40(param_1);
  }
  return;
}



