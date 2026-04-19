
// ================================================================
// SLOT 2 — FUN_1404b3350 (prime Think candidate)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b3350  Name: FUN_1404b3350  Size: 439 bytes
// ------------------------------------------------------------

void FUN_1404b3350(longlong param_1)

{
  ulonglong uVar1;
  longlong *plVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined **local_res8;
  
  FUN_140365300();
  if (*(longlong **)(param_1 + 800) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 800) + 8))();
    *(undefined8 *)(param_1 + 800) = 0;
  }
  if (*(longlong **)(param_1 + 0x328) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x328) + 8))();
    *(undefined8 *)(param_1 + 0x328) = 0;
  }
  if (*(longlong **)(param_1 + 0x330) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x330) + 8))();
    *(undefined8 *)(param_1 + 0x330) = 0;
  }
  if (*(longlong **)(param_1 + 0x338) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x338) + 8))();
    *(undefined8 *)(param_1 + 0x338) = 0;
  }
  FUN_140306290(&DAT_140b27f60,0);
  if (DAT_1409cf1d0 == 0) {
    lVar4 = FUN_140769c58(0x138);
    if (lVar4 == 0) {
      DAT_1409cf1d0 = 0;
    }
    else {
      DAT_1409cf1d0 = FUN_1403ef010(lVar4);
    }
    local_res8 = &PTR_vftable_1409cf1c8;
    if (DAT_140a7bc10 == DAT_140a7bc18) {
      FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
    }
    else {
      *DAT_140a7bc10 = &PTR_vftable_1409cf1c8;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
    }
  }
  FUN_1403f2860(DAT_1409cf1d0);
  plVar2 = DAT_140b31ad8;
  plVar6 = DAT_140b31ad0;
  lVar4 = DAT_140b31af0;
  do {
    plVar3 = DAT_140b31ad0;
    DAT_140b31af0 = lVar4;
    lVar5 = DAT_140b31ae8;
    if (plVar6 == plVar2) {
      for (; DAT_140b31ad8 = plVar3, lVar5 != lVar4; lVar5 = lVar5 + 0x40) {
        FUN_1404b12f0(lVar5);
        plVar3 = DAT_140b31ad8;
      }
      DAT_140b31af0 = DAT_140b31ae8;
      return;
    }
    uVar1 = plVar6[3];
    if (0xf < uVar1) {
      lVar4 = *plVar6;
      lVar5 = lVar4;
      if (0xfff < uVar1 + 1) {
        lVar5 = *(longlong *)(lVar4 + -8);
        if (0x1f < (lVar4 - lVar5) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(lVar4 - lVar5,uVar1 + 0x28);
        }
      }
      thunk_FUN_1407864b8(lVar5);
    }
    plVar6[2] = 0;
    plVar6[3] = 0xf;
    *(undefined1 *)plVar6 = 0;
    plVar6 = plVar6 + 4;
    lVar4 = DAT_140b31af0;
  } while( true );
}




// ================================================================
// SLOT 1 — FUN_1404b21f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b21f0  Name: FUN_1404b21f0  Size: 317 bytes
// ------------------------------------------------------------

undefined8 FUN_1404b21f0(longlong param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 local_28;
  undefined8 local_24;
  
  FUN_1404b1d60();
  FUN_14035e560(param_1,param_2,param_3);
  local_28 = DAT_14086769c;
  local_24 = 0;
  FUN_140261c90(*(undefined8 *)(param_1 + 0x90),&local_28);
  FUN_140261ab0(*(undefined8 *)(param_1 + 0x90),DAT_1408054ec,DAT_1408054d0,DAT_1408181ec,
                DAT_1408181f0,DAT_140805898,DAT_1408007fc);
  FUN_140261e00(*(undefined8 *)(param_1 + 0x90),0,DAT_1407ffaf8,DAT_1407ffaf8,0);
  FUN_140306290(&DAT_140b27f60,1);
  FUN_1403d3200(&DAT_140b27f60);
  FUN_1404c0420(&DAT_140a2aba0);
  *(undefined4 *)(param_1 + 0x234) = DAT_140b16dbc;
  *(undefined4 *)(param_1 + 0x368) = *(undefined4 *)(param_1 + 0x36c);
  iVar1 = FUN_14027c9a0(&DAT_140b0a320,DAT_140b15980);
  *(int *)(param_1 + 0x36c) = *(int *)(param_1 + 0x36c) + iVar1;
  return 1;
}




// ================================================================
// SLOT 43 — FUN_1404b1600
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b1600  Name: FUN_1404b1600  Size: 226 bytes
// ------------------------------------------------------------

void FUN_1404b1600(longlong *param_1,undefined8 *param_2,undefined8 *param_3,undefined4 *param_4)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  lVar1 = param_1[0x12];
  if (lVar1 == 0) {
    *param_2 = 0;
    *(undefined4 *)(param_2 + 1) = 0;
    *param_3 = 0;
    *(undefined4 *)(param_3 + 1) = 0x3f800000;
    *param_4 = 0;
    *(undefined8 *)(param_4 + 1) = 0x3f800000;
  }
  else {
    cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
    if (cVar2 == '\x01') {
      FUN_140261a40(lVar1,&local_18);
      *(undefined4 *)param_2 = local_18;
      *(undefined4 *)(param_2 + 1) = local_10;
      *(undefined4 *)((longlong)param_2 + 4) = local_14;
      FUN_140261690(lVar1,param_3,param_4);
    }
    else {
      uVar3 = (**(code **)(*param_1 + 0x150))(param_1);
      FUN_140261710(lVar1,uVar3,param_2);
      FUN_140261690(lVar1,param_3,param_4);
    }
  }
  return;
}




