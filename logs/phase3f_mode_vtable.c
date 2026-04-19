
// ================================================================
// SCENE MANAGER — FUN_14030c3b0 (3157 bytes, full)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x30c3b0  Name: FUN_14030c3b0  Size: 3157 bytes
// ------------------------------------------------------------

void FUN_14030c3b0(longlong param_1,int param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  undefined1 uVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  undefined4 uVar9;
  int iVar10;
  int iVar11;
  longlong lVar12;
  longlong *plVar13;
  longlong lVar14;
  longlong lVar15;
  undefined8 uVar16;
  undefined1 *puVar17;
  undefined8 *puVar18;
  uint uVar19;
  longlong lVar20;
  bool bVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float local_res18;
  float local_res1c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  
  uVar19 = param_2 - *(int *)(param_1 + 0x2128);
  if (*(char *)(param_1 + 0x2699) != '\0') {
    iVar8 = *(int *)(param_1 + 0x2144) - uVar19;
    *(int *)(param_1 + 0x2128) = param_2;
    if (iVar8 < 0) {
      iVar8 = 0;
    }
    *(int *)(param_1 + 0x598) = *(int *)(param_1 + 0x598) - uVar19;
    *(int *)(param_1 + 0x2144) = iVar8;
    return;
  }
  if (*(char *)(param_1 + 0x48) != '\0') {
    FUN_1402fd330();
  }
  lVar20 = 0;
  iVar8 = 0;
  if (*(char *)(param_1 + 0x2065) == '\0') {
    piVar1 = (int *)(param_1 + 0x2068);
    *piVar1 = *piVar1 - uVar19;
    iVar10 = *(int *)(param_1 + 0x2068);
    if (*piVar1 < 0) {
      iVar10 = iVar8;
    }
    *(int *)(param_1 + 0x2068) = iVar10;
  }
  uVar5 = FUN_14020e2b0();
  *(undefined1 *)(param_1 + 0x21a9) = uVar5;
  if ((DAT_140b153d5 == '\0') && (*(int *)(param_1 + 0x2968) == 2)) {
    FUN_1402f4a80(param_1);
  }
  fVar24 = DAT_1408007f8;
  fVar23 = *(float *)(param_1 + 0x2a10) - (float)uVar19 / DAT_1408007f8;
  *(float *)(param_1 + 0x2a10) = fVar23;
  if (0.0 < fVar23) {
    puVar17 = &DAT_00000008;
LAB_14030c4e6:
    FUN_1402f4ca0(param_1,puVar17);
  }
  else {
    *(undefined4 *)(param_1 + 0x2a10) = 0;
    if (0 < *(int *)(param_1 + 0x2a28)) {
      puVar17 = (undefined1 *)0xfffffff8;
      goto LAB_14030c4e6;
    }
    if (*(longlong *)(param_1 + ((longlong)*(int *)(param_1 + 0x211c) + 0x10a) * 0x28) != 0) {
      FUN_1402e4260(param_1);
    }
  }
  FUN_140305c40(param_1);
  fVar23 = DAT_1407ffaf8;
  fVar24 = (float)uVar19 / fVar24;
  fVar22 = *(float *)(param_1 + 0x5cc) - fVar24;
  *(float *)(param_1 + 0x5cc) = fVar22;
  if (*(longlong *)(param_1 + 0x590) == 0) {
    *(undefined2 *)(param_1 + 0x5c4) = 0;
    *(undefined4 *)(param_1 + 0x5c0) = 0;
LAB_14030c5d6:
    *(undefined1 *)(param_1 + 0x26b3) = 0;
  }
  else if (*(char *)(param_1 + 0x5c4) == '\0') {
    if ((*(char *)(param_1 + 0x5c5) != '\0') && (fVar22 <= 0.0)) {
      if (0.0 < *(float *)(param_1 + 0x5c0)) {
        *(float *)(param_1 + 0x5c0) =
             *(float *)(param_1 + 0x5c0) - fVar24 * *(float *)(param_1 + 0x5c8);
      }
      if (*(float *)(param_1 + 0x5c0) <= 0.0) {
        *(undefined1 *)(param_1 + 0x5c5) = 0;
        *(undefined4 *)(param_1 + 0x5c0) = 0;
        FUN_1402e7620(param_1);
        goto LAB_14030c5d6;
      }
    }
  }
  else {
    if (*(float *)(param_1 + 0x5c0) <= fVar23 && fVar23 != *(float *)(param_1 + 0x5c0)) {
      *(float *)(param_1 + 0x5c0) =
           fVar24 * *(float *)(param_1 + 0x5c8) + *(float *)(param_1 + 0x5c0);
    }
    if (fVar23 <= *(float *)(param_1 + 0x5c0)) {
      *(undefined4 *)(param_1 + 0x5c0) = 0x3f800000;
      *(undefined1 *)(param_1 + 0x5c4) = 0;
    }
  }
  if (*(char *)(param_1 + 0x26b1) == '\0') {
    bVar21 = *(char *)(param_1 + 0x26d8) == '\0';
LAB_14030c635:
    if (!bVar21) {
      FUN_14025c490(*(undefined8 *)(param_1 + 0xff0),*(undefined8 *)(param_1 + 0xdd0));
      *(undefined1 *)(param_1 + 0x26d8) = 0;
    }
  }
  else {
    cVar6 = FUN_1402cd0c0(&DAT_140b27a10);
    cVar7 = *(char *)(param_1 + 0x26d8);
    if (cVar6 != '\0') {
      bVar21 = cVar7 == '\0';
      goto LAB_14030c635;
    }
    if (*(char *)(param_1 + 0x2a34) != '\0') {
      bVar21 = cVar7 == '\0';
      goto LAB_14030c635;
    }
    if (cVar7 == '\0') {
      FUN_14025c080(*(undefined8 *)(param_1 + 0xff0),*(undefined8 *)(param_1 + 0xdd0));
      *(undefined1 *)(param_1 + 0x26d8) = 1;
    }
  }
  lVar12 = FUN_140294a40(&DAT_140a16fd0);
  if ((lVar12 != 0) &&
     (lVar12 = *(longlong *)(lVar12 + 0x68), lVar12 != *(longlong *)(param_1 + 0x2b20))) {
    *(longlong *)(param_1 + 0x2b20) = lVar12;
    puVar18 = (undefined8 *)(lVar12 + 0x358);
    if (puVar18 != &DAT_140a28450) {
      if (0xf < *(ulonglong *)(lVar12 + 0x370)) {
        puVar18 = (undefined8 *)*puVar18;
      }
      FUN_140022730(&DAT_140a28450,puVar18,*(undefined8 *)(lVar12 + 0x368));
    }
    plVar13 = (longlong *)**(longlong **)(param_1 + 0x640);
    if (plVar13 != *(longlong **)(param_1 + 0x640)) {
      do {
        plVar3 = (longlong *)plVar13[5];
        if ((plVar3 != (longlong *)0x0) &&
           (lVar12 = (**(code **)(*plVar3 + 0xf8))(plVar3), lVar12 != 0)) {
          lVar15 = -1;
          do {
            lVar15 = lVar15 + 1;
          } while (*(char *)(lVar12 + lVar15) != '\0');
          if (lVar15 != 0) {
            cVar7 = (**(code **)(*plVar3 + 0x108))(plVar3);
            if (cVar7 != '\0') {
              lVar12 = lVar12 + 2;
            }
            (**(code **)(*plVar3 + 0xa0))(plVar3,lVar12,0);
          }
        }
        plVar3 = (longlong *)plVar13[2];
        if (*(char *)((longlong)plVar3 + 0x19) == '\0') {
          cVar7 = *(char *)(*plVar3 + 0x19);
          plVar13 = plVar3;
          plVar3 = (longlong *)*plVar3;
          while (cVar7 == '\0') {
            cVar7 = *(char *)(*plVar3 + 0x19);
            plVar13 = plVar3;
            plVar3 = (longlong *)*plVar3;
          }
        }
        else {
          cVar7 = *(char *)(plVar13[1] + 0x19);
          plVar4 = (longlong *)plVar13[1];
          plVar3 = plVar13;
          while ((plVar13 = plVar4, cVar7 == '\0' && (plVar3 == (longlong *)plVar13[2]))) {
            cVar7 = *(char *)(plVar13[1] + 0x19);
            plVar4 = (longlong *)plVar13[1];
            plVar3 = plVar13;
          }
        }
      } while (plVar13 != *(longlong **)(param_1 + 0x640));
    }
  }
  if (*(char *)(param_1 + 0x2120) == '\0') goto LAB_14030cbda;
  if (*(int *)(param_1 + 0x211c) == 1) {
    FUN_1403e8a80(param_1);
    FUN_1403e83a0(param_1);
    FUN_1403145c0(param_1);
    FUN_1403edd00(param_1);
    FUN_1403e5aa0(param_1);
    FUN_140310e40(param_1);
    FUN_1403e4cb0(param_1);
    FUN_1403e6ff0(param_1);
    FUN_1403e6630(param_1);
    FUN_1403e79b0(param_1);
    lVar12 = FUN_140294a40(&DAT_140a16fd0);
    if ((*(char *)(lVar12 + 0x3c0) != '\0') ||
       (cVar7 = FUN_14028af60(&DAT_140b153e0), cVar7 != '\0')) {
      plVar13 = (longlong *)FUN_140347b50(0x26a);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
      uVar16 = 0x26b;
LAB_14030cb81:
      plVar13 = (longlong *)FUN_140347b50(uVar16);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
    }
  }
  else {
    if (*(int *)(param_1 + 0x211c) == 0) goto LAB_14030cb9a;
    FUN_140317480(param_1);
    if (*(int *)(param_1 + 0x211c) == 2) {
      FUN_140317d00(param_1);
LAB_14030cb28:
      FUN_140316260(param_1);
    }
    else if (*(int *)(param_1 + 0x211c) == 3) {
      FUN_140312570(param_1);
      puVar2 = (undefined4 *)(param_1 + 0x26dc);
      if (*(longlong *)(param_1 + 0x20c0) != 0) {
        iVar10 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if (((iVar10 == 1) && (DAT_140b15418 != (longlong *)0x0)) &&
           (lVar12 = DAT_140b15418[7], lVar12 != 0)) {
          FUN_1401f9340(&local_68,0,fVar23,0,fVar23);
          uVar9 = FUN_140294a70(&DAT_140a16fd0);
          cVar7 = FUN_140494540(lVar12,uVar9,puVar2);
          if (cVar7 == '\0') {
            puVar18 = (undefined8 *)FUN_1401f9340(&local_78,fVar23,0,0,fVar23);
            local_68 = *puVar18;
            uStack_60 = puVar18[1];
            FUN_1401f9440(&local_78);
          }
          puVar18 = (undefined8 *)FUN_1401f9320(&local_78,&local_68);
          lVar12 = *(longlong *)(param_1 + 0x20c0);
          uVar16 = puVar18[1];
          *(undefined8 *)(lVar12 + 0xb8) = *puVar18;
          *(undefined8 *)(lVar12 + 0xc0) = uVar16;
          FUN_1401f9440(puVar18);
          lVar12 = FUN_140294a40(&DAT_140a16fd0);
          local_74 = *(undefined4 *)(param_1 + 0x26e0);
          local_res18 = *(float *)(*(longlong *)(lVar12 + 0x68) + 0x26f8);
          local_78 = *puVar2;
          local_70 = *(undefined4 *)(param_1 + 0x26e4);
          local_res1c = local_res18;
          FUN_14012f570(*(undefined8 *)(param_1 + 0x20c0),&local_78,0,&local_res18,fVar23);
          FUN_14012fba0(*(undefined8 *)(param_1 + 0x20c0),1);
          FUN_1401f9440(&local_68);
        }
      }
      if (*(longlong *)(param_1 + 0x2ae8) != 0) {
        iVar10 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if (((iVar10 == 1) && (DAT_140b15418 != (longlong *)0x0)) &&
           (lVar12 = DAT_140b15418[8], lVar12 != 0)) {
          FUN_1401f9340(&local_68,0,fVar23,0,fVar23);
          uVar9 = FUN_140294a70(&DAT_140a16fd0);
          cVar7 = FUN_140528a70(lVar12,uVar9,puVar2);
          if (cVar7 == '\0') {
            puVar18 = (undefined8 *)FUN_1401f9340(&local_78,fVar23,0,0,fVar23);
            local_68 = *puVar18;
            uStack_60 = puVar18[1];
            FUN_1401f9440(&local_78);
          }
          puVar18 = (undefined8 *)FUN_1401f9320(&local_78,&local_68);
          lVar12 = *(longlong *)(param_1 + 0x2ae8);
          uVar16 = puVar18[1];
          *(undefined8 *)(lVar12 + 0xb8) = *puVar18;
          *(undefined8 *)(lVar12 + 0xc0) = uVar16;
          FUN_1401f9440(puVar18);
          lVar12 = FUN_140294a40(&DAT_140a16fd0);
          local_74 = *(undefined4 *)(param_1 + 0x26e0);
          local_res1c = *(float *)(*(longlong *)(lVar12 + 0x68) + 0x2720);
          local_res18 = local_res1c * DAT_1408007d4;
          local_res1c = local_res1c * DAT_1408007d4;
          local_78 = *puVar2;
          local_70 = *(undefined4 *)(param_1 + 0x26e4);
          FUN_14012f570(*(undefined8 *)(param_1 + 0x2ae8),&local_78,0,&local_res18,fVar23);
          FUN_14012fba0(*(undefined8 *)(param_1 + 0x2ae8),1);
          FUN_1401f9440(&local_68);
        }
      }
      goto LAB_14030cb28;
    }
    FUN_14031d360(param_1);
    plVar13 = (longlong *)FUN_140347b50(0x239);
    (**(code **)(*plVar13 + 0x78))(plVar13,1);
    cVar7 = FUN_14028af60(&DAT_140b153e0);
    if (cVar7 != '\0') {
      plVar13 = (longlong *)FUN_140347b50(0x26d);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x80))(plVar13,1);
      }
      uVar16 = 0x26c;
      goto LAB_14030cb81;
    }
  }
