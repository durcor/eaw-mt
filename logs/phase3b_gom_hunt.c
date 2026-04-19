
// ================================================================
// SCENE MANAGER UNCONDITIONAL SUB-CALL (FUN_140305c40)
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




// ================================================================
// SCENE MANAGER FLAG-GATED SUB-CALL (FUN_1402fd330)
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
// FULL DECOMPILE OF FUN_14021caf0 (audio / GOM?)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x21caf0  Name: FUN_14021caf0  Size: 539 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14021caf0(char *param_1,int param_2,undefined4 *param_3,undefined4 *param_4,
                  undefined4 *param_5)

{
  undefined4 uVar1;
  char cVar2;
  
  if (((*param_1 != '\0') && (*(longlong *)(param_1 + 0xff0) != 0)) && (param_1[8] != '\0')) {
    if (*(int *)(param_1 + 4) < 1) {
      *(undefined4 *)(param_1 + 0x1000) = *param_3;
      *(undefined4 *)(param_1 + 0x1004) = param_3[1];
      *(undefined4 *)(param_1 + 0x1008) = param_3[2];
      *(undefined4 *)(param_1 + 0x100c) = *param_4;
      *(undefined4 *)(param_1 + 0x1010) = param_4[1];
      *(undefined4 *)(param_1 + 0x1014) = param_4[2];
      *(undefined4 *)(param_1 + 0x1018) = *param_5;
      *(undefined4 *)(param_1 + 0x101c) = param_5[1];
      *(undefined4 *)(param_1 + 0x1020) = param_5[2];
      AIL_set_listener_3D_position(*(undefined8 *)(param_1 + 0xff8),*param_3,param_3[1],param_3[2]);
      AIL_set_listener_3D_orientation
                (*(undefined8 *)(param_1 + 0xff8),*param_4,param_4[1],param_4[2],*param_5,param_5[1]
                 ,param_5[2]);
      FUN_140227f20(param_1 + 0x68,param_2);
      FUN_140229da0(param_1 + 0x490,param_2);
      FUN_14022bc20(param_1 + 0x10,param_2);
      FUN_14022c550(param_1 + 0xf40,param_2);
      FUN_14021d2f0(param_1,0,param_2);
      uVar1 = *(undefined4 *)(param_1 + 4000);
      cVar2 = FUN_14021d2f0(param_1,1,param_2);
      if (cVar2 == '\x01') {
        FUN_1402286e0(param_1 + 0x68,*(undefined4 *)(param_1 + 0xfb4),uVar1);
        FUN_14022a140(param_1 + 0x490,*(undefined4 *)(param_1 + 0xfb4),uVar1);
      }
      cVar2 = FUN_14021d2f0(param_1,2,param_2);
      if (cVar2 == '\x01') {
        FUN_14022c990(param_1 + 0xf40,*(undefined4 *)(param_1 + 0xfc8),uVar1);
      }
      cVar2 = FUN_14021d2f0(param_1,3,param_2);
      if (cVar2 == '\x01') {
        FUN_14022bf90(param_1 + 0x10,*(undefined4 *)(param_1 + 0xfdc),uVar1);
      }
      if (0x7d < (uint)(param_2 - _DAT_140a7e0d0)) {
        AIL_serve();
        _DAT_140a7e0d0 = param_2;
      }
    }
    *(int *)(param_1 + 0xc) = param_2;
  }
  return;
}




// ================================================================
// FUN_1402d72c0 SUB-CALLS
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2d7450  Name: FUN_1402d7450  Size: 1520 bytes
// ------------------------------------------------------------

void FUN_1402d7450(longlong param_1)