// ================================================================
// SLOT 9 — FUN_1404b2360
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2360  Name: FUN_1404b2360  Size: 94 bytes
// ------------------------------------------------------------

undefined8 FUN_1404b2360(longlong param_1)

{
  char cVar1;
  
  if (0 < DAT_140b1540c) {
    return 1;
  }
  if ((((DAT_140b154d0 != '\0') || ((int)(DAT_140b2b368 - DAT_140b2b360 >> 3) != 0)) &&
      (cVar1 = FUN_14028af60(&DAT_140b153e0), cVar1 == '\0')) &&
     (cVar1 = FUN_1402b0350(*(undefined8 *)(param_1 + 0x18)), cVar1 != '\0')) {
    return 1;
  }
  return 0;
}




// ================================================================
// SLOT 10 — FUN_1404b25f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b25f0  Name: FUN_1404b25f0  Size: 14 bytes
// ------------------------------------------------------------

void FUN_1404b25f0(void)

{
  FUN_140306290(&DAT_140b27f60,0);
  return;
}




// ================================================================
// SLOT 11 — FUN_1404b2c70
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2c70  Name: FUN_1404b2c70  Size: 238 bytes
// ------------------------------------------------------------

void FUN_1404b2c70(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  undefined8 local_18;
  undefined4 local_10;
  
  FUN_140306290(&DAT_140b27f60,1);
  if (*(longlong *)(param_1 + 0x18) != 0) {
    local_18 = 0;
    local_10 = 0;
    lVar2 = FUN_1402a9ff0(*(longlong *)(param_1 + 0x18),3,0xffffffff,0xffffffff,&local_18,0,
                          0x7fffffff,0xffffffff);
    if (lVar2 != 0) {
      iVar3 = 0;
      if (0 < *(int *)(lVar2 + 0x10)) {
        lVar4 = 0;
        do {
          lVar1 = *(longlong *)(lVar4 + *(longlong *)(lVar2 + 8));
          if ((((lVar1 != 0) && (*(longlong *)(lVar1 + 0xb8) != 0)) &&
              (*(char *)(*(longlong *)(lVar1 + 0xb8) + 0x2c8) != '\0')) &&
             (*(longlong *)(lVar1 + 0x100) != 0)) {
            FUN_14037de80(*(longlong *)(lVar1 + 0x100) + 8);
          }
          iVar3 = iVar3 + 1;
          lVar4 = lVar4 + 8;
        } while (iVar3 < *(int *)(lVar2 + 0x10));
      }
      FUN_140294360(&DAT_140a16fd0);
    }
  }
  return;
}




// ================================================================
// SLOT 12 — FUN_1404b2d60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2d60  Name: FUN_1404b2d60  Size: 387 bytes
// ------------------------------------------------------------

byte FUN_1404b2d60(longlong param_1,undefined8 param_2,undefined1 param_3)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  byte bVar20;
  longlong *plVar21;
  
  bVar1 = FUN_14021fe20(param_2,0);
  bVar2 = FUN_140362f60(param_1,param_2,param_3);
  bVar3 = FUN_14021feb0(param_2);
  bVar4 = FUN_14021fe20(param_2,1);
  bVar5 = FUN_14021ffa0(param_2,0);
  bVar6 = FUN_1402200b0(param_2,param_1 + 0x340,1);
  bVar7 = FUN_140220030(param_2);
  bVar8 = FUN_14021ffa0(param_2,1);
  plVar21 = (longlong *)(param_1 + 0x348);
  if (0xf < *(ulonglong *)(param_1 + 0x360)) {
    plVar21 = (longlong *)*plVar21;
  }
  bVar9 = FUN_140220140(param_2,plVar21,0);
  bVar10 = FUN_140220030(param_2);
  bVar11 = FUN_14021ffa0(param_2,2);
  bVar12 = FUN_1402200b0(param_2,param_1 + 0x368,4);
  bVar13 = FUN_140220030(param_2);
  bVar14 = FUN_14021ffa0(param_2,3);
  bVar15 = FUN_1402200b0(param_2,param_1 + 0x36c,4);
  bVar16 = FUN_140220030(param_2);
  bVar17 = FUN_14021ffa0(param_2,4);
  bVar18 = FUN_1402200b0(param_2,param_1 + 0x370,4);
  bVar19 = FUN_140220030(param_2);
  bVar20 = FUN_14021feb0(param_2);
  return bVar20 & bVar1 & 1 & bVar2 & bVar3 & bVar4 & bVar5 & bVar6 & bVar7 & bVar8 & bVar9 & bVar10
                  & bVar11 & bVar12 & bVar13 & bVar14 & bVar15 & bVar16 & bVar17 & bVar18 & bVar19;
}