LAB_14030cb9a:
  iVar10 = FUN_14025e960();
  *(float *)(param_1 + 0x1000) = *(float *)(param_1 + 0xff8) / (float)iVar10;
  iVar10 = FUN_14025e930();
  *(float *)(param_1 + 0x1004) = *(float *)(param_1 + 0xffc) / (float)iVar10;
LAB_14030cbda:
  if (*(int *)(param_1 + 0x211c) == 1) {
    lVar12 = *(longlong *)(param_1 + 0x2030);
    if (((lVar12 != 0) && (*(char *)(lVar12 + 0x335) != -1)) &&
       (*(int *)(DAT_140b2eea0 + 0x44) == 0)) {
      FUN_1403a8820(lVar12,1);
      lVar12 = *(longlong *)(param_1 + 0x2030);
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(undefined4 *)(*(longlong *)(lVar12 + 0x1b0) + 0xc) = 0x40000000;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(undefined4 *)(*(longlong *)(lVar12 + 0x1b0) + 0x10) = 0x3f000000;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      if (*(longlong *)(lVar12 + 0x1b0) != 0) {
        *(float *)(*(longlong *)(lVar12 + 0x1b0) + 0x14) =
             (float)DAT_140b0a320 / (float)DAT_140b0a340;
        lVar12 = *(longlong *)(param_1 + 0x2030);
      }
      *(longlong *)(param_1 + 0xf98) = lVar12;
    }
    plVar13 = (longlong *)FUN_140347b50(0);
    lVar12 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2708);
    lVar15 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2728);
    lVar14 = FUN_140294a40(&DAT_140a16fd0);
    if (lVar12 == *(longlong *)(lVar14 + 0x68)) {
      (**(code **)(*plVar13 + 0x300))(plVar13,1);
    }
    else {
      if (lVar15 == *(longlong *)(lVar14 + 0x68)) {
        uVar16 = 0;
      }
      else {
        uVar16 = 2;
      }
      (**(code **)(*plVar13 + 0x300))(plVar13,uVar16);
    }
    FUN_140264c80(*(undefined8 *)(param_1 + 0x27a0),1);
    if (*(longlong *)(param_1 + 0x2160) != 0) {
      FUN_1403e1500(param_1);
      FUN_1403e1790(param_1,*(undefined8 *)(param_1 + 0x2160));
    }
  }
  else if (*(int *)(param_1 + 0x211c) != 0) {
    FUN_14031b590(param_1);
    FUN_14031acd0(param_1);
    FUN_140314360(param_1);
    FUN_14031a450(param_1);
  }
  if ((*(int *)(param_1 + 0x211c) != 0) &&
     (iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar10 == 0)) {
    FUN_1402fca50(param_1);
    iVar10 = iVar8;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar10 = (int)DAT_140b15418[2];
    }
    lVar12 = DAT_140b15418[0x6d];
    iVar11 = FUN_1404b1ff0();
    plVar13 = (longlong *)(param_1 + 0x21c8);
    iVar11 = (int)(((float)(iVar10 - (int)lVar12) / (float)iVar11) * DAT_140805898);
    iVar10 = iVar8;
    if (-1 < iVar11) {
      iVar10 = iVar11;
    }
    if (9 < iVar10) {
      iVar10 = 9;
    }
    lVar12 = 10;
    do {
      plVar3 = (longlong *)*plVar13;
      lVar15 = *plVar3;
      if (iVar10 < lVar20) {
        uVar16 = FUN_14034c200(plVar3,0);
        (**(code **)(lVar15 + 0xa0))(*plVar13,uVar16,0);
LAB_14030ce57:
        lVar15 = *plVar13;
        *(byte *)(lVar15 + 0xc0) = *(byte *)(lVar15 + 0xc0) & 0xef;
        *(undefined4 *)(lVar15 + 0xac) = 0;
        *(undefined4 *)(lVar15 + 0xb4) = 0;
      }
      else {
        FUN_14034c200(plVar3,1);
        (**(code **)(lVar15 + 0xa0))(*plVar13);
        if ((lVar20 != iVar10) || (*(int *)(param_1 + 0x2968) == 0)) goto LAB_14030ce57;
        (**(code **)(*(longlong *)*plVar13 + 0x120))((longlong *)*plVar13,0,1,0,0);
      }
      lVar20 = lVar20 + 1;
      plVar13 = plVar13 + 1;
      lVar12 = lVar12 + -1;
    } while (lVar12 != 0);
  }
  *(bool *)(param_1 + 0x2120) = *(char *)(param_1 + 0x2120) == '\0';
  if (*(longlong *)(param_1 + 0xf88) != 0) {
    cVar7 = FUN_1403d6b70(param_1);
    lVar20 = *(longlong *)(param_1 + 0xf88);
    if (cVar7 == '\0') {
      local_78 = *(undefined4 *)(lVar20 + 0x78);
      local_74 = *(undefined4 *)(lVar20 + 0x7c);
      local_70 = *(undefined4 *)(lVar20 + 0x80);
      FUN_140448c20(&DAT_140b30350,&local_78);
    }
    else {
      puVar18 = (undefined8 *)(*(longlong *)(lVar20 + 0x298) + 0xf8);
      if (0xf < *(ulonglong *)(*(longlong *)(lVar20 + 0x298) + 0x110)) {
        puVar18 = (undefined8 *)*puVar18;
      }
      FUN_140025760("\n## Delayed selection of planet %s\n",puVar18);
      FUN_1403de570(param_1,*(undefined8 *)(param_1 + 0xf88),0);
      *(undefined8 *)(param_1 + 0xf88) = 0;
    }
  }
  iVar10 = *(int *)(param_1 + 0x2144) - uVar19;
  *(int *)(param_1 + 0x2128) = param_2;
  if (iVar10 < 0) {
    iVar10 = iVar8;
  }
  *(int *)(param_1 + 0x598) = *(int *)(param_1 + 0x598) - uVar19;
  *(int *)(param_1 + 0x2144) = iVar10;
  if (DAT_140b30392 != '\0') {
    *(undefined2 *)(param_1 + 0x2218) = 0;
    *(undefined2 *)(param_1 + 0x2188) = 0;
    FUN_1403def00(param_1,0);
    if ((*(code **)(param_1 + 0x27d0) != (code *)0x0) && (*(char *)(param_1 + 0x21a8) != '\0')) {
      (**(code **)(param_1 + 0x27d0))();
      *(undefined1 *)(param_1 + 0x21a8) = 0;
    }
  }
  if (*(int *)(param_1 + 0x211c) - 2U < 2) {
    plVar13 = (longlong *)FUN_140347b50(0xe3);
    uVar16 = (**(code **)(*DAT_140b15418 + 0x1e8))();
    cVar7 = FUN_1402ce420(uVar16,0);
    if (cVar7 != '\0') {
      (**(code **)(*plVar13 + 0x1d0))(plVar13,2);
    }
  }
  return;
}




