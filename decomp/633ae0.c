/* TargetingBehaviorClass::FUN_140633ae0  RVA 0x633ae0  size 3364  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
TargetingBehaviorClass::FUN_140633ae0(longlong *param_1,longlong *param_2,undefined4 param_3)

{
  longlong lVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;
  byte bVar5;
  char cVar6;
  int iVar7;
  longlong lVar8;
  ulonglong uVar9;
  longlong lVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong *plVar13;
  ulonglong *puVar14;
  undefined8 *puVar15;
  ulonglong uVar16;
  undefined4 uVar17;
  ulonglong uVar18;
  bool bVar19;
  float fVar20;
  float local_res10 [2];
  undefined4 local_res18;
  float local_res20 [2];
  float *pfVar21;
  longlong local_118;
  float local_110;
  undefined8 local_108;
  undefined4 local_100;
  ulonglong local_f8;
  undefined4 local_f0;
  float local_e8;
  undefined4 local_e4;
  float local_e0;
  undefined4 local_dc;
  undefined1 local_d8 [8];
  undefined4 local_d0;
  longlong local_c8 [2];
  undefined4 local_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  longlong local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined4 local_80;
  undefined8 local_7c;
  undefined8 local_74;
  undefined4 local_6c;
  undefined4 local_68;
  
  lVar1 = param_2[0x1e];
  if (lVar1 == 0) {
    return 0x80650002;
  }
  local_res18 = param_3;
  FUN_140565de0();
  lVar8 = param_2[0x56];
  local_118 = param_2[0x15];
  if ((lVar8 != 0) && (*(char *)(lVar8 + 0x348) != -1)) {
    local_118 = *(longlong *)(lVar8 + 0xa8);
  }
  lVar8 = local_118;
  cVar3 = (**(code **)(*param_1 + 0x108))(param_1);
  uVar18 = 0;
  if (((cVar3 == '\0') && (lVar8 != 0)) && (lVar8 = *(longlong *)(lVar8 + 0x1f0), lVar8 != 0)) {
    local_res10[0] = -NAN;
    lVar8 = FUN_14041a2e0(lVar8,local_res10,0);
    param_3 = local_res18;
    if ((lVar8 != 0) && (cVar3 = FUN_1405680b0(lVar8), param_3 = local_res18, cVar3 != '\0')) {
      uVar9 = uVar18;
      if (-1 < (int)local_res10[0]) {
        uVar9 = FUN_140396cb0(lVar8);
      }
      cVar3 = (**(code **)(*param_1 + 0x160))(param_1,lVar8,uVar9);
      param_3 = local_res18;
      if ((cVar3 != '\0') &&
         (cVar3 = (**(code **)(*param_1 + 0x168))(param_1,lVar8,uVar9), param_3 = local_res18,
         cVar3 == '\0')) {
        (**(code **)(*param_1 + 0xd0))(param_1,lVar8,uVar9,1,1);
        uVar16 = uVar18;
        while( true ) {
          iVar7 = 0;
          if (param_2[0x5a] != 0) {
            iVar7 = *(int *)(param_2[0x5a] + 0x10);
          }
          param_3 = local_res18;
          if (iVar7 <= (int)uVar16) break;
          lVar10 = FUN_140396cb0(param_2);
          if ((lVar10 != 0) && (*(int *)(*(longlong *)(lVar10 + 0x20) + 0x48) - 5U < 6)) {
            FUN_140387b70(lVar10,lVar8,uVar9);
          }
          uVar16 = (ulonglong)((int)uVar16 + 1);
        }
      }
    }
  }
  lVar8 = *(longlong *)(lVar1 + 8);
  if (lVar8 == 0) {
    iVar7 = (**(code **)(*param_1 + 0xf0))(param_1);
    if (iVar7 != 0) {
      (**(code **)(*param_1 + 0xf8))(param_1,0);
    }
    plVar13 = (longlong *)param_2[0x56];
    if (plVar13 == (longlong *)0x0) {
      lVar8 = *param_2;
      plVar13 = param_2;
    }
    else {
      lVar8 = *plVar13;
    }
    (**(code **)(lVar8 + 0x10))(plVar13,0x12);
    if ((*(byte *)((longlong)param_2 + 0x3a1) & 0x10) != 0) {
      return 0x650000;
    }
    pfVar21 = local_res20;
    local_res20[0] = DAT_140899788;
    local_f8 = 0;
    local_res10[0] = (float)((uint)local_res10[0] & 0xffffff00);
    lVar8 = FUN_140568a00(param_1,param_3,&local_f8,local_res10,pfVar21);
    if (lVar8 == 0) {
      return 0x650000;
    }
    plVar13 = (longlong *)param_2[0x56];
    if ((plVar13 == (longlong *)0x0) || ((char)plVar13[0x69] == -1)) {
      (**(code **)(*param_1 + 0xd0))
                (param_1,lVar8,local_f8,local_res10[0]._0_1_,(ulonglong)pfVar21 & 0xffffffffffffff00
                );
      lVar10 = local_118;
    }
    else {
      uVar11 = (**(code **)(*plVar13 + 0x10))(plVar13,0x16);
      FUN_1404061d0(uVar11,local_res10[0]._0_1_);
      if (local_f8 == 0) {
        uVar17 = 0xffffffff;
      }
      else {
        uVar17 = *(undefined4 *)(local_f8 + 0x18);
      }
      FUN_140406370(uVar11,lVar8,uVar17);
      lVar10 = local_118;
    }
  }
  else {
    if (((((*(char *)(lVar8 + 0x3a7) != '\0') || (*(char *)(lVar8 + 0x3b4) != '\0')) ||
         ((*(byte *)(lVar8 + 0x3a0) & 0x42) != 0)) ||
        ((cVar3 = FUN_14039b140(), cVar3 != '\0' && (*(char *)(param_2[0x53] + 0xa4) == '\0')))) ||
       ((cVar3 = FUN_14039b950(param_2,5,1), cVar3 == '\0' &&
        (cVar3 = FUN_14035f470(DAT_140b15418,(int)param_2[0xb],*(undefined8 *)(lVar1 + 8),1),
        cVar3 == '\x01')))) {
      (**(code **)(*param_1 + 0xd8))
                (param_1,*(longlong *)(lVar1 + 8) == *(longlong *)(lVar1 + 0x148));
      return 0x650000;
    }
    cVar3 = (**(code **)(*param_1 + 0x108))(param_1);
    if (cVar3 == '\0') {
      lVar8 = *(longlong *)(lVar1 + 8);
      local_res20[0] = DAT_140899784;
      local_f8 = local_f8 & 0xffffffff00000000;
      cVar3 = FUN_140567c50(param_1,lVar8,local_res20,&local_f8);
      cVar6 = FUN_14039aa60(param_1[5]);
      if ((cVar6 == '\0') ||
         (cVar6 = FUN_14035f3b0(DAT_140b15418,*(undefined4 *)(lVar8 + 0x58),param_1[5] + 0x78,1),
         cVar6 == '\0')) {
        cVar6 = FUN_140569000(param_1,lVar8);
        if ((cVar3 != '\0') && ((cVar6 != '\0' || (local_res20[0] == DAT_140899780))))
        goto LAB_140633f7b;
      }
      else {
        cVar3 = '\0';
        FUN_140569000(param_1,lVar8);
      }
      pfVar21 = &local_e8;
      local_e8 = DAT_140899788;
      local_108 = 0;
      local_res10[0] = (float)((uint)local_res10[0] & 0xffffff00);
      lVar10 = FUN_140568a00(param_1,param_3,&local_108,local_res10,pfVar21);
      if ((lVar10 != 0) && (lVar10 != lVar8)) {
        if (cVar3 != '\0') {
          pfVar21 = (float *)CONCAT44((int)((ulonglong)pfVar21 >> 0x20),local_res20[0]);
          cVar3 = FUN_140567110(param_1,lVar10,local_e8,lVar8,pfVar21);
          if (cVar3 == '\0') goto LAB_140633f7b;
        }
        (**(code **)(*param_1 + 0xd0))
                  (param_1,lVar10,local_108,0,(ulonglong)pfVar21 & 0xffffffffffffff00);
      }
    }
LAB_140633f7b:
    lVar10 = local_118;
    lVar8 = *(longlong *)(lVar1 + 0x10);
    if (((lVar8 == 0) ||
        (((*(longlong *)(lVar8 + 0x20) != 0 &&
          (*(char *)(*(longlong *)(lVar8 + 0x20) + 0x4d) == '\x01')) &&
         (*(float *)(lVar8 + 0x28) <= 0.0)))) &&
       ((local_118 != 0 && (*(longlong *)(local_118 + 0x1f0) != 0)))) {
      local_res10[0] = -NAN;
      lVar8 = FUN_14041a2e0(*(longlong *)(local_118 + 0x1f0),local_res10,0xffffffff);
      if (lVar8 == 0) {
        lVar8 = *(longlong *)(lVar1 + 8);
      }
      if ((((local_res10[0] != -NAN) && (lVar12 = FUN_140396cb0(lVar8), lVar12 != 0)) &&
          ((*(longlong *)(lVar12 + 0x20) == 0 ||
           ((*(char *)(*(longlong *)(lVar12 + 0x20) + 0x4d) != '\x01' ||
            (0.0 < *(float *)(lVar12 + 0x28))))))) ||
         (lVar12 = FUN_140394610(lVar8,param_2), lVar12 != 0)) {
        lVar10 = *param_1;
        uVar4 = (**(code **)(lVar10 + 0x108))(param_1);
        (**(code **)(lVar10 + 0xd0))(param_1,lVar8,lVar12,*(undefined1 *)(lVar1 + 0xf5),uVar4);
        uVar9 = uVar18;
        while( true ) {
          iVar7 = 0;
          if (param_2[0x5a] != 0) {
            iVar7 = *(int *)(param_2[0x5a] + 0x10);
          }
          lVar10 = local_118;
          if (iVar7 <= (int)uVar9) break;
          lVar10 = FUN_140396cb0(param_2,uVar9);
          if (*(int *)(*(longlong *)(lVar10 + 0x20) + 0x48) - 5U < 6) {
            FUN_140387b70(lVar10,lVar8,lVar12);
          }
          uVar9 = (ulonglong)((int)uVar9 + 1);
        }
      }
    }
  }
  cVar3 = (**(code **)(*param_1 + 0x160))
                    (param_1,*(undefined8 *)(lVar1 + 8),*(undefined8 *)(lVar1 + 0x10));
  if ((cVar3 == '\0') ||
     (cVar3 = (**(code **)(*param_1 + 0x168))
                        (param_1,*(undefined8 *)(lVar1 + 8),*(undefined8 *)(lVar1 + 0x10)),
     cVar3 != '\0')) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  bVar19 = false;
  if ((((lVar10 != 0) && (*(longlong *)(lVar1 + 8) != 0)) &&
      ((*(byte *)(*(longlong *)(lVar1 + 8) + 0x3a0) & 2) == 0)) &&
     (((cVar3 = (**(code **)(*param_1 + 0x108))(param_1), cVar3 != '\0' && (!bVar2)) &&
      (*(longlong *)(lVar10 + 0x1f0) != 0)))) {
    if (*(longlong *)(lVar1 + 0x10) == 0) {
      fVar20 = -NAN;
    }
    else {
      fVar20 = *(float *)(*(longlong *)(lVar1 + 0x10) + 0x18);
    }
    local_res10[0] = -NAN;
    lVar8 = FUN_14041a2e0(*(longlong *)(lVar10 + 0x1f0),local_res10,0);
    lVar10 = local_118;
    if (((lVar8 == 0) ||
        ((*(longlong *)(lVar1 + 8) != lVar8 &&
         (*(longlong *)(*(longlong *)(lVar1 + 8) + 0x2b0) != lVar8)))) ||
       (bVar19 = false, local_res10[0] != fVar20)) {
      bVar19 = true;
    }
  }
  if (DAT_140b15418[7] != 0) {
    bVar5 = FUN_1404944e0(DAT_140b15418[7],param_2);
    uVar18 = (ulonglong)bVar5;
  }
  if (bVar19) {
    if (DAT_140b15418 == (longlong *)0x0) {
      return 0x650000;
    }
    lVar8 = (**(code **)(*DAT_140b15418 + 0x228))();
    if (lVar8 == 0) {
      return 0x650000;
    }
    plVar13 = (longlong *)FUN_140426010(lVar8);
    if (plVar13 == (longlong *)0x0) {
      return 0x650000;
    }
    FUN_140426d50(lVar8,param_2);
    (**(code **)(*plVar13 + 0x18))(plVar13,param_2,0);
    local_c8[0] = 0;
    local_c8[1] = 0;
    local_b8 = _DAT_140800850;
    uStack_b4 = _UNK_140800854;
    uStack_b0 = _UNK_140800858;
    uStack_ac = _UNK_14080085c;
    local_a8 = 0x5d5e0b6b;
    local_90 = 0;
    local_88 = 0;
    local_80 = 0;
    local_7c = 0x3f;
    local_74 = 0x5d5e0b6b;
    local_6c = 0;
    local_68 = 0x1000001;
    local_a4 = 3;
    local_a0 = *(longlong *)(*(longlong *)(lVar1 + 8) + 0x2b0);
    if ((local_a0 == 0) || (*(char *)(local_a0 + 0x348) == -1)) {
      local_a0 = *(longlong *)(lVar1 + 8);
    }
    local_98 = *(undefined8 *)(lVar1 + 0x10);
    FUN_1404f8200(plVar13,local_c8);
  }
  else if (((((*(longlong *)(lVar1 + 8) != 0) &&
             ((*(byte *)(*(longlong *)(lVar1 + 8) + 0x3a0) & 2) == 0)) &&
            (cVar3 = (**(code **)(*param_1 + 0x108))(param_1), cVar3 == '\0')) &&
           (((char)uVar18 == '\0' && (lVar10 != 0)))) && (*(longlong *)(lVar10 + 0x1f0) != 0)) {
    local_res10[0] = -NAN;
    lVar8 = FUN_14041a2e0(*(longlong *)(lVar10 + 0x1f0),local_res10,0xffffffff);
    if (local_res10[0] == -NAN) {
      lVar10 = 0;
    }
    else {
      lVar10 = FUN_140396cb0(lVar8);
    }
    if ((lVar8 != *(longlong *)(lVar1 + 8)) || (lVar10 != *(longlong *)(lVar1 + 0x10))) {
      plVar13 = (longlong *)param_2[0x56];
      if ((plVar13 == (longlong *)0x0) || ((char)plVar13[0x69] == -1)) {
        lVar8 = *param_2;
        plVar13 = param_2;
      }
      else {
        lVar8 = *plVar13;
      }
      plVar13 = (longlong *)(**(code **)(lVar8 + 0x10))(plVar13,1);
      if (plVar13 != (longlong *)0x0) {
        (**(code **)(*plVar13 + 0x260))
                  (plVar13,*(undefined8 *)(lVar1 + 8),*(undefined8 *)(lVar1 + 0x10));
      }
    }
  }
  lVar8 = *(longlong *)(lVar1 + 8);
  if (bVar2) {
    if (lVar8 != 0) {
      if (*(longlong *)(lVar1 + 0x88) == 0) {
        (**(code **)(*param_1 + 0xf8))(param_1,4);
      }
      else {
        cVar3 = FUN_140631cc0(*(longlong *)(lVar1 + 0x88),lVar8);
        if (cVar3 == '\x01') {
          cVar3 = FUN_140632050(*(undefined8 *)(lVar1 + 0x88),*(undefined8 *)(lVar1 + 8));
          if (cVar3 == '\0') {
            (**(code **)(*param_1 + 0xf8))(param_1,3);
          }
          else {
            (**(code **)(*param_1 + 0xf8))(param_1,4);
          }
        }
        else {
          (**(code **)(*param_1 + 0xd8))(param_1,1);
        }
      }
      if (*(longlong *)(lVar1 + 8) != 0) {
        cVar3 = FUN_14039ade0(param_2);
        local_f8 = *(ulonglong *)((longlong)param_2 + 0x84);
        local_f0 = *(undefined4 *)((longlong)param_2 + 0x8c);
        if ((cVar3 != '\0') && (cVar6 = FUN_1403970c0(param_2), cVar6 != '\0')) {
          lVar8 = FUN_1405569b0(local_118);
          cVar3 = '\0';
          if ((lVar8 != 0) && (cVar3 = '\0', *(int *)(lVar8 + 0x48) == 5)) {
            local_108 = 0;
            local_100 = 0;
            uVar11 = FUN_14054f570(lVar8 + 0x18);
            puVar14 = (ulonglong *)FUN_14020acd0(local_d8,&local_108,uVar11);
            local_f8 = *puVar14;
            local_f0 = (undefined4)puVar14[1];
          }
        }
        if ((((DAT_140b16dd0 != '\0') ||
             (cVar6 = (**(code **)(*param_1 + 0x108))(param_1), cVar6 != '\0')) ||
            ((param_2[0x5a] == 0 || (*(int *)(param_2[0x5a] + 0x10) == 0)))) &&
           ((((*(char *)((longlong)param_2 + 0x333) != -1 && (cVar3 == '\0')) &&
             (plVar13 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1),
             plVar13 != (longlong *)0x0)) &&
            (cVar3 = (**(code **)(*plVar13 + 0x210))(plVar13), cVar3 == '\0')))) {
          if (*(longlong *)(lVar1 + 0x10) == 0) {
            local_108 = *(undefined8 *)(*(longlong *)(lVar1 + 8) + 0x78);
            local_100 = *(undefined4 *)(*(longlong *)(lVar1 + 8) + 0x80);
          }
          else {
            puVar15 = (undefined8 *)
                      TeamTargetingBehaviorClass::FUN_140396ce0
                                (*(undefined8 *)(lVar1 + 8),&local_e4,
                                 *(undefined4 *)(*(longlong *)(lVar1 + 0x10) + 0x18));
            local_108 = *puVar15;
            local_100 = *(undefined4 *)(puVar15 + 1);
          }
          FUN_14020acd0(&local_118,param_2 + 0xf,&local_108);
          if ((*(char *)((longlong)param_2 + 0x344) != -1) &&
             (lVar8 = (**(code **)(*param_2 + 0x10))(param_2,0x12), lVar8 != 0)) {
            fVar20 = (float)FUN_1404f5800(lVar8,local_110 - *(float *)((longlong)param_2 + 0x8c));
            local_110 = local_110 + fVar20;
          }
          FUN_14020b100(&local_f8,local_d8,&local_118);
          local_res10[0] = (float)FUN_14020b6d0(local_d0);
          local_res10[0] = ABS(local_res10[0]);
          if (DAT_140805898 < local_res10[0]) {
            fVar20 = DAT_1408007dc * DAT_1408007d4;
            *(undefined1 *)(lVar1 + 0x140) = 1;
            fVar20 = (fVar20 / DAT_1408007f4) * local_110;
            local_e0 = fVar20;
            if (0x1d000000 < ((uint)fVar20 & 0x7f800000)) {
              local_res10[0] = fVar20;
              local_e0 = (float)FUN_140776650(fVar20);
            }
            local_e4 = DAT_1407ffaf8;
            local_res10[0] = fVar20;
            if (0x1d000000 < ((uint)fVar20 & 0x7f800000)) {
              local_e4 = FUN_140776150(fVar20);
            }
            local_dc = 0;
            (**(code **)(*plVar13 + 0x218))(plVar13,&local_e4);
          }
        }
      }
      goto LAB_140634779;
    }
    uVar11 = 0;
  }
  else {
    bVar2 = false;
    if (lVar8 != 0) {
      lVar8 = *(longlong *)(lVar8 + 0xa8);
      if (param_2[0x56] == 0) {
        lVar10 = param_2[0x15];
      }
      else {
        lVar10 = *(longlong *)(param_2[0x56] + 0xa8);
      }
      if ((((lVar8 != 0) && (lVar10 != 0)) && (*(int *)(lVar10 + 0x1c4) != -1)) &&
         ((*(int *)(lVar10 + 0x1c4) == *(int *)(lVar8 + 0x1c4) &&
          (*(int *)(lVar10 + 0x1c8) == *(int *)(lVar8 + 0x1c8))))) {
        bVar2 = true;
      }
    }
    cVar3 = FUN_14039b1a0(param_2);
    if ((cVar3 != '\0') &&
       (plVar13 = (longlong *)
                  (**(code **)(**(longlong **)(param_1[5] + 0x2b0) + 0x10))
                            (*(longlong **)(param_1[5] + 0x2b0),1), plVar13 != (longlong *)0x0)) {
      iVar7 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if ((iVar7 == 2) || (cVar3 = (**(code **)(*plVar13 + 0x230))(plVar13), cVar3 != '\0')) {
        param_2 = *(longlong **)(param_1[5] + 0x2b0);
      }
    }
    bVar19 = false;
    if (param_2[0x15] != 0) {
      lVar8 = *(longlong *)(param_2[0x15] + 0x1f0);
      if (lVar8 == 0) {
        bVar19 = true;
      }
      else {
        lVar8 = FUN_14041a2e0(lVar8,0,0xffffffff);
        bVar19 = lVar8 == *(longlong *)(lVar1 + 8);
      }
    }
    cVar3 = (**(code **)(*param_1 + 0x108))(param_1);
    if (((cVar3 == '\0') && (!bVar2)) && (!bVar19)) {
      if ((char)uVar18 == '\0') {
        (**(code **)(*param_1 + 0xd8))
                  (param_1,*(longlong *)(lVar1 + 8) == *(longlong *)(lVar1 + 0x148));
      }
      goto LAB_140634779;
    }
    uVar11 = 2;
  }
  (**(code **)(*param_1 + 0xf8))(param_1,uVar11);
LAB_140634779:
  if (((*(int *)(lVar1 + 0x18) == 4) && (lVar8 = *(longlong *)(lVar1 + 8), lVar8 != 0)) &&
     ((lVar8 == *(longlong *)(lVar1 + 0x148) &&
      (*(char *)(*(longlong *)(lVar8 + 0x298) + 0x9e) != '\0')))) {
    (**(code **)(*param_1 + 0xf8))(param_1,2);
  }
  return 0x650000;
}


