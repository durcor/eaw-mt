
// ================================================================
// FUN_14044b500 — global AI singleton tick from slot 3 (KEY CANDIDATE)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x44b500  Name: FUN_14044b500  Size: 79 bytes
// ------------------------------------------------------------

void FUN_14044b500(longlong param_1,longlong param_2)

{
  if ((param_2 != 0) && (*(longlong *)(param_1 + 0x30) != param_2)) {
    *(longlong *)(param_1 + 0x30) = param_2;
    if (*(char *)(param_1 + 1) != '\0') {
      *(undefined4 *)(param_1 + 0x58) = 0;
      return;
    }
    *(undefined1 *)(param_1 + 0x58) = DAT_140b2a5f9;
    *(bool *)(param_1 + 0x59) = DAT_140b2c198 != 0;
    *(bool *)(param_1 + 0x5a) = DAT_140b28d63 == '\0';
    *(bool *)(param_1 + 0x5b) = DAT_140b28d60 == '\0';
  }
  return;
}




// ================================================================
// FUN_1404d9df0 — per-frame update from slot 3
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d9df0  Name: FUN_1404d9df0  Size: 100 bytes
// ------------------------------------------------------------

void FUN_1404d9df0(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
  if (0 < iVar3) {
    do {
      lVar1 = FUN_140294bc0(&DAT_140a16fd0,iVar2);
      if (lVar1 != 0) {
        FUN_140285760(lVar1,param_2);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < iVar3);
  }
  return;
}




// ================================================================
// FUN_140288980 — per-player service from slot 3
// ================================================================

// ------------------------------------------------------------
// RVA: 0x288980  Name: FUN_140288980  Size: 510 bytes
// ------------------------------------------------------------

undefined4 FUN_140288980(longlong param_1,int param_2,char param_3,char param_4)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  int iVar7;
  ulonglong uVar8;
  uint uVar9;
  ulonglong uVar11;
  ulonglong uVar10;
  
  uVar11 = 0;
  if (param_2 < 1) {
    param_2 = 0;
  }
  iVar3 = *(int *)(param_1 + 0x88);
  if (param_2 < *(int *)(param_1 + 0x88)) {
    iVar3 = param_2;
  }
  *(int *)(param_1 + 0x84) = iVar3;
  FUN_14032e8c0(DAT_140b15418 + 0xa0,iVar3,param_1);
  bVar1 = false;
  if ((param_3 == '\x01') && (iVar3 = FUN_140408150(&DAT_140b2f020), bVar1 = false, 0 < iVar3)) {
    uVar10 = uVar11;
    do {
      lVar5 = FUN_140408130(&DAT_140b2f020,uVar10);
      if ((((lVar5 != 0) &&
           (cVar2 = FUN_1403751a0(lVar5,*(undefined8 *)(param_1 + 0x68)), cVar2 != '\0')) &&
          (*(int *)(lVar5 + 0x89c) < *(int *)(param_1 + 0x84))) && (*(char *)(lVar5 + 0xc1) != '\0')
         ) {
        iVar4 = *(int *)(param_1 + 0x1b8);
        if (0 < iVar4) {
          plVar6 = *(longlong **)(param_1 + 0x1b0);
          uVar8 = uVar11;
          do {
            if (*plVar6 == lVar5) goto LAB_140288b02;
            uVar8 = uVar8 + 1;
            plVar6 = plVar6 + 1;
          } while ((longlong)uVar8 < (longlong)iVar4);
        }
        if (*(char *)(lVar5 + 0xc2) != '\0') {
          if (0 < iVar4) {
            plVar6 = *(longlong **)(param_1 + 0x1b0);
            uVar8 = uVar11;
            do {
              if (*plVar6 == lVar5) goto LAB_140288aff;
              uVar8 = uVar8 + 1;
              plVar6 = plVar6 + 1;
            } while ((longlong)uVar8 < (longlong)iVar4);
          }
          iVar4 = *(int *)(param_1 + 0x1b8);
          iVar7 = *(int *)(param_1 + 0x1bc) * 2 >> 1;
          if (iVar7 <= iVar4) {
            iVar4 = iVar7 / 10;
            if (iVar7 / 10 < 10) {
              iVar4 = 10;
            }
            FUN_140047aa0(param_1 + 0x1a8,iVar4 + iVar7,0,1);
            iVar4 = *(int *)(param_1 + 0x1b8);
          }
          *(longlong *)(*(longlong *)(param_1 + 0x1b0) + (longlong)iVar4 * 8) = lVar5;
          *(int *)(param_1 + 0x1b8) = *(int *)(param_1 + 0x1b8) + 1;
LAB_140288aff:
          bVar1 = true;
        }
      }
LAB_140288b02:
      uVar9 = (int)uVar10 + 1;
      uVar10 = (ulonglong)uVar9;
    } while ((int)uVar9 < iVar3);
  }
  if (((param_4 == '\x01') && (bVar1)) &&
     ((iVar3 = FUN_140294a70(&DAT_140a16fd0), *(int *)(param_1 + 0x4c) == iVar3 &&
      ((*(longlong *)(param_1 + 0x68) != 0 &&
       (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x670), lVar5 != 0)))))) {
    FUN_1402d5290(&DAT_140b27e60,lVar5,0,0,0);
  }
  return *(undefined4 *)(param_1 + 0x84);
}




