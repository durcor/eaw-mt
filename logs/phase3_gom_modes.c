
// ================================================================
// GALACTIC MAP SUB-TICKS (MODE-1) — GOM CANDIDATES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3e8a80  Name: FUN_1403e8a80  Size: 5921 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403e8a80(longlong param_1)

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  undefined4 uVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  ulonglong ******ppppppuVar9;
  char cVar10;
  int iVar11;
  int iVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  longlong *plVar15;
  longlong *plVar16;
  longlong lVar17;
  undefined8 *puVar18;
  undefined8 *puVar19;
  undefined8 uVar20;
  ulonglong *******pppppppuVar21;
  undefined8 uVar22;
  void *_Buf2;
  ulonglong uVar23;
  ulonglong uVar24;
  ulonglong uVar25;
  longlong lVar26;
  uint uVar27;
  undefined8 extraout_XMM0_Qa;
  ulonglong ******local_2d8;
  ulonglong *****pppppuStack_2d0;
  ulonglong local_2c8;
  ulonglong uStack_2c0;
  undefined8 local_2b8;
  ulonglong *****pppppuStack_2b0;
  ulonglong local_2a8;
  ulonglong uStack_2a0;
  float local_298;
  float local_294;
  float local_290;
  undefined8 local_288;
  undefined8 uStack_280;
  ulonglong local_278;
  ulonglong local_270;
  ulonglong ******local_268 [2];
  ulonglong local_258;
  ulonglong local_250;
  undefined8 local_248;
  undefined8 local_238;
  ulonglong local_230;
  float local_228;
  float local_224;
  float local_220;
  undefined4 local_21c;
  float local_218;
  uint local_214;
  float local_210;
  undefined4 local_20c;
  float local_208;
  float local_204;
  float local_200;
  undefined4 local_1fc;
  short local_1f8 [16];
  undefined1 local_1d8 [12];
  float local_1cc;
  float local_1bc;
  float local_1ac;
  short local_1a8 [184];
  
  if (*(int *)(param_1 + 0x211c) != 1) {
    return;
  }
  if (*(longlong *)(param_1 + 0x2150) == 0) {
    local_1a8[0] = 0x20;
    local_1a8[1] = 0;
    local_2d8 = (ulonglong ******)0x0;
    local_2c8 = _DAT_1407fffd0;
    uStack_2c0 = _UNK_1407fffd8;
    lVar26 = -1;
    do {
      lVar26 = lVar26 + 1;
    } while (local_1a8[lVar26] != 0);
    FUN_1400228a0(&local_2d8,local_1a8);
    FUN_140347c30(3,&local_2d8);
    local_2c8 = _DAT_1407fffd0;
    uStack_2c0 = _UNK_1407fffd8;
    local_1a8[0] = 0x20;
    local_1a8[1] = 0;
    lVar26 = -1;
    local_2d8 = (ulonglong ******)0x0;
    do {
      lVar26 = lVar26 + 1;
    } while (local_1a8[lVar26] != 0);
    FUN_1400228a0(&local_2d8,local_1a8);
    FUN_140347c30(4,&local_2d8);
    plVar15 = (longlong *)FUN_140347b50(5);
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0xa0))(plVar15,0,0);
    }
    plVar15 = (longlong *)FUN_140347b50(0x4a);
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0xa0))(plVar15,0,0);
    }
  }
  else {
    if (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x2150) + 0xb8) + 0x2c9) == '\0') {
      FUN_1403de570(param_1,0,0);
      return;
    }
    lVar26 = FUN_1403956c0();
    lVar26 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(lVar26 + 4));
    lVar26 = *(longlong *)(lVar26 + 0x68);
    FUN_1401f92f0(&local_288);
    cVar10 = FUN_14050bf10(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
    uVar23 = 0;
    if (cVar10 == '\x01') {
      puVar18 = (undefined8 *)FUN_14033e340(lVar26,0);
      local_288 = (undefined8 *)*puVar18;
      uStack_280 = puVar18[1];
    }
    else {
      local_2b8._0_6_ = CONCAT24(s_Neutral_140817d08._4_2_,s_Neutral_140817d08._0_4_);
      local_2b8._0_7_ = CONCAT16(s_Neutral_140817d08[6],(undefined6)local_2b8);
      uStack_2a0 = 0xf;
      local_2a8 = 7;
      local_2b8 = (ulonglong ******)(ulonglong)(uint7)local_2b8;
      uVar20 = FUN_1403303d0(&DAT_140a172d0,&local_2b8);
      puVar18 = (undefined8 *)FUN_14033e340(uVar20,0);
      local_288 = (undefined8 *)*puVar18;
      uStack_280 = puVar18[1];
      if (0xf < uStack_2a0) {
        if (0xfff < uStack_2a0 + 1) {
          if (0x1f < (ulonglong)((longlong)local_2b8 + (-8 - (longlong)local_2b8[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_2b8[-1],uStack_2a0 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
    }
    plVar15 = (longlong *)FUN_140347b50(3);
    plVar16 = (longlong *)FUN_140347b50(699);
    fVar2 = DAT_1408054e8;
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x128))
                (plVar15,(int)((float)local_288 * DAT_1408054e8),
                 (int)(local_288._4_4_ * DAT_1408054e8) & 0xff,
                 (int)((float)uStack_280 * DAT_1408054e8),
                 (int)(uStack_280._4_4_ * DAT_1408054e8) & 0xff,1);
    }
    if (plVar16 != (longlong *)0x0) {
      (**(code **)(*plVar16 + 0x128))
                (plVar16,(int)((float)local_288 * fVar2),(int)(local_288._4_4_ * fVar2) & 0xff,
                 (int)((float)uStack_280 * fVar2),(int)(uStack_280._4_4_ * fVar2) & 0xff,1);
    }
    uVar20 = (**(code **)(**(longlong **)(param_1 + 0x2150) + 0x10))
                       (*(longlong **)(param_1 + 0x2150),3);
    FUN_1403f9610(uVar20,*(undefined8 *)(param_1 + 0x2150));
    uVar13 = FUN_14022f9c0();
    FUN_140022b30(local_1a8,0x80,L"$ %d",uVar13);
    local_2d8 = (ulonglong ******)0x0;
    lVar17 = -1;
    local_2c8 = 0;
    uStack_2c0 = 7;
    do {
      lVar17 = lVar17 + 1;
    } while (local_1a8[lVar17] != 0);
    FUN_1400228a0(&local_2d8,local_1a8);
    uVar24 = local_2c8;
    pppppppuVar21 = &local_2d8;
    if (7 < uStack_2c0) {
      pppppppuVar21 = (ulonglong *******)local_2d8;
    }
    local_2b8 = (ulonglong ******)0x0;
    local_2a8 = 0;
    uStack_2a0 = 0;
    if (local_2c8 < 8) {
      local_2b8 = *pppppppuVar21;
      pppppuStack_2b0 = (ulonglong *****)pppppppuVar21[1];
      uStack_2a0 = 7;
    }
    else {
      uVar25 = local_2c8 | 7;
      if (0x7ffffffffffffffe < uVar25) {
        uVar25 = 0x7ffffffffffffffe;
      }
      local_2b8 = (ulonglong ******)FUN_1400226c0(&local_2b8,uVar25 + 1);
      FUN_14076bd80(local_2b8,pppppppuVar21,uVar24 * 2 + 2);
      uStack_2a0 = uVar25;
    }
    local_2a8 = uVar24;
    FUN_140347c30(4,&local_2b8);
    plVar15 = (longlong *)FUN_140347b50(4);
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x128))
                (plVar15,(int)((float)local_288 * fVar2),(int)(local_288._4_4_ * fVar2) & 0xff,
                 (int)((float)uStack_280 * fVar2) & 0xff,(int)(uStack_280._4_4_ * fVar2) & 0xff,1);
    }
    plVar15 = (longlong *)FUN_140347b50(700);
    if (plVar15 != (longlong *)0x0) {
      (**(code **)(*plVar15 + 0x128))
                (plVar15,(int)((float)local_288 * fVar2),(int)(local_288._4_4_ * fVar2) & 0xff,
                 (int)((float)uStack_280 * fVar2) & 0xff,(int)(uStack_280._4_4_ * fVar2) & 0xff,1);
    }
    plVar15 = (longlong *)FUN_140347b50(5);
    plVar16 = (longlong *)FUN_140347b50(0x2ba);
    if (plVar15 != (longlong *)0x0) {
      cVar10 = FUN_14050bf10(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
      uVar24 = uVar23;
      if (cVar10 == '\x01') {
        lVar17 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2708);
        if (lVar26 == lVar17) {
          uVar22 = 0;
        }
        else {
          lVar17 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2728);
          if (lVar26 == lVar17) {
            uVar22 = 1;
          }
          else {
            lVar17 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2748);
            if (lVar26 == lVar17) {
              uVar22 = 2;
            }
            else {
              local_2b8._0_6_ = CONCAT24(s_Pirates_14085b050._4_2_,s_Pirates_14085b050._0_4_);
              local_2b8._0_7_ = CONCAT16(s_Pirates_14085b050[6],(undefined6)local_2b8);
              uStack_2a0 = 0xf;
              local_2a8 = 7;
              local_2b8 = (ulonglong ******)(ulonglong)(uint7)local_2b8;
              lVar17 = FUN_1403303d0(&DAT_140a172d0,&local_2b8);
              if (0xf < uStack_2a0) {
                if (0xfff < uStack_2a0 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_2b8 + (-8 - (longlong)local_2b8[-1]))) {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_2b8[-1],uStack_2a0 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
              uVar22 = 3;
              if (lVar26 != lVar17) {
                uVar22 = 4;
              }
            }
          }
        }
        uVar24 = FUN_14034c200(plVar15,uVar22);
      }
      (**(code **)(*plVar15 + 0xa0))(plVar15,uVar24,0);
      (**(code **)(*plVar16 + 0xa0))(plVar16,uVar24,0);
    }
    plVar15 = (longlong *)FUN_140347b50(0x45);
    plVar16 = (longlong *)FUN_140347b50(0x46);
    cVar10 = FUN_140375220(*(undefined8 *)(*(longlong *)(param_1 + 0x2150) + 0x298),
                           *(longlong *)(param_1 + 0x2150));
    if (cVar10 == '\0') {
      (**(code **)(*plVar15 + 0x208))(plVar15);
      (**(code **)(*plVar16 + 0x80))(plVar16,1);
    }
    else {
      (**(code **)(*plVar16 + 0x80))(plVar16,0);
    }
    iVar12 = 0;
    if (*(int *)(DAT_140b2eea0 + 0x44) == 3) {
      cVar10 = FUN_14050c300(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
      iVar11 = iVar12;
      if (cVar10 == '\x01') {
        iVar12 = FUN_1403f8af0(uVar20,*(undefined8 *)(param_1 + 0x2150));
        iVar11 = FUN_1403f8b10(uVar20,*(undefined8 *)(param_1 + 0x2150));
      }
      uVar13 = FUN_140372740(*(undefined8 *)(*(longlong *)(param_1 + 0x2150) + 0x298),
                             *(longlong *)(param_1 + 0x2150));
      uVar24 = uVar23;
      if (iVar12 != 0) {
        uVar24 = FUN_1403f8b60(uVar20,*(undefined8 *)(param_1 + 0x2150));
      }
      FUN_1403e1280(param_1,uVar24,0x6a,0x72,uVar13);
      lVar26 = FUN_1403f94e0(uVar20,*(undefined8 *)(param_1 + 0x2150));
      iVar12 = 0x5d;
      uVar24 = uVar23;
      do {
        plVar15 = (longlong *)FUN_140347b50(iVar12);
        if (((longlong)uVar24 < (longlong)iVar11) &&
           (lVar17 = *(longlong *)(*(longlong *)(lVar26 + 8) + uVar24 * 8), lVar17 != 0)) {
          FUN_14001e680(&local_248,*(longlong *)(lVar17 + 0x298) + 0x1820);
          puVar18 = &local_248;
          if (0xf < local_230) {
            puVar18 = local_248;
          }
          (**(code **)(*plVar15 + 0xa0))(plVar15,puVar18);
          if (0xf < local_230) {
            if (0xfff < local_230 + 1) {
              if (0x1f < (ulonglong)((longlong)local_248 + (-8 - local_248[-1]))) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(local_248[-1],local_230 + 0x28);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        else {
          (**(code **)(*plVar15 + 0xa0))(plVar15,0,0);
        }
        FUN_140353ad0(plVar15);
        iVar12 = iVar12 + 1;
        uVar24 = uVar24 + 1;
      } while ((longlong)uVar24 < 2);
      if ((*(longlong *)(param_1 + 0xdf0) == 0) && (0 < *(int *)(param_1 + 0xde8))) {
        uVar24 = uVar23;
        plVar15 = *(longlong **)(param_1 + 0xde0);
        do {
          if (*plVar15 == *(longlong *)(param_1 + 0x2150)) {
            *(longlong **)(param_1 + 0xdf0) =
                 *(longlong **)(param_1 + 0xde0) + (longlong)(int)uVar24 * 0x31;
            break;
          }
          uVar27 = (int)uVar24 + 1;
          uVar24 = (ulonglong)uVar27;
          plVar15 = plVar15 + 0x31;
        } while ((int)uVar27 < *(int *)(param_1 + 0xde8));
      }
      FUN_1402e2910(param_1,*(undefined8 *)(param_1 + 0x2150),1);
    }
    plVar15 = (longlong *)FUN_140347b50(9);
    if (plVar15 != (longlong *)0x0) {
      puVar18 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,"TEXT_BUTTON_PLANET_INFO");
      local_2b8 = (ulonglong ******)0x0;
      local_2a8 = 0;
      uStack_2a0 = 0;
      uVar24 = puVar18[2];
      if (7 < (ulonglong)puVar18[3]) {
        puVar18 = (undefined8 *)*puVar18;
      }
      if (uVar24 < 8) {
        local_2b8 = (ulonglong ******)*puVar18;
        pppppuStack_2b0 = (ulonglong *****)puVar18[1];
        uStack_2a0 = 7;
      }
      else {
        uVar25 = uVar24 | 7;
        if (0x7ffffffffffffffe < uVar25) {
          uVar25 = 0x7ffffffffffffffe;
        }
        local_2b8 = (ulonglong ******)FUN_1400226c0(&local_2b8,uVar25 + 1);
        FUN_14076bd80(local_2b8,puVar18,uVar24 * 2 + 2);
        uStack_2a0 = uVar25;
      }
      local_2a8 = uVar24;
      (**(code **)(*plVar15 + 0x148))(plVar15,&local_2b8,1,0);
      if (7 < uStack_2a0) {
        if (0xfff < uStack_2a0 * 2 + 2) {
          if (0x1f < (ulonglong)((longlong)local_2b8 + (-8 - (longlong)local_2b8[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_2b8[-1],uStack_2a0 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
      }
    }
    if ((*(int *)(DAT_140b2eea0 + 0x44) == 3) &&
       (lVar26 = FUN_140294a40(&DAT_140a16fd0), *(char *)(lVar26 + 0x3c0) != '\0')) {
      uVar20 = 1;
    }
    else {
      uVar20 = 0;
    }
    (**(code **)(*plVar15 + 0x78))(plVar15,uVar20);
    local_268[0] = (ulonglong ******)0x0;
    local_258 = 0;
    local_250 = 0xf;
    cVar10 = FUN_140446220(&DAT_140b30350,4,local_268,local_268);
    if ((cVar10 != '\0') && (plVar15 = (longlong *)FUN_140347b50(0x4a), plVar15 != (longlong *)0x0))
    {
      lVar26 = *plVar15;
      uVar20 = FUN_14034c200(plVar15,(*(int *)(DAT_140b2eea0 + 0x44) - 1U & 0xfffffffd) != 0);
      (**(code **)(lVar26 + 0xa0))(plVar15,uVar20,0);
    }
    if (*(int *)(DAT_140b2eea0 + 0x44) == 0) {
      lVar26 = *(longlong *)(param_1 + 0x2150);
      if ((param_1 != -0x2248) &&
         (cVar10 = FUN_14030a6a0(param_1), plVar15 = DAT_140b15418, cVar10 == '\0')) {
        if (*(longlong *)(param_1 + 0x2150) != 0) {
          lVar17 = FUN_140294bc0(&DAT_140a16fd0,
                                 *(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x58));
          lVar17 = *(longlong *)(lVar17 + 0x68);
          cVar10 = FUN_14050bf10(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
          if (cVar10 != '\x01') {
            do {
              local_230 = 0xf;
              local_238 = 7;
              local_248 = (undefined8 *)
                          (ulonglong)
                          CONCAT16(s_Neutral_140817d08[6],
                                   CONCAT24(s_Neutral_140817d08._4_2_,s_Neutral_140817d08._0_4_));
              lVar17 = FUN_1403303d0(&DAT_140a172d0,&local_248);
              if (0xf < local_230) {
                if (0xfff < local_230 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_248 + (-8 - local_248[-1]))) {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_248[-1],local_230 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
              uVar27 = (int)uVar23 + 1;
              uVar23 = (ulonglong)uVar27;
            } while ((int)uVar27 < 4);
          }
          if (lVar17 != 0) {
            if ((plVar15 != (longlong *)0x0) &&
               (iVar12 = (**(code **)(*plVar15 + 0xe0))(plVar15), iVar12 == 0)) {
              if ((*(char *)(param_1 + 0x2268) == '\0') ||
                 (*(longlong *)(param_1 + 0x2270) != lVar17)) {
                FUN_1403dd680(param_1);
                *(undefined4 *)(param_1 + 0x226c) = 0;
                *(longlong *)(param_1 + 0x2270) = lVar17;
                plVar16 = (longlong *)(**(code **)(*plVar15 + 0x100))(plVar15);
                (**(code **)(*plVar16 + 0x28))
                          (plVar16,*(undefined8 *)
                                    (param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8));
                FUN_140264c80(*(undefined8 *)
                               (param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8),0);
                FUN_140264670(*(undefined8 *)
                               (param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8),1);
                FUN_140265560(*(undefined8 *)
                               (param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8),0,0,1,0
                              ,DAT_1407ffaf8,(float)DAT_140b0a320 / (float)DAT_140b0a340,0);
                uVar20 = FUN_14033e340(lVar17,0);
                FUN_1401f9320(&local_2b8,uVar20);
                uVar13 = (float)local_2b8;
                uVar14 = local_2b8._4_4_;
                uVar4 = pppppuStack_2b0._0_4_;
                lVar17 = FUN_1402648b0(*(undefined8 *)
                                        (param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8
                                        ));
                *(undefined4 *)(lVar17 + 0x13c) = uVar13;
                *(undefined4 *)(lVar17 + 0x140) = uVar14;
                *(undefined4 *)(lVar17 + 0x144) = uVar4;
                *(undefined4 *)(lVar17 + 0x148) = 0x3f800000;
                FUN_1401f9440(&local_2b8);
              }
              FUN_1402619f0(plVar15[0x12],local_1d8);
              local_290 = local_1ac - *(float *)(lVar26 + 0x80);
              local_294 = local_1bc - *(float *)(lVar26 + 0x7c);
              local_298 = local_1cc - *(float *)(lVar26 + 0x78);
              FUN_140139800(&local_298);
              fVar8 = local_290;
              fVar7 = local_294;
              fVar6 = local_298;
              local_2b8 = (ulonglong ******)
                          CONCAT44((uint)(local_290 * 0.0 - local_298) ^ DAT_140800860,
                                   local_290 * 0.0 - local_294);
              pppppuStack_2b0 =
                   (ulonglong *****)
                   CONCAT44(pppppuStack_2b0._4_4_,local_294 * 0.0 - local_298 * 0.0);
              FUN_140139800(&local_2b8);
              fVar3 = local_2b8._4_4_;
              fVar5 = pppppuStack_2b0._0_4_;
              fVar2 = (float)local_2b8;
              local_298 = fVar7 * pppppuStack_2b0._0_4_ - local_2b8._4_4_ * fVar8;
              local_294 = (float)((uint)(fVar6 * pppppuStack_2b0._0_4_ - (float)local_2b8 * fVar8) ^
                                 DAT_140800860);
              local_290 = fVar6 * local_2b8._4_4_ - (float)local_2b8 * fVar7;
              FUN_140139800(&local_298);
              local_1fc = *(undefined4 *)(lVar26 + 0x80);
              local_20c = *(undefined4 *)(lVar26 + 0x7c);
              local_21c = *(undefined4 *)(lVar26 + 0x78);
              local_224 = local_298;
              local_214 = (uint)local_294;
              local_204 = local_290;
              local_228 = fVar2;
              local_220 = fVar6;
              local_218 = fVar3;
              local_210 = fVar7;
              local_208 = fVar5;
              local_200 = fVar8;
              plVar15 = *(longlong **)(param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8);
              (**(code **)(*plVar15 + 0x28))(plVar15,&local_228);
              *(undefined1 *)(param_1 + 0x2268) = 1;
            }
            goto LAB_1403e9992;
          }
        }
LAB_1403e998a:
        FUN_1403dd680(param_1);
      }
    }
    else if (*(char *)(param_1 + 0x2268) != '\0') goto LAB_1403e998a;
LAB_1403e9992:
    if (0xf < local_250) {
      uVar23 = local_250 + 1;
      pppppppuVar21 = (ulonglong *******)local_268[0];
      if (0xfff < uVar23) {
        pppppppuVar21 = (ulonglong *******)local_268[0][-1];
        uVar23 = local_250 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_268[0] + (-8 - (longlong)pppppppuVar21)))
        goto LAB_1403ea198;
      }
      thunk_FUN_1407864b8(pppppppuVar21,uVar23);
    }
    local_258 = 0;
    local_250 = 0xf;
    local_268[0] = (ulonglong ******)((ulonglong)local_268[0] & 0xffffffffffffff00);
    if (7 < uStack_2c0) {
      if (0xfff < uStack_2c0 * 2 + 2) {
        pppppppuVar21 = (ulonglong *******)local_2d8[-1];
        uVar23 = uStack_2c0 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)local_2d8 + (-8 - (longlong)pppppppuVar21))) {
LAB_1403ea198:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(pppppppuVar21,uVar23);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_1401f9440(&local_288);
  }
  uVar24 = 0;
  lVar26 = -1;
  uVar23 = uVar24;
  if (0 < *(int *)(param_1 + 0xde8)) {
    do {
      lVar17 = *(longlong *)(param_1 + 0xde0);
      plVar15 = *(longlong **)(uVar24 + 0x138 + lVar17);
      lVar1 = *plVar15;
      if (*(longlong *)(uVar24 + lVar17) == *(longlong *)(param_1 + 0x2150)) {
        (**(code **)(lVar1 + 0x1d0))(plVar15,3);
      }
      else {
        iVar12 = (**(code **)(lVar1 + 0x1d8))();
        if (iVar12 == 3) {
          plVar15 = *(longlong **)(uVar24 + 0x138 + lVar17);
          (**(code **)(*plVar15 + 0x1d0))(plVar15,1);
        }
      }
      uVar27 = (int)uVar23 + 1;
      uVar24 = uVar24 + 0x188;
      uVar23 = (ulonglong)uVar27;
    } while ((int)uVar27 < *(int *)(param_1 + 0xde8));
  }
  FUN_1403ed800(param_1);
  plVar15 = (longlong *)FUN_140347b50(0x4e);
  lVar17 = FUN_140294a40(&DAT_140a16fd0);
  _Buf2 = (void *)(param_1 + 0x2748);
  puVar18 = *(undefined8 **)(lVar17 + 0x68);
  if (0xf < *(ulonglong *)(param_1 + 0x2760)) {
    _Buf2 = *(void **)(param_1 + 0x2748);
  }
  puVar19 = puVar18;
  if (0xf < (ulonglong)puVar18[3]) {
    puVar19 = (undefined8 *)*puVar18;
  }
  if ((puVar18[2] == *(size_t *)(param_1 + 0x2758)) &&
     (iVar12 = memcmp(puVar19,_Buf2,puVar18[2]), iVar12 == 0)) {
    (**(code **)(*plVar15 + 0x148))(plVar15,0,1,0);
    (**(code **)(*plVar15 + 0x78))(plVar15,1);
  }
  else {
    uVar20 = FUN_140294a40(&DAT_140a16fd0);
    uVar13 = FUN_1402813c0(uVar20);
    puVar18 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,"TEXT_TECH");
    if (7 < (ulonglong)puVar18[3]) {
      puVar18 = (undefined8 *)*puVar18;
    }
    FUN_140022b30(local_1f8,0x10,L"%s %d",puVar18,uVar13);
    local_2d8 = (ulonglong ******)0x0;
    lVar17 = -1;
    local_2c8 = 0;
    uStack_2c0 = 7;
    do {
      lVar17 = lVar17 + 1;
    } while (local_1f8[lVar17] != 0);
    FUN_1400228a0(&local_2d8,local_1f8);
    (**(code **)(*plVar15 + 0x148))(plVar15,&local_2d8,1,0);
    (**(code **)(*plVar15 + 0x78))(plVar15,0);
    if (7 < uStack_2c0) {
      if (0xfff < uStack_2c0 * 2 + 2) {
        if (0x1f < (ulonglong)((longlong)local_2d8 + (-8 - (longlong)local_2d8[-1]))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(local_2d8[-1],uStack_2c0 * 2 + 0x29);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  plVar15 = DAT_140b15418;
  if ((DAT_140b15418 != (longlong *)0x0) &&
     (iVar12 = (**(code **)(*DAT_140b15418 + 0xe0))(DAT_140b15418), iVar12 == 0)) {
    puVar18 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,"TEXT_CYCLES");
    if (7 < (ulonglong)puVar18[3]) {
      puVar18 = (undefined8 *)*puVar18;
    }
    FUN_140022b30(&local_248,0x10,L"%s: %d",puVar18,(int)plVar15[0x6e]);
    local_288 = (undefined8 *)0x0;
    lVar17 = -1;
    local_278 = 0;
    local_270 = 7;
    do {
      lVar17 = lVar17 + 1;
    } while (*(short *)((longlong)&local_248 + lVar17 * 2) != 0);
    FUN_1400228a0(&local_288,&local_248);
    uVar23 = local_278;
    puVar18 = local_288;
    puVar19 = &local_288;
    if (7 < local_270) {
      puVar19 = local_288;
    }
    local_2d8 = (ulonglong ******)0x0;
    local_2c8 = 0;
    uStack_2c0 = 0;
    if (local_278 < 8) {
      local_2d8 = (ulonglong ******)*puVar19;
      pppppuStack_2d0 = (ulonglong *****)puVar19[1];
      uStack_2c0 = 7;
    }
    else {
      uVar24 = local_278 | 7;
      if (0x7ffffffffffffffe < uVar24) {
        uVar24 = 0x7ffffffffffffffe;
      }
      local_2d8 = (ulonglong ******)FUN_1400226c0(&local_2d8,uVar24 + 1);
      FUN_14076bd80(local_2d8,puVar19,uVar23 * 2 + 2);
      uStack_2c0 = uVar24;
    }
    local_2c8 = uVar23;
    FUN_140347c30(0x3c,&local_2d8);
    local_2d8 = (ulonglong ******)0x0;
    puVar19 = &local_288;
    if (7 < local_270) {
      puVar19 = puVar18;
    }
    local_2c8 = 0;
    uStack_2c0 = 0;
    if (uVar23 < 8) {
      local_2d8 = (ulonglong ******)*puVar19;
      pppppuStack_2d0 = (ulonglong *****)puVar19[1];
      uStack_2c0 = 7;
    }
    else {
      uVar24 = uVar23 | 7;
      if (0x7ffffffffffffffe < uVar24) {
        uVar24 = 0x7ffffffffffffffe;
      }
      local_2d8 = (ulonglong ******)FUN_1400226c0(&local_2d8,uVar24 + 1);
      FUN_14076bd80(local_2d8,puVar19,uVar23 * 2 + 2);
      uStack_2c0 = uVar24;
    }
    local_2c8 = uVar23;
    FUN_140347c30(0x2bd,&local_2d8);
    uVar20 = FUN_140294a40(&DAT_140a16fd0);
    FUN_1404b16f0(plVar15,uVar20,0);
    uVar13 = FUN_14022f9c0();
    FUN_140022b30(&local_248,0x10,L"$ %d",uVar13);
    local_268[0] = (ulonglong ******)0x0;
    lVar17 = -1;
    local_258 = 0;
    local_250 = 7;
    do {
      lVar17 = lVar17 + 1;
    } while (*(short *)((longlong)&local_248 + lVar17 * 2) != 0);
    FUN_1400228a0(local_268,&local_248);
    uVar23 = local_258;
    ppppppuVar9 = local_268[0];
    pppppppuVar21 = local_268;
    if (7 < local_250) {
      pppppppuVar21 = (ulonglong *******)local_268[0];
    }
    local_2d8 = (ulonglong ******)0x0;
    local_2c8 = 0;
    uStack_2c0 = 0;
    if (local_258 < 8) {
      local_2d8 = *pppppppuVar21;
      pppppuStack_2d0 = (ulonglong *****)pppppppuVar21[1];
      uStack_2c0 = 7;
    }
    else {
      uVar24 = local_258 | 7;
      if (0x7ffffffffffffffe < uVar24) {
        uVar24 = 0x7ffffffffffffffe;
      }
      local_2d8 = (ulonglong ******)FUN_1400226c0(&local_2d8,uVar24 + 1);
      FUN_14076bd80(local_2d8,pppppppuVar21,uVar23 * 2 + 2);
      uStack_2c0 = uVar24;
    }
    local_2c8 = uVar23;
    uVar20 = FUN_140347c30(0x3d,&local_2d8);
    if (7 < local_250) {
      uVar23 = local_250 * 2 + 2;
      pppppppuVar21 = (ulonglong *******)ppppppuVar9;
      if (0xfff < uVar23) {
        pppppppuVar21 = (ulonglong *******)ppppppuVar9[-1];
        uVar23 = local_250 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)ppppppuVar9 + (-8 - (longlong)pppppppuVar21)))
        goto LAB_1403ea1a4;
      }
      uVar20 = thunk_FUN_1407864b8(pppppppuVar21,uVar23);
    }
    if (7 < local_270) {
      puVar19 = puVar18;
      if (0xfff < local_270 * 2 + 2) {
        puVar19 = (undefined8 *)puVar18[-1];
        uVar23 = local_270 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)puVar18 + (-8 - (longlong)puVar19))) {
LAB_1403ea1a4:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(uVar20,uVar23);
        }
      }
      thunk_FUN_1407864b8(puVar19);
    }
  }
  uVar13 = FUN_140294a70(&DAT_140a16fd0);
  uVar14 = FUN_1402ac700(DAT_140b15418[3],uVar13);
  uVar13 = FUN_1402abb20(DAT_140b15418[3],uVar13,1);
  FUN_140022b30(local_1f8,0x10,L"%d / %d",uVar13,uVar14);
  local_268[0] = (ulonglong ******)0x0;
  local_258 = 0;
  local_250 = 7;
  do {
    lVar26 = lVar26 + 1;
  } while (local_1f8[lVar26] != 0);
  FUN_1400228a0(local_268,local_1f8,lVar26);
  uVar24 = local_250;
  uVar23 = local_258;
  ppppppuVar9 = local_268[0];
  local_2d8 = (ulonglong ******)0x0;
  pppppppuVar21 = local_268;
  if (7 < local_250) {
    pppppppuVar21 = (ulonglong *******)local_268[0];
  }
  local_2c8 = 0;
  uStack_2c0 = 0;
  if (local_258 < 8) {
    local_2d8 = *pppppppuVar21;
    pppppuStack_2d0 = (ulonglong *****)pppppppuVar21[1];
    uStack_2c0 = 7;
  }
  else {
    uVar25 = local_258 | 7;
    if (0x7ffffffffffffffe < uVar25) {
      uVar25 = 0x7ffffffffffffffe;
    }
    local_2d8 = (ulonglong ******)FUN_1400226c0(&local_2d8,uVar25 + 1);
    FUN_14076bd80(local_2d8,pppppppuVar21,uVar23 * 2 + 2);
    uStack_2c0 = uVar25;
  }
  local_2c8 = uVar23;
  FUN_140347c30(0x3e,&local_2d8);
  plVar15 = (longlong *)FUN_140347b50(0x43);
  uVar20 = extraout_XMM0_Qa;
  if ((plVar15 != (longlong *)0x0) &&
     (uVar20 = (**(code **)(*plVar15 + 0x80))(plVar15,DAT_140a2a4d8 == 0),
     *(char *)(param_1 + 0x26b1) != '\0')) {
    plVar16 = (longlong *)(param_1 + 0x26b8);
    if (0xf < *(ulonglong *)(param_1 + 0x26d0)) {
      plVar16 = *(longlong **)(param_1 + 0x26b8);
    }
    if (((*(longlong *)(param_1 + 0x26c8) != 10) || (*plVar16 != 0x6f4c5f746867694c)) ||
       ((short)plVar16[1] != 0x706f)) {
      uVar20 = (**(code **)(*plVar15 + 0x80))(plVar15,1);
    }
  }
  if (7 < uVar24) {
    lVar26 = uVar24 * 2;
    pppppppuVar21 = (ulonglong *******)ppppppuVar9;
    if (0xfff < lVar26 + 2U) {
      pppppppuVar21 = (ulonglong *******)ppppppuVar9[-1];
      if (0x1f < (ulonglong)((longlong)ppppppuVar9 + (-8 - (longlong)pppppppuVar21))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar20,lVar26 + 0x29);
      }
    }
    thunk_FUN_1407864b8(pppppppuVar21);
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e83a0  Name: FUN_1403e83a0  Size: 1256 bytes
// ------------------------------------------------------------

void FUN_1403e83a0(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  longlong *plVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong lVar8;
  longlong lVar9;
  ulonglong uVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  uint uVar13;
  int iVar14;
  longlong local_48 [2];
  undefined8 local_38;
  ulonglong local_30;
  
  if ((*(char *)(param_1 + 0x26a4) == '\0') || (*(char *)(param_1 + 0x2a34) != '\0')) {
    FUN_140347bf0(0x5a,1);
    return;
  }
  if (*(int *)(DAT_140b2eea0 + 0x44) == 3) {
    plVar5 = (longlong *)FUN_140347b50();
    FUN_140347bf0(0x5a);
    (**(code **)(*plVar5 + 0x78))(plVar5,0);
    if (*(char *)(*(longlong *)(*(longlong *)(param_1 + 0x2150) + 0xb8) + 0x2c8) == '\0') {
      iVar4 = *(int *)(*(longlong *)(*(longlong *)(param_1 + 0x2150) + 0x298) + 0x1988);
    }
    else {
      iVar4 = 8;
    }
    (**(code **)(*plVar5 + 0x300))(plVar5,iVar4);
    plVar5 = *(longlong **)(param_1 + 0x2150);
    if (plVar5 != (longlong *)0x0) {
      uVar6 = (**(code **)(*plVar5 + 0x10))(plVar5,3);
      uVar12 = 0;
      uVar11 = uVar12;
      do {
        plVar5 = (longlong *)FUN_140347b50((int)uVar11 + 0x271);
        if (plVar5 != (longlong *)0x0) {
          local_48[0] = 0;
          local_38 = 0;
          local_30 = 0xf;
          FUN_140200cd0(local_48,"TEXT_FLEET_PANE_%d",uVar11);
          lVar8 = *plVar5;
          uVar7 = FUN_1401fa680(&DAT_140a7bc58,local_48,1);
          (**(code **)(lVar8 + 0x148))(plVar5,uVar7,1,0);
          (**(code **)(*plVar5 + 0x78))(plVar5,0);
          (**(code **)(*plVar5 + 0x1a0))(plVar5,1);
          if (0xf < local_30) {
            if (0xfff < local_30 + 1) {
              if (0x1f < (local_48[0] - *(longlong *)(local_48[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        uVar13 = (int)uVar11 + 1;
        uVar11 = (ulonglong)uVar13;
      } while ((int)uVar13 < 3);
      plVar5 = (longlong *)FUN_140347b50(0x277);
      lVar8 = *plVar5;
      if (iVar4 == 8) {
        (**(code **)(lVar8 + 0x78))(plVar5,1);
      }
      else {
        uVar7 = FUN_1401fa460(&DAT_140a7bc58,"TEXT_BASE_LAYOUT");
        (**(code **)(lVar8 + 0x148))(plVar5,uVar7,1,0);
        (**(code **)(*plVar5 + 0x78))(plVar5,0);
        cVar2 = FUN_14050ba40(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
        lVar8 = *plVar5;
        if (cVar2 == '\0') {
          uVar7 = FUN_1401fa460(&DAT_140a7bc58,"TEXT_UNKNOWN");
        }
        else {
          uVar7 = 0;
        }
        (**(code **)(lVar8 + 0x148))(plVar5,uVar7,0,0);
      }
      FUN_1403e3a20(param_1);
      lVar8 = FUN_140395900(*(undefined8 *)(param_1 + 0x2150));
      uVar7 = FUN_140294a40(&DAT_140a16fd0);
      uVar11 = uVar12;
      uVar10 = uVar12;
      if (0 < *(int *)(lVar8 + 0x10)) {
        do {
          lVar9 = *(longlong *)(uVar11 + *(longlong *)(lVar8 + 8));
          if ((lVar9 != 0) && (lVar1 = *(longlong *)(param_1 + 0x2150), lVar1 != 0)) {
            uVar3 = FUN_140294a70(&DAT_140a16fd0);
            cVar2 = FUN_14039a230(lVar9,uVar3,0);
            if (cVar2 == '\0') {
              cVar2 = FUN_14050b5a0(lVar1,0xffffffff);
              if ((cVar2 != '\x01') || (cVar2 = FUN_1403a9760(lVar9), cVar2 != '\0'))
              goto LAB_1403e8698;
            }
            lVar9 = *(longlong *)(uVar11 + *(longlong *)(lVar8 + 8));
            if ((*(longlong *)(lVar9 + 0xc0) == 0) ||
               (*(int *)(*(longlong *)(lVar9 + 0xc0) + 0x18) != 0)) {
              cVar2 = FUN_14039a2c0(lVar9,uVar7);
              if (cVar2 == '\0') goto LAB_1403e86e0;
            }
            else {
              FUN_140391310(lVar9,1);
            }
          }
LAB_1403e8698:
          uVar13 = (int)uVar10 + 1;
          uVar11 = uVar11 + 8;
          uVar10 = (ulonglong)uVar13;
        } while ((int)uVar13 < *(int *)(lVar8 + 0x10));
      }
      lVar8 = FUN_1403f9270(uVar6,*(undefined8 *)(param_1 + 0x2150));
      lVar9 = FUN_140294a40(&DAT_140a16fd0);
      if (lVar8 == lVar9) {
        FUN_1403e2cf0(param_1);
      }
      else {
LAB_1403e86e0:
        FUN_1403e1b70(param_1);
      }
      iVar4 = FUN_1403f8b30(uVar6,*(undefined8 *)(param_1 + 0x2150));
      cVar2 = FUN_14050c420(*(undefined8 *)(param_1 + 0x2150),0xffffffff);
      if ((cVar2 == '\x01') && (0 < iVar4)) {
        iVar14 = iVar4 + -1;
        lVar8 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x58)
                             );
        uVar11 = uVar12;
        if (lVar8 != 0) {
          uVar11 = *(ulonglong *)(lVar8 + 0x68);
        }
        uVar10 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2708);
        if (uVar11 != uVar10) {
          lVar8 = FUN_140294bc0(&DAT_140a16fd0,
                                *(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x58));
          uVar11 = uVar12;
          if (lVar8 != 0) {
            uVar11 = *(ulonglong *)(lVar8 + 0x68);
          }
          uVar10 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2728);
          if (uVar11 == uVar10) {
            iVar14 = iVar4 + 4;
          }
          else {
            lVar8 = FUN_140294bc0(&DAT_140a16fd0,
                                  *(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x58));
            if (lVar8 != 0) {
              uVar12 = *(ulonglong *)(lVar8 + 0x68);
            }
            uVar11 = FUN_1403303d0(&DAT_140a172d0,param_1 + 0x2748);
            if (uVar12 == uVar11) {
              iVar14 = iVar4 + 9;
            }
            else {
              iVar4 = 0;
              if (iVar14 < 1) {
                iVar4 = iVar14;
              }
              iVar14 = iVar4 + 0xf;
            }
          }
        }
        FUN_1403c9fc0(param_1,iVar14);
      }
      plVar5 = (longlong *)FUN_140347b50(0x5c);
      (**(code **)(*plVar5 + 0xa0))(plVar5,0,0);
      (**(code **)(*plVar5 + 0x80))(plVar5,1);
      FUN_1403e8890(param_1);
    }
  }
  else {
    FUN_140347bf0(0x5a,1);
    if (*(char *)(param_1 + 0x10c0) != '\0') {
      FUN_1403dd790(param_1);
      return;
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3145c0  Name: FUN_1403145c0  Size: 5748 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x000140315bbd) */
/* WARNING: Removing unreachable block (ram,0x000140315b59) */
/* WARNING: Removing unreachable block (ram,0x000140315b27) */
/* WARNING: Removing unreachable block (ram,0x000140315b8b) */
/* WARNING: Removing unreachable block (ram,0x000140315bed) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403145c0(longlong param_1)

{
  bool bVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  int iVar16;
  undefined4 uVar17;
  char cVar18;
  char cVar19;
  char cVar20;
  longlong *plVar21;
  longlong lVar22;
  HANDLE pvVar23;
  longlong lVar24;
  undefined8 *puVar25;
  longlong *plVar26;
  LPVOID pvVar27;
  undefined8 uVar28;
  undefined8 *puVar29;
  longlong *plVar30;
  ulonglong uVar31;
  ulonglong uVar32;
  uint uVar33;
  uint uVar34;
  int iVar35;
  ulonglong uVar36;
  char cVar37;
  undefined1 uVar38;
  uint uVar39;
  int iVar40;
  bool bVar41;
  uint local_res8;
  uint local_res10;
  uint local_res18;
  uint local_res20;
  uint in_stack_ffffffffffffff08;
  int *in_stack_ffffffffffffff10;
  undefined4 uVar42;
  longlong *local_d8;
  uint local_d0;
  uint local_cc;
  uint local_c8;
  longlong *local_c0;
  longlong *local_b8;
  uint local_b0;
  uint local_ac;
  undefined8 *local_a8;
  undefined8 *local_a0;
  uint local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  undefined8 local_78;
  undefined1 local_70 [48];
  
  if (*(int *)(param_1 + 0x211c) == 1) {
    if (*(longlong *)(param_1 + 0x2150) == 0) {
      iVar35 = 10;
      if (9 < DAT_140b279f4 + 9) {
        do {
          plVar21 = (longlong *)FUN_140347b50(iVar35);
          if (plVar21 != (longlong *)0x0) {
            (**(code **)(*plVar21 + 0xa0))(plVar21,0,0);
            FUN_140353ad0(plVar21,0xffffffffffffffff,0);
            (**(code **)(*plVar21 + 0x78))(plVar21,1);
            (**(code **)(*plVar21 + 0x80))(plVar21,1);
            (**(code **)(*plVar21 + 0x148))(plVar21,0,1,0);
            (**(code **)(*plVar21 + 0x148))(plVar21,0,0,0);
            (**(code **)(*plVar21 + 0xb0))(plVar21,0);
            FUN_1401f93e0(local_70,0x80,0x80,0x80,0xff);
            (**(code **)(*plVar21 + 0x10))(plVar21);
            FUN_1401f9440(local_70);
          }
          iVar35 = iVar35 + 1;
        } while (iVar35 <= DAT_140b279f4 + 9);
      }
      plVar21 = (longlong *)FUN_140347b50(0x49);
      (**(code **)(*plVar21 + 0x78))(plVar21,1);
      plVar21 = (longlong *)FUN_140347b50(0x48);
      (**(code **)(*plVar21 + 0x78))(plVar21,1);
      FUN_1403e6550(param_1,0);
    }
    else {
      local_78 = FUN_140294a40(&DAT_140a16fd0);
      local_88 = 0xb;
      local_84 = 10;
      local_8c = 10;
      plVar21 = (longlong *)0x0;
      local_90 = 0xb;
      local_c0 = (longlong *)0x0;
      local_res8 = 0;
      local_res20 = 0;
      local_a0 = (undefined8 *)0x0;
      local_b0 = 0;
      local_c8 = 0;
      local_b8 = (longlong *)0x0;
      local_res10 = 0;
      local_cc = 0;
      local_a8 = (undefined8 *)0x0;
      local_98 = 0;
      local_d0 = 0;
      local_d8 = (longlong *)0x0;
      local_ac = 0;
      local_res18 = 0;
      iVar35 = FUN_140294a70(&DAT_140a16fd0);
      lVar22 = FUN_1403956c0(*(undefined8 *)(param_1 + 0x2150));
      if (iVar35 == *(int *)(lVar22 + 4)) {
        FUN_1403e6550(param_1,1);
        local_7c = FUN_140408150(&DAT_140b2f020);
        local_94 = 0;
        uVar34 = 0;
        if (0 < local_7c) {
          do {
            uVar34 = (uint)plVar21;
            lVar22 = FUN_140408130(&DAT_140b2f020,local_94);
            uVar28 = local_78;
            in_stack_ffffffffffffff10 = &local_80;
            in_stack_ffffffffffffff08 = in_stack_ffffffffffffff08 & 0xffffff00;
            cVar18 = FUN_1402804d0(local_78,lVar22,*(undefined8 *)(param_1 + 0x2150),0,
                                   in_stack_ffffffffffffff08,in_stack_ffffffffffffff10,local_70);
            if (*(char *)(lVar22 + 0x4c) == '\0') {
              if (*(char *)(lVar22 + 0x4d) == '\0') {
                if (*(char *)(lVar22 + 0x4f) == '\0') {
                  if (*(char *)(lVar22 + 0x4e) == '\0') {
                    if (*(char *)(lVar22 + 0x50) == '\0') {
                      iVar35 = 7 - (uint)(*(char *)(lVar22 + 0x51) != '\0');
                    }
                    else {
                      iVar35 = 5;
                    }
                  }
                  else {
                    iVar35 = 1;
                  }
                }
                else {
                  iVar35 = 0;
                }
              }
              else {
                iVar35 = 3;
              }
            }
            else {
              iVar35 = 2;
            }
            cVar19 = FUN_140280190(uVar28,lVar22,*(undefined8 *)(param_1 + 0x2150));
            if ((cVar18 == '\0') && (local_80 == 9)) {
              cVar18 = '\x01';
              uVar33 = (int)(local_res18 * 2) >> 1;
              uVar39 = local_ac;
              if ((int)uVar33 <= (int)local_ac) {
                iVar40 = (int)uVar33 / 10;
                if ((int)uVar33 / 10 < 10) {
                  iVar40 = 10;
                }
                uVar39 = uVar33 + iVar40;
                if (uVar39 == 0) {
                  uVar39 = local_ac;
                  if (local_d8 != (longlong *)0x0) {
                    if ((int)local_res18 < 0) {
                      pvVar23 = GetProcessHeap();
                      HeapFree(pvVar23,0,local_d8);
                    }
                    else {
                      thunk_FUN_1407864b8(local_d8);
                    }
                    local_res18 = 0;
                    local_d8 = (longlong *)0x0;
                    uVar39 = 0;
                  }
                }
                else {
                  uVar36 = (ulonglong)(int)uVar39;
                  if (local_d8 == (longlong *)0x0) {
                    if ((int)local_res18 < 0) {
                      pvVar23 = GetProcessHeap();
                      local_d8 = HeapAlloc(pvVar23,0,uVar36 * 8);
                    }
                    else {
                      auVar2._8_8_ = 0;
                      auVar2._0_8_ = uVar36;
                      uVar28 = SUB168(ZEXT816(8) * auVar2,0);
                      if (SUB168(ZEXT816(8) * auVar2,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      local_d8 = (longlong *)thunk_FUN_140769c58(uVar28);
                    }
                    local_res18 = local_res18 ^ (local_res18 ^ uVar39) & 0x7fffffff;
                    uVar39 = 0;
                  }
                  else {
                    if ((int)local_res18 < 0) {
                      pvVar23 = GetProcessHeap();
                      plVar21 = HeapReAlloc(pvVar23,0,local_d8,uVar36 * 8);
                    }
                    else {
                      auVar3._8_8_ = 0;
                      auVar3._0_8_ = uVar36;
                      uVar28 = SUB168(ZEXT816(8) * auVar3,0);
                      if (SUB168(ZEXT816(8) * auVar3,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      plVar21 = (longlong *)thunk_FUN_140769c58(uVar28);
                      if ((int)uVar39 < (int)uVar33) {
                        uVar33 = uVar39;
                      }
                      lVar24 = (longlong)(int)uVar33;
                      if (0 < (int)uVar33) {
                        plVar26 = plVar21;
                        do {
                          *plVar26 = *(longlong *)
                                      (((longlong)local_d8 - (longlong)plVar21) + (longlong)plVar26)
                          ;
                          plVar26 = plVar26 + 1;
                          lVar24 = lVar24 + -1;
                        } while (lVar24 != 0);
                      }
                      thunk_FUN_1407864b8(local_d8);
                      uVar34 = local_b0;
                    }
                    local_res18 = local_res18 ^ (local_res18 ^ uVar39) & 0x7fffffff;
                    local_d8 = plVar21;
                    if ((int)local_ac < (int)uVar39) {
                      uVar39 = local_ac;
                    }
                  }
                }
              }
              local_ac = uVar39 + 1;
              local_d8[(int)uVar39] = lVar22;
            }
            cVar37 = '\x01';
            if ((0 < *(int *)(lVar22 + 0x86c)) &&
               ((*(int *)(lVar22 + 0xf0c) != 0 ||
                (cVar20 = FUN_140370320(lVar22,0x5f,0xffffffff), cVar20 != '\0')))) {
              cVar37 = '\0';
            }
            if ((iVar35 - 5U < 2) && (cVar18 == '\0')) {
              cVar19 = '\0';
            }
            iVar40 = *(int *)(lVar22 + 0x1b40);
            if ((iVar35 != 7) && (cVar37 == *(char *)(param_1 + 0x21ac))) {
              if (iVar40 == -1) {
                if ((cVar18 == '\0') && (cVar19 == '\0')) goto LAB_140315301;
                plVar21 = (longlong *)(lVar22 + 0xf8);
                if (0xf < *(ulonglong *)(lVar22 + 0x110)) {
                  plVar21 = (longlong *)*plVar21;
                }
                FUN_140025760("Warning!  %s does not have GUI_Row set.\n",plVar21);
              }
              if (cVar18 == '\0') {
                if (cVar19 != '\0') {
                  if (iVar40 == 0) {
                    uVar33 = (int)(local_c8 * 2) >> 1;
                    if ((int)uVar33 <= (int)uVar34) {
                      iVar35 = (int)uVar33 / 10;
                      if ((int)uVar33 / 10 < 10) {
                        iVar35 = 10;
                      }
                      uVar39 = iVar35 + uVar33;
                      if (uVar39 == 0) {
                        if (local_a0 != (undefined8 *)0x0) {
                          if ((int)local_c8 < 0) {
                            pvVar23 = GetProcessHeap();
                            HeapFree(pvVar23,0,local_a0);
                          }
                          else {
                            thunk_FUN_1407864b8(local_a0);
                          }
                          local_c8 = 0;
                          local_a0 = (undefined8 *)0x0;
                          uVar34 = 0;
                        }
                      }
                      else {
                        uVar36 = (ulonglong)(int)uVar39;
                        if (local_a0 == (undefined8 *)0x0) {
                          if ((int)local_c8 < 0) {
                            pvVar23 = GetProcessHeap();
                            local_a0 = HeapAlloc(pvVar23,0,uVar36 * 8);
                          }
                          else {
                            auVar10._8_8_ = 0;
                            auVar10._0_8_ = uVar36;
                            uVar28 = SUB168(ZEXT816(8) * auVar10,0);
                            if (SUB168(ZEXT816(8) * auVar10,8) != 0) {
                              uVar28 = 0xffffffffffffffff;
                            }
                            local_a0 = (undefined8 *)thunk_FUN_140769c58(uVar28);
                          }
                          local_c8 = local_c8 ^ (uVar39 ^ local_c8) & 0x7fffffff;
                          uVar34 = 0;
                        }
                        else {
                          if ((int)local_c8 < 0) {
                            pvVar23 = GetProcessHeap();
                            puVar25 = HeapReAlloc(pvVar23,0,local_a0,uVar36 * 8);
                            local_b0 = uVar34;
                          }
                          else {
                            auVar11._8_8_ = 0;
                            auVar11._0_8_ = uVar36;
                            uVar28 = SUB168(ZEXT816(8) * auVar11,0);
                            if (SUB168(ZEXT816(8) * auVar11,8) != 0) {
                              uVar28 = 0xffffffffffffffff;
                            }
                            puVar25 = (undefined8 *)thunk_FUN_140769c58(uVar28);
                            if ((int)uVar39 < (int)uVar33) {
                              uVar33 = uVar39;
                            }
                            lVar24 = (longlong)(int)uVar33;
                            if (0 < (int)uVar33) {
                              puVar29 = puVar25;
                              do {
                                *puVar29 = *(undefined8 *)
                                            (((longlong)local_a0 - (longlong)puVar25) +
                                            (longlong)puVar29);
                                puVar29 = puVar29 + 1;
                                lVar24 = lVar24 + -1;
                              } while (lVar24 != 0);
                            }
                            thunk_FUN_1407864b8(local_a0);
                          }
                          local_c8 = local_c8 ^ (uVar39 ^ local_c8) & 0x7fffffff;
                          uVar34 = uVar39;
                          local_a0 = puVar25;
                          if ((int)local_b0 < (int)uVar39) {
                            uVar34 = local_b0;
                          }
                        }
                      }
                    }
                    lVar24 = (longlong)(int)uVar34;
                    local_b0 = uVar34 + 1;
                    puVar25 = local_a0;
                  }
                  else {
                    uVar34 = (int)(local_d0 * 2) >> 1;
                    if ((int)local_98 < (int)uVar34) {
LAB_140315134:
                      lVar24 = (longlong)(int)local_98;
                      local_98 = local_98 + 1;
                      puVar25 = local_a8;
                    }
                    else {
                      iVar35 = (int)uVar34 / 10;
                      if ((int)uVar34 / 10 < 10) {
                        iVar35 = 10;
                      }
                      uVar33 = uVar34 + iVar35;
                      if (uVar33 == 0) {
                        if (local_a8 == (undefined8 *)0x0) goto LAB_140315134;
                        if ((int)local_d0 < 0) {
                          pvVar23 = GetProcessHeap();
                          HeapFree(pvVar23,0,local_a8);
                        }
                        else {
                          thunk_FUN_1407864b8(local_a8);
                        }
                        local_d0 = 0;
                        local_a8 = (undefined8 *)0x0;
                        local_98 = 1;
                        lVar24 = 0;
                        puVar25 = (undefined8 *)0x0;
                      }
                      else {
                        uVar36 = (ulonglong)(int)uVar33;
                        if (local_a8 == (undefined8 *)0x0) {
                          if ((int)local_d0 < 0) {
                            pvVar23 = GetProcessHeap();
                            puVar25 = HeapAlloc(pvVar23,0,uVar36 * 8);
                          }
                          else {
                            auVar8._8_8_ = 0;
                            auVar8._0_8_ = uVar36;
                            uVar28 = SUB168(ZEXT816(8) * auVar8,0);
                            if (SUB168(ZEXT816(8) * auVar8,8) != 0) {
                              uVar28 = 0xffffffffffffffff;
                            }
                            puVar25 = (undefined8 *)thunk_FUN_140769c58(uVar28);
                          }
                          local_d0 = local_d0 ^ (uVar33 ^ local_d0) & 0x7fffffff;
                          lVar24 = 0;
                          local_98 = 1;
                          local_a8 = puVar25;
                        }
                        else {
                          if ((int)local_d0 < 0) {
                            pvVar23 = GetProcessHeap();
                            puVar25 = HeapReAlloc(pvVar23,0,local_a8,uVar36 * 8);
                          }
                          else {
                            auVar9._8_8_ = 0;
                            auVar9._0_8_ = uVar36;
                            uVar28 = SUB168(ZEXT816(8) * auVar9,0);
                            if (SUB168(ZEXT816(8) * auVar9,8) != 0) {
                              uVar28 = 0xffffffffffffffff;
                            }
                            puVar25 = (undefined8 *)thunk_FUN_140769c58(uVar28);
                            if ((int)uVar33 < (int)uVar34) {
                              uVar34 = uVar33;
                            }
                            lVar24 = (longlong)(int)uVar34;
                            if (0 < (int)uVar34) {
                              puVar29 = puVar25;
                              do {
                                *puVar29 = *(undefined8 *)
                                            (((longlong)local_a8 - (longlong)puVar25) +
                                            (longlong)puVar29);
                                puVar29 = puVar29 + 1;
                                lVar24 = lVar24 + -1;
                              } while (lVar24 != 0);
                            }
                            thunk_FUN_1407864b8(local_a8);
                          }
                          local_d0 = local_d0 ^ (uVar33 ^ local_d0) & 0x7fffffff;
                          if ((int)local_98 < (int)uVar33) {
                            uVar33 = local_98;
                          }
                          lVar24 = (longlong)(int)uVar33;
                          local_98 = uVar33 + 1;
                          local_a8 = puVar25;
                        }
                      }
                    }
                  }
                  puVar25[lVar24] = lVar22;
                }
              }
              else if (iVar40 == 0) {
                uVar33 = (int)(local_res20 * 2) >> 1;
                uVar34 = local_res8;
                if ((int)uVar33 <= (int)local_res8) {
                  iVar35 = (int)uVar33 / 10;
                  if ((int)uVar33 / 10 < 10) {
                    iVar35 = 10;
                  }
                  uVar34 = uVar33 + iVar35;
                  if (uVar34 == 0) {
                    uVar34 = local_res8;
                    if (local_c0 != (longlong *)0x0) {
                      if ((int)local_res20 < 0) {
                        pvVar23 = GetProcessHeap();
                        HeapFree(pvVar23,0,local_c0);
                      }
                      else {
                        thunk_FUN_1407864b8(local_c0);
                      }
                      local_res20 = 0;
                      local_c0 = (longlong *)0x0;
                      uVar34 = 0;
                    }
                  }
                  else {
                    uVar36 = (ulonglong)(int)uVar34;
                    if (local_c0 == (longlong *)0x0) {
                      if ((int)local_res20 < 0) {
                        pvVar23 = GetProcessHeap();
                        local_c0 = HeapAlloc(pvVar23,0,uVar36 * 8);
                      }
                      else {
                        auVar6._8_8_ = 0;
                        auVar6._0_8_ = uVar36;
                        uVar28 = SUB168(ZEXT816(8) * auVar6,0);
                        if (SUB168(ZEXT816(8) * auVar6,8) != 0) {
                          uVar28 = 0xffffffffffffffff;
                        }
                        local_c0 = (longlong *)thunk_FUN_140769c58(uVar28);
                      }
                      local_res20 = local_res20 ^ (uVar34 ^ local_res20) & 0x7fffffff;
                      uVar34 = 0;
                    }
                    else {
                      if ((int)local_res20 < 0) {
                        pvVar23 = GetProcessHeap();
                        plVar21 = HeapReAlloc(pvVar23,0,local_c0,uVar36 * 8);
                      }
                      else {
                        auVar7._8_8_ = 0;
                        auVar7._0_8_ = uVar36;
                        uVar28 = SUB168(ZEXT816(8) * auVar7,0);
                        if (SUB168(ZEXT816(8) * auVar7,8) != 0) {
                          uVar28 = 0xffffffffffffffff;
                        }
                        plVar21 = (longlong *)thunk_FUN_140769c58(uVar28);
                        if ((int)uVar34 < (int)uVar33) {
                          uVar33 = uVar34;
                        }
                        lVar24 = (longlong)(int)uVar33;
                        if (0 < (int)uVar33) {
                          plVar26 = plVar21;
                          do {
                            *plVar26 = *(longlong *)
                                        (((longlong)local_c0 - (longlong)plVar21) +
                                        (longlong)plVar26);
                            plVar26 = plVar26 + 1;
                            lVar24 = lVar24 + -1;
                          } while (lVar24 != 0);
                        }
                        thunk_FUN_1407864b8(local_c0);
                      }
                      local_res20 = local_res20 ^ (uVar34 ^ local_res20) & 0x7fffffff;
                      local_c0 = plVar21;
                      if ((int)local_res8 < (int)uVar34) {
                        uVar34 = local_res8;
                      }
                    }
                  }
                }
                local_res8 = uVar34 + 1;
                local_c0[(int)uVar34] = lVar22;
              }
              else {
                uVar34 = (int)(local_cc * 2) >> 1;
                if ((int)local_res10 < (int)uVar34) {
LAB_140314d68:
                  lVar24 = (longlong)(int)local_res10;
                  local_res10 = local_res10 + 1;
                  local_b8[lVar24] = lVar22;
                }
                else {
                  iVar35 = (int)uVar34 / 10;
                  if ((int)uVar34 / 10 < 10) {
                    iVar35 = 10;
                  }
                  uVar33 = uVar34 + iVar35;
                  if (uVar33 == 0) {
                    if (local_b8 == (longlong *)0x0) goto LAB_140314d68;
                    if ((int)local_cc < 0) {
                      pvVar23 = GetProcessHeap();
                      HeapFree(pvVar23,0,local_b8);
                    }
                    else {
                      thunk_FUN_1407864b8(local_b8);
                    }
                    local_cc = 0;
                    local_b8 = (longlong *)0x0;
                    local_res10 = 1;
                    _DAT_00000000 = lVar22;
                  }
                  else {
                    uVar36 = (ulonglong)(int)uVar33;
                    if (local_b8 == (longlong *)0x0) {
                      if ((int)local_cc < 0) {
                        pvVar23 = GetProcessHeap();
                        local_b8 = HeapAlloc(pvVar23,0,uVar36 * 8);
                      }
                      else {
                        auVar4._8_8_ = 0;
                        auVar4._0_8_ = uVar36;
                        uVar28 = SUB168(ZEXT816(8) * auVar4,0);
                        if (SUB168(ZEXT816(8) * auVar4,8) != 0) {
                          uVar28 = 0xffffffffffffffff;
                        }
                        local_b8 = (longlong *)thunk_FUN_140769c58(uVar28);
                      }
                      local_cc = local_cc ^ (uVar33 ^ local_cc) & 0x7fffffff;
                      local_res10 = 1;
                      *local_b8 = lVar22;
                    }
                    else {
                      if ((int)local_cc < 0) {
                        pvVar23 = GetProcessHeap();
                        plVar21 = HeapReAlloc(pvVar23,0,local_b8,uVar36 * 8);
                      }
                      else {
                        auVar5._8_8_ = 0;
                        auVar5._0_8_ = uVar36;
                        uVar28 = SUB168(ZEXT816(8) * auVar5,0);
                        if (SUB168(ZEXT816(8) * auVar5,8) != 0) {
                          uVar28 = 0xffffffffffffffff;
                        }
                        plVar21 = (longlong *)thunk_FUN_140769c58(uVar28);
                        if ((int)uVar33 < (int)uVar34) {
                          uVar34 = uVar33;
                        }
                        lVar24 = (longlong)(int)uVar34;
                        if (0 < (int)uVar34) {
                          plVar26 = plVar21;
                          do {
                            *plVar26 = *(longlong *)
                                        (((longlong)local_b8 - (longlong)plVar21) +
                                        (longlong)plVar26);
                            plVar26 = plVar26 + 1;
                            lVar24 = lVar24 + -1;
                          } while (lVar24 != 0);
                        }
                        thunk_FUN_1407864b8(local_b8);
                      }
                      local_cc = local_cc ^ (uVar33 ^ local_cc) & 0x7fffffff;
                      if ((int)local_res10 < (int)uVar33) {
                        uVar33 = local_res10;
                      }
                      local_res10 = uVar33 + 1;
                      plVar21[(int)uVar33] = lVar22;
                      local_b8 = plVar21;
                    }
                  }
                }
              }
            }
LAB_140315301:
            plVar21 = (longlong *)(ulonglong)local_b0;
            local_94 = local_94 + 1;
            uVar34 = local_ac;
          } while (local_94 < local_7c);
        }
        uVar17 = DAT_1408007c0;
        if (*(char *)(param_1 + 0x21ac) == '\0') {
          iVar35 = *(int *)(param_1 + 0x21bc);
        }
        else {
          iVar35 = *(int *)(param_1 + 0x21b8);
        }
        bVar1 = iVar35 < DAT_140b26f78;
        bVar41 = *(int *)(param_1 + 0x1120) != local_res8 + local_res10;
        if (0 < (int)local_res8) {
          uVar36 = (ulonglong)local_res8;
          plVar21 = local_c0;
          do {
            uVar42 = (undefined4)((ulonglong)in_stack_ffffffffffffff10 >> 0x20);
            iVar35 = 0;
            uVar38 = 1;
            lVar22 = *plVar21;
            if ((local_d8 != (longlong *)0x0) && (plVar26 = local_d8, 0 < (int)uVar34)) {
              do {
                if (*plVar26 == lVar22) {
                  if (iVar35 != -1) goto LAB_1403153c3;
                  break;
                }
                iVar35 = iVar35 + 1;
                plVar26 = plVar26 + 1;
              } while (iVar35 < (int)uVar34);
            }
            cVar18 = FUN_140282510(local_78,lVar22);
            if (cVar18 != '\0') {
LAB_1403153c3:
              uVar38 = 0;
            }
            in_stack_ffffffffffffff10 = (int *)CONCAT44(uVar42,2);
            plVar26 = (longlong *)
                      FUN_140309af0(param_1,lVar22,&local_90,uVar38,DAT_140b279f4 + 9,
                                    in_stack_ffffffffffffff10,bVar1);
            if ((*(char *)(param_1 + 0x1128) == '\0') && (plVar26 != (longlong *)0x0)) {
              plVar30 = *(longlong **)(param_1 + 0x1118);
              if (plVar30 != (longlong *)0x0) {
                uVar31 = 0;
                uVar32 = uVar31;
                if (0 < *(int *)(param_1 + 0x1120)) {
                  do {
                    if (*plVar30 == lVar22) {
                      if ((int)uVar32 != -1) goto LAB_14031546e;
                      break;
                    }
                    uVar32 = (ulonglong)((int)uVar32 + 1);
                    uVar31 = uVar31 + 1;
                    plVar30 = plVar30 + 1;
                  } while ((longlong)uVar31 < (longlong)*(int *)(param_1 + 0x1120));
                }
              }
              (**(code **)(*plVar26 + 0x120))(plVar26,0,0,uVar17,3);
              bVar41 = true;
            }
LAB_14031546e:
            plVar21 = plVar21 + 1;
            uVar36 = uVar36 - 1;
          } while (uVar36 != 0);
          local_88 = local_90;
        }
        uVar28 = local_78;
        if (0 < (int)local_res10) {
          uVar36 = (ulonglong)local_res10;
          plVar21 = local_b8;
          do {
            uVar42 = (undefined4)((ulonglong)in_stack_ffffffffffffff10 >> 0x20);
            lVar22 = *plVar21;
            cVar18 = FUN_140282510(uVar28,lVar22);
            in_stack_ffffffffffffff10 = (int *)CONCAT44(uVar42,2);
            plVar26 = (longlong *)
                      FUN_140309af0(param_1,lVar22,&local_8c,cVar18 == '\0',DAT_140b279f4 + 9,
                                    in_stack_ffffffffffffff10,bVar1);
            if ((*(char *)(param_1 + 0x1128) == '\0') && (plVar26 != (longlong *)0x0)) {
              plVar30 = *(longlong **)(param_1 + 0x1118);
              if ((plVar30 != (longlong *)0x0) &&
                 (uVar32 = 0, uVar31 = 0, 0 < *(int *)(param_1 + 0x1120))) {
                do {
                  if (*plVar30 == lVar22) {
                    if ((int)uVar31 != -1) goto LAB_14031556b;
                    break;
                  }
                  uVar31 = (ulonglong)((int)uVar31 + 1);
                  uVar32 = uVar32 + 1;
                  plVar30 = plVar30 + 1;
                } while ((longlong)uVar32 < (longlong)*(int *)(param_1 + 0x1120));
              }
              (**(code **)(*plVar26 + 0x120))(plVar26,0,0,uVar17,3);
              bVar41 = true;
            }
LAB_14031556b:
            plVar21 = plVar21 + 1;
            uVar36 = uVar36 - 1;
          } while (uVar36 != 0);
          local_84 = local_8c;
        }
        if (bVar41) {
          pvVar27 = *(LPVOID *)(param_1 + 0x1118);
          if (pvVar27 != (LPVOID)0x0) {
            if ((*(uint *)(param_1 + 0x1124) & 0x80000000) == 0) {
              thunk_FUN_1407864b8(pvVar27);
            }
            else {
              pvVar23 = GetProcessHeap();
              HeapFree(pvVar23,0,pvVar27);
            }
            *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
            *(undefined8 *)(param_1 + 0x1118) = 0;
            *(undefined4 *)(param_1 + 0x1120) = 0;
          }
          uVar36 = 0;
          if (0 < (int)local_res8) {
            uVar32 = (ulonglong)local_res8;
            uVar34 = *(uint *)(param_1 + 0x1120);
            plVar21 = local_c0;
            do {
              iVar35 = *(int *)(param_1 + 0x1124);
              iVar40 = iVar35 * 2 >> 1;
              uVar31 = (ulonglong)uVar34;
              if (iVar40 <= (int)uVar34) {
                pvVar27 = *(LPVOID *)(param_1 + 0x1118);
                iVar16 = iVar40 / 10;
                if (iVar40 / 10 < 10) {
                  iVar16 = 10;
                }
                uVar34 = iVar40 + iVar16;
                if (uVar34 == 0) {
                  if (pvVar27 != (LPVOID)0x0) {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      HeapFree(pvVar23,0,pvVar27);
                    }
                    else {
                      thunk_FUN_1407864b8(pvVar27);
                    }
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    *(undefined8 *)(param_1 + 0x1118) = 0;
                    *(undefined4 *)(param_1 + 0x1120) = 0;
                    uVar31 = uVar36;
                  }
                }
                else {
                  uVar31 = (ulonglong)(int)uVar34;
                  if (pvVar27 == (LPVOID)0x0) {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      pvVar27 = HeapAlloc(pvVar23,0,uVar31 * 8);
                    }
                    else {
                      auVar12._8_8_ = 0;
                      auVar12._0_8_ = uVar31;
                      uVar28 = SUB168(ZEXT816(8) * auVar12,0);
                      if (SUB168(ZEXT816(8) * auVar12,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      pvVar27 = (LPVOID)thunk_FUN_140769c58(uVar28);
                    }
                    *(LPVOID *)(param_1 + 0x1118) = pvVar27;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    uVar31 = 0;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) | uVar34 & 0x7fffffff;
                    *(undefined4 *)(param_1 + 0x1120) = 0;
                  }
                  else {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      pvVar27 = HeapReAlloc(pvVar23,0,pvVar27,uVar31 * 8);
                    }
                    else {
                      auVar13._8_8_ = 0;
                      auVar13._0_8_ = uVar31;
                      uVar28 = SUB168(ZEXT816(8) * auVar13,0);
                      if (SUB168(ZEXT816(8) * auVar13,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      pvVar27 = (LPVOID)thunk_FUN_140769c58(uVar28);
                      uVar33 = *(int *)(param_1 + 0x1124) * 2 >> 1;
                      if ((int)uVar34 < (int)uVar33) {
                        uVar33 = uVar34;
                      }
                      uVar31 = uVar36;
                      if (0 < (int)uVar33) {
                        do {
                          *(undefined8 *)((longlong)pvVar27 + uVar31 * 8) =
                               *(undefined8 *)(*(longlong *)(param_1 + 0x1118) + uVar31 * 8);
                          uVar31 = uVar31 + 1;
                        } while ((longlong)uVar31 < (longlong)(int)uVar33);
                      }
                      thunk_FUN_1407864b8(*(undefined8 *)(param_1 + 0x1118));
                    }
                    *(LPVOID *)(param_1 + 0x1118) = pvVar27;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) | uVar34 & 0x7fffffff;
                    if ((int)*(uint *)(param_1 + 0x1120) < (int)uVar34) {
                      uVar34 = *(uint *)(param_1 + 0x1120);
                    }
                    uVar31 = (ulonglong)uVar34;
                    *(uint *)(param_1 + 0x1120) = uVar34;
                  }
                }
              }
              lVar22 = *plVar21;
              plVar21 = plVar21 + 1;
              *(longlong *)(*(longlong *)(param_1 + 0x1118) + (longlong)(int)uVar31 * 8) = lVar22;
              *(int *)(param_1 + 0x1120) = *(int *)(param_1 + 0x1120) + 1;
              uVar34 = *(uint *)(param_1 + 0x1120);
              uVar32 = uVar32 - 1;
            } while (uVar32 != 0);
          }
          if (0 < (int)local_res10) {
            uVar34 = *(uint *)(param_1 + 0x1120);
            uVar32 = (ulonglong)local_res10;
            plVar21 = local_b8;
            do {
              iVar35 = *(int *)(param_1 + 0x1124);
              iVar40 = iVar35 * 2 >> 1;
              uVar31 = (ulonglong)uVar34;
              if (iVar40 <= (int)uVar34) {
                pvVar27 = *(LPVOID *)(param_1 + 0x1118);
                iVar16 = iVar40 / 10;
                if (iVar40 / 10 < 10) {
                  iVar16 = 10;
                }
                uVar34 = iVar40 + iVar16;
                if (uVar34 == 0) {
                  if (pvVar27 != (LPVOID)0x0) {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      HeapFree(pvVar23,0,pvVar27);
                    }
                    else {
                      thunk_FUN_1407864b8(pvVar27);
                    }
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    *(undefined8 *)(param_1 + 0x1118) = 0;
                    *(undefined4 *)(param_1 + 0x1120) = 0;
                    uVar31 = uVar36;
                  }
                }
                else {
                  uVar31 = (ulonglong)(int)uVar34;
                  if (pvVar27 == (LPVOID)0x0) {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      pvVar27 = HeapAlloc(pvVar23,0,uVar31 * 8);
                    }
                    else {
                      auVar14._8_8_ = 0;
                      auVar14._0_8_ = uVar31;
                      uVar28 = SUB168(ZEXT816(8) * auVar14,0);
                      if (SUB168(ZEXT816(8) * auVar14,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      pvVar27 = (LPVOID)thunk_FUN_140769c58(uVar28);
                    }
                    *(LPVOID *)(param_1 + 0x1118) = pvVar27;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    uVar31 = 0;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) | uVar34 & 0x7fffffff;
                    *(undefined4 *)(param_1 + 0x1120) = 0;
                  }
                  else {
                    if (iVar35 < 0) {
                      pvVar23 = GetProcessHeap();
                      pvVar27 = HeapReAlloc(pvVar23,0,pvVar27,uVar31 * 8);
                    }
                    else {
                      auVar15._8_8_ = 0;
                      auVar15._0_8_ = uVar31;
                      uVar28 = SUB168(ZEXT816(8) * auVar15,0);
                      if (SUB168(ZEXT816(8) * auVar15,8) != 0) {
                        uVar28 = 0xffffffffffffffff;
                      }
                      pvVar27 = (LPVOID)thunk_FUN_140769c58(uVar28);
                      uVar33 = *(int *)(param_1 + 0x1124) * 2 >> 1;
                      if ((int)uVar34 < (int)uVar33) {
                        uVar33 = uVar34;
                      }
                      uVar31 = uVar36;
                      if (0 < (int)uVar33) {
                        do {
                          *(undefined8 *)((longlong)pvVar27 + uVar31 * 8) =
                               *(undefined8 *)(*(longlong *)(param_1 + 0x1118) + uVar31 * 8);
                          uVar31 = uVar31 + 1;
                        } while ((longlong)uVar31 < (longlong)(int)uVar33);
                      }
                      thunk_FUN_1407864b8(*(undefined8 *)(param_1 + 0x1118));
                    }
                    *(LPVOID *)(param_1 + 0x1118) = pvVar27;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) & 0x80000000;
                    *(uint *)(param_1 + 0x1124) = *(uint *)(param_1 + 0x1124) | uVar34 & 0x7fffffff;
                    if ((int)*(uint *)(param_1 + 0x1120) < (int)uVar34) {
                      uVar34 = *(uint *)(param_1 + 0x1120);
                    }
                    uVar31 = (ulonglong)uVar34;
                    *(uint *)(param_1 + 0x1120) = uVar34;
                  }
                }
              }
              lVar22 = *plVar21;
              plVar21 = plVar21 + 1;
              *(longlong *)(*(longlong *)(param_1 + 0x1118) + (longlong)(int)uVar31 * 8) = lVar22;
              *(int *)(param_1 + 0x1120) = *(int *)(param_1 + 0x1120) + 1;
              uVar34 = *(uint *)(param_1 + 0x1120);
              uVar32 = uVar32 - 1;
            } while (uVar32 != 0);
          }
        }
        *(undefined1 *)(param_1 + 0x1128) = 1;
      }
      else {
        FUN_1403e6550(param_1,0);
        local_c0 = plVar21;
      }
      iVar35 = local_88;
      if (local_88 <= DAT_140b279f4 + 9) {
        do {
          plVar21 = (longlong *)FUN_140347b50(iVar35);
          (**(code **)(*plVar21 + 0x80))(plVar21,1);
          (**(code **)(*plVar21 + 0x78))(plVar21,1);
          FUN_140353ad0(plVar21,0xffffffffffffffff,0);
          iVar35 = iVar35 + 2;
        } while (iVar35 <= DAT_140b279f4 + 9);
      }
      iVar35 = local_84;
      if (local_84 <= DAT_140b279f4 + 9) {
        do {
          plVar21 = (longlong *)FUN_140347b50(iVar35);
          (**(code **)(*plVar21 + 0x80))(plVar21,1);
          (**(code **)(*plVar21 + 0x78))(plVar21,1);
          FUN_140353ad0(plVar21,0xffffffffffffffff,0);
          iVar35 = iVar35 + 2;
        } while (iVar35 <= DAT_140b279f4 + 9);
      }
      plVar21 = (longlong *)FUN_140347b50(0x49);
      (**(code **)(*plVar21 + 0x78))(plVar21,1);
      plVar21 = (longlong *)FUN_140347b50(0x48);
      (**(code **)(*plVar21 + 0x78))(plVar21,1);
      if (local_d8 != (longlong *)0x0) {
        thunk_FUN_1407864b8(local_d8);
      }
      if (local_a8 != (undefined8 *)0x0) {
        thunk_FUN_1407864b8(local_a8);
      }
      if (local_b8 != (longlong *)0x0) {
        thunk_FUN_1407864b8(local_b8);
      }
      if (local_a0 != (undefined8 *)0x0) {
        thunk_FUN_1407864b8(local_a0);
      }
      if (local_c0 != (longlong *)0x0) {
        thunk_FUN_1407864b8(local_c0);
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3edd00  Name: FUN_1403edd00  Size: 1933 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403edd00(longlong param_1)

{
  float fVar1;
  int iVar2;
  uint uVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *plVar6;
  undefined4 *puVar7;
  HANDLE hHeap;
  longlong *plVar8;
  longlong lVar9;
  undefined1 *puVar10;
  undefined8 ****ppppuVar11;
  undefined8 *****pppppuVar12;
  ulonglong uVar13;
  int iVar14;
  ulonglong uVar15;
  longlong lVar16;
  ulonglong uVar17;
  undefined8 *puVar18;
  float fVar19;
  undefined ***pppuVar20;
  undefined4 uVar21;
  undefined **local_128;
  LPVOID local_120;
  undefined8 local_118;
  undefined8 ***local_110;
  undefined8 uStack_108;
  ulonglong local_100;
  ulonglong local_f8;
  undefined8 ***local_f0 [2];
  undefined8 local_e0;
  ulonglong local_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined4 local_c0;
  undefined8 local_bc;
  float local_b4;
  undefined8 local_b0;
  undefined8 local_a8;
  longlong *local_a0;
  undefined8 ****local_98 [2];
  longlong local_88;
  ulonglong local_80;
  undefined1 local_78 [16];
  undefined1 local_68 [40];
  
  if ((*(longlong *)(param_1 + 0x28e0) != 0) &&
     (plVar4 = (longlong *)FUN_140347b50(0x242), plVar4 != (longlong *)0x0)) {
    local_a0 = plVar4;
    lVar5 = FUN_140294a40(&DAT_140a16fd0);
    fVar1 = DAT_1408007c0;
    lVar16 = *(longlong *)(param_1 + 0x28c8);
    uVar17 = 0;
    lVar9 = *(longlong *)(param_1 + 0x28c0);
    fVar19 = *(float *)(lVar5 + 0x70) + DAT_1408007c0;
    uVar13 = uVar17;
    if (lVar16 - lVar9 >> 4 != 0) {
      do {
        iVar14 = (int)uVar13;
        plVar4 = (longlong *)FUN_140347b50(iVar14 + 0x246);
        if (plVar4 != (longlong *)0x0) {
          lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x28c0) + uVar13 * 0x10);
          if (lVar9 == 0) {
            (**(code **)(*plVar4 + 0x80))(plVar4,1);
            (**(code **)(*plVar4 + 0x78))(plVar4,1);
          }
          else {
            FUN_14001e680(local_98,lVar9 + 0x1820);
            if (local_88 == 0) {
              pppppuVar12 = (undefined8 *****)"i_button_temporary.tga";
            }
            else {
              pppppuVar12 = local_98;
              if (0xf < local_80) {
                pppppuVar12 = (undefined8 *****)local_98[0];
              }
            }
            (**(code **)(*plVar4 + 0xa0))(plVar4,pppppuVar12,0);
            (**(code **)(*plVar4 + 0x78))(plVar4,0);
            FUN_140353ad0(plVar4,(longlong)
                                 *(int *)(*(longlong *)
                                           (*(longlong *)(param_1 + 0x28c0) + uVar13 * 0x10) + 0xf0)
                          ,0);
            FUN_140353ad0(plVar4,(longlong)iVar14,1);
            plVar8 = (longlong *)FUN_140347b50(iVar14 + 0x25e);
            local_f0[0] = (undefined8 ****)0x0;
            local_e0 = 0;
            local_d8 = 7;
            iVar2 = (int)(*(float *)(*(longlong *)(param_1 + 0x28c0) + 8 + uVar13 * 0x10) + fVar1);
            FUN_140200c00(local_f0,L"$ %d",iVar2);
            (**(code **)(*plVar8 + 0x148))(plVar8,local_f0,1,0);
            (**(code **)(*plVar8 + 0x78))(plVar8,0);
            plVar6 = (longlong *)FUN_140347b50(iVar14 + 0x252);
            lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x28c0) + uVar13 * 0x10);
            puVar18 = (undefined8 *)(lVar9 + 0x140);
            local_110 = (undefined8 ***)0x0;
            local_100 = 0;
            local_f8 = 0;
            uVar13 = *(ulonglong *)(lVar9 + 0x150);
            if (7 < *(ulonglong *)(lVar9 + 0x158)) {
              puVar18 = (undefined8 *)*puVar18;
            }
            if (uVar13 < 8) {
              local_110 = (undefined8 ***)*puVar18;
              uStack_108 = puVar18[1];
              local_f8 = 7;
            }
            else {
              uVar15 = uVar13 | 7;
              if (0x7ffffffffffffffe < uVar15) {
                uVar15 = 0x7ffffffffffffffe;
              }
              local_110 = (undefined8 ***)FUN_1400226c0(&local_110,uVar15 + 1);
              FUN_14076bd80(local_110,puVar18,uVar13 * 2 + 2);
              local_f8 = uVar15;
            }
            local_128 = DynamicVectorClass<class_std::basic_string<wchar_t,struct_std::char_traits<wchar_t>,class_std::allocator<wchar_t>_>_>
                        ::vftable;
            local_120 = (LPVOID)0x0;
            local_118 = 0;
            local_100 = uVar13;
            puVar7 = (undefined4 *)FUN_14034c990(plVar6);
            pppuVar20 = &local_128;
            (**(code **)(*plVar6 + 0x1c8))
                      (plVar6,&local_110,*puVar7,(int)(float)puVar7[1],pppuVar20);
            uVar21 = (undefined4)((ulonglong)pppuVar20 >> 0x20);
            (**(code **)(*plVar6 + 0x148))(plVar6,local_120,1,0);
            if ((int)local_118 < 2) {
              lVar9 = 0;
            }
            else {
              lVar9 = (longlong)local_120 + 0x20;
            }
            (**(code **)(*plVar6 + 0x148))(plVar6,lVar9,0,0);
            (**(code **)(*plVar6 + 0x78))(plVar6,0);
            if ((int)fVar19 < iVar2) {
              (**(code **)(*plVar4 + 0x80))(plVar4,1);
              FUN_1401f93e0(local_68,0x80,0x80,0x80,CONCAT44(uVar21,0xff));
              (**(code **)(*plVar4 + 0x10))(plVar4,local_68);
              (**(code **)(*plVar8 + 0x128))(plVar8,0x80,0x80,0x80,0xff,1);
              (**(code **)(*plVar6 + 0x128))(plVar6,0x80,0x80,0x80,0xff,1);
              (**(code **)(*plVar6 + 0x128))(plVar6,0x80,0x80,0x80,0xff,0);
              puVar10 = local_68;
            }
            else {
              (**(code **)(*plVar4 + 0x80))(plVar4,0);
              FUN_1401f93e0(local_78,0xff,0xff,0xff,CONCAT44(uVar21,0xff));
              (**(code **)(*plVar4 + 0x10))(plVar4,local_78);
              (**(code **)(*plVar8 + 0x128))(plVar8,0xff,0xff,0xff,0xff,1);
              (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0xff,0xff,0xff,1);
              (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0xff,0xff,0xff,0);
              puVar10 = local_78;
            }
            FUN_1401f9440(puVar10);
            local_128 = DynamicVectorClass<class_std::basic_string<wchar_t,struct_std::char_traits<wchar_t>,class_std::allocator<wchar_t>_>_>
                        ::vftable;
            if (local_120 != (LPVOID)0x0) {
              if ((local_118 & 0x8000000000000000) == 0) {
                FUN_14001f020(local_120,3);
              }
              else {
                hHeap = GetProcessHeap();
                HeapFree(hHeap,0,local_120);
              }
              local_120 = (LPVOID)0x0;
              local_118 = (ulonglong)(local_118._4_4_ & 0x80000000) << 0x20;
            }
            if (7 < local_f8) {
              uVar13 = local_f8 * 2 + 2;
              ppppuVar11 = (undefined8 ****)local_110;
              if (0xfff < uVar13) {
                ppppuVar11 = (undefined8 ****)local_110[-1];
                uVar13 = local_f8 * 2 + 0x29;
                if (0x1f < (ulonglong)((longlong)local_110 + (-8 - (longlong)ppppuVar11)))
                goto LAB_1403ee488;
              }
              thunk_FUN_1407864b8(ppppuVar11,uVar13);
            }
            local_100 = 0;
            local_f8 = 7;
            local_110 = (undefined8 ***)((ulonglong)local_110 & 0xffffffffffff0000);
            if (7 < local_d8) {
              uVar13 = local_d8 * 2 + 2;
              ppppuVar11 = (undefined8 ****)local_f0[0];
              if (0xfff < uVar13) {
                ppppuVar11 = (undefined8 ****)local_f0[0][-1];
                uVar13 = local_d8 * 2 + 0x29;
                if (0x1f < (ulonglong)((longlong)local_f0[0] + (-8 - (longlong)ppppuVar11)))
                goto LAB_1403ee488;
              }
              thunk_FUN_1407864b8(ppppuVar11,uVar13);
            }
            local_e0 = 0;
            local_d8 = 7;
            local_f0[0] = (undefined8 ***)((ulonglong)local_f0[0] & 0xffffffffffff0000);
            if (0xf < local_80) {
              if (0xfff < local_80 + 1) {
                ppppuVar11 = (undefined8 ****)local_98[0][-1];
                uVar13 = local_80 + 0x28;
                if ((char *)0x1f < (char *)((longlong)local_98[0] + (-8 - (longlong)ppppuVar11))) {
LAB_1403ee488:
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(ppppuVar11,uVar13);
                }
              }
              thunk_FUN_1407864b8();
            }
          }
        }
        lVar16 = *(longlong *)(param_1 + 0x28c8);
        lVar9 = *(longlong *)(param_1 + 0x28c0);
        plVar4 = local_a0;
        uVar13 = (ulonglong)(iVar14 + 1);
      } while ((ulonglong)(iVar14 + 1) < (ulonglong)(lVar16 - lVar9 >> 4));
    }
    iVar14 = (int)(lVar16 - lVar9 >> 4);
    if (iVar14 < 0xc) {
      iVar14 = iVar14 + 0x252;
      do {
        plVar8 = (longlong *)FUN_140347b50(iVar14 + -0xc);
        if (plVar8 != (longlong *)0x0) {
          (**(code **)(*plVar8 + 0x80))(plVar8,1);
          (**(code **)(*plVar8 + 0x78))(plVar8,1);
          plVar8 = (longlong *)FUN_140347b50(iVar14);
          if (plVar8 != (longlong *)0x0) {
            (**(code **)(*plVar8 + 0x78))(plVar8,1);
            plVar8 = (longlong *)FUN_140347b50(iVar14 + 0xc);
            if (plVar8 != (longlong *)0x0) {
              (**(code **)(*plVar8 + 0x78))(plVar8,1);
            }
          }
        }
        iVar14 = iVar14 + 1;
      } while (iVar14 < 0x25e);
      lVar16 = *(longlong *)(param_1 + 0x28c8);
    }
    uVar3 = ((int)(lVar16 - *(longlong *)(param_1 + 0x28c0) >> 4) + -1) / 2;
    if (-1 < (int)uVar3) {
      if (5 < (int)uVar3) {
        uVar3 = 5;
      }
      uVar17 = (ulonglong)uVar3;
    }
    (**(code **)(*plVar4 + 0x300))(plVar4,uVar17);
    local_d0 = 0x3f800000;
    local_c8 = 0;
    local_c0 = 0;
    local_bc = 0x3f800000;
    local_b0 = 0;
    local_a8 = 0x3f800000;
    local_b4 = (float)(5 - (int)uVar17) * _DAT_140868390;
    lVar9 = FUN_140347b50(0x244);
    if (lVar9 != 0) {
      uVar21 = *(undefined4 *)(lVar9 + 0xa0);
      FUN_14034ba10(plVar4,uVar21,1);
      FUN_140354c80(plVar4,uVar21,&local_d0);
    }
    lVar9 = FUN_140347b50(0x245);
    if (lVar9 != 0) {
      uVar21 = *(undefined4 *)(lVar9 + 0xa0);
      FUN_14034ba10(plVar4,uVar21,1);
      FUN_140354c80(plVar4,uVar21,&local_d0);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e5aa0  Name: FUN_1403e5aa0  Size: 1906 bytes
// ------------------------------------------------------------

void FUN_1403e5aa0(longlong param_1)

{
  float fVar1;
  uint uVar2;
  uint uVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong lVar7;
  longlong *plVar8;
  undefined1 *puVar9;
  undefined8 *puVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  ulonglong *puVar13;
  int iVar14;
  float fVar15;
  uint local_res10;
  undefined1 local_d8 [16];
  undefined1 local_c8 [16];
  ulonglong local_b8;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  ulonglong local_a8;
  ulonglong local_a0;
  longlong local_98 [2];
  undefined8 local_88;
  ulonglong local_80;
  longlong local_78 [2];
  undefined8 local_68;
  ulonglong local_60;
  
  uVar2 = FUN_140347af0(0x346);
  if ((*(longlong *)(param_1 + 0x2908) != 0) && (lVar4 = FUN_140347b50(0x33a), lVar4 != 0)) {
    if (0 < (int)uVar2) {
      iVar14 = 0x34b;
      uVar12 = (ulonglong)uVar2;
      do {
        plVar5 = (longlong *)FUN_140347b50(iVar14 + -5);
        if (plVar5 != (longlong *)0x0) {
          (**(code **)(*plVar5 + 0x80))(plVar5,1);
          FUN_140353ad0(plVar5,0xffffffffffffffff);
          (**(code **)(*plVar5 + 0x78))(plVar5,1);
          plVar5 = (longlong *)FUN_140347b50(iVar14);
          if (plVar5 != (longlong *)0x0) {
            (**(code **)(*plVar5 + 0x80))(plVar5,1);
            (**(code **)(*plVar5 + 0x148))(plVar5,0,1,0);
            (**(code **)(*plVar5 + 0x78))(plVar5,1);
            plVar5 = (longlong *)FUN_140347b50(iVar14 + 5);
            if (plVar5 != (longlong *)0x0) {
              (**(code **)(*plVar5 + 0x80))(plVar5,1);
              (**(code **)(*plVar5 + 0x148))(plVar5,0,1);
              (**(code **)(*plVar5 + 0x78))(plVar5,1);
            }
          }
        }
        iVar14 = iVar14 + 1;
        uVar12 = uVar12 - 1;
      } while (uVar12 != 0);
    }
    lVar4 = FUN_140294a40(&DAT_140a16fd0);
    fVar1 = DAT_1408007c0;
    fVar15 = *(float *)(lVar4 + 0x70) + DAT_1408007c0;
    uVar3 = (uint)(*(longlong *)(param_1 + 0x28f0) - *(longlong *)(param_1 + 0x28e8) >> 4);
    if (uVar3 < uVar2) {
      uVar2 = uVar3;
    }
    local_res10 = 0;
    if (uVar2 != 0) {
      lVar4 = 0;
      do {
        iVar14 = FUN_1405b0160(*(undefined8 *)(lVar4 + *(longlong *)(param_1 + 0x28e8)),0);
        plVar5 = (longlong *)FUN_140347b50(iVar14 + 0x345);
        lVar7 = *(longlong *)(lVar4 + *(longlong *)(param_1 + 0x28e8));
        puVar10 = (undefined8 *)(lVar7 + 0x98);
        if (*(longlong *)(lVar7 + 0xa8) == 0) {
          (**(code **)(*plVar5 + 0xa0))(plVar5,"i_button_temporary.tga",0);
        }
        else {
          if (0xf < *(ulonglong *)(lVar7 + 0xb0)) {
            puVar10 = (undefined8 *)*puVar10;
          }
          (**(code **)(*plVar5 + 0xa0))(plVar5,puVar10,0);
        }
        (**(code **)(*plVar5 + 0x78))(plVar5,0);
        FUN_140353ad0(plVar5,*(undefined4 *)
                              (*(longlong *)(lVar4 + *(longlong *)(param_1 + 0x28e8)) + 0xb8),0);
        FUN_140353ad0(plVar5,(longlong)(int)local_res10,1);
        plVar6 = (longlong *)FUN_140347b50(iVar14 + 0x34a);
        local_78[0] = 0;
        local_68 = 0;
        local_60 = 7;
        lVar7 = *(longlong *)(lVar4 + *(longlong *)(param_1 + 0x28e8));
        puVar13 = (ulonglong *)(lVar7 + 0xc0);
        local_b8 = 0;
        local_a8 = 0;
        local_a0 = 0;
        uVar12 = *(ulonglong *)(lVar7 + 0xd0);
        if (0xf < *(ulonglong *)(lVar7 + 0xd8)) {
          puVar13 = (ulonglong *)*puVar13;
        }
        if (uVar12 < 0x10) {
          local_b8 = *puVar13;
          uStack_b0 = (undefined4)puVar13[1];
          uStack_ac = *(undefined4 *)((longlong)puVar13 + 0xc);
          local_a0 = 0xf;
        }
        else {
          uVar11 = uVar12 | 0xf;
          if (0x7fffffffffffffff < uVar11) {
            uVar11 = 0x7fffffffffffffff;
          }
          if (uVar11 + 1 < 0x1000) {
            if (uVar11 == 0xffffffffffffffff) {
              local_b8 = 0;
            }
            else {
              local_b8 = FUN_140769c58();
            }
          }
          else {
            if (uVar11 + 0x28 <= uVar11 + 1) {
                    /* WARNING: Subroutine does not return */
              FUN_1400225f0();
            }
            lVar7 = FUN_140769c58(uVar11 + 0x28);
            if (lVar7 == 0) goto LAB_1403e6204;
            local_b8 = lVar7 + 0x27U & 0xffffffffffffffe0;
            *(longlong *)(local_b8 - 8) = lVar7;
          }
          FUN_14076bd80(local_b8,puVar13,uVar12 + 1);
          local_a0 = uVar11;
        }
        local_a8 = uVar12;
        FUN_1401fa1a0(&DAT_140a7bc58,&local_b8,local_78,1);
        (**(code **)(*plVar6 + 0x148))(plVar6,local_78,1,0);
        (**(code **)(*plVar6 + 0x78))(plVar6,0);
        if (*(char *)(lVar4 + 0xc + *(longlong *)(param_1 + 0x28e8)) == '\0') {
          plVar8 = (longlong *)FUN_140347b50(iVar14 + 0x34f);
          iVar14 = (int)(*(float *)(lVar4 + 8 + *(longlong *)(param_1 + 0x28e8)) + fVar1);
          if ((int)fVar15 < iVar14) {
            (**(code **)(*plVar5 + 0x80))(plVar5,1);
            FUN_1401f93e0(local_c8,0x80,0x80,0x80,0xff);
            (**(code **)(*plVar5 + 0x10))(plVar5,local_c8);
            (**(code **)(*plVar5 + 0x128))(plVar5,0x80,0x80,0x80,0xff,1);
            (**(code **)(*plVar6 + 0x128))(plVar6,0xc0,0xc0,0xc0,0xff,1);
            (**(code **)(*plVar8 + 0x128))(plVar8,0xc0,0xc0,0xc0,0xff,1);
            puVar9 = local_c8;
          }
          else {
            (**(code **)(*plVar5 + 0x128))(plVar5,0x32,0xff,100,0xff,1);
            (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0xff,0xff,0xff,1);
            (**(code **)(*plVar8 + 0x128))(plVar8,0xff,0xff,0xff,0xff,1);
            (**(code **)(*plVar5 + 0x80))(plVar5,0);
            FUN_1401f93e0(local_d8,0xff,0xff,0xff,0xff);
            (**(code **)(*plVar5 + 0x10))(plVar5,local_d8);
            puVar9 = local_d8;
          }
          FUN_1401f9440(puVar9);
          local_98[0] = 0;
          local_88 = 0;
          local_80 = 7;
          FUN_140200c00(local_98,L"$ %d",iVar14);
          (**(code **)(*plVar8 + 0x148))(plVar8,local_98,1,0);
          (**(code **)(*plVar8 + 0x78))(plVar8,0);
          (**(code **)(*plVar8 + 0x80))(plVar8,0);
          (**(code **)(*plVar8 + 0xa0))(plVar8,0);
          if (7 < local_80) {
            if (0xfff < local_80 * 2 + 2) {
              if (0x1f < (local_98[0] - *(longlong *)(local_98[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(*(longlong *)(local_98[0] + -8),local_80 * 2 + 0x29);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        else {
          (**(code **)(*plVar5 + 0x80))(plVar5,1);
          plVar5 = (longlong *)FUN_140347b50(iVar14 + 0x34f);
          local_98[0] = 0;
          local_88 = 0;
          local_80 = 7;
          FUN_1401f9fe0(&DAT_140a7bc58,"TEXT_BLACK_MARKET_PURCHASED",local_98);
          (**(code **)(*plVar5 + 0x148))(plVar5,local_98,1,0);
          (**(code **)(*plVar5 + 0x78))(plVar5,0);
          (**(code **)(*plVar5 + 0x128))(plVar5,0xff,0x10,0x10,0xff,1);
          (**(code **)(*plVar6 + 0x128))(plVar6,0xc0,0xc0,0xc0,0xff,1);
          if (7 < local_80) {
            if (0xfff < local_80 * 2 + 2) {
              if (0x1f < (local_98[0] - *(longlong *)(local_98[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(*(longlong *)(local_98[0] + -8),local_80 * 2 + 0x29);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        if (0xf < local_a0) {
          uVar11 = local_a0 + 1;
          uVar12 = local_b8;
          if (0xfff < uVar11) {
            uVar12 = *(ulonglong *)(local_b8 - 8);
            uVar11 = local_a0 + 0x28;
            if (0x1f < (local_b8 - uVar12) - 8) goto LAB_1403e6204;
          }
          thunk_FUN_1407864b8(uVar12,uVar11);
        }
        local_a8 = 0;
        local_a0 = 0xf;
        local_b8 = local_b8 & 0xffffffffffffff00;
        if (7 < local_60) {
          if ((0xfff < local_60 * 2 + 2) &&
             (0x1f < (local_78[0] - *(longlong *)(local_78[0] + -8)) - 8U)) {
LAB_1403e6204:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
          thunk_FUN_1407864b8();
        }
        lVar4 = lVar4 + 0x10;
        local_res10 = local_res10 + 1;
      } while (local_res10 < uVar2);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x310e40  Name: FUN_140310e40  Size: 1340 bytes
// ------------------------------------------------------------

void FUN_140310e40(longlong param_1)

{
  float fVar1;
  longlong *plVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 ****ppppuVar7;
  undefined8 uVar8;
  float *pfVar9;
  undefined8 ****ppppuVar10;
  ulonglong uVar11;
  undefined8 uVar12;
  float fVar13;
  float local_res8;
  float local_resc;
  float local_a8;
  float fStack_a4;
  float fStack_a0;
  float fStack_9c;
  undefined1 local_98 [16];
  undefined8 ***local_88 [2];
  undefined8 local_78;
  ulonglong local_70;
  undefined8 ***local_68 [2];
  undefined8 local_58;
  ulonglong local_50;
  longlong local_48 [2];
  undefined8 local_38;
  ulonglong local_30;
  
  if ((*(longlong *)(param_1 + 0x20d0) != 0) &&
     (lVar5 = FUN_14034c770(*(longlong *)(param_1 + 0x20d0),0), lVar5 != 0)) {
    local_48[0] = 0;
    local_38 = 0;
    local_88[0] = (undefined8 ****)0x0;
    local_78 = 0;
    local_68[0] = (undefined8 ****)0x0;
    local_58 = 0;
    local_30 = 7;
    local_70 = 7;
    local_50 = 7;
    FUN_1401f92f0(&local_a8);
    if (((*(char *)(lVar5 + 0x391) == -1) && (*(char *)(lVar5 + 0x392) == -1)) ||
       ((((plVar2 = *(longlong **)(lVar5 + 0x2b0), plVar2 == (longlong *)0x0 ||
          ((char)plVar2[0x67] == -1)) ||
         (lVar6 = (**(code **)(*plVar2 + 0x10))(plVar2,6), lVar6 == 0)) ||
        (cVar3 = FUN_140401a20(lVar6,*(undefined8 *)(lVar5 + 0x298)), cVar3 == '\0')))) {
      lVar6 = FUN_1403973b0(lVar5);
      iVar4 = FUN_1403711c0(lVar6,0);
      fVar1 = *(float *)(lVar5 + 0x32c);
      iVar4 = iVar4 / 2;
      if (fVar1 <= DAT_1407ffaf8) {
        if (0.0 < fVar1) {
          iVar4 = 0;
        }
      }
      else {
        iVar4 = FUN_14022f9c0(fVar1 / DAT_1408007d4);
      }
      ppppuVar7 = (undefined8 ****)FUN_1401fa460(&DAT_140a7bc58,"TEXT_BUTTON_SELL");
      if (local_88 != ppppuVar7) {
        ppppuVar10 = ppppuVar7;
        if ((undefined8 ***)0x7 < ppppuVar7[3]) {
          ppppuVar10 = (undefined8 ****)*ppppuVar7;
        }
        FUN_1400228a0(local_88,ppppuVar10,ppppuVar7[2]);
      }
      ppppuVar7 = (undefined8 ****)(lVar6 + 0x140);
      if (local_68 != ppppuVar7) {
        if (7 < *(ulonglong *)(lVar6 + 0x158)) {
          ppppuVar7 = (undefined8 ****)*ppppuVar7;
        }
        FUN_1400228a0(local_68,ppppuVar7,*(undefined8 *)(lVar6 + 0x150));
      }
      ppppuVar7 = local_68;
      if (7 < local_50) {
        ppppuVar7 = (undefined8 ****)local_68[0];
      }
      ppppuVar10 = local_88;
      if (7 < local_70) {
        ppppuVar10 = (undefined8 ****)local_88[0];
      }
      FUN_140200c00(local_48,L" %s %s ($ %d) ",ppppuVar10,ppppuVar7,iVar4);
      uVar8 = FUN_14034c340(*(undefined8 *)(param_1 + 0x20d0));
      pfVar9 = (float *)FUN_1401f93e0(local_98,0xff,0xff,0xff,0xff);
      local_a8 = *pfVar9;
      fStack_a4 = pfVar9[1];
      fStack_a0 = pfVar9[2];
      fStack_9c = pfVar9[3];
      FUN_1401f9440(local_98);
      uVar12 = 0;
    }
    else {
      FUN_1403973b0(lVar5);
      uVar12 = 1;
      ppppuVar7 = (undefined8 ****)FUN_1401fa460(&DAT_140a7bc58,"TEXT_PRODUCTION_IN_PROGRESS01");
      if (local_88 != ppppuVar7) {
        ppppuVar10 = ppppuVar7;
        if ((undefined8 ***)0x7 < ppppuVar7[3]) {
          ppppuVar10 = (undefined8 ****)*ppppuVar7;
        }
        FUN_1400228a0(local_88,ppppuVar10,ppppuVar7[2]);
      }
      ppppuVar7 = local_88;
      if (7 < local_70) {
        ppppuVar7 = (undefined8 ****)local_88[0];
      }
      FUN_140200c00(local_48,L" %s ",ppppuVar7);
      uVar8 = FUN_14034c0d0(*(undefined8 *)(param_1 + 0x20d0));
      pfVar9 = (float *)FUN_1401f93e0(local_98,0x80,0x80,0x80,0xff);
      local_a8 = *pfVar9;
      fStack_a4 = pfVar9[1];
      fStack_a0 = pfVar9[2];
      fStack_9c = pfVar9[3];
      FUN_1401f9440(local_98);
    }
    (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0xa0))
              (*(longlong **)(param_1 + 0x20d0),uVar8,0);
    (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x148))
              (*(longlong **)(param_1 + 0x20d0),local_48,1,0);
    lVar5 = (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x270))
                      (*(longlong **)(param_1 + 0x20d0),1);
    iVar4 = *(int *)(*(longlong *)(lVar5 + 8) + 0x10);
    lVar5 = (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x270))
                      (*(longlong **)(param_1 + 0x20d0),1);
    fVar13 = (float)iVar4 * *(float *)(lVar5 + 0x40) * *(float *)(param_1 + 0x1000);
    lVar5 = (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x260))();
    *(float *)(lVar5 + 0x14) = fVar13;
    lVar5 = (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x278))();
    fVar1 = DAT_1408054e8;
    *(float *)(lVar5 + 0x14) = fVar13;
    (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x128))
              (*(longlong **)(param_1 + 0x20d0),(int)(local_a8 * fVar1),(int)(fStack_a4 * fVar1),
               (int)(fStack_a0 * fVar1),(int)(fStack_9c * fVar1) & 0xff,1);
    lVar5 = **(longlong **)(param_1 + 0x20d0);
    uVar8 = FUN_14034c5e0(*(longlong **)(param_1 + 0x20d0),0);
    (**(code **)(lVar5 + 0xb8))(*(undefined8 *)(param_1 + 0x20d0),uVar8);
    (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x80))(*(longlong **)(param_1 + 0x20d0),uVar12)
    ;
    uVar8 = FUN_140347b50(0x73);
    pfVar9 = (float *)FUN_14034c0b0(uVar8);
    local_res8 = *pfVar9 - fVar13 / DAT_1408007d4;
    local_resc = pfVar9[1];
    (**(code **)(**(longlong **)(param_1 + 0x20d0) + 0x168))
              (*(longlong **)(param_1 + 0x20d0),&local_res8);
    FUN_1401f9440(&local_a8);
    if (7 < local_50) {
      uVar11 = local_50 * 2 + 2;
      ppppuVar7 = (undefined8 ****)local_68[0];
      if (0xfff < uVar11) {
        ppppuVar7 = (undefined8 ****)local_68[0][-1];
        uVar11 = local_50 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)local_68[0] + (-8 - (longlong)ppppuVar7)))
        goto LAB_140311377;
      }
      thunk_FUN_1407864b8(ppppuVar7,uVar11);
    }
    local_58 = 0;
    local_50 = 7;
    local_68[0] = (undefined8 ***)((ulonglong)local_68[0] & 0xffffffffffff0000);
    if (7 < local_70) {
      uVar11 = local_70 * 2 + 2;
      ppppuVar7 = (undefined8 ****)local_88[0];
      if (0xfff < uVar11) {
        ppppuVar7 = (undefined8 ****)local_88[0][-1];
        uVar11 = local_70 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)local_88[0] + (-8 - (longlong)ppppuVar7)))
        goto LAB_140311377;
      }
      thunk_FUN_1407864b8(ppppuVar7,uVar11);
    }
    local_78 = 0;
    local_70 = 7;
    local_88[0] = (undefined8 ***)((ulonglong)local_88[0] & 0xffffffffffff0000);
    if (7 < local_30) {
      if (0xfff < local_30 * 2 + 2) {
        ppppuVar7 = *(undefined8 *****)(local_48[0] + -8);
        uVar11 = local_30 * 2 + 0x29;
        if (0x1f < (local_48[0] - (longlong)ppppuVar7) - 8U) {
LAB_140311377:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(ppppuVar7,uVar11);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e4cb0  Name: FUN_1403e4cb0  Size: 3560 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */

void FUN_1403e4cb0(longlong param_1)

{
  char cVar1;
  undefined8 ****ppppuVar2;
  uint uVar3;
  undefined8 *******pppppppuVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  longlong lVar10;
  longlong lVar11;
  undefined8 uVar12;
  undefined8 ****ppppuVar13;
  undefined8 *puVar14;
  undefined8 *****pppppuVar15;
  undefined8 *****pppppuVar16;
  undefined8 *****pppppuVar17;
  longlong *plVar18;
  longlong *plVar19;
  HANDLE pvVar20;
  undefined8 ***pppuVar21;
  undefined8 *****pppppuVar22;
  undefined8 *****pppppuVar23;
  ulonglong uVar24;
  ulonglong uVar25;
  undefined8 *******pppppppuVar26;
  undefined8 *******pppppppuVar27;
  int iVar28;
  longlong lVar29;
  int iVar30;
  int iVar31;
  undefined4 extraout_XMM0_Da;
  int local_res20;
  undefined8 ******local_138;
  uint uStack_130;
  undefined4 uStack_12c;
  ulonglong local_128;
  ulonglong uStack_120;
  undefined8 *****local_118;
  longlong local_110;
  int local_108;
  int local_104;
  undefined **local_100;
  LPVOID local_f8;
  undefined8 local_f0;
  undefined **local_e8;
  LPVOID local_e0;
  int local_d8;
  uint local_d4;
  uint local_d0;
  undefined8 ***local_c8;
  longlong local_c0;
  undefined8 local_b8;
  undefined8 *******local_b0 [2];
  ulonglong local_a0;
  ulonglong local_98;
  undefined8 *******local_90 [2];
  ulonglong local_80;
  ulonglong local_78;
  short local_70 [24];
  
  if ((*(longlong *)(param_1 + 0x2150) != 0) && (*(char *)(param_1 + 0x2a34) != '\0')) {
    lVar10 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x2150) + 0x58));
    lVar11 = FUN_140294a40(&DAT_140a16fd0);
    if (lVar10 != lVar11) {
      lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x2150) + 0xb8);
      cVar5 = FUN_14050b970(*(longlong *)(param_1 + 0x2150),0xffffffff);
      if ((cVar5 == '\0') &&
         ((*(int *)(lVar10 + 0x2f4) == -1 ||
          (lVar10 = FUN_140294a40(&DAT_140a16fd0),
          *(char *)(*(longlong *)(lVar10 + 0x68) + 0x10b) == '\0')))) {
        *(undefined1 *)(param_1 + 0x2ae4) = 0;
        FUN_1403ce970(param_1);
        return;
      }
      *(undefined1 *)(param_1 + 0x2ae4) = 1;
    }
    local_e0 = (LPVOID)0x0;
    local_d4 = 0;
    local_e8 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    uVar12 = (**(code **)(**(longlong **)(param_1 + 0x2150) + 0x10))
                       (*(longlong **)(param_1 + 0x2150),3);
    local_b8 = uVar12;
    iVar6 = FUN_140294a70(&DAT_140a16fd0);
    local_104 = 0x2cb;
    iVar7 = FUN_140347af0();
    local_100 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    local_f8 = (LPVOID)0x0;
    local_f0 = 0;
    local_110 = 0;
    local_118 = (undefined8 *****)FUN_140769c58(0x38);
    *local_118 = local_118;
    local_118[1] = local_118;
    local_118[2] = local_118;
    *(undefined2 *)(local_118 + 3) = 0x101;
    iVar8 = FUN_1403f9530(uVar12,*(undefined8 *)(param_1 + 0x2150));
    local_d0 = (uint)(iVar8 < DAT_140b168c8);
    local_108 = 0;
    do {
      local_d8 = 0;
      FUN_1403f9370(uVar12,*(undefined8 *)(param_1 + 0x2150),local_108,&local_e8);
      local_res20 = 0;
      if (0 < local_d8) {
        local_c0 = 0;
        do {
          lVar10 = local_c0;
          local_f0 = local_f0 & 0xffffffff00000000;
          lVar11 = FUN_1403956c0(*(undefined8 *)(local_c0 + (longlong)local_e0));
          if (*(int *)(lVar11 + 4) == iVar6) {
            FUN_1403965f0(*(undefined8 *)(lVar10 + (longlong)local_e0),&local_100,0);
            iVar8 = 0;
            iVar31 = 0;
            pppuVar21 = *(undefined8 ****)(lVar10 + (longlong)local_e0);
            local_c8 = pppuVar21;
            if (0 < (int)local_f0) {
              lVar11 = 0;
              do {
                ppppuVar2 = *(undefined8 *****)(lVar11 + (longlong)local_f8);
                if (((((ulonglong)ppppuVar2[0x74] & 2) == 0) && (ppppuVar2[0x56] == pppuVar21)) &&
                   ((ppppuVar2[0x36] == (undefined8 ***)0x0 ||
                    (*(char *)((longlong)ppppuVar2[0x36] + 0x31) == '\0')))) {
                  ppppuVar13 = (undefined8 ****)FUN_1402f6270(param_1,ppppuVar2);
                  pppppuVar16 = local_118;
                  iVar30 = -1;
                  iVar28 = -1;
                  if (ppppuVar2[0x36] != (undefined8 ***)0x0) {
                    iVar28 = *(int *)(ppppuVar2[0x36] + 1);
                  }
                  pppppuVar17 = (undefined8 *****)local_118[1];
                  cVar1 = *(char *)((longlong)pppppuVar17 + 0x19);
                  pppppuVar15 = pppppuVar17;
                  pppppuVar23 = local_118;
                  cVar5 = cVar1;
                  while (pppppuVar22 = pppppuVar15, cVar5 == '\0') {
                    if (pppppuVar22[4] < ppppuVar13) {
                      pppppuVar15 = (undefined8 *****)pppppuVar22[2];
                      pppppuVar22 = pppppuVar23;
                    }
                    else {
                      pppppuVar15 = (undefined8 *****)*pppppuVar22;
                    }
                    cVar5 = *(char *)((longlong)pppppuVar15 + 0x19);
                    pppppuVar23 = pppppuVar22;
                  }
                  if ((*(char *)((longlong)pppppuVar23 + 0x19) != '\0') ||
                     (ppppuVar13 < pppppuVar23[4])) {
                    pppppuVar23 = local_118;
                  }
                  if (pppppuVar23 == local_118) {
                    uStack_130 = 0;
                    local_138 = (undefined8 ******)pppppuVar17;
                    pppppuVar15 = local_118;
                    while (pppppuVar23 = pppppuVar17, cVar1 == '\0') {
                      if (ppppuVar13 <= pppppuVar23[4]) {
                        pppppuVar17 = (undefined8 *****)*pppppuVar23;
                        pppppuVar15 = pppppuVar23;
                      }
                      else {
                        pppppuVar17 = (undefined8 *****)pppppuVar23[2];
                      }
                      uStack_130 = (uint)(ppppuVar13 <= pppppuVar23[4]);
                      cVar1 = *(char *)((longlong)pppppuVar17 + 0x19);
                      local_138 = (undefined8 ******)pppppuVar23;
                    }
                    if ((*(char *)((longlong)pppppuVar15 + 0x19) != '\0') ||
                       (ppppuVar13 < pppppuVar15[4])) {
                      if (local_110 == 0x492492492492492) goto LAB_1403e5a94;
                      puVar14 = (undefined8 *)FUN_140769c58(0x38);
                      puVar14[4] = ppppuVar13;
                      puVar14[5] = 0;
                      puVar14[6] = 0;
                      *puVar14 = pppppuVar16;
                      puVar14[1] = pppppuVar16;
                      puVar14[2] = pppppuVar16;
                      *(undefined2 *)(puVar14 + 3) = 0;
                      pppppuVar15 = (undefined8 *****)FUN_14004fe80(&local_118);
                    }
                    pppppuVar23 = local_118;
                    *(undefined4 *)(pppppuVar15 + 6) = 1;
                    uStack_130 = 0;
                    pppppuVar16 = (undefined8 *****)local_118[1];
                    cVar5 = *(char *)((longlong)pppppuVar16 + 0x19);
                    local_138 = (undefined8 ******)pppppuVar16;
                    pppppuVar17 = local_118;
                    while (pppppuVar15 = pppppuVar16, cVar5 == '\0') {
                      if (ppppuVar13 <= pppppuVar15[4]) {
                        pppppuVar16 = (undefined8 *****)*pppppuVar15;
                        pppppuVar17 = pppppuVar15;
                      }
                      else {
                        pppppuVar16 = (undefined8 *****)pppppuVar15[2];
                      }
                      uStack_130 = (uint)(ppppuVar13 <= pppppuVar15[4]);
                      cVar5 = *(char *)((longlong)pppppuVar16 + 0x19);
                      local_138 = (undefined8 ******)pppppuVar15;
                    }
                    if ((*(char *)((longlong)pppppuVar17 + 0x19) != '\0') ||
                       (ppppuVar13 < pppppuVar17[4])) {
                      if (local_110 == 0x492492492492492) {
LAB_1403e5a94:
                    /* WARNING: Subroutine does not return */
                        FUN_1400501c0();
                      }
                      puVar14 = (undefined8 *)FUN_140769c58(0x38);
                      puVar14[4] = ppppuVar13;
                      puVar14[5] = 0;
                      puVar14[6] = 0;
                      *puVar14 = pppppuVar23;
                      puVar14[1] = pppppuVar23;
                      puVar14[2] = pppppuVar23;
                      *(undefined2 *)(puVar14 + 3) = 0;
                      pppppuVar17 = (undefined8 *****)FUN_14004fe80(&local_118,&local_138);
                    }
                    pppppuVar17[5] = ppppuVar2;
                    iVar8 = iVar8 + 1;
                    pppuVar21 = local_c8;
                    lVar10 = local_c0;
                    if (iVar7 <= iVar8) break;
                  }
                  else {
                    *(int *)(pppppuVar23 + 6) = *(int *)(pppppuVar23 + 6) + 1;
                    pppuVar21 = local_c8;
                    if (iVar28 != DAT_140b278b0) {
                      if (pppppuVar23[5][0x36] != (undefined8 ***)0x0) {
                        iVar30 = *(int *)(pppppuVar23[5][0x36] + 1);
                      }
                      pppppuVar23[5] = ppppuVar2;
                      if ((iVar30 == DAT_140b278b0) ||
                         ((ppppuVar2[0x36] != (undefined8 ***)0x0 &&
                          (*(char *)((longlong)ppppuVar2[0x36] + 0x32) != '\0')))) {
                        lVar10 = FUN_140769c58(0x50);
                        if (lVar10 == 0) {
                          uVar12 = 0;
                        }
                        else {
                          uVar12 = FUN_1405b1170(lVar10);
                        }
                        FUN_1405b06b0(uVar12,*(undefined4 *)(ppppuVar2 + 10));
                        FUN_1403b01f0(&DAT_140b2ed18,uVar12);
                      }
                      FUN_1403a87c0(ppppuVar2,iVar30);
                      pppuVar21 = local_c8;
                    }
                  }
                }
                iVar31 = iVar31 + 1;
                lVar11 = lVar11 + 8;
                lVar10 = local_c0;
              } while (iVar31 < (int)local_f0);
            }
          }
          local_c0 = lVar10 + 8;
          local_res20 = local_res20 + 1;
          uVar12 = local_b8;
        } while (local_res20 < local_d8);
      }
      uVar3 = local_d0;
      iVar8 = 0;
      local_108 = local_108 + 1;
    } while (local_108 < 3);
    pppppuVar16 = (undefined8 *****)*local_118;
    pppppuVar17 = local_118;
    if (pppppuVar16 != local_118) {
      do {
        iVar6 = local_104;
        if (iVar7 <= iVar8) break;
        if (*(char *)((longlong)pppppuVar16[5] + 0x337) != -1) {
          FUN_140309a80(param_1,local_104,pppppuVar16[5],pppppuVar16[4],
                        *(undefined4 *)(pppppuVar16 + 6),1,1,0);
          plVar18 = (longlong *)FUN_140347b50(iVar6);
          if (((char)uVar3 == '\0') || (*(char *)(param_1 + 0x2ae4) != '\0')) {
            (**(code **)(*plVar18 + 0x80))(plVar18,1);
          }
          else {
            (**(code **)(*plVar18 + 0x80))(plVar18,0);
          }
          iVar8 = iVar8 + 1;
          local_104 = iVar6 + 1;
          pppppuVar17 = local_118;
        }
        pppppuVar15 = (undefined8 *****)pppppuVar16[2];
        if (*(char *)((longlong)pppppuVar15 + 0x19) == '\0') {
          cVar5 = *(char *)((longlong)*pppppuVar15 + 0x19);
          pppppuVar16 = pppppuVar15;
          pppppuVar15 = (undefined8 *****)*pppppuVar15;
          while (cVar5 == '\0') {
            cVar5 = *(char *)((longlong)*pppppuVar15 + 0x19);
            pppppuVar16 = pppppuVar15;
            pppppuVar15 = (undefined8 *****)*pppppuVar15;
          }
        }
        else {
          cVar5 = *(char *)((longlong)pppppuVar16[1] + 0x19);
          pppppuVar23 = (undefined8 *****)pppppuVar16[1];
          pppppuVar15 = pppppuVar16;
          while ((pppppuVar16 = pppppuVar23, cVar5 == '\0' &&
                 (pppppuVar15 == (undefined8 *****)pppppuVar16[2]))) {
            cVar5 = *(char *)((longlong)pppppuVar16[1] + 0x19);
            pppppuVar23 = (undefined8 *****)pppppuVar16[1];
            pppppuVar15 = pppppuVar16;
          }
        }
      } while (pppppuVar16 != pppppuVar17);
    }
    lVar10 = -1;
    for (; iVar8 < iVar7; iVar8 = iVar8 + 1) {
      plVar18 = (longlong *)FUN_140347b50(iVar8 + 0x2cb);
      if (plVar18 != (longlong *)0x0) {
        (**(code **)(*plVar18 + 0x78))(plVar18,1);
        FUN_140353ad0(plVar18,0,0);
      }
    }
    uVar9 = FUN_140347af0(0x60);
    lVar11 = FUN_140397120(*(undefined8 *)(param_1 + 0x2150));
    FUN_140022b30(local_70,0x10,L"%d/%d",*(undefined4 *)(lVar11 + 0x10),uVar9);
    local_90[0] = (undefined8 *******)0x0;
    lVar29 = -1;
    local_80 = 0;
    local_78 = 7;
    do {
      lVar29 = lVar29 + 1;
    } while (local_70[lVar29] != 0);
    FUN_1400228a0(local_90,local_70);
    uVar24 = local_80;
    pppppppuVar4 = local_90[0];
    local_138 = (undefined8 ******)0x0;
    pppppppuVar26 = local_90;
    if (7 < local_78) {
      pppppppuVar26 = local_90[0];
    }
    local_128 = 0;
    uStack_120 = 0;
    if (local_80 < 8) {
      local_138 = *pppppppuVar26;
      uStack_130 = *(uint *)(pppppppuVar26 + 1);
      uStack_12c = *(undefined4 *)((longlong)pppppppuVar26 + 0xc);
      uStack_120 = 7;
    }
    else {
      uVar25 = local_80 | 7;
      if (0x7ffffffffffffffe < uVar25) {
        uVar25 = 0x7ffffffffffffffe;
      }
      local_138 = (undefined8 ******)FUN_1400226c0(&local_138,uVar25 + 1);
      FUN_14076bd80(local_138,pppppppuVar26,uVar24 * 2 + 2);
      uStack_120 = uVar25;
    }
    local_128 = uVar24;
    FUN_140347c30(0x2b7,&local_138);
    iVar6 = FUN_1403f8af0(local_b8,*(undefined8 *)(param_1 + 0x2150));
    uVar9 = FUN_140372740(*(undefined8 *)(*(longlong *)(param_1 + 0x2150) + 0x298),
                          *(longlong *)(param_1 + 0x2150));
    FUN_140022b30(local_70,0x10,L"%d/%d",iVar6,uVar9);
    local_98 = 7;
    local_b0[0] = (undefined8 *******)0x0;
    local_a0 = 0;
    do {
      lVar10 = lVar10 + 1;
    } while (local_70[lVar10] != 0);
    FUN_1400228a0(local_b0,local_70,lVar10);
    uVar24 = local_a0;
    pppppppuVar26 = local_b0[0];
    local_138 = (undefined8 ******)0x0;
    pppppppuVar27 = local_b0;
    if (7 < local_98) {
      pppppppuVar27 = local_b0[0];
    }
    local_128 = 0;
    uStack_120 = 0;
    if (local_a0 < 8) {
      local_138 = *pppppppuVar27;
      uStack_130 = *(uint *)(pppppppuVar27 + 1);
      uStack_12c = *(undefined4 *)((longlong)pppppppuVar27 + 0xc);
      uStack_120 = 7;
    }
    else {
      uVar25 = local_a0 | 7;
      if (0x7ffffffffffffffe < uVar25) {
        uVar25 = 0x7ffffffffffffffe;
      }
      local_138 = (undefined8 ******)FUN_1400226c0(&local_138,uVar25 + 1);
      FUN_14076bd80(local_138,pppppppuVar27,uVar24 * 2 + 2);
      uStack_120 = uVar25;
    }
    local_128 = uVar24;
    uVar9 = FUN_140347c30(0x2b9,&local_138);
    if ((*(char *)(param_1 + 0x2a34) != '\0') &&
       ((*(int *)(param_1 + 0x2ad8) != *(int *)(lVar11 + 0x10) ||
        (*(int *)(param_1 + 0x2adc) != iVar6)))) {
      FUN_1403d9ea0(param_1);
      FUN_1403d7790(param_1);
      uVar9 = FUN_1403d93f0(param_1);
    }
    if (*(longlong *)(param_1 + 0x2160) == 0) {
      iVar6 = 0x32f;
      do {
        plVar18 = (longlong *)FUN_140347b50(iVar6);
        if ((plVar18 != (longlong *)0x0) &&
           (cVar5 = (**(code **)(*plVar18 + 0x2f8))(plVar18), cVar5 != '\0')) {
          (**(code **)(*plVar18 + 0x2f0))(plVar18);
        }
        lVar10 = *plVar18;
        uVar12 = FUN_14034be00(plVar18,0);
        (**(code **)(lVar10 + 0xb0))(plVar18,uVar12);
        if ((*(byte *)(param_1 + 0x2ae0) & 1) == 0) {
          (**(code **)(*plVar18 + 0x78))();
        }
        else {
          FUN_14034c770(plVar18);
          (**(code **)(*plVar18 + 0x78))(plVar18);
          FUN_14034c770(plVar18);
          (**(code **)(*plVar18 + 0x80))(plVar18);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 0x339);
      iVar6 = 0x2df;
      do {
        plVar18 = (longlong *)FUN_140347b50(iVar6);
        iVar7 = FUN_14034c770(plVar18,3);
        uVar9 = extraout_XMM0_Da;
        if (plVar18 != (longlong *)0x0) {
          cVar5 = (**(code **)(*plVar18 + 0x2f8))(plVar18);
          if (cVar5 == '\0') {
LAB_1403e5878:
            if (iVar7 != 1) goto LAB_1403e587e;
LAB_1403e5894:
            lVar10 = *plVar18;
            uVar12 = FUN_14034be00(plVar18,0);
            (**(code **)(lVar10 + 0xb0))(plVar18,uVar12);
          }
          else {
            if (iVar7 - 1U < 2) {
              (**(code **)(*plVar18 + 0x2f0))(plVar18);
              goto LAB_1403e5878;
            }
LAB_1403e587e:
            if (iVar7 == 2) goto LAB_1403e5894;
            (**(code **)(*plVar18 + 0xb0))(plVar18,0);
          }
          if ((*(byte *)(param_1 + 0x2ae0) & 2) == 0) {
            uVar9 = (**(code **)(*plVar18 + 0x78))(plVar18,1);
          }
          else {
            (**(code **)(*plVar18 + 0x78))(plVar18,iVar7 == 0);
            lVar10 = FUN_14034c770(plVar18,0);
            if ((lVar10 == 0) || (*(char *)(param_1 + 0x2ae4) != '\0')) {
              uVar12 = 1;
            }
            else {
              uVar12 = 0;
            }
            uVar9 = (**(code **)(*plVar18 + 0x80))(plVar18,uVar12);
          }
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 0x300);
    }
    else {
      iVar6 = *(int *)(*(longlong *)(param_1 + 0x2160) + 0xa8);
      if ((iVar6 - 0x2cbU < 0x14) || (iVar6 - 0x32fU < 10)) {
        iVar6 = 0x32f;
        do {
          plVar18 = (longlong *)FUN_140347b50(iVar6);
          if (plVar18 == *(longlong **)(param_1 + 0x2160)) {
            plVar19 = (longlong *)(**(code **)(*plVar18 + 0x2e0))(plVar18);
            if (plVar19 != (longlong *)0x0) {
              lVar10 = *plVar19;
              uVar12 = FUN_14034be00(plVar18,0);
              (**(code **)(lVar10 + 0xb0))(plVar19,uVar12);
            }
          }
          else if ((plVar18 != (longlong *)0x0) &&
                  (cVar5 = (**(code **)(*plVar18 + 0x2f8))(plVar18), cVar5 == '\0')) {
            (**(code **)(*plVar18 + 0x2e8))(plVar18,1,0);
            (**(code **)(*plVar18 + 0x80))(plVar18,0);
            (**(code **)(*plVar18 + 0x78))(plVar18,0);
          }
          iVar6 = iVar6 + 1;
        } while (iVar6 < 0x339);
        iVar6 = 0x2df;
        do {
          plVar18 = (longlong *)FUN_140347b50(iVar6);
          uVar9 = (**(code **)(*plVar18 + 0x80))(plVar18,1);
          iVar6 = iVar6 + 1;
        } while (iVar6 < 0x300);
      }
      else if (iVar6 - 0x2dfU < 0x21) {
        iVar6 = 0x2df;
        do {
          plVar18 = (longlong *)FUN_140347b50(iVar6);
          if (plVar18 == *(longlong **)(param_1 + 0x2160)) {
            plVar19 = (longlong *)(**(code **)(*plVar18 + 0x2e0))();
            if (plVar19 != (longlong *)0x0) {
              lVar10 = *plVar19;
              uVar12 = FUN_14034be00(plVar18,0);
              (**(code **)(lVar10 + 0xb0))(plVar19,uVar12);
            }
          }
          else {
            iVar7 = FUN_14034c770(plVar18,3);
            if (((plVar18 != (longlong *)0x0) &&
                (cVar5 = (**(code **)(*plVar18 + 0x2f8))(plVar18), cVar5 == '\0')) &&
               (iVar7 - 1U < 2)) {
              (**(code **)(*plVar18 + 0x2e8))(plVar18,1,0);
              (**(code **)(*plVar18 + 0x80))(plVar18,0);
              (**(code **)(*plVar18 + 0x78))(plVar18,0);
            }
          }
          iVar6 = iVar6 + 1;
        } while (iVar6 < 0x300);
        iVar6 = 0x32f;
        do {
          plVar18 = (longlong *)FUN_140347b50(iVar6);
          uVar9 = (**(code **)(*plVar18 + 0x80))(plVar18,1);
          iVar6 = iVar6 + 1;
        } while (iVar6 < 0x339);
      }
    }
    if (7 < local_98) {
      uVar24 = local_98 * 2 + 2;
      pppppppuVar27 = pppppppuVar26;
      if (0xfff < uVar24) {
        pppppppuVar27 = (undefined8 *******)pppppppuVar26[-1];
        uVar24 = local_98 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)pppppppuVar26 + (-8 - (longlong)pppppppuVar27)))
        goto LAB_1403e5a8e;
      }
      uVar9 = thunk_FUN_1407864b8(pppppppuVar27,uVar24);
    }
    if (7 < local_78) {
      pppppppuVar26 = pppppppuVar4;
      if (0xfff < local_78 * 2 + 2) {
        pppppppuVar26 = (undefined8 *******)pppppppuVar4[-1];
        uVar24 = local_78 * 2 + 0x29;
        if (0x1f < (ulonglong)((longlong)pppppppuVar4 + (-8 - (longlong)pppppppuVar26))) {
LAB_1403e5a8e:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(uVar9,uVar24);
        }
      }
      thunk_FUN_1407864b8(pppppppuVar26);
    }
    cVar5 = *(char *)((longlong)local_118[1] + 0x19);
    ppppuVar2 = local_118[1];
    while (cVar5 == '\0') {
      FUN_14010cb00(&local_118,&local_118,ppppuVar2[2]);
      ppppuVar13 = (undefined8 ****)*ppppuVar2;
      thunk_FUN_1407864b8(ppppuVar2,0x38);
      ppppuVar2 = ppppuVar13;
      cVar5 = *(char *)((longlong)ppppuVar13 + 0x19);
    }
    thunk_FUN_1407864b8(local_118,0x38);
    local_100 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    if (local_f8 != (LPVOID)0x0) {
      if ((local_f0 & 0x8000000000000000) == 0) {
        thunk_FUN_1407864b8();
      }
      else {
        pvVar20 = GetProcessHeap();
        HeapFree(pvVar20,0,local_f8);
      }
      local_f8 = (LPVOID)0x0;
      local_f0 = (ulonglong)(local_f0._4_4_ & 0x80000000) << 0x20;
    }
    local_e8 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
    if (local_e0 != (LPVOID)0x0) {
      if ((local_d4 & 0x80000000) == 0) {
        thunk_FUN_1407864b8();
      }
      else {
        pvVar20 = GetProcessHeap();
        HeapFree(pvVar20,0,local_e0);
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e6ff0  Name: FUN_1403e6ff0  Size: 2124 bytes
// ------------------------------------------------------------

void FUN_1403e6ff0(longlong param_1)

{
  float fVar1;
  char cVar2;
  longlong lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  longlong *plVar9;
  longlong lVar10;
  longlong *plVar11;
  undefined4 *puVar12;
  HANDLE hHeap;
  undefined1 *puVar13;
  undefined8 ****ppppuVar14;
  undefined8 *puVar15;
  undefined8 *****pppppuVar16;
  undefined8 uVar17;
  longlong lVar18;
  ulonglong uVar19;
  ulonglong uVar20;
  int iVar21;
  undefined8 uVar22;
  undefined8 uVar23;
  int iVar24;
  int local_res18;
  undefined8 in_stack_fffffffffffffe98;
  undefined4 uVar26;
  undefined ***pppuVar25;
  int local_158;
  undefined **local_150;
  LPVOID local_148;
  undefined8 local_140;
  int local_138;
  int local_134;
  undefined8 ***local_130;
  undefined4 uStack_128;
  undefined4 uStack_124;
  ulonglong local_120;
  ulonglong local_118;
  longlong local_110;
  longlong local_108;
  undefined8 ***local_100 [2];
  undefined8 local_f0;
  ulonglong local_e8;
  longlong local_e0;
  longlong local_d8;
  undefined8 ****local_d0 [2];
  longlong local_c0;
  ulonglong local_b8;
  undefined1 local_b0 [16];
  undefined1 local_a0 [16];
  short local_90 [40];
  
  if ((*(char *)(param_1 + 0x2b08) != '\0') && (lVar7 = FUN_140347b50(0x355), lVar7 != 0)) {
    lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x2b18) + 0xb8);
    local_110 = lVar7;
    lVar8 = FUN_140294a40(&DAT_140a16fd0);
    iVar24 = *(int *)(lVar7 + 0x178) + *(int *)(lVar7 + 0x160);
    local_138 = (int)(*(float *)(lVar8 + 0x70) + DAT_1408007c0);
    local_134 = iVar24;
    lVar8 = FUN_140294a40(&DAT_140a16fd0);
    local_res18 = 0;
    local_158 = 0;
    cVar2 = *(char *)(lVar8 + 0x449);
    local_d8 = (longlong)iVar24;
    iVar21 = 0;
    if (0 < iVar24) {
      local_108 = 0;
      do {
        lVar8 = local_108;
        iVar24 = local_158 - local_res18;
        plVar9 = (longlong *)FUN_140347b50(iVar24 + 0x35b);
        uVar26 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
        if (local_158 < *(int *)(lVar7 + 0x160)) {
          lVar8 = *(longlong *)(*(longlong *)(lVar7 + 0x158) + lVar8 * 8);
        }
        else {
          lVar8 = *(longlong *)
                   (*(longlong *)(lVar7 + 0x170) +
                   (longlong)(local_158 - *(int *)(lVar7 + 0x160)) * 8);
        }
        local_e0 = *(longlong *)(lVar8 + 0x298);
        if (cVar2 == '\0') {
          iVar21 = *(int *)(local_e0 + 0x870);
        }
        else {
          iVar21 = *(int *)(local_e0 + 0x874);
        }
        if (iVar21 < 1) {
          local_res18 = local_res18 + 1;
        }
        else {
          iVar6 = iVar21;
          if (cVar2 == '\0') {
            if ((*(int *)(param_1 + 0x2b10) != 0x3fffff) && (*(longlong *)(param_1 + 0x2b18) != 0))
            {
              lVar7 = FUN_1402aca60(*(undefined8 *)(DAT_140b15418 + 0x18));
              uVar26 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
              if ((lVar7 != 0) && (lVar3 = *(longlong *)(lVar7 + 0x100), lVar3 != 0)) {
                iVar6 = 0;
                if (0 < DAT_140b16818) {
                  lVar18 = 0;
                  do {
                    puVar15 = (undefined8 *)(DAT_140b16810 + lVar18);
                    if (0xf < (ulonglong)puVar15[3]) {
                      puVar15 = (undefined8 *)*puVar15;
                    }
                    lVar10 = FUN_14037bd90(lVar3 + 8,puVar15);
                    uVar26 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
                    if (lVar10 != 0) {
                      iVar6 = FUN_1405b1cc0(lVar10,lVar7,lVar8,*(undefined8 *)(param_1 + 0x2b18));
                      goto LAB_1403e720b;
                    }
                    iVar6 = iVar6 + 1;
                    lVar18 = lVar18 + 0x20;
                  } while (iVar6 < DAT_140b16818);
                }
              }
            }
            iVar6 = 0;
          }
LAB_1403e720b:
          lVar7 = local_e0;
          FUN_14001e680(local_d0,local_e0 + 0x1820);
          if (local_c0 == 0) {
            pppppuVar16 = (undefined8 *****)"i_button_temporary.tga";
          }
          else {
            pppppuVar16 = local_d0;
            if (0xf < local_b8) {
              pppppuVar16 = (undefined8 *****)local_d0[0];
            }
          }
          (**(code **)(*plVar9 + 0xa0))(plVar9,pppppuVar16,0);
          (**(code **)(*plVar9 + 0x78))(plVar9,0);
          FUN_140353ad0(plVar9,lVar8,0);
          FUN_140353ad0(plVar9,0xffffffffffffffff,1);
          lVar8 = FUN_14034c990(plVar9);
          (**(code **)(*plVar9 + 0x40))(plVar9,*(undefined4 *)(lVar8 + 4));
          iVar5 = local_138;
          if (local_138 < iVar6) {
            (**(code **)(*plVar9 + 0x80))(plVar9,1);
            FUN_1401f93e0(local_a0,0x80,0x80,0x80,CONCAT44(uVar26,0xff));
            (**(code **)(*plVar9 + 0x10))(plVar9,local_a0);
            puVar13 = local_a0;
          }
          else {
            (**(code **)(*plVar9 + 0x80))(plVar9,0);
            FUN_1401f93e0(local_b0,0xff,0xff,0xff,CONCAT44(uVar26,0xff));
            (**(code **)(*plVar9 + 0x10))(plVar9,local_b0);
            puVar13 = local_b0;
          }
          FUN_1401f9440(puVar13);
          plVar11 = (longlong *)FUN_140347b50(iVar24 + 0x365);
          local_100[0] = (undefined8 ****)0x0;
          local_f0 = 0;
          local_e8 = 7;
          FUN_140022b30(local_90,0x10,L"$ %d",iVar6);
          lVar8 = -1;
          do {
            lVar8 = lVar8 + 1;
          } while (local_90[lVar8] != 0);
          FUN_1400228a0(local_100,local_90);
          (**(code **)(*plVar11 + 0x148))(plVar11,local_100,1,0);
          (**(code **)(*plVar11 + 0x78))(plVar11,0);
          (**(code **)(*plVar11 + 0x80))(plVar11,0);
          (**(code **)(*plVar11 + 0xa0))(plVar11,0,0);
          bVar4 = true;
          if (iVar5 < iVar6) {
            (**(code **)(*plVar11 + 0x128))(plVar11,0xc0,0xc0);
            bVar4 = false;
          }
          else {
            if (iVar21 < iVar6) {
              uVar23 = 0;
              uVar17 = 0xf0;
              uVar22 = 0;
            }
            else {
              uVar23 = 0xff;
              uVar22 = 0xff;
              uVar17 = 0xff;
            }
            (**(code **)(*plVar11 + 0x128))(plVar11,uVar17,uVar22,uVar23,0xff,1);
          }
          FUN_140353ad0(plVar9,(longlong)iVar6,2);
          plVar9 = (longlong *)FUN_140347b50(iVar24 + 0x36f);
          local_150 = DynamicVectorClass<class_std::basic_string<wchar_t,struct_std::char_traits<wchar_t>,class_std::allocator<wchar_t>_>_>
                      ::vftable;
          local_148 = (LPVOID)0x0;
          local_140 = 0;
          puVar12 = (undefined4 *)FUN_14034c990(plVar9);
          puVar15 = (undefined8 *)(lVar7 + 0x140);
          uVar26 = *puVar12;
          fVar1 = (float)puVar12[1];
          local_130 = (undefined8 ***)0x0;
          local_120 = 0;
          local_118 = 0;
          uVar20 = *(ulonglong *)(lVar7 + 0x150);
          if (7 < *(ulonglong *)(lVar7 + 0x158)) {
            puVar15 = (undefined8 *)*puVar15;
          }
          if (uVar20 < 8) {
            local_130 = (undefined8 ***)*puVar15;
            uStack_128 = *(undefined4 *)(puVar15 + 1);
            uStack_124 = *(undefined4 *)((longlong)puVar15 + 0xc);
            local_118 = 7;
          }
          else {
            uVar19 = uVar20 | 7;
            if (0x7ffffffffffffffe < uVar19) {
              uVar19 = 0x7ffffffffffffffe;
            }
            local_130 = (undefined8 ***)FUN_1400226c0(&local_130,uVar19 + 1);
            FUN_14076bd80(local_130,puVar15,uVar20 * 2 + 2);
            local_118 = uVar19;
          }
          pppuVar25 = &local_150;
          local_120 = uVar20;
          (**(code **)(*plVar9 + 0x1c8))(plVar9,&local_130,uVar26,(int)fVar1,pppuVar25);
          (**(code **)(*plVar9 + 0x148))(plVar9,local_148,1,0);
          if ((int)local_140 < 2) {
            lVar7 = 0;
          }
          else {
            lVar7 = (longlong)local_148 + 0x20;
          }
          (**(code **)(*plVar9 + 0x148))(plVar9,lVar7,0,0);
          uVar17 = CONCAT71((int7)((ulonglong)pppuVar25 >> 8),0xff);
          if (bVar4) {
            (**(code **)(*plVar9 + 0x128))(plVar9,0xff,0xff,0xff,uVar17,1);
            uVar22 = 0xff;
          }
          else {
            (**(code **)(*plVar9 + 0x128))(plVar9,0xc0,0xc0,0xc0,uVar17,1);
            uVar22 = 0xc0;
          }
          in_stack_fffffffffffffe98 = CONCAT71((int7)((ulonglong)uVar17 >> 8),0xff);
          (**(code **)(*plVar9 + 0x128))(plVar9,uVar22,uVar22,uVar22,in_stack_fffffffffffffe98,0);
          if (7 < local_118) {
            uVar20 = local_118 * 2 + 2;
            ppppuVar14 = (undefined8 ****)local_130;
            if (0xfff < uVar20) {
              ppppuVar14 = (undefined8 ****)local_130[-1];
              uVar20 = local_118 * 2 + 0x29;
              if (0x1f < (ulonglong)((longlong)local_130 + (-8 - (longlong)ppppuVar14)))
              goto LAB_1403e7837;
            }
            thunk_FUN_1407864b8(ppppuVar14,uVar20);
          }
          local_120 = 0;
          local_118 = 7;
          local_130 = (undefined8 ***)((ulonglong)local_130 & 0xffffffffffff0000);
          local_150 = DynamicVectorClass<class_std::basic_string<wchar_t,struct_std::char_traits<wchar_t>,class_std::allocator<wchar_t>_>_>
                      ::vftable;
          if (local_148 != (LPVOID)0x0) {
            if ((local_140 & 0x8000000000000000) == 0) {
              FUN_14001f020(local_148,3);
            }
            else {
              hHeap = GetProcessHeap();
              HeapFree(hHeap,0,local_148);
            }
            local_148 = (LPVOID)0x0;
            local_140 = (ulonglong)(local_140._4_4_ & 0x80000000) << 0x20;
          }
          if (7 < local_e8) {
            uVar20 = local_e8 * 2 + 2;
            ppppuVar14 = (undefined8 ****)local_100[0];
            if (0xfff < uVar20) {
              ppppuVar14 = (undefined8 ****)local_100[0][-1];
              uVar20 = local_e8 * 2 + 0x29;
              if (0x1f < (ulonglong)((longlong)local_100[0] + (-8 - (longlong)ppppuVar14)))
              goto LAB_1403e7837;
            }
            thunk_FUN_1407864b8(ppppuVar14,uVar20);
          }
          local_f0 = 0;
          local_e8 = 7;
          local_100[0] = (undefined8 ***)((ulonglong)local_100[0] & 0xffffffffffff0000);
          lVar7 = local_110;
          if (0xf < local_b8) {
            if (0xfff < local_b8 + 1) {
              ppppuVar14 = (undefined8 ****)local_d0[0][-1];
              uVar20 = local_b8 + 0x28;
              if ((char *)0x1f < (char *)((longlong)local_d0[0] + (-8 - (longlong)ppppuVar14))) {
LAB_1403e7837:
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(ppppuVar14,uVar20);
              }
            }
            thunk_FUN_1407864b8();
            lVar7 = local_110;
          }
        }
        local_158 = local_158 + 1;
        local_108 = local_108 + 1;
        iVar24 = local_134;
        iVar21 = local_res18;
      } while (local_108 < local_d8);
    }
    iVar6 = FUN_140347af0(0x35b);
    iVar24 = iVar24 - iVar21;
    if (iVar24 < iVar6) {
      iVar21 = iVar24 + 0x365;
      uVar20 = (ulonglong)(uint)(iVar6 - iVar24);
      do {
        plVar9 = (longlong *)FUN_140347b50(iVar21 + -10);
        if (plVar9 != (longlong *)0x0) {
          (**(code **)(*plVar9 + 0x80))(plVar9,1);
          FUN_140353ad0(plVar9,0,0);
          FUN_140353ad0(plVar9,0xffffffffffffffff);
          (**(code **)(*plVar9 + 0x78))(plVar9,1);
          plVar9 = (longlong *)FUN_140347b50(iVar21);
          if (plVar9 != (longlong *)0x0) {
            (**(code **)(*plVar9 + 0x78))(plVar9,1);
            plVar9 = (longlong *)FUN_140347b50(iVar21 + 10);
            if (plVar9 != (longlong *)0x0) {
              (**(code **)(*plVar9 + 0x148))(plVar9,0,1,0);
              (**(code **)(*plVar9 + 0x148))(plVar9,0,0);
            }
          }
        }
        iVar21 = iVar21 + 1;
        uVar20 = uVar20 - 1;
      } while (uVar20 != 0);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e6630  Name: FUN_1403e6630  Size: 1928 bytes
// ------------------------------------------------------------

void FUN_1403e6630(longlong param_1)

{
  int iVar1;
  int iVar2;
  LPVOID lpMem;
  char cVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  HANDLE pvVar8;
  undefined1 *puVar9;
  int *piVar10;
  ulonglong uVar11;
  uint uVar12;
  bool local_res10;
  int local_res18;
  int local_res20 [2];
  int local_138;
  int local_134;
  int local_130;
  ulonglong local_128 [2];
  undefined8 local_118;
  ulonglong local_110;
  undefined8 local_108;
  undefined **local_100;
  LPVOID local_f8;
  ulonglong local_f0;
  longlong local_e8;
  undefined1 local_e0 [16];
  undefined1 local_d0 [16];
  undefined1 local_c0 [16];
  undefined1 local_b0 [16];
  undefined1 local_a0 [8];
  LPVOID local_98;
  int local_90;
  uint local_8c;
  short local_88 [36];
  
  if (*(char *)(param_1 + 0x2b58) != '\0') {
    local_108 = FUN_140294a40(&DAT_140a16fd0);
    if (*(char *)(param_1 + 0x2b44) == '\0') {
      lVar4 = FUN_1402aca60(*(undefined8 *)(DAT_140b15418 + 0x18),*(undefined4 *)(param_1 + 0x2b40))
      ;
      if (lVar4 == 0) {
        return;
      }
      local_108 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(lVar4 + 0x58));
    }
    if (0 < *(int *)(param_1 + 0x2b38)) {
      local_130 = 0x3a2;
      local_res18 = 0;
      local_134 = 0x38c;
      local_e8 = 0;
      do {
        iVar2 = local_130;
        iVar1 = local_134;
        uVar12 = 0;
        piVar10 = (int *)(*(longlong *)(param_1 + 0x2b30) + local_e8);
        if (*piVar10 != -1) {
          plVar5 = (longlong *)FUN_140347b50(local_134 + -3);
          plVar6 = (longlong *)FUN_140347b50(iVar1);
          lVar4 = FUN_140294a40(&DAT_140a16fd0);
          local_res10 = *(float *)(lVar4 + 0x70) < (float)piVar10[1];
          if (local_res10) {
            (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0,0,0xff,1);
          }
          else {
            (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0xff,0xff,0xff,1);
          }
          local_res10 = !local_res10;
          FUN_1401f93e0(local_d0,0xff,0xff,0xff,0xff);
          FUN_1401f93e0(local_e0,0x80,0x80,0x80,0xff);
          if (piVar10[9] != -1) {
            plVar6 = (longlong *)FUN_140347b50(iVar2 + -1);
            plVar7 = (longlong *)FUN_140347b50(iVar2);
            iVar1 = piVar10[10];
            cVar3 = FUN_1405237b0(piVar10,local_108,local_res20);
            iVar2 = iVar1;
            if (local_res20[0] < iVar1) {
              iVar2 = local_res20[0];
            }
            local_res20[0] = iVar2;
            FUN_140022b30(local_88,0x20,L"%d/%d",iVar2,iVar1);
            local_110 = 7;
            local_128[0] = 0;
            local_118 = 0;
            lVar4 = -1;
            do {
              lVar4 = lVar4 + 1;
            } while (local_88[lVar4] != 0);
            FUN_1400228a0(local_128,local_88);
            (**(code **)(*plVar7 + 0x148))(plVar7,local_128,1,0);
            if (cVar3 == '\0') {
              (**(code **)(*plVar7 + 0x128))(plVar7,0xff,0,0,0xff,1);
              (**(code **)(*plVar6 + 0x10))(plVar6,local_e0);
              local_res10 = false;
            }
            else {
              (**(code **)(*plVar7 + 0x128))(plVar7,0xff,0xff,0xff,0xff,1);
              (**(code **)(*plVar6 + 0x10))(plVar6,local_d0);
            }
            (**(code **)(*plVar7 + 0x78))(plVar7,0);
            uVar12 = 1;
            if (7 < local_110) {
              if (0xfff < local_110 * 2 + 2) {
                if (0x1f < (local_128[0] - *(longlong *)(local_128[0] - 8)) - 8) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(*(longlong *)(local_128[0] - 8),local_110 * 2 + 0x29);
                }
              }
              thunk_FUN_1407864b8();
            }
          }
          lVar4 = FUN_1405235b0(piVar10);
          if (lVar4 != 0) {
            iVar1 = (local_res18 + uVar12) * 4;
            plVar6 = (longlong *)FUN_140347b50(iVar1 + 0x3a1);
            plVar7 = (longlong *)FUN_140347b50(iVar1 + 0x3a2);
            iVar1 = piVar10[8];
            cVar3 = FUN_140523950(piVar10,local_108,&local_138);
            iVar2 = iVar1;
            if (local_138 < iVar1) {
              iVar2 = local_138;
            }
            local_138 = iVar2;
            FUN_140022b30(local_88,0x20,L"%d/%d",iVar2,iVar1);
            local_110 = 7;
            local_128[0] = 0;
            lVar4 = -1;
            local_118 = 0;
            do {
              lVar4 = lVar4 + 1;
            } while (local_88[lVar4] != 0);
            FUN_1400228a0(local_128,local_88);
            (**(code **)(*plVar7 + 0x148))(plVar7,local_128,1,0);
            if (cVar3 == '\0') {
              (**(code **)(*plVar7 + 0x128))(plVar7,0xff,0,0,0xff,1);
              (**(code **)(*plVar6 + 0x10))(plVar6,local_e0);
              local_res10 = false;
            }
            else {
              (**(code **)(*plVar7 + 0x128))(plVar7,0xff,0xff,0xff,0xff,1);
              (**(code **)(*plVar6 + 0x10))(plVar6,local_d0);
            }
            (**(code **)(*plVar7 + 0x78))(plVar7,0);
            uVar12 = uVar12 + 1;
            if (7 < local_110) {
              if (0xfff < local_110 * 2 + 2) {
                if (0x1f < (local_128[0] - *(longlong *)(local_128[0] - 8)) - 8) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(*(longlong *)(local_128[0] - 8),local_110 * 2 + 0x29);
                }
              }
              thunk_FUN_1407864b8();
            }
          }
          if (uVar12 < 2) {
            FUN_1402d95e0(local_a0,piVar10 + 0xc);
            if (0 < local_90) {
              plVar6 = (longlong *)FUN_140347b50(local_90 + 0x3a1 + (local_res18 + uVar12) * 4);
              local_100 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
              local_f8 = (LPVOID)0x0;
              local_f0 = 0;
              cVar3 = FUN_140523810(piVar10,local_108,&local_100);
              uVar11 = 1;
              if ((int)local_f0 < 1) {
                uVar11 = local_f0 & 0xffffffff;
              }
              FUN_140022b30(local_88,0x20,L"%d/1",uVar11);
              local_128[0] = 0;
              lVar4 = -1;
              local_118 = 0;
              local_110 = 7;
              do {
                lVar4 = lVar4 + 1;
              } while (local_88[lVar4] != 0);
              FUN_1400228a0(local_128,local_88);
              (**(code **)(*plVar6 + 0x148))(plVar6,local_128,1,0);
              if (cVar3 == '\0') {
                (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0,0,0xff,1);
                local_res10 = false;
              }
              else {
                (**(code **)(*plVar6 + 0x128))(plVar6,0xff,0xff,0xff,0xff,1);
              }
              (**(code **)(*plVar6 + 0x78))(plVar6,0);
              if (7 < local_110) {
                if (0xfff < local_110 * 2 + 2) {
                  if (0x1f < (local_128[0] - *(longlong *)(local_128[0] - 8)) - 8) {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(*(longlong *)(local_128[0] - 8),local_110 * 2 + 0x29);
                  }
                }
                thunk_FUN_1407864b8();
              }
              local_118 = 0;
              local_110 = 7;
              local_128[0] = local_128[0] & 0xffffffffffff0000;
              local_100 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
              if (local_f8 != (LPVOID)0x0) {
                if ((local_f0 & 0x8000000000000000) == 0) {
                  thunk_FUN_1407864b8();
                }
                else {
                  pvVar8 = GetProcessHeap();
                  HeapFree(pvVar8,0,local_f8);
                }
              }
            }
            lpMem = local_98;
            if (local_98 != (LPVOID)0x0) {
              if ((local_8c & 0x80000000) == 0) {
                thunk_FUN_1407864b8(local_98);
              }
              else {
                pvVar8 = GetProcessHeap();
                HeapFree(pvVar8,0,lpMem);
              }
            }
          }
          if (local_res10) {
            (**(code **)(*plVar5 + 0x80))(plVar5,*(char *)(param_1 + 0x2b44) != '\0');
            FUN_1401f93e0(local_c0);
            (**(code **)(*plVar5 + 0x10))(plVar5,local_c0);
            puVar9 = local_c0;
          }
          else {
            FUN_1401f93e0(local_b0);
            (**(code **)(*plVar5 + 0x10))(plVar5,local_b0);
            puVar9 = local_b0;
          }
          FUN_1401f9440(puVar9);
          FUN_1401f9440(local_e0);
          FUN_1401f9440(local_d0);
        }
        local_130 = local_130 + 8;
        local_res18 = local_res18 + 2;
        local_e8 = local_e8 + 0x68;
        iVar1 = local_134 + -0x38b;
        local_134 = local_134 + 1;
      } while (iVar1 < *(int *)(param_1 + 0x2b38));
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x3e79b0  Name: FUN_1403e79b0  Size: 212 bytes
// ------------------------------------------------------------

void FUN_1403e79b0(longlong param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined1 uVar4;
  
  if (*(char *)(param_1 + 0x2b70) != '\0') {
    plVar1 = (longlong *)FUN_140347b50(0x3c3);
    plVar2 = (longlong *)FUN_140347b50(0x3c2);
    lVar3 = FUN_140294a40(&DAT_140a16fd0);
    if (DAT_140b27940 < *(float *)(lVar3 + 0x70) || DAT_140b27940 == *(float *)(lVar3 + 0x70)) {
      (**(code **)(*plVar2 + 0x128))(plVar2,0xff,0xff,0xff,0xff,1);
      (**(code **)(*plVar1 + 0x80))(plVar1,0);
      uVar4 = 0xff;
    }
    else {
      (**(code **)(*plVar2 + 0x128))(plVar2,0xa0,0xa0);
      (**(code **)(*plVar1 + 0x80))(plVar1,1);
      uVar4 = 0x40;
    }
    (**(code **)(*plVar1 + 0x128))(plVar1,uVar4,uVar4,uVar4,0xff,1);
  }
  return;
}




// ================================================================
// SPACE BATTLE SUB-TICKS (MODE-2)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x317480  Name: FUN_140317480  Size: 2161 bytes
// ------------------------------------------------------------

void FUN_140317480(longlong param_1)

{
  undefined4 uVar1;
  char cVar2;
  undefined1 uVar3;
  int iVar4;
  undefined8 uVar5;
  byte *pbVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  longlong *plVar9;
  longlong lVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  undefined8 *puVar14;
  undefined8 ****ppppuVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  undefined4 uVar20;
  undefined8 ***local_c8 [2];
  longlong local_b8;
  ulonglong local_b0;
  longlong local_a8 [2];
  undefined8 local_98;
  ulonglong local_90;
  short local_88 [40];
  
  lVar12 = *(longlong *)(DAT_140b15418 + 0x18);
  uVar5 = FUN_140294a40(&DAT_140a16fd0);
  iVar17 = 0;
  if (0 < *(int *)(param_1 + 0x1140)) {
    lVar13 = 0;
    do {
      FUN_140353ad0(*(undefined8 *)(lVar13 + *(longlong *)(param_1 + 0x1138)),1,1);
      iVar17 = iVar17 + 1;
      lVar13 = lVar13 + 8;
    } while (iVar17 < *(int *)(param_1 + 0x1140));
  }
  iVar17 = 0;
  if (0 < *(int *)(param_1 + 0x1158)) {
    lVar13 = 0;
    do {
      FUN_140353ad0(*(undefined8 *)(lVar13 + *(longlong *)(param_1 + 0x1150)),1,1);
      iVar17 = iVar17 + 1;
      lVar13 = lVar13 + 8;
    } while (iVar17 < *(int *)(param_1 + 0x1158));
  }
  uVar1 = DAT_1407ffaf8;
  iVar17 = 0;
  for (lVar13 = *(longlong *)(lVar12 + 0x388); lVar13 != lVar12 + 0x380;
      lVar13 = *(longlong *)(lVar13 + 8)) {
    lVar11 = *(longlong *)(lVar13 + 0x18);
    pbVar6 = (byte *)(lVar11 + 0x388);
    if (lVar11 == 0) {
      pbVar6 = &DAT_000003a0;
    }
    if ((*pbVar6 & 2) == 0) {
      puVar7 = (undefined4 *)(lVar11 + 0x40);
      if (lVar11 == 0) {
        puVar7 = (undefined4 *)&DAT_00000058;
      }
      uVar8 = FUN_140294bc0(&DAT_140a16fd0,*puVar7);
      cVar2 = FUN_1402823e0(uVar8,uVar5);
      if (cVar2 != '\0') {
        plVar9 = (longlong *)(lVar11 + 0x280);
        if (lVar11 == 0) {
          plVar9 = (longlong *)&DAT_00000298;
        }
        lVar11 = *plVar9;
        if (*(char *)(lVar11 + 0xca) != '\0') {
          FUN_14001e680(local_c8,lVar11 + 0x1820);
          if (local_b8 == 0) {
            FUN_140022730(local_c8,"missing.tga",0xb);
          }
          cVar2 = *(char *)(lVar11 + 0xcb);
          plVar9 = (longlong *)FUN_1402deba0(param_1,lVar11,0,cVar2 == '\0');
          if (plVar9 != (longlong *)0x0) {
            ppppuVar15 = local_c8;
            if (0xf < local_b0) {
              ppppuVar15 = (undefined8 ****)local_c8[0];
            }
            (**(code **)(*plVar9 + 0xa0))(plVar9,ppppuVar15,0);
            lVar11 = *plVar9;
            lVar10 = FUN_14034c990(plVar9);
            (**(code **)(lVar11 + 0x40))(plVar9,*(undefined4 *)(lVar10 + 4));
            (**(code **)(*plVar9 + 0xf0))(plVar9,uVar1,0);
            (**(code **)(*plVar9 + 0x148))(plVar9,0,1,0);
            FUN_140353ad0(plVar9);
            if (cVar2 == '\0') {
              (**(code **)(*plVar9 + 0xb0))(plVar9,0);
              lVar11 = FUN_14034c770(plVar9,3);
              if (lVar11 == -1) {
                iVar16 = *(int *)(param_1 + 0x27b8);
                while (iVar16 <= iVar17) {
                  FUN_1402e0370(param_1);
                  iVar16 = *(int *)(param_1 + 0x27b8);
                }
                FUN_140353ad0(plVar9,(longlong)iVar17);
              }
              iVar17 = iVar17 + 1;
            }
          }
          if (0xf < local_b0) {
            if (0xfff < local_b0 + 1) {
              if (0x1f < (ulonglong)((longlong)local_c8[0] + (-8 - (longlong)local_c8[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(local_c8[0][-1],local_b0 + 0x28);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
      }
    }
  }
  iVar18 = 0;
  iVar16 = (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3);
  if (0 < iVar16) {
    do {
      lVar12 = FUN_140294bc0(&DAT_140a16fd0,iVar18);
      cVar2 = FUN_1402823e0(lVar12,uVar5);
      if ((cVar2 != '\0') && (lVar12 = *(longlong *)(lVar12 + 0x3a0), lVar12 != 0)) {
        iVar19 = 2;
        do {
          iVar4 = FUN_140497510(lVar12,iVar19);
          if (0 < iVar4) {
            lVar13 = FUN_140497580(lVar12,iVar19,0);
            cVar2 = FUN_140370320(lVar13);
            if ((cVar2 != '\0') && (*(char *)(lVar13 + 0xc9) != '\0')) {
              FUN_14001e680(local_c8,lVar13 + 0x1820);
              if (local_b8 == 0) {
                FUN_140022730(local_c8,"missing.tga");
              }
              if (*(longlong *)(lVar13 + 0xa60) == 0) {
                plVar9 = (longlong *)FUN_1402deba0(param_1,lVar13,1,0);
              }
              else {
                plVar9 = (longlong *)FUN_1402deba0(param_1,*(longlong *)(lVar13 + 0xa60),0,0);
                if (plVar9 != (longlong *)0x0) {
                  FUN_140353ad0(plVar9,lVar13);
                }
              }
              if (plVar9 != (longlong *)0x0) {
                ppppuVar15 = local_c8;
                if (0xf < local_b0) {
                  ppppuVar15 = (undefined8 ****)local_c8[0];
                }
                (**(code **)(*plVar9 + 0xa0))(plVar9,ppppuVar15,0);
                lVar13 = *plVar9;
                lVar11 = FUN_14034c990(plVar9);
                (**(code **)(lVar13 + 0x40))(plVar9,*(undefined4 *)(lVar11 + 4));
                uVar20 = FUN_140497540(lVar12,iVar19);
                (**(code **)(*plVar9 + 0xf0))(plVar9,uVar20,0);
                FUN_140022b30(local_88,0x10,L"%d%%");
                local_90 = 7;
                local_a8[0] = 0;
                lVar13 = -1;
                local_98 = 0;
                do {
                  lVar13 = lVar13 + 1;
                } while (local_88[lVar13] != 0);
                FUN_1400228a0(local_a8,local_88);
                (**(code **)(*plVar9 + 0x148))(plVar9,local_a8);
                if (7 < local_90) {
                  if (0xfff < local_90 * 2 + 2) {
                    if (0x1f < (local_a8[0] - *(longlong *)(local_a8[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(*(longlong *)(local_a8[0] + -8),local_90 * 2 + 0x29);
                    }
                  }
                  thunk_FUN_1407864b8();
                }
              }
              if (0xf < local_b0) {
                if (0xfff < local_b0 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_c8[0] + (-8 - (longlong)local_c8[0][-1])))
                  {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_c8[0][-1],local_b0 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
            }
          }
          iVar19 = iVar19 + 1;
        } while (iVar19 < 4);
      }
      iVar18 = iVar18 + 1;
    } while (iVar18 < iVar16);
  }
  FUN_1402ded90(param_1);
  if ((*(int *)(param_1 + 0x211c) == 3) &&
     (puVar14 = (undefined8 *)FUN_140468860(&DAT_140b30b40), puVar14[2] != 0)) {
    if (*(char *)(param_1 + 0x1160) == '\0') {
      uVar5 = FUN_140347b50(0x232);
      plVar9 = (longlong *)FUN_14034b350(uVar5,0,0,0);
      iVar16 = *(int *)(param_1 + 0x1140);
      iVar18 = *(int *)(param_1 + 0x1144) * 2 >> 1;
      if (iVar18 <= iVar16) {
        iVar19 = iVar18 / 10;
        if (iVar18 / 10 < 10) {
          iVar19 = 10;
        }
        FUN_140047aa0(param_1 + 0x1130,iVar19 + iVar18,0,
                      CONCAT71((uint7)(uint3)((uint)iVar16 >> 8),1));
        iVar16 = *(int *)(param_1 + 0x1140);
      }
      if (0 < iVar16) {
        FUN_14025c3c0(param_1 + 0x1130,1);
      }
      **(undefined8 **)(param_1 + 0x1138) = plVar9;
      *(int *)(param_1 + 0x1140) = *(int *)(param_1 + 0x1140) + 1;
    }
    else {
      plVar9 = (longlong *)**(undefined8 **)(param_1 + 0x1138);
    }
    if (0xf < (ulonglong)puVar14[3]) {
      puVar14 = (undefined8 *)*puVar14;
    }
    (**(code **)(*plVar9 + 0xa0))(plVar9,puVar14,0);
    lVar12 = *plVar9;
    lVar13 = FUN_14034c990(plVar9);
    (**(code **)(lVar12 + 0x40))(plVar9,*(undefined4 *)(lVar13 + 4));
    (**(code **)(*plVar9 + 0xf0))(plVar9,uVar1,0);
    (**(code **)(*plVar9 + 0x148))(plVar9,0,1,0);
    FUN_140353ad0(plVar9,0,1);
    FUN_140353ad0(plVar9,0,2);
    lVar12 = *plVar9;
    uVar5 = FUN_14034be00(plVar9,3);
    (**(code **)(lVar12 + 0xb0))(plVar9,uVar5);
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  *(undefined1 *)(param_1 + 0x1160) = uVar3;
  iVar16 = *(int *)(param_1 + 0x1140) + -1;
  if (-1 < iVar16) {
    lVar12 = (longlong)iVar16 * 8;
    do {
      lVar13 = *(longlong *)(lVar12 + *(longlong *)(param_1 + 0x1138));
      iVar18 = FUN_14034c770(lVar13,1);
      if (iVar18 == 1) {
        if (lVar13 == *(longlong *)(param_1 + 0x21a0)) {
          *(undefined8 *)(param_1 + 0x21a0) = 0;
        }
        if (iVar16 < *(int *)(param_1 + 0x1140)) {
          FUN_14025c3c0(param_1 + 0x1130,iVar16,iVar16 + 1,
                        (*(int *)(param_1 + 0x1140) - iVar16) + -1);
          *(int *)(param_1 + 0x1140) = *(int *)(param_1 + 0x1140) + -1;
        }
        if (lVar13 != 0) {
          FUN_14034a770(lVar13);
          thunk_FUN_1407864b8(lVar13,200);
        }
      }
      lVar12 = lVar12 + -8;
      iVar16 = iVar16 + -1;
    } while (-1 < iVar16);
  }
  iVar16 = *(int *)(param_1 + 0x1158) + -1;
  if (-1 < iVar16) {
    lVar12 = (longlong)iVar16 * 8;
    do {
      lVar13 = *(longlong *)(lVar12 + *(longlong *)(param_1 + 0x1150));
      iVar18 = FUN_14034c770(lVar13,1);
      if (iVar18 == 1) {
        if (lVar13 == *(longlong *)(param_1 + 0x21a0)) {
          *(undefined8 *)(param_1 + 0x21a0) = 0;
        }
        if (iVar16 < *(int *)(param_1 + 0x1158)) {
          FUN_14025c3c0(param_1 + 0x1148,iVar16,iVar16 + 1,
                        (*(int *)(param_1 + 0x1158) - iVar16) + -1);
          *(int *)(param_1 + 0x1158) = *(int *)(param_1 + 0x1158) + -1;
        }
        if (lVar13 != 0) {
          FUN_14034a770(lVar13);
          thunk_FUN_1407864b8(lVar13,200);
        }
      }
      lVar12 = lVar12 + -8;
      iVar16 = iVar16 + -1;
    } while (-1 < iVar16);
  }
  if (iVar17 < *(int *)(param_1 + 0x27b8)) {
    lVar12 = (longlong)iVar17 * 8;
    do {
      FUN_140264c80(*(undefined8 *)(lVar12 + *(longlong *)(param_1 + 0x27b0)),1);
      iVar17 = iVar17 + 1;
      lVar12 = lVar12 + 8;
    } while (iVar17 < *(int *)(param_1 + 0x27b8));
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x317d00  Name: FUN_140317d00  Size: 2579 bytes
// ------------------------------------------------------------

void FUN_140317d00(longlong param_1)

{
  undefined8 *puVar1;
  bool bVar2;
  char cVar3;
  char cVar4;
  byte bVar5;
  char cVar6;
  int iVar7;
  undefined4 uVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  longlong *plVar12;
  void *pvVar13;
  ulonglong uVar14;
  undefined8 uVar15;
  longlong *plVar16;
  longlong *plVar17;
  undefined8 *puVar18;
  longlong lVar19;
  ulonglong uVar20;
  uint uVar21;
  float fVar22;
  float fVar23;
  ulonglong local_res18;
  undefined8 local_88;
  undefined4 local_80;
  
  plVar17 = DAT_140b15418;
  plVar16 = (longlong *)0x0;
  if ((DAT_140b15418 != (longlong *)0x0) &&
     (iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar7 == 2)) {
    plVar16 = plVar17;
  }
  cVar3 = FUN_14035f790(DAT_140b15418);
  lVar9 = FUN_14028aa00(&DAT_140b153e0,plVar17);
  lVar10 = FUN_140294a40(&DAT_140a16fd0);
  uVar14 = 0;
  local_res18 = 0;
  iVar7 = -1;
  lVar11 = *(longlong *)(*(longlong *)(lVar10 + 0x68) + 0x2b08);
  uVar20 = uVar14;
  if (lVar11 == 0) {
    if (*(char *)(*(longlong *)(lVar10 + 0x68) + 0x10b) == '\x01') {
      local_88 = 0;
      local_80 = 0;
      lVar11 = FUN_1402a9ff0(DAT_140b15418[3],0x5c,0xffffffff,0xffffffff,&local_88,0,0x7fffffff,
                             0xffffffff);
      if (0 < *(int *)(lVar11 + 0x10)) {
        lVar19 = 0;
        do {
          uVar20 = *(ulonglong *)(lVar19 + *(longlong *)(lVar11 + 8));
          if ((uVar20 != 0) && (cVar4 = FUN_14039a2c0(uVar20,lVar10,0), cVar4 == '\x01')) break;
          uVar21 = (int)uVar14 + 1;
          uVar14 = (ulonglong)uVar21;
          lVar19 = lVar19 + 8;
          uVar20 = local_res18;
        } while ((int)uVar21 < *(int *)(lVar11 + 0x10));
      }
    }
  }
  else {
    iVar7 = *(int *)(lVar11 + 0x1de4);
  }
  bVar2 = false;
  lVar11 = FUN_14028a890(&DAT_140b153e0);
  if ((lVar11 != 0) && (bVar2 = false, *(int *)(lVar11 + 0x58) == *(int *)(lVar10 + 0x4c))) {
    bVar2 = true;
  }
  if ((lVar9 == 0) || (bVar2)) {
    plVar12 = (longlong *)FUN_140347b50(0xe5);
    uVar14 = 0;
    if (-1 < iVar7) {
      uVar14 = FUN_14035e4d0(plVar17,*(undefined4 *)(lVar10 + 0x4c),iVar7);
    }
    lVar11 = 0;
    if (plVar17 != (longlong *)0x0) {
      lVar11 = (**(code **)(*plVar17 + 0x220))(plVar17,*(undefined4 *)(lVar10 + 0x4c));
    }
    cVar4 = '\x01';
    if (lVar11 != 0) {
      cVar4 = FUN_140527620(lVar11);
    }
    if ((((uVar14 == 0) && (uVar20 == 0)) || (cVar3 != '\0')) || (cVar4 == '\0')) {
      (**(code **)(*plVar12 + 0x80))(plVar12,1);
      FUN_140353ad0(plVar12,0,0);
      fVar23 = DAT_1407ffaf8;
      (**(code **)(*plVar12 + 0xf0))(plVar12,DAT_1407ffaf8,0);
    }
    else {
      if (uVar14 == 0) {
        lVar11 = FUN_140394e60(uVar20);
        if ((lVar11 == 0) ||
           ((((*(longlong *)(lVar11 + 0x20) != 0 &&
              (*(char *)(*(longlong *)(lVar11 + 0x20) + 0x4d) == '\x01')) &&
             (*(float *)(lVar11 + 0x28) <= 0.0)) || (*(char *)(lVar11 + 0x6e) != '\0')))) {
          fVar22 = 0.0;
        }
        else {
          uVar15 = FUN_140294a40(&DAT_140a16fd0);
          fVar22 = (float)FUN_1402814e0(uVar15,lVar11);
        }
      }
      else {
        fVar22 = (float)FUN_14035e510(plVar16,*(undefined4 *)(lVar10 + 0x4c),iVar7);
        uVar20 = uVar14;
      }
      FUN_140353ad0(plVar12,uVar20,0);
      fVar23 = DAT_1407ffaf8;
      (**(code **)(*plVar12 + 0x80))(plVar12,fVar22 < DAT_1407ffaf8);
      if (fVar22 <= 0.0) {
        fVar22 = fVar23;
      }
      (**(code **)(*plVar12 + 0xf0))(plVar12,fVar22,0);
    }
    lVar11 = FUN_140294a40(&DAT_140a16fd0);
    pvVar13 = (void *)(param_1 + 0x2708);
    puVar1 = *(undefined8 **)(lVar11 + 0x68);
    if (0xf < *(ulonglong *)(param_1 + 0x2720)) {
      pvVar13 = *(void **)(param_1 + 0x2708);
    }
    puVar18 = puVar1;
    if (0xf < (ulonglong)puVar1[3]) {
      puVar18 = (undefined8 *)*puVar1;
    }
    if ((puVar1[2] == *(size_t *)(param_1 + 0x2718)) &&
       (iVar7 = memcmp(puVar18,pvVar13,puVar1[2]), iVar7 == 0)) {
      uVar8 = 0;
    }
    else {
      lVar11 = FUN_140294a40(&DAT_140a16fd0);
      pvVar13 = (void *)(param_1 + 0x2728);
      puVar1 = *(undefined8 **)(lVar11 + 0x68);
      if (0xf < *(ulonglong *)(param_1 + 0x2740)) {
        pvVar13 = *(void **)(param_1 + 0x2728);
      }
      puVar18 = puVar1;
      if (0xf < (ulonglong)puVar1[3]) {
        puVar18 = (undefined8 *)*puVar1;
      }
      if ((puVar1[2] == *(size_t *)(param_1 + 0x2738)) &&
         (iVar7 = memcmp(puVar18,pvVar13,puVar1[2]), iVar7 == 0)) {
        uVar8 = 1;
      }
      else {
        lVar11 = FUN_140294a40(&DAT_140a16fd0);
        pvVar13 = (void *)(param_1 + 0x2748);
        puVar1 = *(undefined8 **)(lVar11 + 0x68);
        if (0xf < *(ulonglong *)(param_1 + 0x2760)) {
          pvVar13 = *(void **)(param_1 + 0x2748);
        }
        puVar18 = puVar1;
        if (0xf < (ulonglong)puVar1[3]) {
          puVar18 = (undefined8 *)*puVar1;
        }
        if (puVar1[2] == *(size_t *)(param_1 + 0x2758)) {
          iVar7 = memcmp(puVar18,pvVar13,puVar1[2]);
          uVar8 = 2;
          if (iVar7 == 0) goto LAB_14031824d;
        }
LAB_140318248:
        uVar8 = 3;
      }
    }
  }
  else {
    plVar12 = (longlong *)FUN_140347b50(0xe5);
    (**(code **)(*plVar12 + 0x80))(plVar12,1);
    FUN_140353ad0(plVar12,0,0);
    fVar23 = DAT_1407ffaf8;
    (**(code **)(*plVar12 + 0xf0))(plVar12,DAT_1407ffaf8,0);
    lVar11 = FUN_140294a40(&DAT_140a16fd0);
    puVar1 = *(undefined8 **)(lVar11 + 0x68);
    pvVar13 = (void *)(param_1 + 0x2708);
    if (0xf < *(ulonglong *)(param_1 + 0x2720)) {
      pvVar13 = *(void **)(param_1 + 0x2708);
    }
    puVar18 = puVar1;
    if (0xf < (ulonglong)puVar1[3]) {
      puVar18 = (undefined8 *)*puVar1;
    }
    if ((puVar1[2] == *(size_t *)(param_1 + 0x2718)) &&
       (iVar7 = memcmp(puVar18,pvVar13,puVar1[2]), iVar7 == 0)) {
      uVar8 = 0;
    }
    else {
      lVar11 = FUN_140294a40(&DAT_140a16fd0);
      puVar1 = *(undefined8 **)(lVar11 + 0x68);
      pvVar13 = (void *)(param_1 + 0x2728);
      if (0xf < *(ulonglong *)(param_1 + 0x2740)) {
        pvVar13 = *(void **)(param_1 + 0x2728);
      }
      puVar18 = puVar1;
      if (0xf < (ulonglong)puVar1[3]) {
        puVar18 = (undefined8 *)*puVar1;
      }
      if ((puVar1[2] == *(size_t *)(param_1 + 0x2738)) &&
         (iVar7 = memcmp(puVar18,pvVar13,puVar1[2]), iVar7 == 0)) {
        uVar8 = 1;
      }
      else {
        lVar11 = FUN_140294a40(&DAT_140a16fd0);
        pvVar13 = (void *)(param_1 + 0x2748);
        puVar1 = *(undefined8 **)(lVar11 + 0x68);
        if (0xf < *(ulonglong *)(param_1 + 0x2760)) {
          pvVar13 = *(void **)(param_1 + 0x2748);
        }
        puVar18 = puVar1;
        if (0xf < (ulonglong)puVar1[3]) {
          puVar18 = (undefined8 *)*puVar1;
        }
        if ((puVar1[2] != *(size_t *)(param_1 + 0x2758)) ||
           (iVar7 = memcmp(puVar18,pvVar13,puVar1[2]), iVar7 != 0)) goto LAB_140318248;
        uVar8 = 2;
      }
    }
  }
LAB_14031824d:
  lVar11 = *plVar12;
  uVar15 = FUN_14034c200(plVar12,uVar8);
  (**(code **)(lVar11 + 0xa0))(plVar12,uVar15,0);
  plVar16 = (longlong *)FUN_140347b50(0xe6);
  FUN_140353ad0(plVar16,0,0);
  lVar11 = *plVar16;
  uVar15 = FUN_14034c340(plVar16);
  (**(code **)(lVar11 + 0xa0))(plVar16,uVar15,0);
  cVar4 = FUN_14028afb0(&DAT_140b153e0);
  if ((cVar4 == '\0') || ((0 < *(int *)(param_1 + 0x2068) && (*(char *)(param_1 + 0x2065) == '\0')))
     ) {
    uVar15 = 1;
  }
  else {
    uVar15 = 0;
  }
  (**(code **)(*plVar16 + 0x80))(plVar16,uVar15);
  cVar4 = FUN_14028afb0(&DAT_140b153e0);
  if (((cVar4 == '\0') || (*(int *)(lVar10 + 0x2d0) == 0)) &&
     (cVar4 = FUN_1402f8b30(param_1,lVar10,plVar17), cVar4 == '\0')) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  plVar17 = (longlong *)FUN_140347b50(0xe7);
  if ((((DAT_140b15418 == (longlong *)0x0) || (DAT_140b15418[5] == 0)) ||
      (*(char *)(DAT_140b15418[5] + 0x20) == '\0')) &&
     ((*(char *)(lVar10 + 0x39c) != '\0' && (bVar2)))) {
    uVar15 = 0;
  }
  else {
    uVar15 = 1;
  }
  (**(code **)(*plVar17 + 0x80))(plVar17,uVar15);
  FUN_140353ad0(plVar17,0,0);
  lVar11 = FUN_140347b50(0x218);
  if ((lVar11 == 0) || (cVar4 = FUN_14034ce60(lVar11), cVar4 != '\0')) {
    bVar5 = *(byte *)(plVar17 + 0x18);
    if ((bVar5 & 4) != 0) goto LAB_140318396;
  }
  else {
    bVar5 = *(byte *)(plVar17 + 0x18);
LAB_140318396:
    *(undefined4 *)((longlong)plVar17 + 0xb4) = 0;
    *(byte *)(plVar17 + 0x18) = bVar5 & 0xef;
    *(undefined4 *)((longlong)plVar17 + 0xac) = 0;
  }
  plVar17 = (longlong *)FUN_140347b50(0xe8);
  bVar5 = FUN_140446480(&DAT_140b30350,*(undefined8 *)(lVar10 + 0x68));
  cVar4 = FUN_14028afb0(&DAT_140b153e0);
  uVar20 = (ulonglong)bVar5;
  if (cVar4 != '\0') {
    uVar20 = 0;
  }
  cVar4 = (char)uVar20;
  if ((DAT_140b15418[5] == 0) || (*(char *)(DAT_140b15418[5] + 0x20) == '\0')) {
    if (cVar4 != '\0') {
      plVar16 = (longlong *)DAT_140b15418[6];
      if (plVar16 == (longlong *)0x0) goto LAB_140318471;
      uVar8 = FUN_140294a70(&DAT_140a16fd0);
      iVar7 = (**(code **)(*plVar16 + 0x48))(plVar16,uVar8);
      if (iVar7 != 0) {
        cVar4 = '\0';
        fVar22 = (float)FUN_140340ac0(plVar16);
        fVar23 = fVar23 - fVar22;
      }
      cVar6 = FUN_1403400b0(plVar16);
      if (cVar6 == '\0') {
        *(byte *)(plVar17 + 0x18) = *(byte *)(plVar17 + 0x18) & 0xef;
        *(undefined4 *)((longlong)plVar17 + 0xb4) = 0;
        *(undefined4 *)((longlong)plVar17 + 0xac) = 0;
      }
      if (cVar4 != '\0') goto LAB_140318471;
    }
  }
  else {
    cVar4 = '\0';
  }
  *(byte *)(plVar17 + 0x18) = *(byte *)(plVar17 + 0x18) & 0xef;
  *(undefined4 *)((longlong)plVar17 + 0xb4) = 0;
  *(undefined4 *)((longlong)plVar17 + 0xac) = 0;
LAB_140318471:
  *(char *)(param_1 + 0x20b0) = cVar4;
  (**(code **)(*plVar17 + 0x80))(plVar17,cVar4 == '\0');
  FUN_140353ad0(plVar17,0,0);
  (**(code **)(*plVar17 + 0xf0))(plVar17,fVar23,0);
  uVar8 = FUN_140294a70(&DAT_140a16fd0);
  lVar9 = FUN_1402ad080(DAT_140b15418[3],uVar8);
  cVar6 = '\x01';
  cVar4 = '\x01';
  lVar11 = *(longlong *)(lVar9 + 0x10);
  if (cVar3 == '\0') {
    while ((lVar11 != lVar9 + 8 && ((cVar6 != '\0' || (cVar4 != '\0'))))) {
      uVar20 = *(longlong *)(lVar11 + 0x18) - 0x18;
      if (*(longlong *)(lVar11 + 0x18) == 0) {
        uVar20 = 0;
      }
      if (uVar20 != 0) {
        uVar8 = FUN_140294a70(&DAT_140a16fd0);
        cVar3 = FUN_14039a230(uVar20,uVar8,0);
        if (cVar3 != '\0') {
          cVar3 = '\0';
          if (*(char *)(uVar20 + 0x33b) == -1) {
            cVar3 = cVar6;
          }
          cVar6 = cVar3;
          if (*(char *)(uVar20 + 0x333) != -1) {
            cVar4 = '\0';
          }
        }
      }
      lVar11 = *(longlong *)(lVar11 + 8);
    }
  }
  plVar17 = (longlong *)FUN_140347b50(0xeb);
  (**(code **)(*plVar17 + 0x80))(plVar17,cVar6);
  plVar17 = (longlong *)FUN_140347b50(0xec);
  if ((cVar6 == '\0') && (cVar4 == '\0')) {
    uVar15 = 0;
  }
  else {
    uVar15 = 1;
  }
  (**(code **)(*plVar17 + 0x80))(plVar17,uVar15);
  plVar17 = (longlong *)FUN_140347b50(0xed);
  (**(code **)(*plVar17 + 0x80))(plVar17,cVar4);
  plVar17 = (longlong *)FUN_140347b50(0xee);
  (**(code **)(*plVar17 + 0x80))(plVar17,cVar4);
  plVar17 = (longlong *)FUN_140347b50(0xef);
  (**(code **)(*plVar17 + 0x80))(plVar17,cVar4);
  plVar17 = (longlong *)FUN_140347b50(0xf0);
  if ((cVar6 == '\0') && (cVar4 == '\0')) {
    (**(code **)(*plVar17 + 0x80))(plVar17,0);
  }
  else {
    (**(code **)(*plVar17 + 0x80))(plVar17,1);
    if (cVar6 != '\0') {
      *(undefined2 *)(param_1 + 0x2060) = 0;
      *(undefined1 *)(param_1 + 0x2063) = 0;
    }
    if (cVar4 != '\0') {
      *(undefined2 *)(param_1 + 0x2062) = 0;
      *(undefined1 *)(param_1 + 0x2060) = 0;
    }
  }
  plVar17 = (longlong *)FUN_140347b50(0xe9);
  if (plVar17 != (longlong *)0x0) {
    (**(code **)(*plVar17 + 0x80))(plVar17,(int)DAT_140b15418[0x2d] == 0);
  }
  lVar11 = FUN_140294a40(&DAT_140a16fd0);
  if (*(char *)(lVar11 + 0x3c0) != '\0') {
    plVar17 = (longlong *)FUN_140347b50(0x26d);
    if (plVar17 != (longlong *)0x0) {
      (**(code **)(*plVar17 + 0x80))(plVar17,1);
    }
    plVar17 = (longlong *)FUN_140347b50(0x26c);
    if (plVar17 != (longlong *)0x0) {
      (**(code **)(*plVar17 + 0x80))(plVar17,1);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x316260  Name: FUN_140316260  Size: 4621 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140316260(longlong param_1)

{
  char cVar1;
  longlong ***ppplVar2;
  longlong ****pppplVar3;
  longlong **pplVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined1 *puVar10;
  ulonglong uVar11;
  longlong ****pppplVar12;
  longlong *plVar13;
  longlong ****pppplVar14;
  undefined8 *puVar15;
  longlong ****pppplVar16;
  int *piVar17;
  longlong ***ppplVar18;
  longlong lVar19;
  undefined8 uVar20;
  longlong ****pppplVar21;
  longlong *plVar22;
  char *pcVar23;
  longlong ****pppplVar24;
  ulonglong uVar25;
  ulonglong uVar26;
  uint uVar27;
  char *pcVar28;
  uint uVar29;
  ulonglong uVar30;
  longlong lVar32;
  longlong ****pppplVar33;
  undefined4 local_res20 [2];
  undefined8 in_stack_fffffffffffffca8;
  undefined4 uVar34;
  longlong ***local_348;
  undefined8 uStack_340;
  longlong **local_338;
  ulonglong uStack_330;
  longlong ***local_328;
  undefined8 uStack_320;
  ulonglong local_318;
  ulonglong local_310;
  longlong ***local_308;
  longlong local_300;
  longlong ***local_2f8;
  longlong **pplStack_2f0;
  longlong **local_2e8;
  longlong **pplStack_2e0;
  longlong ***local_2d8;
  longlong **pplStack_2d0;
  undefined8 local_2c8;
  undefined4 local_2c0;
  int local_2b8;
  int local_2b4;
  longlong local_2b0 [2];
  longlong local_2a0 [2];
  undefined8 local_290;
  ulonglong local_288;
  undefined8 local_280;
  undefined8 local_278;
  undefined4 local_270;
  undefined8 local_26c;
  float local_264;
  undefined8 local_260;
  undefined8 local_258;
  char local_248 [256];
  short local_148 [132];
  longlong ****pppplVar31;
  
  plVar13 = DAT_140b15418;
  uVar34 = (undefined4)((ulonglong)in_stack_fffffffffffffca8 >> 0x20);
  lVar32 = DAT_140b15418[3];
  uVar6 = FUN_140294a70(&DAT_140a16fd0);
  iVar7 = FUN_1402ac5c0(lVar32,uVar6);
  local_2b8 = iVar7;
  iVar8 = FUN_1402aba70(lVar32,uVar6);
  local_2b4 = FUN_14017d070(lVar32,uVar6);
  local_2b4 = iVar8 - local_2b4;
  uVar25 = CONCAT44(uVar34,iVar7);
  FUN_140022b30(local_148,0x80,L"%d/%d",local_2b4,uVar25);
  pppplVar33 = (longlong ****)0x0;
  pplStack_2e0 = (longlong **)0x7;
  local_2f8 = (longlong ***)0x0;
  local_2e8 = (longlong **)0x0;
  lVar32 = -1;
  do {
    lVar32 = lVar32 + 1;
  } while (local_148[lVar32] != 0);
  FUN_1400228a0(&local_2f8,local_148);
  pplVar4 = local_2e8;
  pppplVar12 = &local_2f8;
  if ((longlong ***)0x7 < pplStack_2e0) {
    pppplVar12 = (longlong ****)local_2f8;
  }
  local_348 = (longlong ***)0x0;
  local_338 = (longlong **)0x0;
  uStack_330 = 0;
  if (local_2e8 < (longlong ***)0x8) {
    local_348 = *pppplVar12;
    uStack_340 = pppplVar12[1];
    uStack_330 = 7;
  }
  else {
    uVar26 = (ulonglong)local_2e8 | 7;
    if (0x7ffffffffffffffe < uVar26) {
      uVar26 = 0x7ffffffffffffffe;
    }
    local_348 = (longlong ***)FUN_1400226c0(&local_348,uVar26 + 1);
    FUN_14076bd80(local_348,pppplVar12,(longlong)pplVar4 * 2 + 2);
    uStack_330 = uVar26;
  }
  local_338 = pplVar4;
  FUN_140347c30(7,&local_348);
  pplVar4 = local_2e8;
  pppplVar12 = &local_2f8;
  if ((longlong ***)0x7 < pplStack_2e0) {
    pppplVar12 = (longlong ****)local_2f8;
  }
  local_348 = (longlong ***)0x0;
  local_338 = (longlong **)0x0;
  uStack_330 = 0;
  if (local_2e8 < (longlong ***)0x8) {
    local_348 = *pppplVar12;
    uStack_340 = pppplVar12[1];
    uStack_330 = 7;
  }
  else {
    uVar26 = (ulonglong)local_2e8 | 7;
    if (0x7ffffffffffffffe < uVar26) {
      uVar26 = 0x7ffffffffffffffe;
    }
    local_348 = (longlong ***)FUN_1400226c0(&local_348,uVar26 + 1);
    FUN_14076bd80(local_348,pppplVar12,(longlong)pplVar4 * 2 + 2);
    uStack_330 = uVar26;
  }
  local_338 = pplVar4;
  FUN_140347c30(0x22f,&local_348);
  lVar32 = plVar13[0x55];
  FUN_1400228a0(&local_2f8,L"Unknown",7);
  if (lVar32 == 0) {
    plVar22 = plVar13 + 10;
    local_348 = (longlong ***)0x0;
    uStack_330 = 0xf;
    if (0xf < (ulonglong)plVar13[0xd]) {
      plVar22 = (longlong *)*plVar22;
    }
    lVar32 = -(longlong)plVar22;
    do {
      lVar19 = *plVar22;
      *(char *)((longlong)plVar22 + (longlong)(local_248 + lVar32)) = (char)lVar19;
      plVar22 = (longlong *)((longlong)plVar22 + 1);
    } while ((char)lVar19 != '\0');
    puVar10 = (undefined1 *)FUN_14076c698(local_248,&DAT_14085d320);
    if (puVar10 != (undefined1 *)0x0) {
      *puVar10 = 0;
    }
    puVar10 = (undefined1 *)FUN_14076c698(local_248,&DAT_14085d324);
    if (puVar10 != (undefined1 *)0x0) {
      *puVar10 = 0;
    }
    pcVar23 = (char *)FUN_14076c894(local_248,0x5c);
    uVar26 = uStack_330;
    if (pcVar23 != (char *)0x0) {
      pcVar23 = pcVar23 + 1;
    }
    if (uStack_330 < 2) {
      uVar30 = 0x7fffffffffffffff;
      if (0x7fffffffffffffff - (uStack_330 >> 1) < uStack_330) {
        uVar11 = 0x8000000000000027;
LAB_140316569:
        ppplVar18 = (longlong ***)FUN_140769c58(uVar11);
        if (ppplVar18 == (longlong ***)0x0) goto LAB_140317453;
        pppplVar12 = (longlong ****)((longlong)ppplVar18 + 0x27U & 0xffffffffffffffe0);
        pppplVar12[-1] = ppplVar18;
      }
      else {
        uVar11 = uStack_330 + (uStack_330 >> 1);
        uVar30 = 0xf;
        if (0xf < uVar11) {
          uVar30 = uVar11;
        }
        if (0xfff < uVar30 + 1) {
          uVar11 = uVar30 + 0x28;
          if (uVar11 <= uVar30 + 1) {
                    /* WARNING: Subroutine does not return */
            FUN_1400225f0();
          }
          goto LAB_140316569;
        }
        pppplVar12 = pppplVar33;
        if (uVar30 != 0xffffffffffffffff) {
          pppplVar12 = (longlong ****)FUN_140769c58();
        }
      }
      local_338 = (longlong **)0x2;
      *(undefined2 *)pppplVar12 = 0x4e4d;
      *(undefined1 *)((longlong)pppplVar12 + 2) = 0;
      uVar11 = uVar25;
      uStack_330 = uVar30;
      if (0xf < uVar26) {
        uVar30 = uVar26 + 1;
        pppplVar21 = (longlong ****)local_348;
        if (0xfff < uVar30) {
          pppplVar21 = (longlong ****)local_348[-1];
          uVar30 = uVar26 + 0x28;
          if (0x1f < (ulonglong)((longlong)local_348 + (-8 - (longlong)pppplVar21)))
          goto LAB_140317453;
        }
        thunk_FUN_1407864b8(pppplVar21,uVar30);
        uVar11 = uVar25;
      }
    }
    else {
      local_338 = (longlong **)0x2;
      pppplVar12 = &local_348;
      if (0xf < uStack_330) {
        pppplVar12 = (longlong ****)local_348;
      }
      *(undefined2 *)pppplVar12 = 0x4e4d;
      *(undefined1 *)((longlong)pppplVar12 + 2) = 0;
      uVar11 = uVar25;
      pppplVar12 = (longlong ****)local_348;
    }
    local_348 = (longlong ***)pppplVar12;
    uVar25 = 0xffffffffffffffff;
    pcVar28 = local_248;
    if (pcVar23 != (char *)0x0) {
      pcVar28 = pcVar23;
    }
    do {
      uVar25 = uVar25 + 1;
    } while (pcVar28[uVar25] != '\0');
    if (uStack_330 - (longlong)local_338 < uVar25) {
      FUN_14004f860(&local_348,uVar25,0,pcVar28,uVar25);
    }
    else {
      pppplVar12 = &local_348;
      if (0xf < uStack_330) {
        pppplVar12 = (longlong ****)local_348;
      }
      lVar32 = (longlong)pppplVar12 + (longlong)local_338;
      local_338 = (longlong **)(uVar25 + (longlong)local_338);
      FUN_14076bd80(lVar32,pcVar28,uVar25);
      *(undefined1 *)(uVar25 + lVar32) = 0;
      uVar25 = uVar11;
    }
    pppplVar12 = (longlong ****)FUN_140200ad0(&local_348);
    if (&local_348 != pppplVar12) {
      pppplVar21 = pppplVar12;
      if ((longlong ***)0xf < pppplVar12[3]) {
        pppplVar21 = (longlong ****)*pppplVar12;
      }
      FUN_140022730(&local_348,pppplVar21,pppplVar12[2]);
    }
    cVar5 = FUN_1401fa1a0(&DAT_140a7bc58,&local_348,&local_2f8,1);
    if (cVar5 == '\0') {
      local_328 = (longlong ***)0x0;
      lVar32 = -1;
      local_318 = 0;
      local_310 = 0xf;
      do {
        lVar32 = lVar32 + 1;
      } while (pcVar28[lVar32] != '\0');
      FUN_140022730(&local_328,pcVar28);
      pppplVar12 = (longlong ****)FUN_140200ad0(&local_328);
      if (&local_328 != pppplVar12) {
        pppplVar21 = pppplVar12;
        if ((longlong ***)0xf < pppplVar12[3]) {
          pppplVar21 = (longlong ****)*pppplVar12;
        }
        FUN_140022730(&local_328,pppplVar21,pppplVar12[2]);
      }
      if (0x10 < local_318) {
        local_318 = 0x10;
        pppplVar12 = &local_328;
        if (0xf < local_310) {
          pppplVar12 = (longlong ****)local_328;
        }
        *(undefined1 *)(pppplVar12 + 2) = 0;
      }
      pppplVar12 = (longlong ****)FUN_140200870(local_2a0,&local_328);
      if (&local_2f8 != pppplVar12) {
        if ((longlong ***)0x7 < pplStack_2e0) {
          uVar26 = (longlong)pplStack_2e0 * 2 + 2;
          pppplVar21 = (longlong ****)local_2f8;
          if (0xfff < uVar26) {
            pppplVar21 = (longlong ****)local_2f8[-1];
            uVar26 = (longlong)pplStack_2e0 * 2 + 0x29;
            if (0x1f < (ulonglong)((longlong)local_2f8 + (-8 - (longlong)pppplVar21)))
            goto LAB_140317447;
          }
          thunk_FUN_1407864b8(pppplVar21,uVar26);
        }
        local_2f8 = *pppplVar12;
        pplStack_2f0 = (longlong **)pppplVar12[1];
        local_2e8 = (longlong **)pppplVar12[2];
        pplStack_2e0 = (longlong **)pppplVar12[3];
        pppplVar12[2] = (longlong ***)0x0;
        pppplVar12[3] = (longlong ***)0x7;
        *(undefined2 *)pppplVar12 = 0;
      }
      if (7 < local_288) {
        if (0xfff < local_288 * 2 + 2) {
          pppplVar21 = *(longlong *****)(local_2a0[0] + -8);
          uVar26 = local_288 * 2 + 0x29;
          if (0x1f < (local_2a0[0] - (longlong)pppplVar21) - 8U) {
LAB_140317447:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(pppplVar21,uVar26);
          }
        }
        thunk_FUN_1407864b8();
      }
      if (0xf < local_310) {
        if (0xfff < local_310 + 1) {
          if (0x1f < (ulonglong)((longlong)local_328 + (-8 - (longlong)local_328[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_328[-1],local_310 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
    }
    if (0xf < uStack_330) {
      uVar26 = uStack_330 + 1;
      pppplVar12 = (longlong ****)local_348;
      if (0xfff < uVar26) {
        pppplVar12 = (longlong ****)local_348[-1];
        uVar26 = uStack_330 + 0x28;
        if (0x1f < (ulonglong)((longlong)local_348 + (-8 - (longlong)pppplVar12))) {
LAB_140317453:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(pppplVar12,uVar26);
    }
  }
  else {
    pppplVar12 = (longlong ****)(lVar32 + 0x140);
    if (&local_2f8 != pppplVar12) {
      if (7 < *(ulonglong *)(lVar32 + 0x158)) {
        pppplVar12 = (longlong ****)*pppplVar12;
      }
      FUN_1400228a0(&local_2f8,pppplVar12,*(undefined8 *)(lVar32 + 0x150));
    }
  }
  pplVar4 = local_2e8;
  pppplVar12 = &local_2f8;
  if ((longlong ***)0x7 < pplStack_2e0) {
    pppplVar12 = (longlong ****)local_2f8;
  }
  local_348 = (longlong ***)0x0;
  local_338 = (longlong **)0x0;
  uStack_330 = 0;
  if (local_2e8 < (longlong ***)0x8) {
    local_348 = *pppplVar12;
    uStack_340 = pppplVar12[1];
    uStack_330 = 7;
  }
  else {
    uVar26 = (ulonglong)local_2e8 | 7;
    if (0x7ffffffffffffffe < uVar26) {
      uVar26 = 0x7ffffffffffffffe;
    }
    local_348 = (longlong ***)FUN_1400226c0(&local_348,uVar26 + 1);
    FUN_14076bd80(local_348,pppplVar12,(longlong)pplVar4 * 2 + 2);
    uStack_330 = uVar26;
  }
  local_338 = pplVar4;
  FUN_140347c30(8,&local_348);
  pppplVar12 = (longlong ****)FUN_140347b50(0x218);
  local_2d8 = (longlong ***)pppplVar12;
  if ((pppplVar12 == (longlong ****)0x0) || (cVar5 = FUN_14034ce60(pppplVar12), cVar5 != '\0'))
  goto LAB_1403173a8;
  if ((DAT_140b15418 != (longlong *)0x0) &&
     ((DAT_140b15418[5] != 0 && (*(char *)(DAT_140b15418[5] + 0x20) != '\0')))) {
    (*(code *)(*pppplVar12)[0xf])(pppplVar12,1);
    goto LAB_1403173a8;
  }
  local_2a0[0] = 0;
  local_290 = 0;
  local_288 = 7;
  cVar5 = FUN_14035f790();
  pcVar23 = "TEXT_BUTTON_BEGIN";
  if (cVar5 == '\0') {
    pcVar23 = "TEXT_BUTTON_CLOSE";
  }
  FUN_1401f9fe0(&DAT_140a7bc58,pcVar23,local_2a0);
  plVar13 = (longlong *)FUN_140347b50(0x219);
  (**(code **)(*plVar13 + 0x148))(plVar13,local_2a0,1,0);
  local_300 = 0;
  local_308 = (longlong ***)FUN_140769c58(0x30);
  *local_308 = (longlong **)local_308;
  local_308[1] = (longlong **)local_308;
  local_308[2] = (longlong **)local_308;
  *(undefined2 *)(local_308 + 3) = 0x101;
  iVar7 = FUN_140347af0(0x21a);
  cVar5 = FUN_14028afb0(&DAT_140b153e0);
  if (cVar5 == '\x01') {
    lVar32 = FUN_140294a40(&DAT_140a16fd0);
    if ((lVar32 != 0) &&
       (pppplVar21 = pppplVar33, pppplVar31 = pppplVar33, 0 < *(int *)(lVar32 + 0x2d0))) {
      do {
        ppplVar2 = local_308;
        ppplVar18 = *(longlong ****)((longlong)pppplVar31 + *(longlong *)(lVar32 + 0x2c8));
        if (ppplVar18 != (longlong ***)0x0) {
          pppplVar14 = (longlong ****)local_308[1];
          cVar1 = *(char *)((longlong)pppplVar14 + 0x19);
          pppplVar16 = pppplVar14;
          pppplVar3 = (longlong ****)local_308;
          cVar5 = cVar1;
          while (pppplVar24 = pppplVar16, cVar5 == '\0') {
            if (pppplVar24[4] < ppplVar18) {
              pppplVar16 = (longlong ****)pppplVar24[2];
              pppplVar24 = pppplVar3;
            }
            else {
              pppplVar16 = (longlong ****)*pppplVar24;
            }
            cVar5 = *(char *)((longlong)pppplVar16 + 0x19);
            pppplVar3 = pppplVar24;
          }
          if (((*(char *)((longlong)pppplVar3 + 0x19) == '\0') && (pppplVar3[4] <= ppplVar18)) &&
             (pppplVar3 != (longlong ****)local_308)) {
            *(int *)(pppplVar3 + 5) = *(int *)(pppplVar3 + 5) + 1;
          }
          else {
            uStack_340 = (longlong ***)((ulonglong)uStack_340 & 0xffffffff00000000);
            local_348 = (longlong ***)pppplVar14;
            pppplVar16 = (longlong ****)local_308;
            while (pppplVar3 = pppplVar14, cVar1 == '\0') {
              uStack_340._4_4_ = (uint)((ulonglong)uStack_340 >> 0x20);
              if (pppplVar3[4] < ppplVar18) {
                pppplVar14 = (longlong ****)pppplVar3[2];
                uStack_340 = (longlong ***)((ulonglong)uStack_340._4_4_ << 0x20);
              }
              else {
                uStack_340 = (longlong ***)CONCAT44(uStack_340._4_4_,1);
                pppplVar14 = (longlong ****)*pppplVar3;
                pppplVar16 = pppplVar3;
              }
              cVar1 = *(char *)((longlong)pppplVar14 + 0x19);
              local_348 = (longlong ***)pppplVar3;
            }
            if ((*(char *)((longlong)pppplVar16 + 0x19) != '\0') || (ppplVar18 < pppplVar16[4])) {
              if (local_300 == 0x555555555555555) {
                    /* WARNING: Subroutine does not return */
                FUN_1400501c0();
              }
              puVar15 = (undefined8 *)FUN_140769c58(0x30);
              puVar15[4] = ppplVar18;
              *(undefined4 *)(puVar15 + 5) = 0;
              *puVar15 = ppplVar2;
              puVar15[1] = ppplVar2;
              puVar15[2] = ppplVar2;
              *(undefined2 *)(puVar15 + 3) = 0;
              local_2d8 = local_348;
              pplStack_2d0 = (longlong **)uStack_340;
              pppplVar16 = (longlong ****)FUN_14004fe80(&local_308,&local_2d8,puVar15);
            }
            *(undefined4 *)(pppplVar16 + 5) = 1;
          }
        }
        uVar27 = (int)pppplVar21 + 1;
        pppplVar21 = (longlong ****)(ulonglong)uVar27;
        pppplVar31 = pppplVar31 + 1;
      } while ((int)uVar27 < *(int *)(lVar32 + 0x2d0));
    }
  }
  else {
    lVar32 = FUN_14028aaa0(&DAT_140b153e0,DAT_140b15418);
    local_2b0[0] = *(longlong *)(lVar32 + 0x80);
    local_res20[0] = FUN_140294a70(&DAT_140a16fd0);
    plVar13 = (longlong *)FUN_140045af0(lVar32 + 0x40,&local_348,local_res20);
    local_2c8 = *plVar13;
    if (0 < *(int *)(local_2c8 + 0x28)) {
      piVar17 = *(int **)(local_2c8 + 0x20);
      pppplVar21 = pppplVar33;
      do {
        pppplVar31 = pppplVar33;
        pppplVar14 = pppplVar33;
        if (*piVar17 != 0x3fffff) goto LAB_140316c10;
        pppplVar21 = (longlong ****)((longlong)pppplVar21 + 1);
        piVar17 = piVar17 + 1;
      } while ((longlong)pppplVar21 < (longlong)*(int *)(local_2c8 + 0x28));
    }
  }
  goto LAB_140316ecf;
LAB_140316c10:
  do {
    if (((*(int *)((longlong)pppplVar14 + *(longlong *)(local_2c8 + 0x20)) != 0x3fffff) &&
        (lVar32 = FUN_14028aa50(&DAT_140b153e0,DAT_140b15418), lVar32 != 0)) &&
       ((*(longlong *)(lVar32 + 0x298) != 0 &&
        ((*(char *)(*(longlong *)(lVar32 + 0x298) + 0x2d) == '\0' &&
         (iVar8 = FUN_140294a70(&DAT_140a16fd0), iVar8 == *(int *)(lVar32 + 0x58))))))) {
      ppplVar18 = (longlong ***)FUN_1403973b0(lVar32);
      if ((*(char *)(lVar32 + 0x337) != -1) &&
         ((lVar19 = FUN_140397bf0(lVar32), lVar19 != 0 && (0 < *(int *)(lVar19 + 0x10))))) {
        plVar13 = *(longlong **)(lVar19 + 8);
        pppplVar12 = pppplVar33;
        do {
          if (*(char *)(*(longlong *)(*plVar13 + 0x298) + 0x2d) != '\0') goto LAB_140316eb3;
          pppplVar12 = (longlong ****)((longlong)pppplVar12 + 1);
          plVar13 = plVar13 + 1;
        } while ((longlong)pppplVar12 < (longlong)*(int *)(lVar19 + 0x10));
      }
      cVar5 = *(char *)((longlong)local_308[1] + 0x19);
      pppplVar12 = (longlong ****)local_308[1];
      pppplVar21 = (longlong ****)local_308;
      while (pppplVar16 = pppplVar12, cVar5 == '\0') {
        if (pppplVar16[4] < ppplVar18) {
          pppplVar12 = (longlong ****)pppplVar16[2];
          pppplVar16 = pppplVar21;
        }
        else {
          pppplVar12 = (longlong ****)*pppplVar16;
        }
        cVar5 = *(char *)((longlong)pppplVar12 + 0x19);
        pppplVar21 = pppplVar16;
      }
      if ((*(char *)((longlong)pppplVar21 + 0x19) != '\0') || (ppplVar18 < pppplVar21[4])) {
        pppplVar21 = (longlong ****)local_308;
      }
      if (local_2b0[0] != 0) {
        uVar20 = FUN_1403973b0(lVar32);
        cVar5 = FUN_140373490(*(undefined8 *)(local_2b0[0] + 0x298),uVar20);
        ppplVar2 = local_308;
        if (cVar5 == '\x01') {
          if (pppplVar21 == (longlong ****)local_308) {
            pppplVar12 = (longlong ****)local_308[1];
            uStack_320 = uStack_320 & 0xffffffff00000000;
            cVar5 = *(char *)((longlong)pppplVar12 + 0x19);
            local_328 = (longlong ***)pppplVar12;
            pppplVar21 = (longlong ****)local_308;
            while (pppplVar16 = pppplVar12, cVar5 == '\0') {
              uStack_320._4_4_ = (uint)(uStack_320 >> 0x20);
              if (pppplVar16[4] < ppplVar18) {
                pppplVar12 = (longlong ****)pppplVar16[2];
                uStack_320 = (ulonglong)uStack_320._4_4_ << 0x20;
              }
              else {
                uStack_320 = CONCAT44(uStack_320._4_4_,1);
                pppplVar12 = (longlong ****)*pppplVar16;
                pppplVar21 = pppplVar16;
              }
              cVar5 = *(char *)((longlong)pppplVar12 + 0x19);
              local_328 = (longlong ***)pppplVar16;
            }
            if ((*(char *)((longlong)pppplVar21 + 0x19) != '\0') || (ppplVar18 < pppplVar21[4])) {
              if (local_300 == 0x555555555555555) goto LAB_14031745f;
              puVar15 = (undefined8 *)FUN_140769c58(0x30);
              puVar15[4] = ppplVar18;
              *(undefined4 *)(puVar15 + 5) = 0;
              *puVar15 = ppplVar2;
              puVar15[1] = ppplVar2;
              puVar15[2] = ppplVar2;
              *(undefined2 *)(puVar15 + 3) = 0;
              pppplVar21 = (longlong ****)FUN_14004fe80(&local_308,&local_328,puVar15);
            }
            *(undefined4 *)(pppplVar21 + 5) = 0xffffffff;
          }
          else {
            *(int *)(pppplVar21 + 5) = *(int *)(pppplVar21 + 5) + -1;
          }
          goto LAB_140316eb3;
        }
      }
      ppplVar2 = local_308;
      if (pppplVar21 == (longlong ****)local_308) {
        pppplVar12 = (longlong ****)local_308[1];
        uStack_320 = uStack_320 & 0xffffffff00000000;
        cVar5 = *(char *)((longlong)pppplVar12 + 0x19);
        local_328 = (longlong ***)pppplVar12;
        pppplVar21 = (longlong ****)local_308;
        while (pppplVar16 = pppplVar12, cVar5 == '\0') {
          uStack_320._4_4_ = (uint)(uStack_320 >> 0x20);
          if (pppplVar16[4] < ppplVar18) {
            pppplVar12 = (longlong ****)pppplVar16[2];
            uStack_320 = (ulonglong)uStack_320._4_4_ << 0x20;
          }
          else {
            uStack_320 = CONCAT44(uStack_320._4_4_,1);
            pppplVar12 = (longlong ****)*pppplVar16;
            pppplVar21 = pppplVar16;
          }
          cVar5 = *(char *)((longlong)pppplVar12 + 0x19);
          local_328 = (longlong ***)pppplVar16;
        }
        if ((*(char *)((longlong)pppplVar21 + 0x19) != '\0') || (ppplVar18 < pppplVar21[4])) {
          if (local_300 == 0x555555555555555) {
LAB_14031745f:
                    /* WARNING: Subroutine does not return */
            FUN_1400501c0();
          }
          puVar15 = (undefined8 *)FUN_140769c58(0x30);
          puVar15[4] = ppplVar18;
          *(undefined4 *)(puVar15 + 5) = 0;
          *puVar15 = ppplVar2;
          puVar15[1] = ppplVar2;
          puVar15[2] = ppplVar2;
          *(undefined2 *)(puVar15 + 3) = 0;
          pppplVar21 = (longlong ****)FUN_14004fe80(&local_308,&local_328,puVar15);
        }
        *(undefined4 *)(pppplVar21 + 5) = 1;
      }
      else {
        *(int *)(pppplVar21 + 5) = *(int *)(pppplVar21 + 5) + 1;
      }
    }
LAB_140316eb3:
    uVar27 = (int)pppplVar31 + 1;
    pppplVar31 = (longlong ****)(ulonglong)uVar27;
    pppplVar12 = (longlong ****)local_2d8;
    pppplVar14 = (longlong ****)((longlong)pppplVar14 + 4);
  } while ((int)uVar27 < *(int *)(local_2c8 + 0x28));
LAB_140316ecf:
  pppplVar21 = (longlong ****)*local_308;
  pppplVar31 = pppplVar33;
  if (pppplVar21 != (longlong ****)local_308) {
    do {
      ppplVar18 = pppplVar21[4];
      iVar8 = *(int *)(pppplVar21 + 5);
      plVar13 = (longlong *)FUN_140347b50((int)pppplVar31 + 0x21a);
      uVar6 = (undefined4)(uVar25 >> 0x20);
      if (plVar13 != (longlong *)0x0) {
        FUN_14001e680(&local_348,ppplVar18 + 0x304);
        if ((longlong ***)local_338 == (longlong ***)0x0) {
          (**(code **)(*plVar13 + 0xa0))(plVar13,"i_button_temporary.tga",0);
        }
        else {
          pppplVar14 = &local_348;
          if (0xf < uStack_330) {
            pppplVar14 = (longlong ****)local_348;
          }
          (**(code **)(*plVar13 + 0xa0))(plVar13,pppplVar14,0);
        }
        (**(code **)(*plVar13 + 0x78))(plVar13,0);
        if (iVar8 < 0) {
          (**(code **)(*plVar13 + 0x80))(plVar13,1);
          uVar25 = CONCAT44(uVar6,0xff);
          iVar8 = -iVar8;
          FUN_1401f93e0(&local_2c8,0x80,0x80,0x80,uVar25);
          (**(code **)(*plVar13 + 0x10))(plVar13,&local_2c8);
          plVar22 = &local_2c8;
        }
        else {
          (**(code **)(*plVar13 + 0x80))(plVar13,0);
          uVar25 = CONCAT44(uVar6,0xff);
          FUN_1401f93e0(local_2b0,0xff,0xff,0xff,uVar25);
          (**(code **)(*plVar13 + 0x10))(plVar13,local_2b0);
          plVar22 = local_2b0;
        }
        FUN_1401f9440(plVar22);
        uVar6 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          uVar6 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        iVar9 = FUN_140373500(ppplVar18,uVar6);
        if ((0 < iVar9) && (local_2b8 - local_2b4 < iVar9)) {
          (**(code **)(*plVar13 + 0x80))(plVar13,1);
        }
        (**(code **)(*plVar13 + 0x1e8))(plVar13,iVar8);
        if (iVar8 == 1) {
          (**(code **)(*plVar13 + 0x148))(plVar13,0,1,0);
        }
        else {
          FUN_140022b30(&local_2d8,6,&DAT_140804eb0,iVar8);
          local_328 = (longlong ***)0x0;
          lVar32 = -1;
          local_318 = 0;
          local_310 = 7;
          do {
            lVar32 = lVar32 + 1;
          } while (*(short *)((longlong)&local_2d8 + lVar32 * 2) != 0);
          FUN_1400228a0(&local_328,&local_2d8);
          (**(code **)(*plVar13 + 0x148))(plVar13,&local_328,1,0);
          if (7 < local_310) {
            if (0xfff < local_310 * 2 + 2) {
              if (0x1f < (ulonglong)((longlong)local_328 + (-8 - (longlong)local_328[-1]))) {
                    /* WARNING: Subroutine does not return */
                FUN_1407711cc(local_328[-1],local_310 * 2 + 0x29);
              }
            }
            thunk_FUN_1407864b8();
          }
        }
        FUN_140353ad0(plVar13,ppplVar18);
        uVar27 = (int)pppplVar31 + 1;
        pppplVar31 = (longlong ****)(ulonglong)uVar27;
        if (iVar7 <= (int)uVar27) {
          if (0xf < uStack_330) {
            uVar25 = uStack_330 + 1;
            pppplVar14 = (longlong ****)local_348;
            if (0xfff < uVar25) {
              pppplVar14 = (longlong ****)local_348[-1];
              uVar25 = uStack_330 + 0x28;
              if (0x1f < (ulonglong)((longlong)local_348 + (-8 - (longlong)pppplVar14)))
              goto LAB_140317471;
            }
            thunk_FUN_1407864b8(pppplVar14,uVar25);
          }
          goto LAB_140317204;
        }
        if (0xf < uStack_330) {
          if ((0xfff < uStack_330 + 1) &&
             (pppplVar14 = (longlong ****)local_348[-1],
             0x1f < (ulonglong)((longlong)local_348 + (-8 - (longlong)pppplVar14)))) {
LAB_140317471:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(pppplVar14,uStack_330 + 0x28);
          }
          thunk_FUN_1407864b8();
        }
      }
      pppplVar14 = (longlong ****)pppplVar21[2];
      if (*(char *)((longlong)pppplVar14 + 0x19) == '\0') {
        cVar5 = *(char *)((longlong)*pppplVar14 + 0x19);
        pppplVar21 = pppplVar14;
        pppplVar14 = (longlong ****)*pppplVar14;
        while (cVar5 == '\0') {
          cVar5 = *(char *)((longlong)*pppplVar14 + 0x19);
          pppplVar21 = pppplVar14;
          pppplVar14 = (longlong ****)*pppplVar14;
        }
      }
      else {
        cVar5 = *(char *)((longlong)pppplVar21[1] + 0x19);
        pppplVar16 = (longlong ****)pppplVar21[1];
        pppplVar14 = pppplVar21;
        while ((pppplVar21 = pppplVar16, cVar5 == '\0' &&
               (pppplVar14 == (longlong ****)pppplVar21[2]))) {
          cVar5 = *(char *)((longlong)pppplVar21[1] + 0x19);
          pppplVar16 = (longlong ****)pppplVar21[1];
          pppplVar14 = pppplVar21;
        }
      }
    } while (pppplVar21 != (longlong ****)local_308);
  }
  uVar27 = (uint)pppplVar31;
  uVar29 = uVar27;
  while ((int)uVar29 < iVar7) {
    plVar13 = (longlong *)FUN_140347b50((int)pppplVar31 + 0x21a);
    if (plVar13 != (longlong *)0x0) {
      (**(code **)(*plVar13 + 0xa0))(plVar13,0,0);
      (**(code **)(*plVar13 + 0x78))(plVar13);
      FUN_140353ad0(plVar13);
    }
    uVar29 = (int)pppplVar31 + 1;
    pppplVar31 = (longlong ****)(ulonglong)uVar29;
  }
LAB_140317204:
  uVar27 = (int)((uVar27 - 1) + ((int)(uVar27 - 1) >> 0x1f & 3U)) >> 2;
  if (-1 < (int)uVar27) {
    if (4 < (int)uVar27) {
      uVar27 = 4;
    }
    pppplVar33 = (longlong ****)(ulonglong)uVar27;
  }
  (*(code *)(*pppplVar12)[0x60])(pppplVar12,pppplVar33);
  local_2d8 = (longlong ***)0x3f8000003f800000;
  pplStack_2d0 = (longlong **)CONCAT44(pplStack_2d0._4_4_,0x3f800000);
  local_2c0 = 0;
  local_2c8 = CONCAT44(*(float *)(param_1 + 0xffc) / DAT_1408007d4 - DAT_1408007f0,
                       (float)(*(uint *)(param_1 + 0xff8) ^ DAT_140800860) / DAT_1408007d4);
  (*(code *)(*pppplVar12)[0x19])(pppplVar12,&local_2c8,&local_2d8);
  local_280 = 0x3f800000;
  local_278 = 0;
  local_270 = 0;
  local_26c = 0x3f800000;
  local_260 = 0;
  local_258 = 0x3f800000;
  local_264 = (float)(4 - (int)pppplVar33) * _DAT_14085d68c;
  lVar32 = FUN_140347b50(0x219);
  if (lVar32 != 0) {
    uVar6 = *(undefined4 *)(lVar32 + 0xa0);
    FUN_14034ba10(pppplVar12,uVar6,1);
    FUN_140354c80(pppplVar12,uVar6,&local_280);
  }
  cVar5 = *(char *)((longlong)local_308[1] + 0x19);
  ppplVar18 = (longlong ***)local_308[1];
  while (cVar5 == '\0') {
    FUN_14006c2e0(&local_308,&local_308,ppplVar18[2]);
    ppplVar2 = (longlong ***)*ppplVar18;
    thunk_FUN_1407864b8(ppplVar18,0x30);
    ppplVar18 = ppplVar2;
    cVar5 = *(char *)((longlong)ppplVar2 + 0x19);
  }
  thunk_FUN_1407864b8(local_308,0x30);
  if (7 < local_288) {
    if (0xfff < local_288 * 2 + 2) {
      if (0x1f < (local_2a0[0] - *(longlong *)(local_2a0[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_2a0[0] + -8),local_288 * 2 + 0x29);
      }
    }
    thunk_FUN_1407864b8();
  }
LAB_1403173a8:
  if ((longlong ***)0x7 < pplStack_2e0) {
    if (0xfff < (longlong)pplStack_2e0 * 2 + 2U) {
      if (0x1f < (ulonglong)((longlong)local_2f8 + (-8 - (longlong)local_2f8[-1]))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(local_2f8[-1],(longlong)pplStack_2e0 * 2 + 0x29);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}




// ================================================================
// LAND BATTLE SUB-TICKS (MODE-3)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x312570  Name: FUN_140312570  Size: 1568 bytes
// ------------------------------------------------------------

void FUN_140312570(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  bool bVar3;
  char cVar4;
  byte bVar5;
  char cVar6;
  char cVar7;
  undefined4 uVar8;
  int iVar9;
  longlong *plVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong lVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  float fVar18;
  float fVar19;
  
  cVar4 = FUN_14035f790(DAT_140b15418);
  plVar10 = (longlong *)FUN_140347b50(0xe5);
  bVar5 = 0;
  if ((cVar4 == '\0') && (*(longlong *)(DAT_140b15418 + 0x38) != 0)) {
    uVar8 = FUN_140294a70(&DAT_140a16fd0);
    bVar5 = FUN_140494410(*(undefined8 *)(DAT_140b15418 + 0x38),uVar8);
  }
  (**(code **)(*plVar10 + 0x80))(plVar10,bVar5 ^ 1);
  lVar13 = *plVar10;
  uVar11 = FUN_14034c200(plVar10,3);
  (**(code **)(lVar13 + 0xa0))(plVar10,uVar11,0);
  uVar17 = 0;
  fVar18 = DAT_1407ffaf8;
  if (bVar5 == 0) {
    if (*(char *)(param_1 + 0x20b2) != '\0') {
      if (*(longlong *)(param_1 + 0x20c0) != 0) {
        if ((DAT_140b15418 != 0) && (*(longlong *)(DAT_140b15418 + 0x338) != 0)) {
          FUN_140136ba0();
        }
        if (*(longlong **)(param_1 + 0x20c0) != (longlong *)0x0) {
          (**(code **)(**(longlong **)(param_1 + 0x20c0) + 8))();
          *(undefined8 *)(param_1 + 0x20c0) = 0;
        }
      }
      *(undefined1 *)(param_1 + 0x20b2) = 0;
    }
    uVar8 = FUN_140294a70(&DAT_140a16fd0);
    fVar18 = (float)FUN_140495250(*(undefined8 *)(DAT_140b15418 + 0x38),uVar8);
    if (fVar18 == 0.0) {
      fVar18 = DAT_1407ffaf8;
    }
  }
  fVar19 = DAT_1407ffaf8;
  (**(code **)(*plVar10 + 0xf0))(plVar10,fVar18,0);
  lVar13 = DAT_140b15418;
  lVar12 = FUN_140294a40(&DAT_140a16fd0);
  cVar6 = FUN_14028afb0(&DAT_140b153e0);
  if (cVar6 == '\0') {
    plVar10 = (longlong *)FUN_140347b50(0xe6);
    (**(code **)(*plVar10 + 0x80))(plVar10,1);
    lVar1 = *plVar10;
    uVar11 = FUN_14034c340(plVar10);
    (**(code **)(lVar1 + 0xa0))(plVar10,uVar11,0);
    FUN_140353ad0(plVar10,0,0);
  }
  else {
    plVar10 = (longlong *)FUN_140347b50(0xe6);
    FUN_140353ad0(plVar10,0,0);
    lVar1 = *plVar10;
    uVar11 = FUN_14034c340(plVar10);
    (**(code **)(lVar1 + 0xa0))(plVar10,uVar11,0);
    if ((*(int *)(param_1 + 0x2068) < 1) || (*(char *)(param_1 + 0x2065) != '\0')) {
      (**(code **)(*plVar10 + 0x80))(plVar10,0);
    }
    else {
      (**(code **)(*plVar10 + 0x80))(plVar10,1);
    }
  }
  cVar6 = FUN_14028afb0(&DAT_140b153e0);
  if (((cVar6 == '\0') || (*(int *)(lVar12 + 0x2d0) == 0)) &&
     (cVar6 = FUN_1402f8b30(param_1,lVar12,lVar13), cVar6 == '\0')) {
    bVar3 = false;
  }
  else {
    bVar3 = true;
  }
  plVar10 = (longlong *)FUN_140347b50(0xe7);
  if ((((DAT_140b15418 == 0) || (*(longlong *)(DAT_140b15418 + 0x28) == 0)) ||
      (*(char *)(*(longlong *)(DAT_140b15418 + 0x28) + 0x20) == '\0')) &&
     ((*(char *)(lVar12 + 0x39c) != '\0' && (bVar3)))) {
    uVar11 = 0;
  }
  else {
    uVar11 = 1;
  }
  (**(code **)(*plVar10 + 0x80))(plVar10,uVar11);
  FUN_140353ad0(plVar10,0,0);
  lVar13 = FUN_140347b50(0x218);
  if (((lVar13 != 0) && (cVar6 = FUN_14034ce60(lVar13), cVar6 == '\0')) ||
     ((*(byte *)(plVar10 + 0x18) & 4) != 0)) {
    *(byte *)(plVar10 + 0x18) = *(byte *)(plVar10 + 0x18) & 0xef;
    *(undefined4 *)((longlong)plVar10 + 0xb4) = 0;
    *(undefined4 *)((longlong)plVar10 + 0xac) = 0;
  }
  bVar5 = FUN_140446480(&DAT_140b30350,*(undefined8 *)(lVar12 + 0x68));
  cVar6 = FUN_14028afb0(&DAT_140b153e0);
  uVar15 = (ulonglong)bVar5;
  if (cVar6 != '\0') {
    uVar15 = uVar17;
  }
  cVar6 = (char)uVar15;
  if ((*(longlong *)(DAT_140b15418 + 0x28) == 0) || (cVar7 = FUN_140342dc0(), cVar7 == '\0')) {
    if (cVar6 != '\0') {
      plVar2 = *(longlong **)(DAT_140b15418 + 0x30);
      if (plVar2 == (longlong *)0x0) goto LAB_140312914;
      uVar8 = FUN_140294a70(&DAT_140a16fd0);
      iVar9 = (**(code **)(*plVar2 + 0x48))(plVar2,uVar8);
      if (iVar9 == 2) {
        cVar6 = '\0';
        fVar18 = (float)FUN_140340ac0(plVar2);
        fVar19 = fVar19 - fVar18;
      }
      cVar7 = FUN_1403400b0(plVar2);
      if (cVar7 == '\0') {
        *(byte *)(plVar10 + 0x18) = *(byte *)(plVar10 + 0x18) & 0xef;
        *(undefined4 *)((longlong)plVar10 + 0xb4) = 0;
        *(undefined4 *)((longlong)plVar10 + 0xac) = 0;
      }
      if (cVar6 != '\0') goto LAB_140312914;
    }
  }
  else {
    cVar6 = '\0';
  }
  *(byte *)(plVar10 + 0x18) = *(byte *)(plVar10 + 0x18) & 0xef;
  *(undefined4 *)((longlong)plVar10 + 0xb4) = 0;
  *(undefined4 *)((longlong)plVar10 + 0xac) = 0;
LAB_140312914:
  *(char *)(param_1 + 0x20b0) = cVar6;
  plVar10 = (longlong *)FUN_140347b50(0xe8);
  (**(code **)(*plVar10 + 0x80))(plVar10,cVar6 == '\0');
  FUN_140353ad0(plVar10,0,0);
  (**(code **)(*plVar10 + 0xf0))(plVar10,fVar19,0);
  uVar8 = FUN_140294a70(&DAT_140a16fd0);
  lVar12 = FUN_1402ad080(*(undefined8 *)(DAT_140b15418 + 0x18),uVar8);
  uVar15 = 1;
  uVar16 = 1;
  cVar6 = '\x01';
  lVar13 = *(longlong *)(lVar12 + 0x10);
  if (cVar4 == '\0') {
    while ((lVar13 != lVar12 + 8 && (((char)uVar15 != '\0' || (cVar6 = '\0', (char)uVar16 != '\0')))
           )) {
      uVar14 = *(longlong *)(lVar13 + 0x18) - 0x18;
      if (*(longlong *)(lVar13 + 0x18) == 0) {
        uVar14 = uVar17;
      }
      if (uVar14 != 0) {
        uVar8 = FUN_140294a70(&DAT_140a16fd0);
        cVar4 = FUN_14039a230(uVar14,uVar8,0);
        if (cVar4 != '\0') {
          if (*(char *)(uVar14 + 0x33b) != -1) {
            uVar15 = uVar17;
          }
          if (*(char *)(uVar14 + 0x333) != -1) {
            uVar16 = uVar17;
          }
        }
      }
      cVar6 = (char)uVar16;
      lVar13 = *(longlong *)(lVar13 + 8);
    }
  }
  plVar10 = (longlong *)FUN_140347b50(0xeb);
  cVar4 = (char)uVar15;
  (**(code **)(*plVar10 + 0x80))(plVar10,uVar15);
  plVar10 = (longlong *)FUN_140347b50(0xec);
  if ((cVar4 == '\0') && (cVar6 == '\0')) {
    uVar11 = 0;
  }
  else {
    uVar11 = 1;
  }
  (**(code **)(*plVar10 + 0x80))(plVar10,uVar11);
  plVar10 = (longlong *)FUN_140347b50(0xed);
  (**(code **)(*plVar10 + 0x80))(plVar10,cVar6);
  plVar10 = (longlong *)FUN_140347b50(0xee);
  (**(code **)(*plVar10 + 0x80))(plVar10,cVar6);
  plVar10 = (longlong *)FUN_140347b50(0xef);
  (**(code **)(*plVar10 + 0x80))(plVar10,cVar6);
  plVar10 = (longlong *)FUN_140347b50(0xf0);
  if ((cVar4 == '\0') && (cVar6 == '\0')) {
    (**(code **)(*plVar10 + 0x80))(plVar10,0);
  }
  else {
    (**(code **)(*plVar10 + 0x80))(plVar10,1);
    if (cVar4 != '\0') {
      *(undefined2 *)(param_1 + 0x2060) = 0;
      *(undefined1 *)(param_1 + 0x2063) = 0;
    }
    if (cVar6 != '\0') {
      *(undefined2 *)(param_1 + 0x2062) = 0;
      *(undefined1 *)(param_1 + 0x2060) = 0;
    }
  }
  plVar10 = (longlong *)FUN_140347b50(0xe9);
  if (plVar10 != (longlong *)0x0) {
    (**(code **)(*plVar10 + 0x80))(plVar10,*(int *)(DAT_140b15418 + 0x168) == 0);
  }
  lVar13 = FUN_140294a40(&DAT_140a16fd0);
  if (*(char *)(lVar13 + 0x3c0) != '\0') {
    plVar10 = (longlong *)FUN_140347b50(0x26d);
    if (plVar10 != (longlong *)0x0) {
      (**(code **)(*plVar10 + 0x80))(plVar10,1);
    }
    plVar10 = (longlong *)FUN_140347b50(0x26c);
    if (plVar10 != (longlong *)0x0) {
      (**(code **)(*plVar10 + 0x80))(plVar10,1);
    }
  }
  return;
}




// ================================================================
// TIMER SUBSYSTEM CANDIDATES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x215b90  Name: FUN_140215b90  Size: 474 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140215b90(float param_1)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined1 *puVar3;
  undefined1 local_4a8 [144];
  undefined1 local_418 [1040];
  
  if (DAT_140a7cc34 != '\0') {
    if (0.0 < DAT_140a7cc3c) {
      if (DAT_140a7cc38 < 1) {
        if (_DAT_140a15120 < param_1 / DAT_140a7cc3c) {
          DAT_140a7cc40 = DAT_140a7cc40 + 1;
          FUN_140048ec0(local_418,PTR_s__Slow_Frame_Profile__03d_xml_140a15128);
          FUN_140216150(0,"<Profile_Tree>\n");
          FUN_140216060(DAT_140a7cc28,1);
          FUN_140216150(0,"</Profile_Tree>\n");
          FUN_1402130d0(local_4a8,local_418);
          FUN_140213840(local_4a8,1);
          puVar3 = &DAT_140a15130;
          if (0xf < DAT_140a15148) {
            puVar3 = _DAT_140a15130;
          }
          FUN_140213a50(local_4a8,puVar3,_DAT_140a15140 & 0xffffffff);
          FUN_1402138d0(local_4a8);
          _DAT_140a15140 = 0;
          puVar3 = &DAT_140a15130;
          if (0xf < DAT_140a15148) {
            puVar3 = _DAT_140a15130;
          }
          *puVar3 = 0;
          FUN_1402132f0(local_4a8);
          DAT_140a7cc38 = 0x14;
        }
      }
      else {
        DAT_140a7cc38 = DAT_140a7cc38 + -1;
      }
    }
    puVar2 = DAT_140a7cc28;
    if (DAT_140a7cc34 != '\0') {
      for (; puVar2 != (undefined8 *)0x0; puVar2 = (undefined8 *)*puVar2) {
        puVar1 = (undefined8 *)puVar2[2];
        *(undefined4 *)(puVar2 + 4) = 0;
        puVar2[5] = 0;
        for (; puVar1 != (undefined8 *)0x0; puVar1 = (undefined8 *)*puVar1) {
          FUN_140215dd0(puVar1);
        }
      }
    }
    DAT_140a7cc3c = param_1;
    return;
  }
  DAT_140a7cc3c = param_1;
  return;
}



// ------------------------------------------------------------
// RVA: 0x1a7370  Name: FUN_1401a7370  Size: 891 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_1401a7370(int param_1)

{
  DWORD DVar1;
  LARGE_INTEGER LVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  LARGE_INTEGER local_res10 [3];
  
  if (DAT_140a705e0 == 0) {
    DAT_140a705e0 = 1;
    DAT_140a12bb0 = QueryPerformanceFrequency(local_res10);
    if (DAT_140a12bb0 == 0) goto LAB_1401a7577;
    DAT_140a705e8.s = local_res10[0].s;
  }
  if (DAT_140a12bb0 != 0) {
    if (((DAT_140a705f0.QuadPart == 0) || (param_1 == 1)) || (LVar2 = DAT_140a705f0, param_1 == 4))
    {
      QueryPerformanceCounter(local_res10);
      LVar2 = local_res10[0];
    }
    if (param_1 == 6) {
      lVar3 = LVar2.QuadPart - _DAT_140a705f8;
      _DAT_140a705f8 = LVar2.s;
      return (float)lVar3 / (float)DAT_140a705e8.QuadPart;
    }
    if (param_1 == 5) {
      return (float)(LVar2.QuadPart - _DAT_140a70600) / (float)DAT_140a705e8.QuadPart;
    }
    if (param_1 == 0) {
      _DAT_140a70600 = LVar2.s;
      DAT_140a705f0 = (LARGE_INTEGER)0x0;
      DAT_140a12bb4 = 0;
      _DAT_140a705f8 = LVar2.s;
      return 0.0;
    }
    if (param_1 == 1) {
      if (DAT_140a12bb4 != 0) {
        _DAT_140a70600 = _DAT_140a70600 + (LVar2.QuadPart - (longlong)DAT_140a705f0);
      }
      _DAT_140a705f8 = LVar2.s;
      DAT_140a705f0 = (LARGE_INTEGER)0x0;
      DAT_140a12bb4 = 0;
      return 0.0;
    }
    if (param_1 == 2) {
      if (DAT_140a12bb4 != 0) {
        return 0.0;
      }
      DAT_140a705f0.s = LVar2.s;
      _DAT_140a705f8 = LVar2.s;
      DAT_140a12bb4 = 1;
      return 0.0;
    }
    if (param_1 == 3) {
      DAT_140a705f0.QuadPart = DAT_140a705f0.QuadPart + DAT_140a705e8.QuadPart / 10;
      return 0.0;
    }
    if (param_1 != 4) {
      return DAT_14080080c;
    }
    return (float)LVar2.QuadPart / (float)DAT_140a705e8.QuadPart;
  }
LAB_1401a7577:
  if (((DAT_140a70610 == 0.0) || (param_1 == 1)) || (fVar5 = DAT_140a70610, param_1 == 4)) {
    DVar1 = timeGetTime();
    fVar5 = (float)((double)DVar1 * _DAT_14081c078);
  }
  if (param_1 == 6) {
    fVar4 = fVar5 - _DAT_140a70608;
    _DAT_140a70608 = fVar5;
    return fVar4;
  }
  if (param_1 == 5) {
    fVar5 = fVar5 - DAT_140a7060c;
  }
  else {
    if (param_1 == 0) {
      DAT_140a7060c = fVar5;
      DAT_140a12bb4 = 0;
      _DAT_140a70608 = fVar5;
      DAT_140a70610 = (float)param_1;
      return 0.0;
    }
    if (param_1 == 1) {
      if (DAT_140a12bb4 != 0) {
        DAT_140a7060c = DAT_140a7060c + (fVar5 - DAT_140a70610);
      }
      _DAT_140a70608 = fVar5;
      DAT_140a12bb4 = 0;
      DAT_140a70610 = 0.0;
      return 0.0;
    }
    if (param_1 == 2) {
      if (DAT_140a12bb4 == 0) {
        DAT_140a12bb4 = 1;
        _DAT_140a70608 = fVar5;
        DAT_140a70610 = fVar5;
      }
      return 0.0;
    }
    if (param_1 == 3) {
      DAT_140a70610 = DAT_140a70610 + DAT_1408007b4;
      return 0.0;
    }
    if (param_1 != 4) {
      return DAT_14080080c;
    }
  }
  return fVar5;
}



// ------------------------------------------------------------
// RVA: 0x222e20  Name: FUN_140222e20  Size: 99 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140222e20(void)

{
  LARGE_INTEGER local_res8;
  LARGE_INTEGER local_res10 [3];
  
  QueryPerformanceFrequency(&local_res8);
  _DAT_140a7e2f0 = local_res8.s;
  DAT_140a7e2f8 = local_res8.QuadPart / 1000;
  DAT_140a7e2e8 = 1;
  QueryPerformanceCounter(local_res10);
  _DAT_140a7e300 = local_res10[0];
  return;
}



// ------------------------------------------------------------
// RVA: 0x222fa0  Name: FUN_140222fa0  Size: 38 bytes
// ------------------------------------------------------------

DWORD FUN_140222fa0(void)

{
  LARGE_INTEGER local_res8 [4];
  
  if (DAT_140a7e2e8 == 0) {
    FUN_140222e20();
  }
  QueryPerformanceCounter(local_res8);
  return local_res8[0].s.LowPart;
}




// ================================================================
// LOAD THREAD ENTRY POINT
// ================================================================

// ------------------------------------------------------------
// RVA: 0x8df00  Name: FUN_14008df00  Size: 2340 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14008df00(int *param_1)

{
  float fVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong *plVar4;
  char cVar5;
  int iVar6;
  DWORD DVar7;
  ulonglong uVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  uint uVar14;
  longlong lVar15;
  longlong lVar16;
  int iVar17;
  undefined **local_res10;
  char *local_c8 [2];
  undefined8 local_b8;
  ulonglong local_b0;
  undefined **local_a8;
  char local_9f;
  longlong *local_80;
  undefined4 local_78;
  undefined4 local_74;
  char local_70;
  ulonglong local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  int local_48;
  undefined8 local_40;
  
  cVar5 = FUN_1401c1e90(&DAT_140a78510,DAT_140a110d4);
  plVar4 = DAT_140b15418;
  if (cVar5 == '\0') {
    return;
  }
  iVar6 = *param_1;
  if (iVar6 == 0) {
LAB_14008df5a:
    if (DAT_140b15418 != (longlong *)0x0) {
      (**(code **)(*DAT_140b15418 + 0x40))();
    }
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 == 1) {
    plVar11 = (longlong *)(param_1 + 2);
    if (0xf < *(ulonglong *)(param_1 + 8)) {
      plVar11 = *(longlong **)(param_1 + 2);
    }
    if (*(longlong *)(param_1 + 6) != 0x10) {
      return;
    }
    if (*plVar11 != 0x494745425f434449) {
      return;
    }
    if (plVar11[1] != 0x4e4f545455425f4e) {
      return;
    }
    if (DAT_140a28c20 == '\0') {
      if (DAT_140b15418 == (longlong *)0x0) goto LAB_14008e800;
    }
    else {
      FUN_1402d2790(&DAT_140b27dc0);
      FUN_14021cf60(&DAT_140a7d0a0,0);
      FUN_14021ce30(&DAT_140a7d0a0,0,0,0);
      if (plVar4 == (longlong *)0x0) goto LAB_14008e800;
      *(undefined1 *)((longlong)plVar4 + 0x9a) = 0;
    }
    (**(code **)(*plVar4 + 0x40))(plVar4);
LAB_14008e800:
    DAT_140b0a354 = 0;
    FUN_14008ddc0();
    return;
  }
  if (iVar6 != 0xc) {
    if (iVar6 != 0xe) {
      return;
    }
    if ((param_1[0xb] != 0x20) && (param_1[0xb] != 0xd)) {
      return;
    }
    goto LAB_14008df5a;
  }
  if (DAT_140a61d39 == '\0') {
    return;
  }
  DAT_140a61a68 = &LAB_14008e8c0;
  DAT_140a110d0 = 0;
  FUN_14021d0f0(&DAT_140a7d0a0,DAT_140b30328,DAT_140b3032c,DAT_140b30330,DAT_140b30334);
  if (DAT_140a61df8 != (longlong *)0x0) {
    iVar6 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar6 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar6 == 0) {
      FUN_14007fa80();
    }
  }
  plVar4 = DAT_140a61df8;
  uVar9 = DAT_140a61d58;
  iVar6 = DAT_140a61d54;
  uVar3 = DAT_140a110dc;
  uVar2 = DAT_140a110d8;
  puVar10 = &DAT_140a110e8;
  if (0xf < DAT_140a11100) {
    puVar10 = DAT_140a110e8;
  }
  cVar5 = DAT_140a61d3a;
  FUN_14022e310(&local_a8,"LoadThread");
  local_a8 = LoadingThreadClass::vftable;
  local_80 = (longlong *)0x0;
  local_68[0] = 0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  if (plVar4 != (longlong *)0x0) {
    local_80 = plVar4;
    (**(code **)*plVar4)(plVar4);
  }
  lVar15 = -1;
  local_78 = uVar3;
  lVar16 = -1;
  local_74 = uVar2;
  do {
    lVar16 = lVar16 + 1;
  } while (*(char *)((longlong)puVar10 + lVar16) != '\0');
  local_70 = cVar5;
  FUN_140022730(local_68,puVar10);
  local_48 = iVar6;
  local_40 = uVar9;
  local_c8[0] = (char *)FUN_140769c58(0x20);
  uVar9 = s_IDC_BEGIN_BUTTON_1408052c8._8_8_;
  local_b8 = 0x10;
  local_b0 = 0x1f;
  *(undefined8 *)local_c8[0] = s_IDC_BEGIN_BUTTON_1408052c8._0_8_;
  *(undefined8 *)(local_c8[0] + 8) = uVar9;
  local_c8[0][0x10] = '\0';
  FUN_1401c30d0(&DAT_140a78510,DAT_140a110d4,local_c8,0);
  if (0xf < local_b0) {
    if (0xfff < local_b0 + 1) {
      if ((char *)0x1f < local_c8[0] + (-8 - *(longlong *)(local_c8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_c8[0] + -8),local_b0 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DAT_140a61d50 = DAT_140a61dd4;
  DAT_140a61d4c = 0;
  DAT_140a61d39 = 1;
  _DAT_140a61d88 = 0;
  uRam0000000140a61d90 = 0;
  _DAT_140a61d80 = 0;
  _DAT_140a61d98 = 0;
  uRam0000000140a61da0 = 0;
  _DAT_140a61da8 = 0;
  _DAT_140a61d60 = 0;
  uRam0000000140a61d68 = 0;
  _DAT_140a61d70 = 0;
  uRam0000000140a61d78 = 0;
  FUN_14022e490(&local_a8);
  while (local_9f == '\0') {
    FUN_14008e9a0();
    FUN_1400ca620();
  }
  FUN_14022e560(&local_a8);
  DVar7 = timeGetTime();
  fVar1 = DAT_1407ffaf8;
  uVar13 = 0;
  iVar17 = DVar7 - _DAT_140a7bec0;
  uVar12 = uVar13;
  iVar6 = iVar17;
  if ((DAT_140a61d3a != '\0') || (((DAT_140a61d54 - 1U & 0xfffffffc) == 0 && (DAT_140a61d54 != 3))))
  {
    while ((DVar7 = timeGetTime(), (DVar7 - _DAT_140a7bec0) - iVar17 < 0xea61 ||
           (DVar7 = timeGetTime(), (DVar7 - _DAT_140a7bec0) - iVar6 < 0xafc9))) {
      FUN_14008e9a0();
      uVar8 = uVar13;
      while ((*(int *)((longlong)&DAT_140a61db0 + uVar8) == -1 ||
             (*(float *)((longlong)&DAT_140a61d88 + uVar8) == fVar1))) {
        uVar8 = uVar8 + 4;
        if (0x23 < (longlong)uVar8) {
          if ((DAT_140a61d3a == '\0') &&
             (((DAT_140a61d54 - 1U & 0xfffffffc) != 0 || (DAT_140a61d54 == 3)))) {
            DAT_140a61d50 = (DAT_140a110e0 - DAT_140a61dd4) + DAT_140a61dd4;
          }
          else {
            _DAT_140a61d88 = CONCAT44(DAT_140a61d8c,0x3f800000);
            if (((DAT_140a61d54 - 1U & 0xfffffffc) == 0) && (DAT_140a61d54 != 3)) {
              FUN_1400ca410();
            }
          }
          goto LAB_14008e46b;
        }
      }
      DVar7 = timeGetTime();
      uVar14 = DVar7 - _DAT_140a7bec0;
      if (5000 < uVar14 - (int)uVar12) {
        if ((DAT_140a61d3a == '\0') &&
           (((DAT_140a61d54 - 1U & 0xfffffffc) != 0 || (DAT_140a61d54 == 3)))) {
          DAT_140a61d50 = (DAT_140a110e0 - DAT_140a61dd4) + DAT_140a61dd4;
        }
        else {
          _DAT_140a61d88 = CONCAT44(DAT_140a61d8c,0x3f800000);
          if (((DAT_140a61d54 - 1U & 0xfffffffc) == 0) && (DAT_140a61d54 != 3)) {
            FUN_1400ca410();
          }
        }
        uVar12 = (ulonglong)uVar14;
      }
      FUN_1402089e0();
      uVar2 = DAT_140b0a320;
      FUN_1403b08c0(&DAT_140b2ed18);
      FUN_1402c2910(&DAT_140b15690,uVar2);
      FUN_1404908c0(&PTR_vftable_140a2aaa0);
      if (DAT_1409cf0a0 == 0) {
        lVar16 = FUN_140769c58(0x18);
        if (lVar16 == 0) {
          DAT_1409cf0a0 = 0;
        }
        else {
          DAT_1409cf0a0 = FUN_14006a000(lVar16);
        }
        local_res10 = &PTR_vftable_1409cf098;
        if (DAT_140a7bc10 == DAT_140a7bc18) {
          FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res10);
        }
        else {
          *DAT_140a7bc10 = &PTR_vftable_1409cf098;
          DAT_140a7bc10 = DAT_140a7bc10 + 1;
        }
      }
      FUN_14006a1e0(DAT_1409cf0a0);
      uVar9 = FUN_140025770();
      FUN_14006d1d0(uVar9);
      cVar5 = FUN_1400ca620();
      if (cVar5 != '\0') {
        DVar7 = timeGetTime();
        iVar6 = DVar7 - _DAT_140a7bec0;
      }
    }
  }
LAB_14008e46b:
  FUN_14008eff0();
  FUN_14013cb90();
  if (DAT_140b15418 != (longlong *)0x0) {
    if (DAT_140a11118 == 0) {
      puVar10 = &DAT_140a110e8;
      if (0xf < DAT_140a11100) {
        puVar10 = DAT_140a110e8;
      }
      do {
        lVar15 = lVar15 + 1;
      } while (*(char *)((longlong)puVar10 + lVar15) != '\0');
    }
    else {
      puVar10 = &DAT_140a11108;
      if (0xf < DAT_140a11120) {
        puVar10 = DAT_140a11108;
      }
      do {
        lVar15 = lVar15 + 1;
      } while (*(char *)((longlong)puVar10 + lVar15) != '\0');
    }
    FUN_140022730(DAT_140b15418 + 0xe,puVar10,lVar15);
  }
  FUN_1402a8730(DAT_140b15418[3]);
  FUN_14029dc30(DAT_140b15418[4],0);
  FUN_14028a4d0(&DAT_140b153e0);
  FUN_14021d130(&DAT_140a7d0a0,DAT_140b30328,DAT_140b3032c,DAT_140b30330,DAT_140b30334);
  if (DAT_140a61d3a == '\0') {
LAB_14008e558:
    cVar5 = FUN_14028af60(&DAT_140b153e0);
    if (cVar5 == '\0') {
      local_c8[0] = (char *)FUN_140769c58(0x20);
      uVar9 = s_IDC_BEGIN_BUTTON_1408052c8._8_8_;
      local_b8 = 0x10;
      local_b0 = 0x1f;
      *(undefined8 *)local_c8[0] = s_IDC_BEGIN_BUTTON_1408052c8._0_8_;
      *(undefined8 *)(local_c8[0] + 8) = uVar9;
      local_c8[0][0x10] = '\0';
      lVar15 = FUN_1401c29a0(&DAT_140a78510,DAT_140a110d4,local_c8);
      if (0xf < local_b0) {
        if (0xfff < local_b0 + 1) {
          if ((char *)0x1f < local_c8[0] + (-8 - *(longlong *)(local_c8[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_c8[0] + -8),local_b0 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      if (lVar15 == 0) {
        if (DAT_140a61d3a == '\0') {
          if (DAT_140b15418 != (longlong *)0x0) {
            (**(code **)(*DAT_140b15418 + 0x40))();
          }
          DAT_140b0a354 = 0;
        }
        FUN_14008ddc0();
      }
      DAT_140a61a68 = (undefined1 *)0x0;
      local_a8 = LoadingThreadClass::vftable;
      if (local_80 != (longlong *)0x0) {
        (**(code **)(*local_80 + 8))();
        local_80 = (longlong *)0x0;
      }
      if (0xf < uStack_50) {
        uVar13 = uStack_50 + 1;
        uVar12 = local_68[0];
        if (0xfff < uVar13) {
          uVar12 = *(ulonglong *)(local_68[0] - 8);
          uVar13 = uStack_50 + 0x28;
          if (0x1f < (local_68[0] - uVar12) - 8) goto LAB_14008e815;
        }
        thunk_FUN_1407864b8(uVar12,uVar13);
      }
      goto LAB_14008e5ec;
    }
  }
  else {
    cVar5 = FUN_14028af60(&DAT_140b153e0);
    if (cVar5 == '\0') {
      FUN_1400eed80();
      goto LAB_14008e558;
    }
    FUN_1400c9e10();
  }
  FUN_14008ddc0();
  local_a8 = LoadingThreadClass::vftable;
  DAT_140a61a68 = (undefined1 *)0x0;
  if (local_80 != (longlong *)0x0) {
    (**(code **)(*local_80 + 8))();
    local_80 = (longlong *)0x0;
  }
  if (0xf < uStack_50) {
    if (0xfff < uStack_50 + 1) {
      uVar12 = *(ulonglong *)(local_68[0] - 8);
      uVar13 = uStack_50 + 0x28;
      if (0x1f < (local_68[0] - uVar12) - 8) {
LAB_14008e815:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar12,uVar13);
      }
    }
    thunk_FUN_1407864b8();
  }
LAB_14008e5ec:
  local_68[0] = local_68[0] & 0xffffffffffffff00;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  if (local_80 != (longlong *)0x0) {
    (**(code **)(*local_80 + 8))();
  }
  FUN_14022e3e0(&local_a8);
  return;
}