// ================================================================
// SLOT 13 — FUN_1404b23c0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b23c0  Name: FUN_1404b23c0  Size: 545 bytes
// ------------------------------------------------------------

byte FUN_1404b23c0(longlong param_1,undefined8 param_2,undefined1 param_3,undefined4 param_4,
                  undefined4 param_5)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  longlong lVar4;
  byte bVar5;
  undefined8 uVar6;
  undefined4 local_38;
  undefined8 local_34;
  
  bVar5 = 1;
  FUN_1404b1d60();
  cVar1 = FUN_1402204a0(param_2);
  do {
    if (cVar1 == '\0') {
      local_34 = 0;
      local_38 = DAT_14086769c;
      FUN_140261c90(*(undefined8 *)(param_1 + 0x90),&local_38);
      FUN_140261ab0(*(undefined8 *)(param_1 + 0x90),DAT_1408054ec,DAT_1408054d0,DAT_1408181ec,
                    DAT_1408181f0,DAT_140805898,DAT_1408007fc);
      FUN_140261e00(*(undefined8 *)(param_1 + 0x90),0,DAT_1407ffaf8,DAT_1407ffaf8,0);
      FUN_140306290(&DAT_140b27f60,1);
      FUN_1403d3200(&DAT_140b27f60);
      return bVar5;
    }
    iVar3 = FUN_1402205a0(param_2);
    if (iVar3 == 0) {
      bVar2 = FUN_14035f970(param_1,param_2,param_3,param_4,param_5);
      bVar5 = bVar5 & bVar2;
    }
    else if (iVar3 == 1) {
      cVar1 = FUN_140220610(param_2);
      while (cVar1 != '\0') {
        iVar3 = FUN_140220710(param_2);
        if (iVar3 == 0) {
          lVar4 = param_1 + 0x340;
          uVar6 = 1;
LAB_1404b24ae:
          FUN_140220730(param_2,lVar4,uVar6,0);
        }
        else {
          if (iVar3 != 1) {
            if (iVar3 == 2) {
              lVar4 = param_1 + 0x368;
              uVar6 = 4;
            }
            else if (iVar3 == 3) {
              lVar4 = param_1 + 0x36c;
              uVar6 = 4;
            }
            else {
              if (iVar3 != 4) goto LAB_1404b24b9;
              lVar4 = param_1 + 0x370;
              uVar6 = 4;
            }
            goto LAB_1404b24ae;
          }
          FUN_140220800(param_2,param_1 + 0x348,0);
        }
LAB_1404b24b9:
        FUN_1402206a0(param_2);
        cVar1 = FUN_140220610(param_2);
      }
    }
    FUN_140220520(param_2);
    cVar1 = FUN_1402204a0(param_2);
  } while( true );
}




// ================================================================
// SLOT 18 — FUN_140360970
// ================================================================