{
  longlong *plVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  char cVar5;
  undefined4 *puVar6;
  int iVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined1 local_b8 [12];
  undefined1 local_ac [12];
  undefined1 local_a0 [136];
  
  fVar4 = DAT_140800908;
  fVar3 = DAT_1408007f0;
  fVar2 = DAT_1407ffaf8;
  uVar8 = 0;
  uVar10 = uVar8;
  if (0 < *(int *)(param_1 + 0x78)) {
    do {
      plVar1 = *(longlong **)(uVar10 + *(longlong *)(param_1 + 0x70));
      if ((plVar1 != (longlong *)0x0) && (*plVar1 != 0)) {
        uVar9 = *(uint *)(plVar1 + 1);
        fVar13 = fVar2;
        if ((uVar9 - 2 < 2) && (DAT_140b155c4 == '\x01')) {
          fVar13 = 0.0;
        }
        iVar7 = (int)plVar1[6];
        fVar11 = fVar2;
        if (iVar7 == 4) {
          if (uVar9 < 2) {
            cVar5 = FUN_140228bf0(&DAT_140a7d108,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_140228ac0(&DAT_140a7d108,(int)plVar1[7]);
              iVar7 = 5;
LAB_1402d77e6:
              *(undefined4 *)(plVar1 + 7) = 0xffffffff;
LAB_1402d77ed:
              *(undefined4 *)((longlong)plVar1 + 0x2c) = 0;
              *(undefined4 *)(plVar1 + 8) = 0;
            }
          }
          else if (uVar9 - 2 < 2) {
            cVar5 = FUN_14022ac10(&DAT_140a7d530,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_14022a9d0(&DAT_140a7d530,*(undefined4 *)((longlong)plVar1 + 0x3c));
              *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
              iVar7 = 5;
              goto LAB_1402d77ed;
            }
            local_c8 = (undefined4)plVar1[3];
            local_c4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
            local_c0 = (undefined4)plVar1[4];
            if (plVar1[2] != 0) {
              puVar6 = (undefined4 *)FUN_140398440(plVar1[2],local_a0);
              local_c8 = *puVar6;
              local_c4 = puVar6[1];
              local_c0 = puVar6[2];
              fVar11 = (float)FUN_1402d4b00(param_1,plVar1);
            }
            FUN_14022ab90(&DAT_140a7d530,(int)plVar1[8],&local_c8);
          }
        }
        else if (iVar7 == 6) {
          if (uVar9 < 2) {
            cVar5 = FUN_140228bf0(&DAT_140a7d108,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_140228ac0(&DAT_140a7d108,(int)plVar1[7]);
              iVar7 = 7;
              goto LAB_1402d77e6;
            }
          }
          else if (uVar9 - 2 < 2) {
            cVar5 = FUN_14022ac10(&DAT_140a7d530,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_14022a9d0(&DAT_140a7d530,*(undefined4 *)((longlong)plVar1 + 0x3c));
              *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
              iVar7 = 7;
              goto LAB_1402d77ed;
            }
            local_d8 = (undefined4)plVar1[3];
            local_d4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
            local_d0 = (undefined4)plVar1[4];
            if (plVar1[2] != 0) {
              puVar6 = (undefined4 *)FUN_140398440(plVar1[2],local_ac);
              local_d8 = *puVar6;
              local_d4 = puVar6[1];
              local_d0 = puVar6[2];
              fVar11 = (float)FUN_1402d4b00(param_1,plVar1);
            }
            FUN_14022ab90(&DAT_140a7d530,(int)plVar1[8],&local_d8);
          }
        }
        else if (iVar7 == 8) {
          if (uVar9 < 2) {
            cVar5 = FUN_140228bf0(&DAT_140a7d108,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_140228ac0(&DAT_140a7d108,(int)plVar1[7]);
              iVar7 = 9;
              goto LAB_1402d77e6;
            }
          }
          else if (uVar9 - 2 < 2) {
            cVar5 = FUN_14022ac10(&DAT_140a7d530,(int)plVar1[8]);
            if (cVar5 == '\0') {
              FUN_14022a9d0(&DAT_140a7d530,*(undefined4 *)((longlong)plVar1 + 0x3c));
              *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
              iVar7 = 9;
              goto LAB_1402d77ed;
            }
            local_e8 = (undefined4)plVar1[3];
            local_e4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
            local_e0 = (undefined4)plVar1[4];
            if (plVar1[2] != 0) {
              puVar6 = (undefined4 *)FUN_140398440(plVar1[2],local_b8);
              local_e8 = *puVar6;
              local_e4 = puVar6[1];
              local_e0 = puVar6[2];
              fVar11 = (float)FUN_1402d4b00(param_1,plVar1);
            }
            FUN_14022ab90(&DAT_140a7d530,(int)plVar1[8],&local_e8);
          }
        }
        if (*(char *)((longlong)plVar1 + 100) == '\x01') {
          fVar12 = *(float *)(plVar1 + 0xd) - *(float *)((longlong)plVar1 + 0x6c);
          *(float *)(plVar1 + 0xd) = fVar12;
          if (0.0 < fVar12) {
            fVar13 = ((float)(int)plVar1[10] / fVar3) * fVar12 * fVar11 * fVar13;
            if ((int)plVar1[1] - 2U < 2) {
              fVar13 = fVar13 * *(float *)(param_1 + 0x98);
            }
            cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
            if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')) &&
               (fVar4 <= fVar13)) {
              fVar13 = fVar4;
            }
            if ((int)plVar1[7] == -1) {
              if ((*(int *)((longlong)plVar1 + 0x3c) != -1) &&
                 (cVar5 = FUN_14022ac10(&DAT_140a7d530,(int)plVar1[8]), cVar5 == '\x01')) {
                FUN_14022ab60(&DAT_140a7d530,*(undefined4 *)((longlong)plVar1 + 0x3c),fVar13);
              }
            }
            else {
LAB_1402d799d:
              cVar5 = FUN_140228bf0(&DAT_140a7d108,(int)plVar1[8]);
              if (cVar5 == '\x01') {
                FUN_140228bd0(&DAT_140a7d108,(int)plVar1[7],fVar13);
              }
            }
          }
          else {
            iVar7 = 0;
            if ((int)plVar1[7] == -1) {
              if (*(int *)((longlong)plVar1 + 0x3c) != -1) {
                FUN_14022a9d0(&DAT_140a7d530);
                *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
              }
              *(undefined1 *)((longlong)plVar1 + 100) = 0;
              *(undefined4 *)(plVar1 + 8) = 0;
            }
            else {
              FUN_140228ac0(&DAT_140a7d108);
              *(undefined4 *)(plVar1 + 7) = 0xffffffff;
              *(undefined1 *)((longlong)plVar1 + 100) = 0;
              *(undefined4 *)(plVar1 + 8) = 0;
            }
          }
        }
        else if (*(char *)((longlong)plVar1 + 100) == '\0') {
          fVar13 = ((float)(int)plVar1[10] / fVar3) * fVar11 * fVar13;
          if ((int)plVar1[1] - 2U < 2) {
            fVar13 = fVar13 * *(float *)(param_1 + 0x98);
          }
          cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
          if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')) &&
             (fVar4 <= fVar13)) {
            fVar13 = fVar4;
          }
          if (((plVar1[2] != 0) && ((int)plVar1[1] == 3)) &&
             ((*(int *)((longlong)plVar1 + 0x3c) != -1 &&
              (cVar5 = FUN_14022ac10(&DAT_140a7d530,(int)plVar1[8]), cVar5 == '\x01')))) {
            FUN_14022ab60(&DAT_140a7d530,*(undefined4 *)((longlong)plVar1 + 0x3c),fVar13);
          }
          if ((int)plVar1[7] != -1) goto LAB_1402d799d;
        }
        *(int *)(plVar1 + 6) = iVar7;
      }
      uVar9 = (int)uVar8 + 1;
      uVar8 = (ulonglong)uVar9;
      uVar10 = uVar10 + 8;
    } while ((int)uVar9 < *(int *)(param_1 + 0x78));
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2d7a40  Name: FUN_1402d7a40  Size: 4554 bytes
// ------------------------------------------------------------

