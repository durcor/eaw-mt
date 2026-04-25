
/* =========================================================
 * FUN_1403639d0 — shared tail call in SpaceMode+LandMode slot 22
 * RVA 0x3639d0
 * size: 3916 bytes  params: 0  return: undefined
 * ========================================================= */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403639d0(longlong *param_1)

{
  uint uVar1;
  bool bVar2;
  undefined1 uVar3;
  longlong *plVar4;
  undefined4 uVar5;
  char cVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  longlong lVar11;
  longlong *plVar12;
  longlong lVar13;
  undefined4 *puVar14;
  longlong lVar15;
  longlong lVar16;
  longlong *plVar17;
  char *pcVar18;
  longlong lVar19;
  ulonglong uVar20;
  ulonglong uVar21;
  int iVar22;
  int iVar23;
  uint uVar24;
  int iVar25;
  undefined8 uVar26;
  int local_res20;
  undefined8 in_stack_fffffffffffffef8;
  undefined8 *puVar27;
  undefined4 uVar28;
  undefined8 *in_stack_ffffffffffffff00;
  undefined8 local_e0;
  undefined4 local_d8;
  undefined4 local_d0;
  int local_cc;
  undefined8 local_c8;
  undefined4 local_c0;
  undefined2 local_bc;
  longlong *local_b8;
  longlong *plStack_b0;
  longlong local_a8;
  undefined4 local_a0;
  int local_9c;
  longlong local_98;
  longlong *local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined4 local_70;
  undefined2 local_6c;
  longlong *local_68;
  longlong *plStack_60;
  longlong local_58;
  longlong local_50;
  
  uVar1 = *(uint *)(param_1 + 2);
  if (param_1[0x35] != DAT_140b15808) {
    param_1[0x35] = DAT_140b15808;
    FUN_140365810();
  }
  *(float *)(param_1 + 0x46) =
       DAT_140a28568 * *(float *)((longlong)param_1 + 0x234) * _DAT_140b15478;
  FUN_1404098d0(&DAT_140b2f020);
  if (*(char *)((longlong)param_1 + 0x9a) == '\x01') {
    if (DAT_140b155c1 == '\0') {
      FUN_14028e800(&DAT_140b153e0,0);
    }
    *(undefined1 *)((longlong)param_1 + 0x9a) = 0;
  }
  cVar6 = (**(code **)(*param_1 + 0xf0))(param_1);
  if (cVar6 == '\x01') {
    FUN_140364b20(param_1,uVar1);
  }
  FUN_140364920(param_1,uVar1);
  cVar6 = (**(code **)(*param_1 + 0x240))(param_1);
  iVar23 = 0;
  if (((cVar6 != '\0') && (param_1[0x33] != 0)) &&
     ((param_1[0x3e] == 0 || (*(char *)(param_1[0x3e] + 0x30) == '\0')))) {
    uVar24 = uVar1 & 0x8000000f;
    if ((int)uVar24 < 0) {
      uVar24 = (uVar24 - 1 | 0xfffffff0) + 1;
    }
    iVar22 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
    iVar25 = (int)param_1[0x32];
    if (iVar22 <= (int)param_1[0x32]) {
      iVar25 = iVar22;
    }
    iVar22 = iVar23;
    if (0 < iVar25) {
      do {
        lVar11 = FUN_140294bc0(&DAT_140a16fd0,iVar22);
        if (lVar11 != 0) {
          uVar7 = *(uint *)(lVar11 + 0x4c) & 0x8000000f;
          if ((int)uVar7 < 0) {
            uVar7 = (uVar7 - 1 | 0xfffffff0) + 1;
          }
          if ((uVar7 == uVar24) &&
             (lVar11 = *(longlong *)(param_1[0x33] + (longlong)(int)*(uint *)(lVar11 + 0x4c) * 8),
             lVar11 != 0)) {
            FUN_1404c1670(lVar11,uVar1);
          }
        }
        iVar22 = iVar22 + 1;
      } while (iVar22 < iVar25);
    }
    if (param_1[0x54] != 0) {
      FUN_1404ec2e0(param_1[0x54],uVar1);
    }
  }
  lVar11 = (**(code **)(*param_1 + 0x238))(param_1);
  if (lVar11 != 0) {
    FUN_140539b20(lVar11);
  }
  plVar12 = (longlong *)(**(code **)(*param_1 + 0x100))(param_1);
  lVar13 = (**(code **)(*plVar12 + 0x78))(plVar12);
  lVar16 = *(longlong *)(lVar13 + 0x1c8) - *(longlong *)(lVar13 + 0x1c0);
  lVar11 = lVar16 >> 0x3f;
  if (lVar16 / 0x24 + lVar11 != lVar11) {
    *(longlong *)(lVar13 + 0x1c8) = *(longlong *)(lVar13 + 0x1c0);
  }
  if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
    do {
      lVar11 = FUN_140294bc0(&DAT_140a16fd0,iVar23);
      if (lVar11 != 0) {
        *(undefined4 *)(lVar11 + 0x78) = *(undefined4 *)(lVar11 + 0x7c);
        *(undefined8 *)(lVar11 + 0x7c) = 0;
      }
      iVar23 = iVar23 + 1;
    } while (iVar23 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  if (param_1[3] != 0) {
    FUN_1402be640(param_1[3],uVar1);
  }
  if (param_1[4] != 0) {
    FUN_1402be640(param_1[4],uVar1);
  }
  FUN_140490580((longlong)param_1 + 0x9c);
  lVar11 = (**(code **)(*param_1 + 0x230))(param_1);
  if (lVar11 != 0) {
    FUN_14050a780(lVar11);
  }
  lVar11 = (**(code **)(*param_1 + 0x228))(param_1);
  if (lVar11 != 0) {
    FUN_140426eb0(lVar11);
  }
  if (param_1[0x3e] != 0) {
    FUN_1403bfba0(param_1[0x3e],uVar1);
  }
  plVar12 = (longlong *)param_1[6];
  if ((plVar12 != (longlong *)0x0) && (DAT_140b15490 == '\0')) {
    (**(code **)(*plVar12 + 0x28))(plVar12,uVar1);
  }
  if ((param_1[5] != 0) && (DAT_140b15490 == '\0')) {
    FUN_140344670(param_1[5],uVar1);
  }
  if (param_1[7] != 0) {
    FUN_140495dd0(param_1[7],uVar1);
  }
  if (param_1[8] != 0) {
    FUN_14052a110(param_1[8],uVar1);
  }
  if ((param_1[0x53] != 0) && (DAT_140b15490 == '\0')) {
    FUN_140537ab0(param_1[0x53],uVar1);
  }
  cVar6 = FUN_14028afb0(&DAT_140b153e0);
  if (cVar6 == '\x01') {
    FUN_1403615d0(param_1);
  }
  if ((char)param_1[0x56] != '\0') {
    *(int *)((longlong)param_1 + 0x2b4) = *(int *)((longlong)param_1 + 0x2b4) + -1;
    FUN_1403611f0(param_1);
    if (((*(int *)((longlong)param_1 + 0x2b4) < 1) && (param_1[5] != 0)) &&
       (cVar6 = FUN_140342dc0(), cVar6 == '\0')) {
      FUN_140346460(param_1[5],0x15,(int)param_1[0x57]);
      *(undefined1 *)(param_1 + 0x56) = 0;
      *(undefined8 *)((longlong)param_1 + 0x2b4) = 0xffffffffffffffff;
    }
  }
  uVar28 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
  if (*(int *)((longlong)param_1 + 0x2d4) != 0) {
    lVar11 = FUN_140320c80(&DAT_140b2b300);
    uVar28 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
    if ((lVar11 == param_1[0x5b]) && (param_1[0x62] == 0)) {
      lVar11 = FUN_140320cb0(&DAT_140b2b300);
      uVar28 = (undefined4)((ulonglong)in_stack_fffffffffffffef8 >> 0x20);
      if (lVar11 != 0) {
        plVar12 = param_1 + 0x60;
        lVar13 = -1;
        do {
          lVar13 = lVar13 + 1;
        } while (*(char *)(lVar11 + lVar13) != '\0');
        FUN_140022730(plVar12,lVar11);
        FUN_1401f9320(&local_e0,&DAT_140b16a80);
        plVar17 = plVar12;
        if (0xf < (ulonglong)param_1[99]) {
          plVar17 = (longlong *)*plVar12;
        }
        in_stack_ffffffffffffff00 = &local_e0;
        uVar26 = CONCAT71((int7)((ulonglong)in_stack_fffffffffffffef8 >> 8),1);
        FUN_1402dff30(&DAT_140b27f60,plVar17,DAT_14080080c,0,uVar26,in_stack_ffffffffffffff00,1);
        uVar28 = (undefined4)((ulonglong)uVar26 >> 0x20);
        if (0xf < (ulonglong)param_1[99]) {
          plVar12 = (longlong *)*plVar12;
        }
        FUN_140025760("External Hologram - Displaying subtitle text: %s\n",plVar12);
        FUN_1401f9440(&local_e0);
      }
    }
    cVar6 = FUN_140320d50(&DAT_140b2b300,*(undefined4 *)((longlong)param_1 + 0x2d4));
    if (cVar6 == '\0') {
      pcVar18 = (char *)param_1[0x5b];
      if (pcVar18 == (char *)0x0) {
        pcVar18 = "(CRC)";
      }
      else if (0xf < *(ulonglong *)((longlong)pcVar18 + 0x18)) {
        pcVar18 = *(char **)pcVar18;
      }
      FUN_140025760("External Hologram - Stopping bink movie because speech event %s is no longer playing.\n"
                    ,pcVar18);
      FUN_14030ac40(&DAT_140b27f60);
      plVar12 = param_1 + 0x5c;
      *(undefined4 *)((longlong)param_1 + 0x2d4) = 0;
      param_1[0x5b] = 0;
      param_1[0x5e] = 0;
      if (0xf < (ulonglong)param_1[0x5f]) {
        plVar12 = (longlong *)*plVar12;
      }
      *(undefined1 *)plVar12 = 0;
      if (param_1[0x62] != 0) {
        plVar12 = param_1 + 0x60;
        plVar17 = plVar12;
        if (0xf < (ulonglong)param_1[99]) {
          plVar17 = (longlong *)*plVar12;
        }
        FUN_140025760("External Hologram - Removing subtitle text: %s\n",plVar17);
        plVar17 = plVar12;
        if (0xf < (ulonglong)param_1[99]) {
          plVar17 = (longlong *)*plVar12;
        }
        FUN_1402ff290(&DAT_140b27f60,plVar17);
        param_1[0x62] = 0;
        if (0xf < (ulonglong)param_1[99]) {
          plVar12 = (longlong *)*plVar12;
        }
        *(undefined1 *)plVar12 = 0;
      }
    }
  }
  if (((DAT_140a785a8 == 0) && (param_1[0x5b] != 0)) && (param_1[0x5e] != 0)) {
    bVar2 = false;
    if (*(int *)((longlong)param_1 + 0x2d4) != 0) {
      bVar2 = false;
      if (*(int *)((longlong)param_1 + 0x2d4) != *(int *)(param_1[0x5b] + 0x20)) {
        bVar2 = true;
      }
    }
    lVar11 = FUN_140320c80(&DAT_140b2b300);
    if (((lVar11 != 0) && (lVar11 != param_1[0x5b])) || (bVar2)) {
      plVar12 = (longlong *)param_1[0x5b];
      if (0xf < (ulonglong)plVar12[3]) {
        plVar12 = (longlong *)*plVar12;
      }
      FUN_140025760("External Hologram - Aborting pending hologram speech %s because another speech event is active.\n"
                    ,plVar12);
      plVar12 = param_1 + 0x5c;
      param_1[0x5b] = 0;
      param_1[0x5e] = 0;
      if (0xf < (ulonglong)param_1[0x5f]) {
        plVar12 = (longlong *)*plVar12;
      }
      *(undefined1 *)plVar12 = 0;
      plVar12 = param_1 + 0x60;
      param_1[0x62] = 0;
      if (0xf < (ulonglong)param_1[99]) {
        plVar12 = (longlong *)*plVar12;
      }
      *(undefined1 *)plVar12 = 0;
    }
    else if ((*(int *)((longlong)param_1 + 0x2d4) == 0) || (lVar11 == 0)) {
      FUN_140321170(&DAT_140b2b300,param_1[0x5b]);
      plVar12 = (longlong *)param_1[0x5b];
      if (0xf < (ulonglong)plVar12[3]) {
        plVar12 = (longlong *)*plVar12;
      }
      FUN_140025760("External Hologram - Queueing speech event %s.\n",plVar12);
      plVar12 = param_1 + 0x5c;
      plVar17 = plVar12;
      if (0xf < (ulonglong)param_1[0x5f]) {
        plVar17 = (longlong *)*plVar12;
      }
      FUN_1402fc7a0(&DAT_140b27f60,plVar17,1);
      if (0xf < (ulonglong)param_1[0x5f]) {
        plVar12 = (longlong *)*plVar12;
      }
      FUN_140025760("External Hologram - Playing bink movie %s.\n",plVar12);
      *(undefined4 *)((longlong)param_1 + 0x2d4) = *(undefined4 *)(param_1[0x5b] + 0x20);
    }
  }
  if (param_1[5] == 0) {
    return;
  }
  cVar6 = FUN_140342dc0();
  if (cVar6 == '\0') {
    return;
  }
  local_80 = _DAT_140804fc0;
  uStack_78 = _UNK_140804fc8;
  local_68 = (longlong *)0x0;
  plStack_60 = (longlong *)0x0;
  local_70 = 0;
  local_6c = 0;
  local_58 = 0;
  FUN_140342d80(param_1[5],&local_80);
  cVar6 = (**(code **)(*param_1 + 0xf0))(param_1);
  if (cVar6 == '\0') goto LAB_140364833;
  if (((DAT_140b2c1d8 != (code *)0x0) &&
      (lVar11 = FUN_14028aa00(&DAT_140b153e0,param_1), lVar11 == 0)) && (DAT_140b155c4 == '\0')) {
    (*DAT_140b31418)();
    (*DAT_140b2c1d8)(param_1,0);
  }
  iVar23 = -1;
  iVar25 = local_80._4_4_;
  lVar11 = *(longlong *)(DAT_140b15418 + 0x30);
  if ((lVar11 != 0) && (cVar6 = FUN_14033fe20(lVar11), cVar6 == '\x01')) {
    iVar23 = *(int *)(lVar11 + 0x1c);
  }
  lVar11 = FUN_14028aa00(&DAT_140b153e0,param_1);
  if (lVar11 == 0) goto LAB_140364833;
  lVar11 = FUN_14028aa00(&DAT_140b153e0,param_1);
  local_50 = lVar11;
  local_9c = (**(code **)(*param_1 + 0xe0))(param_1);
  iVar22 = (**(code **)(*param_1 + 0xe0))(param_1);
  if (iVar22 == 2) {
    puVar14 = (undefined4 *)FUN_1402ad0e0();
  }
  else {
    puVar14 = (undefined4 *)FUN_1402ac5a0(*(undefined8 *)(lVar11 + 0x18));
  }
  local_a0 = *puVar14;
  iVar22 = puVar14[2];
  local_88 = FUN_140294bc0(&DAT_140a16fd0,iVar25);
  iVar8 = iVar22;
  if (iVar25 == iVar22) {
    iVar8 = puVar14[1];
  }
  lVar11 = FUN_140294bc0(&DAT_140a16fd0,iVar8);
  local_98 = *(longlong *)(lVar11 + 0x68);
  if ((local_98 == 0) || (*(char *)(local_98 + 0x20a8) == '\0')) {
    FUN_1402a6710(param_1[3],iVar8);
    FUN_1402a6840(param_1[3],iVar8);
  }
  iVar9 = iVar22;
  if ((int)local_80 != 0xe) {
    iVar9 = iVar23;
  }
  iVar23 = iVar8;
  if ((int)local_80 != 0x10) {
    iVar23 = iVar9;
  }
  local_res20 = iVar23;
  if ((int)local_80 == 0xf) {
    lVar13 = FUN_140294bc0(&DAT_140a16fd0,iVar22);
    local_res20 = iVar22;
    if ((*(char *)(*(longlong *)(lVar13 + 0x68) + 0x20a8) == '\0') &&
       (*(char *)(*(longlong *)(lVar13 + 0x68) + 0x108) == '\0')) {
      FUN_1402a6710(param_1[3],iVar22);
      local_res20 = iVar23;
    }
  }
  FUN_140294a40(&DAT_140a16fd0);
  if ((puVar14[10] != 0x3fffff) &&
     (lVar13 = FUN_1402aca60(*(undefined8 *)(DAT_140b15418 + 0x18)), lVar13 != 0)) {
    bVar2 = false;
    plVar12 = (longlong *)FUN_14028aa00(&DAT_140b153e0,param_1);
    local_90 = plVar12;
    if ((*(int *)(lVar13 + 0x58) != iVar25) && ((*(byte *)(lVar13 + 0x3a0) & 0x42) == 0)) {
      puVar27 = &local_e0;
      local_e0 = 0;
      local_d8 = 0;
      in_stack_ffffffffffffff00 =
           (undefined8 *)((ulonglong)in_stack_ffffffffffffff00 & 0xffffffff00000000);
      lVar16 = FUN_1402a9ff0(*(undefined8 *)(DAT_140b15418 + 0x18),0xffffffff,iVar25,0xffffffff,
                             puVar27,in_stack_ffffffffffffff00,0x7fffffff,0xffffffff);
      uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
      if (lVar16 != 0) {
        if (0 < *(int *)(lVar16 + 0x10)) {
          lVar19 = 0;
          plVar17 = *(longlong **)(lVar16 + 8);
          do {
            if ((*plVar17 != 0) && (*(char *)(*(longlong *)(*plVar17 + 0x298) + 0x1f) != '\0')) {
              FUN_140391310(lVar13,CONCAT71((int7)((ulonglong)lVar19 >> 8),1));
              uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
              if ((plVar12 != (longlong *)0x0) && (plVar12[5] != 0)) {
                cVar6 = FUN_1403464b0(plVar12[5],3,iVar25);
                uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
                if (cVar6 == '\x01') {
                  local_d0 = 3;
                  local_b8 = (longlong *)0x0;
                  plStack_b0 = (longlong *)0x0;
                  local_cc = iVar25;
                  local_c8 = 5;
                  local_c0 = 0;
                  local_bc = 0;
                  local_a8 = 0;
                  uVar26 = FUN_140343b90(plVar12[5],&local_d0);
                  plVar4 = plStack_b0;
                  uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
                  plVar17 = local_b8;
                  if (local_b8 != (longlong *)0x0) goto joined_r0x00014036442c;
                }
                else {
                  iVar23 = (**(code **)(*plVar12 + 0xe0))(plVar12);
                  if (iVar23 == 0) {
                    *(undefined4 *)((longlong)plVar12 + 0x374) = *puVar14;
                  }
                }
              }
              bVar2 = true;
              goto LAB_140364358;
            }
            lVar19 = lVar19 + 1;
            plVar17 = plVar17 + 1;
          } while (lVar19 < *(int *)(lVar16 + 0x10));
        }
        local_res20 = iVar25;
      }
    }
    goto LAB_140364358;
  }
  goto LAB_1403646bb;
joined_r0x00014036442c:
  if (plVar17 == plVar4) goto LAB_140364486;
  uVar21 = plVar17[3];
  if (0xf < uVar21) {
    lVar16 = *plVar17;
    uVar20 = uVar21 + 1;
    lVar19 = lVar16;
    if (0xfff < uVar20) {
      lVar19 = *(longlong *)(lVar16 + -8);
      uVar20 = uVar21 + 0x28;
      if (0x1f < (lVar16 - lVar19) - 8U) goto LAB_140364913;
    }
    uVar26 = thunk_FUN_1407864b8(lVar19,uVar20);
  }
  uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
  plVar17[2] = 0;
  plVar17[3] = 0xf;
  *(undefined1 *)plVar17 = 0;
  plVar17 = plVar17 + 4;
  goto joined_r0x00014036442c;
LAB_140364486:
  uVar20 = local_a8 - (longlong)local_b8 & 0xffffffffffffffe0;
  plVar12 = local_b8;
  if (0xfff < uVar20) {
    plVar12 = (longlong *)local_b8[-1];
    uVar20 = uVar20 + 0x27;
    if ((undefined1 *)0x1f < (undefined1 *)((longlong)local_b8 + (-8 - (longlong)plVar12))) {
LAB_140364913:
                    /* WARNING: Subroutine does not return */
      FUN_1407711cc(uVar26,uVar20);
    }
  }
  thunk_FUN_1407864b8(plVar12);
  bVar2 = true;
  plVar12 = local_90;
LAB_140364358:
  cVar6 = FUN_1403464b0(plVar12[5],6,iVar25);
  if ((cVar6 == '\x01') && (*(int *)(lVar13 + 0x58) == iVar25)) {
    iVar23 = -1;
    iVar22 = iVar25;
    if ((local_98 != 0) && (iVar23 = -1, *(char *)(local_98 + 0x20a8) != '\0')) {
      iVar23 = iVar8;
    }
  }
  else {
    cVar6 = FUN_1403464b0(plVar12[5],6,iVar8);
    if ((cVar6 != '\x01') || (*(int *)(lVar13 + 0x58) != iVar8)) goto LAB_1403646bb;
    iVar23 = iVar25;
    iVar22 = -1;
    if ((local_98 != 0) && (iVar22 = -1, *(char *)(local_98 + 0x20a8) != '\0')) {
      iVar22 = iVar8;
    }
  }
  if (((iVar22 != -1) && (iVar23 != -1)) && (!bVar2)) {
    puVar27 = &local_e0;
    local_e0 = 0;
    local_d8 = 0;
    lVar13 = FUN_1402a9ff0(plVar12[3],3,0xffffffff,0xffffffff,puVar27,
                           (ulonglong)in_stack_ffffffffffffff00 & 0xffffffff00000000,0x7fffffff,
                           0xffffffff);
    iVar9 = *(int *)(lVar13 + 0x10);
    if (0 < (longlong)iVar9) {
      lVar16 = 0;
      do {
        lVar19 = *(longlong *)(*(longlong *)(lVar13 + 8) + lVar16 * 8);
        lVar15 = FUN_1403956c0(lVar19);
        uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
        if ((*(int *)(lVar15 + 4) == iVar23) &&
           (*(char *)(*(longlong *)(lVar19 + 0xb8) + 0x2c8) == '\0')) goto LAB_1403646bb;
        lVar16 = lVar16 + 1;
        plVar12 = local_90;
      } while (lVar16 < iVar9);
    }
    local_d0 = 6;
    local_c0 = 0;
    local_bc = 0;
    local_b8 = (longlong *)0x0;
    plStack_b0 = (longlong *)0x0;
    local_a8 = 0;
    local_c8 = 0x1e;
    local_cc = iVar22;
    uVar26 = FUN_140343b90(plVar12[5],&local_d0);
    plVar17 = plStack_b0;
    uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
    plVar12 = local_b8;
    if (local_b8 != (longlong *)0x0) {
      for (; plVar12 != plVar17; plVar12 = plVar12 + 4) {
        uVar20 = plVar12[3];
        if (0xf < uVar20) {
          lVar13 = *plVar12;
          uVar21 = uVar20 + 1;
          lVar16 = lVar13;
          if (0xfff < uVar21) {
            lVar16 = *(longlong *)(lVar13 + -8);
            uVar21 = uVar20 + 0x28;
            if (0x1f < (lVar13 - lVar16) - 8U) goto LAB_140364919;
          }
          uVar26 = thunk_FUN_1407864b8(lVar16,uVar21);
        }
        uVar28 = (undefined4)((ulonglong)puVar27 >> 0x20);
        plVar12[2] = 0;
        plVar12[3] = 0xf;
        *(undefined1 *)plVar12 = 0;
      }
      uVar21 = local_a8 - (longlong)local_b8 & 0xffffffffffffffe0;
      plVar12 = local_b8;
      if (0xfff < uVar21) {
        plVar12 = (longlong *)local_b8[-1];
        uVar21 = uVar21 + 0x27;
        if ((undefined1 *)0x1f < (undefined1 *)((longlong)local_b8 + (-8 - (longlong)plVar12))) {
LAB_140364919:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(uVar26,uVar21);
        }
      }
      thunk_FUN_1407864b8(plVar12);
    }
  }
LAB_1403646bb:
  lVar13 = local_50;
  uVar5 = local_a0;
  *(int *)(local_50 + 700) = iVar25;
  *(int *)(local_50 + 0x2c0) = iVar8;
  *(undefined4 *)(local_50 + 0x2c4) = local_a0;
  uVar10 = (**(code **)(*param_1 + 0xe0))(param_1);
  *(undefined4 *)(lVar13 + 0x2c8) = uVar10;
  uVar3 = DAT_140b30545;
  FUN_14028f520(&DAT_140b153e0,iVar25);
  FUN_14028f320(&DAT_140b153e0,1);
  iVar23 = local_9c;
  FUN_14028bf10(&DAT_140b153e0,local_9c,iVar25,local_res20,CONCAT44(uVar28,uVar5),uVar3);
  FUN_14028c4c0(&DAT_140b153e0);
  if ((DAT_140b2c1d8 != (code *)0x0) && (DAT_140b155c4 == '\0')) {
    (*DAT_140b31418)();
    (*DAT_140b2c1d8)(lVar13,0);
  }
  uVar26 = FUN_1402aca60(*(undefined8 *)(DAT_140b15418 + 0x18),uVar5);
  if (iVar23 == 1) {
    FUN_14032f4d0(DAT_140b15418 + 0xa0,uVar26,local_88);
    FUN_14032a340(DAT_140b15418 + 0xa0,uVar26,lVar11);
  }
  else if (iVar23 == 2) {
    FUN_14032f570(DAT_140b15418 + 0xa0,uVar26,local_88);
    FUN_14032a3b0(DAT_140b15418 + 0xa0,uVar26,lVar11);
  }
  if (local_res20 != -1) {
    uVar26 = FUN_140294bc0(&DAT_140a16fd0,local_res20);
    FUN_14032b330(DAT_140b15418 + 0xa0,uVar26);
  }
  FUN_1402b59b0(*(undefined8 *)(DAT_140b15418 + 0x18));
LAB_140364833:
  uVar26 = FUN_14028f590(&DAT_140b153e0);
  plVar17 = plStack_60;
  plVar12 = local_68;
  if (local_68 != (longlong *)0x0) {
    for (; plVar12 != plVar17; plVar12 = plVar12 + 4) {
      uVar20 = plVar12[3];
      if (0xf < uVar20) {
        lVar11 = *plVar12;
        uVar21 = uVar20 + 1;
        lVar13 = lVar11;
        if (0xfff < uVar21) {
          lVar13 = *(longlong *)(lVar11 + -8);
          uVar21 = uVar20 + 0x28;
          if (0x1f < (lVar11 - lVar13) - 8U) goto LAB_14036490d;
        }
        uVar26 = thunk_FUN_1407864b8(lVar13,uVar21);
      }
      plVar12[2] = 0;
      plVar12[3] = 0xf;
      *(undefined1 *)plVar12 = 0;
    }
    uVar21 = local_58 - (longlong)local_68 & 0xffffffffffffffe0;
    plVar12 = local_68;
    if (0xfff < uVar21) {
      plVar12 = (longlong *)local_68[-1];
      uVar21 = uVar21 + 0x27;
      if ((undefined1 *)0x1f < (undefined1 *)((longlong)local_68 + (-8 - (longlong)plVar12))) {
LAB_14036490d:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar26,uVar21);
      }
    }
    thunk_FUN_1407864b8(plVar12);
  }
  return;
}