// ------------------------------------------------------------
// RVA: 0x360970  Name: FUN_140360970  Size: 1014 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 * FUN_140360970(longlong *param_1)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  char *pcVar6;
  undefined1 *puVar7;
  longlong *plVar8;
  ulonglong uVar9;
  longlong *plVar10;
  longlong lVar11;
  undefined8 ****ppppuVar12;
  uint local_res8 [2];
  uint in_stack_fffffffffffffde8;
  longlong local_208 [2];
  undefined8 local_1f8;
  ulonglong local_1f0;
  undefined1 *local_1e8 [2];
  undefined8 local_1d8;
  ulonglong local_1d0;
  undefined8 ***local_1c8 [3];
  ulonglong local_1b0;
  undefined1 local_1a8 [143];
  undefined1 uStack_119;
  char local_118 [272];
  
  local_res8[0] = 0xffffffff;
  if (param_1[0xc] == 0) {
    return (undefined8 *)&stack0x00000000;
  }
  plVar10 = param_1 + 10;
  plVar8 = plVar10;
  if (0xf < (ulonglong)param_1[0xd]) {
    plVar8 = (longlong *)*plVar10;
  }
  FUN_1402130d0(local_1a8,plVar8);
  cVar1 = FUN_140213840(local_1a8,0,0);
  puVar5 = (undefined8 *)0x0;
  if (cVar1 == '\0') {
    pcVar6 = &DAT_140b15440;
    if (0xf < DAT_140b15458) {
      pcVar6 = _DAT_140b15440;
    }
    lVar3 = -(longlong)pcVar6;
    do {
      cVar1 = *pcVar6;
      pcVar6[(longlong)(local_118 + lVar3)] = cVar1;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    local_208[0] = 0x6465742e;
    local_1f0 = 0xf;
    local_1f8 = 4;
    if (0xf < (ulonglong)param_1[0xd]) {
      plVar10 = (longlong *)*plVar10;
    }
    lVar3 = FUN_1402011a0(plVar10);
    lVar11 = -1;
    local_1e8[0] = (undefined1 *)0x0;
    local_1d8 = 0;
    local_1d0 = 0xf;
    do {
      lVar11 = lVar11 + 1;
    } while (*(char *)(lVar3 + lVar11) != '\0');
    FUN_140022730(local_1e8,lVar3);
    FUN_140058010(local_1c8,(undefined1)local_res8[0],local_1e8,local_208);
    ppppuVar12 = local_1c8;
    if (0xf < local_1b0) {
      ppppuVar12 = (undefined8 ****)local_1c8[0];
    }
    puVar7 = &uStack_119;
    do {
      pcVar6 = puVar7 + 1;
      puVar7 = puVar7 + 1;
    } while (*pcVar6 != '\0');
    lVar3 = 0;
    do {
      cVar1 = *(char *)((longlong)ppppuVar12 + lVar3);
      puVar7[lVar3] = cVar1;
      lVar3 = lVar3 + 1;
    } while (cVar1 != '\0');
    if (0xf < local_1b0) {
      uVar9 = local_1b0 + 1;
      ppppuVar12 = (undefined8 ****)local_1c8[0];
      if (0xfff < uVar9) {
        ppppuVar12 = (undefined8 ****)local_1c8[0][-1];
        uVar9 = local_1b0 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_1c8[0] + (-8 - (longlong)ppppuVar12)))
        goto LAB_140360d61;
      }
      thunk_FUN_1407864b8(ppppuVar12,uVar9);
    }
    if (0xf < local_1d0) {
      uVar9 = local_1d0 + 1;
      puVar7 = local_1e8[0];
      if (0xfff < uVar9) {
        puVar7 = *(undefined1 **)(local_1e8[0] + -8);
        uVar9 = local_1d0 + 0x28;
        if ((undefined1 *)0x1f < local_1e8[0] + (-8 - (longlong)puVar7)) goto LAB_140360d61;
      }
      thunk_FUN_1407864b8(puVar7,uVar9);
    }
    if (0xf < local_1f0) {
      if (0xfff < local_1f0 + 1) {
        puVar7 = *(undefined1 **)(local_208[0] + -8);
        uVar9 = local_1f0 + 0x28;
        if (0x1f < (local_208[0] - (longlong)puVar7) - 8U) {
LAB_140360d61:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar7,uVar9);
        }
      }
      thunk_FUN_1407864b8();
    }
    cVar1 = FUN_1402136f0(local_1a8,local_118,0,0,in_stack_fffffffffffffde8 & 0xffffff00);
    if (cVar1 == '\0') goto LAB_140360d21;
  }
  DAT_140a7bed4 = 1;
  uVar4 = FUN_140213e00(local_1a8);
  FUN_140025760("Loading base map \'%s\' \n",uVar4);
  lVar3 = FUN_140769c58(0x38);
  if (lVar3 != 0) {
    puVar5 = (undefined8 *)FUN_140220280(lVar3,local_1a8);
  }
  cVar1 = FUN_1402204a0(puVar5);
  while (cVar1 != '\0') {
    iVar2 = FUN_1402205a0(puVar5);
    if (iVar2 == 0) {
      cVar1 = FUN_140220610(puVar5);
      while (cVar1 != '\0') {
        iVar2 = FUN_140220710(puVar5);
        if (iVar2 == 6) {
          FUN_140220730(puVar5,local_res8,4,0);
        }
        FUN_1402206a0(puVar5);
        cVar1 = FUN_140220610(puVar5);
      }
    }
    else if (iVar2 == 1) {
      (**(code **)(*param_1 + 0x78))(param_1,puVar5,1,0,0xffffffff);
      if ((local_res8[0] < 8) && (*(uint *)(param_1 + 1) = local_res8[0], local_res8[0] != 7)) {
        FUN_140323110(&DAT_140b2b3b0,0,0);
      }
      iVar2 = (**(code **)(*param_1 + 0xe0))(param_1);
      if (iVar2 == 1) {
        uVar4 = (**(code **)(*param_1 + 0x100))(param_1);
        FUN_14046a2f0(&DAT_140b30b40,uVar4);
      }
    }
    else if (iVar2 == 0x201) {
      (**(code **)(*param_1 + 0x78))(param_1,puVar5,1,0,0xffffffff);
    }
    FUN_140220520(puVar5);
    cVar1 = FUN_1402204a0(puVar5);
  }
  if (puVar5 != (undefined8 *)0x0) {
    (**(code **)*puVar5)(puVar5,1);
  }
  FUN_1402138d0(local_1a8);
  DAT_140a7bed4 = 0;
  puVar5 = (undefined8 *)&DAT_00000001;
  *(undefined1 *)((longlong)param_1 + 0x9a) = 1;
LAB_140360d21:
  FUN_1402132f0(local_1a8);
  return puVar5;
}




// ================================================================
// SLOT 21 — FUN_1404b2bc0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2bc0  Name: FUN_1404b2bc0  Size: 54 bytes
// ------------------------------------------------------------

void FUN_1404b2bc0(longlong param_1,undefined4 param_2)

{
  FUN_14013d820(*(undefined8 *)(*(longlong *)(param_1 + 0x90) + 0x40));
  FUN_140362c30(param_1,param_2);
  return;
}




// ================================================================
// SLOT 22 — FUN_1404b2ef0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2ef0  Name: FUN_1404b2ef0  Size: 1003 bytes
// ------------------------------------------------------------