// ================================================================
// FUN_140220ed0 — signal dispatcher (event 0x23 = unit Think trigger?)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x220ed0  Name: FUN_140220ed0  Size: 26 bytes
// ------------------------------------------------------------

void FUN_140220ed0(undefined8 param_1,longlong param_2,undefined4 param_3,undefined8 param_4)

{
  if (param_2 != 0) {
    FUN_140240940(param_2,param_3,param_4);
    return;
  }
  return;
}




// ================================================================
// LandModeClass slot 5 — FUN_1403b7aa0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b7aa0  Name: FUN_1403b7aa0  Size: 76 bytes
// ------------------------------------------------------------

void FUN_1403b7aa0(undefined8 param_1)

{
  DAT_140b30391 = 0;
  FUN_1401378d0(0);
  FUN_14035cfc0(param_1);
  FUN_14021d090(&DAT_140a7d0a0,DAT_1407ffaf8);
  FUN_14021d0b0(&DAT_140a7d0a0,DAT_1407ffaf8);
  return;
}




// ================================================================
// LandModeClass slot 13 — FUN_1403b8950
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b8950  Name: FUN_1403b8950  Size: 617 bytes
// ------------------------------------------------------------

byte FUN_1403b8950(longlong param_1,undefined8 param_2,char param_3,int param_4,undefined4 param_5)

{
  longlong *plVar1;
  byte bVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined8 uVar5;
  int iVar6;
  
  iVar6 = 5;
  if (-1 < param_4) {
    iVar6 = param_4;
  }
  if (param_3 != '\0') goto LAB_1403b8ab5;
  lVar3 = FUN_140769c58(0x140);
  if (lVar3 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)FUN_140272100(lVar3);
  }
  plVar1 = *(longlong **)(param_1 + 0x328);
  if (plVar1 != plVar4) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    *(longlong **)(param_1 + 0x328) = plVar4;
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
    }
  }
  if (*(longlong **)(param_1 + 0x90) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x90) + 8))();
    *(undefined8 *)(param_1 + 0x90) = 0;
  }
  lVar3 = FUN_140769c58(0x168);
  if (lVar3 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)FUN_1402cdfe0(lVar3);
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
    }
  }
  plVar1 = *(longlong **)(param_1 + 0x330);
  if (plVar1 == plVar4) {
    if (plVar4 != (longlong *)0x0) goto LAB_1403b8a4d;
  }
  else {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    *(longlong **)(param_1 + 0x330) = plVar4;
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
LAB_1403b8a4d:
      (**(code **)(*plVar4 + 8))(plVar4);
    }
  }
  lVar3 = FUN_140769c58(0x28);
  uVar5 = 0;
  if (lVar3 != 0) {
    uVar5 = FUN_140425cf0(lVar3,1);
  }
  *(undefined8 *)(param_1 + 0x388) = uVar5;
  lVar3 = FUN_140769c58(0xe8);
  uVar5 = 0;
  if (lVar3 != 0) {
    uVar5 = FUN_1405045e0(lVar3);
  }
  *(undefined8 *)(param_1 + 0x390) = uVar5;
  lVar3 = FUN_140769c58(0x78);
  uVar5 = 0;
  if (lVar3 != 0) {
    uVar5 = FUN_140537b30(lVar3);
  }
  *(undefined8 *)(param_1 + 0x398) = uVar5;