// ================================================================
// GALACTIC MODE CTOR — FUN_1404b1270
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4b1270  Name: FUN_1404b1270  Size: 122 bytes
// ------------------------------------------------------------

undefined8 * FUN_1404b1270(undefined8 *param_1)

{
  FUN_14035a5e0();
  *param_1 = GalacticModeClass::vftable;
  param_1[100] = 0;
  param_1[0x65] = 0;
  param_1[0x66] = 0;
  param_1[0x67] = 0;
  param_1[0x69] = 0;
  param_1[0x6b] = 0;
  param_1[0x6d] = 0;
  *(undefined1 *)(param_1 + 0x68) = 1;
  param_1[0x6c] = 0xf;
  *(undefined4 *)(param_1 + 0x6e) = 1;
  *(undefined4 *)((longlong)param_1 + 0x374) = 0x3fffff;
  return param_1;
}




// ================================================================
// SPACE MODE CTOR — FUN_1403b5210
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b5210  Name: FUN_1403b5210  Size: 355 bytes
// ------------------------------------------------------------

undefined8 * FUN_1403b5210(undefined8 *param_1)

{
  longlong lVar1;
  
  FUN_14035a5e0();
  *param_1 = LandModeClass::vftable;
  param_1[0x65] = 0;
  param_1[0x66] = 0;
  param_1[0x67] = 0;
  *(undefined4 *)(param_1 + 0x68) = 0;
  *(undefined1 *)(param_1 + 0x69) = 0;
  *(undefined4 *)((longlong)param_1 + 0x34c) = 0;
  param_1[0x6a] = 0;
  param_1[0x6b] = 0;
  lVar1 = FUN_140769c58(0x20);
  *(longlong *)lVar1 = lVar1;
  *(longlong *)(lVar1 + 8) = lVar1;
  param_1[0x6a] = lVar1;
  param_1[0x6c] = 0;
  param_1[0x6d] = 0;
  param_1[0x6e] = 0;
  param_1[0x6f] = 7;
  param_1[0x70] = 8;
  *(undefined4 *)((longlong)param_1 + 0x34c) = 0x3f800000;
  FUN_140048250(param_1 + 0x6c,0x10,param_1[0x6a]);
  param_1[0x71] = 0;
  param_1[0x72] = 0;
  param_1[0x74] = 0;
  param_1[0x75] = 0;
  param_1[0x76] = 0;
  *(undefined1 *)(param_1 + 0x77) = 0;
  *(undefined4 *)((longlong)param_1 + 0x3bc) = 0;
  param_1[0x78] = 0;
  param_1[0x79] = 0;
  lVar1 = FUN_140769c58(0x18);
  *(longlong *)lVar1 = lVar1;
  *(longlong *)(lVar1 + 8) = lVar1;
  param_1[0x78] = lVar1;
  param_1[0x7a] = 0;
  param_1[0x7b] = 0;
  param_1[0x7c] = 0;
  param_1[0x7d] = 7;
  param_1[0x7e] = 8;
  *(undefined4 *)((longlong)param_1 + 0x3bc) = 0x3f800000;
  FUN_140048250(param_1 + 0x7a,0x10,param_1[0x78]);
  FUN_14056c2b0(param_1 + 0x7f);
  param_1[0x82] = 0;
  return param_1;
}