/* =========================================================
 * FUN_140527dd0 — per-player entity update in slot-22 list walk
 * RVA 0x527dd0
 * size: 293 bytes  params: 0  return: undefined
 * ========================================================= */


void FUN_140527dd0(longlong param_1)

{
  longlong *plVar1;
  uint uVar2;
  longlong *plVar3;
  
  FUN_1405369e0();
  FUN_140497bd0(param_1 + 0x160);
  FUN_140285e40(param_1 + 0x180);
  FUN_140285e40(param_1 + 0x198);
  FUN_140285e40(param_1 + 0x1b0);
  FUN_140285e40(param_1 + 0x1c8);
  FUN_140285e40(param_1 + 0x1e0);
  FUN_140497bd0(param_1 + 0x1f8);
  FUN_140285e40(param_1 + 0x218);
  uVar2 = FUN_1404c2030();
  plVar3 = (longlong *)**(longlong **)(param_1 + 0x230);
  if (plVar3 != *(longlong **)(param_1 + 0x230)) {
    do {
      plVar1 = (longlong *)*plVar3;
      if (*(uint *)((longlong)plVar3 + 0x14) < uVar2) {
        *(longlong **)plVar3[1] = plVar1;
        *(longlong *)(*plVar3 + 8) = plVar3[1];
        *(longlong *)(param_1 + 0x238) = *(longlong *)(param_1 + 0x238) + -1;
        FUN_1404ad530(plVar3 + 4);
        thunk_FUN_1407864b8(plVar3,0x40);
      }
      plVar3 = plVar1;
    } while (plVar1 != (longlong *)*(longlong *)(param_1 + 0x230));
  }
  FUN_140285e40(param_1 + 0x250);
  FUN_140497bd0(param_1 + 0x268);
  FUN_140497bd0(param_1 + 0x288);
  FUN_140497bd0(param_1 + 0x2a8);
  FUN_140285e40(param_1 + 0x2c8);
  return;
}