LAB_1403b8ab5:
  bVar2 = FUN_14035f970(param_1,param_2,param_3,iVar6,param_5);
  FUN_140273090(*(undefined8 *)(param_1 + 0x328),1);
  FUN_1402753e0(*(undefined8 *)(param_1 + 0x328),DAT_14085d694,DAT_1408007f8);
  if (param_3 == '\0') {
    FUN_1402d03f0(*(undefined8 *)(param_1 + 0x330),*(undefined8 *)(param_1 + 0x90));
    FUN_140261ab0(*(undefined8 *)(param_1 + 0x90),DAT_1408054ec,DAT_1408054d0,DAT_1408181ec,
                  DAT_1408181f0,DAT_140805898,DAT_1408007fc);
    FUN_140261e00(*(undefined8 *)(param_1 + 0x90),0,DAT_1407ffaf8,DAT_1407ffaf8,0);
    DAT_140b3cc30 = 1;
    *(undefined8 *)(param_1 + 0x410) = 0;
  }
  return bVar2 & 1;
}




// ================================================================
// LandModeClass slot 14 — FUN_1403bafd0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3bafd0  Name: FUN_1403bafd0  Size: 755 bytes
// ------------------------------------------------------------

byte FUN_1403bafd0(longlong *param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined1 auVar2 [16];
  uint uVar3;
  uint uVar4;
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
  byte bVar21;
  byte bVar22;
  byte bVar23;
  byte bVar24;
  byte bVar25;
  byte bVar26;
  undefined8 uVar27;
  undefined4 *puVar28;
  longlong lVar29;
  ulonglong uVar30;
  longlong lVar31;
  ulonglong uVar32;
  undefined4 *puVar33;
  ulonglong uVar34;
  longlong lVar35;
  int iVar36;
  float local_48;
  float local_44;
  float local_40;
  float local_38;
  float local_34;
  float local_30;
  
  (**(code **)(*param_1 + 0x1e0))();
  bVar5 = FUN_14021fe20(param_2,0x100);
  FUN_140275730(param_1[0x65],param_2);
  bVar6 = FUN_14021feb0(param_2);
  bVar7 = FUN_14021fe20(param_2,0x101);
  FUN_140132370(param_1[0x67],param_2);
  bVar8 = FUN_14021feb0(param_2);
  bVar9 = FUN_14021fe20(param_2,0x10a);
  uVar4 = DAT_140b36b94;
  uVar3 = DAT_140b36b90;
  uVar30 = (ulonglong)DAT_140b36b94;
  lVar31 = (longlong)(int)DAT_140b36b90;
  iVar36 = DAT_140b36b94 * DAT_140b36b90;
  auVar2._8_8_ = 0;
  auVar2._0_8_ = (longlong)iVar36;
  uVar27 = SUB168(ZEXT816(4) * auVar2,0);
  if (SUB168(ZEXT816(4) * auVar2,8) != 0) {
    uVar27 = 0xffffffffffffffff;
  }
  puVar28 = (undefined4 *)thunk_FUN_140769c58(uVar27);
  if (0 < (int)uVar3) {
    lVar35 = 0;
    uVar32 = (ulonglong)uVar3;
    puVar33 = puVar28;
    do {
      if (0 < (int)uVar4) {
        lVar29 = lVar35;
        uVar34 = uVar30;
        do {
          lVar1 = lVar29 + 0x10;
          lVar29 = lVar29 + lVar31 * 0x1c;
          *puVar33 = *(undefined4 *)(lVar1 + DAT_140b36ba8);
          puVar33 = puVar33 + 1;
          uVar34 = uVar34 - 1;
        } while (uVar34 != 0);
      }
      lVar35 = lVar35 + 0x1c;
      uVar32 = uVar32 - 1;
    } while (uVar32 != 0);
  }
  FUN_1402200b0(param_2,puVar28,iVar36 * 4,0);
  thunk_FUN_1407864b8(puVar28);
  bVar10 = FUN_14021feb0(param_2);
  bVar11 = FUN_14021fe20(param_2,0x102);
  bVar12 = (**(code **)(*param_1 + 0x60))(param_1,param_2,1);
  bVar13 = FUN_14021feb0(param_2);
  bVar14 = FUN_14021fe20(param_2,0x103);
  local_40 = *(float *)(param_1 + 0x84) - *(float *)((longlong)param_1 + 0x42c);
  local_44 = *(float *)((longlong)param_1 + 0x41c) - *(float *)(param_1 + 0x85);
  local_48 = *(float *)(param_1 + 0x83) - *(float *)((longlong)param_1 + 0x424);
  local_30 = *(float *)((longlong)param_1 + 0x42c) + *(float *)(param_1 + 0x84);
  local_34 = *(float *)(param_1 + 0x85) + *(float *)((longlong)param_1 + 0x41c);
  local_38 = *(float *)((longlong)param_1 + 0x424) + *(float *)(param_1 + 0x83);
  bVar15 = FUN_14021ffa0(param_2,5);
  bVar16 = FUN_1402200b0(param_2,&local_48,0xc);
  bVar17 = FUN_140220030(param_2);
  bVar18 = FUN_14021ffa0(param_2,6);
  bVar19 = FUN_1402200b0(param_2,&local_38,0xc);
  bVar20 = FUN_140220030(param_2);
  bVar21 = FUN_14021feb0(param_2);
  bVar22 = FUN_14021fe20(param_2,0x10b);
  bVar23 = (**(code **)(*param_1 + 0x178))(param_1,param_2);
  bVar24 = FUN_14021feb0(param_2);
  bVar25 = FUN_14021fe20(param_2,0x10c);
  FUN_1402cfaf0(param_1[0x66],param_2);
  bVar26 = FUN_14021feb0(param_2);
  (**(code **)(*param_1 + 0x1d8))(param_1);
  return bVar5 & 1 & bVar6 & bVar7 & bVar8 & bVar9 & bVar10 & bVar11 & bVar12 & bVar13 & bVar14 &
         bVar15 & bVar16 & bVar17 & bVar18 & bVar19 & bVar20 & bVar21 & bVar22 & bVar23 & bVar24 &
         bVar25 & bVar26;
}