// ================================================================
// LAND MODE CTOR — FUN_1404d6ba0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d6ba0  Name: FUN_1404d6ba0  Size: 761 bytes
// ------------------------------------------------------------

undefined8 * FUN_1404d6ba0(undefined8 *param_1)

{
  float fVar1;
  longlong lVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  FUN_14035a5e0();
  *param_1 = SpaceModeClass::vftable;
  param_1[100] = 0;
  param_1[0x65] = 0;
  param_1[0x66] = 0;
  *(undefined1 *)(param_1 + 0x68) = 0;
  *(undefined4 *)((longlong)param_1 + 0x344) = 0;
  param_1[0x69] = 0;
  param_1[0x6a] = 0;
  lVar2 = FUN_140769c58(0x20);
  *(longlong *)lVar2 = lVar2;
  *(longlong *)(lVar2 + 8) = lVar2;
  param_1[0x69] = lVar2;
  param_1[0x6b] = 0;
  param_1[0x6c] = 0;
  param_1[0x6d] = 0;
  param_1[0x6e] = 7;
  param_1[0x6f] = 8;
  *(undefined4 *)((longlong)param_1 + 0x344) = 0x3f800000;
  FUN_140048250(param_1 + 0x6b,0x10,param_1[0x69]);
  fVar4 = DAT_1408795e4;
  fVar1 = DAT_1408007c0;
  fVar3 = DAT_1408795e4 - DAT_1408795e4;
  param_1[0x70] = 0;
  param_1[0x71] = 0;
  param_1[0x78] = 0;
  *(float *)((longlong)param_1 + 0x394) = fVar3 * fVar1;
  *(float *)(param_1 + 0x72) = (fVar4 - fVar4) * fVar1;
  fVar3 = DAT_1408795e8;
  fVar5 = fVar4 - DAT_1408795e8;
  *(float *)(param_1 + 0x73) = (fVar4 - fVar4) * fVar1;
  *(float *)((longlong)param_1 + 0x39c) = ABS(fVar5);
  *(float *)(param_1 + 0x74) = ABS(fVar4 - fVar3);
  fVar5 = *(float *)(param_1 + 0x74) * fVar1;
  *(float *)((longlong)param_1 + 0x39c) = *(float *)((longlong)param_1 + 0x39c) * fVar1;
  *(float *)(param_1 + 0x74) = fVar5;
  *(float *)((longlong)param_1 + 0x3a4) = ABS(fVar4 - fVar3);
  fVar4 = *(float *)((longlong)param_1 + 0x3a4) * fVar1;
  *(float *)((longlong)param_1 + 0x3a4) = fVar4;
  fVar4 = *(float *)(param_1 + 0x73) - fVar4;
  fVar6 = *(float *)((longlong)param_1 + 0x3a4) + *(float *)(param_1 + 0x73);
  fVar8 = (*(float *)((longlong)param_1 + 0x394) - fVar5) + DAT_1408657a4;
  fVar3 = (fVar5 + *(float *)((longlong)param_1 + 0x394)) - DAT_1408657a4;
  fVar7 = (*(float *)(param_1 + 0x72) - *(float *)((longlong)param_1 + 0x39c)) + DAT_1408657a4;
  fVar5 = (*(float *)((longlong)param_1 + 0x39c) + *(float *)(param_1 + 0x72)) - DAT_1408657a4;
  *(float *)((longlong)param_1 + 0x3ac) = (fVar8 + fVar3) * fVar1;
  *(float *)(param_1 + 0x75) = (fVar7 + fVar5) * fVar1;
  *(float *)((longlong)param_1 + 0x3b4) = ABS(fVar5 - fVar7);
  *(float *)(param_1 + 0x77) = ABS(fVar3 - fVar8);
  *(float *)(param_1 + 0x76) = (fVar6 + fVar4) * fVar1;
  *(float *)((longlong)param_1 + 0x3b4) = *(float *)((longlong)param_1 + 0x3b4) * fVar1;
  *(float *)((longlong)param_1 + 0x3bc) = ABS(fVar6 - fVar4);
  *(float *)(param_1 + 0x77) = *(float *)(param_1 + 0x77) * fVar1;
  *(float *)((longlong)param_1 + 0x3bc) = *(float *)((longlong)param_1 + 0x3bc) * fVar1;
  return param_1;
}