void FUN_1404b2ef0(longlong param_1,int param_2)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  int iVar6;
  longlong *plVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  float fVar12;
  undefined4 uVar13;
  undefined8 local_48;
  undefined4 local_40;
  
  FUN_1404b2600();
  FUN_1403639d0(param_1,param_2);
  cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar2 != '\0') {
    FUN_1402cd870(&DAT_140b27a10);
  }
  uVar10 = 0;
  iVar3 = 0;
  if (DAT_140b15418 != 0) {
    iVar3 = *(int *)(DAT_140b15418 + 0x10);
  }
  iVar6 = *(int *)(param_1 + 0x36c);
  if (iVar6 <= iVar3) {
    if ((*(longlong *)(param_1 + 0x18) != 0) &&
       (uVar8 = uVar10, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
      do {
        lVar4 = FUN_140294bc0(&DAT_140a16fd0,uVar8);
        if (lVar4 != 0) {
          plVar7 = (longlong *)(lVar4 + 0x28);
          if (7 < *(ulonglong *)(lVar4 + 0x40)) {
            plVar7 = (longlong *)*plVar7;
          }
          FUN_140025760("* List of Player %S income sources:\n",plVar7);
          fVar12 = (float)FUN_1404b16f0(param_1,lVar4,1);
          if (fVar12 != 0.0) {
            FUN_14027f370(lVar4,fVar12,0);
            cVar2 = FUN_1402814d0(lVar4);
            if ((((cVar2 == '\x01') && (*(char *)(lVar4 + 0x62) == '\0')) &&
                (lVar4 = FUN_1404b06d0(*(undefined8 *)(lVar4 + 0x360),0), lVar4 != 0)) &&
               (*(char *)(lVar4 + 0x69) != '\0')) {
              FUN_140614340(*(longlong *)(lVar4 + 0x40) + 400,fVar12);
            }
          }
        }
        uVar9 = (int)uVar8 + 1;
        uVar8 = (ulonglong)uVar9;
      } while ((int)uVar9 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
      iVar6 = *(int *)(param_1 + 0x36c);
    }
    *(int *)(param_1 + 0x368) = iVar6;
    iVar3 = FUN_14027c9a0(&DAT_140b0a320,DAT_140b15980);
    *(int *)(param_1 + 0x36c) = *(int *)(param_1 + 0x36c) + iVar3;
    *(int *)(param_1 + 0x370) = *(int *)(param_1 + 0x370) + 1;
    local_48 = 0;
    local_40 = 0;
    lVar4 = FUN_1402a9ff0(*(undefined8 *)(param_1 + 0x18),3,0xffffffff,0xffffffff,&local_48,0,
                          0x7fffffff,0xffffffff);
    if ((lVar4 != 0) && (uVar8 = uVar10, uVar11 = uVar10, 0 < *(int *)(lVar4 + 0x10))) {
      do {
        lVar5 = *(longlong *)(uVar11 + *(longlong *)(lVar4 + 8));
        if ((lVar5 != 0) &&
           ((lVar1 = *(longlong *)(lVar5 + 0xb8), lVar1 != 0 &&
            (*(char *)(*(longlong *)(lVar5 + 0x298) + 0xbd) == '\x01')))) {
          if (*(char *)(lVar1 + 0x2c8) == '\0') {
            uVar13 = FUN_1401ffbb0(&DAT_140a13e24,DAT_140b1598c,DAT_140b15990);
            *(undefined4 *)(lVar1 + 0x2a0) = uVar13;
          }
          else {
            *(undefined4 *)(lVar1 + 0x2a0) = 0x3f800000;
          }
        }
        uVar9 = (int)uVar8 + 1;
        uVar8 = (ulonglong)uVar9;
        uVar11 = uVar11 + 8;
      } while ((int)uVar9 < *(int *)(lVar4 + 0x10));
    }
    FUN_1402e4210(&DAT_140b27f60,0x3d,1,0,1);
    if ((*(longlong *)(param_1 + 0x28) != 0) &&
       (uVar8 = uVar10, 0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
      do {
        lVar4 = FUN_140294bc0(&DAT_140a16fd0,uVar8);
        if (lVar4 != 0) {
          FUN_140346460(*(undefined8 *)(param_1 + 0x28),10,*(undefined4 *)(lVar4 + 0x4c));
        }
        uVar9 = (int)uVar8 + 1;
        uVar8 = (ulonglong)uVar9;
      } while ((int)uVar9 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
    }
  }
  if (param_2 == (param_2 / 0x1e) * 0x1e) {
    if ((*(longlong *)(param_1 + 0x28) != 0) && (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
      do {
        lVar4 = FUN_140294bc0(&DAT_140a16fd0,uVar10);
        if (lVar4 != 0) {
          FUN_140346460(*(undefined8 *)(param_1 + 0x28),8,*(undefined4 *)(lVar4 + 0x4c));
        }
        uVar9 = (int)uVar10 + 1;
        uVar10 = (ulonglong)uVar9;
      } while ((int)uVar9 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
    }
    if (*(int *)(param_1 + 0x374) != 0x3fffff) {
      lVar4 = FUN_1402aca60(*(undefined8 *)(param_1 + 0x18));
      if ((lVar4 != 0) && (lVar5 = FUN_140407c00(&DAT_140b2f020,&DAT_140b27870), lVar5 != 0)) {
        FUN_14038d230(lVar4,lVar5,0);
      }
      *(undefined4 *)(param_1 + 0x374) = 0x3fffff;
    }
  }
  return;
}




// ================================================================
// SLOT 23 — FUN_1404b2c00
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2c00  Name: FUN_1404b2c00  Size: 111 bytes
// ------------------------------------------------------------

void FUN_1404b2c00(longlong *param_1,undefined4 param_2)

{
  char cVar1;
  
  FUN_14044bc90(&DAT_140a28c10);
  cVar1 = (**(code **)(*param_1 + 0x48))(param_1);
  if ((cVar1 != '\0') && (DAT_140b2766c == '\0')) {
    param_2 = 0;
  }
  FUN_140362c60(param_1,param_2);
  cVar1 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar1 != '\0') {
    FUN_1402cdeb0(&DAT_140b27a10,param_1[0x12]);
  }
  return;
}




// ================================================================
// SLOT 24 — FUN_1404b32e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b32e0  Name: FUN_1404b32e0  Size: 101 bytes
// ------------------------------------------------------------

void FUN_1404b32e0(longlong param_1,longlong param_2)

{
  if (param_2 != 0) {
    FUN_140364ea0();
    (**(code **)(**(longlong **)(param_1 + 800) + 0x40))(*(longlong **)(param_1 + 800),param_2);
    (**(code **)(**(longlong **)(param_1 + 0x328) + 0x40))(*(longlong **)(param_1 + 0x328),param_2);
    (**(code **)(**(longlong **)(param_1 + 0x330) + 0x40))(*(longlong **)(param_1 + 0x330),param_2);
    (**(code **)(**(longlong **)(param_1 + 0x338) + 0x40))(*(longlong **)(param_1 + 0x338),param_2);
  }
  return;
}




// ================================================================
// SLOT 5 — FUN_1404b1fb0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b1fb0  Name: FUN_1404b1fb0  Size: 53 bytes
// ------------------------------------------------------------

void FUN_1404b1fb0(void)

{
  FUN_14035cfc0();
  FUN_14021d090(&DAT_140a7d0a0,DAT_1407ffaf8);
  FUN_14021d0b0(&DAT_140a7d0a0,DAT_1407ffaf8);
  return;
}




// ================================================================
// SLOT 6 — FUN_1403b5a40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b5a40  Name: FUN_1403b5a40  Size: 32 bytes
// ------------------------------------------------------------

void FUN_1403b5a40(longlong *param_1)

{
  FUN_14035b2e0();
                    /* WARNING: Could not recover jumptable at 0x0001403b5a59. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x148))(param_1);
  return;
}




// ================================================================
// SLOT 40 — FUN_140047940
// ================================================================

// ------------------------------------------------------------
// RVA: 0x47940  Name: FUN_140047940  Size: 8 bytes
// ------------------------------------------------------------

undefined8 FUN_140047940(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x298);
}




// ================================================================
// SLOT 41 — FUN_1404b2330
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2330  Name: FUN_1404b2330  Size: 48 bytes
// ------------------------------------------------------------

void FUN_1404b2330(void)

{
  FUN_14021d090(&DAT_140a7d0a0,DAT_140b30190);
  FUN_14021d0b0(&DAT_140a7d0a0,DAT_140b3019c);
  return;
}




// ================================================================
// SLOT 42 — FUN_1404b2020
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b2020  Name: FUN_1404b2020  Size: 41 bytes
// ------------------------------------------------------------

undefined4 FUN_1404b2020(void)

{
  char cVar1;
  
  cVar1 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar1 == '\x01') {
    return 0;
  }
  return DAT_140b301a8;
}




// ================================================================
// SLOT 44 — FUN_140361470
// ================================================================

// ------------------------------------------------------------
// RVA: 0x361470  Name: FUN_140361470  Size: 349 bytes
// ------------------------------------------------------------

undefined8 FUN_140361470(longlong param_1,longlong param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  ulonglong uVar5;
  uint uVar6;
  uint local_res10;
  undefined1 local_res14;
  undefined1 local_18 [16];
  
  if (param_2 != 0) {
    lVar3 = *(longlong *)(param_1 + 0x240);
    local_res10 = *(uint *)(param_2 + 0xec);
    uVar6 = (local_res10 ^ 0xdeadbeef) & 0x7fffffff;
    iVar4 = (uVar6 % 0x1f31d) * 0x41a7 + (uVar6 / 0x1f31d) * -0xb14;
    iVar1 = iVar4 + 0x7fffffff;
    if (-1 < iVar4) {
      iVar1 = iVar4;
    }
    uVar5 = (longlong)iVar1 & *(ulonglong *)(param_1 + 0x268);
    lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x250) + 8 + uVar5 * 0x10);
    if (lVar2 != lVar3) {
      iVar1 = *(int *)(lVar2 + 0x10);
      iVar4 = iVar1 - local_res10;
      if ((int)local_res10 < iVar1) {
        do {
          if (lVar2 == *(longlong *)(*(longlong *)(param_1 + 0x250) + uVar5 * 0x10))
          goto LAB_140361535;
          lVar2 = *(longlong *)(lVar2 + 8);
        } while ((int)local_res10 < *(int *)(lVar2 + 0x10));
        iVar1 = *(int *)(lVar2 + 0x10);
        iVar4 = iVar1 - local_res10;
      }
      if (SBORROW4(iVar1,local_res10) == iVar4 < 0) {
        if (lVar2 == 0) {
          lVar2 = lVar3;
        }
        if (lVar2 != lVar3) {
          return 0;
        }
      }
    }
LAB_140361535:
    local_res14 = 1;
    FUN_14035a370(param_1 + 0x238,local_18,&local_res10);
    iVar1 = FUN_1402d4160(&DAT_140b27e60);
    if ((((iVar1 < 1) && (lVar3 = FUN_140294a40(&DAT_140a16fd0), lVar3 != 0)) &&
        (*(longlong *)(lVar3 + 0x68) != 0)) &&
       (lVar3 = FUN_14033e920(*(longlong *)(lVar3 + 0x68),param_2), lVar3 != 0)) {
      FUN_1402d5290(&DAT_140b27e60,lVar3,0,0,0);
      return 1;
    }
  }
  return 0;
}




// ================================================================
// SLOT 45 — FUN_1403613a0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3613a0  Name: FUN_1403613a0  Size: 203 bytes
// ------------------------------------------------------------

undefined4 FUN_1403613a0(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  char cVar2;
  int iVar3;
  longlong lVar4;
  
  if ((param_2 != 0) && (lVar4 = FUN_140294a40(&DAT_140a16fd0), lVar4 != 0)) {
    uVar1 = FUN_140294a40(&DAT_140a16fd0);
    cVar2 = FUN_1402824d0(uVar1,*(undefined4 *)(param_2 + 0x58));
    if ((cVar2 != '\0') && (*(char *)(param_1 + 0x278) != '\x01')) {
      iVar3 = FUN_14028a780(&DAT_140b153e0);
      if ((*(int *)(param_2 + 0x58) != iVar3) &&
         (iVar3 = FUN_14028a910(&DAT_140b153e0), *(int *)(param_2 + 0x58) != iVar3)) {
        return 0;
      }
      *(undefined1 *)(param_1 + 0x278) = 1;
      lVar4 = FUN_140294a40(&DAT_140a16fd0);
      lVar4 = *(longlong *)(*(longlong *)(lVar4 + 0x68) + 0x2338);
      if (lVar4 != 0) {
        FUN_1402d5290(&DAT_140b27e60,lVar4,0,0,0);
        return 1;
      }
    }
  }
  return 0;
}




// ================================================================
// SLOT 56 — FUN_140364ef0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x364ef0  Name: FUN_140364ef0  Size: 8 bytes
// ------------------------------------------------------------

void FUN_140364ef0(void)

{
  undefined8 in_R9;
  
  FUN_1401f9440(in_R9);
  return;
}




// ================================================================
// SLOT 74 — FUN_140365540
// ================================================================

// ------------------------------------------------------------
// RVA: 0x365540  Name: FUN_140365540  Size: 238 bytes
// ------------------------------------------------------------

void FUN_140365540(longlong param_1,undefined1 param_2)

{
  longlong lVar1;
  longlong lVar2;
  uint uVar3;
  ulonglong uVar5;
  ulonglong uVar4;
  
  uVar4 = 0;
  lVar2 = *(longlong *)(param_1 + 0x198);
  if (lVar2 != 0) {
    uVar5 = uVar4;
    if (0 < *(int *)(param_1 + 400)) {
      do {
        lVar2 = *(longlong *)(param_1 + 0x198);
        lVar1 = *(longlong *)(uVar5 + lVar2);
        if (lVar1 != 0) {
          FUN_1404c0ac0(lVar1);
          thunk_FUN_1407864b8(lVar1,0x58);
          lVar2 = *(longlong *)(param_1 + 0x198);
        }
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        uVar5 = uVar5 + 8;
      } while ((int)uVar3 < *(int *)(param_1 + 400));
    }
    thunk_FUN_1407864b8(lVar2);
    *(undefined8 *)(param_1 + 0x198) = 0;
    *(undefined4 *)(param_1 + 400) = 0;
  }
  if (*(longlong *)(param_1 + 0x1b0) != 0) {
    thunk_FUN_1407864b8();
    *(undefined8 *)(param_1 + 0x1b0) = 0;
  }
  lVar2 = *(longlong *)(param_1 + 0x1a0);
  if (lVar2 != 0) {
    FUN_1407da9e0(lVar2);
    thunk_FUN_1407864b8(lVar2,0x1390);
    *(undefined8 *)(param_1 + 0x1a0) = 0;
  }
  FUN_1404dd180(param_2);
  return;
}




// ================================================================
// SLOT 75 — FUN_14035cf20
// ================================================================

// ------------------------------------------------------------
// RVA: 0x35cf20  Name: FUN_14035cf20  Size: 152 bytes
// ------------------------------------------------------------

void FUN_14035cf20(longlong param_1,longlong param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  
  if (*(char *)(*(longlong *)(param_2 + 0x298) + 0x76) != '\0') {
    fVar5 = *(float *)(param_1 + 0x14) - *(float *)(*(longlong *)(param_2 + 0x298) + 0x1f94);
    fVar4 = DAT_1408054d0;
    if ((DAT_1408054d0 <= fVar5) && (fVar4 = DAT_1407ffaf8, fVar5 <= DAT_1407ffaf8)) {
      fVar4 = fVar5;
    }
    *(float *)(param_1 + 0x14) = fVar4;
  }
  if ((*(char *)(param_2 + 0x361) != -1) || (*(char *)(param_2 + 0x369) != -1)) {
    iVar1 = *(int *)(param_2 + 0x58);
    if ((-1 < iVar1) &&
       ((iVar1 < *(int *)(param_1 + 0x1e8) && (*(longlong *)(param_2 + 0x298) != 0)))) {
      iVar2 = *(int *)(*(longlong *)(param_2 + 0x298) + 0x1de4);
      if ((-1 < iVar2) &&
         ((iVar2 < *(int *)(*(longlong *)(param_1 + 0x1e0) + 0x10 + (longlong)iVar1 * 0x18) &&
          (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x1e0) + 8 + (longlong)iVar1 * 0x18),
          *(longlong *)(lVar3 + (longlong)iVar2 * 8) == param_2)))) {
        *(undefined8 *)(lVar3 + (longlong)iVar2 * 8) = 0;
      }
    }
  }
  return;
}




// ================================================================
// SLOT 78 — FUN_140364f30
// ================================================================

// ------------------------------------------------------------
// RVA: 0x364f30  Name: FUN_140364f30  Size: 529 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140364f30(undefined8 param_1,longlong param_2,char param_3,int param_4)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  DWORD DVar4;
  longlong *plVar5;
  longlong lVar6;
  undefined8 *puVar7;
  uint uVar8;
  char *pcVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  int iVar12;
  longlong local_68 [2];
  undefined8 local_58;
  ulonglong local_50;
  longlong local_48 [8];
  
  if (DAT_140b28f48 != 0) {
    local_50 = 7;
    local_68[0] = 0;
    local_58 = 0;
    if (param_2 != 0) {
      lVar1 = *(longlong *)(param_2 + 0x298);
      lVar6 = lVar1 + 0x140;
      if (7 < *(ulonglong *)(lVar1 + 0x158)) {
        lVar6 = *(longlong *)(lVar1 + 0x140);
      }
      FUN_14002d480(local_68,lVar6,*(undefined8 *)(lVar1 + 0x150));
    }
    cVar2 = FUN_14028af60(&DAT_140b153e0);
    if (cVar2 == '\0') {
      pcVar9 = "TEXT_TACTICAL_LAND_BATTLE_IS_PENDING";
      if (param_3 != '\x01') {
        pcVar9 = "TEXT_TACTICAL_SPACE_BATTLE_IS_PENDING";
      }
      plVar5 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,pcVar9);
      plVar11 = plVar5;
      if (7 < (ulonglong)plVar5[3]) {
        plVar11 = (longlong *)*plVar5;
      }
      lVar6 = plVar5[2];
    }
    else {
      FUN_14002d480(local_68,&DAT_1408008b4,2);
      uVar3 = FUN_140294a70(&DAT_140a16fd0);
      lVar6 = FUN_140490780(&DAT_140b15560,uVar3);
      if ((lVar6 == 0) || (*(int *)(lVar6 + 4) == 0)) {
        pcVar9 = "TEXT_MP_TACTICAL_LAND_BATTLE_IS_PENDING";
        if (param_3 == '\0') {
          pcVar9 = "TEXT_MP_TACTICAL_SPACE_BATTLE_IS_PENDING";
        }
      }
      else {
        pcVar9 = "TEXT_WAITING_FOR_OPPONENT";
      }
      puVar7 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,pcVar9);
      puVar10 = puVar7;
      if (7 < (ulonglong)puVar7[3]) {
        puVar10 = (undefined8 *)*puVar7;
      }
      FUN_14002d480(local_68,puVar10,puVar7[2]);
      DVar4 = timeGetTime();
      uVar8 = ((DVar4 - _DAT_140a7bec0) - param_4) / 1000;
      if (DAT_140b27628 <= uVar8) {
        uVar8 = DAT_140b27628;
      }
      iVar12 = DAT_140b27628 - uVar8;
      FUN_14002d480(local_68,&DAT_1408005e4,1);
      FUN_140022b30(local_48,0x20,&DAT_1407ffa20,iVar12);
      lVar6 = -1;
      do {
        lVar6 = lVar6 + 1;
      } while (*(short *)((longlong)local_48 + lVar6 * 2) != 0);
      plVar11 = local_48;
    }
    FUN_14002d480(local_68,plVar11,lVar6);
    FUN_1403deac0(&DAT_140b27f60,local_68);
    if (7 < local_50) {
      if (0xfff < local_50 * 2 + 2) {
        if (0x1f < (local_68[0] - *(longlong *)(local_68[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_68[0] + -8),local_50 * 2 + 0x29);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}