// ================================================================
// LandModeClass slot 20 — FUN_1403ba400
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3ba400  Name: FUN_1403ba400  Size: 1221 bytes
// ------------------------------------------------------------

void FUN_1403ba400(longlong *param_1)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  longlong *plVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  float fVar11;
  float fVar12;
  undefined8 local_398;
  undefined4 local_390;
  float local_38c;
  undefined8 local_388;
  undefined8 uStack_380;
  undefined8 local_378;
  undefined8 uStack_370;
  undefined4 local_368;
  undefined4 uStack_364;
  undefined4 uStack_360;
  undefined4 uStack_35c;
  undefined8 local_358;
  undefined8 uStack_350;
  undefined8 local_348;
  undefined8 uStack_340;
  undefined4 local_338;
  undefined4 uStack_334;
  undefined4 uStack_330;
  undefined4 uStack_32c;
  undefined1 local_2f8;
  undefined8 local_2f4;
  undefined4 local_2ec;
  float local_2e8;
  undefined1 local_2d4 [684];
  
  if (DAT_140b2c210 != 0) {
    cVar4 = FUN_14013d2c0();
    if (cVar4 == '\0') {
      iVar5 = thunk_FUN_140178a60();
      if (iVar5 < 1) {
        FUN_14013f1d0(1);
        uVar9 = 0;
      }
      else {
        uVar6 = thunk_FUN_140178a60();
        FUN_14013f1d0(uVar6);
        uVar9 = 0;
      }
    }
    else {
      iVar5 = FUN_14013d2a0(DAT_140b2c210);
      if ((iVar5 != DAT_140a284c0) || (DAT_140b15490 != '\0')) {
        (**(code **)(*param_1 + 0x288))(param_1);
      }
      uVar9 = FUN_14013d250(DAT_140b2c210);
    }
    FUN_14013da00(uVar9);
  }
  if (param_1[0x34] != 0) {
    cVar4 = FUN_14013ecd0();
    if (cVar4 != '\0') {
      iVar5 = FUN_140294a70(&DAT_140a16fd0);
      uVar9 = *(undefined8 *)(param_1[0x12] + 0x40);
      puVar2 = *(undefined8 **)(param_1[0x33] + (longlong)iVar5 * 8);
      uVar6 = *(undefined4 *)((longlong)puVar2 + 0x24);
      uVar1 = *(undefined4 *)(puVar2 + 5);
      uVar10 = *puVar2;
      uVar3 = puVar2[2];
      cVar4 = FUN_14035f790(param_1);
      fVar12 = (float)*(int *)(*(longlong *)(param_1[0x33] + (longlong)iVar5 * 8) + 0x4c) /
               DAT_1408526bc;
      uVar7 = FUN_1404c0dc0();
      FUN_1407db070(param_1[0x34],uVar9,uVar6,uVar1,uVar10,uVar3,uVar7,(cVar4 != '\0') + '\x01',
                    fVar12);
    }
  }
  (**(code **)(*param_1 + 0x2c0))(param_1);
  if (param_1[0x67] != 0) {
    plVar8 = (longlong *)(**(code **)(*(longlong *)param_1[0x65] + 0x78))();
    uVar9 = (**(code **)(*plVar8 + 0x98))(plVar8);
    fVar12 = DAT_140b169dc;
    cVar4 = FUN_140136840(param_1[0x67]);
    if (((cVar4 != '\0') && (DAT_140b2c1f8 != 0)) && (DAT_140b2c208 != 0)) {
      local_398 = 0;
      local_390 = 0x3f800000;
      local_38c = (float)FUN_140136650(param_1[0x67]);
      FUN_140140280(&local_358,*(undefined8 *)(param_1[0x12] + 0x40));
      local_388 = local_358;
      uStack_380 = uStack_350;
      local_378 = local_348;
      uStack_370 = uStack_340;
      local_368 = local_338;
      uStack_364 = uStack_334;
      uStack_360 = uStack_330;
      uStack_35c = uStack_32c;
      FUN_14022d990(&local_388,&local_398);
      local_358 = local_388;
      uStack_350 = uStack_380;
      local_348 = local_378;
      uStack_340 = uStack_370;
      local_338 = local_368;
      uStack_334 = uStack_364;
      uStack_330 = uStack_360;
      uStack_32c = uStack_35c;
      FUN_14022d390(&local_358,local_2d4);
      FUN_1401417c0(&local_358);
      local_38c = local_38c - fVar12;
      local_2f4 = local_398;
      local_2ec = local_390;
      local_2f8 = 1;
      local_2e8 = local_38c;
      cVar4 = FUN_140136880(param_1[0x67]);
      if (cVar4 == '\0') {
        uVar10 = FUN_1402733e0(param_1[0x65],0xffffffff);
        FUN_14013cf40(uVar10);
      }
      else {
        FUN_14013cf40(&DAT_140a28818);
        FUN_1403bbd00(param_1,0);
      }
      (**(code **)(*plVar8 + 0x90))(plVar8,&local_358);
      cVar4 = FUN_14013ce10(DAT_140b2c1f8,DAT_140b2c208,1,1,1);
      if (cVar4 != '\0') {
        (**(code **)(*(longlong *)param_1[0x65] + 0x50))((longlong *)param_1[0x65],1);
        thunk_FUN_1401781f0();
        uVar10 = FUN_14013d250(DAT_140b2c1f8);
        thunk_FUN_140196ef0(uVar10);
      }
      FUN_1403bbd00(param_1,1);
      FUN_140140500(&local_358);
    }
    cVar4 = FUN_140136860(param_1[0x67]);
    if (((cVar4 != '\0') && (DAT_140b2c200 != 0)) && (DAT_140b2c208 != 0)) {
      fVar11 = (float)FUN_140136650(param_1[0x67]);
      FUN_140140280(&local_358,*(undefined8 *)(param_1[0x12] + 0x40));
      local_2f8 = 1;
      local_2f4 = 0;
      local_2ec = 0x3f800000;
      local_2e8 = fVar11 + fVar12;
      (**(code **)(*plVar8 + 0x90))(plVar8,&local_358);
      uVar10 = FUN_1402733e0(param_1[0x65],0xffffffff);
      FUN_14013cf40(uVar10);
      cVar4 = FUN_14013ce10(DAT_140b2c200,DAT_140b2c208,1,1,1);
      if (cVar4 != '\0') {
        (**(code **)(*(longlong *)param_1[0x65] + 0x50))((longlong *)param_1[0x65],2);
        thunk_FUN_1401781f0();
        uVar10 = FUN_14013d250(DAT_140b2c200);
        thunk_FUN_140196f40(uVar10);
      }
      FUN_140140500(&local_358);
    }
    (**(code **)(*plVar8 + 0x90))(plVar8,uVar9);
  }
  return;
}




