/* FUN_1403825b0  RVA 0x3825b0  size 4034  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_1403825b0(longlong param_1,longlong *param_2,longlong param_3)

{
  float fVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  float *pfVar10;
  undefined8 *puVar11;
  longlong *plVar12;
  longlong lVar13;
  float fVar14;
  float extraout_XMM0_Da;
  float fVar15;
  float extraout_XMM0_Da_00;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  undefined8 local_res8;
  undefined **local_res20;
  uint uVar19;
  float local_2d8;
  float fStack_2d4;
  float local_2d0;
  undefined8 local_2c8;
  float local_2c0;
  undefined4 local_2b8;
  undefined4 uStack_2b4;
  float local_2b0;
  undefined8 local_2a8;
  float local_2a0;
  undefined8 local_298;
  undefined8 uStack_290;
  undefined8 local_288;
  undefined8 uStack_280;
  undefined8 local_278;
  undefined8 uStack_270;
  undefined8 local_268;
  undefined8 uStack_260;
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  longlong local_238;
  undefined8 local_230;
  float local_228;
  undefined1 local_220 [12];
  undefined1 local_214 [12];
  undefined1 local_208 [192];
  undefined1 local_148 [272];
  
  if (*(longlong *)(param_1 + 0x10) == 0) {
    return 0;
  }
  if (param_2 == (longlong *)0x0) {
    return 0;
  }
  if ((param_3 != 0) && (*(longlong **)(param_3 + 0x10) != param_2)) {
    return 0;
  }
  if ((*(byte *)(param_2 + 0x74) & 0x40) != 0) {
    return 0;
  }
  lVar7 = (**(code **)(*param_2 + 0x10))(param_2,0x11);
  if (lVar7 != 0) {
    return 0;
  }
  cVar2 = FUN_14039b140(param_2);
  if ((cVar2 != '\0') &&
     (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x298) + 0xa4) == '\0')) {
    return 0;
  }
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x100);
  if (lVar7 != 0) {
    if (0 < *(int *)(lVar7 + 0x394)) {
      return 0;
    }
    if ((*(int *)(lVar7 + 0x394) < 0) && (*(int *)(*(longlong *)(param_1 + 0x20) + 0x48) != 10)) {
      return 0;
    }
  }
  local_238 = lVar7;
  cVar2 = FUN_140540140(*(undefined8 *)(param_1 + 0x20),param_2);
  if (cVar2 == '\x01') {
    return 0;
  }
  cVar2 = FUN_14035f470(DAT_140b15418,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58),param_2,
                        1);
  if (cVar2 == '\x01') {
    return 0;
  }
  if (((*(char *)(*(longlong *)(param_1 + 0x10) + 0x3b4) == '\x01') ||
      (*(char *)((longlong)param_2 + 0x3b4) == '\x01')) &&
     (cVar2 = FUN_14039a540(*(longlong *)(param_1 + 0x10),param_2), cVar2 == '\0')) {
    return 0;
  }
  if (param_3 == 0) {
    uVar8 = FUN_140398440(*(undefined8 *)(param_1 + 0x10),local_214);
    param_3 = FUN_140394a80(param_2,uVar8,0,0);
  }
  fVar1 = DAT_1407ffaf8;
  if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4e) == '\x01') {
    lVar9 = FUN_140385cf0(param_1);
    if (lVar9 == 0) {
      return 0;
    }
    if (*(int *)(param_1 + 0x94) < 0) {
      return 0;
    }
    cVar2 = FUN_14012d2c0(lVar9,*(int *)(param_1 + 0x94),&local_298);
    if (cVar2 == '\0') {
      return 0;
    }
    local_2a0 = (float)local_278;
    local_2a8._0_4_ = (float)local_298;
    local_2a8._4_4_ = (float)local_288;
    FUN_140139800(&local_2a8);
    local_2c0 = *(float *)(param_2 + 0x10) - uStack_270._4_4_;
    local_2c8 = CONCAT44(*(float *)((longlong)param_2 + 0x7c) - uStack_280._4_4_,
                         *(float *)(param_2 + 0xf) - uStack_290._4_4_);
    FUN_140139800(&local_2c8);
    local_2a8._0_4_ = (float)local_2a8 * (float)local_2c8;
    local_2a8._4_4_ = local_2a8._4_4_ * local_2c8._4_4_;
    fVar17 = local_2a0 * local_2c0;
    fVar14 = ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) *
             *(float *)(*(longlong *)(param_1 + 0x20) + 0x200);
    local_res8 = (undefined **)CONCAT44(local_res8._4_4_,fVar14);
    fVar15 = fVar1;
    if (0x1d000000 < ((uint)fVar14 & 0x7f800000)) {
      FUN_140776150();
      fVar15 = extraout_XMM0_Da;
    }
    if (local_2a8._4_4_ + (float)local_2a8 + fVar17 < fVar15) {
      return 0;
    }
  }
  lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x220);
  lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x210);
  if (((lVar9 != 0) && (lVar7 != 0)) && (*(int *)(lVar7 + 0x394) < 0)) {
    lVar13 = lVar9;
  }
  if (param_3 == 0) {
    uVar16 = 0xffffffff;
  }
  else {
    uVar16 = *(undefined4 *)(param_3 + 0x18);
  }
  lVar7 = FUN_140397e00(*(undefined8 *)(param_1 + 0x10),param_2,uVar16);
  if ((lVar7 != 0) ||
     (((lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2b0), lVar7 == 0 ||
       ((cVar2 = FUN_14039b950(lVar7,0x29,1), cVar2 == '\0' &&
        (cVar2 = FUN_14039b950(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2b0),0x22,1),
        cVar2 == '\0')))) && (lVar7 = lVar13, lVar13 != 0)))) {
    if (param_3 == 0) {
      FUN_140398440(param_2);
      local_res8 = (undefined **)((ulonglong)local_res8 & 0xffffffffffffff00);
      if ((char)param_2[0x69] != -1) {
        uVar8 = (**(code **)(*param_2 + 0x10))(param_2,0x16);
        param_2 = (longlong *)UnitAIBehaviorClass::FUN_140405870(uVar8);
        if (param_2 == (longlong *)0x0) {
          return 0;
        }
      }
      pfVar10 = (float *)FUN_140383f70(param_1,&local_2c8,param_2,&local_res8);
      local_2d8 = *pfVar10;
      fStack_2d4 = pfVar10[1];
      local_2d0 = pfVar10[2];
      if ((char)local_res8 == '\0') {
        return 0;
      }
    }
    else {
      pfVar10 = (float *)FUN_140385c70(param_3,&local_2c8);
      local_2d8 = *pfVar10;
      fStack_2d4 = pfVar10[1];
      local_2d0 = pfVar10[2];
    }
    local_268 = _DAT_140800820;
    uStack_260 = _UNK_140800828;
    local_258 = _DAT_140800830;
    uStack_250 = _UNK_140800838;
    local_2c8 = 0;
    local_248 = _DAT_140800840;
    uStack_240 = _UNK_140800848;
    local_298 = _DAT_140800820;
    uStack_290 = _UNK_140800828;
    local_288 = _DAT_140800830;
    uStack_280 = _UNK_140800838;
    local_278 = _DAT_140800840;
    uStack_270 = _UNK_140800848;
    cVar2 = FUN_140385e70(param_1,&local_2c8,&local_268,&local_298);
    if (cVar2 != '\0') {
      fVar15 = (float)FUN_140776d48((local_2c8._4_4_ - fStack_2d4) * (local_2c8._4_4_ - fStack_2d4)
                                    + ((float)local_2c8 - local_2d8) *
                                      ((float)local_2c8 - local_2d8));
      fVar14 = (float)FUN_1403857d0(param_1);
      FUN_140397780(param_2);
      if ((fVar15 <= fVar14 + extraout_XMM0_Da_00) &&
         (*(float *)(*(longlong *)(param_1 + 0x20) + 0x23c) <= fVar15)) {
        if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4f) == '\x01') {
          local_2b8 = FUN_1401ffbb0(&DAT_140a13e24);
          uStack_2b4 = FUN_1401ffbb0(&DAT_140a13e24);
          local_2c0 = (float)FUN_1401ffbb0(&DAT_140a13e24);
        }
        else if ((*(int *)(param_1 + 0x3c) < 0) ||
                (cVar2 = FUN_1401ffdb0(&DAT_140a13e24,0x32), cVar2 != '\0')) {
          local_2b8 = uStack_260._4_4_;
          local_2c0 = uStack_240._4_4_;
          uStack_2b4 = uStack_250._4_4_;
        }
        else {
          local_2b8 = uStack_290._4_4_;
          local_2c0 = uStack_270._4_4_;
          uStack_2b4 = uStack_280._4_4_;
        }
        local_2c8 = CONCAT44(uStack_2b4,local_2b8);
        local_2a8 = CONCAT44(fStack_2d4,local_2d8);
        local_2a0 = local_2d0;
        local_2b0 = local_2c0;
        uVar16 = FUN_140370f00(lVar7,0);
        pfVar10 = (float *)FUN_140399450(*(undefined8 *)(param_1 + 0x10),local_220,param_2,
                                         &local_2a8,&local_2c8,uVar16);
        fVar14 = *pfVar10;
        fVar17 = pfVar10[1];
        fVar18 = pfVar10[2];
        if ((fVar17 * fVar17 + fVar14 * fVar14 + fVar18 * fVar18 != 0.0) &&
           (local_2d8 = fVar14, fStack_2d4 = fVar17, local_2d0 = fVar18,
           cVar2 = FUN_140383ba0(param_1,&local_2d8), cVar2 != '\0')) {
          lVar9 = param_2[0x53];
          if (*(longlong *)(lVar9 + 0x1648) == 0) {
            lVar9 = FUN_1403973b0(param_2);
            puVar11 = (undefined8 *)(lVar9 + 0xf8);
            if (0xf < *(ulonglong *)(lVar9 + 0x110)) {
              puVar11 = (undefined8 *)*puVar11;
            }
            FUN_140025760("Error: Object type %s has no XML category mask assigned to it.\n",puVar11
                         );
            lVar9 = param_2[0x53];
          }
          local_2c8 = CONCAT44(fStack_2d4,local_2d8);
          local_2c0 = local_2d0;
          uVar16 = *(undefined4 *)(lVar7 + 0x1fe8);
          pfVar10 = (float *)FUN_140381dc0(param_1,local_220,fVar15,&local_2c8,
                                           *(undefined8 *)(lVar9 + 0x1648),uVar16);
          local_2d8 = *pfVar10;
          fStack_2d4 = pfVar10[1];
          local_2d0 = pfVar10[2];
          FUN_14020acd0(&local_2c8,&local_2b8,&local_2d8);
          local_228 = local_2c0;
          local_230 = local_2c8;
          lVar9 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x58)
                               );
          puVar11 = &local_230;
          uVar19 = CONCAT31((int3)((uint)uVar16 >> 8),1);
          plVar12 = (longlong *)
                    FUN_14029f810(*(GameObjectManagerClass **)
                                   (*(longlong *)(param_1 + 0x10) + 0x2b8),lVar7,
                                  *(int *)(lVar9 + 0x4c),&local_2b8,puVar11,1,0);
          iVar6 = 0;
          uVar16 = 0;
          local_res20 = (undefined **)0x0;
          if (DAT_140b30338 == '\0') {
            lVar9 = *(longlong *)(param_1 + 0x20);
            local_res20 = *(undefined ***)(lVar9 + 0x240);
            if (local_res20 == (undefined **)0x0) {
              lVar13 = *(longlong *)(param_1 + 0x10);
              local_res20 = *(undefined ***)(*(longlong *)(lVar13 + 0x298) + 0x1240);
              if ((local_res20 == (undefined **)0x0) || (0 < *(int *)(lVar9 + 0x4a4)))
              goto LAB_140382dbc;
            }
            else {
              if (0 < *(int *)(lVar9 + 0x4a4)) goto LAB_140382dbc;
              lVar13 = *(longlong *)(param_1 + 0x10);
            }
            FUN_1402d5240(&DAT_140b27e60,local_res20,plVar12 + 0xf,lVar13,
                          (ulonglong)puVar11 & 0xffffffff00000000,uVar19 & 0xffffff00);
          }
LAB_140382dbc:
          lVar9 = plVar12[0x1d];
          *(undefined4 *)(lVar9 + 0x58) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x50);
          *(undefined4 *)(lVar9 + 0x5c) = 0;
          fVar15 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x478);
          if (fVar15 <= 0.0) {
            fVar15 = *(float *)(lVar7 + 0x474);
          }
          *(float *)(lVar9 + 100) = fVar15;
          if ((local_238 != 0) && (*(int *)(local_238 + 0x394) < 0)) {
            *(int *)(local_238 + 0x394) = *(int *)(local_238 + 0x394) + 1;
            lVar13 = FUN_1403952a0(*(undefined8 *)(param_1 + 0x10),0x46);
            if (lVar13 != 0) {
              *(float *)(lVar9 + 100) = *(float *)(lVar13 + 0x17c) * *(float *)(lVar9 + 100);
            }
          }
          iVar3 = FUN_1405400f0(*(undefined8 *)(param_1 + 0x20));
          if (iVar3 == 0) {
            uVar4 = *(undefined4 *)(lVar7 + 0x440);
          }
          else {
            uVar4 = FUN_1405400f0(*(undefined8 *)(param_1 + 0x20));
          }
          *(undefined4 *)(lVar9 + 0x68) = uVar4;
          fVar15 = (float)FUN_1403857d0(param_1);
          if (fVar15 <= 0.0) {
            fVar15 = *(float *)(lVar7 + 0x4bc);
          }
          *(float *)(lVar9 + 0x6c) = fVar15;
          fVar15 = (float)FUN_140397780(param_2);
          *(float *)(lVar9 + 0x6c) = fVar15 + *(float *)(lVar9 + 0x6c);
          cVar2 = FUN_14039b1a0(param_2);
          if ((cVar2 != '\0') &&
             (cVar2 = FUN_140372210(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298)),
             cVar2 == '\0')) {
            fVar15 = (float)FUN_140397780(param_2[0x56]);
            *(float *)(lVar9 + 0x6c) = fVar15 + *(float *)(lVar9 + 0x6c);
          }
          *(longlong **)(lVar9 + 8) = param_2;
          local_res8 = (undefined **)
                       (CONCAT44(local_res8._4_4_,local_2d0 - local_2b0) & 0xffffffff7fffffff);
          *(float *)(lVar9 + 0x6c) = ABS(local_2d0 - local_2b0) + *(float *)(lVar9 + 0x6c);
          iVar3 = *(int *)(*(longlong *)(param_1 + 0x20) + 0x4a4);
          if (0 < iVar3) {
            if (DAT_140b15418 == 0) {
              iVar5 = 0;
            }
            else {
              iVar5 = *(int *)(DAT_140b15418 + 0x10);
            }
            *(int *)(lVar9 + 0x60) = iVar5 + iVar3;
            FUN_14039d6a0(plVar12);
            *(undefined ***)(lVar9 + 0xb0) = local_res20;
          }
          lVar13 = (**(code **)(*plVar12 + 0x10))(plVar12,8);
          if (lVar13 == 0) {
            local_res8 = (undefined **)0x0;
          }
          else {
            local_res8 = (undefined **)(lVar13 + 0x18);
          }
          uVar8 = FUN_140058570();
          FUN_140220e90(uVar8,param_2 + 7,local_res8,0x28);
          *(undefined4 *)(lVar9 + 0x5c) = 0;
          if (param_3 == 0) {
            uVar4 = 0xffffffff;
          }
          else {
            uVar4 = *(undefined4 *)(param_3 + 0x18);
          }
          *(undefined4 *)(lVar9 + 0x10) = uVar4;
          if (*(int *)(lVar7 + 0x1fe8) == 1) {
            lVar7 = *(longlong *)(lVar9 + 8);
            *(longlong *)(lVar9 + 0x18) = lVar7;
            *(undefined4 *)(lVar9 + 0x20) = uVar4;
            fVar17 = fStack_2d4 - fVar17;
            fVar18 = local_2d0 - fVar18;
            fVar14 = local_2d8 - fVar14;
            if (lVar7 != 0) {
              local_268 = *(undefined8 *)(lVar7 + 0x248);
              uStack_260 = *(undefined8 *)(lVar7 + 0x250);
              local_258 = *(undefined8 *)(lVar7 + 600);
              uStack_250 = *(undefined8 *)(lVar7 + 0x260);
              local_248 = *(undefined8 *)(lVar7 + 0x268);
              uStack_240 = *(undefined8 *)(lVar7 + 0x270);
              FUN_14022d410(&local_268,&local_298);
              *(float *)(lVar9 + 0x24) =
                   (float)local_298 * fVar14 + local_298._4_4_ * fVar17 + (float)uStack_290 * fVar18
              ;
              *(float *)(lVar9 + 0x28) =
                   (float)local_288 * fVar14 + local_288._4_4_ * fVar17 + (float)uStack_280 * fVar18
              ;
              *(float *)(lVar9 + 0x2c) =
                   (float)local_278 * fVar14 + local_278._4_4_ * fVar17 + (float)uStack_270 * fVar18
              ;
            }
          }
          else {
            *(undefined4 *)(lVar9 + 0x74) = *(undefined4 *)((longlong)plVar12 + 0x84);
            *(int *)(lVar9 + 0x78) = (int)plVar12[0x11];
            *(undefined4 *)(lVar9 + 0x7c) = *(undefined4 *)((longlong)plVar12 + 0x8c);
            *(int *)(lVar9 + 0x80) = (int)plVar12[0xf];
            *(undefined4 *)(lVar9 + 0x84) = *(undefined4 *)((longlong)plVar12 + 0x7c);
            *(int *)(lVar9 + 0x88) = (int)plVar12[0x10];
            *(undefined4 *)(lVar9 + 0x8c) =
                 *(undefined4 *)(*(longlong *)(plVar12[0x57] + 0x20) + 0x10);
            uVar4 = FUN_140370f00(lVar7,0);
            *(undefined4 *)(lVar9 + 0x90) = uVar4;
            *(undefined8 *)(lVar9 + 0x18) = 0;
            *(undefined4 *)(lVar9 + 0x20) = 0xffffffff;
            *(float *)(lVar9 + 0x3c) = local_2d8;
            *(float *)(lVar9 + 0x40) = fStack_2d4;
            *(float *)(lVar9 + 0x44) = local_2d0;
          }
          FUN_1403a06a0(*(undefined8 *)(param_1 + 0x10),param_2,plVar12);
          iVar3 = *(int *)(param_1 + 0x5c);
          if (iVar3 != 0) {
            iVar3 = iVar3 + -1;
            *(int *)(param_1 + 0x5c) = iVar3;
          }
          lVar7 = *(longlong *)(param_1 + 0x20);
          if (iVar3 == 0) {
            iVar6 = FUN_1401ffb40(&DAT_140a13e24,(int)(*(float *)(lVar7 + 0x228) * DAT_1408007f0),
                                  (int)(*(float *)(lVar7 + 0x22c) * DAT_1408007f0));
            *(int *)(param_1 + 0x58) =
                 (int)(((longlong)((float)iVar6 * (float)DAT_140b0a340) & 0xffffffffU) / 100);
            if (DAT_140b15418 != 0) {
              uVar16 = *(undefined4 *)(DAT_140b15418 + 0x10);
            }
            *(undefined4 *)(param_1 + 0x68) = uVar16;
            if ((*(int *)(param_1 + 200) != -1) &&
               (lVar7 = FUN_140294bc0(&DAT_140a16fd0), lVar7 != 0)) {
              FUN_140285d70(lVar7,param_1);
              *(undefined4 *)(param_1 + 200) = 0xffffffff;
            }
            iVar6 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),0);
            cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar6,1);
            fVar15 = fVar1;
            if (cVar2 != '\0') {
              fVar15 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x250 + (longlong)iVar6 * 4);
            }
            fVar14 = (float)FUN_140398010(*(undefined8 *)(param_1 + 0x10),1);
            fVar15 = fVar15 * fVar14;
            iVar6 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),1);
            cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar6,1);
            fVar14 = DAT_1408007c0;
            if (cVar2 != '\0') {
              fVar15 = fVar15 * *(float *)(*(longlong *)(param_1 + 0x20) + 0x250 +
                                          (longlong)iVar6 * 4);
            }
            if (fVar15 < fVar1) {
              *(int *)(param_1 + 0x58) =
                   (int)((float)*(uint *)(param_1 + 0x58) * fVar15 + DAT_1408007c0);
            }
            fVar15 = (float)FUN_140398010(*(undefined8 *)(param_1 + 0x10),9);
            FUN_140535cb0(local_208,0);
            FUN_140395c70(*(undefined8 *)(param_1 + 0x10),local_208);
            local_res8 = GreaterThan<float>::vftable;
            local_res20 = Plus<float>::vftable;
            fVar17 = (float)FUN_14033fb70(local_148,&local_res20,&local_res8);
            fVar15 = fVar15 * (fVar17 + fVar1);
            if ((0.0 < fVar15) && (fVar15 < fVar1)) {
              *(int *)(param_1 + 0x58) = (int)((float)*(uint *)(param_1 + 0x58) / fVar15 + fVar14);
            }
            if (fVar15 <= 0.0) {
              fVar15 = 0.0;
            }
            *(int *)(param_1 + 0x5c) =
                 (int)((float)*(uint *)(*(longlong *)(param_1 + 0x20) + 0x230) * fVar15 + fVar14);
          }
          else {
            *(int *)(param_1 + 0x58) =
                 (int)(longlong)((float)DAT_140b0a340 * *(float *)(lVar7 + 0x234));
            iVar3 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),0);
            cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar3,1);
            fVar15 = fVar1;
            if (cVar2 != '\0') {
              fVar15 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x250 + (longlong)iVar3 * 4);
            }
            fVar14 = (float)FUN_140398010(*(undefined8 *)(param_1 + 0x10),1);
            fVar15 = fVar15 * fVar14;
            iVar3 = FUN_140374b50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),1);
            cVar2 = FUN_14039b950(*(undefined8 *)(param_1 + 0x10),iVar3,1);
            fVar14 = DAT_1408007c0;
            if (cVar2 != '\0') {
              fVar15 = fVar15 * *(float *)(*(longlong *)(param_1 + 0x20) + 0x250 +
                                          (longlong)iVar3 * 4);
            }
            *(int *)(param_1 + 0x58) =
                 (int)((float)*(uint *)(param_1 + 0x58) * fVar15 + DAT_1408007c0);
            fVar15 = (float)FUN_140398010(*(undefined8 *)(param_1 + 0x10),9);
            FUN_140535cb0(local_208,0);
            FUN_140395c70(*(undefined8 *)(param_1 + 0x10),local_208);
            local_res8 = GreaterThan<float>::vftable;
            local_res20 = Plus<float>::vftable;
            fVar17 = (float)FUN_14033fb70(local_148,&local_res20,&local_res8);
            fVar15 = fVar15 * (fVar17 + fVar1);
            if (fVar15 <= 0.0) {
              *(undefined4 *)(param_1 + 0x5c) = 0;
            }
            else {
              iVar6 = (int)((float)*(uint *)(param_1 + 0x58) / fVar15 + fVar14);
            }
            *(int *)(param_1 + 0x58) = iVar6;
          }
          FUN_140535fb0(local_208);
          if (*(longlong **)(param_1 + 0xa8) != param_2) {
            if (*(longlong **)(param_1 + 0xa8) != (longlong *)0x0) {
              FUN_1403846c0(param_1,param_1 + 0xa8);
            }
            FUN_140382510(param_1,param_2,param_1 + 0xa8);
          }
          return 1;
        }
      }
    }
  }
  return 0;
}