// ================================================================
// SCENE MANAGER UNCONDITIONAL SUB-CALL — FUN_140305c40
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
// PLANET GETTER — FUN_1402aca60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2aca60  Name: FUN_1402aca60  Size: 169 bytes
// ------------------------------------------------------------

undefined8 FUN_1402aca60(longlong param_1,uint param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  int iVar4;
  longlong *plVar5;
  uint uVar6;
  
  uVar6 = (param_2 ^ 0xdeadbeef) & 0x7fffffff;
  lVar1 = *(longlong *)(param_1 + 0x88);
  iVar4 = (uVar6 % 0x1f31d) * 0x41a7 + (uVar6 / 0x1f31d) * -0xb14;
  iVar2 = iVar4 + 0x7fffffff;
  if (-1 < iVar4) {
    iVar2 = iVar4;
  }
  plVar5 = (longlong *)
           (((longlong)iVar2 & *(ulonglong *)(param_1 + 0xb0)) * 0x10 +
           *(longlong *)(param_1 + 0x98));
  lVar3 = plVar5[1];
  if (lVar3 != lVar1) {
    iVar2 = *(int *)(lVar3 + 0x10);
    iVar4 = iVar2 - param_2;
    if ((int)param_2 < iVar2) {
      do {
        if (lVar3 == *plVar5) {
          return 0;
        }
        lVar3 = *(longlong *)(lVar3 + 8);
      } while ((int)param_2 < *(int *)(lVar3 + 0x10));
      iVar2 = *(int *)(lVar3 + 0x10);
      iVar4 = iVar2 - param_2;
    }
    if (SBORROW4(iVar2,param_2) == iVar4 < 0) {
      if (lVar3 == 0) {
        lVar3 = lVar1;
      }
      if (lVar3 != lVar1) {
        return *(undefined8 *)(lVar3 + 0x18);
      }
    }
  }
  return 0;
}