// ================================================================
// LandModeClass slot 22 — FUN_1403bb440
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3bb440  Name: FUN_1403bb440  Size: 598 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403bb440(longlong param_1,undefined4 param_2)

{
  char *pcVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  longlong lVar6;
  longlong *plVar7;
  undefined **local_res8;
  
  cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar2 != '\0') {
    FUN_1402cd870(&DAT_140b27a10);
  }
  plVar7 = (longlong *)**(longlong **)(param_1 + 0x350);
  if (plVar7 != *(longlong **)(param_1 + 0x350)) {
    do {
      if (plVar7[3] != 0) {
        FUN_140527dd0(plVar7[3],param_2);
        iVar3 = FUN_140294a70(&DAT_140a16fd0);
        if ((int)plVar7[2] == iVar3) {
          cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
          if (cVar2 == '\0') {
            DAT_140b2c19c = FUN_1405275f0(plVar7[3]);
          }
        }
      }
      plVar7 = (longlong *)*plVar7;
    } while (plVar7 != (longlong *)*(longlong *)(param_1 + 0x350));
  }
  iVar3 = 0;
  if (DAT_140a286e0 == 0) {
    lVar6 = FUN_140769c58(0x200);
    if (lVar6 == 0) {
      DAT_140a286e0 = 0;
    }
    else {
      DAT_140a286e0 = FUN_140596850(lVar6);
    }
    local_res8 = &PTR_vftable_140a286d8;
    if (DAT_140a7bc10 == DAT_140a7bc18) {
      FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
    }
    else {
      *DAT_140a7bc10 = &PTR_vftable_140a286d8;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
    }
  }
  FUN_14059d270(DAT_140a286e0);
  if (*(longlong *)(param_1 + 0x328) != 0) {
    FUN_14046a0c0(&DAT_140b30b40,param_2);
  }
  FUN_14056ce20(param_1 + 0x3f8,param_2);
  iVar4 = *(int *)(param_1 + 0x410) + 1;
  *(int *)(param_1 + 0x410) = iVar4;
  if (DAT_140b27dcc == 7) {
    if ((float)DAT_140b0a340 * _DAT_140b301f8 <= (float)iVar4) {
      FUN_14028ee00(&DAT_140b153e0,0);
    }
  }
  if (0 < *(int *)(param_1 + 0x414)) {
    *(int *)(param_1 + 0x414) = *(int *)(param_1 + 0x414) + -1;
  }
  if (*(longlong *)(param_1 + 0x38) != 0) {
    iVar4 = FUN_140294a70(&DAT_140a16fd0);
    pcVar1 = *(char **)(param_1 + 0x38);
    if ((((*pcVar1 != '\0') && (-1 < iVar4)) && (iVar4 < *(int *)(pcVar1 + 0x28))) &&
       (*(char *)((longlong)iVar4 * 0x78 + 0x28 + *(longlong *)(pcVar1 + 0x20)) == '\x01')) {
      *(undefined4 *)(param_1 + 0x410) = 0;
    }
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    uVar5 = FUN_140294a70(&DAT_140a16fd0);
    cVar2 = FUN_1405289e0(*(undefined8 *)(param_1 + 0x40),uVar5);
    if (cVar2 == '\x01') {
      *(undefined4 *)(param_1 + 0x410) = 0;
    }
  }
  if ((*(longlong *)(param_1 + 0x28) != 0) && (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3))) {
    do {
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,iVar3);
      if (lVar6 != 0) {
        FUN_140346460(*(undefined8 *)(param_1 + 0x28),0x11,*(undefined4 *)(lVar6 + 0x4c));
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  FUN_1403639d0(param_1,param_2);
  return;
}




// ================================================================
// SpaceModeClass slot 13 — FUN_1404d7da0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d7da0  Name: FUN_1404d7da0  Size: 776 bytes
// ------------------------------------------------------------

byte FUN_1404d7da0(longlong param_1,undefined8 param_2,char param_3,int param_4,undefined4 param_5)

{
  longlong *plVar1;
  byte bVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined8 uVar5;
  int iVar6;
  
  iVar6 = 5;
  if (-1 < param_4) {
    iVar6 = param_4;
  }
  if (param_3 != '\0') goto LAB_1404d7f30;
  lVar3 = FUN_140769c58(0x140);
  if (lVar3 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)FUN_140272100(lVar3);
  }
  plVar1 = *(longlong **)(param_1 + 0x328);
  if (plVar1 != plVar4) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    *(longlong **)(param_1 + 0x328) = plVar4;
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
    }
  }
  lVar3 = FUN_140769c58(0x98);
  if (lVar3 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)FUN_140266940(lVar3);
  }
  plVar1 = *(longlong **)(param_1 + 800);
  if (plVar1 != plVar4) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    *(longlong **)(param_1 + 800) = plVar4;
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
    }
  }
  if (*(longlong **)(param_1 + 0x90) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0x90) + 8))();
    *(undefined8 *)(param_1 + 0x90) = 0;
  }
  lVar3 = FUN_140769c58(0x168);
  if (lVar3 == 0) {
    plVar4 = (longlong *)0x0;
  }
  else {
    plVar4 = (longlong *)FUN_1402cdfe0(lVar3);
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
    }
  }
  plVar1 = *(longlong **)(param_1 + 0x330);
  if (plVar1 == plVar4) {
    if (plVar4 != (longlong *)0x0) goto LAB_1404d7ee6;
  }
  else {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    *(longlong **)(param_1 + 0x330) = plVar4;
    if (plVar4 != (longlong *)0x0) {
      (**(code **)*plVar4)(plVar4);
LAB_1404d7ee6:
      (**(code **)(*plVar4 + 8))(plVar4);
    }
  }
  lVar3 = FUN_140769c58(0x28);
  uVar5 = 0;
  if (lVar3 != 0) {
    uVar5 = FUN_140425cf0(lVar3,2);
  }
  *(undefined8 *)(param_1 + 0x380) = uVar5;
  lVar3 = FUN_140769c58(0x78);
  uVar5 = 0;
  if (lVar3 != 0) {
    uVar5 = FUN_140537b30(lVar3);
  }
  *(undefined8 *)(param_1 + 0x388) = uVar5;