void FUN_1402d7a40(longlong param_1,int param_2,undefined4 param_3)

{
  longlong *plVar1;
  float fVar2;
  float fVar3;
  char cVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  longlong lVar8;
  undefined4 *puVar9;
  undefined8 ****ppppuVar10;
  int iVar11;
  ulonglong uVar12;
  longlong *plVar13;
  longlong *plVar14;
  undefined4 uVar15;
  int iVar16;
  longlong lVar17;
  int iVar18;
  longlong lVar19;
  float fVar20;
  undefined4 *in_stack_fffffffffffffdb8;
  float in_stack_fffffffffffffdd0;
  undefined8 ***local_208 [3];
  ulonglong local_1f0;
  undefined4 local_1e8;
  undefined4 local_1e4;
  undefined4 local_1e0;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined1 local_1b8 [16];
  undefined1 local_1a8 [12];
  undefined1 local_19c [12];
  undefined1 local_190 [24];
  undefined1 local_178 [320];
  
  fVar3 = DAT_140800908;
  fVar2 = DAT_1408007f0;
  iVar18 = 0;
  if (0 < *(int *)(param_1 + 0x78)) {
    lVar19 = 0;
    do {
      plVar1 = *(longlong **)(*(longlong *)(param_1 + 0x70) + lVar19 * 8);
      if ((plVar1 != (longlong *)0x0) && (plVar14 = (longlong *)*plVar1, plVar14 != (longlong *)0x0)
         ) {
        uVar6 = (undefined4)plVar1[6];
        iVar11 = param_2 - *(int *)((longlong)plVar1 + 0x34);
        switch(uVar6) {
        case 1:
          uVar6 = FUN_1401ffb40(&DAT_140a13e20,*(undefined1 *)((longlong)plVar14 + 0xc5),
                                *(undefined1 *)((longlong)plVar14 + 0xc6));
          *(undefined4 *)(plVar1 + 10) = uVar6;
          uVar6 = FUN_1401ffb40(&DAT_140a13e20,*(undefined1 *)((longlong)plVar14 + 0xc9),
                                *(undefined1 *)((longlong)plVar14 + 0xca));
          *(undefined4 *)(plVar1 + 0xc) = uVar6;
          uVar6 = FUN_1401ffb40(&DAT_140a13e20,*(undefined1 *)((longlong)plVar14 + 199),
                                (char)plVar14[0x19]);
          *(undefined4 *)((longlong)plVar1 + 0x54) = uVar6;
          uVar6 = FUN_1401ffb40(&DAT_140a13e20,*(undefined4 *)((longlong)plVar14 + 0xcc),
                                (int)plVar14[0x1a]);
          *(undefined4 *)(plVar1 + 0xb) = uVar6;
          uVar6 = FUN_1401ffb40(&DAT_140a13e20,*(undefined4 *)((longlong)plVar14 + 0xd4),
                                (int)plVar14[0x1b]);
          *(undefined4 *)((longlong)plVar1 + 0x5c) = uVar6;
          if ((int)plVar1[0xb] < 1) {
LAB_1402d7bdd:
            uVar6 = 5;
            if (plVar14[0xf] - plVar14[0xe] >> 5 != 0) {
              uVar6 = 3;
            }
          }
          else {
            *(int *)(plVar1 + 9) = (int)plVar1[0xb];
            uVar6 = 2;
          }
          break;
        case 2:
          *(int *)(plVar1 + 9) = (int)plVar1[9] - iVar11;
          if ((int)plVar1[9] < 1) {
            *(undefined4 *)(plVar1 + 9) = 0;
            goto LAB_1402d7bdd;
          }
          break;
        case 3:
          iVar11 = (int)(plVar14[0xf] - plVar14[0xe] >> 5);
          if (iVar11 < 1) {
            uVar6 = 5;
          }
          else {
            if ((char)plVar14[10] == '\0') {
              iVar7 = FUN_1401ffb40(&DAT_140a13e20,0,iVar11 + -1);
            }
            else {
              iVar7 = *(int *)((longlong)plVar14 + 0x24);
              lVar8 = (longlong)iVar7;
              if ((iVar7 < 0) ||
                 ((int)(*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50) >> 3) <= iVar7
                 )) {
                plVar13 = plVar14;
                if (0xf < (ulonglong)plVar14[3]) {
                  plVar13 = (longlong *)*plVar14;
                }
                FUN_140025760("ERROR: SFXEvent %s has InternalIndex %d, but data vector size is %d\n"
                              ,plVar13,*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50)
                                       >> 3);
                iVar7 = 0;
              }
              else {
                lVar17 = *(longlong *)(param_1 + 0xc0);
                iVar7 = *(int *)(lVar17 + 4 + lVar8 * 0xc);
                *(int *)(lVar17 + 4 + lVar8 * 0xc) = iVar7 + 1;
                if (iVar11 <= iVar7 + 1) {
                  *(undefined4 *)(lVar17 + 4 + lVar8 * 0xc) = 0;
                }
              }
            }
            iVar16 = iVar11 + -1;
            if (iVar7 < iVar11 + -1) {
              iVar16 = iVar7;
            }
            FUN_14001e680(local_208,(longlong)iVar16 * 0x20 + plVar14[0xe]);
            if ((*(char *)((longlong)plVar14 + 0x2f) == '\x01') && ((int)plVar1[5] != 0)) {
              ppppuVar10 = local_208;
              if (0xf < local_1f0) {
                ppppuVar10 = (undefined8 ****)local_208[0];
              }
              lVar8 = FUN_140200240(ppppuVar10);
              lVar17 = -1;
              do {
                lVar17 = lVar17 + 1;
              } while (*(char *)(lVar8 + lVar17) != '\0');
              FUN_140022730(local_208,lVar8);
            }
            if (*(uint *)(plVar1 + 1) < 2) {
              if (*plVar1 == 0) {
                fVar20 = 0.0;
              }
              else {
                lVar8 = plVar1[10];
                fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
                if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')) &&
                   (fVar3 <= fVar20)) {
                  fVar20 = fVar3;
                }
              }
              iVar11 = *(int *)(param_1 + 8) + 1;
              *(int *)(param_1 + 8) = iVar11;
              if (iVar11 == 0) {
                *(undefined4 *)(param_1 + 8) = 1;
                iVar11 = 1;
              }
              in_stack_fffffffffffffdd0 = (float)(int)plVar1[0xc] / fVar2;
              in_stack_fffffffffffffdb8 =
                   (undefined4 *)
                   CONCAT44((int)((ulonglong)in_stack_fffffffffffffdb8 >> 0x20),fVar20);
              iVar7 = FUN_140228750(&DAT_140a7d108,local_208,iVar11,param_3,
                                    in_stack_fffffffffffffdb8,
                                    *(undefined1 *)((longlong)plVar14 + 0x2f),(char)plVar14[0x17],
                                    in_stack_fffffffffffffdd0,1,
                                    (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                    *(undefined4 *)((longlong)plVar1 + 0x2c));
              *(int *)(plVar1 + 7) = iVar7;
              if (iVar7 != -1) {
                iVar16 = 0;
                *(int *)(plVar1 + 8) = iVar11;
                lVar8 = 0;
                if (0 < *(int *)(param_1 + 0x78)) {
                  do {
                    if (((lVar8 != lVar19) &&
                        (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                        *(uint *)(lVar17 + 8) < 2)) && (*(int *)(lVar17 + 0x38) == iVar7)) {
                      *(undefined4 *)(lVar17 + 0x30) = 0xb;
                      *(undefined4 *)(lVar17 + 0x38) = 0xffffffff;
                    }
                    iVar16 = iVar16 + 1;
                    lVar8 = lVar8 + 1;
                  } while (iVar16 < *(int *)(param_1 + 0x78));
                }
                goto LAB_1402d8089;
              }
              *(undefined4 *)(plVar1 + 8) = 0;
              uVar15 = 0xb;
            }
            else {
              if (*(uint *)(plVar1 + 1) - 2 < 2) {
                local_1e8 = (undefined4)plVar1[3];
                local_1e4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
                local_1e0 = (undefined4)plVar1[4];
                if (plVar1[2] != 0) {
                  puVar9 = (undefined4 *)FUN_140398440(plVar1[2],local_1a8);
                  local_1e8 = *puVar9;
                  local_1e4 = puVar9[1];
                  local_1e0 = puVar9[2];
                }
                cVar5 = FUN_1402d3f80(param_1,plVar1,&local_1e8);
                if (cVar5 == '\x01') {
                  if (*plVar1 == 0) {
                    fVar20 = 0.0;
                  }
                  else {
                    lVar8 = plVar1[10];
                    fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                    fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                    cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
                    if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')
                        ) && (fVar3 <= fVar20)) {
                      fVar20 = fVar3;
                    }
                  }
                  iVar11 = *(int *)(param_1 + 8) + 1;
                  *(int *)(param_1 + 8) = iVar11;
                  if (iVar11 == 0) {
                    *(undefined4 *)(param_1 + 8) = 1;
                    iVar11 = 1;
                  }
                  in_stack_fffffffffffffdd0 =
                       (float)CONCAT31((int3)((uint)in_stack_fffffffffffffdd0 >> 8),
                                       *(undefined1 *)((longlong)plVar14 + 0x2f));
                  in_stack_fffffffffffffdb8 = &local_1e8;
                  iVar7 = FUN_14022a500(&DAT_140a7d530,local_208,iVar11,param_3,
                                        in_stack_fffffffffffffdb8,
                                        *(undefined4 *)((longlong)plVar14 + 0xdc),fVar20,
                                        in_stack_fffffffffffffdd0,(char)plVar14[0x17],1,
                                        (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                        *(undefined4 *)((longlong)plVar1 + 0x2c));
                  *(int *)((longlong)plVar1 + 0x3c) = iVar7;
                  if (iVar7 != -1) {
                    iVar16 = 0;
                    *(int *)(plVar1 + 8) = iVar11;
                    lVar8 = 0;
                    if (0 < *(int *)(param_1 + 0x78)) {
                      do {
                        if (((lVar8 != lVar19) &&
                            (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                            *(int *)(lVar17 + 8) - 2U < 2)) && (*(int *)(lVar17 + 0x3c) == iVar7)) {
                          *(undefined4 *)(lVar17 + 0x30) = 0xb;
                          *(undefined4 *)(lVar17 + 0x3c) = 0xffffffff;
                        }
                        iVar16 = iVar16 + 1;
                        lVar8 = lVar8 + 1;
                      } while (iVar16 < *(int *)(param_1 + 0x78));
                      uVar15 = 4;
                      goto LAB_1402d8002;
                    }
                    goto LAB_1402d8089;
                  }
                }
                else {
                  *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
                }
                uVar15 = 0xb;
                *(undefined4 *)(plVar1 + 8) = 0;
                uVar6 = 0xb;
                if (0xf < local_1f0) {
                  if (0xfff < local_1f0 + 1) {
                    if (0x1f < (ulonglong)
                               ((longlong)local_208[0] + (-8 - (longlong)local_208[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(local_208[0][-1],local_1f0 + 0x28);
                    }
                  }
                  goto LAB_1402d7ea9;
                }
                break;
              }
LAB_1402d8089:
              uVar15 = 4;
            }
LAB_1402d8002:
            uVar6 = uVar15;
            if (0xf < local_1f0) {
              if (0xfff < local_1f0 + 1) {
                if (0x1f < (ulonglong)((longlong)local_208[0] + (-8 - (longlong)local_208[0][-1])))
                {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_208[0][-1],local_1f0 + 0x28);
                }
              }
LAB_1402d7ea9:
              thunk_FUN_1407864b8();
              uVar6 = uVar15;
            }
          }
          break;
        case 5:
          iVar11 = (int)(plVar14[0xc] - plVar14[0xb] >> 5);
          if (iVar11 < 1) {
            if (0xf < (ulonglong)plVar14[3]) {
              plVar14 = (longlong *)*plVar14;
            }
            FUN_140048ec0(local_178,
                          "Error: SFXEvent \'%s\' has no primary samples defined for it.\n",plVar14)
            ;
            FUN_1402149d0(local_178);
            uVar6 = 7;
          }
          else {
            if ((char)plVar14[10] == '\0') {
              iVar7 = FUN_1401ffb40(&DAT_140a13e20,0,iVar11 + -1);
            }
            else {
              iVar7 = *(int *)((longlong)plVar14 + 0x24);
              lVar8 = (longlong)iVar7;
              if ((iVar7 < 0) ||
                 ((int)(*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50) >> 3) <= iVar7
                 )) {
                plVar13 = plVar14;
                if (0xf < (ulonglong)plVar14[3]) {
                  plVar13 = (longlong *)*plVar14;
                }
                FUN_140025760("ERROR: SFXEvent %s has InternalIndex %d, but data vector size is %d\n"
                              ,plVar13,*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50)
                                       >> 3);
                iVar7 = 0;
              }
              else {
                lVar17 = *(longlong *)(param_1 + 0xc0);
                iVar7 = *(int *)(lVar17 + lVar8 * 0xc);
                *(int *)(lVar17 + lVar8 * 0xc) = iVar7 + 1;
                if (iVar11 <= iVar7 + 1) {
                  *(undefined4 *)(lVar17 + lVar8 * 0xc) = 0;
                }
              }
            }
            iVar16 = iVar11 + -1;
            if (iVar7 < iVar11 + -1) {
              iVar16 = iVar7;
            }
            FUN_14001e680(local_208,(longlong)iVar16 * 0x20 + plVar14[0xb]);
            if ((*(char *)((longlong)plVar14 + 0x2f) == '\x01') && ((int)plVar1[5] != 0)) {
              ppppuVar10 = local_208;
              if (0xf < local_1f0) {
                ppppuVar10 = (undefined8 ****)local_208[0];
              }
              lVar8 = FUN_140200240(ppppuVar10);
              lVar17 = -1;
              do {
                lVar17 = lVar17 + 1;
              } while (*(char *)(lVar8 + lVar17) != '\0');
              FUN_140022730(local_208,lVar8);
            }
            cVar5 = *(char *)((longlong)plVar14 + 0xba);
            if (*(uint *)(plVar1 + 1) < 2) {
              if (*plVar1 == 0) {
                fVar20 = 0.0;
              }
              else {
                lVar8 = plVar1[10];
                fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                cVar4 = FUN_14022cf20(&DAT_140a7dfe0);
                if (((cVar4 == '\x01') && (cVar4 = FUN_1401c3c40(&DAT_140a78510), cVar4 == '\0')) &&
                   (fVar3 <= fVar20)) {
                  fVar20 = fVar3;
                }
              }
              iVar11 = *(int *)(param_1 + 8) + 1;
              *(int *)(param_1 + 8) = iVar11;
              if (iVar11 == 0) {
                *(undefined4 *)(param_1 + 8) = 1;
                iVar11 = 1;
              }
              in_stack_fffffffffffffdd0 = (float)(int)plVar1[0xc] / fVar2;
              in_stack_fffffffffffffdb8 =
                   (undefined4 *)
                   CONCAT44((int)((ulonglong)in_stack_fffffffffffffdb8 >> 0x20),fVar20);
              iVar7 = FUN_140228750(&DAT_140a7d108,local_208,iVar11,param_3,
                                    in_stack_fffffffffffffdb8,
                                    *(undefined1 *)((longlong)plVar14 + 0x2f),(char)plVar14[0x17],
                                    in_stack_fffffffffffffdd0,cVar5 != -1,
                                    (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                    *(undefined4 *)((longlong)plVar1 + 0x2c));
              *(int *)(plVar1 + 7) = iVar7;
              if (iVar7 == -1) {
                *(undefined4 *)(plVar1 + 8) = 0;
                uVar15 = 0xb;
              }
              else {
                iVar16 = 0;
                *(int *)(plVar1 + 8) = iVar11;
                lVar8 = 0;
                if (*(int *)(param_1 + 0x78) < 1) goto LAB_1402d8654;
                do {
                  if (((lVar8 != lVar19) &&
                      (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                      *(uint *)(lVar17 + 8) < 2)) && (*(int *)(lVar17 + 0x38) == iVar7)) {
                    *(undefined4 *)(lVar17 + 0x30) = 0xb;
                    *(undefined4 *)(lVar17 + 0x38) = 0xffffffff;
                  }
                  iVar16 = iVar16 + 1;
                  lVar8 = lVar8 + 1;
                } while (iVar16 < *(int *)(param_1 + 0x78));
                uVar15 = 6;
              }
            }
            else {
              if (*(uint *)(plVar1 + 1) - 2 < 2) {
                local_1d8 = (undefined4)plVar1[3];
                local_1d4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
                local_1d0 = (undefined4)plVar1[4];
                if (plVar1[2] != 0) {
                  puVar9 = (undefined4 *)FUN_140398440(plVar1[2],local_19c);
                  cVar5 = *(char *)((longlong)plVar14 + 0xba);
                  local_1d8 = *puVar9;
                  local_1d4 = puVar9[1];
                  local_1d0 = puVar9[2];
                }
                cVar4 = FUN_1402d3f80(param_1,plVar1,&local_1d8);
                if (cVar4 != '\x01') {
                  *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
                  *(undefined4 *)(plVar1 + 8) = 0;
                  uVar6 = 0xb;
                  if (0xf < local_1f0) {
                    uVar12 = local_1f0 + 1;
                    ppppuVar10 = (undefined8 ****)local_208[0];
                    if (0xfff < uVar12) {
                      ppppuVar10 = (undefined8 ****)local_208[0][-1];
                      uVar12 = local_1f0 + 0x28;
                      if (0x1f < (ulonglong)((longlong)local_208[0] + (-8 - (longlong)ppppuVar10)))
                      {
LAB_1402d8bf8:
                    /* WARNING: Subroutine does not return */
                        FUN_1407711cc(ppppuVar10,uVar12);
                      }
                    }
                    thunk_FUN_1407864b8(ppppuVar10,uVar12);
                  }
                  break;
                }
                if (*plVar1 == 0) {
                  fVar20 = 0.0;
                }
                else {
                  lVar8 = plVar1[10];
                  fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                  fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                  cVar4 = FUN_14022cf20(&DAT_140a7dfe0);
                  if (((cVar4 == '\x01') && (cVar4 = FUN_1401c3c40(&DAT_140a78510), cVar4 == '\0'))
                     && (fVar3 <= fVar20)) {
                    fVar20 = fVar3;
                  }
                }
                iVar11 = *(int *)(param_1 + 8) + 1;
                *(int *)(param_1 + 8) = iVar11;
                if (iVar11 == 0) {
                  *(undefined4 *)(param_1 + 8) = 1;
                  iVar11 = 1;
                }
                in_stack_fffffffffffffdd0 =
                     (float)CONCAT31((int3)((uint)in_stack_fffffffffffffdd0 >> 8),
                                     *(undefined1 *)((longlong)plVar14 + 0x2f));
                in_stack_fffffffffffffdb8 = &local_1d8;
                iVar7 = FUN_14022a500(&DAT_140a7d530,local_208,iVar11,param_3,
                                      in_stack_fffffffffffffdb8,
                                      *(undefined4 *)((longlong)plVar14 + 0xdc),fVar20,
                                      in_stack_fffffffffffffdd0,(char)plVar14[0x17],cVar5 != -1,
                                      (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                      *(undefined4 *)((longlong)plVar1 + 0x2c));
                *(int *)((longlong)plVar1 + 0x3c) = iVar7;
                if (iVar7 == -1) {
                  uVar15 = 0xb;
                  *(undefined4 *)(plVar1 + 8) = 0;
                  uVar6 = uVar15;
                  if (local_1f0 < 0x10) break;
                  if (0xfff < local_1f0 + 1) {
                    ppppuVar10 = (undefined8 ****)local_208[0][-1];
                    uVar12 = local_1f0 + 0x28;
                    if (0x1f < (ulonglong)((longlong)local_208[0] + (-8 - (longlong)ppppuVar10)))
                    goto LAB_1402d8bf8;
                  }
                  goto LAB_1402d7ea9;
                }
                iVar16 = 0;
                *(int *)(plVar1 + 8) = iVar11;
                lVar8 = 0;
                if (0 < *(int *)(param_1 + 0x78)) {
                  do {
                    if (((lVar8 != lVar19) &&
                        (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                        *(int *)(lVar17 + 8) - 2U < 2)) && (*(int *)(lVar17 + 0x3c) == iVar7)) {
                      *(undefined4 *)(lVar17 + 0x30) = 0xb;
                      *(undefined4 *)(lVar17 + 0x3c) = 0xffffffff;
                    }
                    iVar16 = iVar16 + 1;
                    lVar8 = lVar8 + 1;
                  } while (iVar16 < *(int *)(param_1 + 0x78));
                  uVar15 = 6;
                  goto LAB_1402d85b5;
                }
              }
LAB_1402d8654:
              uVar15 = 6;
            }
LAB_1402d85b5:
            uVar6 = uVar15;
            if (0xf < local_1f0) {
              if (0xfff < local_1f0 + 1) {
                if (0x1f < (ulonglong)((longlong)local_208[0] + (-8 - (longlong)local_208[0][-1])))
                {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_208[0][-1],local_1f0 + 0x28);
                }
              }
              goto LAB_1402d7ea9;
            }
          }
          break;
        case 7:
          iVar11 = (int)(plVar14[0x12] - plVar14[0x11] >> 5);
          if (iVar11 < 1) {
            uVar6 = 9;
          }
          else {
            if ((char)plVar14[10] == '\0') {
              iVar7 = FUN_1401ffb40(&DAT_140a13e20,0,iVar11 + -1);
            }
            else {
              iVar7 = *(int *)((longlong)plVar14 + 0x24);
              lVar8 = (longlong)iVar7;
              if ((iVar7 < 0) ||
                 ((int)(*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50) >> 3) <= iVar7
                 )) {
                plVar13 = plVar14;
                if (0xf < (ulonglong)plVar14[3]) {
                  plVar13 = (longlong *)*plVar14;
                }
                FUN_140025760("ERROR: SFXEvent %s has InternalIndex %d, but data vector size is %d\n"
                              ,plVar13,*(longlong *)(param_1 + 0x58) - *(longlong *)(param_1 + 0x50)
                                       >> 3);
                iVar7 = 0;
              }
              else {
                lVar17 = *(longlong *)(param_1 + 0xc0);
                iVar7 = *(int *)(lVar17 + 8 + lVar8 * 0xc);
                *(int *)(lVar17 + 8 + lVar8 * 0xc) = iVar7 + 1;
                if (iVar11 <= iVar7 + 1) {
                  *(undefined4 *)(lVar17 + 8 + lVar8 * 0xc) = 0;
                }
              }
            }
            iVar16 = iVar11 + -1;
            if (iVar7 < iVar11 + -1) {
              iVar16 = iVar7;
            }
            FUN_14001e680(local_208,(longlong)iVar16 * 0x20 + plVar14[0x11]);
            if ((*(char *)((longlong)plVar14 + 0x2f) == '\x01') && ((int)plVar1[5] != 0)) {
              ppppuVar10 = local_208;
              if (0xf < local_1f0) {
                ppppuVar10 = (undefined8 ****)local_208[0];
              }
              lVar8 = FUN_140200240(ppppuVar10);
              lVar17 = -1;
              do {
                lVar17 = lVar17 + 1;
              } while (*(char *)(lVar8 + lVar17) != '\0');
              FUN_140022730(local_208,lVar8);
            }
            if (*(uint *)(plVar1 + 1) < 2) {
              if (*plVar1 == 0) {
                fVar20 = 0.0;
              }
              else {
                lVar8 = plVar1[10];
                fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
                if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')) &&
                   (fVar3 <= fVar20)) {
                  fVar20 = fVar3;
                }
              }
              iVar11 = *(int *)(param_1 + 8) + 1;
              *(int *)(param_1 + 8) = iVar11;
              if (iVar11 == 0) {
                *(undefined4 *)(param_1 + 8) = 1;
                iVar11 = 1;
              }
              in_stack_fffffffffffffdd0 = (float)(int)plVar1[0xc] / fVar2;
              in_stack_fffffffffffffdb8 =
                   (undefined4 *)
                   CONCAT44((int)((ulonglong)in_stack_fffffffffffffdb8 >> 0x20),fVar20);
              iVar7 = FUN_140228750(&DAT_140a7d108,local_208,iVar11,param_3,
                                    in_stack_fffffffffffffdb8,
                                    *(undefined1 *)((longlong)plVar14 + 0x2f),(char)plVar14[0x17],
                                    in_stack_fffffffffffffdd0,1,
                                    (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                    *(undefined4 *)((longlong)plVar1 + 0x2c));
              *(int *)(plVar1 + 7) = iVar7;
              if (iVar7 != -1) {
                iVar16 = 0;
                *(int *)(plVar1 + 8) = iVar11;
                lVar8 = 0;
                if (0 < *(int *)(param_1 + 0x78)) {
                  do {
                    if (((lVar8 != lVar19) &&
                        (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                        *(uint *)(lVar17 + 8) < 2)) && (*(int *)(lVar17 + 0x38) == iVar7)) {
                      *(undefined4 *)(lVar17 + 0x30) = 0xb;
                      *(undefined4 *)(lVar17 + 0x38) = 0xffffffff;
                    }
                    iVar16 = iVar16 + 1;
                    lVar8 = lVar8 + 1;
                  } while (iVar16 < *(int *)(param_1 + 0x78));
                }
                goto LAB_1402d8af8;
              }
              *(undefined4 *)(plVar1 + 8) = 0;
              uVar15 = 0xb;
            }
            else {
              if (*(uint *)(plVar1 + 1) - 2 < 2) {
                local_1c8 = (undefined4)plVar1[3];
                local_1c4 = *(undefined4 *)((longlong)plVar1 + 0x1c);
                local_1c0 = (undefined4)plVar1[4];
                if (plVar1[2] != 0) {
                  puVar9 = (undefined4 *)FUN_140398440(plVar1[2],local_190);
                  local_1c8 = *puVar9;
                  local_1c4 = puVar9[1];
                  local_1c0 = puVar9[2];
                }
                cVar5 = FUN_1402d3f80(param_1,plVar1,&local_1c8);
                if (cVar5 == '\x01') {
                  if (*plVar1 == 0) {
                    fVar20 = 0.0;
                  }
                  else {
                    lVar8 = plVar1[10];
                    fVar20 = (float)FUN_1402d4b00(param_1,plVar1);
                    fVar20 = ((float)(int)lVar8 / fVar2) * fVar20;
                    cVar5 = FUN_14022cf20(&DAT_140a7dfe0);
                    if (((cVar5 == '\x01') && (cVar5 = FUN_1401c3c40(&DAT_140a78510), cVar5 == '\0')
                        ) && (fVar3 <= fVar20)) {
                      fVar20 = fVar3;
                    }
                  }
                  iVar11 = *(int *)(param_1 + 8) + 1;
                  *(int *)(param_1 + 8) = iVar11;
                  if (iVar11 == 0) {
                    *(undefined4 *)(param_1 + 8) = 1;
                    iVar11 = 1;
                  }
                  in_stack_fffffffffffffdd0 =
                       (float)CONCAT31((int3)((uint)in_stack_fffffffffffffdd0 >> 8),
                                       *(undefined1 *)((longlong)plVar14 + 0x2f));
                  in_stack_fffffffffffffdb8 = &local_1c8;
                  iVar7 = FUN_14022a500(&DAT_140a7d530,local_208,iVar11,param_3,
                                        in_stack_fffffffffffffdb8,
                                        *(undefined4 *)((longlong)plVar14 + 0xdc),fVar20,
                                        in_stack_fffffffffffffdd0,(char)plVar14[0x17],1,
                                        (float)*(int *)((longlong)plVar1 + 0x54) / fVar2,
                                        *(undefined4 *)((longlong)plVar1 + 0x2c));
                  *(int *)((longlong)plVar1 + 0x3c) = iVar7;
                  if (iVar7 != -1) {
                    iVar16 = 0;
                    *(int *)(plVar1 + 8) = iVar11;
                    lVar8 = 0;
                    if (0 < *(int *)(param_1 + 0x78)) {
                      do {
                        if (((lVar8 != lVar19) &&
                            (lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x70) + lVar8 * 8),
                            *(int *)(lVar17 + 8) - 2U < 2)) && (*(int *)(lVar17 + 0x3c) == iVar7)) {
                          *(undefined4 *)(lVar17 + 0x30) = 0xb;
                          *(undefined4 *)(lVar17 + 0x3c) = 0xffffffff;
                        }
                        iVar16 = iVar16 + 1;
                        lVar8 = lVar8 + 1;
                      } while (iVar16 < *(int *)(param_1 + 0x78));
                      uVar15 = 8;
                      goto LAB_1402d8a71;
                    }
                    goto LAB_1402d8af8;
                  }
                }
                else {
                  *(undefined4 *)((longlong)plVar1 + 0x3c) = 0xffffffff;
                }
                uVar15 = 0xb;
                *(undefined4 *)(plVar1 + 8) = 0;
                uVar6 = 0xb;
                if (0xf < local_1f0) {
                  if (0xfff < local_1f0 + 1) {
                    if (0x1f < (ulonglong)
                               ((longlong)local_208[0] + (-8 - (longlong)local_208[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(local_208[0][-1],local_1f0 + 0x28);
                    }
                  }
                  goto LAB_1402d7ea9;
                }
                break;
              }
LAB_1402d8af8:
              uVar15 = 8;
            }
LAB_1402d8a71:
            uVar6 = uVar15;
            if (0xf < local_1f0) {
              if (0xfff < local_1f0 + 1) {
                if (0x1f < (ulonglong)((longlong)local_208[0] + (-8 - (longlong)local_208[0][-1])))
                {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_208[0][-1],local_1f0 + 0x28);
                }
              }
              goto LAB_1402d7ea9;
            }
          }
          break;
        case 9:
          if (*(int *)((longlong)plVar1 + 0x5c) < 1) {
LAB_1402d8b34:
            uVar6 = 0xb;
          }
          else {
            *(int *)((longlong)plVar1 + 0x4c) = *(int *)((longlong)plVar1 + 0x5c);
            uVar6 = 10;
          }
          break;
        case 10:
          *(int *)((longlong)plVar1 + 0x4c) = *(int *)((longlong)plVar1 + 0x4c) - iVar11;
          if (*(int *)((longlong)plVar1 + 0x4c) < 1) {
            *(undefined4 *)((longlong)plVar1 + 0x4c) = 0;
            goto LAB_1402d8b34;
          }
          break;
        case 0xb:
          iVar11 = *(int *)((longlong)plVar1 + 0x44) + 1;
          *(int *)((longlong)plVar1 + 0x44) = iVar11;
          if ((*(char *)((longlong)plVar14 + 0xba) == -1) ||
             (iVar11 < *(char *)((longlong)plVar14 + 0xba))) {
            uVar6 = 1;
          }
          else {
            lVar8 = plVar1[0xf];
            uVar6 = 0;
            if ((((lVar8 != 0) && (*(uint *)(plVar1 + 1) < 2)) &&
                (*(char *)(lVar8 + 0x2a) == '\x01')) ||
               (((lVar8 = plVar14[0x22], lVar8 != 0 && (*(uint *)(plVar1 + 1) < 2)) &&
                (*(char *)(lVar8 + 0x2a) == '\x01')))) {
              if (plVar1[2] == 0) {
                in_stack_fffffffffffffdd0 = (float)((uint)in_stack_fffffffffffffdd0 & 0xffffff00);
                in_stack_fffffffffffffdb8 = (undefined4 *)0x0;
                FUN_1402d5440(param_1,lVar8,0,local_1b8,0,0,0,in_stack_fffffffffffffdd0);
              }
              else {
                in_stack_fffffffffffffdb8 =
                     (undefined4 *)((ulonglong)in_stack_fffffffffffffdb8 & 0xffffffffffffff00);
                FUN_1402d5320(param_1,lVar8,plVar1[2],0,in_stack_fffffffffffffdb8);
              }
            }
          }
        }
        *(undefined4 *)(plVar1 + 6) = uVar6;
      }
      iVar18 = iVar18 + 1;
      lVar19 = lVar19 + 1;
    } while (iVar18 < *(int *)(param_1 + 0x78));
  }
  return;
}




// ================================================================
// XREFS TO GameObjectClass VTABLE ENTRIES
// ================================================================

// Symbol: GameObjectClass @ fffffffec0000000 (type: Class)
// Found symbol: GameObjectManagerClass @ RVA 0xfffffffec0000000