// ================================================================
// RTTI SYMBOL SEARCH — game mode classes
// ================================================================

// SYMBOL: GalacticModeClass @ RVA 0xfffffffec0000000 type=Class source=ANALYSIS
// NOT FOUND: GalacticConflictModeClass
// NOT FOUND: SpaceConflictModeClass
// NOT FOUND: LandConflictModeClass
// SYMBOL: GameModeClass @ RVA 0xfffffffec0000000 type=Class source=ANALYSIS
// SYMBOL: GameModeManagerClass @ RVA 0xfffffffec0000000 type=Class source=ANALYSIS
// SYMBOL: GameObjectManagerClass @ RVA 0xfffffffec0000000 type=Class source=ANALYSIS
// NOT FOUND: GalacticModeClass::vftable
// NOT FOUND: SpaceConflictModeClass::vftable
// NOT FOUND: LandConflictModeClass::vftable
// NOT FOUND: GameModeClass::Think
// NOT FOUND: GameModeClass::Update
// NOT FOUND: GameModeClass::Service
// NOT FOUND: GameModeClass::Next_State
// NOT FOUND: GameObjectManagerClass::Think
// NOT FOUND: GameObjectManagerClass::Service
// NOT FOUND: DynamicVectorClass


// Scanning for GameMode/GOM in all symbol names...
// (none found)