LAB_1404d7f30:
  bVar2 = FUN_14035f970(param_1,param_2,param_3,iVar6,param_5);
  if (*(longlong *)(param_1 + 0x200) != 0) {
    FUN_140360d70(param_1,DAT_1408054d0);
  }
  FUN_140273090(*(undefined8 *)(param_1 + 0x328),1);
  FUN_1402753e0(*(undefined8 *)(param_1 + 0x328),DAT_1408007f0,DAT_1408007f8);
  if (*(longlong *)(param_1 + 0x200) != 0) {
    FUN_140360d70(param_1,DAT_1408007c0);
  }
  if (param_3 == '\0') {
    FUN_140261ab0(*(undefined8 *)(param_1 + 0x90),DAT_1408054ec,DAT_1408054d0,DAT_1408181ec,
                  DAT_1408181f0,DAT_140805898,DAT_1408007fc);
    FUN_140261e00(*(undefined8 *)(param_1 + 0x90),0,DAT_1407ffaf8,DAT_1407ffaf8,0);
    *(undefined **)(*(longlong *)(param_1 + 0x330) + 0x148) = &DAT_140a2a4f0;
    FUN_1402d03f0(*(undefined8 *)(param_1 + 0x330),*(undefined8 *)(param_1 + 0x90));
    FUN_1402cee30(*(undefined8 *)(param_1 + 0x330),DAT_140b15848,1);
    FUN_1402d03f0(*(undefined8 *)(param_1 + 0x330),*(undefined8 *)(param_1 + 0x90));
    DAT_140b3cc30 = param_3;
    *(undefined8 *)(param_1 + 0x3c0) = 0;
  }
  return bVar2 & 1;
}




// ================================================================
// SpaceModeClass slot 20 — FUN_1404d8de0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d8de0  Name: FUN_1404d8de0  Size: 438 bytes
// ------------------------------------------------------------

void FUN_1404d8de0(longlong *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  char cVar7;
  int iVar8;
  undefined4 uVar9;
  longlong lVar10;
  float fVar11;
  undefined **local_48;
  longlong *local_40;
  undefined4 local_38;
  
  if (param_1[0x34] != 0) {
    cVar7 = FUN_14013ecd0();
    if (cVar7 == '\0') {
      lVar10 = FUN_1407dac70(param_1[0x34]);
      *(byte *)(lVar10 + 0x30) = *(byte *)(lVar10 + 0x30) | 1;
    }
    else {
      lVar10 = FUN_1407dac70(param_1[0x34]);
      *(byte *)(lVar10 + 0x30) = *(byte *)(lVar10 + 0x30) & 0xfe;
      FUN_1407dae70(param_1[0x34],DAT_140b169bc,0);
      iVar8 = FUN_140294a70(&DAT_140a16fd0);
      uVar3 = *(undefined8 *)(param_1[0x12] + 0x40);
      puVar4 = *(undefined8 **)(param_1[0x33] + (longlong)iVar8 * 8);
      uVar1 = *(undefined4 *)((longlong)puVar4 + 0x24);
      uVar2 = *(undefined4 *)(puVar4 + 5);
      uVar5 = *puVar4;
      uVar6 = puVar4[2];
      local_48 = SpaceReinforceFOWSampler::vftable;
      local_40 = param_1;
      local_38 = FUN_140294a70(&DAT_140a16fd0);
      if (DAT_140b169d4 == '\0') {
        cVar7 = FUN_1402f9c30(&DAT_140b27f60);
      }
      else {
        cVar7 = *(char *)((longlong)param_1 + 0x2cc);
      }
      fVar11 = (float)*(int *)(*(longlong *)(param_1[0x33] + (longlong)iVar8 * 8) + 0x4c) /
               DAT_1408526bc;
      if (cVar7 == '\0') {
        uVar9 = FUN_1404c0dc0();
        FUN_1407db070(param_1[0x34],uVar3,uVar1,uVar2,uVar5,uVar6,uVar9,0,fVar11);
      }
      else {
        uVar9 = FUN_1404c0dc0();
        FUN_1407db250(param_1[0x34],uVar3,uVar1,uVar2,uVar5,uVar6,uVar9,3,fVar11,&local_48);
      }
    }
  }
                    /* WARNING: Could not recover jumptable at 0x0001404d8f8f. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x2c0))(param_1);
  return;
}



