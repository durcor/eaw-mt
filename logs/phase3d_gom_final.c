
// ================================================================
// XREFS TO DAT_140b2b3a8 and DAT_140b2b2f0 (guard_check_icall targets)
// ================================================================

// --- xrefs to DAT_0xb2b3a8 ---
//   DATA from FUN_0x28f320 @ site 0x28f33b
//   DATA from FUN_0x5f6b0 @ site 0x5fa6f
//   DATA from FUN_0x2af7c0 @ site 0x2afb09
//   DATA from FUN_0x28e890 @ site 0x28e98e
//   DATA from FUN_0x28d940 @ site 0x28d970
//   DATA from FUN_0x5d990 @ site 0x5ea5f
//   DATA from FUN_0x4da50 @ site 0x4db75
//   DATA from FUN_0x6ad230 @ site 0x6ad264
//   DATA from FUN_0x6ad230 @ site 0x6ad5dc
//   DATA from FUN_0x6ac1b0 @ site 0x6ac1e5
//   DATA from FUN_0x4b250 @ site 0x4c552
//   DATA from FUN_0x1098e0 @ site 0x1098e9
//   DATA from FUN_0x1089e0 @ site 0x108c13
//   DATA from FUN_0x107900 @ site 0x107ac6
//   DATA from FUN_0x105d30 @ site 0x105d39
//   DATA from FUN_0x105190 @ site 0x1051f5
//   DATA from FUN_0x1053c0 @ site 0x1055ac
//   DATA from FUN_0x22f10 @ site 0x231e4
//   DATA from FUN_0x4623e0 @ site 0x46241a
//   DATA from FUN_0x4608c0 @ site 0x460937
//   DATA from FUN_0x60c60 @ site 0x60ef4
//   DATA from FUN_0x60c60 @ site 0x60f72
//   DATA from FUN_0x2b0520 @ site 0x2b0784
//   WRITE from FUN_0x7ef6a0 @ site 0x7ef6a2
// ------------------------------------------------------------
// RVA: 0x28f320  Name: FUN_14028f320  Size: 499 bytes
// ------------------------------------------------------------

void FUN_14028f320(longlong param_1,char param_2)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong *plVar5;
  int iVar6;
  undefined **local_res8;
  
  if (*(longlong *)(param_1 + 0x38) != 0) {
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_1402d6f20(&DAT_140b27e60);
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140323660(&DAT_140b2b3b0);
    if (DAT_140a10b50 == 0) {
      lVar4 = FUN_140769c58(0x78);
      if (lVar4 == 0) {
        DAT_140a10b50 = 0;
      }
      else {
        DAT_140a10b50 = FUN_140221820(lVar4);
      }
      local_res8 = &PTR_vftable_140a10b48;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_140a10b48;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140220ed0(DAT_140a10b50,param_1,0x24,*(undefined8 *)(param_1 + 0x38));
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x28))();
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x10))();
    plVar5 = *(longlong **)(param_1 + 0x48);
    if (plVar5 != (longlong *)0x0) {
      iVar1 = *(int *)(param_1 + 0x50);
      iVar6 = 0;
      if (0 < iVar1) {
        do {
          if (*plVar5 == *(longlong *)(param_1 + 0x38)) {
            if ((-1 < iVar6) && (iVar6 < iVar1)) {
              FUN_14025c3c0(param_1 + 0x40,iVar6,iVar6 + 1,(iVar1 - iVar6) + -1);
              *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + -1;
            }
            break;
          }
          iVar6 = iVar6 + 1;
          plVar5 = plVar5 + 1;
        } while (iVar6 < iVar1);
      }
    }
    puVar2 = *(undefined8 **)(param_1 + 0x38);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)*puVar2)(puVar2,1);
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined1 *)(param_1 + 0x1e5) = 0;
    FUN_1403027d0(&DAT_140b27f60);
    if (*(int *)(param_1 + 0x50) != 0) {
      plVar5 = (longlong *)**(undefined8 **)(param_1 + 0x48);
      *(longlong **)(param_1 + 0x38) = plVar5;
      (**(code **)(*plVar5 + 0x58))();
      (**(code **)(**(longlong **)(param_1 + 0x38) + 0x30))();
      uVar3 = (**(code **)(**(longlong **)(param_1 + 0x38) + 0xe0))();
      *(undefined4 *)(param_1 + 0x1c) = uVar3;
      FUN_14027bf80(&DAT_140b0a320);
      return;
    }
    FUN_14047f690();
    FUN_1402566d0();
    DAT_140b0a354 = 1;
    if (param_2 != '\0') {
      FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,0);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x5f6b0  Name: FUN_14005f6b0  Size: 1779 bytes
// ------------------------------------------------------------

void FUN_14005f6b0(char param_1)

{
  char cVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  int iVar10;
  ulonglong uVar11;
  longlong *plVar12;
  undefined8 *puVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  uint uVar16;
  bool bVar17;
  longlong local_c8 [2];
  longlong local_b8;
  ulonglong local_b0;
  ulonglong local_a8 [2];
  undefined8 local_98;
  ulonglong local_90;
  ulonglong local_88 [2];
  undefined8 local_78;
  ulonglong local_70;
  ulonglong local_68 [2];
  undefined8 local_58;
  ulonglong local_50;
  longlong local_48 [2];
  undefined8 local_38;
  ulonglong local_30;
  
  cVar1 = FUN_14028af60(&DAT_140b153e0);
  if (cVar1 == '\0') {
    uVar3 = FUN_140294a70(&DAT_140a16fd0);
    iVar10 = DAT_140a2ab00._4_4_ * 2 >> 1;
    if (iVar10 <= (int)DAT_140a2ab00) {
      iVar4 = iVar10 / 10;
      if (iVar10 / 10 < 10) {
        iVar4 = 10;
      }
      FUN_140021ec0(&PTR_vftable_140a2aaf0,iVar4 + iVar10,0,1);
    }
    *(undefined4 *)(DAT_140a2aaf8 + (longlong)(int)DAT_140a2ab00 * 4) = uVar3;
    DAT_140a2ab00._0_4_ = (int)DAT_140a2ab00 + 1;
  }
  cVar1 = DAT_140b155c4;
  uVar11 = 0;
  uVar15 = uVar11;
  if (0 < (int)DAT_140a2ab00) {
    do {
      iVar10 = *(int *)(uVar15 + DAT_140a2aaf8);
      iVar4 = FUN_140294a70(&DAT_140a16fd0);
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,iVar10);
      local_c8[0] = 0;
      local_b8 = 0;
      local_b0 = 7;
      if (lVar6 != 0) {
        cVar2 = FUN_1402940c0(&DAT_140a16fd0,lVar6,1);
        if (cVar2 != '\0') {
          FUN_14005fe90(lVar6);
        }
        lVar7 = FUN_140294a40(&DAT_140a16fd0);
        if (lVar6 != lVar7) {
          plVar12 = (longlong *)(lVar6 + 0x28);
          if (local_c8 != plVar12) {
            if (7 < *(ulonglong *)(lVar6 + 0x40)) {
              plVar12 = (longlong *)*plVar12;
            }
            FUN_1400228a0(local_c8,plVar12,*(undefined8 *)(lVar6 + 0x38));
          }
          FUN_14002d480(local_c8,&DAT_1408005e4,1);
          puVar8 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,"TEXT_PLAYER_LEFT_GAME");
          puVar13 = puVar8;
          if (7 < (ulonglong)puVar8[3]) {
            puVar13 = (undefined8 *)*puVar8;
          }
          FUN_14002d480(local_c8,puVar13,puVar8[2]);
          uVar9 = FUN_140281940(lVar6);
          FUN_1401f9320(local_a8,uVar9);
          FUN_140049810(&DAT_1409cf320,local_c8,local_a8,0,0,0);
          FUN_1401f9440(local_a8);
        }
      }
      iVar5 = FUN_140294af0(&DAT_140a16fd0);
      bVar17 = iVar10 == iVar4 || iVar5 < 2;
      if ((DAT_140b15418 != (longlong *)0x0) &&
         (iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar10 == 0)) {
        cVar2 = FUN_14028af60(&DAT_140b153e0);
        if ((cVar2 != '\0') && (cVar1 == '\0')) {
          FUN_140092bb0(&DAT_140a10d88,1);
        }
        bVar17 = true;
      }
      cVar2 = FUN_14028b060(&DAT_140b153e0);
      if (((cVar2 != '\0') ||
          ((cVar2 = FUN_14028afb0(&DAT_140b153e0), cVar2 != '\0' &&
           (cVar2 = FUN_14028af60(&DAT_140b153e0), cVar2 != '\0')))) &&
         ((bVar17 || (iVar10 = FUN_140294af0(&DAT_140a16fd0), iVar10 < 2)))) {
        if (cVar1 == '\0') {
          FUN_140087770(DAT_140b15418,1);
        }
LAB_14005f9b8:
        iVar10 = DAT_140b153f8;
        if (DAT_140a61ab2 != '\0') {
          FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,&DAT_140a10be0);
          DAT_140a61ab2 = '\0';
        }
        FUN_1400a3c40();
        thunk_FUN_14003f220(&DAT_140a60df0);
        FUN_1400337b0(&DAT_140a60df0);
        FUN_14028d940(&DAT_140b153e0);
        FUN_140297630(&DAT_140a16fd0,0);
        DAT_140b155e8 = 0;
        FUN_1400338e0(&DAT_140a60df0);
        FUN_140049350(&DAT_1409cf320);
        FUN_140049350(&DAT_1409cf390);
        FUN_1403027d0(&DAT_140b27f60);
        DAT_140a61b0d = 0;
        FUN_140069890();
        uVar9 = FUN_14001fb80();
        FUN_14007e040(uVar9,"players 0");
        FUN_140322780(&DAT_140b2b3a8);
        FUN_1403207c0(&DAT_140b2b2f0);
        FUN_140321830(&DAT_140b2b300,1);
        FUN_1402d2790(&DAT_140b27dc0);
        FUN_1402d6f20(&DAT_140b27e60);
        FUN_140323660(&DAT_140b2b3b0);
        if (((DAT_140a61c80 == '\0') && (DAT_140a61e08 == '\0')) && (DAT_140a61fb8 == '\0')) {
          if (DAT_140a61f20 != '\0') {
            FUN_1400b1c00();
          }
          FUN_1401c1e00(&DAT_140a78510);
          if ((iVar10 == 1) && (cVar1 == '\0')) {
            FUN_1400b1240();
            if (local_b8 != 0) {
              local_48[0] = 0;
              local_38 = 0;
              local_30 = 7;
              FUN_1400228a0(local_48,&PTR_1407ff7e8,0);
              local_68[0] = 0;
              local_58 = 0;
              local_50 = 7;
              FUN_1400228a0(local_68,&PTR_1407ff7e8,0);
              local_88[0] = 0;
              local_78 = 0;
              local_70 = 7;
              FUN_1400228a0(local_88,L"TEXT_BUTTON_OK",0xe);
              local_a8[0] = 0;
              local_98 = 0;
              local_90 = 7;
              FUN_1400228a0(local_a8,&PTR_1407ff7e8,0);
              FUN_140099d90(&DAT_140065c90,local_a8,local_88,local_68,local_48,local_c8,0,0,0,1);
              if (7 < local_90) {
                uVar15 = local_90 * 2 + 2;
                uVar11 = local_a8[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_a8[0] - 8);
                  uVar15 = local_90 * 2 + 0x29;
                  if (0x1f < (local_a8[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_98 = 0;
              local_90 = 7;
              local_a8[0] = local_a8[0] & 0xffffffffffff0000;
              if (7 < local_70) {
                uVar15 = local_70 * 2 + 2;
                uVar11 = local_88[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_88[0] - 8);
                  uVar15 = local_70 * 2 + 0x29;
                  if (0x1f < (local_88[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_78 = 0;
              local_70 = 7;
              local_88[0] = local_88[0] & 0xffffffffffff0000;
              if (7 < local_50) {
                uVar15 = local_50 * 2 + 2;
                uVar11 = local_68[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_68[0] - 8);
                  uVar15 = local_50 * 2 + 0x29;
                  if (0x1f < (local_68[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_58 = 0;
              local_50 = 7;
              local_68[0] = local_68[0] & 0xffffffffffff0000;
              if (7 < local_30) {
                if (0xfff < local_30 * 2 + 2) {
                  uVar11 = *(ulonglong *)(local_48[0] + -8);
                  uVar15 = local_30 * 2 + 0x29;
                  if (0x1f < (local_48[0] - uVar11) - 8) {
LAB_14005fd9f:
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(uVar11,uVar15);
                  }
                }
                thunk_FUN_1407864b8();
              }
            }
          }
          else if ((DAT_140b303d0 == 0) && (param_1 == '\x01')) {
            FUN_1400c58e0();
          }
        }
        if (local_b0 < 8) {
          DAT_140a2ab00._0_4_ = 0;
          return;
        }
        uVar14 = local_b0 * 2 + 2;
        lVar6 = local_c8[0];
        if (0xfff < uVar14) {
          lVar6 = *(longlong *)(local_c8[0] + -8);
          uVar14 = local_b0 * 2 + 0x29;
          if (0x1f < (local_c8[0] - lVar6) - 8U) {
LAB_14005fd99:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(lVar6,uVar14);
          }
        }
        thunk_FUN_1407864b8(lVar6,uVar14);
        DAT_140a2ab00._0_4_ = 0;
        return;
      }
      if (bVar17) goto LAB_14005f9b8;
      if (7 < local_b0) {
        if (0xfff < local_b0 * 2 + 2) {
          lVar6 = *(longlong *)(local_c8[0] + -8);
          uVar14 = local_b0 * 2 + 0x29;
          if (0x1f < (local_c8[0] - lVar6) - 8U) goto LAB_14005fd99;
        }
        thunk_FUN_1407864b8();
      }
      uVar16 = (int)uVar11 + 1;
      uVar11 = (ulonglong)uVar16;
      uVar15 = uVar15 + 4;
    } while ((int)uVar16 < (int)DAT_140a2ab00);
  }
  DAT_140a2ab00._0_4_ = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x2af7c0  Name: FUN_1402af7c0  Size: 1096 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402af7c0(longlong param_1,longlong param_2,int param_3,int param_4,longlong param_5,
                  char param_6)

{
  undefined ***pppuVar1;
  undefined4 uVar2;
  char cVar3;
  int iVar4;
  HANDLE hHeap;
  undefined8 *puVar5;
  ulonglong uVar6;
  undefined8 *******pppppppuVar7;
  LPVOID lpMem;
  undefined8 *******pppppppuVar8;
  longlong lVar9;
  longlong lVar10;
  undefined4 *puVar11;
  int iVar12;
  undefined8 uVar13;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  int local_74;
  undefined4 local_70;
  undefined8 *******local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  if (param_2 == 0) {
    return;
  }
  puVar5 = (undefined8 *)(*(longlong *)(param_2 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(param_2 + 0x298) + 0x110)) {
    puVar5 = (undefined8 *)*puVar5;
  }
  FUN_140025760("GameObjectManagerClass::Initiate_Space_Conflict -- Planet %s\n",puVar5);
  local_98 = *(undefined4 *)(param_2 + 0x50);
  local_88 = DynamicVectorClass<int>::vftable;
  local_90 = -1;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  local_74 = -0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 *******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  uVar13 = _DAT_140802a40;
  local_94 = param_3;
  if (param_5 != 0) {
    lVar9 = -1;
    do {
      lVar9 = lVar9 + 1;
    } while (*(char *)(param_5 + lVar9) != '\0');
    uVar13 = FUN_140022730(local_68);
  }
  pppppppuVar8 = local_68[0];
  lpMem = local_80;
  uVar2 = local_98;
  lVar9 = *(longlong *)(param_2 + 0xb8);
  if (param_4 == -1) {
    param_4 = -1;
  }
  if ((lVar9 == 0) || (param_4 != -1)) {
LAB_1402af91d:
    if (param_4 == -1) goto LAB_1402af922;
  }
  else {
    iVar12 = 0;
    if (0 < *(int *)(lVar9 + 0xd0)) {
      lVar10 = 0;
      do {
        lVar9 = *(longlong *)(lVar10 + *(longlong *)(lVar9 + 200));
        if (((lVar9 != 0) &&
            (cVar3 = FUN_1403a9760(lVar9), uVar13 = extraout_XMM0_Qa, cVar3 != '\x01')) &&
           (param_4 = *(int *)(lVar9 + 0x58), param_4 != param_3)) goto LAB_1402af91d;
        lVar9 = *(longlong *)(param_2 + 0xb8);
        iVar12 = iVar12 + 1;
        lVar10 = lVar10 + 8;
      } while (iVar12 < *(int *)(lVar9 + 0xd0));
    }
LAB_1402af922:
    if (((*(int *)(*(longlong *)(param_2 + 0xb8) + 0x148) == 0) ||
        (param_4 = *(int *)(*(longlong *)(param_2 + 0xb8) + 0x13c), param_4 == param_3)) ||
       (param_4 == -1)) {
      iVar12 = local_74;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
        if (0xfff < uStack_50 + 1) {
          pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
          uVar6 = uStack_50 + 0x28;
          if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7)))
          goto LAB_1402afc03;
        }
        thunk_FUN_1407864b8(pppppppuVar7);
        iVar12 = local_74;
      }
      goto joined_r0x0001402afbaf;
    }
  }
  local_90 = param_4;
  if (param_6 == '\0') {
    iVar12 = *(int *)(param_1 + 0x478);
    iVar4 = *(int *)(param_1 + 0x47c) * 2 >> 1;
    if (iVar4 <= iVar12) {
      iVar12 = iVar4 / 10;
      if (iVar4 / 10 < 10) {
        iVar12 = 10;
      }
      FUN_1402b6fa0(param_1 + 0x468,iVar12 + iVar4,0,1);
      iVar12 = *(int *)(param_1 + 0x478);
    }
    puVar11 = (undefined4 *)((longlong)iVar12 * 0x58 + *(longlong *)(param_1 + 0x470));
    *(int *)(param_1 + 0x478) = iVar12 + 1;
    *puVar11 = uVar2;
    puVar11[1] = local_94;
    puVar11[2] = param_4;
    pppuVar1 = (undefined ***)(puVar11 + 4);
    iVar12 = local_74;
    if (pppuVar1 != &local_88) {
      FUN_14001f910(pppuVar1);
      iVar12 = local_74;
      puVar11[9] = local_74;
      iVar4 = local_74 * 2 >> 1;
      if (iVar4 != 0) {
        FUN_140021ec0(pppuVar1,iVar4,0,1);
        if (0 < (int)local_78) {
          uVar6 = (ulonglong)local_78;
          lVar9 = 0;
          do {
            *(undefined4 *)(lVar9 + *(longlong *)(puVar11 + 6)) =
                 *(undefined4 *)((longlong)lpMem + lVar9);
            uVar6 = uVar6 - 1;
            lVar9 = lVar9 + 4;
          } while (uVar6 != 0);
        }
        puVar11[8] = local_78;
      }
    }
    puVar11[10] = local_70;
    if ((undefined8 ********)(puVar11 + 0xc) != local_68) {
      pppppppuVar7 = local_68;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
      }
      FUN_140022730(puVar11 + 0xc,pppppppuVar7,local_58);
      lpMem = local_80;
      pppppppuVar8 = local_68[0];
      iVar12 = local_74;
    }
    *(undefined1 *)(puVar11 + 0x14) = local_48;
  }
  else {
    FUN_14029bb40(param_1 + 0x468,&local_98);
    lpMem = local_80;
    pppppppuVar8 = local_68[0];
    iVar12 = local_74;
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar13 = extraout_XMM0_Qa_00;
  if ((param_3 == iVar4) && (DAT_140b155d4 == param_3)) {
    lVar9 = FUN_140371450(*(undefined8 *)(param_2 + 0x298),DAT_140b155d8);
    if (lVar9 == 0) {
      lVar9 = FUN_140294a40(&DAT_140a16fd0);
      lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x68) + 0x650);
      uVar13 = extraout_XMM0_Qa_01;
      if (lVar9 != 0) {
        uVar13 = FUN_1402d5290(&DAT_140b27e60,lVar9,0,0,0);
      }
    }
    else {
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,0);
      uVar13 = FUN_140321170(&DAT_140b2b300,lVar9);
    }
  }
  if (0xf < uStack_50) {
    uVar6 = uStack_50 + 1;
    pppppppuVar7 = pppppppuVar8;
    if (0xfff < uVar6) {
      pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
      uVar6 = uStack_50 + 0x28;
      if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7))) {
LAB_1402afc03:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar13,uVar6);
      }
    }
    thunk_FUN_1407864b8(pppppppuVar7,uVar6);
  }
joined_r0x0001402afbaf:
  if (lpMem != (LPVOID)0x0) {
    if (iVar12 < 0) {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,lpMem);
    }
    else {
      thunk_FUN_1407864b8(lpMem);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x28e890  Name: FUN_14028e890  Size: 1189 bytes
// ------------------------------------------------------------

void FUN_14028e890(longlong param_1,int param_2,longlong *param_3)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  DWORD DVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  int iVar9;
  longlong *plVar10;
  undefined **local_res8;
  longlong local_48 [3];
  ulonglong local_30;
  
  plVar8 = (longlong *)0x0;
  *(undefined1 *)(param_1 + 0xb1) = 1;
  DAT_140a2ab00._0_4_ = 0;
  *(undefined8 *)(param_1 + 0x1d0) = 0;
  *(undefined1 *)(param_1 + 0x1d8) = 0;
  *(undefined4 *)(param_1 + 0x1dc) = 0;
  *(undefined2 *)(param_1 + 0x1e1) = 0;
  *(undefined1 *)(param_1 + 0x1e0) = 0;
  *(undefined1 *)(param_1 + 0x1e4) = 0;
  uVar5 = FUN_14028a6f0();
  FUN_1404e67d0(uVar5);
  if (param_3 == (longlong *)0x0) {
    plVar10 = (longlong *)&DAT_14085af28;
  }
  else {
    DAT_140b313e8 = 0;
    plVar10 = param_3;
    if (0xf < (ulonglong)param_3[3]) {
      plVar10 = (longlong *)*param_3;
    }
  }
  puVar6 = (undefined8 *)
           (**(code **)(*DAT_140b310b0 + 0x10))(DAT_140b310b0,local_48,(longlong)param_2);
  if (0xf < (ulonglong)puVar6[3]) {
    puVar6 = (undefined8 *)*puVar6;
  }
  FUN_140025760("GameModeManagerClass::Start_Game -- Starting %s Mode: %s.\n",puVar6,plVar10);
  if (0xf < local_30) {
    if (0xfff < local_30 + 1) {
      if (0x1f < (local_48[0] - *(longlong *)(local_48[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  if (*(int *)(param_1 + 500) == -1) {
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_140321830(&DAT_140b2b300,0);
  }
  FUN_140323660(&DAT_140b2b3b0);
  FUN_1403567c0(&DAT_140b2bcb0);
  bVar3 = false;
  if (*(int *)(param_1 + 0x50) == 0) {
    bVar3 = true;
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_14027d020(&DAT_140b0a320);
    DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe0);
    if (*(char *)(param_1 + 0xe8) != '\0') {
      DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe4);
    }
    *(undefined4 *)(param_1 + 0x2c) = 0;
    uVar5 = FUN_14028a5d0();
    FUN_1404d9d40(uVar5);
    FUN_140293390(&DAT_140a16fd0,0);
    FUN_1404907c0(param_1 + 0x180,"PENDING_TACTICAL_BATTLE_VOTE");
    FUN_1404908b0(param_1 + 0x180);
  }
  else {
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x50))();
  }
  if (param_2 == 0) {
    lVar7 = FUN_140769c58(0x378);
    if (lVar7 == 0) goto LAB_14028ec28;
    plVar8 = (longlong *)FUN_1404b1270(lVar7);
  }
  else {
    if (param_2 == 1) {
      lVar7 = FUN_140769c58(0x430);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1403b5210(lVar7);
      }
    }
    else {
      if (param_2 != 2) goto LAB_14028ec28;
      lVar7 = FUN_140769c58(0x3c8);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1404d6ba0(lVar7);
      }
    }
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    DVar4 = timeGetTime();
    *(DWORD *)(param_1 + 0x1e8) = DVar4;
  }
  if (plVar8 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x50);
    iVar9 = *(int *)(param_1 + 0x54) * 2 >> 1;
    if (iVar9 <= iVar1) {
      iVar2 = iVar9 / 10;
      if (iVar9 / 10 < 10) {
        iVar2 = 10;
      }
      FUN_140047aa0(param_1 + 0x40,iVar2 + iVar9,0,CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),1));
      iVar1 = *(int *)(param_1 + 0x50);
    }
    if (0 < iVar1) {
      FUN_14025c3c0(param_1 + 0x40,1);
    }
    **(undefined8 **)(param_1 + 0x48) = plVar8;
    *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + 1;
    (**(code **)(*plVar8 + 8))(plVar8,*(undefined4 *)(param_1 + 0x18),param_2);
    *(longlong **)(param_1 + 0x38) = plVar8;
    (**(code **)(*plVar8 + 0x18))(plVar8);
    (**(code **)(*plVar8 + 0x30))(plVar8);
    if (DAT_1409cf1c0 == 0) {
      lVar7 = FUN_140769c58(0x10c8);
      if (lVar7 == 0) {
        DAT_1409cf1c0 = 0;
      }
      else {
        DAT_1409cf1c0 = FUN_140262010(lVar7);
      }
      local_res8 = &PTR_vftable_1409cf1b8;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_1409cf1b8;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140262af0(DAT_1409cf1c0);
    *(int *)(param_1 + 0x1c) = param_2;
    if ((param_3 != (longlong *)0x0) && (param_2 == 0)) {
      *(undefined *)(plVar8 + 0x68) = *(undefined *)((longlong)param_3 + 0xcb);
      if (plVar8 + 0x69 != param_3) {
        plVar10 = param_3;
        if (0xf < (ulonglong)param_3[3]) {
          plVar10 = (longlong *)*param_3;
        }
        FUN_140022730(plVar8 + 0x69,plVar10,param_3[2]);
      }
    }
  }
LAB_14028ec28:
  if ((bVar3) && (param_2 != 0)) {
    FUN_140292050(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140291f70(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140292590(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140291d80(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3);
    FUN_1402b09b0(*(undefined8 *)(DAT_140b15418 + 0x18),param_3);
    iVar1 = *(int *)(param_1 + 0x18);
    if (((iVar1 - 1U & 0xfffffffc) != 0) || (iVar1 == 3)) {
      FUN_1402924d0(&DAT_140a16fd0,iVar1,param_3,0);
    }
  }
  if (((*(int *)(param_1 + 0x18) - 1U & 0xfffffffc) != 0) || (*(int *)(param_1 + 0x18) == 3)) {
    FUN_14027bf00(&DAT_140b0a320,DAT_140a7c398,DAT_140a7c39c);
  }
  FUN_14027bf80(&DAT_140b0a320);
  DAT_140b0a354 = 0;
  *(undefined1 *)(param_1 + 0xb1) = 0;
  FUN_14028c640(param_1 + 0xf0);
  return;
}



// ------------------------------------------------------------
// RVA: 0x28d940  Name: FUN_14028d940  Size: 667 bytes
// ------------------------------------------------------------

void FUN_14028d940(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong *plVar5;
  int iVar6;
  undefined **local_res8;
  
  lVar4 = *(longlong *)(param_1 + 0x38);
  do {
    if (lVar4 == 0) {
      FUN_14027d570(&DAT_140b0a320);
      FUN_1404b3b10(&DAT_140b2ed18);
      FUN_1402c1e30(&DAT_140b15690);
      *(undefined4 *)(param_1 + 0x1c) = 0xffffffff;
      *(undefined8 *)(param_1 + 0x38) = 0;
      *(undefined8 *)(param_1 + 0x24) = 0;
      *(undefined1 *)(param_1 + 0xb1) = 0;
      *(undefined4 *)(param_1 + 0x80) = 1;
      *(undefined4 *)(param_1 + 0x20) = 0;
      if (*(longlong *)(param_1 + 0x88) != 0) {
        FUN_140260f10(&DAT_140b0a130);
        *(undefined8 *)(param_1 + 0x88) = 0;
      }
      if (*(longlong *)(param_1 + 0x90) != 0) {
        FUN_140260f10(&DAT_140b0a130);
        *(undefined8 *)(param_1 + 0x90) = 0;
      }
      FUN_1404080d0(&DAT_140b2f020);
      FUN_1404dc640();
      *(undefined8 *)(param_1 + 500) = 0xffffffffffffffff;
      *(undefined4 *)(param_1 + 0x1fc) = 0;
      return;
    }
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_1402d6f20(&DAT_140b27e60);
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140323660(&DAT_140b2b3b0);
    if (DAT_140a10b50 == 0) {
      lVar4 = FUN_140769c58(0x78);
      if (lVar4 == 0) {
        DAT_140a10b50 = 0;
      }
      else {
        DAT_140a10b50 = FUN_140221820(lVar4);
      }
      local_res8 = &PTR_vftable_140a10b48;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_140a10b48;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140220ed0(DAT_140a10b50,param_1,0x24,*(undefined8 *)(param_1 + 0x38));
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x28))();
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x10))();
    plVar5 = *(longlong **)(param_1 + 0x48);
    if ((plVar5 != (longlong *)0x0) && (iVar1 = *(int *)(param_1 + 0x50), 0 < iVar1)) {
      iVar6 = 0;
      do {
        if (*plVar5 == *(longlong *)(param_1 + 0x38)) {
          if ((-1 < iVar6) && (iVar6 < iVar1)) {
            FUN_14025c3c0(param_1 + 0x40,iVar6,iVar6 + 1,(iVar1 - iVar6) + -1);
            *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + -1;
          }
          break;
        }
        iVar6 = iVar6 + 1;
        plVar5 = plVar5 + 1;
      } while (iVar6 < iVar1);
    }
    puVar2 = *(undefined8 **)(param_1 + 0x38);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)*puVar2)(puVar2,1);
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined1 *)(param_1 + 0x1e5) = 0;
    FUN_1403027d0(&DAT_140b27f60);
    if (*(int *)(param_1 + 0x50) == 0) {
      FUN_14047f690();
      FUN_1402566d0();
      DAT_140b0a354 = 1;
      FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,0);
    }
    else {
      plVar5 = (longlong *)**(undefined8 **)(param_1 + 0x48);
      *(longlong **)(param_1 + 0x38) = plVar5;
      (**(code **)(*plVar5 + 0x58))();
      (**(code **)(**(longlong **)(param_1 + 0x38) + 0x30))();
      uVar3 = (**(code **)(**(longlong **)(param_1 + 0x38) + 0xe0))();
      *(undefined4 *)(param_1 + 0x1c) = uVar3;
      FUN_14027bf80(&DAT_140b0a320);
    }
    lVar4 = *(longlong *)(param_1 + 0x38);
  } while( true );
}



// ------------------------------------------------------------
// RVA: 0x5d990  Name: FUN_14005d990  Size: 7440 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x00014005f137) */
/* WARNING: Removing unreachable block (ram,0x00014005f161) */
/* WARNING: Removing unreachable block (ram,0x00014005f166) */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14005d990(HINSTANCE param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  byte *pbVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  uint uVar5;
  uint uVar6;
  longlong *plVar7;
  char cVar8;
  char cVar9;
  ATOM AVar10;
  DWORD DVar11;
  BOOL BVar12;
  undefined4 uVar13;
  LONG X;
  int iVar14;
  int iVar15;
  undefined8 uVar16;
  longlong lVar17;
  undefined8 *puVar18;
  ulonglong uVar19;
  longlong lVar20;
  HWND hWnd;
  undefined8 uVar21;
  FILE *_File;
  char *pcVar22;
  HANDLE hProcess;
  LONG LVar23;
  undefined1 *puVar24;
  ulonglong uVar25;
  undefined8 *puVar26;
  LONG LVar27;
  HINSTANCE pHVar28;
  ulonglong uVar29;
  ulonglong uVar30;
  LONG Y;
  uint uVar31;
  float fVar32;
  ulonglong in_stack_fffffffffffff9f8;
  undefined8 in_stack_fffffffffffffa00;
  undefined4 uVar33;
  tagRECT local_5b8;
  undefined1 local_5a8 [16];
  ulonglong local_598;
  undefined4 local_590;
  char local_58c;
  undefined1 local_58b;
  undefined8 local_588;
  ulonglong local_580;
  DWORD local_578;
  undefined8 local_574;
  ulonglong local_568 [2];
  undefined8 local_558;
  ulonglong local_550;
  ulonglong local_548 [2];
  undefined8 local_538;
  ulonglong local_530;
  longlong local_528;
  undefined **local_520;
  WNDCLASSEXA local_518;
  undefined8 local_4c8;
  undefined4 local_4c0;
  tagMSG local_4b8;
  char local_488 [271];
  undefined1 uStack_379;
  byte local_378 [512];
  byte local_178 [320];
  
  uVar33 = (undefined4)((ulonglong)in_stack_fffffffffffffa00 >> 0x20);
  puVar18 = (undefined8 *)0x0;
  local_4b8.wParam = 0;
  DAT_140a619d8 = param_1;
  strncpy((char *)&local_518,"952F41D039DB4507A84B40488E76722E 0123456789ABCDEIPA",0x33);
  FUN_14021db70();
  cVar8 = FUN_14007f880();
  if (cVar8 == '\0') {
    return 0;
  }
  uVar16 = FUN_140067c80();
  cVar8 = FUN_14006a050(uVar16);
  if (cVar8 == '\0') {
    return 0;
  }
  DAT_140a13e20 = FUN_140222fa0();
  _DAT_140a2ab18 = FUN_140222fa0();
  cVar8 = FUN_140067d70(param_3);
  if ((DAT_140a61aaa == '\0') && (DAT_140a61aab == '\0')) {
    puVar26 = puVar18;
    if ((DAT_140a10d70 != 0) && (puVar26 = &DAT_140a10d60, 0xf < DAT_140a10d78)) {
      puVar26 = DAT_140a10d60;
    }
    cVar9 = FUN_14021dce0(puVar26);
    if (cVar9 == '\0') {
      return 0;
    }
  }
  DVar11 = GetModuleFileNameA((HMODULE)0x0,(LPSTR)local_378,0x104);
  if (DVar11 == 0) {
LAB_14005db21:
    GetLastError();
  }
  else {
    in_stack_fffffffffffff9f8 = 0;
    _splitpath((char *)local_378,(char *)local_178,local_488,(char *)0x0,(char *)0x0);
    puVar26 = puVar18;
    do {
      pbVar1 = local_178 + (longlong)puVar26;
      local_378[(longlong)puVar26] = *pbVar1;
      puVar26 = (undefined8 *)((longlong)puVar26 + 1);
    } while (*pbVar1 != 0);
    puVar24 = &uStack_379;
    do {
      pcVar22 = puVar24 + 1;
      puVar24 = puVar24 + 1;
    } while (*pcVar22 != '\0');
    lVar17 = 0;
    do {
      cVar9 = local_488[lVar17];
      puVar24[lVar17] = cVar9;
      lVar17 = lVar17 + 1;
    } while (cVar9 != '\0');
    BVar12 = SetCurrentDirectoryA((LPCSTR)local_378);
    if (BVar12 == 0) goto LAB_14005db21;
  }
  lVar17 = FUN_140769c58(0x38);
  if (lVar17 != 0) {
    puVar18 = (undefined8 *)FUN_140216a50(lVar17);
  }
  DAT_140a7d080 = puVar18;
  if (DAT_140a15330 != 0) {
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    FUN_140217740(puVar18,puVar26);
    lVar17 = DAT_140a15330;
    if (0x7fffffffffffffffU - DAT_140a15330 < 0xe) {
                    /* WARNING: Subroutine does not return */
      FUN_140022610();
    }
    local_5b8._0_8_ = (undefined8 *)0x0;
    puVar18 = (undefined8 *)&local_5b8;
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    uVar30 = 0xf;
    local_5a8 = ZEXT816(0);
    uVar29 = DAT_140a15330 + 0xe;
    if (0xf < uVar29) {
      uVar30 = uVar29 | 0xf;
      if (uVar30 < 0x8000000000000000) {
        if (uVar30 < 0x16) {
          uVar30 = 0x16;
        }
        if (uVar30 + 1 < 0x1000) {
          if (uVar30 == 0xffffffffffffffff) {
            puVar18 = (undefined8 *)0x0;
            local_5b8._0_8_ = puVar18;
          }
          else {
            puVar18 = (undefined8 *)FUN_140769c58();
            local_5b8._0_8_ = puVar18;
          }
          goto LAB_14005dc48;
        }
        uVar19 = uVar30 + 0x28;
        if (uVar19 <= uVar30 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
      }
      else {
        uVar30 = 0x7fffffffffffffff;
        uVar19 = 0x8000000000000027;
      }
      lVar20 = FUN_140769c58(uVar19);
      if (lVar20 == 0) goto LAB_14005f660;
      puVar18 = (undefined8 *)(lVar20 + 0x27U & 0xffffffffffffffe0);
      puVar18[0xffffffffffffffff] = lVar20;
      local_5b8._0_8_ = puVar18;
    }
LAB_14005dc48:
    local_5a8._8_8_ = uVar30;
    local_5a8._0_8_ = uVar29;
    FUN_14076bd80(puVar18,puVar26,lVar17);
    *(undefined8 *)((longlong)puVar18 + lVar17) = s_Data_Patch_meg_140802db8._0_8_;
    *(undefined4 *)((longlong)puVar18 + lVar17 + 8) = s_Data_Patch_meg_140802db8._8_4_;
    *(undefined2 *)((longlong)puVar18 + lVar17 + 0xc) = s_Data_Patch_meg_140802db8._12_2_;
    *(undefined1 *)((longlong)puVar18 + lVar17 + 0xeU) = 0;
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\patch2.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f654;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f654:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\64Patch.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f65a;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f65a:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    if (0xf < (ulonglong)local_5a8._8_8_) {
      uVar29 = local_5a8._8_8_ + 1;
      puVar18 = (undefined8 *)local_5b8._0_8_;
      if (0xfff < uVar29) {
        puVar18 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
        uVar29 = local_5a8._8_8_ + 0x28;
        if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar18))) {
LAB_14005f660:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(puVar18,uVar29);
    }
  }
  FUN_140217740(DAT_140a7d080,&DAT_140802dec);
  FUN_140216d20(DAT_140a7d080,"Data\\Patch.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\patch2.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\64Patch.meg",0,0);
  if (DAT_140a15330 != 0) {
    FUN_140214110(&DAT_140a15320);
  }
  DAT_140a7c090 = FUN_140200090();
  FUN_140067590();
  local_518.cbSize = 0;
  local_518.style = 0;
  local_518.cbClsExtra = _DAT_140802a40;
  local_518.cbWndExtra = _UNK_140802a44;
  local_518.hInstance = _UNK_140802a48;
  FUN_140022730(&local_518,"Arial Unicode MS",0x10);
  FUN_140270020(&local_518);
  local_580 = 0xf;
  local_588 = 0xb;
  local_598._0_1_ = s_EmpireAtWar_140802e08[0];
  local_598._1_1_ = s_EmpireAtWar_140802e08[1];
  local_598._2_1_ = s_EmpireAtWar_140802e08[2];
  local_598._3_1_ = s_EmpireAtWar_140802e08[3];
  local_598._4_1_ = s_EmpireAtWar_140802e08[4];
  local_598._5_1_ = s_EmpireAtWar_140802e08[5];
  local_598._6_1_ = s_EmpireAtWar_140802e08[6];
  local_598._7_1_ = s_EmpireAtWar_140802e08[7];
  local_590 = (uint)CONCAT12(s_EmpireAtWar_140802e08[10],s_EmpireAtWar_140802e08._8_2_);
  FUN_140270020(&local_598);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_520,"Software\\Petroglyph",1,1,in_stack_fffffffffffff9f8);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_528,"Software\\Petroglyph\\StarWars FOC",1,1,in_stack_fffffffffffff9f8);
  FUN_14021f740(&local_578,"Software\\Petroglyph\\StarWars FOC\\Profiles",1,1,
                in_stack_fffffffffffff9f8 & 0xffffffffffffff00);
  FUN_14021f810(&local_578);
  FUN_14021f810(&local_528);
  FUN_14021f810(&local_520);
  if (((DAT_140a61b0f == '\0') &&
      (DAT_140a10cb8 = CreateMutexA((LPSECURITY_ATTRIBUTES)0x0,1,"EAWIsRunning"),
      DAT_140a10cb8 != (HANDLE)0xffffffffffffffff)) && (DVar11 = GetLastError(), DVar11 == 0xb7)) {
    ReleaseMutex(DAT_140a10cb8);
    DAT_140a10cb8 = (HANDLE)0xffffffffffffffff;
    hWnd = FindWindowA("TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
    if (hWnd == (HWND)0x0) {
      return 0;
    }
    SetForegroundWindow(hWnd);
    return 0;
  }
  if ((0xb < DAT_140a10bb0 + 1U) || ((0x9c1U >> (DAT_140a10bb0 + 1U & 0x1f) & 1) == 0)) {
    DAT_140a7c090 = DAT_140a10bb0;
  }
  if (cVar8 == '\0') {
    if (DAT_140a10cb8 == (HANDLE)0xffffffffffffffff) {
      return 0;
    }
    ReleaseMutex(DAT_140a10cb8);
    return 0;
  }
  local_578 = 0x103;
  GetUserNameA(local_488,&local_578);
  local_578 = 0x103;
  GetComputerNameA((LPSTR)local_178,&local_578);
  FUN_140025760("     %s\n\n",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
  uVar16 = FUN_14021dae0();
  FUN_140025760("     Version : %s\n",uVar16);
  uVar16 = FUN_14021d7e0(0,0);
  uVar21 = FUN_14021d740();
  uVar13 = FUN_14021d780();
  FUN_140025760("     Build   : %d by %s at %s\n",uVar13,uVar21,uVar16);
  CoInitialize((LPVOID)0x0);
  lVar17 = -1;
  do {
    lVar17 = lVar17 + 1;
  } while (*(short *)(PTR_u_Star_Wars_R___Empire_at_War_TM___140a10ca8 + lVar17 * 2) != 0);
  FUN_1400228a0(&DAT_140a15560);
  SetCursor((HCURSOR)0x0);
  timeGetDevCaps((LPTIMECAPS)&DAT_140a61b10,8);
  timeBeginPeriod(DAT_140a61b10);
  FUN_140499340();
  FUN_1402029c0(&DAT_140a7c230);
  FUN_1400517b0(&PTR_vftable_140a10a20);
  FUN_14021eef0(&PTR_vftable_140a15360);
  if (DAT_140a61a9b == '\0') {
    DAT_140a61aac = DAT_140a7c441;
  }
  DAT_1409cf308 = DAT_140a61ae8;
  if ((DAT_140a61ae8 == 0) && (DAT_1409cf308 = DAT_140a7c350, DAT_140a61aac != '\0')) {
    DAT_1409cf308 = DAT_140a7c35c;
  }
  DAT_1409cf30c = DAT_140a61aec;
  if ((DAT_140a61aec == 0) && (DAT_1409cf30c = DAT_140a7c354, DAT_140a61aac != '\0')) {
    DAT_1409cf30c = DAT_140a7c360;
  }
  DAT_1409cf310 = DAT_140a7c358;
  if (DAT_140a61af0 != 0) {
    DAT_1409cf310 = DAT_140a61af0;
  }
  FUN_140307830(&DAT_140b27f60);
  if ((DAT_140a7c43c != '\0') || (DAT_140a61aac == '\0')) {
    param_4 = 3;
  }
  if (DAT_140a61a9e == '\0') {
    FUN_140036d60(&DAT_140a60df0,DAT_140a7c46d);
  }
  FUN_1404913a0(FUN_14004a220,FUN_140038150,&LAB_1400381e0,FUN_140072170);
  DAT_140a61a28 = FUN_14004e480();
  DAT_140a60e28 = DAT_140a7c43f;
  FUN_140307830(&DAT_140b27f60,DAT_1409cf308,DAT_1409cf30c);
  DAT_140a61af4 = 0;
  DAT_140a619d8 = param_1;
  EnumDisplayMonitors((HDC)0x0,(LPCRECT)0x0,FUN_140065ad0,0);
  local_518.lpfnWndProc = FUN_140060c60;
  local_518.cbSize = 0x50;
  local_518.style = 0x200b;
  local_518.cbClsExtra = 0;
  local_518.cbWndExtra = 0;
  local_518.hInstance = param_1;
  local_518.hIcon = LoadIconA(param_1,(LPCSTR)0x83);
  local_518.hbrBackground = _DAT_140803530;
  local_518.lpszMenuName = _UNK_140803538;
  local_518.hCursor = (HCURSOR)0x0;
  local_518.lpszClassName = "TESTWINCLASS";
  local_518.hIconSm = LoadIconA(local_518.hInstance,(LPCSTR)0x84);
  AVar10 = RegisterClassExA(&local_518);
  if (AVar10 == 0) goto LAB_14005f55c;
  local_5b8.bottom = DAT_1409cf30c;
  local_5b8.right = DAT_1409cf308;
  local_5b8._0_8_ = (undefined8 *)0x0;
  iVar15 = 0;
  iVar14 = 0;
  if (-1 < DAT_140a10bb4) {
    local_5b8.top = DAT_140a61afc;
    local_5b8.left = DAT_140a61af8;
    local_5b8.bottom = DAT_1409cf30c + DAT_140a61afc;
    local_5b8.right = DAT_1409cf308 + DAT_140a61af8;
    iVar15 = DAT_140a61afc;
    iVar14 = DAT_140a61af8;
  }
  AdjustWindowRect(&local_5b8,0xcf0000,0);
  Y = local_5b8.top;
  LVar27 = local_5b8.bottom;
  if (local_5b8.top < iVar15) {
    Y = local_5b8.top + (iVar15 - local_5b8.top);
    LVar27 = local_5b8.bottom + (iVar15 - local_5b8.top);
    local_5b8.top = Y;
    local_5b8.bottom = LVar27;
  }
  X = local_5b8.left;
  LVar23 = local_5b8.right;
  if (local_5b8.left < iVar14) {
    X = local_5b8.left + (iVar14 - local_5b8.left);
    LVar23 = local_5b8.right + (iVar14 - local_5b8.left);
    local_5b8.left = X;
    local_5b8.right = LVar23;
  }
  uVar30 = 0;
  uVar29 = CONCAT44(uVar33,Y);
  DAT_140a619d0 =
       CreateWindowExA(0,"TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0,0xcf0000,X,
                       Y,LVar23 - X,LVar27 - Y,(HWND)0x0,(HMENU)0x0,param_1,(LPVOID)0x0);
  if (DAT_140a619d0 == (HWND)0x0) goto LAB_14005f55c;
  ShowWindow(DAT_140a619d0,param_4);
  UpdateWindow(DAT_140a619d0);
  FUN_140214a00(DAT_140a619d0);
  cVar8 = FUN_14004b250();
  if (cVar8 != '\x01') goto LAB_14005f55c;
  puVar18 = &DAT_140a10d40;
  if (0xf < DAT_140a10d58) {
    puVar18 = DAT_140a10d40;
  }
  _File = (FILE *)FUN_14078013c(puVar18,&DAT_140803298);
  if (_File != (FILE *)0x0) {
    local_378[0] = 0;
    pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    while (pcVar22 != (char *)0x0) {
      if ((0x2d < local_378[0]) ||
         ((0x200000002401U >> ((longlong)(char)local_378[0] & 0x3fU) & 1) == 0)) {
        uVar16 = FUN_14001fb80();
        FUN_14007e040(uVar16,local_378);
      }
      pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    }
    fclose(_File);
  }
  if (DAT_140a61a50 != (undefined8 *)0x0) {
    (**(code **)*DAT_140a61a50)(DAT_140a61a50,1);
    DAT_140a61a50 = (undefined8 *)0x0;
  }
  if (DAT_140a61a99 == '\0') {
    if (DAT_140a61a9a == '\0') {
      if (DAT_140a10d10 == 0) {
        DAT_140a61aa8 = DAT_140a61ab7;
        _guard_check_icall(&DAT_140b27f60);
        if ((DAT_140a61ace == '\0') && (DAT_140a619ef == '\0')) {
          if (DAT_140a61a9f == '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,0);
            uVar29 = uVar29 & 0xffffffffffffff00;
            FUN_14044d920(&DAT_140b306dd,"Logo1",0,FUN_140065780,1,uVar29);
          }
          else {
            FUN_1400c58e0();
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
        }
      }
      else {
        FUN_140053c10(&PTR_vftable_140a10b18);
        cVar8 = FUN_140055250(&PTR_vftable_140a10b18,&DAT_140a10d00);
        if (cVar8 == '\0') {
          FUN_140033850(&DAT_140a60df0);
          puVar18 = &DAT_140a10d00;
          if (0xf < DAT_140a10d18) {
            puVar18 = DAT_140a10d00;
          }
          cVar8 = FUN_140052d10(&PTR_vftable_140a10b18,0,puVar18,0);
          pcVar22 = (char *)FUN_14004fba0();
          if (*pcVar22 != '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
          if (cVar8 != '\0') goto LAB_14005e71d;
        }
        DAT_140a619ef = '\x01';
      }
    }
    else {
      cVar8 = FUN_140069040();
      if (cVar8 == '\0') {
        DAT_140a619ef = '\x01';
      }
    }
  }
  else {
    cVar8 = FUN_140068b20();
    if (cVar8 == '\0') {
      DAT_140a619ef = '\x01';
    }
  }
LAB_14005e71d:
  fVar4 = DAT_14080350c;
  fVar3 = DAT_1408007f8;
  if (DAT_140a61b0e != '\0') {
    DAT_140a14d01 = 1;
  }
  if (DAT_140a619ef == '\0') {
    iVar15 = 0;
    local_528 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    do {
      lVar17 = local_528;
      if ((((DAT_140b2f1d8 == '\x01') && (DAT_140b2f1d8 = '\0', DAT_140a61a98 == '\0')) &&
          (DAT_140a61a98 = '\x01', DAT_140a61ae0 == 0)) && (DAT_140a61acc == '\0')) {
        FUN_14025fb10(2);
        DAT_140a61acc = '\x01';
      }
      local_5b8._0_8_ = (undefined8 *)0x0;
      local_5b8._8_8_ = local_5b8._8_8_ & 0xffffffff00000000;
      local_4c8 = 0;
      local_4c0 = 0x3f800000;
      local_578 = 0;
      local_574 = 0x3f800000;
      if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x158))(DAT_140b15418,&local_5b8,&local_4c8,&local_578);
      }
      iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      while (iVar14 != 0) {
        if (local_4b8.message == 0x12) {
          DAT_140a619ef = '\x01';
          break;
        }
        iVar14 = TranslateAcceleratorA(local_4b8.hwnd,(HACCEL)0x0,&local_4b8);
        if (iVar14 == 0) {
          TranslateMessage(&local_4b8);
          DispatchMessageA(&local_4b8);
        }
        iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      }
      cVar8 = thunk_FUN_1401768f0();
      if (((cVar8 != '\0') && (DAT_1409cf314 != '\0')) &&
         ((DAT_140a61b08 != 0 &&
          (DVar11 = timeGetTime(), 1000 < (DVar11 - DAT_140a61b08) - _DAT_140a7bec0)))) {
        FUN_140066980();
        uVar16 = FUN_14004fba0();
        FUN_140262af0(uVar16);
        DAT_140a61b08 = 0;
      }
      thunk_FUN_14024bb80();
      FUN_1402505c0();
      uVar5 = DAT_140b0a320;
      FUN_1402089e0();
      FUN_1403b08c0(&DAT_140b2ed18);
      FUN_1402c2910(&DAT_140b15690,uVar5);
      if (((DAT_140b15418 != (longlong *)0x0) &&
          (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0')) && (DAT_140b1574e == '\0')) {
        FUN_1402c0c70(&DAT_140b15690,uVar5);
      }
      FUN_1404908c0(&PTR_vftable_140a2aaa0);
      FUN_1400caa60();
      if (((byte)uVar5 & 0x3f) == 0x3f) {
        DVar11 = timeGetTime();
        FUN_14021e7b0(DVar11 - _DAT_140a7bec0);
        FUN_1400503d0(&DAT_140a109b0);
      }
      FUN_140051d10(&DAT_140a10a98);
      if ((*(int *)(lVar17 + 0xd4) < DAT_140a61b1c) &&
         (_Init_thread_header(&DAT_140a61b1c), DAT_140a61b1c == -1)) {
        DVar11 = timeGetTime();
        _DAT_140a61b18 = DVar11 - _DAT_140a7bec0;
        _Init_thread_footer(&DAT_140a61b1c);
      }
      DVar11 = timeGetTime();
      iVar14 = DVar11 - _DAT_140a7bec0;
      uVar31 = iVar14 - _DAT_140a61b18;
      _DAT_140a61b18 = iVar14;
      FUN_140215b90((float)uVar31 * fVar4);
      FUN_140339bc0(&DAT_140b2ba20,iVar14);
      FUN_140022c50(&DAT_1409cf080,iVar14);
      uVar16 = FUN_140067c80();
      FUN_14006a1e0(uVar16);
      uVar16 = FUN_140025770();
      FUN_14006d1d0(uVar16);
      if (DAT_1409cf314 == '\x01') {
        uVar29 = uVar29 & 0xffffffffffffff00;
        FUN_14021caf0(&DAT_140a7d0a0,iVar14,&local_5b8,&local_4c8,&local_578,uVar29);
        FUN_1402d72c0(&DAT_140b27e60,iVar14,uVar5);
        FUN_1402d2ab0(&DAT_140b27dc0,iVar14,uVar5);
        FUN_140321dc0(&DAT_140b2b300,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b3a8,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b2f0,iVar14,uVar5);
        FUN_140325190(&DAT_140b2b3b0,iVar14);
      }
      _guard_check_icall(&DAT_140a2a120,iVar14);
      FUN_1401c8420(&DAT_140a78510);
      cVar8 = thunk_FUN_1401768f0();
      if (cVar8 != '\0') {
        cVar8 = FUN_14044d840(&DAT_140b306dd);
        if (((cVar8 != '\0') && (cVar8 = FUN_14044d780(&DAT_140b306dd), cVar8 != '\0')) &&
           (FUN_14044f130(&DAT_140b306dd), DAT_140a111e0 != 0)) {
          uVar29 = uVar29 & 0xffffffffffffff00;
          puVar18 = &DAT_140a111d0;
          if (0xf < DAT_140a111e8) {
            puVar18 = DAT_140a111d0;
          }
          FUN_14044d920(&DAT_140b306dd,puVar18,0,0,1,uVar29);
        }
        FUN_14030c3b0(&DAT_140b27f60,iVar14);
        if (DAT_1409cf314 != '\0') {
          FUN_14002ee40(&DAT_140a60df0,iVar14);
        }
        if (DAT_1409cf314 != '\0') {
          FUN_140305890(&DAT_140b27f60);
        }
        FUN_1400493f0(&DAT_1409cf320,iVar14);
        FUN_1400493f0(&DAT_1409cf390);
      }
      if ((DAT_140b0a355 != '\0' || DAT_140b1574c != '\0') && (DAT_140a61b0d == '\0')) {
        if (DAT_140a61e60 != '\0') {
          FUN_14009aa80();
        }
        if ((DAT_140b1574e == '\0') || (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,&PTR_1407ff7e8,0);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,&local_598,local_548,local_568,&local_518,0,0,0,0,uVar30);
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f6a2:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            goto LAB_14005efe5;
          }
        }
        else {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,L"TEXT_BUTTON_CONTINUE_AFTER_SYNC",0x1f);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,local_568,local_548,&local_598,&local_518,0,0,0,0,uVar30);
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f648:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
LAB_14005efe5:
            thunk_FUN_1407864b8();
          }
        }
        DAT_140a61b0d = '\x01';
        FUN_14027c9c0(&DAT_140b0a320);
        if (DAT_140a61ab2 != '\0') {
          local_590._0_1_ = s_oosevents_log_140802f20[8];
          local_590._1_1_ = s_oosevents_log_140802f20[9];
          local_590._2_1_ = s_oosevents_log_140802f20[10];
          local_590._3_1_ = s_oosevents_log_140802f20[0xb];
          local_58c = s_oosevents_log_140802f20[0xc];
          local_580 = 0xf;
          local_588 = 0xd;
          local_598._0_1_ = s_oosevents_log_140802f20[0];
          local_598._1_1_ = s_oosevents_log_140802f20[1];
          local_598._2_1_ = s_oosevents_log_140802f20[2];
          local_598._3_1_ = s_oosevents_log_140802f20[3];
          local_598._4_1_ = s_oosevents_log_140802f20[4];
          local_598._5_1_ = s_oosevents_log_140802f20[5];
          local_598._6_1_ = s_oosevents_log_140802f20[6];
          local_598._7_1_ = s_oosevents_log_140802f20[7];
          local_58b = 0;
          FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320);
          DAT_140a61ab2 = '\0';
          if (0xf < local_580) {
            if ((0xfff < local_580 + 1) && (0x1f < (local_598 - *(longlong *)(local_598 - 8)) - 8))
            {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            thunk_FUN_1407864b8();
          }
        }
        FUN_140047860(&DAT_140b153e0);
      }
      uVar16 = FUN_14004fba0();
      FUN_140262b80(uVar16);
      FUN_1403b1b50(&DAT_140a286f0);
      if ((DAT_1409cf314 == '\0') &&
         (((cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0' || (DAT_140b155c4 != '\0')) &&
          (cVar8 = FUN_14024cef0(), cVar8 == '\0')))) {
        WaitMessage();
      }
      else {
        cVar8 = FUN_14027c5f0(&DAT_140b0a320);
        if (((((cVar8 == '\0') || (DAT_140b155c4 == '\0')) || (DAT_140a785a8 == 0)) ||
            (cVar9 = FUN_1401c0c70(&DAT_140a78510), cVar9 == '\0')) && (cVar8 != '\0')) {
          if (DAT_140a61e70 != '\0') {
            FUN_14009b9f0();
          }
          FUN_14027c360(&DAT_140b0a320,iVar14);
          uVar6 = DAT_140b0a320;
          FUN_1402c0c70(&DAT_140b15690,DAT_140b0a320);
          if (uVar6 == ((int)uVar6 / 100) * 100) {
            if (DAT_140a10970 == 0) {
              lVar20 = FUN_140769c58(0xc0);
              lVar17 = 0;
              if (lVar20 != 0) {
                lVar17 = FUN_140475660(lVar20);
              }
              local_520 = &PTR_vftable_140a10968;
              DAT_140a10970 = lVar17;
              if (DAT_140a7bc10 == DAT_140a7bc18) {
                FUN_14001ded0(&DAT_140a7bc08);
              }
              else {
                *DAT_140a7bc10 = &PTR_vftable_140a10968;
                DAT_140a7bc10 = DAT_140a7bc10 + 1;
              }
            }
            FUN_1404767c0(DAT_140a10970);
          }
          FUN_14020f000((float)DAT_140b0a320 / (float)DAT_140b0a340);
          FUN_14028d400(&DAT_140b153e0);
          if (DAT_140a61d38 == '\0') {
            FUN_1402718e0((float)DAT_140b0a320 / (float)DAT_140b0a340);
          }
          FUN_140297e30(&DAT_140a16fd0);
          FUN_14001dc60();
          plVar7 = DAT_140b15418;
          if (DAT_140b15418 != (longlong *)0x0) {
            FUN_1403271d0(DAT_140b15418 + 0x14);
            FUN_140490480((longlong)plVar7 + 0x9c);
            FUN_1403268c0(plVar7 + 0x14);
          }
          FUN_14044a3f0(&DAT_140b30350);
          cVar8 = FUN_14028af60(&DAT_140b153e0);
          if ((((cVar8 != '\0') || ((DAT_140a61e60 == '\0' && (DAT_140b30690 == 0)))) &&
              (DAT_140b15418 != (longlong *)0x0)) &&
             (iVar15 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar15 == 0)) {
            lVar17 = DAT_140b15418[5];
            cVar8 = FUN_140342dc0(lVar17);
            if ((cVar8 == '\x01') && (cVar8 = FUN_140342d80(lVar17), cVar8 == '\x01')) {
              FUN_140092bb0(&DAT_140a10d88);
            }
          }
          FUN_14028a4c0(&DAT_140b153e0);
          FUN_14027cb70(&DAT_140b0a320);
          iVar15 = iVar14;
          if (0 < (int)DAT_140a2ab00) {
            FUN_14005f6b0(1);
          }
        }
        else {
          FUN_14009ba60(iVar14);
          FUN_1404908c0(&DAT_140b15560);
          Sleep(0);
          if (((500 < (uint)(iVar14 - iVar15)) &&
              (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) && (DAT_140b155c4 == '\0')) {
            FUN_14027d6a0(&DAT_140b0a320);
            iVar15 = iVar14;
          }
        }
      }
      if ((DAT_140a61aac != '\0') || (DAT_1409cf314 != '\0')) {
        DVar11 = timeGetTime();
        uVar19 = (ulonglong)DAT_140b0a320;
        iVar14 = DVar11 - _DAT_140a7bec0;
        if (DAT_140a61d38 != '\0') {
          FUN_1402718e0((float)(uint)(iVar14 - _DAT_140a61d3c) / fVar3);
        }
        fVar32 = (((float)uVar19 - (float)(int)uVar5) * (float)DAT_140b0a340) / fVar3;
        fVar2 = 0.0;
        if (0.0 <= fVar32) {
          fVar2 = fVar32;
        }
        cVar8 = FUN_14027d800(&DAT_140b0a320);
        if (cVar8 != '\0') {
          FUN_14028c580(&DAT_140b153e0);
          FUN_140271840();
          FUN_140301750(&DAT_140b27f60);
          FUN_14002ffb0(&DAT_140a60df0);
          FUN_14025ca30(DAT_140a619f0,(float)uVar31 / fVar3);
          FUN_140060330(fVar2 != 0.0);
        }
        FUN_14027c2f0(&DAT_140b0a320,iVar14);
        timeGetTime();
        FUN_14027c330(&DAT_140b0a320);
      }
    } while (DAT_140a619ef == '\0');
  }
LAB_14005f55c:
  timeEndPeriod(DAT_140a61b10);
  if (DAT_140a10cb8 != (HANDLE)0xffffffffffffffff) {
    ReleaseMutex(DAT_140a10cb8);
  }
  if ((((DAT_140a61acd != '\0') && (iVar15 = GetSystemMetrics(0x57), iVar15 != 0)) &&
      (DVar11 = ExpandEnvironmentStringsA("%SystemRoot%\\ehome\\ehshell.exe",local_488,0x104),
      DVar11 != 0)) && (DVar11 = GetFileAttributesA(local_488), DVar11 != 0xffffffff)) {
    ShellExecuteA((HWND)0x0,"open",local_488,(LPCSTR)0x0,(LPCSTR)0x0,1);
  }
  hProcess = GetCurrentProcess();
  TerminateProcess(hProcess,0);
  return 0;
}



// ------------------------------------------------------------
// RVA: 0x4da50  Name: FUN_14004da50  Size: 1199 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14004da50(void)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  
  if (DAT_140b30690 != 0) {
    FUN_14044f130(&DAT_140b306dd);
  }
  FUN_1400c6e30();
  FUN_140051ba0(&DAT_140a10a98,0);
  FUN_140050240(&DAT_140a109b0,0);
  FUN_14048aff0(&DAT_140a2aa40);
  uVar2 = FUN_14004fc30();
  FUN_140475540(uVar2);
  FUN_140332920(&DAT_140a172d0);
  FUN_14030c2e0(&DAT_140b27f60);
  FUN_14044cfc0(&DAT_140a28c10);
  FUN_1404724c0(&DAT_140a2a060);
  FUN_14047add0(&DAT_140b31160);
  FUN_1404746a0(&DAT_140a2a0b0);
  FUN_1402d7060(&DAT_140b27e60);
  FUN_140467f40(&DAT_140a29f20);
  FUN_140470d20(&DAT_140a29fa0);
  FUN_140470430(&DAT_140a29f60);
  FUN_14048a180(&DAT_140a2a9c0);
  thunk_FUN_140427fc0(&DAT_140b2f170);
  FUN_140359830(&DAT_140b2bcb0,0);
  _guard_check_icall(&DAT_140b2f2e0);
  uVar2 = FUN_14004fd50();
  FUN_140488ab0(uVar2);
  FUN_1402d2900(&DAT_140b27dc0);
  FUN_140321c80(&DAT_140b2b300);
  FUN_140322730(&DAT_140b2b3a8);
  FUN_1403207a0(&DAT_140b2b2f0);
  thunk_FUN_140477030(&DAT_140a2a120);
  FUN_1401c8070(&DAT_140a78510,0);
  FUN_1401f9aa0(&DAT_140a7bc28);
  FUN_1401f9aa0(&DAT_140a7bc58);
  FUN_14021c940(&DAT_140a7d0a0);
  thunk_FUN_1402c91c0(&DAT_140b15800);
  thunk_FUN_1404456f0(&DAT_140b30160);
  FUN_14048c510(&DAT_140b31330);
  FUN_14046a680(&DAT_140b30b40);
  FUN_140325150(&DAT_140b2b3b0);
  FUN_1404828d0(&DAT_140a2a850);
  FUN_140483180();
  FUN_140489bb0(&DAT_140a2a980);
  uVar2 = FUN_14004fb30();
  FUN_14046f8a0(uVar2);
  uVar2 = FUN_14001fb80();
  FUN_14007d3a0(uVar2);
  FUN_14047f690();
  FUN_14048f1d0();
  FUN_1404809d0(&DAT_140a2a810);
  FUN_1404817f0();
  FUN_140489320();
  FUN_14048f8c0(&PTR_vftable_140a2aa78);
  uVar2 = FUN_14004faa0();
  FUN_140476820(uVar2);
  if (DAT_140a619ed != '\0') {
    FUN_140030770(&DAT_140a60df0);
    FUN_1403b1af0(&DAT_140a286f0);
    uVar2 = FUN_14004fba0();
    FUN_140263300(uVar2);
    if (DAT_1409cf008 != 0) {
      puVar3 = DAT_140a7bc08;
      if (DAT_140a7bc08 != DAT_140a7bc10) {
        do {
          if ((undefined *)*puVar3 == &DAT_1409cf000) break;
          puVar3 = puVar3 + 1;
        } while (puVar3 != DAT_140a7bc10);
        if (puVar3 != DAT_140a7bc10) {
          FUN_14076bd80(puVar3,puVar3 + 1,(longlong)DAT_140a7bc10 - (longlong)(puVar3 + 1));
          DAT_140a7bc10 = DAT_140a7bc10 + -1;
          if (DAT_140a7bc08 == DAT_140a7bc10) {
            DAT_140a7bc10 = DAT_140a7bc08;
          }
        }
      }
      lVar1 = DAT_1409cf008;
      if (DAT_1409cf008 != 0) {
        FUN_14007dd80(DAT_1409cf008);
        thunk_FUN_1407864b8(lVar1,0xd0);
      }
      DAT_1409cf008 = 0;
    }
    if (DAT_140a61a08 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a08 = 0;
    }
    if (DAT_140a61a10 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a10 = 0;
    }
    if (DAT_140a61a18 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a18 = 0;
    }
    if (DAT_140b2eea0 != (undefined8 *)0x0) {
      (**(code **)*DAT_140b2eea0)(DAT_140b2eea0,1);
      DAT_140b2eea0 = (undefined8 *)0x0;
    }
    if (DAT_140a619f8 != (undefined8 *)0x0) {
      (**(code **)*DAT_140a619f8)(DAT_140a619f8,1);
      DAT_140a619f8 = (undefined8 *)0x0;
    }
    _DAT_140a28790 = 0;
    DAT_140b306a0 = 0;
    FUN_1400fc8d0();
    FUN_1400fc9a0();
    if (DAT_140a619f0 != (undefined8 *)0x0) {
      FUN_14025c680();
      if (DAT_140a619f0 != (undefined8 *)0x0) {
        (**(code **)*DAT_140a619f0)(DAT_140a619f0,1);
      }
      DAT_140a619f0 = (undefined8 *)0x0;
    }
    FUN_140362b20();
    FUN_14026dbe0();
    FUN_14048a9c0();
    FUN_14025fd10();
    DAT_140a619ed = '\0';
  }
  FUN_1401d4730();
  FUN_1401d2a00();
  FUN_14002d750();
  FUN_140215e10();
  FUN_140049290(&DAT_1409cf320);
  FUN_140049290(&DAT_1409cf390);
  uVar2 = FUN_14004fcc0();
  FUN_14047fd90(uVar2);
  FUN_140261370(&DAT_140b0a130);
  FUN_140261260(&DAT_140b0a130,DAT_140a61a30,"EmpireAtWar-Bold");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a38,"EmpireAtWar-Light");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a40,"EmpireAtWar-Medium");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a48,"EmpireAtWar-Stencil");
  DAT_140b15668 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x6ad230  Name: FUN_1406ad230  Size: 994 bytes
// ------------------------------------------------------------

void FUN_1406ad230(longlong param_1)

{
  float fVar1;
  longlong lVar2;
  float fVar3;
  undefined4 uVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  longlong *plVar9;
  longlong *plVar10;
  bool bVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  undefined4 local_res8 [2];
  undefined4 local_res18 [4];
  undefined8 in_stack_ffffffffffffff68;
  undefined4 uVar15;
  undefined8 in_stack_ffffffffffffff70;
  undefined4 uVar16;
  ulonglong in_stack_ffffffffffffff78;
  uint in_stack_ffffffffffffff90;
  
  fVar3 = DAT_1407ffaf8;
  uVar15 = (undefined4)((ulonglong)in_stack_ffffffffffffff68 >> 0x20);
  uVar16 = (undefined4)((ulonglong)in_stack_ffffffffffffff70 >> 0x20);
  bVar11 = false;
  uVar8 = 0;
  if (*(int *)(param_1 + 0xb8) == 4) {
    cVar5 = FUN_1403223f0(&DAT_140b2b3a8);
    bVar11 = cVar5 == '\0';
  }
  else {
    lVar2 = *(longlong *)(param_1 + 0x18);
    if (lVar2 == 0) goto LAB_1406ad41a;
    iVar6 = FUN_1403956a0(lVar2);
    if (lVar2 == *(longlong *)(param_1 + 0x18)) {
      lVar2 = *(longlong *)(param_1 + 0x10);
      if ((iVar6 != *(int *)(lVar2 + 0x98)) && (iVar6 != *(int *)(lVar2 + 0xa0))) {
        iVar7 = *(int *)(lVar2 + 0x9c);
LAB_1406ad2d5:
        if (iVar6 != iVar7) goto LAB_1406ad41a;
      }
    }
    else {
      if (lVar2 != *(longlong *)(param_1 + 0x20)) goto LAB_1406ad41a;
      lVar2 = *(longlong *)(param_1 + 0x10);
      if ((iVar6 != *(int *)(lVar2 + 0xb8)) && (iVar6 != *(int *)(lVar2 + 0xc0))) {
        iVar7 = *(int *)(lVar2 + 0xbc);
        goto LAB_1406ad2d5;
      }
    }
    iVar6 = FUN_140395660(*(longlong *)(param_1 + 0x18));
    if (iVar6 == 2) {
      iVar6 = FUN_140395660(*(undefined8 *)(param_1 + 0x20));
      if (iVar6 != 2) {
        return;
      }
      bVar11 = true;
    }
    else {
      plVar9 = (longlong *)FUN_140264770(*(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x2a0));
      iVar6 = (**(code **)(*plVar9 + 0x28))(plVar9);
      fVar14 = (float)FUN_1402647a0(*(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x2a0));
      fVar13 = fVar3;
      if (fVar14 / (float)iVar6 <= fVar3) {
        fVar13 = fVar14 / (float)iVar6;
      }
      fVar14 = 0.0;
      if (0.0 <= fVar13) {
        fVar14 = fVar13;
      }
      if (fVar14 < DAT_14080090c) {
        return;
      }
    }
    if (*(char *)(param_1 + 0x50) == '\0') {
      if (*(longlong *)(param_1 + 0x60) != 0) {
        if (*(longlong *)(param_1 + 0x60) == *(longlong *)(param_1 + 0x18)) {
          fVar13 = *(float *)(param_1 + 0x38);
        }
        else {
          fVar13 = *(float *)(param_1 + 0x3c);
        }
        lVar2 = *(longlong *)(param_1 + 0x10);
        fVar14 = *(float *)(lVar2 + 0x74);
        fVar1 = *(float *)(lVar2 + 0x70);
        fVar12 = (float)FUN_140480ed0(lVar2,*(undefined8 *)(*(longlong *)(param_1 + 0x58) + 0x298));
        FUN_14022f9c0((fVar13 * fVar14 + fVar1) * fVar12);
        in_stack_ffffffffffffff90 = 0xffffffff;
        in_stack_ffffffffffffff78 = 0;
        uVar16 = 0;
        uVar15 = 0;
        FUN_1403a9e30(*(undefined8 *)(param_1 + 0x60),1);
        if ((*(byte *)(*(longlong *)(param_1 + 0x60) + 0x3a0) & 0x40) != 0) {
          FUN_1406ac660(param_1,*(undefined8 *)(param_1 + 0x58));
          return;
        }
      }
      *(undefined1 *)(param_1 + 0x50) = 1;
    }
  }
  if (!bVar11) {
    return;
  }
LAB_1406ad41a:
  *(undefined1 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x60) = 0;
  local_res8[0] = 0xffffffff;
  local_res18[0] = 0xffffffff;
  FUN_1406ac1b0(param_1,local_res8,local_res18);
  iVar6 = FUN_14027b4d0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x2a0) + 0xb0)
                        ,local_res8[0]);
  iVar7 = FUN_14027b4d0(*(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x20) + 0x2a0) + 0xb0)
                        ,local_res18[0]);
  if (iVar6 == iVar7) {
    if (iVar6 != 0) {
      uVar8 = FUN_1401ffb40(&DAT_140a13e24,0,iVar6 + -1);
    }
    plVar9 = (longlong *)
             FUN_14027b480(*(undefined8 *)
                            (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x2a0) + 0xb0),
                           local_res8[0],uVar8);
    plVar10 = (longlong *)
              FUN_14027b480(*(undefined8 *)
                             (*(longlong *)(*(longlong *)(param_1 + 0x20) + 0x2a0) + 0xb0),
                            local_res18[0],uVar8);
    if ((plVar9 != (longlong *)0x0) && (plVar10 != (longlong *)0x0)) {
      iVar6 = (**(code **)(*plVar10 + 0x28))(plVar10);
      iVar7 = (**(code **)(*plVar9 + 0x28))(plVar9);
      if (iVar7 == iVar6) {
        fVar13 = (float)(**(code **)(*plVar9 + 0x38))(plVar9);
        fVar14 = (float)(**(code **)(*plVar10 + 0x38))(plVar10);
        uVar4 = DAT_1408007b4;
        if (fVar13 == fVar14) {
          in_stack_ffffffffffffff90 = in_stack_ffffffffffffff90 & 0xffffff00;
          in_stack_ffffffffffffff78 = in_stack_ffffffffffffff78 & 0xffffffff00000000;
          FUN_1403a92f0(*(undefined8 *)(param_1 + 0x18),local_res8[0],uVar8,0,
                        CONCAT44(uVar15,DAT_1408007b4),CONCAT44(uVar16,fVar3),
                        in_stack_ffffffffffffff78,0,0,in_stack_ffffffffffffff90);
          FUN_1403a92f0(*(undefined8 *)(param_1 + 0x20),local_res18[0],uVar8,0,uVar4,fVar3,
                        in_stack_ffffffffffffff78 & 0xffffffff00000000,0,0,
                        in_stack_ffffffffffffff90 & 0xffffff00);
          if (*(int *)(param_1 + 0xb8) == 4) {
            FUN_1403225a0(&DAT_140b2b3a8);
          }
        }
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x6ac1b0  Name: FUN_1406ac1b0  Size: 1187 bytes
// ------------------------------------------------------------

void FUN_1406ac1b0(longlong param_1,undefined4 *param_2,undefined4 *param_3)

{
  longlong *plVar1;
  bool bVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  undefined8 *puVar5;
  char cVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  int iVar10;
  float fVar11;
  undefined8 *local_58;
  longlong local_50;
  undefined8 *local_48;
  uint uStack_40;
  
  if (((*(int *)(param_1 + 0xb8) == 4) || (cVar6 = FUN_1403223c0(&DAT_140b2b3a8), cVar6 != '\x01'))
     || (iVar10 = 4, *(char *)(*(longlong *)(param_1 + 0x10) + 0x68) == '\0')) {
    local_50 = 0;
    puVar7 = (undefined8 *)FUN_140769c58(0x28);
    uStack_40 = 0;
    *puVar7 = puVar7;
    puVar7[1] = puVar7;
    puVar7[2] = puVar7;
    *(undefined2 *)(puVar7 + 3) = 0x101;
    local_58 = puVar7;
    if ((*(char *)((longlong)puVar7 + 0x19) != '\0') || (1 < *(int *)((longlong)puVar7 + 0x1c))) {
      local_48 = puVar7;
      if (local_50 == 0x666666666666666) {
                    /* WARNING: Subroutine does not return */
        FUN_1400501c0();
      }
      puVar8 = (undefined8 *)FUN_140769c58(0x28);
      *(undefined8 *)((longlong)puVar8 + 0x1c) = 1;
      *puVar8 = puVar7;
      puVar8[1] = puVar7;
      puVar8[2] = puVar7;
      *(undefined2 *)(puVar8 + 3) = 0;
      puVar7 = (undefined8 *)FUN_14004fe80(&local_58,&local_48,puVar8);
    }
    puVar5 = local_58;
    uStack_40 = 0;
    *(undefined4 *)(puVar7 + 4) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x78);
    puVar7 = (undefined8 *)local_58[1];
    cVar6 = *(char *)((longlong)puVar7 + 0x19);
    local_48 = puVar7;
    puVar8 = local_58;
    while (puVar3 = puVar7, cVar6 == '\0') {
      bVar2 = 1 < *(int *)((longlong)puVar3 + 0x1c);
      if (bVar2) {
        puVar7 = (undefined8 *)*puVar3;
        puVar8 = puVar3;
      }
      else {
        puVar7 = (undefined8 *)puVar3[2];
      }
      uStack_40 = (uint)bVar2;
      cVar6 = *(char *)((longlong)puVar7 + 0x19);
      local_48 = puVar3;
    }
    if ((*(char *)((longlong)puVar8 + 0x19) != '\0') || (2 < *(int *)((longlong)puVar8 + 0x1c))) {
      if (local_50 == 0x666666666666666) {
                    /* WARNING: Subroutine does not return */
        FUN_1400501c0();
      }
      puVar7 = (undefined8 *)FUN_140769c58(0x28);
      *(undefined8 *)((longlong)puVar7 + 0x1c) = 2;
      *puVar7 = puVar5;
      puVar7[1] = puVar5;
      puVar7[2] = puVar5;
      *(undefined2 *)(puVar7 + 3) = 0;
      puVar8 = (undefined8 *)FUN_14004fe80(&local_58,&local_48,puVar7);
    }
    puVar5 = local_58;
    uStack_40 = 0;
    *(undefined4 *)(puVar8 + 4) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x7c);
    puVar7 = (undefined8 *)local_58[1];
    cVar6 = *(char *)((longlong)puVar7 + 0x19);
    local_48 = puVar7;
    puVar8 = local_58;
    while (puVar3 = puVar7, cVar6 == '\0') {
      bVar2 = 2 < *(int *)((longlong)puVar3 + 0x1c);
      if (bVar2) {
        puVar7 = (undefined8 *)*puVar3;
        puVar8 = puVar3;
      }
      else {
        puVar7 = (undefined8 *)puVar3[2];
      }
      uStack_40 = (uint)bVar2;
      cVar6 = *(char *)((longlong)puVar7 + 0x19);
      local_48 = puVar3;
    }
    if ((*(char *)((longlong)puVar8 + 0x19) != '\0') || (3 < *(int *)((longlong)puVar8 + 0x1c))) {
      if (local_50 == 0x666666666666666) {
                    /* WARNING: Subroutine does not return */
        FUN_1400501c0();
      }
      puVar7 = (undefined8 *)FUN_140769c58(0x28);
      *(undefined8 *)((longlong)puVar7 + 0x1c) = 3;
      *puVar7 = puVar5;
      puVar7[1] = puVar5;
      puVar7[2] = puVar5;
      *(undefined2 *)(puVar7 + 3) = 0;
      puVar8 = (undefined8 *)FUN_14004fe80(&local_58,&local_48,puVar7);
    }
    *(float *)(puVar8 + 4) =
         DAT_1407ffaf8 -
         (*(float *)(*(longlong *)(param_1 + 0x10) + 0x7c) +
         *(float *)(*(longlong *)(param_1 + 0x10) + 0x78));
    FUN_1406ad620(&local_58,param_1 + 0xbc);
    fVar11 = 0.0;
    puVar7 = (undefined8 *)*local_58;
    while (puVar7 != local_58) {
      puVar8 = (undefined8 *)puVar7[2];
      fVar11 = fVar11 + *(float *)(puVar7 + 4);
      if (*(char *)((longlong)puVar8 + 0x19) == '\0') {
        cVar6 = *(char *)((longlong)*puVar8 + 0x19);
        puVar7 = puVar8;
        puVar8 = (undefined8 *)*puVar8;
        while (cVar6 == '\0') {
          cVar6 = *(char *)((longlong)*puVar8 + 0x19);
          puVar7 = puVar8;
          puVar8 = (undefined8 *)*puVar8;
        }
      }
      else {
        cVar6 = *(char *)((longlong)puVar7[1] + 0x19);
        puVar5 = (undefined8 *)puVar7[1];
        puVar8 = puVar7;
        while ((puVar7 = puVar5, cVar6 == '\0' && (puVar8 == (undefined8 *)puVar7[2]))) {
          cVar6 = *(char *)((longlong)puVar7[1] + 0x19);
          puVar5 = (undefined8 *)puVar7[1];
          puVar8 = puVar7;
        }
      }
    }
    fVar11 = (float)FUN_140532480(0,fVar11);
    puVar7 = (undefined8 *)*local_58;
    while (puVar7 != local_58) {
      fVar11 = fVar11 - *(float *)(puVar7 + 4);
      if (fVar11 <= 0.0) {
        iVar10 = *(int *)((longlong)puVar7 + 0x1c);
        if (iVar10 != 0) goto LAB_1406ac512;
        break;
      }
      puVar8 = (undefined8 *)puVar7[2];
      if (*(char *)((longlong)puVar8 + 0x19) == '\0') {
        cVar6 = *(char *)((longlong)*puVar8 + 0x19);
        puVar7 = puVar8;
        puVar8 = (undefined8 *)*puVar8;
        while (cVar6 == '\0') {
          cVar6 = *(char *)((longlong)*puVar8 + 0x19);
          puVar7 = puVar8;
          puVar8 = (undefined8 *)*puVar8;
        }
      }
      else {
        cVar6 = *(char *)((longlong)puVar7[1] + 0x19);
        puVar5 = (undefined8 *)puVar7[1];
        puVar8 = puVar7;
        while ((puVar7 = puVar5, cVar6 == '\0' && (puVar8 == (undefined8 *)puVar7[2]))) {
          cVar6 = *(char *)((longlong)puVar7[1] + 0x19);
          puVar5 = (undefined8 *)puVar7[1];
          puVar8 = puVar7;
        }
      }
    }
    iVar10 = 3;
LAB_1406ac512:
    *(int *)(param_1 + 0xbc) = iVar10;
    cVar6 = *(char *)((longlong)local_58[1] + 0x19);
    plVar4 = (longlong *)local_58[1];
    while (cVar6 == '\0') {
      FUN_140066530(&local_58,&local_58,plVar4[2]);
      plVar1 = (longlong *)*plVar4;
      thunk_FUN_1407864b8(plVar4,0x28);
      plVar4 = plVar1;
      cVar6 = *(char *)((longlong)plVar1 + 0x19);
    }
    thunk_FUN_1407864b8(local_58,0x28);
    if (iVar10 == 1) {
      *param_2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x98);
      *param_3 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xbc);
      *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_1 + 0x18);
      uVar9 = *(undefined8 *)(param_1 + 0x20);
    }
    else {
      if (iVar10 != 2) {
        if (iVar10 == 3) {
          *param_2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa0);
          *param_3 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xc0);
          *(undefined8 *)(param_1 + 0x58) = 0;
          *(undefined8 *)(param_1 + 0x60) = 0;
          goto LAB_1406ac620;
        }
        if (iVar10 != 4) goto LAB_1406ac620;
        goto LAB_1406ac586;
      }
      *param_2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x9c);
      *param_3 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xb8);
      *(undefined8 *)(param_1 + 0x58) = *(undefined8 *)(param_1 + 0x20);
      uVar9 = *(undefined8 *)(param_1 + 0x18);
    }
    *(undefined8 *)(param_1 + 0x60) = uVar9;
  }
  else {
LAB_1406ac586:
    *param_2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa4);
    *param_3 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xc4);
    *(undefined8 *)(param_1 + 0x58) = 0;
    *(undefined8 *)(param_1 + 0x60) = 0;
  }
LAB_1406ac620:
  *(int *)(param_1 + 0xb8) = iVar10;
  return;
}



// ------------------------------------------------------------
// RVA: 0x4b250  Name: FUN_14004b250  Size: 10192 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14004b250(void)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  DWORD DVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong *plVar8;
  ulonglong uVar9;
  undefined8 *puVar10;
  char *pcVar11;
  undefined8 *******pppppppuVar12;
  uint uVar13;
  longlong lVar14;
  ulonglong uVar15;
  longlong lVar16;
  undefined8 *puVar17;
  undefined4 local_res8 [2];
  code *local_res10;
  undefined4 local_res18;
  undefined4 local_res1c;
  undefined8 in_stack_fffffffffffffeb0;
  undefined4 uVar20;
  undefined8 uVar18;
  longlong *plVar19;
  undefined4 uVar21;
  undefined8 *******local_58 [2];
  longlong local_48;
  ulonglong local_40;
  
  uVar20 = (undefined4)((ulonglong)in_stack_fffffffffffffeb0 >> 0x20);
  uVar15 = 0;
  local_res8[0] = 0;
  FUN_140048f20();
  FUN_1401d4160(DAT_140a619d0);
  FUN_1401d25b0();
  FUN_1401d3c20();
  FUN_1401d4700(DAT_140a619d0);
  uVar6 = FUN_14004fb30();
  cVar3 = FUN_14046ea70(uVar6);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                "DatabaseConversionSystemClass::Get().System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize DatabaseConversionSystem.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_1402cb210(&DAT_140b15800,&DAT_140a10888);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheGameConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_1404457e0(&DAT_140b30160,&DAT_140a108e8);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudioConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheAudioConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_14048b8a0(&DAT_140b31330,&DAT_140a10928,0);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheMousePointerDataManager.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_140261280(&DAT_140b0a130);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","FontManager.System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize FontManager.");
    FUN_14004da50();
    return 0;
  }
  uVar6 = FUN_14004faa0();
  FUN_1404766a0(uVar6);
  FUN_14047fd00(&DAT_1409cf314);
  FUN_14047fcf0(FUN_14004f320);
  uVar6 = FUN_14004fcc0();
  cVar3 = FUN_14047fd10(uVar6,DAT_140a619d8,DAT_140a619d0);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                "TheDebugWindowManagerClass::Get().System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize DebugWindowManager.");
  }
  lVar16 = -1;
  lVar14 = -1;
  local_58[0] = (undefined8 *******)0x0;
  local_48 = 0;
  local_40 = 0xf;
  do {
    lVar14 = lVar14 + 1;
  } while (PTR_s___Data_XML_GraphicDetails_xml_140a10798[lVar14] != '\0');
  FUN_140022730(local_58);
  cVar3 = FUN_14048aa00(&DAT_140a2aa40,local_58,0);
  if (0xf < local_40) {
    if (0xfff < local_40 + 1) {
      if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGraphicDetailManager.System_Initialize",uVar7,
                uVar6,CONCAT44(uVar20,DVar4));
  if (cVar3 == '\0') {
    pcVar11 = "Error: Unable to initialize GraphicDetailManager.";
  }
  else {
    local_res8[0] = 0;
    DAT_140a61a30 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    if (DAT_140a61a30 == 0) {
      local_res8[0] = 0;
      DAT_140a61a30 =
           FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    }
    local_res8[0] = 0;
    DAT_140a61a38 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409cf410,0x1064c,0,local_res8,"EmpireAtWar-Light");
    local_res8[0] = 0;
    DAT_140a61a40 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409efb00,0x10414,0,local_res8,"EmpireAtWar-Medium");
    local_res8[0] = 0;
    uVar20 = 1;
    DAT_140a61a48 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409fff20,0x10830,0,local_res8,"EmpireAtWar-Stencil");
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    uVar18 = CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","EAW Fonts Loaded",uVar7,uVar6,uVar18);
    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
    cVar3 = FUN_14004df10(DAT_140a61aac,DAT_1409cf308,DAT_1409cf30c,DAT_1409cf310,DAT_140a61ac0);
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    plVar19 = (longlong *)CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","Init_3D_Game",uVar7,uVar6,plVar19);
    if (cVar3 == '\0') {
      pcVar11 = "Error: Unable to initialize via Init_3D_Game().";
    }
    else {
      uVar7 = FUN_1402c8210(&DAT_140b15800,DAT_140a7c090,DAT_140a13ed8);
      FUN_14001e680(local_58,uVar7);
      if (local_48 == 0) {
        lVar14 = -1;
        do {
          lVar14 = lVar14 + 1;
        } while (PTR_s_Splash_TGA_140a10ba8[lVar14] != '\0');
        FUN_140022730(local_58);
      }
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      FUN_140025760("Loading splash screen %s\n",pppppppuVar12);
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      plVar8 = (longlong *)FUN_14025e990(pppppppuVar12,1,0);
      if (plVar8 == (longlong *)0x0) {
        pppppppuVar12 = local_58;
        if (0xf < local_40) {
          pppppppuVar12 = local_58[0];
        }
        FUN_140025760("Failed to load splash screen %s\n",pppppppuVar12);
      }
      else {
        (**(code **)*plVar8)(plVar8);
        lVar14 = FUN_140769c58(0x90);
        uVar9 = uVar15;
        if (lVar14 != 0) {
          uVar6 = CONCAT44((int)((ulonglong)uVar6 >> 0x20),DAT_1407ffaf8);
          plVar19 = plVar8;
          uVar9 = FUN_14025ff10(lVar14,0,0,DAT_1407ffaf8,uVar6,plVar8);
        }
        DAT_140a61a50 = uVar9;
        (**(code **)(*plVar8 + 8))(plVar8);
        FUN_14025c080(DAT_140a619f0,DAT_140a61a50);
      }
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      if (0xf < local_40) {
        if (0xfff < local_40 + 1) {
          if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      pcVar11 = (char *)FUN_14004fba0();
      if (*pcVar11 == '\x01') {
        uVar7 = FUN_14004fba0();
        FUN_140263300(uVar7);
      }
      iVar5 = FUN_14048b800(&DAT_140b31330);
      uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
      uVar9 = uVar15;
      if (0 < iVar5) {
        do {
          puVar10 = (undefined8 *)FUN_14048b7d0(&DAT_140b31330,uVar9);
          uVar20 = (undefined4)((ulonglong)uVar6 >> 0x20);
          uVar21 = (undefined4)((ulonglong)plVar19 >> 0x20);
          if (puVar10 != (undefined8 *)0x0) {
            plVar8 = puVar10 + 5;
            if (0xf < (ulonglong)puVar10[8]) {
              plVar8 = (longlong *)*plVar8;
            }
            puVar17 = puVar10;
            if (0xf < (ulonglong)puVar10[3]) {
              puVar17 = (undefined8 *)*puVar10;
            }
            uVar7 = FUN_14004fba0();
            plVar19 = (longlong *)CONCAT44(uVar21,*(undefined4 *)(puVar10 + 10));
            uVar6 = CONCAT44(uVar20,*(undefined4 *)((longlong)puVar10 + 0x4c));
            FUN_140262a00(uVar7,puVar17,plVar8,*(undefined4 *)(puVar10 + 9),uVar6,plVar19);
          }
          uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
          uVar13 = (int)uVar9 + 1;
          uVar9 = (ulonglong)uVar13;
        } while ((int)uVar13 < iVar5);
      }
      uVar6 = FUN_14004fba0();
      FUN_1402625a0(uVar6);
      uVar6 = FUN_14004fba0();
      FUN_140262490(uVar6,0);
      FUN_14020e010();
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","KeyboardClass::Init",uVar7,uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      FUN_140242cd0("Data/Scripts/Library/");
      FUN_140242cd0("Data/Scripts/GameObject/");
      FUN_140242cd0("Data/Scripts/FreeStore/");
      FUN_140242cd0("Data/Scripts/Miscellaneous/");
      FUN_140242cd0("Data/Scripts/Story/");
      uVar6 = FUN_14001fb80();
      cVar3 = FUN_14007d100(uVar6);
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheConsole::Get().System_Initialize",uVar7,
                    uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      if (cVar3 == '\0') {
        pcVar11 = "Error: Unable to initialize TheConsole.";
      }
      else {
        cVar3 = FUN_14021c2f0(&DAT_140a7d0a0,DAT_140a7c090,DAT_140a61ab6);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudio.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          FUN_1402149d0("Error: Unable to successfully initialize the TheAudio system.");
        }
        cVar3 = FUN_1401f9a60(&DAT_140a7bc58,DAT_140a7c090,
                              PTR_s___Data_Text_MasterTextFile_txt_140a10768);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameText.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          pcVar11 = "Error: Unable to initialize TheGameText.";
        }
        else {
          FUN_14004f150();
          cVar3 = FUN_1401f9a60(&DAT_140a7bc28,DAT_140a7c090,
                                PTR_s___Data_Text_CreditsText_txt_140a10770);
          DVar4 = timeGetTime();
          uVar9 = FUN_140216790();
          uVar6 = FUN_140216710();
          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheCreditsText.System_Initialize",uVar6,
                        uVar9,CONCAT44(uVar20,DVar4));
          if (cVar3 == '\0') {
            pcVar11 = "Error: Unable to initialize TheCreditsText.";
          }
          else {
            uVar20 = 1;
            cVar3 = FUN_1401c7bb0(&DAT_140a78510,PTR_s___Data_Resources_GUIDialog_GUIDi_140a10750,
                                  PTR_s___Data_Resources_GUIDialog_Resou_140a10758,
                                  PTR_s___Data_XML_GUIDialogs_xml_140a10760,
                                  uVar9 & 0xffffffffffffff00,FUN_140037840);
            DVar4 = timeGetTime();
            uVar6 = FUN_140216790();
            uVar7 = FUN_140216710();
            uVar18 = CONCAT44(uVar20,DVar4);
            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGUIDialogManager.System_Initialize",
                          uVar7,uVar6,uVar18);
            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
            if (cVar3 == '\0') {
              pcVar11 = "Error: Unable to initialize TheGUIDialogManager.";
            }
            else {
              _DAT_140a79880 = FUN_140037eb0;
              lVar14 = -1;
              local_58[0] = (undefined8 *******)0x0;
              local_48 = 0;
              local_40 = 0xf;
              do {
                lVar14 = lVar14 + 1;
              } while (PTR_s___Data_XML_SFXEventFiles_xml_140a10780[lVar14] != '\0');
              FUN_140022730(local_58);
              cVar3 = FUN_1402d5df0(&DAT_140b27e60,local_58,0);
              if (0xf < local_40) {
                if (0xfff < local_40 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])))
                  {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
              DVar4 = timeGetTime();
              uVar6 = FUN_140216790();
              uVar7 = FUN_140216710();
              uVar18 = CONCAT44(uVar20,DVar4);
              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheSFXEventManager.System_Initialize",
                            uVar7,uVar6,uVar18);
              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
              if (cVar3 == '\0') {
                pcVar11 = "Error: Unable to initialize TheSFXEventManager.";
              }
              else {
                lVar14 = -1;
                local_58[0] = (undefined8 *******)0x0;
                local_48 = 0;
                local_40 = 0xf;
                do {
                  lVar14 = lVar14 + 1;
                } while (PTR_s___Data_XML_TerrainDecalFX_xml_140a10790[lVar14] != '\0');
                FUN_140022730(local_58);
                cVar3 = FUN_140470ef0(&DAT_140a2a020,local_58,0);
                if (0xf < local_40) {
                  if (0xfff < local_40 + 1) {
                    if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))
                       ) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                    }
                  }
                  thunk_FUN_1407864b8();
                }
                DVar4 = timeGetTime();
                uVar6 = FUN_140216790();
                uVar7 = FUN_140216710();
                uVar18 = CONCAT44(uVar20,DVar4);
                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                              "TheTerrainDecalFXManager.System_Initialize",uVar7,uVar6,uVar18);
                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                if (cVar3 == '\0') {
                  pcVar11 = "Error: Unable to initialize TheTerrainDecalFXManager.";
                }
                else {
                  lVar14 = -1;
                  local_58[0] = (undefined8 *******)0x0;
                  local_48 = 0;
                  local_40 = 0xf;
                  do {
                    lVar14 = lVar14 + 1;
                  } while (PTR_s___Data_XML_DynamicTrackFX_xml_140a107a0[lVar14] != '\0');
                  FUN_140022730(local_58);
                  cVar3 = FUN_14046ffe0(&DAT_140a29f60,local_58,0);
                  if (0xf < local_40) {
                    if (0xfff < local_40 + 1) {
                      if (0x1f < (ulonglong)
                                 ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                      }
                    }
                    thunk_FUN_1407864b8();
                  }
                  DVar4 = timeGetTime();
                  uVar6 = FUN_140216790();
                  uVar7 = FUN_140216710();
                  uVar18 = CONCAT44(uVar20,DVar4);
                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                "TheDynamicTrackFXManager.System_Initialize",uVar7,uVar6,uVar18);
                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                  if (cVar3 == '\0') {
                    pcVar11 = "Error: Unable to initialize TheDynamicTrackFXManager.";
                  }
                  else {
                    lVar14 = -1;
                    local_58[0] = (undefined8 *******)0x0;
                    local_48 = 0;
                    local_40 = 0xf;
                    do {
                      lVar14 = lVar14 + 1;
                    } while (PTR_s___Data_XML_StarWars3DTextCrawl_x_140a107c0[lVar14] != '\0');
                    FUN_140022730(local_58);
                    cVar3 = FUN_140489d30(&DAT_140a2a9c0,local_58,0);
                    if (0xf < local_40) {
                      if (0xfff < local_40 + 1) {
                        if (0x1f < (ulonglong)
                                   ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                          FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                        }
                      }
                      thunk_FUN_1407864b8();
                    }
                    DVar4 = timeGetTime();
                    uVar6 = FUN_140216790();
                    uVar7 = FUN_140216710();
                    uVar18 = CONCAT44(uVar20,DVar4);
                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                  "TheDraw3DTextCrawlManager.System_Initialize",uVar7,uVar6,uVar18);
                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                    if (cVar3 == '\0') {
                      pcVar11 = "Error: Unable to initialize TheDraw3DTextCrawlManager.";
                    }
                    else {
                      lVar14 = -1;
                      local_58[0] = (undefined8 *******)0x0;
                      local_48 = 0;
                      local_40 = 0xf;
                      do {
                        lVar14 = lVar14 + 1;
                      } while (PTR_s___Data_XML_SurfaceFX_xml_140a10788[lVar14] != '\0');
                      FUN_140022730(local_58);
                      cVar3 = FUN_140470750(&DAT_140a29fa0,local_58,0);
                      if (0xf < local_40) {
                        if (0xfff < local_40 + 1) {
                          if (0x1f < (ulonglong)
                                     ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                          }
                        }
                        thunk_FUN_1407864b8();
                      }
                      DVar4 = timeGetTime();
                      uVar6 = FUN_140216790();
                      uVar7 = FUN_140216710();
                      uVar18 = CONCAT44(uVar20,DVar4);
                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                    "TheSurfaceFXEventManager.System_Initialize",uVar7,uVar6,uVar18)
                      ;
                      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                      if (cVar3 == '\0') {
                        pcVar11 = "Error: Unable to initialize TheSurfaceFXEventManager.";
                      }
                      else {
                        lVar14 = -1;
                        local_58[0] = (undefined8 *******)0x0;
                        local_48 = 0;
                        local_40 = 0xf;
                        do {
                          lVar14 = lVar14 + 1;
                        } while (PTR_s___Data_XML_ShadowBlobMaterials_x_140a107a8[lVar14] != '\0');
                        FUN_140022730(local_58);
                        cVar3 = FUN_1404280b0(&DAT_140b2f170,local_58,0);
                        if (0xf < local_40) {
                          if (0xfff < local_40 + 1) {
                            if (0x1f < (ulonglong)
                                       ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                            }
                          }
                          thunk_FUN_1407864b8();
                        }
                        DVar4 = timeGetTime();
                        uVar6 = FUN_140216790();
                        uVar7 = FUN_140216710();
                        uVar18 = CONCAT44(uVar20,DVar4);
                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                      "TheShadowBlobManager.System_Initialize",uVar7,uVar6,uVar18);
                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                        if (cVar3 == '\0') {
                          pcVar11 = "Error: Unable to initialize TheShadowBlobManager.";
                        }
                        else {
                          FUN_140427e20(&DAT_140b2f170,0);
                          cVar3 = FUN_14047ffe0(PTR_s___Data_XML_TacticalCameras_xml_140a107b0);
                          DVar4 = timeGetTime();
                          uVar6 = FUN_140216790();
                          uVar7 = FUN_140216710();
                          uVar18 = CONCAT44(uVar20,DVar4);
                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                        "TacticalCameraConstantsManager::System_Initialize",uVar7,
                                        uVar6,uVar18);
                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                          if (cVar3 == '\0') {
                            pcVar11 = "Error: Unable to initialize TacticalCameraConstantsManager.";
                          }
                          else {
                            lVar14 = -1;
                            local_58[0] = (undefined8 *******)0x0;
                            local_48 = 0;
                            local_40 = 0xf;
                            do {
                              lVar14 = lVar14 + 1;
                            } while (PTR_s___Data_XML_RadarMap_xml_140a10838[lVar14] != '\0');
                            FUN_140022730(local_58);
                            cVar3 = FUN_140359390(&DAT_140b2bcb0,local_58,0);
                            if (0xf < local_40) {
                              if (0xfff < local_40 + 1) {
                                if (0x1f < (ulonglong)
                                           ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])
                                           )) {
                    /* WARNING: Subroutine does not return */
                                  FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                }
                              }
                              thunk_FUN_1407864b8();
                            }
                            DVar4 = timeGetTime();
                            uVar6 = FUN_140216790();
                            uVar7 = FUN_140216710();
                            uVar18 = CONCAT44(uVar20,DVar4);
                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                          "TheRadarMap.System_Initialize",uVar7,uVar6,uVar18);
                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                            if (cVar3 == '\0') {
                              pcVar11 = "Error: Unable to initialize TheRadarMap.";
                            }
                            else {
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_UnitAbilityTypes_xml_140a10878[lVar14] !=
                                       '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_14043fab0(&DAT_140b2f2e0,local_58,0);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheUnitAbilityTypeManager.System_Initialize",uVar7,
                                            uVar6,uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                FUN_1402149d0(
                                             "Error: Unable to successfully initialize the TheUnitAbilityTypeManager system."
                                             );
                              }
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_MusicEvents_xml_140a107c8[lVar14] != '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_1402d2370(&DAT_140b27dc0,local_58);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheMusicEventManager.System_Initialize",uVar7,uVar6,
                                            uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                pcVar11 = "Error: Unable to initialize TheMusicEventManager.";
                              }
                              else {
                                lVar14 = -1;
                                local_58[0] = (undefined8 *******)0x0;
                                local_48 = 0;
                                local_40 = 0xf;
                                do {
                                  lVar14 = lVar14 + 1;
                                } while (PTR_s___Data_XML_SpeechEvents_xml_140a107d0[lVar14] != '\0'
                                        );
                                FUN_140022730(local_58);
                                cVar3 = FUN_140321450(&DAT_140b2b300,local_58);
                                if (0xf < local_40) {
                                  if (0xfff < local_40 + 1) {
                                    if (0x1f < (ulonglong)
                                               ((longlong)local_58[0] +
                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                    }
                                  }
                                  thunk_FUN_1407864b8();
                                }
                                DVar4 = timeGetTime();
                                uVar6 = FUN_140216790();
                                uVar7 = FUN_140216710();
                                uVar18 = CONCAT44(uVar20,DVar4);
                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                              "TheSpeechEventManager.System_Initialize",uVar7,uVar6,
                                              uVar18);
                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                if (cVar3 == '\0') {
                                  pcVar11 = "Error: Unable to initialize TheSpeechEventManager.";
                                }
                                else {
                                  cVar3 = FUN_1403226e0(&DAT_140b2b3a8);
                                  DVar4 = timeGetTime();
                                  uVar6 = FUN_140216790();
                                  uVar7 = FUN_140216710();
                                  uVar18 = CONCAT44(uVar20,DVar4);
                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                "TheSpeechConversationManager.System_Initialize",
                                                uVar7,uVar6,uVar18);
                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                  if (cVar3 == '\0') {
                                    pcVar11 = 
                                    "Error: Unable to initialize TheSpeechConversationManager.";
                                  }
                                  else {
                                    cVar3 = FUN_140320760(&DAT_140b2b2f0);
                                    DVar4 = timeGetTime();
                                    uVar6 = FUN_140216790();
                                    uVar7 = FUN_140216710();
                                    uVar18 = CONCAT44(uVar20,DVar4);
                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                  "TheSFXConversationManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                    if (cVar3 == '\0') {
                                      pcVar11 = 
                                      "Error: Unable to initialize TheSFXConversationManager.";
                                    }
                                    else {
                                      local_res10 = FUN_14032e230;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      local_res10 = (code *)&LAB_140107ed0;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      lVar14 = -1;
                                      local_58[0] = (undefined8 *******)0x0;
                                      local_48 = 0;
                                      local_40 = 0xf;
                                      do {
                                        lVar14 = lVar14 + 1;
                                      } while (PTR_s___Data_XML_AnimationSFXMaps_txt_140a107d8
                                               [lVar14] != '\0');
                                      FUN_140022730(local_58);
                                      cVar3 = FUN_1404770a0(&DAT_140a2a120,local_58,0);
                                      if (0xf < local_40) {
                                        if (0xfff < local_40 + 1) {
                                          if (0x1f < (ulonglong)
                                                     ((longlong)local_58[0] +
                                                     (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                          }
                                        }
                                        thunk_FUN_1407864b8();
                                      }
                                      DVar4 = timeGetTime();
                                      uVar6 = FUN_140216790();
                                      uVar7 = FUN_140216710();
                                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                    "TheAnimSFXMapper.System_Initialize",uVar7,uVar6
                                                    ,CONCAT44(uVar20,DVar4));
                                      if (cVar3 == '\0') {
                                        pcVar11 = "Error: Unable to initialize TheAnimSFXMapper.";
                                      }
                                      else {
                                        lVar14 = -1;
                                        local_58[0] = (undefined8 *******)0x0;
                                        local_48 = 0;
                                        local_40 = 0xf;
                                        do {
                                          lVar14 = lVar14 + 1;
                                        } while (PTR_s___Data_XML_CommandBarComponentFi_140a10808
                                                 [lVar14] != '\0');
                                        FUN_140022730(local_58);
                                        uVar21 = 1;
                                        uVar20 = 1;
                                        cVar3 = FUN_14030ae50(&DAT_140b27f60,local_58,DAT_140a619f0,
                                                              FUN_14009cec0,&LAB_14009db00,
                                                              FUN_1400374b0,FUN_1400375c0,
                                                              FUN_1400a7aa0,FUN_140037670,
                                                              FUN_140037720,&LAB_1400a3540,
                                                              &LAB_140037210,&LAB_140037230,
                                                              FUN_140037250,FUN_140037410,
                                                              FUN_140037760,FUN_140037450,
                                                              &LAB_140037470,&LAB_140037810,
                                                              FUN_1400d6770,&LAB_1400d63f0,
                                                              &LAB_1400378e0,&LAB_140037900,
                                                              &LAB_140037920,FUN_140037940,
                                                              FUN_140037ae0,FUN_140037c30,
                                                              FUN_140037a00,FUN_14009d2a0,
                                                              FUN_1400d3c20,FUN_1400dc410,
                                                              FUN_140037cb0,&LAB_140037ce0,
                                                              &LAB_140037cf0,0);
                                        if (0xf < local_40) {
                                          if (0xfff < local_40 + 1) {
                                            if (0x1f < (ulonglong)
                                                       ((longlong)local_58[0] +
                                                       (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                            }
                                          }
                                          thunk_FUN_1407864b8();
                                        }
                                        DVar4 = timeGetTime();
                                        uVar6 = FUN_140216790();
                                        uVar7 = FUN_140216710();
                                        uVar18 = CONCAT44(uVar20,DVar4);
                                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                      "TheCommandBar.System_Initialize",uVar7,uVar6,
                                                      uVar18);
                                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                        if (cVar3 == '\0') {
                                          pcVar11 = "Error: Unable to initialize TheCommandBar.";
                                        }
                                        else {
                                          cVar3 = FUN_140447190(&DAT_140b30350,FUN_140123d70,
                                                                FUN_14005f6b0,DAT_140a61a10);
                                          DVar4 = timeGetTime();
                                          uVar6 = FUN_140216790();
                                          uVar7 = FUN_140216710();
                                          uVar18 = CONCAT44(uVar20,DVar4);
                                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                        "TheTutorial.Init",uVar7,uVar6,uVar18);
                                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                          if (cVar3 == '\0') {
                                            pcVar11 = "Error: Unable to initialize TheTutorial.";
                                          }
                                          else {
                                            cVar3 = FUN_14044ce30(&DAT_140a28c10,DAT_140a619f0);
                                            DVar4 = timeGetTime();
                                            uVar6 = FUN_140216790();
                                            uVar7 = FUN_140216710();
                                            uVar18 = CONCAT44(uVar20,DVar4);
                                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                          "CameraFXManager.System_Initialize",uVar7,
                                                          uVar6,uVar18);
                                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                            if (cVar3 == '\0') {
                                              pcVar11 = 
                                              "Error: Unable to initialize CameraFXManager.";
                                            }
                                            else {
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (PTR_s___Data_XML_FactionFiles_xml_140a10830
                                                       [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              FUN_140331d70(&DAT_140a172d0,local_58);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                            "FactionList.Initialize",uVar7,uVar6,
                                                            uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (
                                                  PTR_s___Data_XML_TradeRouteLines_xml_140a10818
                                                  [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              cVar3 = FUN_140467740(&DAT_140a29f20,local_58,0);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                        
                                                  "TheTradeRouteLineManager.System_Initialize",uVar7
                                                  ,uVar6,uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              if (cVar3 == '\0') {
                                                pcVar11 = "Error: Unable to initialize FactionList."
                                                ;
                                              }
                                              else {
                                                if (DAT_140b31008 == 0) {
LAB_14004cc39:
                                                  bVar1 = false;
                                                }
                                                else {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_TargetingPrioritySetF_140a10858
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  uVar15 = 1;
                                                  cVar3 = FUN_140483f10(DAT_140b31008,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004cc39;
                                                  bVar1 = true;
                                                }
                                                if ((uVar15 != 0) && (0xf < local_40)) {
                                                  if (0xfff < local_40 + 1) {
                                                    if (0x1f < (ulonglong)
                                                               ((longlong)local_58[0] +
                                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28)
                                                      ;
                                                    }
                                                  }
                                                  thunk_FUN_1407864b8();
                                                }
                                                bVar2 = false;
                                                DVar4 = timeGetTime();
                                                uVar6 = FUN_140216790();
                                                uVar7 = FUN_140216710();
                                                uVar18 = CONCAT44(uVar20,DVar4);
                                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                            
                                                  "TheTargetingPrioritySetManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                if (bVar1) {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_HardPointDataFiles_xm_140a10820
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404738b0(&DAT_140a2a0b0,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HardPointDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HardPointDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_1404814d0(
                                                  PTR_s___Data_XML_HeroClash_xml_140a10800);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HeroClashTypeClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HeroClashTypeClass.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140408830(&DAT_140b2f020,
                                                                                                                                                    
                                                  PTR_s___Data_XML_GameObjectFiles_xml_140a107e0);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "GameObjectTypeManager.Read_Parse_Game_Object_Databases"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize GameObjectTypeManager."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_TradeRouteFiles_xml_140a10810
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404715e0(&DAT_140a2a060,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TradeRouteManager.System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TradeRouteManager.";
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_CampaignFiles_xml_140a10828
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140479fe0(&DAT_140b31160,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "CampaignDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize CampaignDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_14048f1b0(&LAB_1402e0550,
                                                                          FUN_1400f5200,
                                                                          FUN_1400d1f00);
                                                    DVar4 = timeGetTime();
                                                    uVar6 = FUN_140216790();
                                                    uVar7 = FUN_140216710();
                                                    uVar18 = CONCAT44(uVar20,DVar4);
                                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                    
                                                  "SpecialAbilityClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (PTR_s___Data_XML_Movies_xml_140a10848
                                                           [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140480560(&DAT_140a2a810,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "MovieManager.System_Initialize",
                                                                uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_LightningEffectTypes__140a10850
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140482490(&DAT_140a2a850,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheLightningTypeManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_WeatherScenarios_xml_140a10868
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140489660(&DAT_140a2a980,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheWeatherScenarioManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  uVar6 = FUN_14004fde0();
                                                  cVar3 = FUN_140477c10(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheMovementClassManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheMovementClassManager."
                                                  ;
                                                  goto LAB_14004b60c;
                                                  }
                                                  FUN_140248c60();
                                                  FUN_140248be0();
                                                  DAT_140b09d30 = 0;
                                                  if (DAT_140b31018 == 0) {
LAB_14004d373:
                                                    bVar1 = false;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_DifficultyAdjustments_140a10860
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  bVar2 = true;
                                                  cVar3 = FUN_140484e30(DAT_140b31018,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004d373;
                                                  bVar1 = true;
                                                  }
                                                  if ((bVar2) && (0xf < local_40)) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheDifficultyAdjustmentManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (!bVar1) goto LAB_14004b611;
                                                  DAT_140b31150 = &DAT_140a619ef;
                                                  uVar6 = FUN_14004fc30();
                                                  cVar3 = FUN_1404753b0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheAIDataManagerClass::Get().System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    FUN_1402149d0(
                                                  "Error: Unable to initialize TheAIDataManager.");
                                                  }
                                                  uVar6 = FUN_14004fd50();
                                                  cVar3 = FUN_1404889d0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheGameScoringManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheGameScoringManagerClass."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_WeatherModifiers_xml_140a10840
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_14046a350(&DAT_140b30b40,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "TheWeatherSystem.System_Initialize"
                                                                ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheWeatherSystem.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140488ec0(
                                                  PTR_s___Data_XML_LightSources_xml_140a107b8);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LightTypeManager::System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LightTypeManager.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140482d30(
                                                  PTR_s___Data_XML_LensFlares_xml_140a10778);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LensFlareTypeManager::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LensFlareTypeManager."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_WeatherAudio_xml_140a107f8
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140324e70(&DAT_140b2b3b0,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheWeatherAudioManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 != '\0') {
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar16 = lVar16 + 1;
                                                    } while (PTR_s_BlackMarketItems_xml_140a10880
                                                             [lVar16] != '\0');
                                                    FUN_140022730(local_58,
                                                  PTR_s_BlackMarketItems_xml_140a10880,lVar16);
                                                  FUN_14048f5b0(&PTR_vftable_140a2aa78,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar15 = FUN_140216790();
                                                  uVar6 = FUN_140216710();
                                                  uVar7 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "BlackMarketItemList.Initialize",
                                                                uVar6,uVar15,uVar7);
                                                  FUN_140451cd0();
                                                  DAT_140b2b5d0 = &DAT_1409cf314;
                                                  FUN_1404737c0(&DAT_140a2a0b0);
                                                  FUN_140479400(&DAT_140b31160);
                                                  FUN_140330450(&DAT_140a172d0);
                                                  FUN_1402c79d0(&DAT_140b15800);
                                                  _guard_check_icall(&DAT_140b30160);
                                                  FUN_140483ea0(DAT_140b31008);
                                                  FUN_1403239f0(&DAT_140b2b3b0);
                                                  FUN_140084a70();
                                                  FUN_1400a9800();
                                                  FUN_140086ff0();
                                                  FUN_140105240();
                                                  FUN_140107b70();
                                                  FUN_14008e880();
                                                  FUN_140085380();
                                                  FUN_140463c80(FUN_1400dca70,&LAB_14004fe70);
                                                  FUN_14047cb40(&DAT_140b311a8);
                                                  FUN_1402a73a0();
                                                  _DAT_140b15480 = FUN_140056480;
                                                  uVar9 = CONCAT71((int7)((ulonglong)uVar7 >> 8),1);
                                                  local_res10 = (code *)0x3d99999a3c23d70a;
                                                  local_res18 = 0x3c23d70a;
                                                  local_res1c = 0x3d99999a;
                                                  FUN_140049110(&DAT_1409cf320,&local_res18,
                                                                &local_res10,0,
                                                                uVar15 & 0xffffffffffffff00,uVar9,
                                                                "Arial Bold",CONCAT44(uVar21,9));
                                                  local_res1c = DAT_1408022fc;
                                                  local_res10 = (code *)0x3c23d70a3ccccccd;
                                                  local_res18 = 0x3ccccccd;
                                                  FUN_140049110(&DAT_1409cf390,&local_res18,
                                                                &local_res10,1,1,
                                                                uVar9 & 0xffffffffffffff00,"Arial",7
                                                               );
                                                  FUN_140049270(&DAT_1409cf390,DAT_1408007b4,
                                                                DAT_1408022f8,30000,0x18);
                                                  DAT_140b15668 = FUN_140038100;
                                                  FUN_140297630(&DAT_140a16fd0,0);
                                                  FUN_140297630(&DAT_140a17008,0);
                                                  FUN_140293390(&DAT_140a17008,1);
                                                  FUN_1403d4490(&DAT_140b27f60);
                                                  return 1;
                                                  }
                                                  pcVar11 = 
                                                  "Error: Unable to initialize the TheWeatherAudioManager."
                                                  ;
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                                else {
                                                  pcVar11 = 
                                                  "Error: Unable to initialize TargetingPrioritySetManager."
                                                  ;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_14004b60c:
  FUN_1402149d0(pcVar11);
LAB_14004b611:
  FUN_14004da50();
  return 0;
}



// ------------------------------------------------------------
// RVA: 0x1098e0  Name: FUN_1401098e0  Size: 197 bytes
// ------------------------------------------------------------

void FUN_1401098e0(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DAT_140a12221 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x1089e0  Name: FUN_1401089e0  Size: 1896 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401089e0(uint param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  longlong lVar9;
  undefined8 *******pppppppuVar10;
  longlong lVar11;
  char cVar12;
  undefined8 *puVar13;
  uint in_stack_ffffffffffffefa8;
  undefined8 ******local_1048;
  uint3 uStack_1040;
  char cStack_103d;
  undefined4 uStack_103c;
  ulonglong local_1038;
  ulonglong local_1030;
  short local_1028 [2048];
  
  if ((DAT_140a12224 != -1) && (DAT_140a12224 <= *(int *)(DAT_140a62328 + 0x10))) {
    puVar13 = (undefined8 *)((longlong)DAT_140a12224 * 0x70 + *(longlong *)(DAT_140a62328 + 8));
    *(undefined1 *)((longlong)puVar13 + 0x6d) = 1;
    puVar6 = puVar13;
    if (0xf < (ulonglong)puVar13[3]) {
      puVar6 = (undefined8 *)*puVar13;
    }
    plVar3 = (longlong *)FUN_140108430(puVar6,*(undefined4 *)(puVar13 + 0xd));
    cVar12 = '\x01';
    if (plVar3 != (longlong *)0x0) {
      lVar11 = *plVar3;
      uVar7 = (ulonglong)DAT_140a62340;
      if (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3)) {
        do {
          puVar6 = (undefined8 *)0x0;
          if (param_1 < DAT_140a6233c) {
            return;
          }
          if (cVar12 == '\0') {
            return;
          }
          puVar1 = *(undefined4 **)(lVar11 + uVar7 * 8);
          switch(*puVar1) {
          case 0:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = 0;
            FUN_1402d5010(&DAT_140b27e60,&local_1048,0,0,0);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
LAB_140108b67:
              thunk_FUN_1407864b8();
            }
            break;
          case 1:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = in_stack_ffffffffffffefa8 & 0xffffff00;
            FUN_1402d2070(&DAT_140b27dc0,&local_1048,0,0,in_stack_ffffffffffffefa8);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            break;
          case 2:
            lVar11 = *(longlong *)(puVar1 + 2);
            FUN_140322780(&DAT_140b2b3a8);
            FUN_1403207c0(&DAT_140b2b2f0);
            FUN_140321830(&DAT_140b2b300,1);
            lVar9 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar9 = lVar9 + 1;
            } while (*(char *)(lVar11 + lVar9) != '\0');
            FUN_140022730(&local_1048,lVar11);
            FUN_140320f80(&DAT_140b2b300,&local_1048,1);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            DAT_140a12221 = '\0';
            break;
          case 3:
            uVar5 = CONCAT71((int7)((ulonglong)lVar11 >> 8),1);
            goto LAB_140108cd1;
          case 4:
            uVar5 = 0;
LAB_140108cd1:
            FUN_1402fc7a0(&DAT_140b27f60,*(undefined8 *)(puVar1 + 2),uVar5);
            break;
          case 6:
            FUN_1401099f0(*(undefined8 *)(puVar1 + 2));
            break;
          case 7:
            if ((puVar13[6] != 0) && (puVar6 = puVar13 + 4, 0xf < (ulonglong)puVar13[7])) {
              puVar6 = (undefined8 *)*puVar6;
            }
            plVar4 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0;
            uVar7 = plVar4[2];
            if (7 < (ulonglong)plVar4[3]) {
              plVar4 = (longlong *)*plVar4;
            }
            if (uVar7 < 8) {
              local_1048 = (undefined8 ******)*plVar4;
              _uStack_1040 = *(undefined4 *)(plVar4 + 1);
              uStack_103c = *(uint *)((longlong)plVar4 + 0xc);
              uVar8 = 7;
            }
            else {
              uVar8 = uVar7 | 7;
              if (0x7ffffffffffffffe < uVar8) {
                uVar8 = 0x7ffffffffffffffe;
              }
              local_1048 = (undefined8 ******)FUN_1400226c0(&local_1048,uVar8 + 1);
              FUN_14076bd80(local_1048,plVar4,uVar7 * 2 + 2);
            }
            local_1038 = uVar7;
            local_1030 = uVar8;
            if (puVar6 != (undefined8 *)0x0) {
              pppppppuVar10 = &local_1048;
              if (7 < uVar8) {
                pppppppuVar10 = (undefined8 *******)local_1048;
              }
              FUN_140022b30(local_1028,0x800,pppppppuVar10,puVar6);
              lVar11 = -1;
              do {
                lVar11 = lVar11 + 1;
              } while (local_1028[lVar11] != 0);
              FUN_1400228a0(&local_1048,local_1028);
            }
            FUN_140108720(&local_1048);
            if (7 < local_1030) {
              if (0xfff < local_1030 * 2 + 2) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 * 2 + 0x29);
                }
              }
              thunk_FUN_1407864b8();
            }
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 8:
            FUN_1401093b0(puVar13[0xc]);
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 9:
            local_1048 = (undefined8 ******)s_IDC_STATIC_1408181e0._0_8_;
            local_1030 = 0xf;
            local_1038 = 10;
            uStack_1040._2_1_ = '\0';
            uStack_1040._0_1_ = s_IDC_STATIC_1408181e0[8];
            uStack_1040._1_1_ = s_IDC_STATIC_1408181e0[9];
            uVar5 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            FUN_1401c8ea0(&DAT_140a78510,DAT_140a12218,&local_1048,uVar5);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 0xb:
            puVar2 = *(undefined1 **)(puVar1 + 2);
            in_stack_ffffffffffffefa8 = CONCAT31((int3)(in_stack_ffffffffffffefa8 >> 8),puVar2[0xc])
            ;
            FUN_1401f9470(&DAT_140a62348,*puVar2,puVar2[4],puVar2[8],in_stack_ffffffffffffefa8);
            break;
          case 0xc:
            FUN_1401091b0(**(undefined4 **)(puVar1 + 2));
            break;
          case 0xd:
            DAT_140a6233c = **(int **)(puVar1 + 2) + param_1;
            break;
          case 0x10:
            FUN_140109bf0();
            break;
          case 0x12:
            FUN_14030ac40(&DAT_140b27f60);
            break;
          case 0x13:
            cVar12 = DAT_140a12221;
            if (DAT_140a12221 != '\0') break;
            goto LAB_1401090cc;
          }
          DAT_140a62340 = DAT_140a62340 + 1;
LAB_1401090cc:
          lVar11 = *plVar3;
          uVar7 = (ulonglong)DAT_140a62340;
        } while (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3));
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x107900  Name: FUN_140107900  Size: 611 bytes
// ------------------------------------------------------------

void FUN_140107900(void)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  int iVar11;
  int iVar12;
  ulonglong uVar13;
  
  if (DAT_140a62320 != '\0') {
    FUN_1401c1d20(&DAT_140a78510,DAT_140a12218,1);
    DAT_140a12218 = 0xffffffff;
    uVar13 = (ulonglong)*(int *)(DAT_140a62328 + 0x10);
    iVar7 = *(int *)(DAT_140a62328 + 0x10) + -1;
    if (-1 < iVar7) {
      lVar9 = (longlong)iVar7 * 0x70;
      lVar4 = uVar13 * 0x70;
      lVar8 = DAT_140a62328;
      lVar10 = lVar9;
      do {
        lVar3 = *(longlong *)(lVar8 + 8);
        iVar12 = (int)uVar13;
        if ((((*(char *)(lVar9 + 0x6e + lVar3) != '\0') && (*(char *)(lVar9 + 0x6d + lVar3) != '\0')
             ) && (-1 < iVar7)) && (iVar11 = *(int *)(lVar8 + 0x10), iVar7 < iVar11)) {
          uVar5 = (iVar11 - iVar7) - 1;
          uVar13 = (ulonglong)uVar5;
          if (uVar5 != 0) {
            if ((*(uint *)(lVar8 + 0x14) & 0x80000000) == 0) {
              if (iVar12 < iVar7) {
                iVar6 = iVar7 + -1 + uVar5;
                if (0 < (int)uVar5) {
                  do {
                    iVar11 = iVar11 + -1;
                    lVar3 = (longlong)iVar6;
                    iVar6 = iVar6 + -1;
                    FUN_14010a170(lVar3 * 0x70 + *(longlong *)(lVar8 + 8),
                                  (longlong)iVar11 * 0x70 + *(longlong *)(lVar8 + 8));
                    uVar13 = uVar13 - 1;
                  } while (uVar13 != 0);
                }
              }
              else if (0 < (int)uVar5) {
                iVar11 = 0;
                do {
                  iVar2 = iVar12 + iVar11;
                  iVar6 = iVar11 + iVar7;
                  iVar11 = iVar11 + 1;
                  FUN_14010a170((longlong)iVar6 * 0x70 + *(longlong *)(lVar8 + 8),
                                (longlong)iVar2 * 0x70 + *(longlong *)(lVar8 + 8));
                } while (iVar11 < (int)uVar5);
              }
            }
            else {
              FUN_14076bd80(lVar3 + lVar10,lVar4 + lVar3,(longlong)(int)uVar5 * 0x70);
            }
          }
          *(int *)(lVar8 + 0x10) = *(int *)(lVar8 + 0x10) + -1;
          lVar8 = DAT_140a62328;
        }
        lVar4 = lVar4 + -0x70;
        lVar10 = lVar10 + -0x70;
        uVar13 = (ulonglong)(iVar12 - 1);
        lVar9 = lVar9 + -0x70;
        iVar7 = iVar7 + -1;
      } while (-1 < iVar7);
    }
    if (DAT_140a12221 == '\0') {
      FUN_140321830(&DAT_140b2b300,1);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
    }
    cVar1 = FUN_14028af60(&DAT_140b153e0);
    if (cVar1 == '\0') {
      iVar7 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (iVar7 == 0) {
        FUN_14028c720(&DAT_140b153e0,1);
      }
      else if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
    }
    FUN_14030ac40(&DAT_140b27f60);
    FUN_14032edd0(DAT_140b15418 + 0x14,"close_story_dialog");
    DAT_140a62320 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x105d30  Name: FUN_140105d30  Size: 190 bytes
// ------------------------------------------------------------

void FUN_140105d30(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x105190  Name: FUN_140105190  Size: 163 bytes
// ------------------------------------------------------------

void FUN_140105190(void)

{
  if (DAT_140a622e0 != '\0') {
    if (DAT_140a622f0 != 0) {
      FUN_140271a10();
    }
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_1401c1d20(&DAT_140a78510,DAT_140a121f0,1);
    DAT_140a121f0 = 0xffffffff;
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_14032edd0(DAT_140b15418 + 0xa0,"close_story_dialog");
    DAT_140a622e0 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x1053c0  Name: FUN_1401053c0  Size: 1988 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401053c0(uint param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined4 *puVar3;
  ulonglong uVar4;
  undefined1 *puVar5;
  uint uVar6;
  uint uVar7;
  longlong *plVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  longlong lVar11;
  undefined8 *****pppppuVar12;
  longlong lVar13;
  undefined8 *puVar14;
  uint in_stack_ffffffffffffef98;
  undefined8 ****local_1058;
  undefined4 uStack_1050;
  uint3 uStack_104c;
  undefined1 uStack_1049;
  ulonglong local_1048;
  ulonglong local_1040;
  short local_1038 [2056];
  
  uVar7 = DAT_1407ffaf8;
  lVar1 = DAT_140a622e8[1];
  lVar2 = *DAT_140a622e8;
  while ((DAT_140a62304 < (uint)(lVar1 - lVar2 >> 3) && (DAT_140a62300 <= param_1))) {
    puVar3 = *(undefined4 **)(*DAT_140a622e8 + (ulonglong)DAT_140a62304 * 8);
    switch(*puVar3) {
    case 0:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = 0;
      FUN_1402d5010(&DAT_140b27e60,&local_1058,0,0,0);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 1:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = in_stack_ffffffffffffef98 & 0xffffff00;
      FUN_1402d2070(&DAT_140b27dc0,&local_1058,0,0,in_stack_ffffffffffffef98);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 2:
      lVar13 = *(longlong *)(puVar3 + 2);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,1);
      lVar11 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar11 = lVar11 + 1;
      } while (*(char *)(lVar13 + lVar11) != '\0');
      FUN_140022730(&local_1058,lVar13);
      FUN_140320f80(&DAT_140b2b300,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 3:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),CONCAT71((uint7)(uint3)(DAT_140a62304 >> 8),1));
      break;
    case 4:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),0);
      break;
    case 5:
      if (DAT_140a622f0 != 0) {
        in_stack_ffffffffffffef98 = uVar7;
        FUN_140271700(DAT_140a622f0,**(undefined1 **)(puVar3 + 2),
                      CONCAT71((int7)((ulonglong)puVar3 >> 8),1),0,uVar7);
      }
      break;
    case 6:
      FUN_140105fd0(*(undefined8 *)(puVar3 + 2));
      break;
    case 7:
      plVar8 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      local_1058 = (undefined8 ****)0x0;
      local_1048 = 0;
      local_1040 = 0;
      uVar4 = plVar8[2];
      if (7 < (ulonglong)plVar8[3]) {
        plVar8 = (longlong *)*plVar8;
      }
      if (uVar4 < 8) {
        local_1058 = (undefined8 ****)*plVar8;
        uStack_1050 = *(undefined4 *)(plVar8 + 1);
        _uStack_104c = *(undefined4 *)((longlong)plVar8 + 0xc);
        uVar10 = 7;
      }
      else {
        uVar10 = uVar4 | 7;
        if (0x7ffffffffffffffe < uVar10) {
          uVar10 = 0x7ffffffffffffffe;
        }
        local_1058 = (undefined8 ****)FUN_1400226c0(&local_1058,uVar10 + 1);
        FUN_14076bd80(local_1058,plVar8,uVar4 * 2 + 2);
      }
      local_1048 = uVar4;
      local_1040 = uVar10;
      if (DAT_140a12208 != 0) {
        puVar14 = &DAT_140a121f8;
        if (7 < DAT_140a12210) {
          puVar14 = DAT_140a121f8;
        }
        pppppuVar12 = &local_1058;
        if (7 < uVar10) {
          pppppuVar12 = (undefined8 *****)local_1058;
        }
        FUN_140022b30(local_1038,0x800,pppppuVar12,puVar14);
        lVar13 = -1;
        do {
          lVar13 = lVar13 + 1;
        } while (local_1038[lVar13] != 0);
        FUN_1400228a0(&local_1058,local_1038);
      }
      FUN_140106360(&local_1058);
      if (7 < local_1040) {
        if (0xfff < local_1040 * 2 + 2) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 8:
      FUN_140106680();
      break;
    case 9:
      local_1040 = 0xf;
      local_1048 = 10;
      uStack_1050._2_1_ = '\0';
      uStack_1050._0_2_ = s_IDC_STATIC_1408181e0._8_2_;
      local_1058 = (undefined8 ****)s_IDC_STATIC_1408181e0._0_8_;
      uVar9 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      FUN_1401c8ea0(&DAT_140a78510,DAT_140a121f0,&local_1058,uVar9);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 10:
      local_1040 = 0xf;
      local_1048 = 0xe;
      uVar6 = (uint)_uStack_104c >> 0x18;
      uStack_1049 = (undefined1)uVar6;
      uStack_104c = (uint3)(ushort)s_IDC_STORY_TEXT_140818168._12_2_;
      local_1058 = (undefined8 ****)s_IDC_STORY_TEXT_140818168._0_8_;
      uStack_1050 = s_IDC_STORY_TEXT_140818168._8_4_;
      FUN_1401c40b0(&DAT_140a78510,DAT_140a121f0,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0xb:
      puVar5 = *(undefined1 **)(puVar3 + 2);
      in_stack_ffffffffffffef98 = CONCAT31((int3)(in_stack_ffffffffffffef98 >> 8),puVar5[0xc]);
      FUN_1401f9470(&DAT_140a62310,*puVar5,puVar5[4],puVar5[8],in_stack_ffffffffffffef98);
      break;
    case 0xc:
      FUN_140106bb0(**(undefined4 **)(puVar3 + 2));
      break;
    case 0xd:
      DAT_140a62300 = **(int **)(puVar3 + 2) + param_1;
      break;
    case 0xe:
      if (**(char **)(puVar3 + 2) == '\0') {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
      else {
        (**(code **)(*DAT_140b15418 + 0x38))();
      }
      break;
    case 0xf:
      FUN_140106e60(*(undefined8 *)(puVar3 + 2));
      break;
    case 0x10:
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._0_2_ = (ushort)(byte)s_IDC_IMAGE_1408002e8[8];
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      FUN_1401c3810(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._1_1_ = '\0';
      uStack_1050._0_1_ = s_IDC_IMAGE_1408002e8[8];
      FUN_1401c38d0(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0x11:
      if (DAT_140a622f0 != 0) {
        *(undefined1 *)(DAT_140a622f0 + 0x1d) = **(undefined1 **)(puVar3 + 2);
      }
      break;
    case 0x12:
      if (DAT_140b30690 != 0) {
        FUN_14044f130(&DAT_140b306dd);
      }
    }
    DAT_140a62304 = DAT_140a62304 + 1;
  }
  s_IDC_STORY_TEXT_140818168[8] = (char)s_IDC_STORY_TEXT_140818168._8_4_;
  s_IDC_STORY_TEXT_140818168[9] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,1);
  s_IDC_STORY_TEXT_140818168[10] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,2);
  s_IDC_STORY_TEXT_140818168[0xb] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,3);
  s_IDC_STORY_TEXT_140818168[0xc] = (char)s_IDC_STORY_TEXT_140818168._12_2_;
  s_IDC_STORY_TEXT_140818168[0xd] = (char)((ushort)s_IDC_STORY_TEXT_140818168._12_2_ >> 8);
  s_IDC_STATIC_1408181e0[8] = (char)s_IDC_STATIC_1408181e0._8_2_;
  s_IDC_STATIC_1408181e0[9] = (char)((ushort)s_IDC_STATIC_1408181e0._8_2_ >> 8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x22f10  Name: FUN_140022f10  Size: 1749 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140022f10(int *param_1)

{
  char *pcVar1;
  char cVar2;
  undefined8 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  DWORD DVar8;
  undefined8 uVar9;
  longlong *plVar10;
  longlong lVar11;
  undefined8 *puVar12;
  longlong lVar13;
  undefined8 *****pppppuVar14;
  undefined8 *puVar15;
  uint uVar16;
  undefined8 *puVar17;
  undefined8 *puVar18;
  longlong *in_stack_fffffffffffffe50;
  undefined8 ****local_198;
  char local_190 [2];
  undefined1 local_18e;
  longlong local_188;
  ulonglong local_180;
  char *local_178 [2];
  undefined8 local_168;
  ulonglong local_160;
  undefined8 local_158;
  undefined4 local_150;
  undefined8 ****local_148 [2];
  longlong local_138;
  undefined8 local_130;
  char local_128 [8];
  char acStack_120 [4];
  char acStack_11c [4];
  char local_118;
  undefined1 local_117 [271];
  
  if (DAT_140b26fc0 != 0) {
    *(undefined1 *)(param_1 + 3) = 1;
    param_1[4] = DAT_140a13e10;
    DAT_140b27e40 = 1;
    DAT_140b27ef4 = 1;
    puVar17 = (undefined8 *)0x0;
    DAT_140a13e10 = 0;
    *param_1 = *param_1 + 1;
    iVar7 = *param_1;
    if (DAT_140b26fc0 <= *param_1) {
      *param_1 = 0;
      iVar7 = 0;
    }
    FUN_14001e680(local_148,(longlong)iVar7 * 0x20 + _DAT_140b26fb8);
    if (local_138 != 0) {
      puVar12 = puVar17;
      if (DAT_140a785a8 == 0) {
        uVar9 = FUN_1402c8210(&DAT_140b15800,DAT_140a7c090,DAT_140a13ed8);
        FUN_14001e680(&local_198,uVar9);
        if (local_188 == 0) {
          FUN_140022730(&local_198,"Splash.TGA",10);
        }
        pppppuVar14 = &local_198;
        if (0xf < local_180) {
          pppppuVar14 = (undefined8 *****)local_198;
        }
        plVar10 = (longlong *)FUN_14025e990(pppppuVar14,1,0);
        if (plVar10 != (longlong *)0x0) {
          (**(code **)*plVar10)(plVar10);
        }
        lVar11 = FUN_140769c58(0x90);
        if (lVar11 != 0) {
          in_stack_fffffffffffffe50 = plVar10;
          puVar12 = (undefined8 *)FUN_14025ff10(lVar11,0,0,DAT_1407ffaf8,DAT_1407ffaf8,plVar10);
        }
        if (plVar10 != (longlong *)0x0) {
          (**(code **)(*plVar10 + 8))(plVar10);
        }
        if (0xf < local_180) {
          if (0xfff < local_180 + 1) {
            if (0x1f < (ulonglong)((longlong)local_198 + (-8 - (longlong)local_198[-1]))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(local_198[-1],local_180 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        local_188 = 0;
        local_180 = 0xf;
        local_198 = (undefined8 ****)((ulonglong)local_198 & 0xffffffffffffff00);
      }
      thunk_FUN_14003f220(&DAT_140a60df0);
      FUN_1400337b0(&DAT_140a60df0);
      FUN_14028d940(&DAT_140b153e0);
      FUN_140297630(&DAT_140a16fd0,0);
      DAT_140b155e8 = 0;
      FUN_1400338e0(&DAT_140a60df0);
      FUN_140049350(&DAT_1409cf320);
      FUN_140049350(&DAT_1409cf390);
      FUN_1403027d0(&DAT_140b27f60);
      puVar3 = DAT_140b2b5e8;
      if (DAT_140b2b5f0 != 0) {
        if (DAT_140b2b5f0 < DAT_140b2b618 >> 3) {
          FUN_140023670(&DAT_140b2b5e0,*DAT_140b2b5e8,DAT_140b2b5e8);
        }
        else {
          *(undefined8 *)DAT_140b2b5e8[1] = 0;
          puVar3 = (undefined8 *)*puVar3;
          while (puVar3 != (undefined8 *)0x0) {
            puVar15 = (undefined8 *)*puVar3;
            thunk_FUN_1407864b8(puVar3,0x38);
            puVar3 = puVar15;
          }
          *DAT_140b2b5e8 = DAT_140b2b5e8;
          DAT_140b2b5e8[1] = DAT_140b2b5e8;
          puVar3 = DAT_140b2b5e8;
          DAT_140b2b5f0 = 0;
          puVar15 = (undefined8 *)
                    ((ulonglong)((longlong)DAT_140b2b600 + (7 - (longlong)DAT_140b2b5f8)) >> 3);
          if (DAT_140b2b600 < DAT_140b2b5f8) {
            puVar15 = puVar17;
          }
          puVar18 = DAT_140b2b5f8;
          if (puVar15 != (undefined8 *)0x0) {
            for (; puVar15 != (undefined8 *)0x0; puVar15 = (undefined8 *)((longlong)puVar15 + -1)) {
              *puVar18 = puVar3;
              puVar18 = puVar18 + 1;
            }
          }
        }
      }
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300);
      FUN_1402d2790(&DAT_140b27dc0);
      FUN_1402d6f20(&DAT_140b27e60);
      FUN_140323660(&DAT_140b2b3b0);
      local_118 = s___Data_Art_Maps__1407ffab8[0x10];
      local_128[0] = s___Data_Art_Maps__1407ffab8[0];
      local_128[1] = s___Data_Art_Maps__1407ffab8[1];
      local_128[2] = s___Data_Art_Maps__1407ffab8[2];
      local_128[3] = s___Data_Art_Maps__1407ffab8[3];
      local_128[4] = s___Data_Art_Maps__1407ffab8[4];
      local_128[5] = s___Data_Art_Maps__1407ffab8[5];
      local_128[6] = s___Data_Art_Maps__1407ffab8[6];
      local_128[7] = s___Data_Art_Maps__1407ffab8[7];
      acStack_120[0] = s___Data_Art_Maps__1407ffab8[8];
      acStack_120[1] = s___Data_Art_Maps__1407ffab8[9];
      acStack_120[2] = s___Data_Art_Maps__1407ffab8[10];
      acStack_120[3] = s___Data_Art_Maps__1407ffab8[0xb];
      acStack_11c[0] = s___Data_Art_Maps__1407ffab8[0xc];
      acStack_11c[1] = s___Data_Art_Maps__1407ffab8[0xd];
      acStack_11c[2] = s___Data_Art_Maps__1407ffab8[0xe];
      acStack_11c[3] = s___Data_Art_Maps__1407ffab8[0xf];
      FUN_14076c430(local_117,0,0xf3);
      pppppuVar14 = local_148;
      if (0xf < local_130) {
        pppppuVar14 = (undefined8 *****)local_148[0];
      }
      lVar11 = (longlong)&local_130 + 7;
      do {
        pcVar1 = (char *)(lVar11 + 1);
        lVar11 = lVar11 + 1;
      } while (*pcVar1 != '\0');
      lVar13 = 0;
      do {
        cVar2 = *(char *)((longlong)pppppuVar14 + lVar13);
        *(char *)(lVar11 + lVar13) = cVar2;
        lVar13 = lVar13 + 1;
      } while (cVar2 != '\0');
      FUN_1401fece0(CONCAT71((int7)((ulonglong)lVar11 >> 8),1));
      local_178[0] = (char *)0x0;
      local_168 = 0;
      local_160 = 0xf;
      lVar11 = -1;
      do {
        lVar13 = lVar11 + 1;
        lVar11 = lVar11 + 1;
      } while (local_128[lVar13] != '\0');
      FUN_140022730(local_178,local_128);
      FUN_140056480(local_178,0,0xffffffff,3,1);
      if (0xf < local_160) {
        if (0xfff < local_160 + 1) {
          if ((char *)0x1f < local_178[0] + (-8 - *(longlong *)(local_178[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_178[0] + -8),local_160 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      FUN_1401fece0(0);
      FUN_1402c1e30(&DAT_140b15690);
      FUN_1402c0ab0(&DAT_140b15690,local_148,0);
      if (puVar12 != (undefined8 *)0x0) {
        (**(code **)*puVar12)(puVar12,1);
      }
      FUN_1402a74f0(DAT_140b15418[3],0);
      FUN_1402a74f0(DAT_140b15418[4],0);
      if (DAT_140b15418 != (longlong *)0x0) {
        lVar11 = DAT_140b15418[0x12];
        lVar13 = (**(code **)(*DAT_140b15418 + 0x1e8))();
        if ((lVar11 != 0) && (lVar13 != 0)) {
          FUN_1402d03f0(lVar13,lVar11);
        }
      }
      local_190[0] = s_BasicRebel_1407ffad0[8];
      local_190[1] = s_BasicRebel_1407ffad0[9];
      local_198 = (undefined8 ****)s_BasicRebel_1407ffad0._0_8_;
      local_180 = 0xf;
      local_188 = 10;
      local_18e = 0;
      uVar9 = FUN_140294bc0(&DAT_140a16fd0,0);
      FUN_1402886d0(uVar9,&local_198);
      if (0xf < local_180) {
        if (0xfff < local_180 + 1) {
          if (0x1f < (ulonglong)((longlong)local_198 + (-8 - (longlong)local_198[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_198[-1],local_180 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_178[0] = (char *)FUN_140769c58(0x20);
      uVar6 = s_AI_Player_Underworld_1407ffae0._12_4_;
      uVar5 = s_AI_Player_Underworld_1407ffae0._8_4_;
      uVar4 = s_AI_Player_Underworld_1407ffae0._4_4_;
      local_168 = 0x14;
      local_160 = 0x1f;
      *(undefined4 *)local_178[0] = s_AI_Player_Underworld_1407ffae0._0_4_;
      *(undefined4 *)(local_178[0] + 4) = uVar4;
      *(undefined4 *)(local_178[0] + 8) = uVar5;
      *(undefined4 *)(local_178[0] + 0xc) = uVar6;
      *(undefined4 *)(local_178[0] + 0x10) = s_AI_Player_Underworld_1407ffae0._16_4_;
      local_178[0][0x14] = '\0';
      uVar9 = FUN_140294bc0(&DAT_140a16fd0,6);
      FUN_1402886d0(uVar9,local_178);
      if (0xf < local_160) {
        if (0xfff < local_160 + 1) {
          if ((char *)0x1f < local_178[0] + (-8 - *(longlong *)(local_178[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_178[0] + -8),local_160 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_158 = 0;
      local_150 = 0;
      lVar11 = FUN_1402a9ff0(DAT_140b15418[3],0x1a,0xffffffff,0xffffffff,&local_158,
                             (ulonglong)in_stack_fffffffffffffe50 & 0xffffffff00000000,0x7fffffff,
                             0xffffffff);
      puVar12 = puVar17;
      if (0 < *(int *)(lVar11 + 0x10)) {
        do {
          uVar16 = (int)puVar17 + 1;
          puVar17 = (undefined8 *)(ulonglong)uVar16;
          *(undefined1 *)
           (*(longlong *)(*(longlong *)(lVar11 + 8) + -8 + (longlong)(puVar12 + 1)) + 0x3cb) = 1;
          puVar12 = puVar12 + 1;
        } while ((int)uVar16 < *(int *)(lVar11 + 0x10));
      }
      DAT_140b155c4 = 1;
      FUN_1402cca80(&DAT_140b27a10,1);
      FUN_1400c6c30();
      DVar8 = timeGetTime();
      param_1[1] = DVar8 - _DAT_140a7bec0;
    }
    if (0xf < local_130) {
      if (0xfff < local_130 + 1) {
        if (0x1f < (ulonglong)((longlong)local_148[0] + (-8 - (longlong)local_148[0][-1]))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(local_148[0][-1],local_130 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x4623e0  Name: FUN_1404623e0  Size: 248 bytes
// ------------------------------------------------------------

void FUN_1404623e0(longlong param_1,undefined8 *param_2)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  if ((*(longlong *)(param_1 + 0x60) != 0) || (param_2 != (undefined8 *)0x0)) {
    if ((param_2 == (undefined8 *)0x0) &&
       ((param_2 = (undefined8 *)0x0, *(longlong *)(param_1 + 0x60) != 0 &&
        (param_2 = (undefined8 *)(param_1 + 0x50), 0xf < *(ulonglong *)(param_1 + 0x68))))) {
      param_2 = (undefined8 *)*param_2;
    }
    FUN_140322780(&DAT_140b2b3a8);
    FUN_140321830(&DAT_140b2b300,1);
    lVar1 = -1;
    local_28[0] = 0;
    local_18 = 0;
    local_10 = 0xf;
    do {
      lVar1 = lVar1 + 1;
    } while (*(char *)((longlong)param_2 + lVar1) != '\0');
    FUN_140022730(local_28,param_2);
    FUN_140320f80(&DAT_140b2b300,local_28,1);
    if (0xf < local_10) {
      if (0xfff < local_10 + 1) {
        if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140467540("STORY REWARD - Speech %s\n",param_2);
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x4608c0  Name: FUN_1404608c0  Size: 620 bytes
// ------------------------------------------------------------

void FUN_1404608c0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  char *pcVar3;
  longlong *plVar4;
  longlong lVar5;
  bool bVar6;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  if (*(longlong *)(param_1 + 0x60) != 0) {
    FUN_1404614a0();
  }
  if (*(longlong *)(param_1 + 0x140) != 0) {
    puVar2 = (undefined8 *)(param_1 + 0x130);
    if (0xf < *(ulonglong *)(param_1 + 0x148)) {
      puVar2 = (undefined8 *)*puVar2;
    }
    if (*(longlong *)(param_1 + 0x60) == 0) {
      if (puVar2 == (undefined8 *)0x0) goto LAB_1404609e6;
    }
    else if (((puVar2 == (undefined8 *)0x0) &&
             (puVar2 = (undefined8 *)0x0, *(longlong *)(param_1 + 0x60) != 0)) &&
            (puVar2 = (undefined8 *)(param_1 + 0x50), 0xf < *(ulonglong *)(param_1 + 0x68))) {
      puVar2 = (undefined8 *)*puVar2;
    }
    FUN_140322780(&DAT_140b2b3a8);
    FUN_140321830(&DAT_140b2b300,1);
    lVar5 = -1;
    local_28[0] = 0;
    local_18 = 0;
    local_10 = 0xf;
    do {
      lVar5 = lVar5 + 1;
    } while (*(char *)((longlong)puVar2 + lVar5) != '\0');
    FUN_140022730(local_28,puVar2);
    FUN_140320f80(&DAT_140b2b300,local_28,1);
    if (0xf < local_10) {
      if (0xfff < local_10 + 1) {
        if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140467540("STORY REWARD - Speech %s\n",puVar2);
  }
LAB_1404609e6:
  if (*(longlong *)(param_1 + 0x160) == 0) {
    return;
  }
  if (*(longlong *)(param_1 + 0x180) == 0) {
    plVar4 = (longlong *)(param_1 + 0x150);
    if (0xf < *(ulonglong *)(param_1 + 0x168)) {
      plVar4 = (longlong *)*plVar4;
    }
    if ((*(longlong *)(param_1 + 0x60) == 0) && (plVar4 == (longlong *)0x0)) {
LAB_140460a8e:
      FUN_140467540(
                   "STORY REWARD - ERROR!  Commandbar movie failed due to incomplete parameters.\r\n"
                   );
      return;
    }
    iVar1 = FID_conflict_atoi("0");
    bVar6 = iVar1 != 0;
    if ((plVar4 == (longlong *)0x0) &&
       (plVar4 = (longlong *)(param_1 + 0x50), 0xf < *(ulonglong *)(param_1 + 0x68))) {
      plVar4 = (longlong *)*plVar4;
    }
    goto LAB_140460b0b;
  }
  pcVar3 = (char *)(param_1 + 0x170);
  if (0xf < *(ulonglong *)(param_1 + 0x188)) {
    pcVar3 = *(char **)pcVar3;
  }
  plVar4 = (longlong *)(param_1 + 0x150);
  if (0xf < *(ulonglong *)(param_1 + 0x168)) {
    plVar4 = (longlong *)*plVar4;
  }
  if ((*(longlong *)(param_1 + 0x60) == 0) && (plVar4 == (longlong *)0x0)) goto LAB_140460a8e;
  bVar6 = false;
  if (*(longlong *)(param_1 + 0x80) == 0) {
    if (pcVar3 != (char *)0x0) goto LAB_140460adc;
  }
  else if (pcVar3 == (char *)0x0) {
    pcVar3 = (char *)(param_1 + 0x70);
    if (0xf < *(ulonglong *)(param_1 + 0x88)) {
      pcVar3 = *(char **)pcVar3;
    }
    iVar1 = FID_conflict_atoi(pcVar3);
    if (iVar1 != 0) {
      bVar6 = true;
    }
  }
  else {
LAB_140460adc:
    iVar1 = FID_conflict_atoi(pcVar3);
    bVar6 = false;
    if (iVar1 != 0) {
      bVar6 = true;
    }
  }
  if ((plVar4 == (longlong *)0x0) &&
     (plVar4 = (longlong *)(param_1 + 0x50), 0xf < *(ulonglong *)(param_1 + 0x68))) {
    plVar4 = (longlong *)*plVar4;
  }
LAB_140460b0b:
  FUN_1402fc7a0(&DAT_140b27f60,plVar4,bVar6);
  return;
}



// ------------------------------------------------------------
// RVA: 0x60c60  Name: FUN_140060c60  Size: 3957 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

LRESULT FUN_140060c60(HWND param_1,uint param_2,ulonglong param_3,int *param_4)

{
  int iVar1;
  ulonglong uVar2;
  char cVar3;
  char cVar4;
  DWORD DVar5;
  undefined8 uVar6;
  longlong lVar7;
  LRESULT LVar8;
  short sVar9;
  int iVar10;
  uint uVar11;
  short sVar12;
  float fVar13;
  float fVar14;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88 [2];
  tagPAINTSTRUCT local_78;
  
  cVar3 = FUN_1401d4170();
  if (cVar3 != '\0') {
    return 1;
  }
  sVar9 = (short)param_4;
  uVar11 = (uint)param_4;
  sVar12 = (short)((ulonglong)param_4 >> 0x10);
  if (0x111 < param_2) {
    if (param_2 < 0x215) {
      if (param_2 == 0x214) {
        SetWindowPos(param_1,(HWND)0x0,0,0,param_4[2] - *param_4,param_4[3] - param_4[1],2);
        FUN_140304770(&DAT_140b27f60,0);
        return 0;
      }
      switch(param_2) {
      case 0x112:
        lVar7 = thunk_FUN_1401775f0(param_3);
        if (lVar7 != 0) {
          return 1;
        }
        break;
      case 0x200:
        if (DAT_1409cf314 == '\0') {
          return 0;
        }
        FUN_140022ea0(&DAT_1409cf080,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
        local_90._0_4_ = (float)(int)sVar9;
        local_90._4_4_ = (float)(int)sVar12;
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,0xb,&local_90);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,0xb,&local_90);
        if (cVar3 != '\0') {
          return 0;
        }
        if (DAT_140a785a8 == 0) {
          local_98._0_4_ = (float)local_90;
          local_98._4_4_ = local_90._4_4_;
          FUN_1402f6c30(&DAT_140b27f60,7,&local_98);
          return 0;
        }
        DAT_140b2a08c = (float)local_90;
        DAT_140b2a090 = local_90._4_4_;
        if (DAT_140a61eb9 != '\0') {
          FUN_1402e7430(&DAT_140b27f60);
          return 0;
        }
        return 0;
      case 0x201:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,2);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,2);
        if (cVar3 != '\0') {
          return 0;
        }
        fVar14 = (float)(int)(short)local_90;
        fVar13 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,0,0,local_88,0);
        cVar4 = FUN_140065d00();
        if ((cVar4 == '\0') && (cVar3 != '\0')) {
          return 0;
        }
        local_98 = CONCAT44(fVar13,fVar14);
        cVar3 = FUN_14004a260(&DAT_1409cf390,0,&local_98);
        if (cVar3 != '\0') {
          return 0;
        }
        local_98 = CONCAT44(fVar13,fVar14);
        cVar3 = FUN_1402f6c30(&DAT_140b27f60,0,&local_98);
        if (cVar3 == '\0') {
          local_98 = CONCAT44(fVar13,fVar14);
          FUN_140030fc0(&DAT_140a60df0,0,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x202:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        sVar9 = (short)local_90;
        uVar2 = (ulonglong)local_90 >> 0x10;
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,1,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          if ((DAT_140a60eec == '\0') && (DAT_140a60edf == '\0')) {
            local_98 = CONCAT44((float)(int)(short)uVar2,(float)(int)sVar9);
            cVar3 = FUN_1402f6c30(&DAT_140b27f60,1,&local_98);
            if (cVar3 != '\0') goto LAB_140061407;
          }
          FUN_140307860(&DAT_140b27f60,0,0);
        }
        else if (DAT_140a61eb9 == '\0') {
          if (DAT_140a62320 != '\0') {
            FUN_1402e7260(&DAT_140b27f60);
          }
        }
        else {
          FUN_1402e7350(&DAT_140b27f60);
        }
LAB_140061407:
        if (DAT_140a28c91 == '\0') {
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,1,&local_98,cVar3,0);
          return 0;
        }
        return 0;
      case 0x203:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,4);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,4);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,6,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,4,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,6,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x204:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,5);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,5);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,2,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,2,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,2,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x205:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,3,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          if ((DAT_140a60edf == '\0') && (DAT_140a60eec == '\0')) {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            cVar3 = FUN_1402f6c30(&DAT_140b27f60,3,&local_98);
            if (cVar3 != '\0') {
              return 0;
            }
          }
          FUN_140307860(&DAT_140b27f60,0,0);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          FUN_140030fc0(&DAT_140a60df0,3,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x206:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,7);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,7);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,7,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,5,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,7,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x207:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,8);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,8);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,4,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,4,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x208:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,5,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,5,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x209:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,10);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,10);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,8,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,6,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,8,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x20a:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        sVar9 = (short)(param_3 >> 0x10);
        iVar10 = (int)sVar9;
        iVar1 = (iVar10 >> 0x1f & 1U) + 9;
        sVar12 = -(sVar9 / 0x78);
        if (-1 < sVar9) {
          sVar12 = sVar9 / 0x78;
        }
        local_90 = CONCAT44((float)iVar10,(float)iVar10);
        cVar3 = FUN_1401c3c40(&DAT_140a78510);
        if (cVar3 == '\x01') {
          local_98 = 0;
          cVar3 = FUN_1401c3660(&DAT_140a78510,iVar1,param_3 & 0xffffffff,&local_98,0);
          if (cVar3 == '\x01') {
            return 0;
          }
        }
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,0xc,&local_90);
        if (cVar3 == '\0') {
          cVar3 = FUN_1402cc3e0(&DAT_140b27a10,0xc,&local_90);
          if (cVar3 == '\0') {
            local_88[0] = local_90;
            FUN_140030fc0(&DAT_140a60df0,iVar1,local_88,0,(int)sVar12);
            return 0;
          }
          return 0;
        }
        return 0;
      }
    }
    else {
      if (param_2 == 0x231) {
        thunk_FUN_1401772f0();
        return 0;
      }
      if (param_2 == 0x232) {
        thunk_FUN_1401774b0();
        FUN_140304770(&DAT_140b27f60,0);
        uVar6 = FUN_14004fba0();
        FUN_140262af0(uVar6);
        return 0;
      }
    }
switchD_140060df4_caseD_12:
    LVar8 = DefWindowProcA(param_1,param_2,param_3,(LPARAM)param_4);
    return LVar8;
  }
  if (param_2 == 0x111) {
    LVar8 = DefWindowProcA(param_1,0x111,param_3,(LPARAM)param_4);
    return LVar8;
  }
  if (param_2 < 0x10) {
    if (param_2 == 0xf) {
      BeginPaint(param_1,&local_78);
      EndPaint(param_1,&local_78);
      return 0;
    }
    if (param_2 != 2) {
      if (param_2 == 3) {
        return 0;
      }
      if (param_2 == 5) {
        thunk_FUN_140177350(param_3);
        GetWindowPlacement(param_1,(WINDOWPLACEMENT *)&local_78);
        if (DAT_140a61aac != '\0') {
          if (local_78.fErase == 3) {
            DAT_140a7c43c = 1;
          }
          else {
            if (local_78.fErase != 2) {
              DAT_140a7c35c = (int)sVar9;
              DAT_140a7c360 = (int)sVar12;
            }
            DAT_140a7c43c = 0;
          }
          FUN_140203330(&DAT_140a7c230);
        }
        FUN_1400c6b80();
        if (((((DAT_140a619ed != '\0') && (0 < sVar12)) && (0 < sVar9)) &&
            (((int)sVar9 != (uVar11 & 0xffff) &&
             ((int)sVar12 != ((uint)((ulonglong)param_4 >> 0x10) & 0xffff))))) &&
           ((ReleaseCapture(), DAT_140a61aac != '\0' && (local_78.fErase == 3)))) {
          SendMessageA(param_1,0x47,0,0);
        }
        if (DAT_1409cf314 != '\0') {
          uVar6 = FUN_14004fba0();
          FUN_140262af0(uVar6);
          return 0;
        }
        return 0;
      }
      goto switchD_140060df4_caseD_12;
    }
switchD_140060df4_caseD_10:
    ClipCursor((RECT *)0x0);
    PostQuitMessage(0);
    return 0;
  }
  switch(param_2) {
  case 0x10:
    goto switchD_140060df4_caseD_10;
  case 0x11:
    if (param_4 == (int *)0x1) {
      return 1;
    }
    break;
  default:
    goto switchD_140060df4_caseD_12;
  case 0x14:
    return 1;
  case 0x16:
    if (param_4 == (int *)0x1) {
      DAT_140a619ef = 1;
      return 0;
    }
    break;
  case 0x1c:
    if ((param_3 == 0) && (DAT_140b31208 == '\0')) {
      DAT_1409cf314 = 0;
      DAT_140a61b08 = 0;
      if (DAT_140a619ee == '\0') {
        FUN_14021ca20(&DAT_140a7d0a0);
        FUN_1402d7050(&DAT_140b27e60);
        FUN_1402d2830(&DAT_140b27dc0);
        FUN_1402d7050(&DAT_140b2b300);
        FUN_140320780(&DAT_140b2b3a8);
        FUN_140320780(&DAT_140b2b2f0);
        FUN_1402d7050(&DAT_140b2b3b0);
        FUN_14044de50(&DAT_140b306dd,1);
        DAT_140a619ee = '\x01';
      }
    }
    else {
      DAT_1409cf314 = 1;
      if (DAT_140a619ee == '\x01') {
        FUN_14021ca80(&DAT_140a7d0a0);
        FUN_1402d28e0(&DAT_140b27e60);
        FUN_1402d28e0(&DAT_140b27dc0);
        FUN_1402d28e0(&DAT_140b2b300);
        FUN_140320790(&DAT_140b2b3a8);
        FUN_140320790(&DAT_140b2b2f0);
        FUN_1402d28e0(&DAT_140b2b3b0);
        FUN_14044de50(&DAT_140b306dd,0);
        DAT_140a619ee = '\0';
      }
      if ((DAT_140a110d0 != '\0') &&
         ((DAT_140b15418 == 0 || (cVar3 = FUN_14028af60(&DAT_140b153e0), cVar3 == '\0')))) {
        DVar5 = timeGetTime();
        DAT_140a61b08 = DVar5 - _DAT_140a7bec0;
      }
    }
    FUN_140033690(&DAT_140a60df0);
    FUN_140022ee0(&DAT_1409cf080);
    return 0;
  case 0x100:
    goto switchD_140060df4_caseD_100;
  case 0x101:
    lVar7 = FUN_14001fb80();
    if (*(char *)(lVar7 + 1) != '\0') {
      return 1;
    }
    if (DAT_140a110d0 == '\0') {
      return 1;
    }
    cVar3 = FUN_1401c3c40(&DAT_140a78510);
    if (cVar3 == '\x01') {
      return 1;
    }
    cVar3 = FUN_14044b3f0(&DAT_140a28c10,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
    if (cVar3 == '\0') {
      cVar3 = FUN_1402cc080(&DAT_140b27a10,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
      if (cVar3 != '\0') {
        return 0;
      }
      cVar3 = FUN_1402f6bc0(&DAT_140b27f60,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
      if (cVar3 == '\0') {
        FUN_140033580(&DAT_140a60df0,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
        return 0;
      }
      return 0;
    }
    return 0;
  case 0x102:
    if (DAT_140a110d0 != '\0') {
      local_90 = 0;
      FUN_1401c3660(&DAT_140a78510,0xc,param_3 & 0xffffffff,&local_90,0);
      return 0;
    }
    return 1;
  case 0x104:
    break;
  }
  if ((DAT_1409cf314 == '\0') ||
     ((cVar3 = FUN_14020e290(), cVar3 != '\0' && ((param_3 == 0x73 || (param_3 == 9)))))) {
    return 0;
  }
switchD_140060df4_caseD_100:
  if (((uVar11 >> 0x1e & 1) != 0) && (cVar3 = FUN_1401c3c40(&DAT_140a78510), cVar3 == '\0')) {
    return 1;
  }
  if (DAT_140a110d0 == '\0') {
    return 1;
  }
  FUN_140061fe0(param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
  return 1;
}



// ------------------------------------------------------------
// RVA: 0x2b0520  Name: FUN_1402b0520  Size: 1163 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402b0520(longlong param_1,undefined4 param_2,undefined ***param_3,int param_4,int param_5,
                  longlong param_6,char param_7)

{
  undefined ***pppuVar1;
  undefined1 auVar2 [16];
  uint uVar3;
  int iVar4;
  longlong lVar5;
  HANDLE pvVar6;
  longlong lVar7;
  int iVar8;
  int iVar9;
  undefined8 *puVar10;
  undefined8 ******ppppppuVar11;
  ulonglong uVar12;
  undefined8 ******ppppppuVar13;
  LPVOID lpMem;
  longlong lVar14;
  uint uVar15;
  undefined8 ******ppppppuVar16;
  undefined4 *puVar17;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 uVar18;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_res10;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  uint local_74;
  undefined4 local_70;
  undefined8 *****local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  lVar5 = FUN_1402aca60();
  if (lVar5 == 0) {
    return;
  }
  puVar10 = (undefined8 *)(*(longlong *)(lVar5 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(lVar5 + 0x298) + 0x110)) {
    puVar10 = (undefined8 *)*puVar10;
  }
  FUN_140025760("GameObjectManagerClass::Land_Invade -- Invading planet %s\n",puVar10);
  ppppppuVar13 = (undefined8 ******)0x0;
  local_88 = DynamicVectorClass<int>::vftable;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  lVar14 = -1;
  local_74 = 0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 ******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  local_98 = param_2;
  local_94 = param_4;
  if (&local_88 != param_3) {
    uVar15 = *(uint *)((longlong)param_3 + 0x14);
    uVar3 = (int)(uVar15 * 2) >> 1;
    local_74 = uVar15;
    if (uVar3 != 0) {
      if ((int)uVar15 < 0) {
        pvVar6 = GetProcessHeap();
        local_80 = HeapAlloc(pvVar6,0,(longlong)(int)uVar3 * 4);
      }
      else {
        auVar2._8_8_ = 0;
        auVar2._0_8_ = (longlong)(int)uVar3;
        lVar7 = SUB168(ZEXT816(4) * auVar2,0);
        if (SUB168(ZEXT816(4) * auVar2,8) != 0) {
          lVar7 = lVar14;
        }
        local_80 = (LPVOID)thunk_FUN_140769c58(lVar7);
      }
      local_78 = *(uint *)(param_3 + 2);
      local_74 = (uVar15 ^ uVar3) & 0x7fffffff ^ uVar15;
      ppppppuVar11 = ppppppuVar13;
      ppppppuVar16 = ppppppuVar13;
      if (0 < (int)local_78) {
        do {
          uVar15 = (int)ppppppuVar16 + 1;
          *(undefined4 *)((longlong)ppppppuVar11 + (longlong)local_80) =
               *(undefined4 *)((longlong)ppppppuVar11 + (longlong)param_3[1]);
          local_78 = *(uint *)(param_3 + 2);
          ppppppuVar11 = (undefined8 ******)((longlong)ppppppuVar11 + 4);
          ppppppuVar16 = (undefined8 ******)(ulonglong)uVar15;
        } while ((int)uVar15 < (int)local_78);
      }
    }
  }
  local_90 = param_5;
  ppppppuVar11 = ppppppuVar13;
  local_res10 = param_2;
  if (param_6 != 0) {
    do {
      lVar14 = lVar14 + 1;
    } while (*(char *)(param_6 + lVar14) != '\0');
    FUN_140022730(local_68,param_6,lVar14);
    local_res10 = local_98;
    ppppppuVar11 = (undefined8 ******)local_68[0];
  }
  uVar15 = local_78;
  lpMem = local_80;
  iVar9 = local_90;
  if (local_90 == -1) {
    if (*(int *)(*(longlong *)(lVar5 + 0xb8) + 0x130) == 0) {
      lVar14 = FUN_1403956c0(lVar5);
      iVar9 = *(int *)(lVar14 + 0xc);
      local_90 = iVar9;
      if (((iVar9 == -1) ||
          (lVar14 = FUN_140294aa0(&DAT_140a16fd0), iVar9 == *(int *)(lVar14 + 0x4c))) &&
         (lVar14 = FUN_140397120(lVar5), *(int *)(lVar14 + 0x10) != 0)) {
        iVar9 = *(int *)(**(longlong **)(lVar14 + 8) + 0x58);
        local_90 = iVar9;
      }
    }
    else {
      iVar9 = *(int *)(*(longlong *)(lVar5 + 0xb8) + 0x124);
      local_90 = iVar9;
    }
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar18 = extraout_XMM0_Qa;
  if (param_4 == iVar4) {
    lVar14 = FUN_140294a40(&DAT_140a16fd0);
    lVar14 = *(longlong *)(*(longlong *)(lVar14 + 0x68) + 0x640);
    uVar18 = extraout_XMM0_Qa_00;
    if (DAT_140b155d4 == param_4) {
      lVar5 = FUN_140371450(*(undefined8 *)(lVar5 + 0x298),DAT_140b155d8);
      if (lVar5 != 0) {
        FUN_140322780(&DAT_140b2b3a8);
        FUN_1403207c0(&DAT_140b2b2f0);
        FUN_140321830(&DAT_140b2b300,0);
        uVar18 = FUN_140321170(&DAT_140b2b300,lVar5);
        goto LAB_1402b07ed;
      }
      lVar5 = FUN_140294a40(&DAT_140a16fd0);
      lVar14 = *(longlong *)(*(longlong *)(lVar5 + 0x68) + 0x650);
      uVar18 = extraout_XMM0_Qa_01;
    }
    if (lVar14 != 0) {
      uVar18 = FUN_1402d5290(&DAT_140b27e60,lVar14,0,0,0);
    }
  }
LAB_1402b07ed:
  if (local_94 != iVar9) {
    if (param_7 == '\0') {
      iVar4 = *(int *)(param_1 + 0x490);
      iVar8 = *(int *)(param_1 + 0x494) * 2 >> 1;
      if (iVar8 <= iVar4) {
        iVar4 = iVar8 / 10;
        if (iVar8 / 10 < 10) {
          iVar4 = 10;
        }
        uVar18 = FUN_1402b6fa0(param_1 + 0x480,iVar4 + iVar8,0,1);
        iVar4 = *(int *)(param_1 + 0x490);
      }
      puVar17 = (undefined4 *)((longlong)iVar4 * 0x58 + *(longlong *)(param_1 + 0x488));
      *(int *)(param_1 + 0x490) = iVar4 + 1;
      *puVar17 = local_res10;
      pppuVar1 = (undefined ***)(puVar17 + 4);
      puVar17[1] = local_94;
      puVar17[2] = iVar9;
      if (pppuVar1 != &local_88) {
        uVar18 = FUN_14001f910(pppuVar1);
        puVar17[9] = local_74;
        iVar9 = (int)(local_74 * 2) >> 1;
        if (iVar9 != 0) {
          uVar18 = FUN_140021ec0(pppuVar1,iVar9,0,1);
          if (0 < (int)uVar15) {
            uVar12 = (ulonglong)uVar15;
            do {
              *(undefined4 *)((longlong)ppppppuVar13 + *(longlong *)(puVar17 + 6)) =
                   *(undefined4 *)((longlong)ppppppuVar13 + (longlong)lpMem);
              uVar12 = uVar12 - 1;
              ppppppuVar13 = (undefined8 ******)((longlong)ppppppuVar13 + 4);
            } while (uVar12 != 0);
          }
          puVar17[8] = uVar15;
        }
      }
      puVar17[10] = local_70;
      if ((undefined8 ******)(puVar17 + 0xc) != local_68) {
        ppppppuVar13 = local_68;
        if (0xf < uStack_50) {
          ppppppuVar13 = ppppppuVar11;
        }
        uVar18 = FUN_140022730(puVar17 + 0xc,ppppppuVar13,local_58);
        lpMem = local_80;
        ppppppuVar11 = (undefined8 ******)local_68[0];
      }
      *(undefined1 *)(puVar17 + 0x14) = local_48;
    }
    else {
      uVar18 = FUN_14029bb40(param_1 + 0x480,&local_98);
      lpMem = local_80;
      ppppppuVar11 = (undefined8 ******)local_68[0];
    }
  }
  if (0xf < uStack_50) {
    ppppppuVar13 = ppppppuVar11;
    if (0xfff < uStack_50 + 1) {
      ppppppuVar13 = (undefined8 ******)ppppppuVar11[-1];
      if (0x1f < (ulonglong)((longlong)ppppppuVar11 + (-8 - (longlong)ppppppuVar13))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar18,uStack_50 + 0x28);
      }
    }
    thunk_FUN_1407864b8(ppppppuVar13);
  }
  if (lpMem != (LPVOID)0x0) {
    if ((local_74 & 0x80000000) == 0) {
      thunk_FUN_1407864b8(lpMem);
    }
    else {
      pvVar6 = GetProcessHeap();
      HeapFree(pvVar6,0,lpMem);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x7ef6a0  Name: FUN_1407ef6a0  Size: 35 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1407ef6a0(void)

{
  _DAT_140b2b3a8 = 0;
  DAT_140b2b398 = 0;
  DAT_140b2b3a0 = 0;
  DAT_140a17250 = 0xffffffff;
  return;
}



// --- xrefs to DAT_0xb2b2f0 ---
//   DATA from FUN_0x28f320 @ site 0x28f34c
//   DATA from FUN_0x5f6b0 @ site 0x5fa7b
//   DATA from FUN_0x2af7c0 @ site 0x2afb15
//   DATA from FUN_0x28e890 @ site 0x28e99a
//   DATA from FUN_0x28d940 @ site 0x28d97c
//   DATA from FUN_0x5d990 @ site 0x5ea70
//   DATA from FUN_0x4da50 @ site 0x4db81
//   DATA from FUN_0x4b250 @ site 0x4c5ad
//   DATA from FUN_0x1098e0 @ site 0x1098f5
//   DATA from FUN_0x1089e0 @ site 0x108c1f
//   DATA from FUN_0x107900 @ site 0x107ad2
//   DATA from FUN_0x105d30 @ site 0x105d45
//   DATA from FUN_0x105190 @ site 0x105201
//   DATA from FUN_0x1053c0 @ site 0x1055b8
//   DATA from FUN_0x22f10 @ site 0x231f0
//   DATA from FUN_0x60c60 @ site 0x60f00
//   DATA from FUN_0x60c60 @ site 0x60f7e
//   DATA from FUN_0x2b0520 @ site 0x2b0790
//   WRITE from FUN_0x7ef580 @ site 0x7ef582
// ------------------------------------------------------------
// RVA: 0x28f320  Name: FUN_14028f320  Size: 499 bytes
// ------------------------------------------------------------

void FUN_14028f320(longlong param_1,char param_2)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong *plVar5;
  int iVar6;
  undefined **local_res8;
  
  if (*(longlong *)(param_1 + 0x38) != 0) {
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_1402d6f20(&DAT_140b27e60);
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140323660(&DAT_140b2b3b0);
    if (DAT_140a10b50 == 0) {
      lVar4 = FUN_140769c58(0x78);
      if (lVar4 == 0) {
        DAT_140a10b50 = 0;
      }
      else {
        DAT_140a10b50 = FUN_140221820(lVar4);
      }
      local_res8 = &PTR_vftable_140a10b48;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_140a10b48;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140220ed0(DAT_140a10b50,param_1,0x24,*(undefined8 *)(param_1 + 0x38));
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x28))();
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x10))();
    plVar5 = *(longlong **)(param_1 + 0x48);
    if (plVar5 != (longlong *)0x0) {
      iVar1 = *(int *)(param_1 + 0x50);
      iVar6 = 0;
      if (0 < iVar1) {
        do {
          if (*plVar5 == *(longlong *)(param_1 + 0x38)) {
            if ((-1 < iVar6) && (iVar6 < iVar1)) {
              FUN_14025c3c0(param_1 + 0x40,iVar6,iVar6 + 1,(iVar1 - iVar6) + -1);
              *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + -1;
            }
            break;
          }
          iVar6 = iVar6 + 1;
          plVar5 = plVar5 + 1;
        } while (iVar6 < iVar1);
      }
    }
    puVar2 = *(undefined8 **)(param_1 + 0x38);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)*puVar2)(puVar2,1);
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined1 *)(param_1 + 0x1e5) = 0;
    FUN_1403027d0(&DAT_140b27f60);
    if (*(int *)(param_1 + 0x50) != 0) {
      plVar5 = (longlong *)**(undefined8 **)(param_1 + 0x48);
      *(longlong **)(param_1 + 0x38) = plVar5;
      (**(code **)(*plVar5 + 0x58))();
      (**(code **)(**(longlong **)(param_1 + 0x38) + 0x30))();
      uVar3 = (**(code **)(**(longlong **)(param_1 + 0x38) + 0xe0))();
      *(undefined4 *)(param_1 + 0x1c) = uVar3;
      FUN_14027bf80(&DAT_140b0a320);
      return;
    }
    FUN_14047f690();
    FUN_1402566d0();
    DAT_140b0a354 = 1;
    if (param_2 != '\0') {
      FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,0);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x5f6b0  Name: FUN_14005f6b0  Size: 1779 bytes
// ------------------------------------------------------------

void FUN_14005f6b0(char param_1)

{
  char cVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  int iVar10;
  ulonglong uVar11;
  longlong *plVar12;
  undefined8 *puVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  uint uVar16;
  bool bVar17;
  longlong local_c8 [2];
  longlong local_b8;
  ulonglong local_b0;
  ulonglong local_a8 [2];
  undefined8 local_98;
  ulonglong local_90;
  ulonglong local_88 [2];
  undefined8 local_78;
  ulonglong local_70;
  ulonglong local_68 [2];
  undefined8 local_58;
  ulonglong local_50;
  longlong local_48 [2];
  undefined8 local_38;
  ulonglong local_30;
  
  cVar1 = FUN_14028af60(&DAT_140b153e0);
  if (cVar1 == '\0') {
    uVar3 = FUN_140294a70(&DAT_140a16fd0);
    iVar10 = DAT_140a2ab00._4_4_ * 2 >> 1;
    if (iVar10 <= (int)DAT_140a2ab00) {
      iVar4 = iVar10 / 10;
      if (iVar10 / 10 < 10) {
        iVar4 = 10;
      }
      FUN_140021ec0(&PTR_vftable_140a2aaf0,iVar4 + iVar10,0,1);
    }
    *(undefined4 *)(DAT_140a2aaf8 + (longlong)(int)DAT_140a2ab00 * 4) = uVar3;
    DAT_140a2ab00._0_4_ = (int)DAT_140a2ab00 + 1;
  }
  cVar1 = DAT_140b155c4;
  uVar11 = 0;
  uVar15 = uVar11;
  if (0 < (int)DAT_140a2ab00) {
    do {
      iVar10 = *(int *)(uVar15 + DAT_140a2aaf8);
      iVar4 = FUN_140294a70(&DAT_140a16fd0);
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,iVar10);
      local_c8[0] = 0;
      local_b8 = 0;
      local_b0 = 7;
      if (lVar6 != 0) {
        cVar2 = FUN_1402940c0(&DAT_140a16fd0,lVar6,1);
        if (cVar2 != '\0') {
          FUN_14005fe90(lVar6);
        }
        lVar7 = FUN_140294a40(&DAT_140a16fd0);
        if (lVar6 != lVar7) {
          plVar12 = (longlong *)(lVar6 + 0x28);
          if (local_c8 != plVar12) {
            if (7 < *(ulonglong *)(lVar6 + 0x40)) {
              plVar12 = (longlong *)*plVar12;
            }
            FUN_1400228a0(local_c8,plVar12,*(undefined8 *)(lVar6 + 0x38));
          }
          FUN_14002d480(local_c8,&DAT_1408005e4,1);
          puVar8 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,"TEXT_PLAYER_LEFT_GAME");
          puVar13 = puVar8;
          if (7 < (ulonglong)puVar8[3]) {
            puVar13 = (undefined8 *)*puVar8;
          }
          FUN_14002d480(local_c8,puVar13,puVar8[2]);
          uVar9 = FUN_140281940(lVar6);
          FUN_1401f9320(local_a8,uVar9);
          FUN_140049810(&DAT_1409cf320,local_c8,local_a8,0,0,0);
          FUN_1401f9440(local_a8);
        }
      }
      iVar5 = FUN_140294af0(&DAT_140a16fd0);
      bVar17 = iVar10 == iVar4 || iVar5 < 2;
      if ((DAT_140b15418 != (longlong *)0x0) &&
         (iVar10 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar10 == 0)) {
        cVar2 = FUN_14028af60(&DAT_140b153e0);
        if ((cVar2 != '\0') && (cVar1 == '\0')) {
          FUN_140092bb0(&DAT_140a10d88,1);
        }
        bVar17 = true;
      }
      cVar2 = FUN_14028b060(&DAT_140b153e0);
      if (((cVar2 != '\0') ||
          ((cVar2 = FUN_14028afb0(&DAT_140b153e0), cVar2 != '\0' &&
           (cVar2 = FUN_14028af60(&DAT_140b153e0), cVar2 != '\0')))) &&
         ((bVar17 || (iVar10 = FUN_140294af0(&DAT_140a16fd0), iVar10 < 2)))) {
        if (cVar1 == '\0') {
          FUN_140087770(DAT_140b15418,1);
        }
LAB_14005f9b8:
        iVar10 = DAT_140b153f8;
        if (DAT_140a61ab2 != '\0') {
          FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,&DAT_140a10be0);
          DAT_140a61ab2 = '\0';
        }
        FUN_1400a3c40();
        thunk_FUN_14003f220(&DAT_140a60df0);
        FUN_1400337b0(&DAT_140a60df0);
        FUN_14028d940(&DAT_140b153e0);
        FUN_140297630(&DAT_140a16fd0,0);
        DAT_140b155e8 = 0;
        FUN_1400338e0(&DAT_140a60df0);
        FUN_140049350(&DAT_1409cf320);
        FUN_140049350(&DAT_1409cf390);
        FUN_1403027d0(&DAT_140b27f60);
        DAT_140a61b0d = 0;
        FUN_140069890();
        uVar9 = FUN_14001fb80();
        FUN_14007e040(uVar9,"players 0");
        FUN_140322780(&DAT_140b2b3a8);
        FUN_1403207c0(&DAT_140b2b2f0);
        FUN_140321830(&DAT_140b2b300,1);
        FUN_1402d2790(&DAT_140b27dc0);
        FUN_1402d6f20(&DAT_140b27e60);
        FUN_140323660(&DAT_140b2b3b0);
        if (((DAT_140a61c80 == '\0') && (DAT_140a61e08 == '\0')) && (DAT_140a61fb8 == '\0')) {
          if (DAT_140a61f20 != '\0') {
            FUN_1400b1c00();
          }
          FUN_1401c1e00(&DAT_140a78510);
          if ((iVar10 == 1) && (cVar1 == '\0')) {
            FUN_1400b1240();
            if (local_b8 != 0) {
              local_48[0] = 0;
              local_38 = 0;
              local_30 = 7;
              FUN_1400228a0(local_48,&PTR_1407ff7e8,0);
              local_68[0] = 0;
              local_58 = 0;
              local_50 = 7;
              FUN_1400228a0(local_68,&PTR_1407ff7e8,0);
              local_88[0] = 0;
              local_78 = 0;
              local_70 = 7;
              FUN_1400228a0(local_88,L"TEXT_BUTTON_OK",0xe);
              local_a8[0] = 0;
              local_98 = 0;
              local_90 = 7;
              FUN_1400228a0(local_a8,&PTR_1407ff7e8,0);
              FUN_140099d90(&DAT_140065c90,local_a8,local_88,local_68,local_48,local_c8,0,0,0,1);
              if (7 < local_90) {
                uVar15 = local_90 * 2 + 2;
                uVar11 = local_a8[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_a8[0] - 8);
                  uVar15 = local_90 * 2 + 0x29;
                  if (0x1f < (local_a8[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_98 = 0;
              local_90 = 7;
              local_a8[0] = local_a8[0] & 0xffffffffffff0000;
              if (7 < local_70) {
                uVar15 = local_70 * 2 + 2;
                uVar11 = local_88[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_88[0] - 8);
                  uVar15 = local_70 * 2 + 0x29;
                  if (0x1f < (local_88[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_78 = 0;
              local_70 = 7;
              local_88[0] = local_88[0] & 0xffffffffffff0000;
              if (7 < local_50) {
                uVar15 = local_50 * 2 + 2;
                uVar11 = local_68[0];
                if (0xfff < uVar15) {
                  uVar11 = *(ulonglong *)(local_68[0] - 8);
                  uVar15 = local_50 * 2 + 0x29;
                  if (0x1f < (local_68[0] - uVar11) - 8) goto LAB_14005fd9f;
                }
                thunk_FUN_1407864b8(uVar11,uVar15);
              }
              local_58 = 0;
              local_50 = 7;
              local_68[0] = local_68[0] & 0xffffffffffff0000;
              if (7 < local_30) {
                if (0xfff < local_30 * 2 + 2) {
                  uVar11 = *(ulonglong *)(local_48[0] + -8);
                  uVar15 = local_30 * 2 + 0x29;
                  if (0x1f < (local_48[0] - uVar11) - 8) {
LAB_14005fd9f:
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(uVar11,uVar15);
                  }
                }
                thunk_FUN_1407864b8();
              }
            }
          }
          else if ((DAT_140b303d0 == 0) && (param_1 == '\x01')) {
            FUN_1400c58e0();
          }
        }
        if (local_b0 < 8) {
          DAT_140a2ab00._0_4_ = 0;
          return;
        }
        uVar14 = local_b0 * 2 + 2;
        lVar6 = local_c8[0];
        if (0xfff < uVar14) {
          lVar6 = *(longlong *)(local_c8[0] + -8);
          uVar14 = local_b0 * 2 + 0x29;
          if (0x1f < (local_c8[0] - lVar6) - 8U) {
LAB_14005fd99:
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(lVar6,uVar14);
          }
        }
        thunk_FUN_1407864b8(lVar6,uVar14);
        DAT_140a2ab00._0_4_ = 0;
        return;
      }
      if (bVar17) goto LAB_14005f9b8;
      if (7 < local_b0) {
        if (0xfff < local_b0 * 2 + 2) {
          lVar6 = *(longlong *)(local_c8[0] + -8);
          uVar14 = local_b0 * 2 + 0x29;
          if (0x1f < (local_c8[0] - lVar6) - 8U) goto LAB_14005fd99;
        }
        thunk_FUN_1407864b8();
      }
      uVar16 = (int)uVar11 + 1;
      uVar11 = (ulonglong)uVar16;
      uVar15 = uVar15 + 4;
    } while ((int)uVar16 < (int)DAT_140a2ab00);
  }
  DAT_140a2ab00._0_4_ = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x2af7c0  Name: FUN_1402af7c0  Size: 1096 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402af7c0(longlong param_1,longlong param_2,int param_3,int param_4,longlong param_5,
                  char param_6)

{
  undefined ***pppuVar1;
  undefined4 uVar2;
  char cVar3;
  int iVar4;
  HANDLE hHeap;
  undefined8 *puVar5;
  ulonglong uVar6;
  undefined8 *******pppppppuVar7;
  LPVOID lpMem;
  undefined8 *******pppppppuVar8;
  longlong lVar9;
  longlong lVar10;
  undefined4 *puVar11;
  int iVar12;
  undefined8 uVar13;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  int local_74;
  undefined4 local_70;
  undefined8 *******local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  if (param_2 == 0) {
    return;
  }
  puVar5 = (undefined8 *)(*(longlong *)(param_2 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(param_2 + 0x298) + 0x110)) {
    puVar5 = (undefined8 *)*puVar5;
  }
  FUN_140025760("GameObjectManagerClass::Initiate_Space_Conflict -- Planet %s\n",puVar5);
  local_98 = *(undefined4 *)(param_2 + 0x50);
  local_88 = DynamicVectorClass<int>::vftable;
  local_90 = -1;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  local_74 = -0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 *******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  uVar13 = _DAT_140802a40;
  local_94 = param_3;
  if (param_5 != 0) {
    lVar9 = -1;
    do {
      lVar9 = lVar9 + 1;
    } while (*(char *)(param_5 + lVar9) != '\0');
    uVar13 = FUN_140022730(local_68);
  }
  pppppppuVar8 = local_68[0];
  lpMem = local_80;
  uVar2 = local_98;
  lVar9 = *(longlong *)(param_2 + 0xb8);
  if (param_4 == -1) {
    param_4 = -1;
  }
  if ((lVar9 == 0) || (param_4 != -1)) {
LAB_1402af91d:
    if (param_4 == -1) goto LAB_1402af922;
  }
  else {
    iVar12 = 0;
    if (0 < *(int *)(lVar9 + 0xd0)) {
      lVar10 = 0;
      do {
        lVar9 = *(longlong *)(lVar10 + *(longlong *)(lVar9 + 200));
        if (((lVar9 != 0) &&
            (cVar3 = FUN_1403a9760(lVar9), uVar13 = extraout_XMM0_Qa, cVar3 != '\x01')) &&
           (param_4 = *(int *)(lVar9 + 0x58), param_4 != param_3)) goto LAB_1402af91d;
        lVar9 = *(longlong *)(param_2 + 0xb8);
        iVar12 = iVar12 + 1;
        lVar10 = lVar10 + 8;
      } while (iVar12 < *(int *)(lVar9 + 0xd0));
    }
LAB_1402af922:
    if (((*(int *)(*(longlong *)(param_2 + 0xb8) + 0x148) == 0) ||
        (param_4 = *(int *)(*(longlong *)(param_2 + 0xb8) + 0x13c), param_4 == param_3)) ||
       (param_4 == -1)) {
      iVar12 = local_74;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
        if (0xfff < uStack_50 + 1) {
          pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
          uVar6 = uStack_50 + 0x28;
          if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7)))
          goto LAB_1402afc03;
        }
        thunk_FUN_1407864b8(pppppppuVar7);
        iVar12 = local_74;
      }
      goto joined_r0x0001402afbaf;
    }
  }
  local_90 = param_4;
  if (param_6 == '\0') {
    iVar12 = *(int *)(param_1 + 0x478);
    iVar4 = *(int *)(param_1 + 0x47c) * 2 >> 1;
    if (iVar4 <= iVar12) {
      iVar12 = iVar4 / 10;
      if (iVar4 / 10 < 10) {
        iVar12 = 10;
      }
      FUN_1402b6fa0(param_1 + 0x468,iVar12 + iVar4,0,1);
      iVar12 = *(int *)(param_1 + 0x478);
    }
    puVar11 = (undefined4 *)((longlong)iVar12 * 0x58 + *(longlong *)(param_1 + 0x470));
    *(int *)(param_1 + 0x478) = iVar12 + 1;
    *puVar11 = uVar2;
    puVar11[1] = local_94;
    puVar11[2] = param_4;
    pppuVar1 = (undefined ***)(puVar11 + 4);
    iVar12 = local_74;
    if (pppuVar1 != &local_88) {
      FUN_14001f910(pppuVar1);
      iVar12 = local_74;
      puVar11[9] = local_74;
      iVar4 = local_74 * 2 >> 1;
      if (iVar4 != 0) {
        FUN_140021ec0(pppuVar1,iVar4,0,1);
        if (0 < (int)local_78) {
          uVar6 = (ulonglong)local_78;
          lVar9 = 0;
          do {
            *(undefined4 *)(lVar9 + *(longlong *)(puVar11 + 6)) =
                 *(undefined4 *)((longlong)lpMem + lVar9);
            uVar6 = uVar6 - 1;
            lVar9 = lVar9 + 4;
          } while (uVar6 != 0);
        }
        puVar11[8] = local_78;
      }
    }
    puVar11[10] = local_70;
    if ((undefined8 ********)(puVar11 + 0xc) != local_68) {
      pppppppuVar7 = local_68;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
      }
      FUN_140022730(puVar11 + 0xc,pppppppuVar7,local_58);
      lpMem = local_80;
      pppppppuVar8 = local_68[0];
      iVar12 = local_74;
    }
    *(undefined1 *)(puVar11 + 0x14) = local_48;
  }
  else {
    FUN_14029bb40(param_1 + 0x468,&local_98);
    lpMem = local_80;
    pppppppuVar8 = local_68[0];
    iVar12 = local_74;
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar13 = extraout_XMM0_Qa_00;
  if ((param_3 == iVar4) && (DAT_140b155d4 == param_3)) {
    lVar9 = FUN_140371450(*(undefined8 *)(param_2 + 0x298),DAT_140b155d8);
    if (lVar9 == 0) {
      lVar9 = FUN_140294a40(&DAT_140a16fd0);
      lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x68) + 0x650);
      uVar13 = extraout_XMM0_Qa_01;
      if (lVar9 != 0) {
        uVar13 = FUN_1402d5290(&DAT_140b27e60,lVar9,0,0,0);
      }
    }
    else {
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,0);
      uVar13 = FUN_140321170(&DAT_140b2b300,lVar9);
    }
  }
  if (0xf < uStack_50) {
    uVar6 = uStack_50 + 1;
    pppppppuVar7 = pppppppuVar8;
    if (0xfff < uVar6) {
      pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
      uVar6 = uStack_50 + 0x28;
      if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7))) {
LAB_1402afc03:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar13,uVar6);
      }
    }
    thunk_FUN_1407864b8(pppppppuVar7,uVar6);
  }
joined_r0x0001402afbaf:
  if (lpMem != (LPVOID)0x0) {
    if (iVar12 < 0) {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,lpMem);
    }
    else {
      thunk_FUN_1407864b8(lpMem);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x28e890  Name: FUN_14028e890  Size: 1189 bytes
// ------------------------------------------------------------

void FUN_14028e890(longlong param_1,int param_2,longlong *param_3)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  DWORD DVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plVar8;
  int iVar9;
  longlong *plVar10;
  undefined **local_res8;
  longlong local_48 [3];
  ulonglong local_30;
  
  plVar8 = (longlong *)0x0;
  *(undefined1 *)(param_1 + 0xb1) = 1;
  DAT_140a2ab00._0_4_ = 0;
  *(undefined8 *)(param_1 + 0x1d0) = 0;
  *(undefined1 *)(param_1 + 0x1d8) = 0;
  *(undefined4 *)(param_1 + 0x1dc) = 0;
  *(undefined2 *)(param_1 + 0x1e1) = 0;
  *(undefined1 *)(param_1 + 0x1e0) = 0;
  *(undefined1 *)(param_1 + 0x1e4) = 0;
  uVar5 = FUN_14028a6f0();
  FUN_1404e67d0(uVar5);
  if (param_3 == (longlong *)0x0) {
    plVar10 = (longlong *)&DAT_14085af28;
  }
  else {
    DAT_140b313e8 = 0;
    plVar10 = param_3;
    if (0xf < (ulonglong)param_3[3]) {
      plVar10 = (longlong *)*param_3;
    }
  }
  puVar6 = (undefined8 *)
           (**(code **)(*DAT_140b310b0 + 0x10))(DAT_140b310b0,local_48,(longlong)param_2);
  if (0xf < (ulonglong)puVar6[3]) {
    puVar6 = (undefined8 *)*puVar6;
  }
  FUN_140025760("GameModeManagerClass::Start_Game -- Starting %s Mode: %s.\n",puVar6,plVar10);
  if (0xf < local_30) {
    if (0xfff < local_30 + 1) {
      if (0x1f < (local_48[0] - *(longlong *)(local_48[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_48[0] + -8),local_30 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  if (*(int *)(param_1 + 500) == -1) {
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_140321830(&DAT_140b2b300,0);
  }
  FUN_140323660(&DAT_140b2b3b0);
  FUN_1403567c0(&DAT_140b2bcb0);
  bVar3 = false;
  if (*(int *)(param_1 + 0x50) == 0) {
    bVar3 = true;
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_14027d020(&DAT_140b0a320);
    DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe0);
    if (*(char *)(param_1 + 0xe8) != '\0') {
      DAT_140a13e24 = *(undefined4 *)(param_1 + 0xe4);
    }
    *(undefined4 *)(param_1 + 0x2c) = 0;
    uVar5 = FUN_14028a5d0();
    FUN_1404d9d40(uVar5);
    FUN_140293390(&DAT_140a16fd0,0);
    FUN_1404907c0(param_1 + 0x180,"PENDING_TACTICAL_BATTLE_VOTE");
    FUN_1404908b0(param_1 + 0x180);
  }
  else {
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x50))();
  }
  if (param_2 == 0) {
    lVar7 = FUN_140769c58(0x378);
    if (lVar7 == 0) goto LAB_14028ec28;
    plVar8 = (longlong *)FUN_1404b1270(lVar7);
  }
  else {
    if (param_2 == 1) {
      lVar7 = FUN_140769c58(0x430);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1403b5210(lVar7);
      }
    }
    else {
      if (param_2 != 2) goto LAB_14028ec28;
      lVar7 = FUN_140769c58(0x3c8);
      if (lVar7 != 0) {
        plVar8 = (longlong *)FUN_1404d6ba0(lVar7);
      }
    }
    *(undefined4 *)(param_1 + 0x1ec) = 0;
    DVar4 = timeGetTime();
    *(DWORD *)(param_1 + 0x1e8) = DVar4;
  }
  if (plVar8 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x50);
    iVar9 = *(int *)(param_1 + 0x54) * 2 >> 1;
    if (iVar9 <= iVar1) {
      iVar2 = iVar9 / 10;
      if (iVar9 / 10 < 10) {
        iVar2 = 10;
      }
      FUN_140047aa0(param_1 + 0x40,iVar2 + iVar9,0,CONCAT71((uint7)(uint3)((uint)iVar1 >> 8),1));
      iVar1 = *(int *)(param_1 + 0x50);
    }
    if (0 < iVar1) {
      FUN_14025c3c0(param_1 + 0x40,1);
    }
    **(undefined8 **)(param_1 + 0x48) = plVar8;
    *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + 1;
    (**(code **)(*plVar8 + 8))(plVar8,*(undefined4 *)(param_1 + 0x18),param_2);
    *(longlong **)(param_1 + 0x38) = plVar8;
    (**(code **)(*plVar8 + 0x18))(plVar8);
    (**(code **)(*plVar8 + 0x30))(plVar8);
    if (DAT_1409cf1c0 == 0) {
      lVar7 = FUN_140769c58(0x10c8);
      if (lVar7 == 0) {
        DAT_1409cf1c0 = 0;
      }
      else {
        DAT_1409cf1c0 = FUN_140262010(lVar7);
      }
      local_res8 = &PTR_vftable_1409cf1b8;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_1409cf1b8;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140262af0(DAT_1409cf1c0);
    *(int *)(param_1 + 0x1c) = param_2;
    if ((param_3 != (longlong *)0x0) && (param_2 == 0)) {
      *(undefined *)(plVar8 + 0x68) = *(undefined *)((longlong)param_3 + 0xcb);
      if (plVar8 + 0x69 != param_3) {
        plVar10 = param_3;
        if (0xf < (ulonglong)param_3[3]) {
          plVar10 = (longlong *)*param_3;
        }
        FUN_140022730(plVar8 + 0x69,plVar10,param_3[2]);
      }
    }
  }
LAB_14028ec28:
  if ((bVar3) && (param_2 != 0)) {
    FUN_140292050(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140291f70(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140292590(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3,param_1 + 0xbc);
    FUN_140291d80(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0x18),param_3);
    FUN_1402b09b0(*(undefined8 *)(DAT_140b15418 + 0x18),param_3);
    iVar1 = *(int *)(param_1 + 0x18);
    if (((iVar1 - 1U & 0xfffffffc) != 0) || (iVar1 == 3)) {
      FUN_1402924d0(&DAT_140a16fd0,iVar1,param_3,0);
    }
  }
  if (((*(int *)(param_1 + 0x18) - 1U & 0xfffffffc) != 0) || (*(int *)(param_1 + 0x18) == 3)) {
    FUN_14027bf00(&DAT_140b0a320,DAT_140a7c398,DAT_140a7c39c);
  }
  FUN_14027bf80(&DAT_140b0a320);
  DAT_140b0a354 = 0;
  *(undefined1 *)(param_1 + 0xb1) = 0;
  FUN_14028c640(param_1 + 0xf0);
  return;
}



// ------------------------------------------------------------
// RVA: 0x28d940  Name: FUN_14028d940  Size: 667 bytes
// ------------------------------------------------------------

void FUN_14028d940(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong *plVar5;
  int iVar6;
  undefined **local_res8;
  
  lVar4 = *(longlong *)(param_1 + 0x38);
  do {
    if (lVar4 == 0) {
      FUN_14027d570(&DAT_140b0a320);
      FUN_1404b3b10(&DAT_140b2ed18);
      FUN_1402c1e30(&DAT_140b15690);
      *(undefined4 *)(param_1 + 0x1c) = 0xffffffff;
      *(undefined8 *)(param_1 + 0x38) = 0;
      *(undefined8 *)(param_1 + 0x24) = 0;
      *(undefined1 *)(param_1 + 0xb1) = 0;
      *(undefined4 *)(param_1 + 0x80) = 1;
      *(undefined4 *)(param_1 + 0x20) = 0;
      if (*(longlong *)(param_1 + 0x88) != 0) {
        FUN_140260f10(&DAT_140b0a130);
        *(undefined8 *)(param_1 + 0x88) = 0;
      }
      if (*(longlong *)(param_1 + 0x90) != 0) {
        FUN_140260f10(&DAT_140b0a130);
        *(undefined8 *)(param_1 + 0x90) = 0;
      }
      FUN_1404080d0(&DAT_140b2f020);
      FUN_1404dc640();
      *(undefined8 *)(param_1 + 500) = 0xffffffffffffffff;
      *(undefined4 *)(param_1 + 0x1fc) = 0;
      return;
    }
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_1402d6f20(&DAT_140b27e60);
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140323660(&DAT_140b2b3b0);
    if (DAT_140a10b50 == 0) {
      lVar4 = FUN_140769c58(0x78);
      if (lVar4 == 0) {
        DAT_140a10b50 = 0;
      }
      else {
        DAT_140a10b50 = FUN_140221820(lVar4);
      }
      local_res8 = &PTR_vftable_140a10b48;
      if (DAT_140a7bc10 == DAT_140a7bc18) {
        FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,&local_res8);
      }
      else {
        *DAT_140a7bc10 = &PTR_vftable_140a10b48;
        DAT_140a7bc10 = DAT_140a7bc10 + 1;
      }
    }
    FUN_140220ed0(DAT_140a10b50,param_1,0x24,*(undefined8 *)(param_1 + 0x38));
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x28))();
    (**(code **)(**(longlong **)(param_1 + 0x38) + 0x10))();
    plVar5 = *(longlong **)(param_1 + 0x48);
    if ((plVar5 != (longlong *)0x0) && (iVar1 = *(int *)(param_1 + 0x50), 0 < iVar1)) {
      iVar6 = 0;
      do {
        if (*plVar5 == *(longlong *)(param_1 + 0x38)) {
          if ((-1 < iVar6) && (iVar6 < iVar1)) {
            FUN_14025c3c0(param_1 + 0x40,iVar6,iVar6 + 1,(iVar1 - iVar6) + -1);
            *(int *)(param_1 + 0x50) = *(int *)(param_1 + 0x50) + -1;
          }
          break;
        }
        iVar6 = iVar6 + 1;
        plVar5 = plVar5 + 1;
      } while (iVar6 < iVar1);
    }
    puVar2 = *(undefined8 **)(param_1 + 0x38);
    if (puVar2 != (undefined8 *)0x0) {
      (**(code **)*puVar2)(puVar2,1);
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined1 *)(param_1 + 0x1e5) = 0;
    FUN_1403027d0(&DAT_140b27f60);
    if (*(int *)(param_1 + 0x50) == 0) {
      FUN_14047f690();
      FUN_1402566d0();
      DAT_140b0a354 = 1;
      FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320,0);
    }
    else {
      plVar5 = (longlong *)**(undefined8 **)(param_1 + 0x48);
      *(longlong **)(param_1 + 0x38) = plVar5;
      (**(code **)(*plVar5 + 0x58))();
      (**(code **)(**(longlong **)(param_1 + 0x38) + 0x30))();
      uVar3 = (**(code **)(**(longlong **)(param_1 + 0x38) + 0xe0))();
      *(undefined4 *)(param_1 + 0x1c) = uVar3;
      FUN_14027bf80(&DAT_140b0a320);
    }
    lVar4 = *(longlong *)(param_1 + 0x38);
  } while( true );
}



// ------------------------------------------------------------
// RVA: 0x5d990  Name: FUN_14005d990  Size: 7440 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x00014005f137) */
/* WARNING: Removing unreachable block (ram,0x00014005f161) */
/* WARNING: Removing unreachable block (ram,0x00014005f166) */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14005d990(HINSTANCE param_1,undefined8 param_2,undefined8 param_3,int param_4)

{
  byte *pbVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  uint uVar5;
  uint uVar6;
  longlong *plVar7;
  char cVar8;
  char cVar9;
  ATOM AVar10;
  DWORD DVar11;
  BOOL BVar12;
  undefined4 uVar13;
  LONG X;
  int iVar14;
  int iVar15;
  undefined8 uVar16;
  longlong lVar17;
  undefined8 *puVar18;
  ulonglong uVar19;
  longlong lVar20;
  HWND hWnd;
  undefined8 uVar21;
  FILE *_File;
  char *pcVar22;
  HANDLE hProcess;
  LONG LVar23;
  undefined1 *puVar24;
  ulonglong uVar25;
  undefined8 *puVar26;
  LONG LVar27;
  HINSTANCE pHVar28;
  ulonglong uVar29;
  ulonglong uVar30;
  LONG Y;
  uint uVar31;
  float fVar32;
  ulonglong in_stack_fffffffffffff9f8;
  undefined8 in_stack_fffffffffffffa00;
  undefined4 uVar33;
  tagRECT local_5b8;
  undefined1 local_5a8 [16];
  ulonglong local_598;
  undefined4 local_590;
  char local_58c;
  undefined1 local_58b;
  undefined8 local_588;
  ulonglong local_580;
  DWORD local_578;
  undefined8 local_574;
  ulonglong local_568 [2];
  undefined8 local_558;
  ulonglong local_550;
  ulonglong local_548 [2];
  undefined8 local_538;
  ulonglong local_530;
  longlong local_528;
  undefined **local_520;
  WNDCLASSEXA local_518;
  undefined8 local_4c8;
  undefined4 local_4c0;
  tagMSG local_4b8;
  char local_488 [271];
  undefined1 uStack_379;
  byte local_378 [512];
  byte local_178 [320];
  
  uVar33 = (undefined4)((ulonglong)in_stack_fffffffffffffa00 >> 0x20);
  puVar18 = (undefined8 *)0x0;
  local_4b8.wParam = 0;
  DAT_140a619d8 = param_1;
  strncpy((char *)&local_518,"952F41D039DB4507A84B40488E76722E 0123456789ABCDEIPA",0x33);
  FUN_14021db70();
  cVar8 = FUN_14007f880();
  if (cVar8 == '\0') {
    return 0;
  }
  uVar16 = FUN_140067c80();
  cVar8 = FUN_14006a050(uVar16);
  if (cVar8 == '\0') {
    return 0;
  }
  DAT_140a13e20 = FUN_140222fa0();
  _DAT_140a2ab18 = FUN_140222fa0();
  cVar8 = FUN_140067d70(param_3);
  if ((DAT_140a61aaa == '\0') && (DAT_140a61aab == '\0')) {
    puVar26 = puVar18;
    if ((DAT_140a10d70 != 0) && (puVar26 = &DAT_140a10d60, 0xf < DAT_140a10d78)) {
      puVar26 = DAT_140a10d60;
    }
    cVar9 = FUN_14021dce0(puVar26);
    if (cVar9 == '\0') {
      return 0;
    }
  }
  DVar11 = GetModuleFileNameA((HMODULE)0x0,(LPSTR)local_378,0x104);
  if (DVar11 == 0) {
LAB_14005db21:
    GetLastError();
  }
  else {
    in_stack_fffffffffffff9f8 = 0;
    _splitpath((char *)local_378,(char *)local_178,local_488,(char *)0x0,(char *)0x0);
    puVar26 = puVar18;
    do {
      pbVar1 = local_178 + (longlong)puVar26;
      local_378[(longlong)puVar26] = *pbVar1;
      puVar26 = (undefined8 *)((longlong)puVar26 + 1);
    } while (*pbVar1 != 0);
    puVar24 = &uStack_379;
    do {
      pcVar22 = puVar24 + 1;
      puVar24 = puVar24 + 1;
    } while (*pcVar22 != '\0');
    lVar17 = 0;
    do {
      cVar9 = local_488[lVar17];
      puVar24[lVar17] = cVar9;
      lVar17 = lVar17 + 1;
    } while (cVar9 != '\0');
    BVar12 = SetCurrentDirectoryA((LPCSTR)local_378);
    if (BVar12 == 0) goto LAB_14005db21;
  }
  lVar17 = FUN_140769c58(0x38);
  if (lVar17 != 0) {
    puVar18 = (undefined8 *)FUN_140216a50(lVar17);
  }
  DAT_140a7d080 = puVar18;
  if (DAT_140a15330 != 0) {
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    FUN_140217740(puVar18,puVar26);
    lVar17 = DAT_140a15330;
    if (0x7fffffffffffffffU - DAT_140a15330 < 0xe) {
                    /* WARNING: Subroutine does not return */
      FUN_140022610();
    }
    local_5b8._0_8_ = (undefined8 *)0x0;
    puVar18 = (undefined8 *)&local_5b8;
    puVar26 = &DAT_140a15320;
    if (0xf < DAT_140a15338) {
      puVar26 = DAT_140a15320;
    }
    uVar30 = 0xf;
    local_5a8 = ZEXT816(0);
    uVar29 = DAT_140a15330 + 0xe;
    if (0xf < uVar29) {
      uVar30 = uVar29 | 0xf;
      if (uVar30 < 0x8000000000000000) {
        if (uVar30 < 0x16) {
          uVar30 = 0x16;
        }
        if (uVar30 + 1 < 0x1000) {
          if (uVar30 == 0xffffffffffffffff) {
            puVar18 = (undefined8 *)0x0;
            local_5b8._0_8_ = puVar18;
          }
          else {
            puVar18 = (undefined8 *)FUN_140769c58();
            local_5b8._0_8_ = puVar18;
          }
          goto LAB_14005dc48;
        }
        uVar19 = uVar30 + 0x28;
        if (uVar19 <= uVar30 + 1) {
                    /* WARNING: Subroutine does not return */
          FUN_1400225f0();
        }
      }
      else {
        uVar30 = 0x7fffffffffffffff;
        uVar19 = 0x8000000000000027;
      }
      lVar20 = FUN_140769c58(uVar19);
      if (lVar20 == 0) goto LAB_14005f660;
      puVar18 = (undefined8 *)(lVar20 + 0x27U & 0xffffffffffffffe0);
      puVar18[0xffffffffffffffff] = lVar20;
      local_5b8._0_8_ = puVar18;
    }
LAB_14005dc48:
    local_5a8._8_8_ = uVar30;
    local_5a8._0_8_ = uVar29;
    FUN_14076bd80(puVar18,puVar26,lVar17);
    *(undefined8 *)((longlong)puVar18 + lVar17) = s_Data_Patch_meg_140802db8._0_8_;
    *(undefined4 *)((longlong)puVar18 + lVar17 + 8) = s_Data_Patch_meg_140802db8._8_4_;
    *(undefined2 *)((longlong)puVar18 + lVar17 + 0xc) = s_Data_Patch_meg_140802db8._12_2_;
    *(undefined1 *)((longlong)puVar18 + lVar17 + 0xeU) = 0;
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\patch2.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f654;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f654:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    puVar18 = (undefined8 *)FUN_1400663e0(&local_518,&DAT_140a15320,"Data\\64Patch.meg");
    if (&local_5b8 != (tagRECT *)puVar18) {
      if (0xf < (ulonglong)local_5a8._8_8_) {
        pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 1);
        puVar26 = (undefined8 *)local_5b8._0_8_;
        if ((HINSTANCE)0xfff < pHVar28) {
          puVar26 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
          pHVar28 = (HINSTANCE)(local_5a8._8_8_ + 0x28);
          if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar26))) goto LAB_14005f65a;
        }
        thunk_FUN_1407864b8(puVar26,pHVar28);
      }
      local_5b8._0_8_ = *puVar18;
      local_5b8._8_8_ = puVar18[1];
      local_5a8 = *(undefined1 (*) [16])(puVar18 + 2);
      puVar18[2] = 0;
      puVar18[3] = 0xf;
      *(undefined1 *)puVar18 = 0;
    }
    if ((HINSTANCE)0xf < local_518.hInstance) {
      if ((undefined1 *)0xfff < (undefined1 *)((longlong)&(local_518.hInstance)->unused + 1U)) {
        puVar26 = *(undefined8 **)(local_518._0_8_ + -8);
        pHVar28 = local_518.hInstance + 10;
        if (0x1f < (local_518._0_8_ - (longlong)puVar26) - 8U) {
LAB_14005f65a:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(puVar26,pHVar28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_140216b50(DAT_140a7d080,&local_5b8,0,0);
    if (0xf < (ulonglong)local_5a8._8_8_) {
      uVar29 = local_5a8._8_8_ + 1;
      puVar18 = (undefined8 *)local_5b8._0_8_;
      if (0xfff < uVar29) {
        puVar18 = *(undefined8 **)(local_5b8._0_8_ + 0xfffffffffffffff8);
        uVar29 = local_5a8._8_8_ + 0x28;
        if (0x1f < (ulonglong)(local_5b8._0_8_ + (-8 - (longlong)puVar18))) {
LAB_14005f660:
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc();
        }
      }
      thunk_FUN_1407864b8(puVar18,uVar29);
    }
  }
  FUN_140217740(DAT_140a7d080,&DAT_140802dec);
  FUN_140216d20(DAT_140a7d080,"Data\\Patch.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\patch2.meg",0,0);
  FUN_140216d20(DAT_140a7d080,"Data\\64Patch.meg",0,0);
  if (DAT_140a15330 != 0) {
    FUN_140214110(&DAT_140a15320);
  }
  DAT_140a7c090 = FUN_140200090();
  FUN_140067590();
  local_518.cbSize = 0;
  local_518.style = 0;
  local_518.cbClsExtra = _DAT_140802a40;
  local_518.cbWndExtra = _UNK_140802a44;
  local_518.hInstance = _UNK_140802a48;
  FUN_140022730(&local_518,"Arial Unicode MS",0x10);
  FUN_140270020(&local_518);
  local_580 = 0xf;
  local_588 = 0xb;
  local_598._0_1_ = s_EmpireAtWar_140802e08[0];
  local_598._1_1_ = s_EmpireAtWar_140802e08[1];
  local_598._2_1_ = s_EmpireAtWar_140802e08[2];
  local_598._3_1_ = s_EmpireAtWar_140802e08[3];
  local_598._4_1_ = s_EmpireAtWar_140802e08[4];
  local_598._5_1_ = s_EmpireAtWar_140802e08[5];
  local_598._6_1_ = s_EmpireAtWar_140802e08[6];
  local_598._7_1_ = s_EmpireAtWar_140802e08[7];
  local_590 = (uint)CONCAT12(s_EmpireAtWar_140802e08[10],s_EmpireAtWar_140802e08._8_2_);
  FUN_140270020(&local_598);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_520,"Software\\Petroglyph",1,1,in_stack_fffffffffffff9f8);
  in_stack_fffffffffffff9f8 = in_stack_fffffffffffff9f8 & 0xffffffffffffff00;
  FUN_14021f740(&local_528,"Software\\Petroglyph\\StarWars FOC",1,1,in_stack_fffffffffffff9f8);
  FUN_14021f740(&local_578,"Software\\Petroglyph\\StarWars FOC\\Profiles",1,1,
                in_stack_fffffffffffff9f8 & 0xffffffffffffff00);
  FUN_14021f810(&local_578);
  FUN_14021f810(&local_528);
  FUN_14021f810(&local_520);
  if (((DAT_140a61b0f == '\0') &&
      (DAT_140a10cb8 = CreateMutexA((LPSECURITY_ATTRIBUTES)0x0,1,"EAWIsRunning"),
      DAT_140a10cb8 != (HANDLE)0xffffffffffffffff)) && (DVar11 = GetLastError(), DVar11 == 0xb7)) {
    ReleaseMutex(DAT_140a10cb8);
    DAT_140a10cb8 = (HANDLE)0xffffffffffffffff;
    hWnd = FindWindowA("TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
    if (hWnd == (HWND)0x0) {
      return 0;
    }
    SetForegroundWindow(hWnd);
    return 0;
  }
  if ((0xb < DAT_140a10bb0 + 1U) || ((0x9c1U >> (DAT_140a10bb0 + 1U & 0x1f) & 1) == 0)) {
    DAT_140a7c090 = DAT_140a10bb0;
  }
  if (cVar8 == '\0') {
    if (DAT_140a10cb8 == (HANDLE)0xffffffffffffffff) {
      return 0;
    }
    ReleaseMutex(DAT_140a10cb8);
    return 0;
  }
  local_578 = 0x103;
  GetUserNameA(local_488,&local_578);
  local_578 = 0x103;
  GetComputerNameA((LPSTR)local_178,&local_578);
  FUN_140025760("     %s\n\n",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0);
  uVar16 = FUN_14021dae0();
  FUN_140025760("     Version : %s\n",uVar16);
  uVar16 = FUN_14021d7e0(0,0);
  uVar21 = FUN_14021d740();
  uVar13 = FUN_14021d780();
  FUN_140025760("     Build   : %d by %s at %s\n",uVar13,uVar21,uVar16);
  CoInitialize((LPVOID)0x0);
  lVar17 = -1;
  do {
    lVar17 = lVar17 + 1;
  } while (*(short *)(PTR_u_Star_Wars_R___Empire_at_War_TM___140a10ca8 + lVar17 * 2) != 0);
  FUN_1400228a0(&DAT_140a15560);
  SetCursor((HCURSOR)0x0);
  timeGetDevCaps((LPTIMECAPS)&DAT_140a61b10,8);
  timeBeginPeriod(DAT_140a61b10);
  FUN_140499340();
  FUN_1402029c0(&DAT_140a7c230);
  FUN_1400517b0(&PTR_vftable_140a10a20);
  FUN_14021eef0(&PTR_vftable_140a15360);
  if (DAT_140a61a9b == '\0') {
    DAT_140a61aac = DAT_140a7c441;
  }
  DAT_1409cf308 = DAT_140a61ae8;
  if ((DAT_140a61ae8 == 0) && (DAT_1409cf308 = DAT_140a7c350, DAT_140a61aac != '\0')) {
    DAT_1409cf308 = DAT_140a7c35c;
  }
  DAT_1409cf30c = DAT_140a61aec;
  if ((DAT_140a61aec == 0) && (DAT_1409cf30c = DAT_140a7c354, DAT_140a61aac != '\0')) {
    DAT_1409cf30c = DAT_140a7c360;
  }
  DAT_1409cf310 = DAT_140a7c358;
  if (DAT_140a61af0 != 0) {
    DAT_1409cf310 = DAT_140a61af0;
  }
  FUN_140307830(&DAT_140b27f60);
  if ((DAT_140a7c43c != '\0') || (DAT_140a61aac == '\0')) {
    param_4 = 3;
  }
  if (DAT_140a61a9e == '\0') {
    FUN_140036d60(&DAT_140a60df0,DAT_140a7c46d);
  }
  FUN_1404913a0(FUN_14004a220,FUN_140038150,&LAB_1400381e0,FUN_140072170);
  DAT_140a61a28 = FUN_14004e480();
  DAT_140a60e28 = DAT_140a7c43f;
  FUN_140307830(&DAT_140b27f60,DAT_1409cf308,DAT_1409cf30c);
  DAT_140a61af4 = 0;
  DAT_140a619d8 = param_1;
  EnumDisplayMonitors((HDC)0x0,(LPCRECT)0x0,FUN_140065ad0,0);
  local_518.lpfnWndProc = FUN_140060c60;
  local_518.cbSize = 0x50;
  local_518.style = 0x200b;
  local_518.cbClsExtra = 0;
  local_518.cbWndExtra = 0;
  local_518.hInstance = param_1;
  local_518.hIcon = LoadIconA(param_1,(LPCSTR)0x83);
  local_518.hbrBackground = _DAT_140803530;
  local_518.lpszMenuName = _UNK_140803538;
  local_518.hCursor = (HCURSOR)0x0;
  local_518.lpszClassName = "TESTWINCLASS";
  local_518.hIconSm = LoadIconA(local_518.hInstance,(LPCSTR)0x84);
  AVar10 = RegisterClassExA(&local_518);
  if (AVar10 == 0) goto LAB_14005f55c;
  local_5b8.bottom = DAT_1409cf30c;
  local_5b8.right = DAT_1409cf308;
  local_5b8._0_8_ = (undefined8 *)0x0;
  iVar15 = 0;
  iVar14 = 0;
  if (-1 < DAT_140a10bb4) {
    local_5b8.top = DAT_140a61afc;
    local_5b8.left = DAT_140a61af8;
    local_5b8.bottom = DAT_1409cf30c + DAT_140a61afc;
    local_5b8.right = DAT_1409cf308 + DAT_140a61af8;
    iVar15 = DAT_140a61afc;
    iVar14 = DAT_140a61af8;
  }
  AdjustWindowRect(&local_5b8,0xcf0000,0);
  Y = local_5b8.top;
  LVar27 = local_5b8.bottom;
  if (local_5b8.top < iVar15) {
    Y = local_5b8.top + (iVar15 - local_5b8.top);
    LVar27 = local_5b8.bottom + (iVar15 - local_5b8.top);
    local_5b8.top = Y;
    local_5b8.bottom = LVar27;
  }
  X = local_5b8.left;
  LVar23 = local_5b8.right;
  if (local_5b8.left < iVar14) {
    X = local_5b8.left + (iVar14 - local_5b8.left);
    LVar23 = local_5b8.right + (iVar14 - local_5b8.left);
    local_5b8.left = X;
    local_5b8.right = LVar23;
  }
  uVar30 = 0;
  uVar29 = CONCAT44(uVar33,Y);
  DAT_140a619d0 =
       CreateWindowExA(0,"TESTWINCLASS",PTR_s_Star_Wars__Empire_at_War__Forces_140a10cb0,0xcf0000,X,
                       Y,LVar23 - X,LVar27 - Y,(HWND)0x0,(HMENU)0x0,param_1,(LPVOID)0x0);
  if (DAT_140a619d0 == (HWND)0x0) goto LAB_14005f55c;
  ShowWindow(DAT_140a619d0,param_4);
  UpdateWindow(DAT_140a619d0);
  FUN_140214a00(DAT_140a619d0);
  cVar8 = FUN_14004b250();
  if (cVar8 != '\x01') goto LAB_14005f55c;
  puVar18 = &DAT_140a10d40;
  if (0xf < DAT_140a10d58) {
    puVar18 = DAT_140a10d40;
  }
  _File = (FILE *)FUN_14078013c(puVar18,&DAT_140803298);
  if (_File != (FILE *)0x0) {
    local_378[0] = 0;
    pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    while (pcVar22 != (char *)0x0) {
      if ((0x2d < local_378[0]) ||
         ((0x200000002401U >> ((longlong)(char)local_378[0] & 0x3fU) & 1) == 0)) {
        uVar16 = FUN_14001fb80();
        FUN_14007e040(uVar16,local_378);
      }
      pcVar22 = common_fgets<char>(local_378,0x1ff,_File);
    }
    fclose(_File);
  }
  if (DAT_140a61a50 != (undefined8 *)0x0) {
    (**(code **)*DAT_140a61a50)(DAT_140a61a50,1);
    DAT_140a61a50 = (undefined8 *)0x0;
  }
  if (DAT_140a61a99 == '\0') {
    if (DAT_140a61a9a == '\0') {
      if (DAT_140a10d10 == 0) {
        DAT_140a61aa8 = DAT_140a61ab7;
        _guard_check_icall(&DAT_140b27f60);
        if ((DAT_140a61ace == '\0') && (DAT_140a619ef == '\0')) {
          if (DAT_140a61a9f == '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,0);
            uVar29 = uVar29 & 0xffffffffffffff00;
            FUN_14044d920(&DAT_140b306dd,"Logo1",0,FUN_140065780,1,uVar29);
          }
          else {
            FUN_1400c58e0();
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
        }
      }
      else {
        FUN_140053c10(&PTR_vftable_140a10b18);
        cVar8 = FUN_140055250(&PTR_vftable_140a10b18,&DAT_140a10d00);
        if (cVar8 == '\0') {
          FUN_140033850(&DAT_140a60df0);
          puVar18 = &DAT_140a10d00;
          if (0xf < DAT_140a10d18) {
            puVar18 = DAT_140a10d00;
          }
          cVar8 = FUN_140052d10(&PTR_vftable_140a10b18,0,puVar18,0);
          pcVar22 = (char *)FUN_14004fba0();
          if (*pcVar22 != '\0') {
            uVar16 = FUN_14004fba0();
            FUN_140263070(uVar16,0,1);
            uVar16 = FUN_14004fba0();
            FUN_140262490(uVar16,1);
          }
          if (cVar8 != '\0') goto LAB_14005e71d;
        }
        DAT_140a619ef = '\x01';
      }
    }
    else {
      cVar8 = FUN_140069040();
      if (cVar8 == '\0') {
        DAT_140a619ef = '\x01';
      }
    }
  }
  else {
    cVar8 = FUN_140068b20();
    if (cVar8 == '\0') {
      DAT_140a619ef = '\x01';
    }
  }
LAB_14005e71d:
  fVar4 = DAT_14080350c;
  fVar3 = DAT_1408007f8;
  if (DAT_140a61b0e != '\0') {
    DAT_140a14d01 = 1;
  }
  if (DAT_140a619ef == '\0') {
    iVar15 = 0;
    local_528 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    do {
      lVar17 = local_528;
      if ((((DAT_140b2f1d8 == '\x01') && (DAT_140b2f1d8 = '\0', DAT_140a61a98 == '\0')) &&
          (DAT_140a61a98 = '\x01', DAT_140a61ae0 == 0)) && (DAT_140a61acc == '\0')) {
        FUN_14025fb10(2);
        DAT_140a61acc = '\x01';
      }
      local_5b8._0_8_ = (undefined8 *)0x0;
      local_5b8._8_8_ = local_5b8._8_8_ & 0xffffffff00000000;
      local_4c8 = 0;
      local_4c0 = 0x3f800000;
      local_578 = 0;
      local_574 = 0x3f800000;
      if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x158))(DAT_140b15418,&local_5b8,&local_4c8,&local_578);
      }
      iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      while (iVar14 != 0) {
        if (local_4b8.message == 0x12) {
          DAT_140a619ef = '\x01';
          break;
        }
        iVar14 = TranslateAcceleratorA(local_4b8.hwnd,(HACCEL)0x0,&local_4b8);
        if (iVar14 == 0) {
          TranslateMessage(&local_4b8);
          DispatchMessageA(&local_4b8);
        }
        iVar14 = PeekMessageA(&local_4b8,(HWND)0x0,0,0,3);
      }
      cVar8 = thunk_FUN_1401768f0();
      if (((cVar8 != '\0') && (DAT_1409cf314 != '\0')) &&
         ((DAT_140a61b08 != 0 &&
          (DVar11 = timeGetTime(), 1000 < (DVar11 - DAT_140a61b08) - _DAT_140a7bec0)))) {
        FUN_140066980();
        uVar16 = FUN_14004fba0();
        FUN_140262af0(uVar16);
        DAT_140a61b08 = 0;
      }
      thunk_FUN_14024bb80();
      FUN_1402505c0();
      uVar5 = DAT_140b0a320;
      FUN_1402089e0();
      FUN_1403b08c0(&DAT_140b2ed18);
      FUN_1402c2910(&DAT_140b15690,uVar5);
      if (((DAT_140b15418 != (longlong *)0x0) &&
          (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0')) && (DAT_140b1574e == '\0')) {
        FUN_1402c0c70(&DAT_140b15690,uVar5);
      }
      FUN_1404908c0(&PTR_vftable_140a2aaa0);
      FUN_1400caa60();
      if (((byte)uVar5 & 0x3f) == 0x3f) {
        DVar11 = timeGetTime();
        FUN_14021e7b0(DVar11 - _DAT_140a7bec0);
        FUN_1400503d0(&DAT_140a109b0);
      }
      FUN_140051d10(&DAT_140a10a98);
      if ((*(int *)(lVar17 + 0xd4) < DAT_140a61b1c) &&
         (_Init_thread_header(&DAT_140a61b1c), DAT_140a61b1c == -1)) {
        DVar11 = timeGetTime();
        _DAT_140a61b18 = DVar11 - _DAT_140a7bec0;
        _Init_thread_footer(&DAT_140a61b1c);
      }
      DVar11 = timeGetTime();
      iVar14 = DVar11 - _DAT_140a7bec0;
      uVar31 = iVar14 - _DAT_140a61b18;
      _DAT_140a61b18 = iVar14;
      FUN_140215b90((float)uVar31 * fVar4);
      FUN_140339bc0(&DAT_140b2ba20,iVar14);
      FUN_140022c50(&DAT_1409cf080,iVar14);
      uVar16 = FUN_140067c80();
      FUN_14006a1e0(uVar16);
      uVar16 = FUN_140025770();
      FUN_14006d1d0(uVar16);
      if (DAT_1409cf314 == '\x01') {
        uVar29 = uVar29 & 0xffffffffffffff00;
        FUN_14021caf0(&DAT_140a7d0a0,iVar14,&local_5b8,&local_4c8,&local_578,uVar29);
        FUN_1402d72c0(&DAT_140b27e60,iVar14,uVar5);
        FUN_1402d2ab0(&DAT_140b27dc0,iVar14,uVar5);
        FUN_140321dc0(&DAT_140b2b300,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b3a8,iVar14,uVar5);
        _guard_check_icall(&DAT_140b2b2f0,iVar14,uVar5);
        FUN_140325190(&DAT_140b2b3b0,iVar14);
      }
      _guard_check_icall(&DAT_140a2a120,iVar14);
      FUN_1401c8420(&DAT_140a78510);
      cVar8 = thunk_FUN_1401768f0();
      if (cVar8 != '\0') {
        cVar8 = FUN_14044d840(&DAT_140b306dd);
        if (((cVar8 != '\0') && (cVar8 = FUN_14044d780(&DAT_140b306dd), cVar8 != '\0')) &&
           (FUN_14044f130(&DAT_140b306dd), DAT_140a111e0 != 0)) {
          uVar29 = uVar29 & 0xffffffffffffff00;
          puVar18 = &DAT_140a111d0;
          if (0xf < DAT_140a111e8) {
            puVar18 = DAT_140a111d0;
          }
          FUN_14044d920(&DAT_140b306dd,puVar18,0,0,1,uVar29);
        }
        FUN_14030c3b0(&DAT_140b27f60,iVar14);
        if (DAT_1409cf314 != '\0') {
          FUN_14002ee40(&DAT_140a60df0,iVar14);
        }
        if (DAT_1409cf314 != '\0') {
          FUN_140305890(&DAT_140b27f60);
        }
        FUN_1400493f0(&DAT_1409cf320,iVar14);
        FUN_1400493f0(&DAT_1409cf390);
      }
      if ((DAT_140b0a355 != '\0' || DAT_140b1574c != '\0') && (DAT_140a61b0d == '\0')) {
        if (DAT_140a61e60 != '\0') {
          FUN_14009aa80();
        }
        if ((DAT_140b1574e == '\0') || (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,&PTR_1407ff7e8,0);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,&local_598,local_548,local_568,&local_518,0,0,0,0,uVar30);
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f6a2;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f6a2:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            goto LAB_14005efe5;
          }
        }
        else {
          local_518.cbSize = 0;
          local_518.style = 0;
          local_518.cbClsExtra = 0;
          local_518.cbWndExtra = 0;
          local_518.hInstance = (HINSTANCE)&DAT_00000007;
          FUN_1400228a0(&local_518,L"TEXT_BUTTON_CONTINUE_AFTER_SYNC",0x1f);
          local_598 = 0;
          local_588 = 0;
          local_580 = 7;
          FUN_1400228a0(&local_598,&PTR_1407ff7e8,0);
          local_548[0] = 0;
          local_538 = 0;
          local_530 = 7;
          FUN_1400228a0(local_548,L"TEXT_BUTTON_STOP",0x10);
          local_568[0] = 0;
          local_558 = 0;
          local_550 = 7;
          FUN_1400228a0(local_568,L"TEXT_OUT_OF_SYNC",0x10);
          uVar30 = uVar30 & 0xffffffffffffff00;
          uVar29 = 0;
          FUN_140099d90(FUN_140061e30,local_568,local_548,&local_598,&local_518,0,0,0,0,uVar30);
          if (7 < local_550) {
            uVar19 = local_550 * 2 + 2;
            uVar25 = local_568[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_568[0] - 8);
              uVar19 = local_550 * 2 + 0x29;
              if (0x1f < (local_568[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_558 = 0;
          local_550 = 7;
          local_568[0] = local_568[0] & 0xffffffffffff0000;
          if (7 < local_530) {
            uVar19 = local_530 * 2 + 2;
            uVar25 = local_548[0];
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_548[0] - 8);
              uVar19 = local_530 * 2 + 0x29;
              if (0x1f < (local_548[0] - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_538 = 0;
          local_530 = 7;
          local_548[0] = local_548[0] & 0xffffffffffff0000;
          if (7 < local_580) {
            uVar19 = local_580 * 2 + 2;
            uVar25 = local_598;
            if (0xfff < uVar19) {
              uVar25 = *(ulonglong *)(local_598 - 8);
              uVar19 = local_580 * 2 + 0x29;
              if (0x1f < (local_598 - uVar25) - 8) goto LAB_14005f648;
            }
            thunk_FUN_1407864b8(uVar25,uVar19);
          }
          local_588 = 0;
          local_580 = 7;
          local_598 = local_598 & 0xffffffffffff0000;
          if ((HINSTANCE)&DAT_00000007 < local_518.hInstance) {
            if ((0xfff < (longlong)local_518.hInstance * 2 + 2U) &&
               (0x1f < (local_518._0_8_ - *(longlong *)(local_518._0_8_ + -8)) - 8U)) {
LAB_14005f648:
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
LAB_14005efe5:
            thunk_FUN_1407864b8();
          }
        }
        DAT_140a61b0d = '\x01';
        FUN_14027c9c0(&DAT_140b0a320);
        if (DAT_140a61ab2 != '\0') {
          local_590._0_1_ = s_oosevents_log_140802f20[8];
          local_590._1_1_ = s_oosevents_log_140802f20[9];
          local_590._2_1_ = s_oosevents_log_140802f20[10];
          local_590._3_1_ = s_oosevents_log_140802f20[0xb];
          local_58c = s_oosevents_log_140802f20[0xc];
          local_580 = 0xf;
          local_588 = 0xd;
          local_598._0_1_ = s_oosevents_log_140802f20[0];
          local_598._1_1_ = s_oosevents_log_140802f20[1];
          local_598._2_1_ = s_oosevents_log_140802f20[2];
          local_598._3_1_ = s_oosevents_log_140802f20[3];
          local_598._4_1_ = s_oosevents_log_140802f20[4];
          local_598._5_1_ = s_oosevents_log_140802f20[5];
          local_598._6_1_ = s_oosevents_log_140802f20[6];
          local_598._7_1_ = s_oosevents_log_140802f20[7];
          local_58b = 0;
          FUN_1402c20d0(&DAT_140b15690,DAT_140b0a320);
          DAT_140a61ab2 = '\0';
          if (0xf < local_580) {
            if ((0xfff < local_580 + 1) && (0x1f < (local_598 - *(longlong *)(local_598 - 8)) - 8))
            {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc();
            }
            thunk_FUN_1407864b8();
          }
        }
        FUN_140047860(&DAT_140b153e0);
      }
      uVar16 = FUN_14004fba0();
      FUN_140262b80(uVar16);
      FUN_1403b1b50(&DAT_140a286f0);
      if ((DAT_1409cf314 == '\0') &&
         (((cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 == '\0' || (DAT_140b155c4 != '\0')) &&
          (cVar8 = FUN_14024cef0(), cVar8 == '\0')))) {
        WaitMessage();
      }
      else {
        cVar8 = FUN_14027c5f0(&DAT_140b0a320);
        if (((((cVar8 == '\0') || (DAT_140b155c4 == '\0')) || (DAT_140a785a8 == 0)) ||
            (cVar9 = FUN_1401c0c70(&DAT_140a78510), cVar9 == '\0')) && (cVar8 != '\0')) {
          if (DAT_140a61e70 != '\0') {
            FUN_14009b9f0();
          }
          FUN_14027c360(&DAT_140b0a320,iVar14);
          uVar6 = DAT_140b0a320;
          FUN_1402c0c70(&DAT_140b15690,DAT_140b0a320);
          if (uVar6 == ((int)uVar6 / 100) * 100) {
            if (DAT_140a10970 == 0) {
              lVar20 = FUN_140769c58(0xc0);
              lVar17 = 0;
              if (lVar20 != 0) {
                lVar17 = FUN_140475660(lVar20);
              }
              local_520 = &PTR_vftable_140a10968;
              DAT_140a10970 = lVar17;
              if (DAT_140a7bc10 == DAT_140a7bc18) {
                FUN_14001ded0(&DAT_140a7bc08);
              }
              else {
                *DAT_140a7bc10 = &PTR_vftable_140a10968;
                DAT_140a7bc10 = DAT_140a7bc10 + 1;
              }
            }
            FUN_1404767c0(DAT_140a10970);
          }
          FUN_14020f000((float)DAT_140b0a320 / (float)DAT_140b0a340);
          FUN_14028d400(&DAT_140b153e0);
          if (DAT_140a61d38 == '\0') {
            FUN_1402718e0((float)DAT_140b0a320 / (float)DAT_140b0a340);
          }
          FUN_140297e30(&DAT_140a16fd0);
          FUN_14001dc60();
          plVar7 = DAT_140b15418;
          if (DAT_140b15418 != (longlong *)0x0) {
            FUN_1403271d0(DAT_140b15418 + 0x14);
            FUN_140490480((longlong)plVar7 + 0x9c);
            FUN_1403268c0(plVar7 + 0x14);
          }
          FUN_14044a3f0(&DAT_140b30350);
          cVar8 = FUN_14028af60(&DAT_140b153e0);
          if ((((cVar8 != '\0') || ((DAT_140a61e60 == '\0' && (DAT_140b30690 == 0)))) &&
              (DAT_140b15418 != (longlong *)0x0)) &&
             (iVar15 = (**(code **)(*DAT_140b15418 + 0xe0))(), iVar15 == 0)) {
            lVar17 = DAT_140b15418[5];
            cVar8 = FUN_140342dc0(lVar17);
            if ((cVar8 == '\x01') && (cVar8 = FUN_140342d80(lVar17), cVar8 == '\x01')) {
              FUN_140092bb0(&DAT_140a10d88);
            }
          }
          FUN_14028a4c0(&DAT_140b153e0);
          FUN_14027cb70(&DAT_140b0a320);
          iVar15 = iVar14;
          if (0 < (int)DAT_140a2ab00) {
            FUN_14005f6b0(1);
          }
        }
        else {
          FUN_14009ba60(iVar14);
          FUN_1404908c0(&DAT_140b15560);
          Sleep(0);
          if (((500 < (uint)(iVar14 - iVar15)) &&
              (cVar8 = FUN_14028af60(&DAT_140b153e0), cVar8 != '\0')) && (DAT_140b155c4 == '\0')) {
            FUN_14027d6a0(&DAT_140b0a320);
            iVar15 = iVar14;
          }
        }
      }
      if ((DAT_140a61aac != '\0') || (DAT_1409cf314 != '\0')) {
        DVar11 = timeGetTime();
        uVar19 = (ulonglong)DAT_140b0a320;
        iVar14 = DVar11 - _DAT_140a7bec0;
        if (DAT_140a61d38 != '\0') {
          FUN_1402718e0((float)(uint)(iVar14 - _DAT_140a61d3c) / fVar3);
        }
        fVar32 = (((float)uVar19 - (float)(int)uVar5) * (float)DAT_140b0a340) / fVar3;
        fVar2 = 0.0;
        if (0.0 <= fVar32) {
          fVar2 = fVar32;
        }
        cVar8 = FUN_14027d800(&DAT_140b0a320);
        if (cVar8 != '\0') {
          FUN_14028c580(&DAT_140b153e0);
          FUN_140271840();
          FUN_140301750(&DAT_140b27f60);
          FUN_14002ffb0(&DAT_140a60df0);
          FUN_14025ca30(DAT_140a619f0,(float)uVar31 / fVar3);
          FUN_140060330(fVar2 != 0.0);
        }
        FUN_14027c2f0(&DAT_140b0a320,iVar14);
        timeGetTime();
        FUN_14027c330(&DAT_140b0a320);
      }
    } while (DAT_140a619ef == '\0');
  }
LAB_14005f55c:
  timeEndPeriod(DAT_140a61b10);
  if (DAT_140a10cb8 != (HANDLE)0xffffffffffffffff) {
    ReleaseMutex(DAT_140a10cb8);
  }
  if ((((DAT_140a61acd != '\0') && (iVar15 = GetSystemMetrics(0x57), iVar15 != 0)) &&
      (DVar11 = ExpandEnvironmentStringsA("%SystemRoot%\\ehome\\ehshell.exe",local_488,0x104),
      DVar11 != 0)) && (DVar11 = GetFileAttributesA(local_488), DVar11 != 0xffffffff)) {
    ShellExecuteA((HWND)0x0,"open",local_488,(LPCSTR)0x0,(LPCSTR)0x0,1);
  }
  hProcess = GetCurrentProcess();
  TerminateProcess(hProcess,0);
  return 0;
}



// ------------------------------------------------------------
// RVA: 0x4da50  Name: FUN_14004da50  Size: 1199 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14004da50(void)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  
  if (DAT_140b30690 != 0) {
    FUN_14044f130(&DAT_140b306dd);
  }
  FUN_1400c6e30();
  FUN_140051ba0(&DAT_140a10a98,0);
  FUN_140050240(&DAT_140a109b0,0);
  FUN_14048aff0(&DAT_140a2aa40);
  uVar2 = FUN_14004fc30();
  FUN_140475540(uVar2);
  FUN_140332920(&DAT_140a172d0);
  FUN_14030c2e0(&DAT_140b27f60);
  FUN_14044cfc0(&DAT_140a28c10);
  FUN_1404724c0(&DAT_140a2a060);
  FUN_14047add0(&DAT_140b31160);
  FUN_1404746a0(&DAT_140a2a0b0);
  FUN_1402d7060(&DAT_140b27e60);
  FUN_140467f40(&DAT_140a29f20);
  FUN_140470d20(&DAT_140a29fa0);
  FUN_140470430(&DAT_140a29f60);
  FUN_14048a180(&DAT_140a2a9c0);
  thunk_FUN_140427fc0(&DAT_140b2f170);
  FUN_140359830(&DAT_140b2bcb0,0);
  _guard_check_icall(&DAT_140b2f2e0);
  uVar2 = FUN_14004fd50();
  FUN_140488ab0(uVar2);
  FUN_1402d2900(&DAT_140b27dc0);
  FUN_140321c80(&DAT_140b2b300);
  FUN_140322730(&DAT_140b2b3a8);
  FUN_1403207a0(&DAT_140b2b2f0);
  thunk_FUN_140477030(&DAT_140a2a120);
  FUN_1401c8070(&DAT_140a78510,0);
  FUN_1401f9aa0(&DAT_140a7bc28);
  FUN_1401f9aa0(&DAT_140a7bc58);
  FUN_14021c940(&DAT_140a7d0a0);
  thunk_FUN_1402c91c0(&DAT_140b15800);
  thunk_FUN_1404456f0(&DAT_140b30160);
  FUN_14048c510(&DAT_140b31330);
  FUN_14046a680(&DAT_140b30b40);
  FUN_140325150(&DAT_140b2b3b0);
  FUN_1404828d0(&DAT_140a2a850);
  FUN_140483180();
  FUN_140489bb0(&DAT_140a2a980);
  uVar2 = FUN_14004fb30();
  FUN_14046f8a0(uVar2);
  uVar2 = FUN_14001fb80();
  FUN_14007d3a0(uVar2);
  FUN_14047f690();
  FUN_14048f1d0();
  FUN_1404809d0(&DAT_140a2a810);
  FUN_1404817f0();
  FUN_140489320();
  FUN_14048f8c0(&PTR_vftable_140a2aa78);
  uVar2 = FUN_14004faa0();
  FUN_140476820(uVar2);
  if (DAT_140a619ed != '\0') {
    FUN_140030770(&DAT_140a60df0);
    FUN_1403b1af0(&DAT_140a286f0);
    uVar2 = FUN_14004fba0();
    FUN_140263300(uVar2);
    if (DAT_1409cf008 != 0) {
      puVar3 = DAT_140a7bc08;
      if (DAT_140a7bc08 != DAT_140a7bc10) {
        do {
          if ((undefined *)*puVar3 == &DAT_1409cf000) break;
          puVar3 = puVar3 + 1;
        } while (puVar3 != DAT_140a7bc10);
        if (puVar3 != DAT_140a7bc10) {
          FUN_14076bd80(puVar3,puVar3 + 1,(longlong)DAT_140a7bc10 - (longlong)(puVar3 + 1));
          DAT_140a7bc10 = DAT_140a7bc10 + -1;
          if (DAT_140a7bc08 == DAT_140a7bc10) {
            DAT_140a7bc10 = DAT_140a7bc08;
          }
        }
      }
      lVar1 = DAT_1409cf008;
      if (DAT_1409cf008 != 0) {
        FUN_14007dd80(DAT_1409cf008);
        thunk_FUN_1407864b8(lVar1,0xd0);
      }
      DAT_1409cf008 = 0;
    }
    if (DAT_140a61a08 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a08 = 0;
    }
    if (DAT_140a61a10 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a10 = 0;
    }
    if (DAT_140a61a18 != 0) {
      FUN_140260f10(&DAT_140b0a130);
      DAT_140a61a18 = 0;
    }
    if (DAT_140b2eea0 != (undefined8 *)0x0) {
      (**(code **)*DAT_140b2eea0)(DAT_140b2eea0,1);
      DAT_140b2eea0 = (undefined8 *)0x0;
    }
    if (DAT_140a619f8 != (undefined8 *)0x0) {
      (**(code **)*DAT_140a619f8)(DAT_140a619f8,1);
      DAT_140a619f8 = (undefined8 *)0x0;
    }
    _DAT_140a28790 = 0;
    DAT_140b306a0 = 0;
    FUN_1400fc8d0();
    FUN_1400fc9a0();
    if (DAT_140a619f0 != (undefined8 *)0x0) {
      FUN_14025c680();
      if (DAT_140a619f0 != (undefined8 *)0x0) {
        (**(code **)*DAT_140a619f0)(DAT_140a619f0,1);
      }
      DAT_140a619f0 = (undefined8 *)0x0;
    }
    FUN_140362b20();
    FUN_14026dbe0();
    FUN_14048a9c0();
    FUN_14025fd10();
    DAT_140a619ed = '\0';
  }
  FUN_1401d4730();
  FUN_1401d2a00();
  FUN_14002d750();
  FUN_140215e10();
  FUN_140049290(&DAT_1409cf320);
  FUN_140049290(&DAT_1409cf390);
  uVar2 = FUN_14004fcc0();
  FUN_14047fd90(uVar2);
  FUN_140261370(&DAT_140b0a130);
  FUN_140261260(&DAT_140b0a130,DAT_140a61a30,"EmpireAtWar-Bold");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a38,"EmpireAtWar-Light");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a40,"EmpireAtWar-Medium");
  FUN_140261260(&DAT_140b0a130,DAT_140a61a48,"EmpireAtWar-Stencil");
  DAT_140b15668 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x4b250  Name: FUN_14004b250  Size: 10192 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14004b250(void)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  DWORD DVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong *plVar8;
  ulonglong uVar9;
  undefined8 *puVar10;
  char *pcVar11;
  undefined8 *******pppppppuVar12;
  uint uVar13;
  longlong lVar14;
  ulonglong uVar15;
  longlong lVar16;
  undefined8 *puVar17;
  undefined4 local_res8 [2];
  code *local_res10;
  undefined4 local_res18;
  undefined4 local_res1c;
  undefined8 in_stack_fffffffffffffeb0;
  undefined4 uVar20;
  undefined8 uVar18;
  longlong *plVar19;
  undefined4 uVar21;
  undefined8 *******local_58 [2];
  longlong local_48;
  ulonglong local_40;
  
  uVar20 = (undefined4)((ulonglong)in_stack_fffffffffffffeb0 >> 0x20);
  uVar15 = 0;
  local_res8[0] = 0;
  FUN_140048f20();
  FUN_1401d4160(DAT_140a619d0);
  FUN_1401d25b0();
  FUN_1401d3c20();
  FUN_1401d4700(DAT_140a619d0);
  uVar6 = FUN_14004fb30();
  cVar3 = FUN_14046ea70(uVar6);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                "DatabaseConversionSystemClass::Get().System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize DatabaseConversionSystem.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_1402cb210(&DAT_140b15800,&DAT_140a10888);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheGameConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_1404457e0(&DAT_140b30160,&DAT_140a108e8);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudioConstants.System_Initialize",uVar7,uVar6,
                uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheAudioConstants.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_14048b8a0(&DAT_140b31330,&DAT_140a10928,0);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize TheMousePointerDataManager.");
    FUN_14004da50();
    return 0;
  }
  cVar3 = FUN_140261280(&DAT_140b0a130);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","FontManager.System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize FontManager.");
    FUN_14004da50();
    return 0;
  }
  uVar6 = FUN_14004faa0();
  FUN_1404766a0(uVar6);
  FUN_14047fd00(&DAT_1409cf314);
  FUN_14047fcf0(FUN_14004f320);
  uVar6 = FUN_14004fcc0();
  cVar3 = FUN_14047fd10(uVar6,DAT_140a619d8,DAT_140a619d0);
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  uVar18 = CONCAT44(uVar20,DVar4);
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                "TheDebugWindowManagerClass::Get().System_Initialize",uVar7,uVar6,uVar18);
  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
  if (cVar3 == '\0') {
    FUN_1402149d0("Error: Unable to initialize DebugWindowManager.");
  }
  lVar16 = -1;
  lVar14 = -1;
  local_58[0] = (undefined8 *******)0x0;
  local_48 = 0;
  local_40 = 0xf;
  do {
    lVar14 = lVar14 + 1;
  } while (PTR_s___Data_XML_GraphicDetails_xml_140a10798[lVar14] != '\0');
  FUN_140022730(local_58);
  cVar3 = FUN_14048aa00(&DAT_140a2aa40,local_58,0);
  if (0xf < local_40) {
    if (0xfff < local_40 + 1) {
      if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DVar4 = timeGetTime();
  uVar6 = FUN_140216790();
  uVar7 = FUN_140216710();
  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGraphicDetailManager.System_Initialize",uVar7,
                uVar6,CONCAT44(uVar20,DVar4));
  if (cVar3 == '\0') {
    pcVar11 = "Error: Unable to initialize GraphicDetailManager.";
  }
  else {
    local_res8[0] = 0;
    DAT_140a61a30 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    if (DAT_140a61a30 == 0) {
      local_res8[0] = 0;
      DAT_140a61a30 =
           FUN_140260c20(&DAT_140b0a130,&DAT_1409dfa60,0x10094,0,local_res8,"EmpireAtWar-Bold");
    }
    local_res8[0] = 0;
    DAT_140a61a38 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409cf410,0x1064c,0,local_res8,"EmpireAtWar-Light");
    local_res8[0] = 0;
    DAT_140a61a40 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409efb00,0x10414,0,local_res8,"EmpireAtWar-Medium");
    local_res8[0] = 0;
    uVar20 = 1;
    DAT_140a61a48 =
         FUN_140260c20(&DAT_140b0a130,&DAT_1409fff20,0x10830,0,local_res8,"EmpireAtWar-Stencil");
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    uVar18 = CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","EAW Fonts Loaded",uVar7,uVar6,uVar18);
    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
    cVar3 = FUN_14004df10(DAT_140a61aac,DAT_1409cf308,DAT_1409cf30c,DAT_1409cf310,DAT_140a61ac0);
    DVar4 = timeGetTime();
    uVar6 = FUN_140216790();
    uVar7 = FUN_140216710();
    plVar19 = (longlong *)CONCAT44(uVar20,DVar4);
    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","Init_3D_Game",uVar7,uVar6,plVar19);
    if (cVar3 == '\0') {
      pcVar11 = "Error: Unable to initialize via Init_3D_Game().";
    }
    else {
      uVar7 = FUN_1402c8210(&DAT_140b15800,DAT_140a7c090,DAT_140a13ed8);
      FUN_14001e680(local_58,uVar7);
      if (local_48 == 0) {
        lVar14 = -1;
        do {
          lVar14 = lVar14 + 1;
        } while (PTR_s_Splash_TGA_140a10ba8[lVar14] != '\0');
        FUN_140022730(local_58);
      }
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      FUN_140025760("Loading splash screen %s\n",pppppppuVar12);
      pppppppuVar12 = local_58;
      if (0xf < local_40) {
        pppppppuVar12 = local_58[0];
      }
      plVar8 = (longlong *)FUN_14025e990(pppppppuVar12,1,0);
      if (plVar8 == (longlong *)0x0) {
        pppppppuVar12 = local_58;
        if (0xf < local_40) {
          pppppppuVar12 = local_58[0];
        }
        FUN_140025760("Failed to load splash screen %s\n",pppppppuVar12);
      }
      else {
        (**(code **)*plVar8)(plVar8);
        lVar14 = FUN_140769c58(0x90);
        uVar9 = uVar15;
        if (lVar14 != 0) {
          uVar6 = CONCAT44((int)((ulonglong)uVar6 >> 0x20),DAT_1407ffaf8);
          plVar19 = plVar8;
          uVar9 = FUN_14025ff10(lVar14,0,0,DAT_1407ffaf8,uVar6,plVar8);
        }
        DAT_140a61a50 = uVar9;
        (**(code **)(*plVar8 + 8))(plVar8);
        FUN_14025c080(DAT_140a619f0,DAT_140a61a50);
      }
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      FUN_14025de30();
      FUN_14025c8c0(DAT_140a619f0);
      thunk_FUN_1401781f0();
      thunk_FUN_14013ce50();
      if (0xf < local_40) {
        if (0xfff < local_40 + 1) {
          if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      pcVar11 = (char *)FUN_14004fba0();
      if (*pcVar11 == '\x01') {
        uVar7 = FUN_14004fba0();
        FUN_140263300(uVar7);
      }
      iVar5 = FUN_14048b800(&DAT_140b31330);
      uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
      uVar9 = uVar15;
      if (0 < iVar5) {
        do {
          puVar10 = (undefined8 *)FUN_14048b7d0(&DAT_140b31330,uVar9);
          uVar20 = (undefined4)((ulonglong)uVar6 >> 0x20);
          uVar21 = (undefined4)((ulonglong)plVar19 >> 0x20);
          if (puVar10 != (undefined8 *)0x0) {
            plVar8 = puVar10 + 5;
            if (0xf < (ulonglong)puVar10[8]) {
              plVar8 = (longlong *)*plVar8;
            }
            puVar17 = puVar10;
            if (0xf < (ulonglong)puVar10[3]) {
              puVar17 = (undefined8 *)*puVar10;
            }
            uVar7 = FUN_14004fba0();
            plVar19 = (longlong *)CONCAT44(uVar21,*(undefined4 *)(puVar10 + 10));
            uVar6 = CONCAT44(uVar20,*(undefined4 *)((longlong)puVar10 + 0x4c));
            FUN_140262a00(uVar7,puVar17,plVar8,*(undefined4 *)(puVar10 + 9),uVar6,plVar19);
          }
          uVar20 = (undefined4)((ulonglong)plVar19 >> 0x20);
          uVar13 = (int)uVar9 + 1;
          uVar9 = (ulonglong)uVar13;
        } while ((int)uVar13 < iVar5);
      }
      uVar6 = FUN_14004fba0();
      FUN_1402625a0(uVar6);
      uVar6 = FUN_14004fba0();
      FUN_140262490(uVar6,0);
      FUN_14020e010();
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","KeyboardClass::Init",uVar7,uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      FUN_140242cd0("Data/Scripts/Library/");
      FUN_140242cd0("Data/Scripts/GameObject/");
      FUN_140242cd0("Data/Scripts/FreeStore/");
      FUN_140242cd0("Data/Scripts/Miscellaneous/");
      FUN_140242cd0("Data/Scripts/Story/");
      uVar6 = FUN_14001fb80();
      cVar3 = FUN_14007d100(uVar6);
      DVar4 = timeGetTime();
      uVar6 = FUN_140216790();
      uVar7 = FUN_140216710();
      uVar18 = CONCAT44(uVar20,DVar4);
      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheConsole::Get().System_Initialize",uVar7,
                    uVar6,uVar18);
      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
      if (cVar3 == '\0') {
        pcVar11 = "Error: Unable to initialize TheConsole.";
      }
      else {
        cVar3 = FUN_14021c2f0(&DAT_140a7d0a0,DAT_140a7c090,DAT_140a61ab6);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheAudio.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          FUN_1402149d0("Error: Unable to successfully initialize the TheAudio system.");
        }
        cVar3 = FUN_1401f9a60(&DAT_140a7bc58,DAT_140a7c090,
                              PTR_s___Data_Text_MasterTextFile_txt_140a10768);
        DVar4 = timeGetTime();
        uVar6 = FUN_140216790();
        uVar7 = FUN_140216710();
        uVar18 = CONCAT44(uVar20,DVar4);
        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGameText.System_Initialize",uVar7,uVar6,
                      uVar18);
        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
        if (cVar3 == '\0') {
          pcVar11 = "Error: Unable to initialize TheGameText.";
        }
        else {
          FUN_14004f150();
          cVar3 = FUN_1401f9a60(&DAT_140a7bc28,DAT_140a7c090,
                                PTR_s___Data_Text_CreditsText_txt_140a10770);
          DVar4 = timeGetTime();
          uVar9 = FUN_140216790();
          uVar6 = FUN_140216710();
          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheCreditsText.System_Initialize",uVar6,
                        uVar9,CONCAT44(uVar20,DVar4));
          if (cVar3 == '\0') {
            pcVar11 = "Error: Unable to initialize TheCreditsText.";
          }
          else {
            uVar20 = 1;
            cVar3 = FUN_1401c7bb0(&DAT_140a78510,PTR_s___Data_Resources_GUIDialog_GUIDi_140a10750,
                                  PTR_s___Data_Resources_GUIDialog_Resou_140a10758,
                                  PTR_s___Data_XML_GUIDialogs_xml_140a10760,
                                  uVar9 & 0xffffffffffffff00,FUN_140037840);
            DVar4 = timeGetTime();
            uVar6 = FUN_140216790();
            uVar7 = FUN_140216710();
            uVar18 = CONCAT44(uVar20,DVar4);
            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheGUIDialogManager.System_Initialize",
                          uVar7,uVar6,uVar18);
            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
            if (cVar3 == '\0') {
              pcVar11 = "Error: Unable to initialize TheGUIDialogManager.";
            }
            else {
              _DAT_140a79880 = FUN_140037eb0;
              lVar14 = -1;
              local_58[0] = (undefined8 *******)0x0;
              local_48 = 0;
              local_40 = 0xf;
              do {
                lVar14 = lVar14 + 1;
              } while (PTR_s___Data_XML_SFXEventFiles_xml_140a10780[lVar14] != '\0');
              FUN_140022730(local_58);
              cVar3 = FUN_1402d5df0(&DAT_140b27e60,local_58,0);
              if (0xf < local_40) {
                if (0xfff < local_40 + 1) {
                  if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])))
                  {
                    /* WARNING: Subroutine does not return */
                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                  }
                }
                thunk_FUN_1407864b8();
              }
              DVar4 = timeGetTime();
              uVar6 = FUN_140216790();
              uVar7 = FUN_140216710();
              uVar18 = CONCAT44(uVar20,DVar4);
              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n","TheSFXEventManager.System_Initialize",
                            uVar7,uVar6,uVar18);
              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
              if (cVar3 == '\0') {
                pcVar11 = "Error: Unable to initialize TheSFXEventManager.";
              }
              else {
                lVar14 = -1;
                local_58[0] = (undefined8 *******)0x0;
                local_48 = 0;
                local_40 = 0xf;
                do {
                  lVar14 = lVar14 + 1;
                } while (PTR_s___Data_XML_TerrainDecalFX_xml_140a10790[lVar14] != '\0');
                FUN_140022730(local_58);
                cVar3 = FUN_140470ef0(&DAT_140a2a020,local_58,0);
                if (0xf < local_40) {
                  if (0xfff < local_40 + 1) {
                    if (0x1f < (ulonglong)((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))
                       ) {
                    /* WARNING: Subroutine does not return */
                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                    }
                  }
                  thunk_FUN_1407864b8();
                }
                DVar4 = timeGetTime();
                uVar6 = FUN_140216790();
                uVar7 = FUN_140216710();
                uVar18 = CONCAT44(uVar20,DVar4);
                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                              "TheTerrainDecalFXManager.System_Initialize",uVar7,uVar6,uVar18);
                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                if (cVar3 == '\0') {
                  pcVar11 = "Error: Unable to initialize TheTerrainDecalFXManager.";
                }
                else {
                  lVar14 = -1;
                  local_58[0] = (undefined8 *******)0x0;
                  local_48 = 0;
                  local_40 = 0xf;
                  do {
                    lVar14 = lVar14 + 1;
                  } while (PTR_s___Data_XML_DynamicTrackFX_xml_140a107a0[lVar14] != '\0');
                  FUN_140022730(local_58);
                  cVar3 = FUN_14046ffe0(&DAT_140a29f60,local_58,0);
                  if (0xf < local_40) {
                    if (0xfff < local_40 + 1) {
                      if (0x1f < (ulonglong)
                                 ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                        FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                      }
                    }
                    thunk_FUN_1407864b8();
                  }
                  DVar4 = timeGetTime();
                  uVar6 = FUN_140216790();
                  uVar7 = FUN_140216710();
                  uVar18 = CONCAT44(uVar20,DVar4);
                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                "TheDynamicTrackFXManager.System_Initialize",uVar7,uVar6,uVar18);
                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                  if (cVar3 == '\0') {
                    pcVar11 = "Error: Unable to initialize TheDynamicTrackFXManager.";
                  }
                  else {
                    lVar14 = -1;
                    local_58[0] = (undefined8 *******)0x0;
                    local_48 = 0;
                    local_40 = 0xf;
                    do {
                      lVar14 = lVar14 + 1;
                    } while (PTR_s___Data_XML_StarWars3DTextCrawl_x_140a107c0[lVar14] != '\0');
                    FUN_140022730(local_58);
                    cVar3 = FUN_140489d30(&DAT_140a2a9c0,local_58,0);
                    if (0xf < local_40) {
                      if (0xfff < local_40 + 1) {
                        if (0x1f < (ulonglong)
                                   ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                          FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                        }
                      }
                      thunk_FUN_1407864b8();
                    }
                    DVar4 = timeGetTime();
                    uVar6 = FUN_140216790();
                    uVar7 = FUN_140216710();
                    uVar18 = CONCAT44(uVar20,DVar4);
                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                  "TheDraw3DTextCrawlManager.System_Initialize",uVar7,uVar6,uVar18);
                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                    if (cVar3 == '\0') {
                      pcVar11 = "Error: Unable to initialize TheDraw3DTextCrawlManager.";
                    }
                    else {
                      lVar14 = -1;
                      local_58[0] = (undefined8 *******)0x0;
                      local_48 = 0;
                      local_40 = 0xf;
                      do {
                        lVar14 = lVar14 + 1;
                      } while (PTR_s___Data_XML_SurfaceFX_xml_140a10788[lVar14] != '\0');
                      FUN_140022730(local_58);
                      cVar3 = FUN_140470750(&DAT_140a29fa0,local_58,0);
                      if (0xf < local_40) {
                        if (0xfff < local_40 + 1) {
                          if (0x1f < (ulonglong)
                                     ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                          }
                        }
                        thunk_FUN_1407864b8();
                      }
                      DVar4 = timeGetTime();
                      uVar6 = FUN_140216790();
                      uVar7 = FUN_140216710();
                      uVar18 = CONCAT44(uVar20,DVar4);
                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                    "TheSurfaceFXEventManager.System_Initialize",uVar7,uVar6,uVar18)
                      ;
                      uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                      if (cVar3 == '\0') {
                        pcVar11 = "Error: Unable to initialize TheSurfaceFXEventManager.";
                      }
                      else {
                        lVar14 = -1;
                        local_58[0] = (undefined8 *******)0x0;
                        local_48 = 0;
                        local_40 = 0xf;
                        do {
                          lVar14 = lVar14 + 1;
                        } while (PTR_s___Data_XML_ShadowBlobMaterials_x_140a107a8[lVar14] != '\0');
                        FUN_140022730(local_58);
                        cVar3 = FUN_1404280b0(&DAT_140b2f170,local_58,0);
                        if (0xf < local_40) {
                          if (0xfff < local_40 + 1) {
                            if (0x1f < (ulonglong)
                                       ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                            }
                          }
                          thunk_FUN_1407864b8();
                        }
                        DVar4 = timeGetTime();
                        uVar6 = FUN_140216790();
                        uVar7 = FUN_140216710();
                        uVar18 = CONCAT44(uVar20,DVar4);
                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                      "TheShadowBlobManager.System_Initialize",uVar7,uVar6,uVar18);
                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                        if (cVar3 == '\0') {
                          pcVar11 = "Error: Unable to initialize TheShadowBlobManager.";
                        }
                        else {
                          FUN_140427e20(&DAT_140b2f170,0);
                          cVar3 = FUN_14047ffe0(PTR_s___Data_XML_TacticalCameras_xml_140a107b0);
                          DVar4 = timeGetTime();
                          uVar6 = FUN_140216790();
                          uVar7 = FUN_140216710();
                          uVar18 = CONCAT44(uVar20,DVar4);
                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                        "TacticalCameraConstantsManager::System_Initialize",uVar7,
                                        uVar6,uVar18);
                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                          if (cVar3 == '\0') {
                            pcVar11 = "Error: Unable to initialize TacticalCameraConstantsManager.";
                          }
                          else {
                            lVar14 = -1;
                            local_58[0] = (undefined8 *******)0x0;
                            local_48 = 0;
                            local_40 = 0xf;
                            do {
                              lVar14 = lVar14 + 1;
                            } while (PTR_s___Data_XML_RadarMap_xml_140a10838[lVar14] != '\0');
                            FUN_140022730(local_58);
                            cVar3 = FUN_140359390(&DAT_140b2bcb0,local_58,0);
                            if (0xf < local_40) {
                              if (0xfff < local_40 + 1) {
                                if (0x1f < (ulonglong)
                                           ((longlong)local_58[0] + (-8 - (longlong)local_58[0][-1])
                                           )) {
                    /* WARNING: Subroutine does not return */
                                  FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                }
                              }
                              thunk_FUN_1407864b8();
                            }
                            DVar4 = timeGetTime();
                            uVar6 = FUN_140216790();
                            uVar7 = FUN_140216710();
                            uVar18 = CONCAT44(uVar20,DVar4);
                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                          "TheRadarMap.System_Initialize",uVar7,uVar6,uVar18);
                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                            if (cVar3 == '\0') {
                              pcVar11 = "Error: Unable to initialize TheRadarMap.";
                            }
                            else {
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_UnitAbilityTypes_xml_140a10878[lVar14] !=
                                       '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_14043fab0(&DAT_140b2f2e0,local_58,0);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheUnitAbilityTypeManager.System_Initialize",uVar7,
                                            uVar6,uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                FUN_1402149d0(
                                             "Error: Unable to successfully initialize the TheUnitAbilityTypeManager system."
                                             );
                              }
                              lVar14 = -1;
                              local_58[0] = (undefined8 *******)0x0;
                              local_48 = 0;
                              local_40 = 0xf;
                              do {
                                lVar14 = lVar14 + 1;
                              } while (PTR_s___Data_XML_MusicEvents_xml_140a107c8[lVar14] != '\0');
                              FUN_140022730(local_58);
                              cVar3 = FUN_1402d2370(&DAT_140b27dc0,local_58);
                              if (0xf < local_40) {
                                if (0xfff < local_40 + 1) {
                                  if (0x1f < (ulonglong)
                                             ((longlong)local_58[0] +
                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                  }
                                }
                                thunk_FUN_1407864b8();
                              }
                              DVar4 = timeGetTime();
                              uVar6 = FUN_140216790();
                              uVar7 = FUN_140216710();
                              uVar18 = CONCAT44(uVar20,DVar4);
                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                            "TheMusicEventManager.System_Initialize",uVar7,uVar6,
                                            uVar18);
                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                              if (cVar3 == '\0') {
                                pcVar11 = "Error: Unable to initialize TheMusicEventManager.";
                              }
                              else {
                                lVar14 = -1;
                                local_58[0] = (undefined8 *******)0x0;
                                local_48 = 0;
                                local_40 = 0xf;
                                do {
                                  lVar14 = lVar14 + 1;
                                } while (PTR_s___Data_XML_SpeechEvents_xml_140a107d0[lVar14] != '\0'
                                        );
                                FUN_140022730(local_58);
                                cVar3 = FUN_140321450(&DAT_140b2b300,local_58);
                                if (0xf < local_40) {
                                  if (0xfff < local_40 + 1) {
                                    if (0x1f < (ulonglong)
                                               ((longlong)local_58[0] +
                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                    }
                                  }
                                  thunk_FUN_1407864b8();
                                }
                                DVar4 = timeGetTime();
                                uVar6 = FUN_140216790();
                                uVar7 = FUN_140216710();
                                uVar18 = CONCAT44(uVar20,DVar4);
                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                              "TheSpeechEventManager.System_Initialize",uVar7,uVar6,
                                              uVar18);
                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                if (cVar3 == '\0') {
                                  pcVar11 = "Error: Unable to initialize TheSpeechEventManager.";
                                }
                                else {
                                  cVar3 = FUN_1403226e0(&DAT_140b2b3a8);
                                  DVar4 = timeGetTime();
                                  uVar6 = FUN_140216790();
                                  uVar7 = FUN_140216710();
                                  uVar18 = CONCAT44(uVar20,DVar4);
                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                "TheSpeechConversationManager.System_Initialize",
                                                uVar7,uVar6,uVar18);
                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                  if (cVar3 == '\0') {
                                    pcVar11 = 
                                    "Error: Unable to initialize TheSpeechConversationManager.";
                                  }
                                  else {
                                    cVar3 = FUN_140320760(&DAT_140b2b2f0);
                                    DVar4 = timeGetTime();
                                    uVar6 = FUN_140216790();
                                    uVar7 = FUN_140216710();
                                    uVar18 = CONCAT44(uVar20,DVar4);
                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                  "TheSFXConversationManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                    uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                    if (cVar3 == '\0') {
                                      pcVar11 = 
                                      "Error: Unable to initialize TheSFXConversationManager.";
                                    }
                                    else {
                                      local_res10 = FUN_14032e230;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      local_res10 = (code *)&LAB_140107ed0;
                                      FUN_140046fc0(&DAT_140b2b378,&local_res10);
                                      lVar14 = -1;
                                      local_58[0] = (undefined8 *******)0x0;
                                      local_48 = 0;
                                      local_40 = 0xf;
                                      do {
                                        lVar14 = lVar14 + 1;
                                      } while (PTR_s___Data_XML_AnimationSFXMaps_txt_140a107d8
                                               [lVar14] != '\0');
                                      FUN_140022730(local_58);
                                      cVar3 = FUN_1404770a0(&DAT_140a2a120,local_58,0);
                                      if (0xf < local_40) {
                                        if (0xfff < local_40 + 1) {
                                          if (0x1f < (ulonglong)
                                                     ((longlong)local_58[0] +
                                                     (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                            FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                          }
                                        }
                                        thunk_FUN_1407864b8();
                                      }
                                      DVar4 = timeGetTime();
                                      uVar6 = FUN_140216790();
                                      uVar7 = FUN_140216710();
                                      FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                    "TheAnimSFXMapper.System_Initialize",uVar7,uVar6
                                                    ,CONCAT44(uVar20,DVar4));
                                      if (cVar3 == '\0') {
                                        pcVar11 = "Error: Unable to initialize TheAnimSFXMapper.";
                                      }
                                      else {
                                        lVar14 = -1;
                                        local_58[0] = (undefined8 *******)0x0;
                                        local_48 = 0;
                                        local_40 = 0xf;
                                        do {
                                          lVar14 = lVar14 + 1;
                                        } while (PTR_s___Data_XML_CommandBarComponentFi_140a10808
                                                 [lVar14] != '\0');
                                        FUN_140022730(local_58);
                                        uVar21 = 1;
                                        uVar20 = 1;
                                        cVar3 = FUN_14030ae50(&DAT_140b27f60,local_58,DAT_140a619f0,
                                                              FUN_14009cec0,&LAB_14009db00,
                                                              FUN_1400374b0,FUN_1400375c0,
                                                              FUN_1400a7aa0,FUN_140037670,
                                                              FUN_140037720,&LAB_1400a3540,
                                                              &LAB_140037210,&LAB_140037230,
                                                              FUN_140037250,FUN_140037410,
                                                              FUN_140037760,FUN_140037450,
                                                              &LAB_140037470,&LAB_140037810,
                                                              FUN_1400d6770,&LAB_1400d63f0,
                                                              &LAB_1400378e0,&LAB_140037900,
                                                              &LAB_140037920,FUN_140037940,
                                                              FUN_140037ae0,FUN_140037c30,
                                                              FUN_140037a00,FUN_14009d2a0,
                                                              FUN_1400d3c20,FUN_1400dc410,
                                                              FUN_140037cb0,&LAB_140037ce0,
                                                              &LAB_140037cf0,0);
                                        if (0xf < local_40) {
                                          if (0xfff < local_40 + 1) {
                                            if (0x1f < (ulonglong)
                                                       ((longlong)local_58[0] +
                                                       (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                              FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                            }
                                          }
                                          thunk_FUN_1407864b8();
                                        }
                                        DVar4 = timeGetTime();
                                        uVar6 = FUN_140216790();
                                        uVar7 = FUN_140216710();
                                        uVar18 = CONCAT44(uVar20,DVar4);
                                        FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                      "TheCommandBar.System_Initialize",uVar7,uVar6,
                                                      uVar18);
                                        uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                        if (cVar3 == '\0') {
                                          pcVar11 = "Error: Unable to initialize TheCommandBar.";
                                        }
                                        else {
                                          cVar3 = FUN_140447190(&DAT_140b30350,FUN_140123d70,
                                                                FUN_14005f6b0,DAT_140a61a10);
                                          DVar4 = timeGetTime();
                                          uVar6 = FUN_140216790();
                                          uVar7 = FUN_140216710();
                                          uVar18 = CONCAT44(uVar20,DVar4);
                                          FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                        "TheTutorial.Init",uVar7,uVar6,uVar18);
                                          uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                          if (cVar3 == '\0') {
                                            pcVar11 = "Error: Unable to initialize TheTutorial.";
                                          }
                                          else {
                                            cVar3 = FUN_14044ce30(&DAT_140a28c10,DAT_140a619f0);
                                            DVar4 = timeGetTime();
                                            uVar6 = FUN_140216790();
                                            uVar7 = FUN_140216710();
                                            uVar18 = CONCAT44(uVar20,DVar4);
                                            FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                          "CameraFXManager.System_Initialize",uVar7,
                                                          uVar6,uVar18);
                                            uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                            if (cVar3 == '\0') {
                                              pcVar11 = 
                                              "Error: Unable to initialize CameraFXManager.";
                                            }
                                            else {
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (PTR_s___Data_XML_FactionFiles_xml_140a10830
                                                       [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              FUN_140331d70(&DAT_140a172d0,local_58);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                            "FactionList.Initialize",uVar7,uVar6,
                                                            uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              lVar14 = -1;
                                              local_58[0] = (undefined8 *******)0x0;
                                              local_48 = 0;
                                              local_40 = 0xf;
                                              do {
                                                lVar14 = lVar14 + 1;
                                              } while (
                                                  PTR_s___Data_XML_TradeRouteLines_xml_140a10818
                                                  [lVar14] != '\0');
                                              FUN_140022730(local_58);
                                              cVar3 = FUN_140467740(&DAT_140a29f20,local_58,0);
                                              if (0xf < local_40) {
                                                if (0xfff < local_40 + 1) {
                                                  if (0x1f < (ulonglong)
                                                             ((longlong)local_58[0] +
                                                             (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                    FUN_1407711cc(local_58[0][-1],local_40 + 0x28);
                                                  }
                                                }
                                                thunk_FUN_1407864b8();
                                              }
                                              DVar4 = timeGetTime();
                                              uVar6 = FUN_140216790();
                                              uVar7 = FUN_140216710();
                                              uVar18 = CONCAT44(uVar20,DVar4);
                                              FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                        
                                                  "TheTradeRouteLineManager.System_Initialize",uVar7
                                                  ,uVar6,uVar18);
                                              uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                              if (cVar3 == '\0') {
                                                pcVar11 = "Error: Unable to initialize FactionList."
                                                ;
                                              }
                                              else {
                                                if (DAT_140b31008 == 0) {
LAB_14004cc39:
                                                  bVar1 = false;
                                                }
                                                else {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_TargetingPrioritySetF_140a10858
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  uVar15 = 1;
                                                  cVar3 = FUN_140483f10(DAT_140b31008,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004cc39;
                                                  bVar1 = true;
                                                }
                                                if ((uVar15 != 0) && (0xf < local_40)) {
                                                  if (0xfff < local_40 + 1) {
                                                    if (0x1f < (ulonglong)
                                                               ((longlong)local_58[0] +
                                                               (-8 - (longlong)local_58[0][-1]))) {
                    /* WARNING: Subroutine does not return */
                                                      FUN_1407711cc(local_58[0][-1],local_40 + 0x28)
                                                      ;
                                                    }
                                                  }
                                                  thunk_FUN_1407864b8();
                                                }
                                                bVar2 = false;
                                                DVar4 = timeGetTime();
                                                uVar6 = FUN_140216790();
                                                uVar7 = FUN_140216710();
                                                uVar18 = CONCAT44(uVar20,DVar4);
                                                FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                            
                                                  "TheTargetingPrioritySetManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                if (bVar1) {
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_HardPointDataFiles_xm_140a10820
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404738b0(&DAT_140a2a0b0,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HardPointDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HardPointDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_1404814d0(
                                                  PTR_s___Data_XML_HeroClash_xml_140a10800);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "HeroClashTypeClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize HeroClashTypeClass.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140408830(&DAT_140b2f020,
                                                                                                                                                    
                                                  PTR_s___Data_XML_GameObjectFiles_xml_140a107e0);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "GameObjectTypeManager.Read_Parse_Game_Object_Databases"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize GameObjectTypeManager."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_TradeRouteFiles_xml_140a10810
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_1404715e0(&DAT_140a2a060,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TradeRouteManager.System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TradeRouteManager.";
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_CampaignFiles_xml_140a10828
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140479fe0(&DAT_140b31160,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "CampaignDataManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize CampaignDataManager."
                                                  ;
                                                  }
                                                  else {
                                                    cVar3 = FUN_14048f1b0(&LAB_1402e0550,
                                                                          FUN_1400f5200,
                                                                          FUN_1400d1f00);
                                                    DVar4 = timeGetTime();
                                                    uVar6 = FUN_140216790();
                                                    uVar7 = FUN_140216710();
                                                    uVar18 = CONCAT44(uVar20,DVar4);
                                                    FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                    
                                                  "SpecialAbilityClass::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (PTR_s___Data_XML_Movies_xml_140a10848
                                                           [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140480560(&DAT_140a2a810,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "MovieManager.System_Initialize",
                                                                uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_LightningEffectTypes__140a10850
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140482490(&DAT_140a2a850,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheLightningTypeManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  lVar14 = -1;
                                                  local_58[0] = (undefined8 *******)0x0;
                                                  local_48 = 0;
                                                  local_40 = 0xf;
                                                  do {
                                                    lVar14 = lVar14 + 1;
                                                  } while (
                                                  PTR_s___Data_XML_WeatherScenarios_xml_140a10868
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140489660(&DAT_140a2a980,local_58,0);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheWeatherScenarioManager.System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') goto LAB_14004b611;
                                                  uVar6 = FUN_14004fde0();
                                                  cVar3 = FUN_140477c10(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheMovementClassManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheMovementClassManager."
                                                  ;
                                                  goto LAB_14004b60c;
                                                  }
                                                  FUN_140248c60();
                                                  FUN_140248be0();
                                                  DAT_140b09d30 = 0;
                                                  if (DAT_140b31018 == 0) {
LAB_14004d373:
                                                    bVar1 = false;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_DifficultyAdjustments_140a10860
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  bVar2 = true;
                                                  cVar3 = FUN_140484e30(DAT_140b31018,local_58,0);
                                                  if (cVar3 == '\0') goto LAB_14004d373;
                                                  bVar1 = true;
                                                  }
                                                  if ((bVar2) && (0xf < local_40)) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheDifficultyAdjustmentManagerPtr->System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (!bVar1) goto LAB_14004b611;
                                                  DAT_140b31150 = &DAT_140a619ef;
                                                  uVar6 = FUN_14004fc30();
                                                  cVar3 = FUN_1404753b0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheAIDataManagerClass::Get().System_Initialize",
                                                  uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    FUN_1402149d0(
                                                  "Error: Unable to initialize TheAIDataManager.");
                                                  }
                                                  uVar6 = FUN_14004fd50();
                                                  cVar3 = FUN_1404889d0(uVar6);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheGameScoringManagerClass::Get().System_Initialize"
                                                  ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheGameScoringManagerClass."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_WeatherModifiers_xml_140a10840
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_14046a350(&DAT_140b30b40,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "TheWeatherSystem.System_Initialize"
                                                                ,uVar7,uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize TheWeatherSystem.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140488ec0(
                                                  PTR_s___Data_XML_LightSources_xml_140a107b8);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LightTypeManager::System_Initialize",uVar7,uVar6,
                                                  uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LightTypeManager.";
                                                  }
                                                  else {
                                                    cVar3 = FUN_140482d30(
                                                  PTR_s___Data_XML_LensFlares_xml_140a10778);
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "LensFlareTypeManager::System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 == '\0') {
                                                    pcVar11 = 
                                                  "Error: Unable to initialize LensFlareTypeManager."
                                                  ;
                                                  }
                                                  else {
                                                    lVar14 = -1;
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar14 = lVar14 + 1;
                                                    } while (
                                                  PTR_s___Data_XML_WeatherAudio_xml_140a107f8
                                                  [lVar14] != '\0');
                                                  FUN_140022730(local_58);
                                                  cVar3 = FUN_140324e70(&DAT_140b2b3b0,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar6 = FUN_140216790();
                                                  uVar7 = FUN_140216710();
                                                  uVar18 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                                                                                
                                                  "TheWeatherAudioManager.System_Initialize",uVar7,
                                                  uVar6,uVar18);
                                                  uVar20 = (undefined4)((ulonglong)uVar18 >> 0x20);
                                                  if (cVar3 != '\0') {
                                                    local_58[0] = (undefined8 *******)0x0;
                                                    local_48 = 0;
                                                    local_40 = 0xf;
                                                    do {
                                                      lVar16 = lVar16 + 1;
                                                    } while (PTR_s_BlackMarketItems_xml_140a10880
                                                             [lVar16] != '\0');
                                                    FUN_140022730(local_58,
                                                  PTR_s_BlackMarketItems_xml_140a10880,lVar16);
                                                  FUN_14048f5b0(&PTR_vftable_140a2aa78,local_58);
                                                  if (0xf < local_40) {
                                                    if (0xfff < local_40 + 1) {
                                                      if (0x1f < (ulonglong)
                                                                 ((longlong)local_58[0] +
                                                                 (-8 - (longlong)local_58[0][-1])))
                                                      {
                    /* WARNING: Subroutine does not return */
                                                        FUN_1407711cc(local_58[0][-1],
                                                                      local_40 + 0x28);
                                                      }
                                                    }
                                                    thunk_FUN_1407864b8();
                                                  }
                                                  DVar4 = timeGetTime();
                                                  uVar15 = FUN_140216790();
                                                  uVar6 = FUN_140216710();
                                                  uVar7 = CONCAT44(uVar20,DVar4);
                                                  FUN_140214a70("_MemLog.txt","%s,%d,%d,%d\r\n",
                                                                "BlackMarketItemList.Initialize",
                                                                uVar6,uVar15,uVar7);
                                                  FUN_140451cd0();
                                                  DAT_140b2b5d0 = &DAT_1409cf314;
                                                  FUN_1404737c0(&DAT_140a2a0b0);
                                                  FUN_140479400(&DAT_140b31160);
                                                  FUN_140330450(&DAT_140a172d0);
                                                  FUN_1402c79d0(&DAT_140b15800);
                                                  _guard_check_icall(&DAT_140b30160);
                                                  FUN_140483ea0(DAT_140b31008);
                                                  FUN_1403239f0(&DAT_140b2b3b0);
                                                  FUN_140084a70();
                                                  FUN_1400a9800();
                                                  FUN_140086ff0();
                                                  FUN_140105240();
                                                  FUN_140107b70();
                                                  FUN_14008e880();
                                                  FUN_140085380();
                                                  FUN_140463c80(FUN_1400dca70,&LAB_14004fe70);
                                                  FUN_14047cb40(&DAT_140b311a8);
                                                  FUN_1402a73a0();
                                                  _DAT_140b15480 = FUN_140056480;
                                                  uVar9 = CONCAT71((int7)((ulonglong)uVar7 >> 8),1);
                                                  local_res10 = (code *)0x3d99999a3c23d70a;
                                                  local_res18 = 0x3c23d70a;
                                                  local_res1c = 0x3d99999a;
                                                  FUN_140049110(&DAT_1409cf320,&local_res18,
                                                                &local_res10,0,
                                                                uVar15 & 0xffffffffffffff00,uVar9,
                                                                "Arial Bold",CONCAT44(uVar21,9));
                                                  local_res1c = DAT_1408022fc;
                                                  local_res10 = (code *)0x3c23d70a3ccccccd;
                                                  local_res18 = 0x3ccccccd;
                                                  FUN_140049110(&DAT_1409cf390,&local_res18,
                                                                &local_res10,1,1,
                                                                uVar9 & 0xffffffffffffff00,"Arial",7
                                                               );
                                                  FUN_140049270(&DAT_1409cf390,DAT_1408007b4,
                                                                DAT_1408022f8,30000,0x18);
                                                  DAT_140b15668 = FUN_140038100;
                                                  FUN_140297630(&DAT_140a16fd0,0);
                                                  FUN_140297630(&DAT_140a17008,0);
                                                  FUN_140293390(&DAT_140a17008,1);
                                                  FUN_1403d4490(&DAT_140b27f60);
                                                  return 1;
                                                  }
                                                  pcVar11 = 
                                                  "Error: Unable to initialize the TheWeatherAudioManager."
                                                  ;
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                                else {
                                                  pcVar11 = 
                                                  "Error: Unable to initialize TargetingPrioritySetManager."
                                                  ;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_14004b60c:
  FUN_1402149d0(pcVar11);
LAB_14004b611:
  FUN_14004da50();
  return 0;
}



// ------------------------------------------------------------
// RVA: 0x1098e0  Name: FUN_1401098e0  Size: 197 bytes
// ------------------------------------------------------------

void FUN_1401098e0(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  DAT_140a12221 = 0;
  return;
}



// ------------------------------------------------------------
// RVA: 0x1089e0  Name: FUN_1401089e0  Size: 1896 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401089e0(uint param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  longlong *plVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  longlong lVar9;
  undefined8 *******pppppppuVar10;
  longlong lVar11;
  char cVar12;
  undefined8 *puVar13;
  uint in_stack_ffffffffffffefa8;
  undefined8 ******local_1048;
  uint3 uStack_1040;
  char cStack_103d;
  undefined4 uStack_103c;
  ulonglong local_1038;
  ulonglong local_1030;
  short local_1028 [2048];
  
  if ((DAT_140a12224 != -1) && (DAT_140a12224 <= *(int *)(DAT_140a62328 + 0x10))) {
    puVar13 = (undefined8 *)((longlong)DAT_140a12224 * 0x70 + *(longlong *)(DAT_140a62328 + 8));
    *(undefined1 *)((longlong)puVar13 + 0x6d) = 1;
    puVar6 = puVar13;
    if (0xf < (ulonglong)puVar13[3]) {
      puVar6 = (undefined8 *)*puVar13;
    }
    plVar3 = (longlong *)FUN_140108430(puVar6,*(undefined4 *)(puVar13 + 0xd));
    cVar12 = '\x01';
    if (plVar3 != (longlong *)0x0) {
      lVar11 = *plVar3;
      uVar7 = (ulonglong)DAT_140a62340;
      if (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3)) {
        do {
          puVar6 = (undefined8 *)0x0;
          if (param_1 < DAT_140a6233c) {
            return;
          }
          if (cVar12 == '\0') {
            return;
          }
          puVar1 = *(undefined4 **)(lVar11 + uVar7 * 8);
          switch(*puVar1) {
          case 0:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = 0;
            FUN_1402d5010(&DAT_140b27e60,&local_1048,0,0,0);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
LAB_140108b67:
              thunk_FUN_1407864b8();
            }
            break;
          case 1:
            lVar11 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar11 = lVar11 + 1;
            } while (*(char *)(*(longlong *)(puVar1 + 2) + lVar11) != '\0');
            FUN_140022730(&local_1048);
            in_stack_ffffffffffffefa8 = in_stack_ffffffffffffefa8 & 0xffffff00;
            FUN_1402d2070(&DAT_140b27dc0,&local_1048,0,0,in_stack_ffffffffffffefa8);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            break;
          case 2:
            lVar11 = *(longlong *)(puVar1 + 2);
            FUN_140322780(&DAT_140b2b3a8);
            FUN_1403207c0(&DAT_140b2b2f0);
            FUN_140321830(&DAT_140b2b300,1);
            lVar9 = -1;
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0xf;
            do {
              lVar9 = lVar9 + 1;
            } while (*(char *)(lVar11 + lVar9) != '\0');
            FUN_140022730(&local_1048,lVar11);
            FUN_140320f80(&DAT_140b2b300,&local_1048,1);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              thunk_FUN_1407864b8();
            }
            DAT_140a12221 = '\0';
            break;
          case 3:
            uVar5 = CONCAT71((int7)((ulonglong)lVar11 >> 8),1);
            goto LAB_140108cd1;
          case 4:
            uVar5 = 0;
LAB_140108cd1:
            FUN_1402fc7a0(&DAT_140b27f60,*(undefined8 *)(puVar1 + 2),uVar5);
            break;
          case 6:
            FUN_1401099f0(*(undefined8 *)(puVar1 + 2));
            break;
          case 7:
            if ((puVar13[6] != 0) && (puVar6 = puVar13 + 4, 0xf < (ulonglong)puVar13[7])) {
              puVar6 = (undefined8 *)*puVar6;
            }
            plVar4 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            local_1048 = (undefined8 ******)0x0;
            local_1038 = 0;
            local_1030 = 0;
            uVar7 = plVar4[2];
            if (7 < (ulonglong)plVar4[3]) {
              plVar4 = (longlong *)*plVar4;
            }
            if (uVar7 < 8) {
              local_1048 = (undefined8 ******)*plVar4;
              _uStack_1040 = *(undefined4 *)(plVar4 + 1);
              uStack_103c = *(uint *)((longlong)plVar4 + 0xc);
              uVar8 = 7;
            }
            else {
              uVar8 = uVar7 | 7;
              if (0x7ffffffffffffffe < uVar8) {
                uVar8 = 0x7ffffffffffffffe;
              }
              local_1048 = (undefined8 ******)FUN_1400226c0(&local_1048,uVar8 + 1);
              FUN_14076bd80(local_1048,plVar4,uVar7 * 2 + 2);
            }
            local_1038 = uVar7;
            local_1030 = uVar8;
            if (puVar6 != (undefined8 *)0x0) {
              pppppppuVar10 = &local_1048;
              if (7 < uVar8) {
                pppppppuVar10 = (undefined8 *******)local_1048;
              }
              FUN_140022b30(local_1028,0x800,pppppppuVar10,puVar6);
              lVar11 = -1;
              do {
                lVar11 = lVar11 + 1;
              } while (local_1028[lVar11] != 0);
              FUN_1400228a0(&local_1048,local_1028);
            }
            FUN_140108720(&local_1048);
            if (7 < local_1030) {
              if (0xfff < local_1030 * 2 + 2) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 * 2 + 0x29);
                }
              }
              thunk_FUN_1407864b8();
            }
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 8:
            FUN_1401093b0(puVar13[0xc]);
            uStack_1040._0_1_ = s_IDC_STORY_RECAP_140818248[8];
            uStack_1040._1_1_ = s_IDC_STORY_RECAP_140818248[9];
            uStack_1040._2_1_ = s_IDC_STORY_RECAP_140818248[10];
            cStack_103d = s_IDC_STORY_RECAP_140818248[0xb];
            uStack_103c._0_3_ =
                 CONCAT12(s_IDC_STORY_RECAP_140818248[0xe],s_IDC_STORY_RECAP_140818248._12_2_);
            local_1030 = 0xf;
            local_1038 = 0xf;
            local_1048 = (undefined8 ******)s_IDC_STORY_RECAP_140818248._0_8_;
            uStack_103c = (uint)(uint3)uStack_103c;
            FUN_1401c4920(&DAT_140a78510,DAT_140a12218,&local_1048);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 9:
            local_1048 = (undefined8 ******)s_IDC_STATIC_1408181e0._0_8_;
            local_1030 = 0xf;
            local_1038 = 10;
            uStack_1040._2_1_ = '\0';
            uStack_1040._0_1_ = s_IDC_STATIC_1408181e0[8];
            uStack_1040._1_1_ = s_IDC_STATIC_1408181e0[9];
            uVar5 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar1 + 2));
            FUN_1401c8ea0(&DAT_140a78510,DAT_140a12218,&local_1048,uVar5);
            if (0xf < local_1030) {
              if (0xfff < local_1030 + 1) {
                if (0x1f < (ulonglong)((longlong)local_1048 + (-8 - (longlong)local_1048[-1]))) {
                    /* WARNING: Subroutine does not return */
                  FUN_1407711cc(local_1048[-1],local_1030 + 0x28);
                }
              }
              goto LAB_140108b67;
            }
            break;
          case 0xb:
            puVar2 = *(undefined1 **)(puVar1 + 2);
            in_stack_ffffffffffffefa8 = CONCAT31((int3)(in_stack_ffffffffffffefa8 >> 8),puVar2[0xc])
            ;
            FUN_1401f9470(&DAT_140a62348,*puVar2,puVar2[4],puVar2[8],in_stack_ffffffffffffefa8);
            break;
          case 0xc:
            FUN_1401091b0(**(undefined4 **)(puVar1 + 2));
            break;
          case 0xd:
            DAT_140a6233c = **(int **)(puVar1 + 2) + param_1;
            break;
          case 0x10:
            FUN_140109bf0();
            break;
          case 0x12:
            FUN_14030ac40(&DAT_140b27f60);
            break;
          case 0x13:
            cVar12 = DAT_140a12221;
            if (DAT_140a12221 != '\0') break;
            goto LAB_1401090cc;
          }
          DAT_140a62340 = DAT_140a62340 + 1;
LAB_1401090cc:
          lVar11 = *plVar3;
          uVar7 = (ulonglong)DAT_140a62340;
        } while (uVar7 < (ulonglong)(plVar3[1] - lVar11 >> 3));
      }
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x107900  Name: FUN_140107900  Size: 611 bytes
// ------------------------------------------------------------

void FUN_140107900(void)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  int iVar11;
  int iVar12;
  ulonglong uVar13;
  
  if (DAT_140a62320 != '\0') {
    FUN_1401c1d20(&DAT_140a78510,DAT_140a12218,1);
    DAT_140a12218 = 0xffffffff;
    uVar13 = (ulonglong)*(int *)(DAT_140a62328 + 0x10);
    iVar7 = *(int *)(DAT_140a62328 + 0x10) + -1;
    if (-1 < iVar7) {
      lVar9 = (longlong)iVar7 * 0x70;
      lVar4 = uVar13 * 0x70;
      lVar8 = DAT_140a62328;
      lVar10 = lVar9;
      do {
        lVar3 = *(longlong *)(lVar8 + 8);
        iVar12 = (int)uVar13;
        if ((((*(char *)(lVar9 + 0x6e + lVar3) != '\0') && (*(char *)(lVar9 + 0x6d + lVar3) != '\0')
             ) && (-1 < iVar7)) && (iVar11 = *(int *)(lVar8 + 0x10), iVar7 < iVar11)) {
          uVar5 = (iVar11 - iVar7) - 1;
          uVar13 = (ulonglong)uVar5;
          if (uVar5 != 0) {
            if ((*(uint *)(lVar8 + 0x14) & 0x80000000) == 0) {
              if (iVar12 < iVar7) {
                iVar6 = iVar7 + -1 + uVar5;
                if (0 < (int)uVar5) {
                  do {
                    iVar11 = iVar11 + -1;
                    lVar3 = (longlong)iVar6;
                    iVar6 = iVar6 + -1;
                    FUN_14010a170(lVar3 * 0x70 + *(longlong *)(lVar8 + 8),
                                  (longlong)iVar11 * 0x70 + *(longlong *)(lVar8 + 8));
                    uVar13 = uVar13 - 1;
                  } while (uVar13 != 0);
                }
              }
              else if (0 < (int)uVar5) {
                iVar11 = 0;
                do {
                  iVar2 = iVar12 + iVar11;
                  iVar6 = iVar11 + iVar7;
                  iVar11 = iVar11 + 1;
                  FUN_14010a170((longlong)iVar6 * 0x70 + *(longlong *)(lVar8 + 8),
                                (longlong)iVar2 * 0x70 + *(longlong *)(lVar8 + 8));
                } while (iVar11 < (int)uVar5);
              }
            }
            else {
              FUN_14076bd80(lVar3 + lVar10,lVar4 + lVar3,(longlong)(int)uVar5 * 0x70);
            }
          }
          *(int *)(lVar8 + 0x10) = *(int *)(lVar8 + 0x10) + -1;
          lVar8 = DAT_140a62328;
        }
        lVar4 = lVar4 + -0x70;
        lVar10 = lVar10 + -0x70;
        uVar13 = (ulonglong)(iVar12 - 1);
        lVar9 = lVar9 + -0x70;
        iVar7 = iVar7 + -1;
      } while (-1 < iVar7);
    }
    if (DAT_140a12221 == '\0') {
      FUN_140321830(&DAT_140b2b300,1);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
    }
    cVar1 = FUN_14028af60(&DAT_140b153e0);
    if (cVar1 == '\0') {
      iVar7 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar7 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (iVar7 == 0) {
        FUN_14028c720(&DAT_140b153e0,1);
      }
      else if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
    }
    FUN_14030ac40(&DAT_140b27f60);
    FUN_14032edd0(DAT_140b15418 + 0x14,"close_story_dialog");
    DAT_140a62320 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x105d30  Name: FUN_140105d30  Size: 190 bytes
// ------------------------------------------------------------

void FUN_140105d30(longlong param_1)

{
  longlong lVar1;
  longlong local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  FUN_140322780(&DAT_140b2b3a8);
  FUN_1403207c0(&DAT_140b2b2f0);
  FUN_140321830(&DAT_140b2b300,1);
  local_10 = 0xf;
  local_28[0] = 0;
  lVar1 = -1;
  local_18 = 0;
  do {
    lVar1 = lVar1 + 1;
  } while (*(char *)(param_1 + lVar1) != '\0');
  FUN_140022730(local_28,param_1);
  FUN_140320f80(&DAT_140b2b300,local_28,1);
  if (0xf < local_10) {
    if (0xfff < local_10 + 1) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x105190  Name: FUN_140105190  Size: 163 bytes
// ------------------------------------------------------------

void FUN_140105190(void)

{
  if (DAT_140a622e0 != '\0') {
    if (DAT_140a622f0 != 0) {
      FUN_140271a10();
    }
    if (DAT_140b30690 != 0) {
      FUN_14044f130(&DAT_140b306dd);
    }
    FUN_1401c1d20(&DAT_140a78510,DAT_140a121f0,1);
    DAT_140a121f0 = 0xffffffff;
    FUN_140321830(&DAT_140b2b300,1);
    FUN_140322780(&DAT_140b2b3a8);
    FUN_1403207c0(&DAT_140b2b2f0);
    FUN_14032edd0(DAT_140b15418 + 0xa0,"close_story_dialog");
    DAT_140a622e0 = '\0';
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x1053c0  Name: FUN_1401053c0  Size: 1988 bytes
// ------------------------------------------------------------

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */

void FUN_1401053c0(uint param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined4 *puVar3;
  ulonglong uVar4;
  undefined1 *puVar5;
  uint uVar6;
  uint uVar7;
  longlong *plVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  longlong lVar11;
  undefined8 *****pppppuVar12;
  longlong lVar13;
  undefined8 *puVar14;
  uint in_stack_ffffffffffffef98;
  undefined8 ****local_1058;
  undefined4 uStack_1050;
  uint3 uStack_104c;
  undefined1 uStack_1049;
  ulonglong local_1048;
  ulonglong local_1040;
  short local_1038 [2056];
  
  uVar7 = DAT_1407ffaf8;
  lVar1 = DAT_140a622e8[1];
  lVar2 = *DAT_140a622e8;
  while ((DAT_140a62304 < (uint)(lVar1 - lVar2 >> 3) && (DAT_140a62300 <= param_1))) {
    puVar3 = *(undefined4 **)(*DAT_140a622e8 + (ulonglong)DAT_140a62304 * 8);
    switch(*puVar3) {
    case 0:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = 0;
      FUN_1402d5010(&DAT_140b27e60,&local_1058,0,0,0);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 1:
      lVar13 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar13 = lVar13 + 1;
      } while (*(char *)(*(longlong *)(puVar3 + 2) + lVar13) != '\0');
      FUN_140022730(&local_1058);
      in_stack_ffffffffffffef98 = in_stack_ffffffffffffef98 & 0xffffff00;
      FUN_1402d2070(&DAT_140b27dc0,&local_1058,0,0,in_stack_ffffffffffffef98);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 2:
      lVar13 = *(longlong *)(puVar3 + 2);
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,1);
      lVar11 = -1;
      local_1058 = (undefined8 *****)0x0;
      local_1048 = 0;
      local_1040 = 0xf;
      do {
        lVar11 = lVar11 + 1;
      } while (*(char *)(lVar13 + lVar11) != '\0');
      FUN_140022730(&local_1058,lVar13);
      FUN_140320f80(&DAT_140b2b300,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 3:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),CONCAT71((uint7)(uint3)(DAT_140a62304 >> 8),1));
      break;
    case 4:
      FUN_140105e00(*(undefined8 *)(puVar3 + 2),0);
      break;
    case 5:
      if (DAT_140a622f0 != 0) {
        in_stack_ffffffffffffef98 = uVar7;
        FUN_140271700(DAT_140a622f0,**(undefined1 **)(puVar3 + 2),
                      CONCAT71((int7)((ulonglong)puVar3 >> 8),1),0,uVar7);
      }
      break;
    case 6:
      FUN_140105fd0(*(undefined8 *)(puVar3 + 2));
      break;
    case 7:
      plVar8 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      local_1058 = (undefined8 ****)0x0;
      local_1048 = 0;
      local_1040 = 0;
      uVar4 = plVar8[2];
      if (7 < (ulonglong)plVar8[3]) {
        plVar8 = (longlong *)*plVar8;
      }
      if (uVar4 < 8) {
        local_1058 = (undefined8 ****)*plVar8;
        uStack_1050 = *(undefined4 *)(plVar8 + 1);
        _uStack_104c = *(undefined4 *)((longlong)plVar8 + 0xc);
        uVar10 = 7;
      }
      else {
        uVar10 = uVar4 | 7;
        if (0x7ffffffffffffffe < uVar10) {
          uVar10 = 0x7ffffffffffffffe;
        }
        local_1058 = (undefined8 ****)FUN_1400226c0(&local_1058,uVar10 + 1);
        FUN_14076bd80(local_1058,plVar8,uVar4 * 2 + 2);
      }
      local_1048 = uVar4;
      local_1040 = uVar10;
      if (DAT_140a12208 != 0) {
        puVar14 = &DAT_140a121f8;
        if (7 < DAT_140a12210) {
          puVar14 = DAT_140a121f8;
        }
        pppppuVar12 = &local_1058;
        if (7 < uVar10) {
          pppppuVar12 = (undefined8 *****)local_1058;
        }
        FUN_140022b30(local_1038,0x800,pppppuVar12,puVar14);
        lVar13 = -1;
        do {
          lVar13 = lVar13 + 1;
        } while (local_1038[lVar13] != 0);
        FUN_1400228a0(&local_1058,local_1038);
      }
      FUN_140106360(&local_1058);
      if (7 < local_1040) {
        if (0xfff < local_1040 * 2 + 2) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 8:
      FUN_140106680();
      break;
    case 9:
      local_1040 = 0xf;
      local_1048 = 10;
      uStack_1050._2_1_ = '\0';
      uStack_1050._0_2_ = s_IDC_STATIC_1408181e0._8_2_;
      local_1058 = (undefined8 ****)s_IDC_STATIC_1408181e0._0_8_;
      uVar9 = FUN_1401fa460(&DAT_140a7bc58,*(undefined8 *)(puVar3 + 2));
      FUN_1401c8ea0(&DAT_140a78510,DAT_140a121f0,&local_1058,uVar9);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 10:
      local_1040 = 0xf;
      local_1048 = 0xe;
      uVar6 = (uint)_uStack_104c >> 0x18;
      uStack_1049 = (undefined1)uVar6;
      uStack_104c = (uint3)(ushort)s_IDC_STORY_TEXT_140818168._12_2_;
      local_1058 = (undefined8 ****)s_IDC_STORY_TEXT_140818168._0_8_;
      uStack_1050 = s_IDC_STORY_TEXT_140818168._8_4_;
      FUN_1401c40b0(&DAT_140a78510,DAT_140a121f0,&local_1058,1);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0xb:
      puVar5 = *(undefined1 **)(puVar3 + 2);
      in_stack_ffffffffffffef98 = CONCAT31((int3)(in_stack_ffffffffffffef98 >> 8),puVar5[0xc]);
      FUN_1401f9470(&DAT_140a62310,*puVar5,puVar5[4],puVar5[8],in_stack_ffffffffffffef98);
      break;
    case 0xc:
      FUN_140106bb0(**(undefined4 **)(puVar3 + 2));
      break;
    case 0xd:
      DAT_140a62300 = **(int **)(puVar3 + 2) + param_1;
      break;
    case 0xe:
      if (**(char **)(puVar3 + 2) == '\0') {
        (**(code **)(*DAT_140b15418 + 0x40))();
      }
      else {
        (**(code **)(*DAT_140b15418 + 0x38))();
      }
      break;
    case 0xf:
      FUN_140106e60(*(undefined8 *)(puVar3 + 2));
      break;
    case 0x10:
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._0_2_ = (ushort)(byte)s_IDC_IMAGE_1408002e8[8];
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      FUN_1401c3810(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_1058 = (undefined8 ****)s_IDC_IMAGE_1408002e8._0_8_;
      local_1040 = 0xf;
      local_1048 = 9;
      uStack_1050._1_1_ = '\0';
      uStack_1050._0_1_ = s_IDC_IMAGE_1408002e8[8];
      FUN_1401c38d0(&DAT_140a78510,DAT_140a121f0,&local_1058);
      if (0xf < local_1040) {
        if (0xfff < local_1040 + 1) {
          if (0x1f < (ulonglong)((longlong)local_1058 + (-8 - (longlong)local_1058[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_1058[-1],local_1040 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      break;
    case 0x11:
      if (DAT_140a622f0 != 0) {
        *(undefined1 *)(DAT_140a622f0 + 0x1d) = **(undefined1 **)(puVar3 + 2);
      }
      break;
    case 0x12:
      if (DAT_140b30690 != 0) {
        FUN_14044f130(&DAT_140b306dd);
      }
    }
    DAT_140a62304 = DAT_140a62304 + 1;
  }
  s_IDC_STORY_TEXT_140818168[8] = (char)s_IDC_STORY_TEXT_140818168._8_4_;
  s_IDC_STORY_TEXT_140818168[9] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,1);
  s_IDC_STORY_TEXT_140818168[10] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,2);
  s_IDC_STORY_TEXT_140818168[0xb] = SUB41(s_IDC_STORY_TEXT_140818168._8_4_,3);
  s_IDC_STORY_TEXT_140818168[0xc] = (char)s_IDC_STORY_TEXT_140818168._12_2_;
  s_IDC_STORY_TEXT_140818168[0xd] = (char)((ushort)s_IDC_STORY_TEXT_140818168._12_2_ >> 8);
  s_IDC_STATIC_1408181e0[8] = (char)s_IDC_STATIC_1408181e0._8_2_;
  s_IDC_STATIC_1408181e0[9] = (char)((ushort)s_IDC_STATIC_1408181e0._8_2_ >> 8);
  return;
}



// ------------------------------------------------------------
// RVA: 0x22f10  Name: FUN_140022f10  Size: 1749 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140022f10(int *param_1)

{
  char *pcVar1;
  char cVar2;
  undefined8 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  DWORD DVar8;
  undefined8 uVar9;
  longlong *plVar10;
  longlong lVar11;
  undefined8 *puVar12;
  longlong lVar13;
  undefined8 *****pppppuVar14;
  undefined8 *puVar15;
  uint uVar16;
  undefined8 *puVar17;
  undefined8 *puVar18;
  longlong *in_stack_fffffffffffffe50;
  undefined8 ****local_198;
  char local_190 [2];
  undefined1 local_18e;
  longlong local_188;
  ulonglong local_180;
  char *local_178 [2];
  undefined8 local_168;
  ulonglong local_160;
  undefined8 local_158;
  undefined4 local_150;
  undefined8 ****local_148 [2];
  longlong local_138;
  undefined8 local_130;
  char local_128 [8];
  char acStack_120 [4];
  char acStack_11c [4];
  char local_118;
  undefined1 local_117 [271];
  
  if (DAT_140b26fc0 != 0) {
    *(undefined1 *)(param_1 + 3) = 1;
    param_1[4] = DAT_140a13e10;
    DAT_140b27e40 = 1;
    DAT_140b27ef4 = 1;
    puVar17 = (undefined8 *)0x0;
    DAT_140a13e10 = 0;
    *param_1 = *param_1 + 1;
    iVar7 = *param_1;
    if (DAT_140b26fc0 <= *param_1) {
      *param_1 = 0;
      iVar7 = 0;
    }
    FUN_14001e680(local_148,(longlong)iVar7 * 0x20 + _DAT_140b26fb8);
    if (local_138 != 0) {
      puVar12 = puVar17;
      if (DAT_140a785a8 == 0) {
        uVar9 = FUN_1402c8210(&DAT_140b15800,DAT_140a7c090,DAT_140a13ed8);
        FUN_14001e680(&local_198,uVar9);
        if (local_188 == 0) {
          FUN_140022730(&local_198,"Splash.TGA",10);
        }
        pppppuVar14 = &local_198;
        if (0xf < local_180) {
          pppppuVar14 = (undefined8 *****)local_198;
        }
        plVar10 = (longlong *)FUN_14025e990(pppppuVar14,1,0);
        if (plVar10 != (longlong *)0x0) {
          (**(code **)*plVar10)(plVar10);
        }
        lVar11 = FUN_140769c58(0x90);
        if (lVar11 != 0) {
          in_stack_fffffffffffffe50 = plVar10;
          puVar12 = (undefined8 *)FUN_14025ff10(lVar11,0,0,DAT_1407ffaf8,DAT_1407ffaf8,plVar10);
        }
        if (plVar10 != (longlong *)0x0) {
          (**(code **)(*plVar10 + 8))(plVar10);
        }
        if (0xf < local_180) {
          if (0xfff < local_180 + 1) {
            if (0x1f < (ulonglong)((longlong)local_198 + (-8 - (longlong)local_198[-1]))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(local_198[-1],local_180 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        local_188 = 0;
        local_180 = 0xf;
        local_198 = (undefined8 ****)((ulonglong)local_198 & 0xffffffffffffff00);
      }
      thunk_FUN_14003f220(&DAT_140a60df0);
      FUN_1400337b0(&DAT_140a60df0);
      FUN_14028d940(&DAT_140b153e0);
      FUN_140297630(&DAT_140a16fd0,0);
      DAT_140b155e8 = 0;
      FUN_1400338e0(&DAT_140a60df0);
      FUN_140049350(&DAT_1409cf320);
      FUN_140049350(&DAT_1409cf390);
      FUN_1403027d0(&DAT_140b27f60);
      puVar3 = DAT_140b2b5e8;
      if (DAT_140b2b5f0 != 0) {
        if (DAT_140b2b5f0 < DAT_140b2b618 >> 3) {
          FUN_140023670(&DAT_140b2b5e0,*DAT_140b2b5e8,DAT_140b2b5e8);
        }
        else {
          *(undefined8 *)DAT_140b2b5e8[1] = 0;
          puVar3 = (undefined8 *)*puVar3;
          while (puVar3 != (undefined8 *)0x0) {
            puVar15 = (undefined8 *)*puVar3;
            thunk_FUN_1407864b8(puVar3,0x38);
            puVar3 = puVar15;
          }
          *DAT_140b2b5e8 = DAT_140b2b5e8;
          DAT_140b2b5e8[1] = DAT_140b2b5e8;
          puVar3 = DAT_140b2b5e8;
          DAT_140b2b5f0 = 0;
          puVar15 = (undefined8 *)
                    ((ulonglong)((longlong)DAT_140b2b600 + (7 - (longlong)DAT_140b2b5f8)) >> 3);
          if (DAT_140b2b600 < DAT_140b2b5f8) {
            puVar15 = puVar17;
          }
          puVar18 = DAT_140b2b5f8;
          if (puVar15 != (undefined8 *)0x0) {
            for (; puVar15 != (undefined8 *)0x0; puVar15 = (undefined8 *)((longlong)puVar15 + -1)) {
              *puVar18 = puVar3;
              puVar18 = puVar18 + 1;
            }
          }
        }
      }
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300);
      FUN_1402d2790(&DAT_140b27dc0);
      FUN_1402d6f20(&DAT_140b27e60);
      FUN_140323660(&DAT_140b2b3b0);
      local_118 = s___Data_Art_Maps__1407ffab8[0x10];
      local_128[0] = s___Data_Art_Maps__1407ffab8[0];
      local_128[1] = s___Data_Art_Maps__1407ffab8[1];
      local_128[2] = s___Data_Art_Maps__1407ffab8[2];
      local_128[3] = s___Data_Art_Maps__1407ffab8[3];
      local_128[4] = s___Data_Art_Maps__1407ffab8[4];
      local_128[5] = s___Data_Art_Maps__1407ffab8[5];
      local_128[6] = s___Data_Art_Maps__1407ffab8[6];
      local_128[7] = s___Data_Art_Maps__1407ffab8[7];
      acStack_120[0] = s___Data_Art_Maps__1407ffab8[8];
      acStack_120[1] = s___Data_Art_Maps__1407ffab8[9];
      acStack_120[2] = s___Data_Art_Maps__1407ffab8[10];
      acStack_120[3] = s___Data_Art_Maps__1407ffab8[0xb];
      acStack_11c[0] = s___Data_Art_Maps__1407ffab8[0xc];
      acStack_11c[1] = s___Data_Art_Maps__1407ffab8[0xd];
      acStack_11c[2] = s___Data_Art_Maps__1407ffab8[0xe];
      acStack_11c[3] = s___Data_Art_Maps__1407ffab8[0xf];
      FUN_14076c430(local_117,0,0xf3);
      pppppuVar14 = local_148;
      if (0xf < local_130) {
        pppppuVar14 = (undefined8 *****)local_148[0];
      }
      lVar11 = (longlong)&local_130 + 7;
      do {
        pcVar1 = (char *)(lVar11 + 1);
        lVar11 = lVar11 + 1;
      } while (*pcVar1 != '\0');
      lVar13 = 0;
      do {
        cVar2 = *(char *)((longlong)pppppuVar14 + lVar13);
        *(char *)(lVar11 + lVar13) = cVar2;
        lVar13 = lVar13 + 1;
      } while (cVar2 != '\0');
      FUN_1401fece0(CONCAT71((int7)((ulonglong)lVar11 >> 8),1));
      local_178[0] = (char *)0x0;
      local_168 = 0;
      local_160 = 0xf;
      lVar11 = -1;
      do {
        lVar13 = lVar11 + 1;
        lVar11 = lVar11 + 1;
      } while (local_128[lVar13] != '\0');
      FUN_140022730(local_178,local_128);
      FUN_140056480(local_178,0,0xffffffff,3,1);
      if (0xf < local_160) {
        if (0xfff < local_160 + 1) {
          if ((char *)0x1f < local_178[0] + (-8 - *(longlong *)(local_178[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_178[0] + -8),local_160 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      FUN_1401fece0(0);
      FUN_1402c1e30(&DAT_140b15690);
      FUN_1402c0ab0(&DAT_140b15690,local_148,0);
      if (puVar12 != (undefined8 *)0x0) {
        (**(code **)*puVar12)(puVar12,1);
      }
      FUN_1402a74f0(DAT_140b15418[3],0);
      FUN_1402a74f0(DAT_140b15418[4],0);
      if (DAT_140b15418 != (longlong *)0x0) {
        lVar11 = DAT_140b15418[0x12];
        lVar13 = (**(code **)(*DAT_140b15418 + 0x1e8))();
        if ((lVar11 != 0) && (lVar13 != 0)) {
          FUN_1402d03f0(lVar13,lVar11);
        }
      }
      local_190[0] = s_BasicRebel_1407ffad0[8];
      local_190[1] = s_BasicRebel_1407ffad0[9];
      local_198 = (undefined8 ****)s_BasicRebel_1407ffad0._0_8_;
      local_180 = 0xf;
      local_188 = 10;
      local_18e = 0;
      uVar9 = FUN_140294bc0(&DAT_140a16fd0,0);
      FUN_1402886d0(uVar9,&local_198);
      if (0xf < local_180) {
        if (0xfff < local_180 + 1) {
          if (0x1f < (ulonglong)((longlong)local_198 + (-8 - (longlong)local_198[-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_198[-1],local_180 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_178[0] = (char *)FUN_140769c58(0x20);
      uVar6 = s_AI_Player_Underworld_1407ffae0._12_4_;
      uVar5 = s_AI_Player_Underworld_1407ffae0._8_4_;
      uVar4 = s_AI_Player_Underworld_1407ffae0._4_4_;
      local_168 = 0x14;
      local_160 = 0x1f;
      *(undefined4 *)local_178[0] = s_AI_Player_Underworld_1407ffae0._0_4_;
      *(undefined4 *)(local_178[0] + 4) = uVar4;
      *(undefined4 *)(local_178[0] + 8) = uVar5;
      *(undefined4 *)(local_178[0] + 0xc) = uVar6;
      *(undefined4 *)(local_178[0] + 0x10) = s_AI_Player_Underworld_1407ffae0._16_4_;
      local_178[0][0x14] = '\0';
      uVar9 = FUN_140294bc0(&DAT_140a16fd0,6);
      FUN_1402886d0(uVar9,local_178);
      if (0xf < local_160) {
        if (0xfff < local_160 + 1) {
          if ((char *)0x1f < local_178[0] + (-8 - *(longlong *)(local_178[0] + -8))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_178[0] + -8),local_160 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_158 = 0;
      local_150 = 0;
      lVar11 = FUN_1402a9ff0(DAT_140b15418[3],0x1a,0xffffffff,0xffffffff,&local_158,
                             (ulonglong)in_stack_fffffffffffffe50 & 0xffffffff00000000,0x7fffffff,
                             0xffffffff);
      puVar12 = puVar17;
      if (0 < *(int *)(lVar11 + 0x10)) {
        do {
          uVar16 = (int)puVar17 + 1;
          puVar17 = (undefined8 *)(ulonglong)uVar16;
          *(undefined1 *)
           (*(longlong *)(*(longlong *)(lVar11 + 8) + -8 + (longlong)(puVar12 + 1)) + 0x3cb) = 1;
          puVar12 = puVar12 + 1;
        } while ((int)uVar16 < *(int *)(lVar11 + 0x10));
      }
      DAT_140b155c4 = 1;
      FUN_1402cca80(&DAT_140b27a10,1);
      FUN_1400c6c30();
      DVar8 = timeGetTime();
      param_1[1] = DVar8 - _DAT_140a7bec0;
    }
    if (0xf < local_130) {
      if (0xfff < local_130 + 1) {
        if (0x1f < (ulonglong)((longlong)local_148[0] + (-8 - (longlong)local_148[0][-1]))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(local_148[0][-1],local_130 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x60c60  Name: FUN_140060c60  Size: 3957 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

LRESULT FUN_140060c60(HWND param_1,uint param_2,ulonglong param_3,int *param_4)

{
  int iVar1;
  ulonglong uVar2;
  char cVar3;
  char cVar4;
  DWORD DVar5;
  undefined8 uVar6;
  longlong lVar7;
  LRESULT LVar8;
  short sVar9;
  int iVar10;
  uint uVar11;
  short sVar12;
  float fVar13;
  float fVar14;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88 [2];
  tagPAINTSTRUCT local_78;
  
  cVar3 = FUN_1401d4170();
  if (cVar3 != '\0') {
    return 1;
  }
  sVar9 = (short)param_4;
  uVar11 = (uint)param_4;
  sVar12 = (short)((ulonglong)param_4 >> 0x10);
  if (0x111 < param_2) {
    if (param_2 < 0x215) {
      if (param_2 == 0x214) {
        SetWindowPos(param_1,(HWND)0x0,0,0,param_4[2] - *param_4,param_4[3] - param_4[1],2);
        FUN_140304770(&DAT_140b27f60,0);
        return 0;
      }
      switch(param_2) {
      case 0x112:
        lVar7 = thunk_FUN_1401775f0(param_3);
        if (lVar7 != 0) {
          return 1;
        }
        break;
      case 0x200:
        if (DAT_1409cf314 == '\0') {
          return 0;
        }
        FUN_140022ea0(&DAT_1409cf080,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
        local_90._0_4_ = (float)(int)sVar9;
        local_90._4_4_ = (float)(int)sVar12;
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,0xb,&local_90);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,0xb,&local_90);
        if (cVar3 != '\0') {
          return 0;
        }
        if (DAT_140a785a8 == 0) {
          local_98._0_4_ = (float)local_90;
          local_98._4_4_ = local_90._4_4_;
          FUN_1402f6c30(&DAT_140b27f60,7,&local_98);
          return 0;
        }
        DAT_140b2a08c = (float)local_90;
        DAT_140b2a090 = local_90._4_4_;
        if (DAT_140a61eb9 != '\0') {
          FUN_1402e7430(&DAT_140b27f60);
          return 0;
        }
        return 0;
      case 0x201:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,2);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,2);
        if (cVar3 != '\0') {
          return 0;
        }
        fVar14 = (float)(int)(short)local_90;
        fVar13 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,0,0,local_88,0);
        cVar4 = FUN_140065d00();
        if ((cVar4 == '\0') && (cVar3 != '\0')) {
          return 0;
        }
        local_98 = CONCAT44(fVar13,fVar14);
        cVar3 = FUN_14004a260(&DAT_1409cf390,0,&local_98);
        if (cVar3 != '\0') {
          return 0;
        }
        local_98 = CONCAT44(fVar13,fVar14);
        cVar3 = FUN_1402f6c30(&DAT_140b27f60,0,&local_98);
        if (cVar3 == '\0') {
          local_98 = CONCAT44(fVar13,fVar14);
          FUN_140030fc0(&DAT_140a60df0,0,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x202:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        sVar9 = (short)local_90;
        uVar2 = (ulonglong)local_90 >> 0x10;
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,1,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          if ((DAT_140a60eec == '\0') && (DAT_140a60edf == '\0')) {
            local_98 = CONCAT44((float)(int)(short)uVar2,(float)(int)sVar9);
            cVar3 = FUN_1402f6c30(&DAT_140b27f60,1,&local_98);
            if (cVar3 != '\0') goto LAB_140061407;
          }
          FUN_140307860(&DAT_140b27f60,0,0);
        }
        else if (DAT_140a61eb9 == '\0') {
          if (DAT_140a62320 != '\0') {
            FUN_1402e7260(&DAT_140b27f60);
          }
        }
        else {
          FUN_1402e7350(&DAT_140b27f60);
        }
LAB_140061407:
        if (DAT_140a28c91 == '\0') {
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,1,&local_98,cVar3,0);
          return 0;
        }
        return 0;
      case 0x203:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,4);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,4);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,6,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,4,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,6,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x204:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,5);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,5);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,2,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,2,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,2,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x205:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,3,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          if ((DAT_140a60edf == '\0') && (DAT_140a60eec == '\0')) {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            cVar3 = FUN_1402f6c30(&DAT_140b27f60,3,&local_98);
            if (cVar3 != '\0') {
              return 0;
            }
          }
          FUN_140307860(&DAT_140b27f60,0,0);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          FUN_140030fc0(&DAT_140a60df0,3,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x206:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,7);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,7);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,7,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,5,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,7,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x207:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        SetCapture(param_1);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,8);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,8);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,4,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,4,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x208:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        ReleaseCapture();
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,5,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          local_98 = CONCAT44((float)(int)(short)((ulonglong)local_90 >> 0x10),
                              (float)(int)(short)local_90);
          FUN_140030fc0(&DAT_140a60df0,5,&local_98,0,0);
          return 0;
        }
        return 0;
      case 0x209:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        local_90 = CONCAT44(local_90._4_4_,uVar11);
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,10);
        if (cVar3 != '\0') {
          return 0;
        }
        cVar3 = FUN_1402cc3e0(&DAT_140b27a10,10);
        if (cVar3 != '\0') {
          return 0;
        }
        FUN_14026cb30(local_88,&local_90);
        cVar3 = FUN_1401c3660(&DAT_140a78510,8,0,local_88,0);
        if (cVar3 == '\0') {
          FUN_140065d00();
          sVar9 = (short)local_90;
          fVar14 = (float)(int)(short)((ulonglong)local_90 >> 0x10);
          local_98 = CONCAT44(fVar14,(float)(int)sVar9);
          cVar3 = FUN_1402f6c30(&DAT_140b27f60,6,&local_98);
          if (cVar3 == '\0') {
            local_98 = CONCAT44(fVar14,(float)(int)sVar9);
            FUN_140030fc0(&DAT_140a60df0,8,&local_98,0,0);
            return 0;
          }
          return 0;
        }
        return 0;
      case 0x20a:
        if (DAT_140a110d0 == '\0') {
          return 1;
        }
        sVar9 = (short)(param_3 >> 0x10);
        iVar10 = (int)sVar9;
        iVar1 = (iVar10 >> 0x1f & 1U) + 9;
        sVar12 = -(sVar9 / 0x78);
        if (-1 < sVar9) {
          sVar12 = sVar9 / 0x78;
        }
        local_90 = CONCAT44((float)iVar10,(float)iVar10);
        cVar3 = FUN_1401c3c40(&DAT_140a78510);
        if (cVar3 == '\x01') {
          local_98 = 0;
          cVar3 = FUN_1401c3660(&DAT_140a78510,iVar1,param_3 & 0xffffffff,&local_98,0);
          if (cVar3 == '\x01') {
            return 0;
          }
        }
        cVar3 = FUN_14044b3f0(&DAT_140a28c10,0xc,&local_90);
        if (cVar3 == '\0') {
          cVar3 = FUN_1402cc3e0(&DAT_140b27a10,0xc,&local_90);
          if (cVar3 == '\0') {
            local_88[0] = local_90;
            FUN_140030fc0(&DAT_140a60df0,iVar1,local_88,0,(int)sVar12);
            return 0;
          }
          return 0;
        }
        return 0;
      }
    }
    else {
      if (param_2 == 0x231) {
        thunk_FUN_1401772f0();
        return 0;
      }
      if (param_2 == 0x232) {
        thunk_FUN_1401774b0();
        FUN_140304770(&DAT_140b27f60,0);
        uVar6 = FUN_14004fba0();
        FUN_140262af0(uVar6);
        return 0;
      }
    }
switchD_140060df4_caseD_12:
    LVar8 = DefWindowProcA(param_1,param_2,param_3,(LPARAM)param_4);
    return LVar8;
  }
  if (param_2 == 0x111) {
    LVar8 = DefWindowProcA(param_1,0x111,param_3,(LPARAM)param_4);
    return LVar8;
  }
  if (param_2 < 0x10) {
    if (param_2 == 0xf) {
      BeginPaint(param_1,&local_78);
      EndPaint(param_1,&local_78);
      return 0;
    }
    if (param_2 != 2) {
      if (param_2 == 3) {
        return 0;
      }
      if (param_2 == 5) {
        thunk_FUN_140177350(param_3);
        GetWindowPlacement(param_1,(WINDOWPLACEMENT *)&local_78);
        if (DAT_140a61aac != '\0') {
          if (local_78.fErase == 3) {
            DAT_140a7c43c = 1;
          }
          else {
            if (local_78.fErase != 2) {
              DAT_140a7c35c = (int)sVar9;
              DAT_140a7c360 = (int)sVar12;
            }
            DAT_140a7c43c = 0;
          }
          FUN_140203330(&DAT_140a7c230);
        }
        FUN_1400c6b80();
        if (((((DAT_140a619ed != '\0') && (0 < sVar12)) && (0 < sVar9)) &&
            (((int)sVar9 != (uVar11 & 0xffff) &&
             ((int)sVar12 != ((uint)((ulonglong)param_4 >> 0x10) & 0xffff))))) &&
           ((ReleaseCapture(), DAT_140a61aac != '\0' && (local_78.fErase == 3)))) {
          SendMessageA(param_1,0x47,0,0);
        }
        if (DAT_1409cf314 != '\0') {
          uVar6 = FUN_14004fba0();
          FUN_140262af0(uVar6);
          return 0;
        }
        return 0;
      }
      goto switchD_140060df4_caseD_12;
    }
switchD_140060df4_caseD_10:
    ClipCursor((RECT *)0x0);
    PostQuitMessage(0);
    return 0;
  }
  switch(param_2) {
  case 0x10:
    goto switchD_140060df4_caseD_10;
  case 0x11:
    if (param_4 == (int *)0x1) {
      return 1;
    }
    break;
  default:
    goto switchD_140060df4_caseD_12;
  case 0x14:
    return 1;
  case 0x16:
    if (param_4 == (int *)0x1) {
      DAT_140a619ef = 1;
      return 0;
    }
    break;
  case 0x1c:
    if ((param_3 == 0) && (DAT_140b31208 == '\0')) {
      DAT_1409cf314 = 0;
      DAT_140a61b08 = 0;
      if (DAT_140a619ee == '\0') {
        FUN_14021ca20(&DAT_140a7d0a0);
        FUN_1402d7050(&DAT_140b27e60);
        FUN_1402d2830(&DAT_140b27dc0);
        FUN_1402d7050(&DAT_140b2b300);
        FUN_140320780(&DAT_140b2b3a8);
        FUN_140320780(&DAT_140b2b2f0);
        FUN_1402d7050(&DAT_140b2b3b0);
        FUN_14044de50(&DAT_140b306dd,1);
        DAT_140a619ee = '\x01';
      }
    }
    else {
      DAT_1409cf314 = 1;
      if (DAT_140a619ee == '\x01') {
        FUN_14021ca80(&DAT_140a7d0a0);
        FUN_1402d28e0(&DAT_140b27e60);
        FUN_1402d28e0(&DAT_140b27dc0);
        FUN_1402d28e0(&DAT_140b2b300);
        FUN_140320790(&DAT_140b2b3a8);
        FUN_140320790(&DAT_140b2b2f0);
        FUN_1402d28e0(&DAT_140b2b3b0);
        FUN_14044de50(&DAT_140b306dd,0);
        DAT_140a619ee = '\0';
      }
      if ((DAT_140a110d0 != '\0') &&
         ((DAT_140b15418 == 0 || (cVar3 = FUN_14028af60(&DAT_140b153e0), cVar3 == '\0')))) {
        DVar5 = timeGetTime();
        DAT_140a61b08 = DVar5 - _DAT_140a7bec0;
      }
    }
    FUN_140033690(&DAT_140a60df0);
    FUN_140022ee0(&DAT_1409cf080);
    return 0;
  case 0x100:
    goto switchD_140060df4_caseD_100;
  case 0x101:
    lVar7 = FUN_14001fb80();
    if (*(char *)(lVar7 + 1) != '\0') {
      return 1;
    }
    if (DAT_140a110d0 == '\0') {
      return 1;
    }
    cVar3 = FUN_1401c3c40(&DAT_140a78510);
    if (cVar3 == '\x01') {
      return 1;
    }
    cVar3 = FUN_14044b3f0(&DAT_140a28c10,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
    if (cVar3 == '\0') {
      cVar3 = FUN_1402cc080(&DAT_140b27a10,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
      if (cVar3 != '\0') {
        return 0;
      }
      cVar3 = FUN_1402f6bc0(&DAT_140b27f60,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
      if (cVar3 == '\0') {
        FUN_140033580(&DAT_140a60df0,param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
        return 0;
      }
      return 0;
    }
    return 0;
  case 0x102:
    if (DAT_140a110d0 != '\0') {
      local_90 = 0;
      FUN_1401c3660(&DAT_140a78510,0xc,param_3 & 0xffffffff,&local_90,0);
      return 0;
    }
    return 1;
  case 0x104:
    break;
  }
  if ((DAT_1409cf314 == '\0') ||
     ((cVar3 = FUN_14020e290(), cVar3 != '\0' && ((param_3 == 0x73 || (param_3 == 9)))))) {
    return 0;
  }
switchD_140060df4_caseD_100:
  if (((uVar11 >> 0x1e & 1) != 0) && (cVar3 = FUN_1401c3c40(&DAT_140a78510), cVar3 == '\0')) {
    return 1;
  }
  if (DAT_140a110d0 == '\0') {
    return 1;
  }
  FUN_140061fe0(param_3 & 0xffffffff,(ulonglong)param_4 & 0xffffffff);
  return 1;
}



// ------------------------------------------------------------
// RVA: 0x2b0520  Name: FUN_1402b0520  Size: 1163 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402b0520(longlong param_1,undefined4 param_2,undefined ***param_3,int param_4,int param_5,
                  longlong param_6,char param_7)

{
  undefined ***pppuVar1;
  undefined1 auVar2 [16];
  uint uVar3;
  int iVar4;
  longlong lVar5;
  HANDLE pvVar6;
  longlong lVar7;
  int iVar8;
  int iVar9;
  undefined8 *puVar10;
  undefined8 ******ppppppuVar11;
  ulonglong uVar12;
  undefined8 ******ppppppuVar13;
  LPVOID lpMem;
  longlong lVar14;
  uint uVar15;
  undefined8 ******ppppppuVar16;
  undefined4 *puVar17;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 uVar18;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_res10;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  uint local_74;
  undefined4 local_70;
  undefined8 *****local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  lVar5 = FUN_1402aca60();
  if (lVar5 == 0) {
    return;
  }
  puVar10 = (undefined8 *)(*(longlong *)(lVar5 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(lVar5 + 0x298) + 0x110)) {
    puVar10 = (undefined8 *)*puVar10;
  }
  FUN_140025760("GameObjectManagerClass::Land_Invade -- Invading planet %s\n",puVar10);
  ppppppuVar13 = (undefined8 ******)0x0;
  local_88 = DynamicVectorClass<int>::vftable;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  lVar14 = -1;
  local_74 = 0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 ******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  local_98 = param_2;
  local_94 = param_4;
  if (&local_88 != param_3) {
    uVar15 = *(uint *)((longlong)param_3 + 0x14);
    uVar3 = (int)(uVar15 * 2) >> 1;
    local_74 = uVar15;
    if (uVar3 != 0) {
      if ((int)uVar15 < 0) {
        pvVar6 = GetProcessHeap();
        local_80 = HeapAlloc(pvVar6,0,(longlong)(int)uVar3 * 4);
      }
      else {
        auVar2._8_8_ = 0;
        auVar2._0_8_ = (longlong)(int)uVar3;
        lVar7 = SUB168(ZEXT816(4) * auVar2,0);
        if (SUB168(ZEXT816(4) * auVar2,8) != 0) {
          lVar7 = lVar14;
        }
        local_80 = (LPVOID)thunk_FUN_140769c58(lVar7);
      }
      local_78 = *(uint *)(param_3 + 2);
      local_74 = (uVar15 ^ uVar3) & 0x7fffffff ^ uVar15;
      ppppppuVar11 = ppppppuVar13;
      ppppppuVar16 = ppppppuVar13;
      if (0 < (int)local_78) {
        do {
          uVar15 = (int)ppppppuVar16 + 1;
          *(undefined4 *)((longlong)ppppppuVar11 + (longlong)local_80) =
               *(undefined4 *)((longlong)ppppppuVar11 + (longlong)param_3[1]);
          local_78 = *(uint *)(param_3 + 2);
          ppppppuVar11 = (undefined8 ******)((longlong)ppppppuVar11 + 4);
          ppppppuVar16 = (undefined8 ******)(ulonglong)uVar15;
        } while ((int)uVar15 < (int)local_78);
      }
    }
  }
  local_90 = param_5;
  ppppppuVar11 = ppppppuVar13;
  local_res10 = param_2;
  if (param_6 != 0) {
    do {
      lVar14 = lVar14 + 1;
    } while (*(char *)(param_6 + lVar14) != '\0');
    FUN_140022730(local_68,param_6,lVar14);
    local_res10 = local_98;
    ppppppuVar11 = (undefined8 ******)local_68[0];
  }
  uVar15 = local_78;
  lpMem = local_80;
  iVar9 = local_90;
  if (local_90 == -1) {
    if (*(int *)(*(longlong *)(lVar5 + 0xb8) + 0x130) == 0) {
      lVar14 = FUN_1403956c0(lVar5);
      iVar9 = *(int *)(lVar14 + 0xc);
      local_90 = iVar9;
      if (((iVar9 == -1) ||
          (lVar14 = FUN_140294aa0(&DAT_140a16fd0), iVar9 == *(int *)(lVar14 + 0x4c))) &&
         (lVar14 = FUN_140397120(lVar5), *(int *)(lVar14 + 0x10) != 0)) {
        iVar9 = *(int *)(**(longlong **)(lVar14 + 8) + 0x58);
        local_90 = iVar9;
      }
    }
    else {
      iVar9 = *(int *)(*(longlong *)(lVar5 + 0xb8) + 0x124);
      local_90 = iVar9;
    }
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar18 = extraout_XMM0_Qa;
  if (param_4 == iVar4) {
    lVar14 = FUN_140294a40(&DAT_140a16fd0);
    lVar14 = *(longlong *)(*(longlong *)(lVar14 + 0x68) + 0x640);
    uVar18 = extraout_XMM0_Qa_00;
    if (DAT_140b155d4 == param_4) {
      lVar5 = FUN_140371450(*(undefined8 *)(lVar5 + 0x298),DAT_140b155d8);
      if (lVar5 != 0) {
        FUN_140322780(&DAT_140b2b3a8);
        FUN_1403207c0(&DAT_140b2b2f0);
        FUN_140321830(&DAT_140b2b300,0);
        uVar18 = FUN_140321170(&DAT_140b2b300,lVar5);
        goto LAB_1402b07ed;
      }
      lVar5 = FUN_140294a40(&DAT_140a16fd0);
      lVar14 = *(longlong *)(*(longlong *)(lVar5 + 0x68) + 0x650);
      uVar18 = extraout_XMM0_Qa_01;
    }
    if (lVar14 != 0) {
      uVar18 = FUN_1402d5290(&DAT_140b27e60,lVar14,0,0,0);
    }
  }
LAB_1402b07ed:
  if (local_94 != iVar9) {
    if (param_7 == '\0') {
      iVar4 = *(int *)(param_1 + 0x490);
      iVar8 = *(int *)(param_1 + 0x494) * 2 >> 1;
      if (iVar8 <= iVar4) {
        iVar4 = iVar8 / 10;
        if (iVar8 / 10 < 10) {
          iVar4 = 10;
        }
        uVar18 = FUN_1402b6fa0(param_1 + 0x480,iVar4 + iVar8,0,1);
        iVar4 = *(int *)(param_1 + 0x490);
      }
      puVar17 = (undefined4 *)((longlong)iVar4 * 0x58 + *(longlong *)(param_1 + 0x488));
      *(int *)(param_1 + 0x490) = iVar4 + 1;
      *puVar17 = local_res10;
      pppuVar1 = (undefined ***)(puVar17 + 4);
      puVar17[1] = local_94;
      puVar17[2] = iVar9;
      if (pppuVar1 != &local_88) {
        uVar18 = FUN_14001f910(pppuVar1);
        puVar17[9] = local_74;
        iVar9 = (int)(local_74 * 2) >> 1;
        if (iVar9 != 0) {
          uVar18 = FUN_140021ec0(pppuVar1,iVar9,0,1);
          if (0 < (int)uVar15) {
            uVar12 = (ulonglong)uVar15;
            do {
              *(undefined4 *)((longlong)ppppppuVar13 + *(longlong *)(puVar17 + 6)) =
                   *(undefined4 *)((longlong)ppppppuVar13 + (longlong)lpMem);
              uVar12 = uVar12 - 1;
              ppppppuVar13 = (undefined8 ******)((longlong)ppppppuVar13 + 4);
            } while (uVar12 != 0);
          }
          puVar17[8] = uVar15;
        }
      }
      puVar17[10] = local_70;
      if ((undefined8 ******)(puVar17 + 0xc) != local_68) {
        ppppppuVar13 = local_68;
        if (0xf < uStack_50) {
          ppppppuVar13 = ppppppuVar11;
        }
        uVar18 = FUN_140022730(puVar17 + 0xc,ppppppuVar13,local_58);
        lpMem = local_80;
        ppppppuVar11 = (undefined8 ******)local_68[0];
      }
      *(undefined1 *)(puVar17 + 0x14) = local_48;
    }
    else {
      uVar18 = FUN_14029bb40(param_1 + 0x480,&local_98);
      lpMem = local_80;
      ppppppuVar11 = (undefined8 ******)local_68[0];
    }
  }
  if (0xf < uStack_50) {
    ppppppuVar13 = ppppppuVar11;
    if (0xfff < uStack_50 + 1) {
      ppppppuVar13 = (undefined8 ******)ppppppuVar11[-1];
      if (0x1f < (ulonglong)((longlong)ppppppuVar11 + (-8 - (longlong)ppppppuVar13))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar18,uStack_50 + 0x28);
      }
    }
    thunk_FUN_1407864b8(ppppppuVar13);
  }
  if (lpMem != (LPVOID)0x0) {
    if ((local_74 & 0x80000000) == 0) {
      thunk_FUN_1407864b8(lpMem);
    }
    else {
      pvVar6 = GetProcessHeap();
      HeapFree(pvVar6,0,lpMem);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x7ef580  Name: FUN_1407ef580  Size: 35 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1407ef580(void)

{
  _DAT_140b2b2f0 = 0;
  _DAT_140b2b2e0 = 0;
  _DAT_140b2b2e8 = 0;
  _DAT_140a171f0 = 0xffffffff;
  return;
}




// ================================================================
// POST-SCENE-MANAGER GAME-ACTIVE CALLEES
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2ee40  Name: FUN_14002ee40  Size: 4453 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14002ee40(longlong *param_1,int param_2)

{
  float *pfVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  undefined4 uVar5;
  undefined8 uVar6;
  HWND hWnd;
  longlong *plVar7;
  longlong *plVar8;
  longlong lVar9;
  longlong lVar10;
  undefined4 *puVar11;
  longlong *plVar12;
  float fVar13;
  float fVar14;
  double dVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined8 local_res18;
  float local_res20;
  float local_res24;
  ulonglong in_stack_ffffffffffffff28;
  undefined1 *puVar20;
  float local_c8;
  float fStack_c4;
  undefined8 local_c0;
  float local_b8;
  undefined1 local_b0 [8];
  float local_a8;
  undefined1 local_a0 [12];
  float local_94;
  POINT local_90;
  tagPOINT local_88;
  longlong local_80 [11];
  
  cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
  if ((cVar2 == '\x01') || (DAT_140a28c30 != '\0')) {
    if (DAT_140a785a8 == 0) {
      FUN_1403b1660(&DAT_140a286f0);
    }
    FUN_14003f220(param_1);
    FUN_1400370d0(param_1);
    FUN_14003d950(param_1);
    if (((DAT_140b15418 != (longlong *)0x0) &&
        (cVar2 = (**(code **)(*DAT_140b15418 + 0xf8))(), cVar2 != '\x01')) &&
       (DAT_140b15418 != (longlong *)0x0)) {
      (**(code **)(*DAT_140b15418 + 0xf0))();
    }
    FUN_14003cad0(param_1);
    return;
  }
  if (DAT_140b15418 == (longlong *)0x0) {
    return;
  }
  if ((DAT_140b15418[5] != 0) && (cVar2 = FUN_140342dc0(), cVar2 == '\x01')) {
    FUN_14003f220(param_1);
    FUN_1400370d0(param_1);
    FUN_14003d950(param_1);
    FUN_14003e8b0(param_1);
    FUN_140038930(param_1);
  }
  if (DAT_140a28c91 != '\0') {
    return;
  }
  iVar3 = param_2 - *(int *)((longlong)param_1 + 0x8dc);
  *(int *)((longlong)param_1 + 0x8dc) = param_2;
  if (*(char *)((longlong)param_1 + 0xee) == '\x01') {
    *(int *)(param_1 + 0x1e) = (int)param_1[0x1e] + iVar3;
  }
  else {
    *(undefined4 *)(param_1 + 0x1e) = 0;
  }
  if ((char)param_1[0x22] == '\x01') {
    *(int *)((longlong)param_1 + 0x114) = *(int *)((longlong)param_1 + 0x114) + iVar3;
  }
  else {
    *(undefined4 *)((longlong)param_1 + 0x114) = 0;
  }
  if (*(char *)((longlong)param_1 + 0xfc) == '\x01') {
    if (*(char *)((longlong)param_1 + 0xec) == '\x01') {
      cVar2 = FUN_14020e2b0();
      if (cVar2 == '\x01') {
        FUN_140039220(param_1);
        if (DAT_140b15418 != (longlong *)0x0) {
          if (*(char *)((longlong)DAT_140b15418 + 0x2d1) == '\x01') goto LAB_14002efeb;
          cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))();
          if (cVar2 == '\x01') {
            uVar6 = (**(code **)(*DAT_140b15418 + 0x1e8))();
            FUN_1402cf150(uVar6);
          }
        }
        *(undefined1 *)((longlong)param_1 + 0xed) = 1;
      }
    }
    else if ((*(char *)((longlong)param_1 + 0xed) == '\x01') &&
            (cVar2 = FUN_14020e2b0(), cVar2 == '\0')) {
      if ((DAT_140b15418 != (longlong *)0x0) &&
         (cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))(), cVar2 == '\x01')) {
        uVar6 = (**(code **)(*DAT_140b15418 + 0x1e8))();
        FUN_1402cf160(uVar6);
      }
      *(undefined1 *)((longlong)param_1 + 0xed) = 0;
      FUN_140039170(param_1);
    }
  }
LAB_14002efeb:
  local_a0._0_4_ = 0x18;
  local_a0._4_4_ = 0;
  BVar4 = GetCursorInfo((PCURSORINFO)local_a0);
  if (BVar4 == 0) {
    local_88.x = 0;
    local_88.y = 0;
  }
  else {
    local_88.x = local_90.x;
    local_88.y = local_90.y;
  }
  hWnd = (HWND)FUN_14025ea40();
  ScreenToClient(hWnd,&local_88);
  local_res20 = (float)local_88.x;
  local_res24 = (float)local_88.y;
  local_c8 = (float)*(int *)((longlong)param_1 + 0xdc);
  fStack_c4 = (float)(int)param_1[0x1c];
  FUN_140040430(param_1,&local_res20);
  FUN_140040aa0(param_1,&local_res20,0);
  if (((char)param_1[0x24] == '\x01') && (*(char *)((longlong)param_1 + 0x111) == '\x01')) {
    dVar15 = (double)FUN_1407760bc((double)(*(float *)(param_1 + 0x23) - local_res20));
    dVar16 = (double)FUN_1407760bc((double)(*(float *)((longlong)param_1 + 0x11c) - local_res24));
    if ((float)dVar15 <= (float)dVar16) {
      fVar13 = *(float *)((longlong)param_1 + 0x11c) - local_res24;
    }
    else {
      fVar13 = *(float *)(param_1 + 0x23) - local_res20;
    }
    dVar15 = (double)FUN_1407760bc((double)fVar13);
    if ((DAT_140b169f4 < (float)dVar15) && (499 < *(uint *)((longlong)param_1 + 0x114))) {
      if (DAT_140b169f8 <= (float)dVar15) {
        FUN_1400403f0(param_1);
        FUN_140041680(param_1);
      }
      else {
        FUN_140040040(param_1,param_1 + 0x23,&local_res20);
        FUN_140040640(param_1,param_1 + 0x23,&local_res20);
      }
    }
  }
  if ((((*(char *)((longlong)param_1 + 0x121) == '\x01') &&
       (*(char *)((longlong)param_1 + 0xef) == '\x01')) && (*param_1 == 0)) && (param_1[1] == 0)) {
    dVar15 = (double)FUN_1407760bc((double)(*(float *)((longlong)param_1 + 0xf4) - local_res20));
    dVar16 = (double)FUN_1407760bc((double)(*(float *)(param_1 + 0x1f) - local_res24));
    if ((float)dVar15 <= (float)dVar16) {
      fVar13 = *(float *)(param_1 + 0x1f) - local_res24;
    }
    else {
      fVar13 = *(float *)((longlong)param_1 + 0xf4) - local_res20;
    }
    dVar15 = (double)FUN_1407760bc((double)fVar13);
    if ((DAT_140b169f4 < (float)dVar15) && (499 < *(uint *)(param_1 + 0x1e))) {
      FUN_140040040(param_1,(longlong)param_1 + 0xf4,&local_res20);
      FUN_140040640(param_1,(longlong)param_1 + 0xf4,&local_res20);
    }
  }
  uVar5 = (undefined4)(in_stack_ffffffffffffff28 >> 0x20);
  if ((local_res20 != (float)*(int *)((longlong)param_1 + 0xdc)) ||
     (local_res24 != (float)(int)param_1[0x1c])) {
    ((POINT *)((longlong)param_1 + 0xdc))->x = local_88.x;
    ((POINT *)((longlong)param_1 + 0xdc))->y = local_88.y;
    local_c8 = (float)*(int *)((longlong)param_1 + 0xdc);
    in_stack_ffffffffffffff28 = in_stack_ffffffffffffff28 & 0xffffffff00000000;
    fStack_c4 = (float)(int)param_1[0x1c];
    local_res18 = CONCAT44(fStack_c4,local_c8);
    FUN_140030fc0(param_1,0xb,&local_res18,0,in_stack_ffffffffffffff28);
    uVar5 = (undefined4)(in_stack_ffffffffffffff28 >> 0x20);
  }
  if ((*(char *)((longlong)param_1 + 0xef) != '\0') && (param_1[1] != 0)) {
    FUN_1401f9320(local_a0,&DAT_140a61780);
    iVar3 = FUN_1401ffb40(&DAT_140a13e20,0,0xc);
    local_94 = local_94 + (float)iVar3 / DAT_1408007f0;
    (**(code **)(*(longlong *)param_1[1] + 0x28))((longlong *)param_1[1],local_a0);
    FUN_1401f9440(local_a0);
  }
  fVar13 = DAT_1407ffaf8;
  if (DAT_140b15418 == (longlong *)0x0) {
LAB_14002fca6:
    if (DAT_140a61eb9 != '\0') {
      FUN_1402e7430(&DAT_140b27f60);
    }
  }
  else {
    if ((char)DAT_140b15418[0x13] != '\0') {
      if (*(char *)((longlong)DAT_140b15418 + 0x2d1) == '\0') {
        cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))();
        if (cVar2 == '\x01') {
          FUN_14003c360(param_1,&local_res18,&local_c8,param_2);
          fVar19 = local_res18._4_4_;
          if (((float)local_res18 == 0.0) && (local_res18._4_4_ == 0.0)) {
            if ((*(float *)(param_1 + 6) != 0.0) || (*(float *)((longlong)param_1 + 0x34) != 0.0)) {
              fVar17 = (fVar13 / _DAT_140b15a9c) * *(float *)(param_1 + 6);
              fVar14 = (fVar13 / _DAT_140b15a9c) * *(float *)((longlong)param_1 + 0x34);
              local_res18 = CONCAT44(local_res18._4_4_,fVar17) & 0xffffffff7fffffff;
              *(float *)(param_1 + 6) = fVar17;
              fVar18 = DAT_1408007b4;
              *(float *)((longlong)param_1 + 0x34) = fVar14;
              if ((ABS(fVar17) < fVar18) &&
                 (local_res18 = CONCAT44(fVar19,fVar14) & 0xffffffff7fffffff, ABS(fVar14) < fVar18))
              {
                param_1[6] = 0;
              }
            }
          }
          else {
            fVar18 = DAT_140b15aa0 * (float)local_res18 + *(float *)(param_1 + 6);
            fVar19 = DAT_140b15aa0 * local_res18._4_4_ + *(float *)((longlong)param_1 + 0x34);
            *(float *)(param_1 + 6) = fVar18;
            *(float *)((longlong)param_1 + 0x34) = fVar19;
            if ((float)local_res18 < 0.0) {
              if (fVar18 <= (float)local_res18) {
                fVar18 = (float)local_res18;
              }
            }
            else if ((float)local_res18 <= fVar18) {
              fVar18 = (float)local_res18;
            }
            *(float *)(param_1 + 6) = fVar18;
            if (local_res18._4_4_ < 0.0) {
              if (fVar19 <= local_res18._4_4_) {
                fVar19 = local_res18._4_4_;
              }
            }
            else if (local_res18._4_4_ <= fVar19) {
              fVar19 = local_res18._4_4_;
            }
            *(float *)((longlong)param_1 + 0x34) = fVar19;
          }
          uVar6 = (**(code **)(*DAT_140b15418 + 0x1e8))();
          FUN_1402d0820(uVar6);
          *(int *)(param_1 + 0x1b) = param_2;
        }
        else {
          if (DAT_140b15418 == (longlong *)0x0) goto LAB_14002fca6;
          cVar2 = (**(code **)(*DAT_140b15418 + 0xf8))();
          if (cVar2 == '\x01') {
            FUN_14003c360(param_1,&local_res18,&local_c8,param_2);
            fVar19 = local_res18._4_4_;
            if (((float)local_res18 == 0.0) && (local_res18._4_4_ == 0.0)) {
              pfVar1 = (float *)(param_1 + 6);
              if ((*(float *)(param_1 + 6) != 0.0) || (*(float *)((longlong)param_1 + 0x34) != 0.0))
              {
                fVar14 = fVar13 / _DAT_140b15a9c;
                fVar17 = fVar14 * *pfVar1;
                local_res18 = CONCAT44(local_res18._4_4_,fVar17) & 0xffffffff7fffffff;
                *pfVar1 = fVar17;
                fVar18 = DAT_1408007b4;
                *(float *)((longlong)param_1 + 0x34) = fVar14 * *(float *)((longlong)param_1 + 0x34)
                ;
                if ((ABS(fVar17) < fVar18) &&
                   (local_res18 = CONCAT44(fVar19,*(float *)((longlong)param_1 + 0x34)) &
                                  0xffffffff7fffffff,
                   ABS(*(float *)((longlong)param_1 + 0x34)) < fVar18)) {
                  *pfVar1 = 0.0;
                  *(undefined4 *)((longlong)param_1 + 0x34) = 0;
                }
              }
            }
            else {
              pfVar1 = (float *)(param_1 + 6);
              fVar18 = DAT_140b15aa0 * local_res18._4_4_;
              fVar19 = DAT_140b15aa0 * (float)local_res18 + *pfVar1;
              *pfVar1 = fVar19;
              *(float *)((longlong)param_1 + 0x34) = fVar18 + *(float *)((longlong)param_1 + 0x34);
              if ((float)local_res18 < 0.0) {
                if (fVar19 <= (float)local_res18) {
                  fVar19 = (float)local_res18;
                }
              }
              else if ((float)local_res18 <= fVar19) {
                fVar19 = (float)local_res18;
              }
              fVar18 = *(float *)((longlong)param_1 + 0x34);
              *pfVar1 = fVar19;
              if (local_res18._4_4_ < 0.0) {
                if (fVar18 <= local_res18._4_4_) {
                  fVar18 = local_res18._4_4_;
                }
              }
              else if (local_res18._4_4_ <= fVar18) {
                fVar18 = local_res18._4_4_;
              }
              *(float *)((longlong)param_1 + 0x34) = fVar18;
            }
            FUN_1403c3210(DAT_140b2eea0);
            *(int *)(param_1 + 0x1b) = param_2;
            goto LAB_14002f5f3;
          }
        }
      }
      else {
        cVar2 = (**(code **)(*DAT_140b15418 + 0xf8))();
        if (cVar2 == '\x01') {
LAB_14002f5f3:
          FUN_1403c3bb0(DAT_140b2eea0,param_2);
        }
      }
    }
    if (((DAT_140b15418 == (longlong *)0x0) || ((char)DAT_140b15418[0x13] == '\0')) ||
       (cVar2 = FUN_1401c3c40(&DAT_140a78510), cVar2 != '\0')) goto LAB_14002fca6;
    plVar8 = (longlong *)param_1[0x1a];
    local_80[0] = 0;
    if (plVar8 != (longlong *)0x0) {
      uVar6 = (**(code **)(*plVar8 + 0x10))(plVar8,3);
      FUN_1403fd890(uVar6,param_1[0x1a],0);
      param_1[0x1a] = 0;
    }
    local_c0 = CONCAT44(fStack_c4,local_c8);
    FUN_1402fbc70(&DAT_140b27f60,&local_c0,local_80);
    if (local_80[0] == 0) {
      FUN_1402e32f0(&DAT_140b27f60);
      local_c0 = CONCAT44(fStack_c4,local_c8);
      local_80[1] = 0;
      plVar7 = (longlong *)FUN_140036600(param_1,&local_c0,local_80 + 1,0);
      plVar8 = (longlong *)param_1[300];
      if (plVar7 != plVar8) {
        if (plVar8 != (longlong *)0x0) {
          FUN_140044f00(param_1,plVar8,0);
        }
        if (plVar7 == (longlong *)0x0) {
          param_1[300] = 0;
        }
        else {
          param_1[300] = (longlong)plVar7;
          FUN_140044f00(param_1,plVar7,fVar13);
        }
      }
      DAT_140b27d98 = plVar7;
      _DAT_140b29f90 = plVar7;
      plVar8 = (longlong *)FUN_14003fe80(param_1,&local_c8);
      if (plVar8 != (longlong *)0x0) {
        plVar7 = (longlong *)plVar8[2];
      }
      if (plVar7 == (longlong *)0x0) {
        if (DAT_140a625a0 == '\0') {
          FUN_1402f4940(&DAT_140b27f60,1,0);
        }
LAB_14002f776:
        FUN_1403b1660(&DAT_140a286f0);
        FUN_14003f220(param_1);
      }
      else {
        cVar2 = FUN_14039b070(plVar7);
        if ((cVar2 == '\0') && (*(char *)((longlong)plVar7 + 0x39a) == -1)) goto LAB_14002f776;
        plVar12 = (longlong *)0x0;
        if ((plVar8 != (longlong *)0x0) &&
           ((((lVar9 = plVar8[4], lVar9 != 0 && (*(char *)(lVar9 + 0x4d) == '\x01')) &&
             (*(float *)(plVar8 + 5) <= 0.0)) || (plVar12 = plVar8, *(char *)(lVar9 + 0x4c) == '\0')
            ))) {
          plVar12 = (longlong *)0x0;
        }
        if ((DAT_140b15418 == (longlong *)0x0) ||
           (cVar2 = (**(code **)(*DAT_140b15418 + 0xf8))(), cVar2 == '\0')) {
          plVar8 = (longlong *)plVar7[0x56];
          if ((plVar8 == (longlong *)0x0) || ((char)plVar8[0x69] == -1)) {
            plVar8 = plVar7;
          }
          FUN_1403b1580(&DAT_140a286f0,plVar8,plVar12);
          FUN_1402e2910(&DAT_140b27f60,plVar7,0);
          FUN_14003ed90(param_1,plVar7);
        }
        else {
          if (*(char *)((longlong)plVar7 + 0x335) == -1) {
            if (*(char *)((longlong)plVar7 + 0x336) == -1) goto LAB_14002f876;
          }
          else {
            uVar6 = (**(code **)(*plVar7 + 0x10))(plVar7,3);
            FUN_1403fd890(uVar6,plVar7,1);
            param_1[0x1a] = (longlong)plVar7;
          }
          FUN_1402e2910(&DAT_140b27f60,plVar7,0);
        }
      }
LAB_14002f876:
      FUN_1403b21d0(&DAT_140a287c8,plVar7,&local_c8);
    }
    else {
      FUN_1402e32f0(&DAT_140b27f60);
      FUN_1403b1660(&DAT_140a286f0);
      FUN_14003f220(param_1);
      plVar7 = (longlong *)0x0;
    }
    local_b0._0_4_ = 0;
    local_b0._4_4_ = 0;
    local_a8 = 0.0;
    iVar3 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar3 == 1) {
      FUN_1403bb2d0(DAT_140b15418,DAT_140b0a158,DAT_140b0a15c,local_b0);
    }
    else {
      iVar3 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (iVar3 == 2) {
        puVar20 = local_a0;
        FUN_140261a70(DAT_140b15418[0x12],DAT_140b0a158,DAT_140b0a15c,&local_c0,puVar20);
        uVar5 = (undefined4)((ulonglong)puVar20 >> 0x20);
        local_b0._4_4_ =
             (((local_c0._4_4_ + (float)local_a0._4_4_) - local_c0._4_4_) /
             ((local_b8 + (float)local_a0._8_4_) - local_b8)) * (0.0 - local_b8) + local_c0._4_4_;
        local_b0._0_4_ =
             ((((float)local_c0 + (float)local_a0._0_4_) - (float)local_c0) /
             ((local_b8 + (float)local_a0._8_4_) - local_b8)) * (0.0 - local_b8) + (float)local_c0;
      }
    }
    _DAT_140b2a63c = local_b0._0_4_;
    _DAT_140b2a644 = local_a8;
    _DAT_140b2a640 = local_b0._4_4_;
    FUN_1400345c0(param_1,local_b0);
    fVar19 = DAT_1408007d4;
    if ((*(char *)((longlong)param_1 + 0x929) != '\0') &&
       (((param_1[0x10a] != 0 || (param_1[0x10b] != 0)) &&
        (0.0 < *(float *)((longlong)param_1 + 0x954) || *(float *)((longlong)param_1 + 0x954) == 0.0
        )))) {
      iVar3 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (iVar3 == 1) {
        if ((DAT_140b15418 != (longlong *)0x0) && (param_1[0x10a] != 0)) {
          fVar18 = *(float *)((longlong)param_1 + 0x954) * fVar19;
          lVar9 = FUN_140294a40(&DAT_140a16fd0);
          if (*(longlong *)(lVar9 + 0x68) != 0) {
            fVar18 = fVar18 * *(float *)(*(longlong *)(lVar9 + 0x68) + 0x2d30);
          }
          local_a0._0_8_ = local_b0;
          local_c0 = CONCAT44(fVar18,fVar18);
          uVar6 = CONCAT44(uVar5,fVar13);
          local_a0._8_4_ = local_a8;
          FUN_14012f570(param_1[0x10a],local_a0,0,&local_c0,uVar6);
          uVar5 = (undefined4)((ulonglong)uVar6 >> 0x20);
          FUN_14012fba0(param_1[0x10a],1);
        }
      }
      else {
        iVar3 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if (((iVar3 == 2) && (DAT_140b15418 != (longlong *)0x0)) && (param_1[0x10b] != 0)) {
          fVar18 = *(float *)((longlong)param_1 + 0x954) * fVar19;
          lVar9 = FUN_140294a40(&DAT_140a16fd0);
          if (*(longlong *)(lVar9 + 0x68) != 0) {
            fVar18 = fVar18 * *(float *)(*(longlong *)(lVar9 + 0x68) + 0x2d30);
          }
          local_a0._0_8_ = local_b0;
          local_c0 = CONCAT44(fVar18,fVar18);
          uVar6 = CONCAT44(uVar5,fVar13);
          local_a0._8_4_ = local_a8;
          FUN_14013a460(param_1[0x10b],local_a0,0,&local_c0,uVar6);
          uVar5 = (undefined4)((ulonglong)uVar6 >> 0x20);
          *(undefined1 *)(param_1[0x10b] + 0xcc) = 1;
        }
      }
    }
    if (((*(char *)((longlong)param_1 + 0x929) != '\0') && (param_1[0x10c] != 0)) &&
       (0.0 < *(float *)(param_1 + 0x12a))) {
      iVar3 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if (((iVar3 == 1) && (DAT_140b15418 != (longlong *)0x0)) &&
         ((param_1[0x10c] != 0 && (lVar9 = *(longlong *)param_1[0x127], lVar9 != 0)))) {
        fVar19 = *(float *)(param_1 + 0x12a) * fVar19;
        lVar10 = FUN_140294a40(&DAT_140a16fd0);
        if (*(longlong *)(lVar10 + 0x68) != 0) {
          fVar19 = fVar19 * *(float *)(*(longlong *)(lVar10 + 0x68) + 0x2d68);
        }
        local_a0._0_8_ = *(undefined8 *)(lVar9 + 0x78);
        local_a0._8_4_ = *(float *)(lVar9 + 0x80);
        local_c0 = CONCAT44(fVar19,fVar19);
        FUN_14012f570(param_1[0x10c],local_a0,0,&local_c0,CONCAT44(uVar5,fVar13));
        FUN_14012fba0(param_1[0x10c],1);
      }
    }
    local_res18 = local_res18 & 0xffffffffffffff00;
    uVar5 = FUN_140039980(param_1,plVar7,local_b0,&local_c8,param_1[0x104],&local_res18);
    uVar5 = FUN_140038b60(param_1,uVar5,&local_c8,local_res18 & 0xff);
    FUN_140038f80(param_1,uVar5,0);
  }
  if ((DAT_140b15418 != (longlong *)0x0) && ((char)DAT_140b15418[0x13] != '\0')) {
    lVar9 = FUN_140294a40(&DAT_140a16fd0);
    *(int *)(param_1 + 0xb) = (int)*(float *)(lVar9 + 0x70);
    FUN_1403e6dc0(&DAT_140b27f60);
  }
  FUN_14003d950(param_1);
  if (DAT_140b15418 == (longlong *)0x0) goto LAB_14002fe40;
  if ((char)DAT_140b15418[0x13] == '\x01') {
    cVar2 = (**(code **)(*DAT_140b15418 + 0xf8))();
    if (cVar2 == '\x01') {
LAB_14002fd68:
      iVar3 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))();
        if (cVar2 == '\x01') {
          FUN_14003cad0(param_1);
          FUN_14003ce60(param_1);
          goto LAB_14002fdd5;
        }
        iVar3 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
      }
    }
    else {
      iVar3 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        (**(code **)(*DAT_140b15418 + 0xf0))();
        goto LAB_14002fd68;
      }
    }
    if (iVar3 == 0) {
      FUN_14003d2b0(param_1);
    }
  }
LAB_14002fdd5:
  if (DAT_140b15418 != (longlong *)0x0) {
    lVar9 = FUN_14035e4c0();
    if (lVar9 != 0) {
      FUN_1403be8b0(lVar9,DAT_140a619f0,DAT_140a60de8);
    }
    if (((DAT_140b15418 != (longlong *)0x0) &&
        (cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))(), plVar8 = DAT_140b15418, cVar2 == '\x01'))
       && (DAT_140b15418 != (longlong *)0x0)) {
      cVar2 = FUN_14020e2b0();
      uVar6 = (**(code **)(*plVar8 + 0x1e8))(plVar8);
      _guard_check_icall(uVar6,cVar2 == '\0');
    }
  }
LAB_14002fe40:
  FUN_14003e240(param_1);
  if ((char)param_1[0x11c] != '\0') {
    puVar11 = (undefined4 *)FUN_1402126b0(&DAT_140a7c3a8,0x45);
    cVar2 = FUN_14020e350(*puVar11);
    if (cVar2 == '\0') {
      *(undefined1 *)(param_1 + 0x11c) = 0;
    }
  }
  if (*(char *)((longlong)param_1 + 0x8e1) != '\0') {
    puVar11 = (undefined4 *)FUN_1402126b0(&DAT_140a7c3a8,0x46);
    cVar2 = FUN_14020e350(*puVar11);
    if (cVar2 == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x8e1) = 0;
    }
  }
  if (*(char *)((longlong)param_1 + 0x8e2) != '\0') {
    puVar11 = (undefined4 *)FUN_1402126b0(&DAT_140a7c3a8,0x47);
    cVar2 = FUN_14020e350(*puVar11);
    if (cVar2 == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x8e2) = 0;
    }
  }
  if (*(char *)((longlong)param_1 + 0x8e3) != '\0') {
    puVar11 = (undefined4 *)FUN_1402126b0(&DAT_140a7c3a8,0x48);
    cVar2 = FUN_14020e350(*puVar11);
    if (cVar2 == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x8e3) = 0;
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x305890  Name: FUN_140305890  Size: 930 bytes
// ------------------------------------------------------------

void FUN_140305890(longlong param_1,undefined4 param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  longlong *plVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined8 *puVar11;
  longlong *plVar12;
  longlong *plVar13;
  longlong lVar14;
  
  iVar8 = FUN_14025e960();
  iVar9 = FUN_14025e930();
  if (((float)iVar8 != *(float *)(param_1 + 0x1008)) ||
     ((float)iVar9 != *(float *)(param_1 + 0x100c))) {
    FUN_140304770(param_1,0);
  }
  cVar7 = FUN_1402cd0c0(&DAT_140b27a10);
  if (((cVar7 != '\0') && (*(char *)(param_1 + 0x21a8) != '\0')) &&
     (*(code **)(param_1 + 0x27d0) != (code *)0x0)) {
    (**(code **)(param_1 + 0x27d0))();
    *(undefined1 *)(param_1 + 0x21a8) = 0;
  }
  iVar8 = 0;
  if (*(char *)(param_1 + 0x2699) != '\0') {
    FUN_1403def00(param_1,0);
    *(undefined8 *)(param_1 + 0x21a0) = 0;
    *(undefined8 *)(param_1 + 0x2030) = 0;
  }
  if ((*(int *)(param_1 + 0x211c) != 0) && (*(char *)(param_1 + 0x2699) == '\0')) {
    plVar12 = (longlong *)**(longlong **)(param_1 + 0x640);
    if (plVar12 != *(longlong **)(param_1 + 0x640)) {
      do {
        (**(code **)(*(longlong *)plVar12[5] + 0x60))((longlong *)plVar12[5],param_2);
        plVar13 = (longlong *)plVar12[2];
        if (*(char *)((longlong)plVar13 + 0x19) == '\0') {
          cVar7 = *(char *)(*plVar13 + 0x19);
          plVar12 = plVar13;
          plVar13 = (longlong *)*plVar13;
          while (cVar7 == '\0') {
            cVar7 = *(char *)(*plVar13 + 0x19);
            plVar12 = plVar13;
            plVar13 = (longlong *)*plVar13;
          }
        }
        else {
          cVar7 = *(char *)(plVar12[1] + 0x19);
          plVar6 = (longlong *)plVar12[1];
          plVar13 = plVar12;
          while ((plVar12 = plVar6, cVar7 == '\0' && (plVar13 == (longlong *)plVar12[2]))) {
            cVar7 = *(char *)(plVar12[1] + 0x19);
            plVar6 = (longlong *)plVar12[1];
            plVar13 = plVar12;
          }
        }
      } while (plVar12 != *(longlong **)(param_1 + 0x640));
    }
    if (*(char *)(param_1 + 0x2268) != '\0') {
      FUN_140265280(*(undefined8 *)(param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8),
                    param_2,0);
    }
    if (*(char *)(param_1 + 0x2698) != '\0') {
      puVar11 = (undefined8 *)(param_1 + 0x2288);
      lVar14 = 10;
      do {
        FUN_140265280(*puVar11,param_2,0);
        puVar11 = puVar11 + 0xd;
        lVar14 = lVar14 + -1;
      } while (lVar14 != 0);
    }
    if ((*(char *)(param_1 + 4000) != '\0') && (*(longlong *)(param_1 + 0xfa8) != 0)) {
      FUN_140265280(*(longlong *)(param_1 + 0xfa8),param_2,0);
    }
    if (*(longlong *)(param_1 + 0x2278) != 0) {
      FUN_140265280(*(longlong *)(param_1 + 0x2278),param_2,0);
    }
    if ((*(longlong *)(param_1 + 0x20d0) != 0) &&
       (((*(int *)(DAT_140b2eea0 + 0x44) != 3 || (*(int *)(param_1 + 0x211c) != 1)) ||
        (lVar14 = FUN_14034c770(*(longlong *)(param_1 + 0x20d0),0), lVar14 == 0)))) {
      lVar14 = *(longlong *)(param_1 + 0x20d0);
      if (lVar14 != 0) {
        FUN_14034a770(lVar14);
        thunk_FUN_1407864b8(lVar14,200);
      }
      *(undefined8 *)(param_1 + 0x20d0) = 0;
      *(undefined8 *)(param_1 + 0x21a0) = 0;
    }
    fVar5 = DAT_14085d6a8;
    fVar4 = DAT_14085acb4;
    fVar3 = DAT_1408054e8;
    fVar2 = DAT_140804fb8;
    fVar1 = DAT_1407ffaf8;
    if (DAT_140b15418 != 0) {
      iVar8 = *(int *)(DAT_140b15418 + 0x10);
    }
    plVar12 = (longlong *)**(longlong **)(param_1 + 11000);
    if (plVar12 != *(longlong **)(param_1 + 11000)) {
      do {
        plVar13 = (longlong *)plVar12[9];
        if (plVar13 == (longlong *)0x0) {
LAB_140305b3f:
          plVar13 = (longlong *)*plVar12;
          *(longlong **)plVar12[1] = plVar13;
          *(longlong *)(*plVar12 + 8) = plVar12[1];
          *(longlong *)(param_1 + 0x2b00) = *(longlong *)(param_1 + 0x2b00) + -1;
          FID_conflict__Tidy_deallocate(plVar12 + 2);
          thunk_FUN_1407864b8(plVar12,0x50);
        }
        else {
          iVar9 = iVar8 - *(int *)((longlong)plVar12 + 0x44);
          if (0x3c < iVar9) {
            FUN_14034af40();
            thunk_FUN_1407864b8(plVar13,0x7e8);
            plVar12[9] = 0;
            goto LAB_140305b3f;
          }
          *(undefined4 *)((longlong)plVar12 + 0x3c) = 0;
          iVar10 = iVar9 + -0x1e;
          if (iVar9 + -0x1e < 1) {
            iVar10 = 0;
          }
          *(float *)(plVar12 + 8) = ((float)iVar9 / fVar2) * fVar5;
          (**(code **)(*plVar13 + 0x20))(plVar13,(int)((fVar1 - (float)iVar10 / fVar4) * fVar3));
          plVar13 = (longlong *)*plVar12;
        }
        plVar12 = plVar13;
      } while (plVar13 != *(longlong **)(param_1 + 11000));
    }
  }
  return;
}




// ================================================================
// GOM STRING HOLDER — FUN_14029efe0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x29efe0  Name: FUN_14029efe0  Size: 646 bytes
// ------------------------------------------------------------

void FUN_14029efe0(longlong param_1,longlong *param_2,undefined8 param_3,longlong param_4,
                  undefined4 param_5,undefined1 param_6,char param_7)

{
  int iVar1;
  longlong *plVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined ***pppuVar5;
  undefined8 ****ppppuVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 local_68;
  undefined4 local_60;
  undefined **local_58;
  longlong local_50;
  undefined8 ***local_48 [2];
  longlong local_38;
  ulonglong local_30;
  
  lVar7 = 0;
  iVar1 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0xe0))();
  if ((iVar1 == 1) && (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x338), lVar7 != 0)) {
    local_58 = TerrainHeightSamplerClass::vftable;
    pppuVar5 = &local_58;
    local_50 = lVar7;
  }
  else {
    pppuVar5 = (undefined ***)0x0;
  }
  plVar2 = (longlong *)FUN_14014d570(param_2,pppuVar5,param_6);
  if (plVar2 != (longlong *)0x0) {
    (**(code **)*plVar2)(plVar2);
    local_68 = 0;
    local_60 = 0;
    plVar3 = (longlong *)FUN_14029f810(param_1,param_4,param_5,&local_68,&local_68,1,0);
    FUN_14039d6a0(plVar3);
    *(byte *)((longlong)plVar3 + 0x3a1) = *(byte *)((longlong)plVar3 + 0x3a1) | 8;
    lVar4 = plVar3[0x54];
    FUN_14001e680(local_48,plVar3[0x53] + 0xf8);
    if (local_30 - local_38 < 9) {
      FUN_14004f860(local_48,9,0,"_combined",9);
    }
    else {
      ppppuVar6 = local_48;
      if (0xf < local_30) {
        ppppuVar6 = (undefined8 ****)local_48[0];
      }
      lVar8 = (longlong)ppppuVar6 + local_38;
      local_38 = local_38 + 9;
      FUN_14076bd80(lVar8,"_combined",9);
      *(undefined1 *)(lVar8 + 9) = 0;
    }
    lVar8 = -1;
    ppppuVar6 = local_48;
    if (0xf < local_30) {
      ppppuVar6 = (undefined8 ****)local_48[0];
    }
    do {
      lVar8 = lVar8 + 1;
    } while (*(char *)((longlong)ppppuVar6 + lVar8) != '\0');
    FUN_140022730(plVar2 + 0x19);
    FUN_140265d10(lVar4,plVar2);
    FUN_14039f280(plVar3);
    FUN_1403a8ff0(plVar3);
    if (param_7 == '\0') {
      lVar4 = (**(code **)(*plVar3 + 0x10))(plVar3,0x14);
      if (lVar4 != 0) {
        FUN_1403a54c0(plVar3,lVar4);
      }
    }
    if (lVar7 != 0) {
      FUN_140136fb0(lVar7,param_3);
    }
    plVar3 = (longlong *)(param_4 + 0xf8);
    if (0xf < *(ulonglong *)(param_4 + 0x110)) {
      plVar3 = (longlong *)*plVar3;
    }
    FUN_140025760("GOM::Combine_Objects combined %d objects of type %s\n",param_2[1] - *param_2 >> 3
                  ,plVar3);
    if (0xf < local_30) {
      if (0xfff < local_30 + 1) {
        if (0x1f < (ulonglong)((longlong)local_48[0] + (-8 - (longlong)local_48[0][-1]))) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(local_48[0][-1],local_30 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
    (**(code **)(*plVar2 + 8))(plVar2);
  }
  return;
}




// ================================================================
// FRAME-ENDING CALLS
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2ffb0  Name: FUN_14002ffb0  Size: 88 bytes
// ------------------------------------------------------------

void FUN_14002ffb0(longlong param_1)

{
  char cVar1;
  float local_res18;
  float local_res1c;
  
  cVar1 = FUN_1402cd0c0(&DAT_140b27a10);
  if (cVar1 == '\x01') {
    FUN_14003f220();
    return;
  }
  local_res18 = (float)*(int *)(param_1 + 0xdc);
  local_res1c = (float)*(int *)(param_1 + 0xe0);
  FUN_14003f330(param_1,&local_res18);
  return;
}



// ------------------------------------------------------------
// RVA: 0x60330  Name: FUN_140060330  Size: 1808 bytes
// ------------------------------------------------------------

/* WARNING: Removing unreachable block (ram,0x000140060a04) */

void FUN_140060330(char param_1)

{
  longlong lVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char cVar5;
  uint uVar6;
  uint uVar7;
  undefined8 uVar8;
  longlong *plVar9;
  longlong lVar10;
  undefined8 uVar11;
  int iVar12;
  uint uVar13;
  float fVar14;
  ulonglong in_stack_fffffffffffffea8;
  char *local_148 [2];
  undefined8 local_138;
  ulonglong local_130;
  char local_128 [288];
  
  if (DAT_140a619ed != '\0') {
    fVar14 = (float)DAT_140b0a320;
    cVar5 = FUN_14027d800(&DAT_140b0a320);
    if (((cVar5 != '\0') && (DAT_140b3069b == '\0')) &&
       (cVar5 = thunk_FUN_1401768f0(), cVar5 != '\0')) {
      FUN_14028bd50(&DAT_140b153e0);
    }
    FUN_14025fc80(&DAT_140a61ad0);
    cVar5 = FUN_14025de30();
    if (cVar5 != '\0') {
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_14028c500(&DAT_140b153e0,fVar14);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      uVar11 = 0;
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        in_stack_fffffffffffffea8 = 0;
        FUN_14013d030(0,1,0,0,0);
        FUN_1403dda70(&DAT_140b27f60);
        FUN_140302620(&DAT_140b27f60);
        FUN_140300c00(&DAT_140b27f60);
        FUN_140302520(&DAT_140b27f60);
        FUN_1402fce50(&DAT_140b27f60);
        FUN_140301190(&DAT_140b27f60);
        FUN_140301120(&DAT_140b27f60);
        FUN_1402ff5a0(&DAT_140b27f60);
        FUN_1402ffcb0(&DAT_140b27f60,0,1);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_1403023b0(&DAT_140b27f60,1);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_140301100(&DAT_140b27f60);
        FUN_1403010d0(&DAT_140b27f60);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_1402717c0();
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_1402ffcb0(&DAT_140b27f60,2,4);
        FUN_140300dc0(&DAT_140b27f60);
        FUN_1402ffcb0(&DAT_140b27f60,5,6);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_140300980(&DAT_140b27f60);
        FUN_1402ffcb0(&DAT_140b27f60,7,8);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_1403023b0(&DAT_140b27f60,0);
      }
      if (DAT_140a61acf == '\0') {
        FUN_14044dea0(&DAT_140b306dd);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_1402ffcb0(&DAT_140b27f60,9,10);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_14025c8c0(DAT_140a619f0);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if (cVar5 != '\0') {
        FUN_1402ffcb0(&DAT_140b27f60,0xb,0xc);
      }
      if (((DAT_140a61acf == '\0') && (FUN_14044e2f0(&DAT_140b306dd), DAT_140a61acf == '\0')) &&
         (cVar5 = FUN_14044d840(&DAT_140b306dd), cVar5 != '\0')) {
        FUN_14044e020(&DAT_140b306dd);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_140271740();
      }
      uVar8 = FUN_14001fb80();
      FUN_14007d7b0(uVar8);
      FUN_14025c8c0(DAT_140a619f8);
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        uVar8 = FUN_14004fba0();
        FUN_140262aa0(uVar8,fVar14);
      }
      cVar5 = FUN_14027d800(&DAT_140b0a320);
      if ((cVar5 != '\0') && (DAT_140b3069b == '\0')) {
        FUN_14025fb30(DAT_140803510);
        FUN_14028c520(&DAT_140b153e0);
      }
      thunk_FUN_1401781f0();
      if ((DAT_140a61acf != '\0') && (param_1 != '\0')) {
        FUN_140048ec0(local_128,".\\ScreenShots\\Shot%05d.tga",DAT_140a61aa4);
        DAT_140a61aa4 = DAT_140a61aa4 + 1;
        CreateDirectoryA("ScreenShots\\",(LPSECURITY_ATTRIBUTES)0x0);
        FUN_140025760("Writing screen shot file: %s\n",local_128);
        local_148[0] = (char *)0x0;
        local_138 = 0;
        local_130 = 0xf;
        lVar10 = -1;
        do {
          lVar1 = lVar10 + 1;
          lVar10 = lVar10 + 1;
        } while (local_128[lVar1] != '\0');
        FUN_140022730(local_148,local_128);
        thunk_FUN_140178d40(local_148);
        if (0xf < local_130) {
          if (0xfff < local_130 + 1) {
            if ((char *)0x1f < local_148[0] + (-8 - *(longlong *)(local_148[0] + -8))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_148[0] + -8),local_130 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
      }
      if ((DAT_140a61b0c != '\0') && (param_1 != '\0')) {
        FUN_140066670();
        DAT_140a61b0c = '\0';
      }
      thunk_FUN_14013ce50();
      uVar6 = FUN_140222e90();
      if ((DAT_140a61b2c != 0) && (DAT_140a61b2c <= uVar6)) {
        iVar12 = DAT_140a10d80;
        if (DAT_140a10d80 < 0xf) {
          iVar12 = 0xf;
        }
        uVar7 = (int)(1000 / (longlong)iVar12) - 1;
        if ((int)uVar7 < 0) {
          uVar7 = 0;
        }
        uVar13 = uVar6 - DAT_140a61b2c;
        if (uVar13 < uVar7) {
          DAT_140a61b2c = uVar6;
          Sleep(uVar7 - uVar13);
          uVar6 = DAT_140a61b2c;
        }
      }
      DAT_140a61b2c = uVar6;
      if (DAT_140a61acc == '\x01') {
        DAT_140a61acc = '\0';
        plVar9 = (longlong *)FUN_14025e990("SPLASH_E3.TGA",1,0);
        if (plVar9 != (longlong *)0x0) {
          (**(code **)*plVar9)(plVar9);
        }
        lVar10 = FUN_140769c58(0x90);
        if (lVar10 != 0) {
          in_stack_fffffffffffffea8 =
               CONCAT44((int)(in_stack_fffffffffffffea8 >> 0x20),DAT_1407ffaf8);
          uVar11 = FUN_14025ff10(lVar10,0,0,DAT_1407ffaf8,in_stack_fffffffffffffea8,plVar9);
        }
        DAT_140a61ae0 = uVar11;
        if (plVar9 != (longlong *)0x0) {
          (**(code **)(*plVar9 + 8))(plVar9);
        }
        FUN_14025c080(DAT_140a619f0,DAT_140a61ae0);
        local_148[0] = (char *)FUN_140769c58(0x20);
        uVar4 = s_Splash_E3_Music_Event_140803238._12_4_;
        uVar3 = s_Splash_E3_Music_Event_140803238._8_4_;
        uVar2 = s_Splash_E3_Music_Event_140803238._4_4_;
        local_138 = 0x15;
        local_130 = 0x1f;
        *(undefined4 *)local_148[0] = s_Splash_E3_Music_Event_140803238._0_4_;
        *(undefined4 *)(local_148[0] + 4) = uVar2;
        *(undefined4 *)(local_148[0] + 8) = uVar3;
        *(undefined4 *)(local_148[0] + 0xc) = uVar4;
        *(undefined4 *)(local_148[0] + 0x10) = s_Splash_E3_Music_Event_140803238._16_4_;
        local_148[0][0x14] = s_Splash_E3_Music_Event_140803238[0x14];
        local_148[0][0x15] = '\0';
        lVar10 = FUN_1402d1e50(&DAT_140b27dc0,local_148);
        if (0xf < local_130) {
          if (0xfff < local_130 + 1) {
            if ((char *)0x1f < local_148[0] + (-8 - *(longlong *)(local_148[0] + -8))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_148[0] + -8),local_130 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        local_138 = 0;
        local_130 = 0xf;
        local_148[0] = (char *)((ulonglong)local_148[0] & 0xffffffffffffff00);
        if (lVar10 != 0) {
          FUN_1402d21f0(&DAT_140b27dc0,lVar10,0xb,0,in_stack_fffffffffffffea8 & 0xffffffffffffff00);
          DAT_140a10ca0 = FUN_14021cef0(&DAT_140a7d0a0,1);
          DAT_140a10ca4 = FUN_14021cef0(&DAT_140a7d0a0,2);
          FUN_14021ce30(&DAT_140a7d0a0,1,0,0);
          FUN_14021ce30(&DAT_140a7d0a0,2,0,0);
        }
        if ((DAT_140b15418 != 0) && (*(longlong *)(DAT_140b15418 + 0x28) != 0)) {
          FUN_140341fe0();
        }
      }
    }
  }
  return;
}




// ================================================================
// GameObjectManager STRING HOLDERS
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2af7c0  Name: FUN_1402af7c0  Size: 1096 bytes
// ------------------------------------------------------------

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402af7c0(longlong param_1,longlong param_2,int param_3,int param_4,longlong param_5,
                  char param_6)

{
  undefined ***pppuVar1;
  undefined4 uVar2;
  char cVar3;
  int iVar4;
  HANDLE hHeap;
  undefined8 *puVar5;
  ulonglong uVar6;
  undefined8 *******pppppppuVar7;
  LPVOID lpMem;
  undefined8 *******pppppppuVar8;
  longlong lVar9;
  longlong lVar10;
  undefined4 *puVar11;
  int iVar12;
  undefined8 uVar13;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  int local_74;
  undefined4 local_70;
  undefined8 *******local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  if (param_2 == 0) {
    return;
  }
  puVar5 = (undefined8 *)(*(longlong *)(param_2 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(param_2 + 0x298) + 0x110)) {
    puVar5 = (undefined8 *)*puVar5;
  }
  FUN_140025760("GameObjectManagerClass::Initiate_Space_Conflict -- Planet %s\n",puVar5);
  local_98 = *(undefined4 *)(param_2 + 0x50);
  local_88 = DynamicVectorClass<int>::vftable;
  local_90 = -1;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  local_74 = -0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 *******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  uVar13 = _DAT_140802a40;
  local_94 = param_3;
  if (param_5 != 0) {
    lVar9 = -1;
    do {
      lVar9 = lVar9 + 1;
    } while (*(char *)(param_5 + lVar9) != '\0');
    uVar13 = FUN_140022730(local_68);
  }
  pppppppuVar8 = local_68[0];
  lpMem = local_80;
  uVar2 = local_98;
  lVar9 = *(longlong *)(param_2 + 0xb8);
  if (param_4 == -1) {
    param_4 = -1;
  }
  if ((lVar9 == 0) || (param_4 != -1)) {
LAB_1402af91d:
    if (param_4 == -1) goto LAB_1402af922;
  }
  else {
    iVar12 = 0;
    if (0 < *(int *)(lVar9 + 0xd0)) {
      lVar10 = 0;
      do {
        lVar9 = *(longlong *)(lVar10 + *(longlong *)(lVar9 + 200));
        if (((lVar9 != 0) &&
            (cVar3 = FUN_1403a9760(lVar9), uVar13 = extraout_XMM0_Qa, cVar3 != '\x01')) &&
           (param_4 = *(int *)(lVar9 + 0x58), param_4 != param_3)) goto LAB_1402af91d;
        lVar9 = *(longlong *)(param_2 + 0xb8);
        iVar12 = iVar12 + 1;
        lVar10 = lVar10 + 8;
      } while (iVar12 < *(int *)(lVar9 + 0xd0));
    }
LAB_1402af922:
    if (((*(int *)(*(longlong *)(param_2 + 0xb8) + 0x148) == 0) ||
        (param_4 = *(int *)(*(longlong *)(param_2 + 0xb8) + 0x13c), param_4 == param_3)) ||
       (param_4 == -1)) {
      iVar12 = local_74;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
        if (0xfff < uStack_50 + 1) {
          pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
          uVar6 = uStack_50 + 0x28;
          if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7)))
          goto LAB_1402afc03;
        }
        thunk_FUN_1407864b8(pppppppuVar7);
        iVar12 = local_74;
      }
      goto joined_r0x0001402afbaf;
    }
  }
  local_90 = param_4;
  if (param_6 == '\0') {
    iVar12 = *(int *)(param_1 + 0x478);
    iVar4 = *(int *)(param_1 + 0x47c) * 2 >> 1;
    if (iVar4 <= iVar12) {
      iVar12 = iVar4 / 10;
      if (iVar4 / 10 < 10) {
        iVar12 = 10;
      }
      FUN_1402b6fa0(param_1 + 0x468,iVar12 + iVar4,0,1);
      iVar12 = *(int *)(param_1 + 0x478);
    }
    puVar11 = (undefined4 *)((longlong)iVar12 * 0x58 + *(longlong *)(param_1 + 0x470));
    *(int *)(param_1 + 0x478) = iVar12 + 1;
    *puVar11 = uVar2;
    puVar11[1] = local_94;
    puVar11[2] = param_4;
    pppuVar1 = (undefined ***)(puVar11 + 4);
    iVar12 = local_74;
    if (pppuVar1 != &local_88) {
      FUN_14001f910(pppuVar1);
      iVar12 = local_74;
      puVar11[9] = local_74;
      iVar4 = local_74 * 2 >> 1;
      if (iVar4 != 0) {
        FUN_140021ec0(pppuVar1,iVar4,0,1);
        if (0 < (int)local_78) {
          uVar6 = (ulonglong)local_78;
          lVar9 = 0;
          do {
            *(undefined4 *)(lVar9 + *(longlong *)(puVar11 + 6)) =
                 *(undefined4 *)((longlong)lpMem + lVar9);
            uVar6 = uVar6 - 1;
            lVar9 = lVar9 + 4;
          } while (uVar6 != 0);
        }
        puVar11[8] = local_78;
      }
    }
    puVar11[10] = local_70;
    if ((undefined8 ********)(puVar11 + 0xc) != local_68) {
      pppppppuVar7 = local_68;
      if (0xf < uStack_50) {
        pppppppuVar7 = pppppppuVar8;
      }
      FUN_140022730(puVar11 + 0xc,pppppppuVar7,local_58);
      lpMem = local_80;
      pppppppuVar8 = local_68[0];
      iVar12 = local_74;
    }
    *(undefined1 *)(puVar11 + 0x14) = local_48;
  }
  else {
    FUN_14029bb40(param_1 + 0x468,&local_98);
    lpMem = local_80;
    pppppppuVar8 = local_68[0];
    iVar12 = local_74;
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar13 = extraout_XMM0_Qa_00;
  if ((param_3 == iVar4) && (DAT_140b155d4 == param_3)) {
    lVar9 = FUN_140371450(*(undefined8 *)(param_2 + 0x298),DAT_140b155d8);
    if (lVar9 == 0) {
      lVar9 = FUN_140294a40(&DAT_140a16fd0);
      lVar9 = *(longlong *)(*(longlong *)(lVar9 + 0x68) + 0x650);
      uVar13 = extraout_XMM0_Qa_01;
      if (lVar9 != 0) {
        uVar13 = FUN_1402d5290(&DAT_140b27e60,lVar9,0,0,0);
      }
    }
    else {
      FUN_140322780(&DAT_140b2b3a8);
      FUN_1403207c0(&DAT_140b2b2f0);
      FUN_140321830(&DAT_140b2b300,0);
      uVar13 = FUN_140321170(&DAT_140b2b300,lVar9);
    }
  }
  if (0xf < uStack_50) {
    uVar6 = uStack_50 + 1;
    pppppppuVar7 = pppppppuVar8;
    if (0xfff < uVar6) {
      pppppppuVar7 = (undefined8 *******)pppppppuVar8[-1];
      uVar6 = uStack_50 + 0x28;
      if (0x1f < (ulonglong)((longlong)pppppppuVar8 + (-8 - (longlong)pppppppuVar7))) {
LAB_1402afc03:
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar13,uVar6);
      }
    }
    thunk_FUN_1407864b8(pppppppuVar7,uVar6);
  }
joined_r0x0001402afbaf:
  if (lpMem != (LPVOID)0x0) {
    if (iVar12 < 0) {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,lpMem);
    }
    else {
      thunk_FUN_1407864b8(lpMem);
    }
  }
  return;
}



// ------------------------------------------------------------
// RVA: 0x2b0520  Name: FUN_1402b0520  Size: 1163 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402b0520(longlong param_1,undefined4 param_2,undefined ***param_3,int param_4,int param_5,
                  longlong param_6,char param_7)

{
  undefined ***pppuVar1;
  undefined1 auVar2 [16];
  uint uVar3;
  int iVar4;
  longlong lVar5;
  HANDLE pvVar6;
  longlong lVar7;
  int iVar8;
  int iVar9;
  undefined8 *puVar10;
  undefined8 ******ppppppuVar11;
  ulonglong uVar12;
  undefined8 ******ppppppuVar13;
  LPVOID lpMem;
  longlong lVar14;
  uint uVar15;
  undefined8 ******ppppppuVar16;
  undefined4 *puVar17;
  undefined8 extraout_XMM0_Qa;
  undefined8 extraout_XMM0_Qa_00;
  undefined8 uVar18;
  undefined8 extraout_XMM0_Qa_01;
  undefined4 local_res10;
  undefined4 local_98;
  int local_94;
  int local_90;
  undefined **local_88;
  LPVOID local_80;
  uint local_78;
  uint local_74;
  undefined4 local_70;
  undefined8 *****local_68 [2];
  undefined8 local_58;
  ulonglong uStack_50;
  undefined1 local_48;
  
  lVar5 = FUN_1402aca60();
  if (lVar5 == 0) {
    return;
  }
  puVar10 = (undefined8 *)(*(longlong *)(lVar5 + 0x298) + 0xf8);
  if (0xf < *(ulonglong *)(*(longlong *)(lVar5 + 0x298) + 0x110)) {
    puVar10 = (undefined8 *)*puVar10;
  }
  FUN_140025760("GameObjectManagerClass::Land_Invade -- Invading planet %s\n",puVar10);
  ppppppuVar13 = (undefined8 ******)0x0;
  local_88 = DynamicVectorClass<int>::vftable;
  local_80 = (LPVOID)0x0;
  local_78 = 0;
  lVar14 = -1;
  local_74 = 0x80000000;
  local_70 = 0x3fffff;
  local_68[0] = (undefined8 ******)0x0;
  local_58 = _DAT_140802a40;
  uStack_50 = _UNK_140802a48;
  local_48 = 0;
  local_98 = param_2;
  local_94 = param_4;
  if (&local_88 != param_3) {
    uVar15 = *(uint *)((longlong)param_3 + 0x14);
    uVar3 = (int)(uVar15 * 2) >> 1;
    local_74 = uVar15;
    if (uVar3 != 0) {
      if ((int)uVar15 < 0) {
        pvVar6 = GetProcessHeap();
        local_80 = HeapAlloc(pvVar6,0,(longlong)(int)uVar3 * 4);
      }
      else {
        auVar2._8_8_ = 0;
        auVar2._0_8_ = (longlong)(int)uVar3;
        lVar7 = SUB168(ZEXT816(4) * auVar2,0);
        if (SUB168(ZEXT816(4) * auVar2,8) != 0) {
          lVar7 = lVar14;
        }
        local_80 = (LPVOID)thunk_FUN_140769c58(lVar7);
      }
      local_78 = *(uint *)(param_3 + 2);
      local_74 = (uVar15 ^ uVar3) & 0x7fffffff ^ uVar15;
      ppppppuVar11 = ppppppuVar13;
      ppppppuVar16 = ppppppuVar13;
      if (0 < (int)local_78) {
        do {
          uVar15 = (int)ppppppuVar16 + 1;
          *(undefined4 *)((longlong)ppppppuVar11 + (longlong)local_80) =
               *(undefined4 *)((longlong)ppppppuVar11 + (longlong)param_3[1]);
          local_78 = *(uint *)(param_3 + 2);
          ppppppuVar11 = (undefined8 ******)((longlong)ppppppuVar11 + 4);
          ppppppuVar16 = (undefined8 ******)(ulonglong)uVar15;
        } while ((int)uVar15 < (int)local_78);
      }
    }
  }
  local_90 = param_5;
  ppppppuVar11 = ppppppuVar13;
  local_res10 = param_2;
  if (param_6 != 0) {
    do {
      lVar14 = lVar14 + 1;
    } while (*(char *)(param_6 + lVar14) != '\0');
    FUN_140022730(local_68,param_6,lVar14);
    local_res10 = local_98;
    ppppppuVar11 = (undefined8 ******)local_68[0];
  }
  uVar15 = local_78;
  lpMem = local_80;
  iVar9 = local_90;
  if (local_90 == -1) {
    if (*(int *)(*(longlong *)(lVar5 + 0xb8) + 0x130) == 0) {
      lVar14 = FUN_1403956c0(lVar5);
      iVar9 = *(int *)(lVar14 + 0xc);
      local_90 = iVar9;
      if (((iVar9 == -1) ||
          (lVar14 = FUN_140294aa0(&DAT_140a16fd0), iVar9 == *(int *)(lVar14 + 0x4c))) &&
         (lVar14 = FUN_140397120(lVar5), *(int *)(lVar14 + 0x10) != 0)) {
        iVar9 = *(int *)(**(longlong **)(lVar14 + 8) + 0x58);
        local_90 = iVar9;
      }
    }
    else {
      iVar9 = *(int *)(*(longlong *)(lVar5 + 0xb8) + 0x124);
      local_90 = iVar9;
    }
  }
  iVar4 = FUN_140294a70(&DAT_140a16fd0);
  uVar18 = extraout_XMM0_Qa;
  if (param_4 == iVar4) {
    lVar14 = FUN_140294a40(&DAT_140a16fd0);
    lVar14 = *(longlong *)(*(longlong *)(lVar14 + 0x68) + 0x640);
    uVar18 = extraout_XMM0_Qa_00;
    if (DAT_140b155d4 == param_4) {
      lVar5 = FUN_140371450(*(undefined8 *)(lVar5 + 0x298),DAT_140b155d8);
      if (lVar5 != 0) {
        FUN_140322780(&DAT_140b2b3a8);
        FUN_1403207c0(&DAT_140b2b2f0);
        FUN_140321830(&DAT_140b2b300,0);
        uVar18 = FUN_140321170(&DAT_140b2b300,lVar5);
        goto LAB_1402b07ed;
      }
      lVar5 = FUN_140294a40(&DAT_140a16fd0);
      lVar14 = *(longlong *)(*(longlong *)(lVar5 + 0x68) + 0x650);
      uVar18 = extraout_XMM0_Qa_01;
    }
    if (lVar14 != 0) {
      uVar18 = FUN_1402d5290(&DAT_140b27e60,lVar14,0,0,0);
    }
  }
LAB_1402b07ed:
  if (local_94 != iVar9) {
    if (param_7 == '\0') {
      iVar4 = *(int *)(param_1 + 0x490);
      iVar8 = *(int *)(param_1 + 0x494) * 2 >> 1;
      if (iVar8 <= iVar4) {
        iVar4 = iVar8 / 10;
        if (iVar8 / 10 < 10) {
          iVar4 = 10;
        }
        uVar18 = FUN_1402b6fa0(param_1 + 0x480,iVar4 + iVar8,0,1);
        iVar4 = *(int *)(param_1 + 0x490);
      }
      puVar17 = (undefined4 *)((longlong)iVar4 * 0x58 + *(longlong *)(param_1 + 0x488));
      *(int *)(param_1 + 0x490) = iVar4 + 1;
      *puVar17 = local_res10;
      pppuVar1 = (undefined ***)(puVar17 + 4);
      puVar17[1] = local_94;
      puVar17[2] = iVar9;
      if (pppuVar1 != &local_88) {
        uVar18 = FUN_14001f910(pppuVar1);
        puVar17[9] = local_74;
        iVar9 = (int)(local_74 * 2) >> 1;
        if (iVar9 != 0) {
          uVar18 = FUN_140021ec0(pppuVar1,iVar9,0,1);
          if (0 < (int)uVar15) {
            uVar12 = (ulonglong)uVar15;
            do {
              *(undefined4 *)((longlong)ppppppuVar13 + *(longlong *)(puVar17 + 6)) =
                   *(undefined4 *)((longlong)ppppppuVar13 + (longlong)lpMem);
              uVar12 = uVar12 - 1;
              ppppppuVar13 = (undefined8 ******)((longlong)ppppppuVar13 + 4);
            } while (uVar12 != 0);
          }
          puVar17[8] = uVar15;
        }
      }
      puVar17[10] = local_70;
      if ((undefined8 ******)(puVar17 + 0xc) != local_68) {
        ppppppuVar13 = local_68;
        if (0xf < uStack_50) {
          ppppppuVar13 = ppppppuVar11;
        }
        uVar18 = FUN_140022730(puVar17 + 0xc,ppppppuVar13,local_58);
        lpMem = local_80;
        ppppppuVar11 = (undefined8 ******)local_68[0];
      }
      *(undefined1 *)(puVar17 + 0x14) = local_48;
    }
    else {
      uVar18 = FUN_14029bb40(param_1 + 0x480,&local_98);
      lpMem = local_80;
      ppppppuVar11 = (undefined8 ******)local_68[0];
    }
  }
  if (0xf < uStack_50) {
    ppppppuVar13 = ppppppuVar11;
    if (0xfff < uStack_50 + 1) {
      ppppppuVar13 = (undefined8 ******)ppppppuVar11[-1];
      if (0x1f < (ulonglong)((longlong)ppppppuVar11 + (-8 - (longlong)ppppppuVar13))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(uVar18,uStack_50 + 0x28);
      }
    }
    thunk_FUN_1407864b8(ppppppuVar13);
  }
  if (lpMem != (LPVOID)0x0) {
    if ((local_74 & 0x80000000) == 0) {
      thunk_FUN_1407864b8(lpMem);
    }
    else {
      pvVar6 = GetProcessHeap();
      HeapFree(pvVar6,0,lpMem);
    }
  }
  return;
}




// ================================================================
// GameObjectClass STRING HOLDER
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3a9e30  Name: FUN_1403a9e30  Size: 6165 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1403a9e30(longlong *param_1,uint param_2,float param_3,undefined *param_4,
                  undefined *param_5,undefined1 *param_6,float *param_7,char param_8,char param_9,
                  undefined4 param_10,int param_11,undefined8 *******param_12,char param_13,
                  undefined1 *param_14)

{
  float fVar1;
  bool bVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  longlong *plVar7;
  longlong lVar8;
  undefined *puVar9;
  longlong lVar10;
  undefined8 uVar11;
  undefined *puVar12;
  undefined *puVar13;
  int iVar14;
  char *pcVar15;
  undefined8 *puVar16;
  undefined8 *******pppppppuVar17;
  longlong *plVar18;
  wchar_t *pwVar19;
  uint uVar20;
  undefined4 uVar21;
  longlong lVar22;
  longlong *plVar23;
  wchar_t *pwVar24;
  bool bVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  float fVar30;
  float fVar31;
  int local_res8;
  undefined *in_stack_fffffffffffffe88;
  ulonglong uVar32;
  undefined1 *in_stack_fffffffffffffe90;
  undefined *puVar33;
  float *in_stack_fffffffffffffe98;
  undefined4 uVar34;
  ulonglong in_stack_fffffffffffffea8;
  undefined8 in_stack_fffffffffffffeb0;
  longlong local_128;
  undefined8 local_120;
  undefined4 local_118;
  undefined8 ******local_110 [2];
  longlong local_100;
  ulonglong local_f8;
  undefined8 local_f0;
  ulonglong local_e0;
  ulonglong local_d8;
  
  if (param_14 != (undefined1 *)0x0) {
    *param_14 = 0;
  }
  lVar22 = param_1[0x53];
  if ((((((0.0 < *(float *)(lVar22 + 0x23b0)) && (lVar8 = param_1[0x15], lVar8 != 0)) &&
        (*(int *)(lVar8 + 0x48) == 0x16)) && ((lVar22 != 0 && (*(char *)(lVar22 + 0xd1) != '\0'))))
      && (*(char *)((longlong)param_1 + 0x333) != -1)) &&
     ((*(char *)(lVar8 + 0x2b3) == '\0' &&
      (plVar7 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1), plVar7 != (longlong *)0x0))))
  {
    (**(code **)(*plVar7 + 0x118))(plVar7,1);
  }
  if (((param_1[0x2a] != 0) && (lVar22 = *(longlong *)(param_1[0x2a] + 0x10), lVar22 != 0)) &&
     ((*(byte *)(lVar22 + 0x3a0) & 0x42) == 0)) {
    FUN_1403a9e30(lVar22,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,param_10,0,
                  0,0,0);
    return;
  }
  fVar26 = (float)FUN_140396df0(param_1);
  uVar34 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
  uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffe90 >> 0x20);
  puVar13 = (undefined *)0x0;
  lVar22 = -1;
  if (param_8 == '\0') {
    if (*(char *)(param_1[0x53] + 0x9c) == '\0') {
      if (((*(char *)(param_1[0x53] + 0x9d) != '\0') &&
          (plVar7 = (longlong *)param_1[0x56], plVar7 != (longlong *)0x0)) &&
         ((lVar8 = (**(code **)(*plVar7 + 0x10))(plVar7,0x16), lVar8 != 0 &&
          (iVar5 = FUN_140405300(lVar8), 1 < iVar5)))) {
        fVar26 = (float)FUN_1403727a0(param_1[0x53],param_1);
        iVar14 = 0;
        do {
          lVar22 = FUN_1404052d0(lVar8,iVar14);
          uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffeb0 >> 0x20);
          if (lVar22 != 0) {
            fVar27 = (float)FUN_1403727a0(*(undefined8 *)(lVar22 + 0x298),lVar22);
            in_stack_fffffffffffffeb0 = CONCAT44(uVar21,0xffffffff);
            in_stack_fffffffffffffea8 = in_stack_fffffffffffffea8 & 0xffffffffffffff00;
            FUN_1403a9e30(lVar22,param_2,fVar27 * (param_3 / fVar26),param_4,param_5,param_6,param_7
                          ,1,in_stack_fffffffffffffea8,in_stack_fffffffffffffeb0,0,0,0,0);
          }
          iVar14 = iVar14 + 1;
        } while (iVar14 < iVar5);
        return;
      }
      goto LAB_1403aa1be;
    }
    plVar7 = (longlong *)param_1[0x56];
    if (plVar7 == (longlong *)0x0) goto LAB_1403aa1be;
    lVar8 = (**(code **)(*plVar7 + 0x10))(plVar7,0x16);
    uVar34 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
    uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffe90 >> 0x20);
    if (lVar8 == 0) goto LAB_1403aa1be;
    iVar5 = FUN_140405300(lVar8);
    uVar34 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
    uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffe90 >> 0x20);
    if (iVar5 < 2) goto LAB_1403aa1be;
    puVar9 = puVar13;
    do {
      plVar7 = (longlong *)FUN_1404052d0(lVar8,puVar9);
      if (((plVar7 != (longlong *)0x0) && (plVar7 != param_1)) &&
         (*(char *)(plVar7[0x53] + 0x9c) == '\0')) {
        in_stack_fffffffffffffea8 = in_stack_fffffffffffffea8 & 0xffffffffffffff00;
        in_stack_fffffffffffffe88 = param_5;
        in_stack_fffffffffffffe90 = param_6;
        in_stack_fffffffffffffe98 = param_7;
        FUN_1403a9e30(plVar7,param_2,param_3 / (float)(iVar5 + -1),param_4,param_5,param_6,param_7,1
                      ,in_stack_fffffffffffffea8,0xffffffff,0,0,0,0);
      }
      uVar34 = (undefined4)((ulonglong)in_stack_fffffffffffffe98 >> 0x20);
      uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffe90 >> 0x20);
      uVar20 = (int)puVar9 + 1;
      puVar9 = (undefined *)(ulonglong)uVar20;
    } while ((int)uVar20 < iVar5);
    bVar25 = param_2 == 0;
LAB_1403aa1ce:
    if (!bVar25) {
      return;
    }
  }
  else {
LAB_1403aa1be:
    if ((param_2 != 0) && (param_2 != 0xd)) {
      bVar25 = *(char *)((longlong)param_1 + 0x37d) == -1;
      goto LAB_1403aa1ce;
    }
  }
  if (*(char *)(param_1[0x53] + 0xa3) != '\0') {
    return;
  }
  if (*(char *)((longlong)param_1 + 0x3a7) != '\0') {
    return;
  }
  local_128 = 0;
  if (param_4 != (undefined *)0x0) {
    local_128 = *(longlong *)(param_4 + 0xe8);
  }
  if ((((*(byte *)((longlong)param_1 + 0x3a1) & 0x10) != 0) && (param_4 != (undefined *)0x0)) &&
     ((local_128 == 0 || (*(longlong **)(local_128 + 8) == param_1)))) {
    *(byte *)((longlong)param_1 + 0x3a1) = *(byte *)((longlong)param_1 + 0x3a1) & 0xef;
  }
  fVar27 = param_3;
  if (param_9 == '\0') {
    fVar27 = (float)FUN_140398010(param_1,7);
    fVar27 = fVar27 * param_3;
  }
  cVar3 = '\0';
  if (param_4 != (undefined *)0x0) {
    if (local_128 == 0) {
      if ((param_4[0x3a1] & 0x10) != 0) {
        param_4[0x3a1] = param_4[0x3a1] & 0xef;
        fVar28 = (float)FUN_140398010(param_4,8);
        puVar9 = param_4;
        goto LAB_1403aa2cf;
      }
    }
    else if ((*(int *)(local_128 + 0x58) != 0x3fffff) &&
            (puVar9 = (undefined *)FUN_1402aca60(DAT_140b15418[3]), puVar9 != (undefined *)0x0)) {
      if ((puVar9[0x3a1] & 0x10) != 0) {
        puVar9[0x3a1] = puVar9[0x3a1] & 0xef;
      }
      fVar28 = (float)FUN_140398010(puVar9,8);
LAB_1403aa2cf:
      fVar27 = fVar27 * fVar28;
      cVar3 = FUN_140397f60(puVar9,3);
    }
  }
  lVar8 = param_1[0x2e];
  if ((((lVar8 != 0) && (DAT_140b15418 != (longlong *)0x0)) &&
      (cVar4 = (**(code **)(*DAT_140b15418 + 0xf0))(), cVar4 == '\x01')) &&
     (cVar4 = FUN_14053e1c0(lVar8), cVar4 != '\0')) {
    fVar27 = fVar27 * _DAT_140b27044;
  }
  if ((cVar3 != '\0') && (param_2 == 1)) {
    iVar5 = *(int *)((longlong)param_1 + 0x304);
    iVar14 = DAT_140b0a320 - iVar5;
    *(int *)((longlong)param_1 + 0x304) = DAT_140b0a320;
    if (iVar5 == 0) {
      iVar14 = 999999;
    }
    fVar28 = (float)(**(code **)(DAT_140b26ff0 + 0x10))
                              (&DAT_140b26ff0,(float)iVar14 / (float)DAT_140b0a340);
    fVar27 = fVar27 * fVar28;
  }
  if (*(char *)((longlong)param_1 + 0x35c) != -1) {
    fVar27 = fVar27 * _DAT_140b27630;
  }
  if (param_7 != (float *)0x0) {
    *param_7 = 1.0;
  }
  if (((0xd < param_2) || ((0x2101U >> (param_2 & 0x1f) & 1) == 0)) && (DAT_140a2864b == '\0')) {
    return;
  }
  if ((((param_1[0x57] != 0) && (lVar8 = *(longlong *)(param_1[0x57] + 0x20), lVar8 != 0)) &&
      (lVar8 = *(longlong *)(lVar8 + 0x28), lVar8 != 0)) && (*(char *)(lVar8 + 0x20) != '\0')) {
    return;
  }
  if ((*(byte *)(param_1 + 0x74) & 0x20) != 0) {
    return;
  }
  if (((DAT_140b15418 != (longlong *)0x0) &&
      (cVar3 = (**(code **)(*DAT_140b15418 + 0xf0))(), cVar3 == '\x01')) &&
     ((lVar8 = DAT_140b15418[6], lVar8 != 0 && (cVar3 = FUN_1403400b0(lVar8), cVar3 == '\x01')))) {
    iVar5 = DAT_140b153fc;
    if (DAT_140b15418 != (longlong *)0x0) {
      iVar5 = (**(code **)(*DAT_140b15418 + 0xe0))();
    }
    if (iVar5 == 2) {
      cVar3 = FUN_1403400c0(lVar8,(int)param_1[10]);
      if (cVar3 == '\x01') {
        lVar8 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
        if (lVar8 == 0) {
          fVar27 = fVar27 * fRam00000000000020b8;
        }
        else {
          fVar27 = fVar27 * *(float *)(*(longlong *)(lVar8 + 0x68) + 0x20b8);
        }
      }
    }
    else {
      iVar5 = DAT_140b153fc;
      if (DAT_140b15418 != (longlong *)0x0) {
        iVar5 = (**(code **)(*DAT_140b15418 + 0xe0))();
      }
      if ((iVar5 == 1) && (cVar3 = FUN_1403400c0(lVar8,(int)param_1[10]), cVar3 == '\x01')) {
        lVar8 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
        puVar9 = puVar13;
        if (lVar8 != 0) {
          puVar9 = *(undefined **)(lVar8 + 0x68);
        }
        fVar27 = fVar27 * *(float *)(puVar9 + 0x2198);
      }
    }
  }
  local_110[0] = (undefined8 *******)0x0;
  local_100 = 0;
  local_f8 = 0xf;
  if (param_5 != (undefined *)0x0) {
    do {
      lVar22 = lVar22 + 1;
    } while (param_5[lVar22] != '\0');
    FUN_140022730(local_110,param_5);
  }
  if ((param_13 == '\0') && (local_128 != 0)) {
    if (*(longlong **)(local_128 + 8) == param_1) {
      iVar5 = *(int *)(local_128 + 0x10);
      if (iVar5 < 0) goto LAB_1403aa5b1;
      lVar22 = param_1[0x5a];
      if (((lVar22 == 0) || (*(int *)(lVar22 + 0x10) <= iVar5)) ||
         (lVar22 = *(longlong *)(*(longlong *)(lVar22 + 8) + (longlong)iVar5 * 8), lVar22 == 0))
      goto LAB_1403aa622;
      lVar22 = *(longlong *)(lVar22 + 0x20);
      puVar16 = (undefined8 *)(lVar22 + 0xa0);
      if (0xf < *(ulonglong *)(lVar22 + 0xb8)) {
        puVar16 = (undefined8 *)*puVar16;
      }
      lVar22 = -1;
      do {
        lVar22 = lVar22 + 1;
      } while (*(char *)((longlong)puVar16 + lVar22) != '\0');
LAB_1403aa619:
      FUN_140022730(local_110);
    }
    else {
LAB_1403aa5b1:
      if (param_4 != (undefined *)0x0) {
        iVar5 = DAT_140b153fc;
        if (DAT_140b15418 != (longlong *)0x0) {
          iVar5 = (**(code **)(*DAT_140b15418 + 0xe0))();
        }
        if ((iVar5 == 1) && (lVar22 = FUN_140394a80(param_1,param_4 + 0x78,0,0), lVar22 != 0)) {
          puVar16 = (undefined8 *)(*(longlong *)(lVar22 + 0x20) + 0xa0);
          if (0xf < *(ulonglong *)(*(longlong *)(lVar22 + 0x20) + 0xb8)) {
            puVar16 = (undefined8 *)*puVar16;
          }
          lVar22 = -1;
          do {
            lVar22 = lVar22 + 1;
          } while (*(char *)((longlong)puVar16 + lVar22) != '\0');
          goto LAB_1403aa619;
        }
      }
    }
  }
LAB_1403aa622:
  if (param_6 != (undefined1 *)0x0) {
    *param_6 = 0;
  }
  if ((*(float *)((longlong)param_1 + 0x5c) <= 0.0) || (fVar27 <= 0.0)) goto LAB_1403ab582;
  bVar25 = false;
  local_res8 = -1;
  if ((param_4 == (undefined *)0x0) ||
     ((local_128 == 0 || (*(int *)(local_128 + 0x58) == 0x3fffff)))) {
LAB_1403aa870:
    bVar25 = false;
    local_res8 = -1;
    puVar9 = param_4;
    puVar12 = puVar13;
    if (local_128 != 0) goto LAB_1403aa895;
  }
  else {
    lVar22 = FUN_1402aca60(DAT_140b15418[3]);
    if (lVar22 != 0) {
      lVar8 = *(longlong *)(lVar22 + 0x2b0);
      if ((lVar8 == 0) || (cVar3 = FUN_14039b950(lVar8,0x22,1), cVar3 == '\0')) {
        cVar3 = FUN_14039b950(lVar22,0x22,1);
        if (cVar3 == '\0') goto LAB_1403aa870;
        if (lVar8 == 0) {
          lVar8 = lVar22;
        }
      }
      lVar22 = *(longlong *)(lVar8 + 0x100);
      if (lVar22 != 0) {
        puVar9 = puVar13;
        if (local_100 != 0) {
          pppppppuVar17 = local_110;
          if (0xf < local_f8) {
            pppppppuVar17 = (undefined8 *******)local_110[0];
          }
          puVar9 = (undefined *)FUN_140394d40(param_1,pppppppuVar17);
        }
        plVar7 = *(longlong **)(lVar22 + 0x280);
        if (((plVar7 != (longlong *)0x0) &&
            ((plVar7 == param_1 || (plVar7 == (longlong *)param_1[0x56])))) &&
           ((*(int *)(lVar22 + 0x288) == -1 ||
            ((puVar9 != (undefined *)0x0 && (*(int *)(puVar9 + 0x18) == *(int *)(lVar22 + 0x288)))))
           )) {
          if (*(longlong *)(lVar8 + 0x100) != 0) {
            *(undefined1 *)(*(longlong *)(lVar8 + 0x100) + 0x28c) = 1;
          }
          uVar11 = CONCAT44(uVar21,0x3fffff);
          local_120 = 0;
          local_118 = 0;
          in_stack_fffffffffffffe88 =
               (undefined *)CONCAT44((int)((ulonglong)in_stack_fffffffffffffe88 >> 0x20),0xffffffff)
          ;
          FUN_14038a350(lVar8,0x22,0,&local_120,in_stack_fffffffffffffe88,uVar11);
          uVar21 = (undefined4)((ulonglong)uVar11 >> 0x20);
          cVar3 = FUN_14043fa90(&DAT_140b2f2e0,0x22);
          if ((cVar3 == '\0') && (*(char *)(lVar8 + 0x348) != -1)) {
            if (*(longlong *)(lVar8 + 0x110) == 0) goto LAB_1403aa895;
            uVar11 = FUN_1404ec820();
            lVar10 = FUN_140397d90(uVar11,0x22);
          }
          else {
            lVar10 = *(longlong *)(lVar8 + 0x5f0);
          }
          if ((lVar10 != 0) && (param_9 == '\0')) {
            cVar3 = FUN_14043fa90(&DAT_140b2f2e0,0x22);
            if ((cVar3 == '\0') && (*(char *)(lVar8 + 0x348) != -1)) {
              puVar9 = puVar13;
              if (*(longlong *)(lVar8 + 0x110) != 0) {
                uVar11 = FUN_1404ec820();
                puVar9 = (undefined *)FUN_140397d90(uVar11,0x22);
              }
            }
            else {
              puVar9 = *(undefined **)(lVar8 + 0x5f0);
            }
            fVar28 = (float)FUN_140565990(puVar9,8);
            local_res8 = *(int *)(lVar22 + 0x288);
            fVar27 = fVar27 * fVar28;
            bVar25 = true;
            goto LAB_1403aa895;
          }
        }
      }
      goto LAB_1403aa870;
    }
LAB_1403aa895:
    puVar9 = (undefined *)FUN_1402aca60(param_1[0x57],*(undefined4 *)(local_128 + 0x58));
    puVar12 = *(undefined **)(param_4 + 0x298);
  }
  if (param_9 == '\0') {
    fVar28 = (float)FUN_14038e0c0(param_1,puVar9);
    fVar27 = fVar28 * fVar27;
  }
  if (((param_1[0x20] != 0) &&
      (cVar3 = FUN_14037d160(param_1[0x20] + 8,param_4,puVar9,param_13), cVar3 == '\x01')) &&
     (fVar27 = 0.0, param_14 != (undefined1 *)0x0)) {
    *param_14 = 1;
  }
  fVar28 = DAT_1407ffaf8;
  uVar6 = (undefined4)((ulonglong)in_stack_fffffffffffffe88 >> 0x20);
  fVar30 = fVar27;
  if (param_9 == '\0') {
    iVar5 = param_11;
    if (((local_128 == 0) || (fVar27 <= 0.0)) ||
       ((*(char *)((longlong)param_1 + 0x342) == -1 &&
        ((*(char *)((longlong)param_1 + 0x36b) == -1 && (*(char *)((longlong)param_1 + 0x341) == -1)
         ))))) {
      if (((param_2 == 0) || ((local_128 != 0 || (fVar27 <= 0.0)))) ||
         ((*(char *)((longlong)param_1 + 0x342) == -1 &&
          ((*(char *)((longlong)param_1 + 0x36b) == -1 &&
           (*(char *)((longlong)param_1 + 0x341) == -1)))))) goto LAB_1403aaa58;
      if (param_11 == 0) {
        (**(code **)(*DAT_140b31050 + 0x28))(DAT_140b31050,&local_f0,param_2);
        puVar16 = &local_f0;
        if (0xf < local_d8) {
          puVar16 = local_f0;
        }
        iVar5 = FUN_140215a30(puVar16,local_e0 & 0xffffffff,0);
        if (0xf < local_d8) {
          if (0xfff < local_d8 + 1) {
            if (0x1f < (ulonglong)((longlong)local_f0 + (-8 - local_f0[-1]))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(local_f0[-1],local_d8 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        uVar6 = (undefined4)((ulonglong)in_stack_fffffffffffffe88 >> 0x20);
        if (iVar5 == 0) goto LAB_1403aaa58;
      }
    }
    else if (param_11 == 0) {
      iVar5 = *(int *)(local_128 + 0x68);
    }
    in_stack_fffffffffffffe88 = (undefined *)CONCAT44(uVar6,*(undefined4 *)(param_1[0x53] + 0x43c));
    fVar28 = (float)FUN_140395850(param_1,param_4,iVar5,param_1[0x53] + 0x3f8,
                                  in_stack_fffffffffffffe88);
    fVar30 = fVar27 * fVar28;
  }
LAB_1403aaa58:
  if (*(char *)((longlong)param_1 + 0x342) == -1) {
    fVar31 = 0.0;
  }
  else if (param_1[0x1e] == 0) {
    fVar31 = 0.0;
  }
  else {
    fVar31 = *(float *)(param_1[0x1e] + 0xfc);
  }
  fVar1 = *(float *)((longlong)param_1 + 0x5c);
  fVar29 = fVar30;
  if (param_2 != 0x16) {
    if (((*(char *)((longlong)param_1 + 0x36b) != -1) && (0.0 < fVar30)) &&
       (lVar22 = (**(code **)(*param_1 + 0x10))(param_1,0x39), lVar22 != 0)) {
      in_stack_fffffffffffffe88 = param_4;
      puVar33 = param_5;
      fVar29 = (float)FUN_14056ba20(lVar22,param_1,param_2,fVar30,param_4,param_5);
      uVar21 = (undefined4)((ulonglong)puVar33 >> 0x20);
      if ((fVar30 <= fVar29) && (param_6 != (undefined1 *)0x0)) {
        *param_6 = 1;
      }
      fVar30 = fVar30 - fVar29;
    }
    if ((*(char *)((longlong)param_1 + 0x342) != -1) && (0.0 < fVar30)) {
      uVar11 = (**(code **)(*param_1 + 0x10))(param_1,0x10);
      in_stack_fffffffffffffe88 = param_4;
      puVar33 = param_5;
      fVar29 = (float)FUN_140549ac0(uVar11,param_1,param_2,fVar30,param_4,param_5);
      uVar21 = (undefined4)((ulonglong)puVar33 >> 0x20);
      if ((fVar30 <= fVar29) && (param_6 != (undefined1 *)0x0)) {
        *param_6 = 1;
      }
      fVar30 = fVar30 - fVar29;
    }
    if ((*(char *)((longlong)param_1 + 0x341) != -1) && (0.0 < fVar30)) {
      uVar11 = (**(code **)(*param_1 + 0x10))(param_1,0xf);
      in_stack_fffffffffffffe88 = param_4;
      fVar29 = (float)FUN_14056c230(uVar11,param_1,param_2,fVar30,param_4);
      fVar30 = fVar30 - fVar29;
      if ((0.0 < fVar29) && ((puVar12 != (undefined *)0x0 && (puVar12[0x48] != '\0')))) {
        if (param_1[0x15] != 0) {
          FUN_1405545a0(param_1[0x15],0,*(undefined4 *)(puVar12 + 0x4c8));
          if ((param_1[0x15] != 0) && (*(longlong *)(param_1[0x15] + 0x1f0) != 0)) {
            FUN_140422040();
          }
        }
        uVar11 = FUN_140058570();
        FUN_140220ed0(uVar11,param_1 + 7,0x16);
        FUN_1403a7e30(param_1,0);
        FUN_1403a7cf0(param_1,0);
      }
    }
    fVar29 = fVar27;
    if (0.0 < fVar28) {
      fVar29 = fVar30 / fVar28;
    }
  }
  uVar6 = (undefined4)((ulonglong)in_stack_fffffffffffffe88 >> 0x20);
  if (param_9 == '\0') {
    if ((local_128 == 0) || (fVar29 <= 0.0)) {
      if ((param_2 == 0) || (((param_2 == 0x13 || (local_128 != 0)) || (fVar29 <= 0.0))))
      goto LAB_1403aad16;
      if (param_11 == 0) {
        (**(code **)(*DAT_140b31050 + 0x28))(DAT_140b31050,&local_f0,param_2);
        puVar16 = &local_f0;
        if (0xf < local_d8) {
          puVar16 = local_f0;
        }
        param_11 = FUN_140215a30(puVar16,local_e0 & 0xffffffff,0);
        if (0xf < local_d8) {
          if (0xfff < local_d8 + 1) {
            if (0x1f < (ulonglong)((longlong)local_f0 + (-8 - local_f0[-1]))) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(local_f0[-1],local_d8 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        uVar6 = (undefined4)((ulonglong)in_stack_fffffffffffffe88 >> 0x20);
        if (param_11 == 0) goto LAB_1403aad16;
      }
    }
    else if (param_11 == 0) {
      param_11 = *(int *)(local_128 + 0x68);
    }
    in_stack_fffffffffffffe88 = (undefined *)CONCAT44(uVar6,*(undefined4 *)(param_1[0x53] + 0x438));
    fVar27 = (float)FUN_140395850(param_1,param_4,param_11,param_1[0x53] + 0x3d8,
                                  in_stack_fffffffffffffe88);
    fVar29 = fVar29 * fVar27;
    if (param_7 != (float *)0x0) {
      *param_7 = fVar27;
    }
  }
LAB_1403aad16:
  if ((puVar12 != (undefined *)0x0) && (puVar12[0x47] == '\0')) {
    fVar29 = 0.0;
  }
  lVar22 = param_1[0x40];
  if ((lVar22 != 0) || (lVar22 = FUN_140390d90(param_1), lVar22 != 0)) {
    *(undefined1 *)(lVar22 + 8) = 3;
  }
  bVar2 = true;
  if ((local_100 != 0) && (0.0 < fVar29)) {
    if ((param_12 != (undefined8 *******)0x0) && (param_12 != local_110)) {
      pppppppuVar17 = local_110;
      if (0xf < local_f8) {
        pppppppuVar17 = (undefined8 *******)local_110[0];
      }
      FUN_140022730(param_12,pppppppuVar17);
    }
    pppppppuVar17 = local_110;
    if (0xf < local_f8) {
      pppppppuVar17 = (undefined8 *******)local_110[0];
    }
    lVar22 = FUN_140394d40(param_1,pppppppuVar17);
    if (lVar22 == 0) {
      if (((((!bVar25) || (local_res8 != -1)) || (param_1[0x5a] == 0)) ||
          ((*(int *)(param_1[0x5a] + 0x10) < 1 || (param_4 == (undefined *)0x0)))) ||
         (lVar22 = FUN_140394a80(param_1,param_4 + 0x78,0,0), lVar22 == 0)) goto LAB_1403aaf26;
      plVar18 = *(longlong **)(lVar22 + 0x20);
      plVar7 = plVar18 + 0x14;
      if (plVar7 == (longlong *)0x0) {
        plVar23 = (longlong *)&DAT_14085af28;
      }
      else {
        plVar23 = plVar7;
        if (0xf < (ulonglong)plVar18[0x17]) {
          plVar23 = (longlong *)*plVar7;
        }
      }
      if (0xf < (ulonglong)plVar18[3]) {
        plVar18 = (longlong *)*plVar18;
      }
      FUN_140025760("GameObjectClass::Take_Damage -- Lucky shot was targeted at hull, but switched to hardpoint %s (%s)\n"
                    ,plVar18,plVar23);
      if (plVar7 != (longlong *)0x0) {
        FUN_1401dc3c0(param_12,plVar7);
      }
    }
    if (*(char *)(*(longlong *)(lVar22 + 0x20) + 0x4d) == '\x01') {
      bVar25 = false;
      bVar2 = bVar25;
      if (((0.0 < *(float *)(lVar22 + 0x28)) &&
          (in_stack_fffffffffffffe88 = param_4,
          fVar27 = (float)FUN_140387f50(lVar22,param_1,param_2,fVar29,param_4), fVar27 <= 0.0)) &&
         ((*(char *)(param_1[0x53] + 0xa8) == '\x01' && (lVar22 = param_1[0x5a], lVar22 != 0)))) {
        if (0 < *(int *)(lVar22 + 0x10)) {
          plVar7 = *(longlong **)(lVar22 + 8);
          puVar12 = puVar13;
          do {
            lVar8 = *plVar7;
            if (((lVar8 != 0) && (*(char *)(*(longlong *)(lVar8 + 0x20) + 0x4d) == '\x01')) &&
               ((bVar2 = bVar25, *(longlong *)(lVar8 + 0x20) == 0 ||
                (bVar2 = false, 0.0 < *(float *)(lVar8 + 0x28))))) goto LAB_1403aaf26;
            uVar20 = (int)puVar12 + 1;
            puVar12 = (undefined *)(ulonglong)uVar20;
            plVar7 = plVar7 + 1;
          } while ((int)uVar20 < *(int *)(lVar22 + 0x10));
        }
        *(undefined4 *)((longlong)param_1 + 0x5c) = 0;
        bVar2 = true;
      }
    }
  }
LAB_1403aaf26:
  if (((*(char *)(param_1[0x53] + 0x81) == '\0') || (param_2 != 7)) ||
     ((*(byte *)((longlong)param_1 + 0x3a1) & 2) != 0)) {
    if (bVar2) {
      if (local_128 == 0) {
        uVar6 = 0xffffffff;
      }
      else {
        uVar6 = FUN_1402c7990(&DAT_140b15800,*(undefined4 *)(local_128 + 0x68));
      }
      in_stack_fffffffffffffe88 = puVar9;
      FUN_1403ab890(param_1,param_2,fVar29,param_4,puVar9,CONCAT44(uVar21,uVar6),
                    CONCAT44(uVar34,param_10));
    }
  }
  else {
    *(byte *)((longlong)param_1 + 0x3a1) = *(byte *)((longlong)param_1 + 0x3a1) | 2;
    if (*(char *)((longlong)param_1 + 0x35f) == -1) {
      local_d8 = 0xf;
      local_e0 = 7;
      local_f0 = (undefined8 *)
                 (ulonglong)
                 CONCAT16(s_BURNING_140866398[6],
                          CONCAT24(s_BURNING_140866398._4_2_,s_BURNING_140866398._0_4_));
      uVar11 = FUN_1404c3520(&local_f0);
      if (0xf < local_d8) {
        if (0xfff < local_d8 + 1) {
          if (0x1f < ((longlong)local_f0 - *(longlong *)((longlong)local_f0 - 8)) - 8U) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)((longlong)local_f0 - 8),local_d8 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_e0 = 0;
      local_d8 = 0xf;
      local_f0 = (undefined8 *)((ulonglong)local_f0 & 0xffffffffffffff00);
      FUN_14038c570(param_1,uVar11,0);
    }
    if (*(char *)((longlong)param_1 + 0x352) == -1) {
      local_f0 = (undefined8 *)0x0;
      local_e0 = 0;
      local_d8 = 0xf;
      FUN_140022730(&local_f0,"INDIGENOUS_UNIT",0xf);
      uVar11 = FUN_1404c3520(&local_f0);
      if (0xf < local_d8) {
        if (0xfff < local_d8 + 1) {
          if (0x1f < ((longlong)local_f0 - *(longlong *)((longlong)local_f0 - 8)) - 8U) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)((longlong)local_f0 - 8),local_d8 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      local_e0 = 0;
      local_d8 = 0xf;
      local_f0 = (undefined8 *)((ulonglong)local_f0 & 0xffffffffffffff00);
      FUN_14038c570(param_1,uVar11,0);
    }
    FUN_1403727a0(param_1[0x53],param_1);
    FUN_1403a89d0(param_1);
    plVar7 = (longlong *)param_1[0x56];
    if (((plVar7 != (longlong *)0x0) && ((char)plVar7[0x69] != -1)) &&
       (lVar22 = (**(code **)(*plVar7 + 0x10))(plVar7,0x16), lVar22 != 0)) {
      FUN_1404058c0(lVar22,param_1);
    }
    if (((*(char *)(param_1[0x53] + 0x83) == '\0') && (*(char *)((longlong)param_1 + 0x33b) != -1))
       && (plVar7 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,9), plVar7 != (longlong *)0x0
          )) {
      (**(code **)(*plVar7 + 0xd8))(plVar7,1);
    }
  }
  if ((char)param_1[0x6e] != -1) {
    if (*(char *)((longlong)param_1 + 0x342) == -1) {
      fVar27 = 0.0;
    }
    else if (param_1[0x1e] == 0) {
      fVar27 = 0.0;
    }
    else {
      fVar27 = *(float *)(param_1[0x1e] + 0xfc);
    }
    if (0.0 < ((fVar31 + fVar1) - *(float *)((longlong)param_1 + 0x5c)) - fVar27) {
      uVar11 = (**(code **)(*param_1 + 0x10))(param_1,0x3e);
      FUN_14058bc70(uVar11);
    }
  }
  if ((puVar9 != (undefined *)0x0) && ((puVar9[0x3a0] & 0x40) == 0)) {
    if (*(char *)((longlong)param_1 + 0x344) == -1) {
      plVar7 = (longlong *)param_1[0x56];
      if ((plVar7 != (longlong *)0x0) && (*(char *)((longlong)plVar7 + 0x344) != -1)) {
        lVar22 = *plVar7;
        goto LAB_1403ab229;
      }
    }
    else {
      lVar22 = *param_1;
      plVar7 = param_1;
LAB_1403ab229:
      uVar11 = (**(code **)(lVar22 + 0x10))(plVar7,0x12);
      FUN_1404f59d0(uVar11);
    }
    if (*(char *)((longlong)param_1 + 0x373) == -1) {
      plVar7 = (longlong *)param_1[0x56];
      if ((plVar7 != (longlong *)0x0) && (*(char *)((longlong)plVar7 + 0x373) != -1)) {
        lVar22 = *plVar7;
        goto LAB_1403ab269;
      }
    }
    else {
      lVar22 = *param_1;
      plVar7 = param_1;
LAB_1403ab269:
      uVar11 = (**(code **)(lVar22 + 0x10))(plVar7,0x41);
      FUN_14058d930(uVar11,puVar9);
    }
    if (*(char *)((longlong)param_1 + 0x374) == -1) {
      plVar7 = (longlong *)param_1[0x56];
      if ((plVar7 == (longlong *)0x0) || (*(char *)((longlong)plVar7 + 0x374) == -1))
      goto LAB_1403ab2b8;
      lVar22 = *plVar7;
    }
    else {
      lVar22 = *param_1;
      plVar7 = param_1;
    }
    uVar11 = (**(code **)(lVar22 + 0x10))(plVar7,0x42);
    FUN_14058e370(uVar11,puVar9);
  }
LAB_1403ab2b8:
  fVar27 = (float)FUN_140396df0(param_1);
  if ((0.0 < fVar27) && (fVar27 < fVar26)) {
    puVar12 = puVar13;
    if ((param_4 != (undefined *)0x0) &&
       (((puVar12 = param_4, param_4[0x33a] != -1 &&
         (puVar12 = puVar13, *(longlong *)(param_4 + 0xe8) != 0)) &&
        (puVar12 = (undefined *)0x0, *(int *)(*(longlong *)(param_4 + 0xe8) + 0x58) != 0x3fffff))))
    {
      puVar12 = (undefined *)FUN_1402aca60(DAT_140b15418[3]);
    }
    fVar30 = DAT_140b2733c;
    fVar28 = DAT_140b27338;
    iVar5 = FUN_140294a70(&DAT_140a16fd0);
    if ((fVar26 <= fVar30) || (fVar30 < fVar27)) {
      if ((fVar26 <= fVar28) || (fVar28 < fVar27)) goto LAB_1403ab3c4;
      if ((int)param_1[0xb] == iVar5) {
        lVar22 = *(longlong *)(param_1[0x53] + 0x1320);
      }
      else {
        if ((puVar12 == (undefined *)0x0) || (*(int *)(puVar12 + 0x58) != iVar5))
        goto LAB_1403ab3c4;
        lVar22 = *(longlong *)(*(longlong *)(puVar12 + 0x298) + 0x1340);
      }
    }
    else if ((int)param_1[0xb] == iVar5) {
      lVar22 = *(longlong *)(param_1[0x53] + 0x1330);
    }
    else {
      if ((puVar12 == (undefined *)0x0) || (*(int *)(puVar12 + 0x58) != iVar5)) goto LAB_1403ab3c4;
      lVar22 = *(longlong *)(*(longlong *)(puVar12 + 0x298) + 0x1350);
    }
    if (lVar22 != 0) {
      FUN_1402d5290(&DAT_140b27e60,lVar22,0,0,
                    (ulonglong)in_stack_fffffffffffffe88 & 0xffffffff00000000);
    }
  }
LAB_1403ab3c4:
  if (*(float *)((longlong)param_1 + 0x5c) <= 0.0 && *(float *)((longlong)param_1 + 0x5c) != 0.0) {
    puVar12 = puVar13;
    if (param_4 != (undefined *)0x0) {
      puVar12 = (undefined *)FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(param_4 + 0x58));
    }
    if (puVar9 != (undefined *)0x0) {
      puVar13 = (undefined *)FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(puVar9 + 0x58));
    }
    lVar22 = FUN_140294bc0(&DAT_140a16fd0,(int)param_1[0xb]);
    if (lVar22 == 0) {
      pwVar19 = L"None";
    }
    else {
      pwVar19 = (wchar_t *)(lVar22 + 0x28);
      if (7 < *(ulonglong *)(lVar22 + 0x40)) {
        pwVar19 = *(wchar_t **)pwVar19;
      }
    }
    puVar16 = (undefined8 *)(param_1[0x53] + 0xf8);
    if (0xf < *(ulonglong *)(param_1[0x53] + 0x110)) {
      puVar16 = (undefined8 *)*puVar16;
    }
    FUN_140025760("!!! %s(ID: %d, Owner: %S) Health == %f after taking damage !!!\n",puVar16,pwVar19
                  ,(int)param_1[10],(double)*(float *)((longlong)param_1 + 0x5c));
    if (puVar12 == (undefined *)0x0) {
      pwVar19 = L"None";
    }
    else {
      pwVar19 = (wchar_t *)(puVar12 + 0x28);
      if (7 < *(ulonglong *)(puVar12 + 0x40)) {
        pwVar19 = *(wchar_t **)pwVar19;
      }
    }
    if (param_4 == (undefined *)0x0) {
      puVar16 = (undefined8 *)&DAT_14085af28;
      uVar20 = 0x3fffff;
    }
    else {
      uVar20 = *(uint *)(param_4 + 0x50);
      puVar16 = (undefined8 *)(*(longlong *)(param_4 + 0x298) + 0xf8);
      if (0xf < *(ulonglong *)(*(longlong *)(param_4 + 0x298) + 0x110)) {
        puVar16 = (undefined8 *)*puVar16;
      }
    }
    uVar21 = 0x3fffff;
    pcVar15 = "false";
    if (param_8 != '\0') {
      pcVar15 = "true";
    }
    puVar12 = &DAT_14085af28;
    if (param_5 != (undefined *)0x0) {
      puVar12 = param_5;
    }
    uVar32 = (ulonglong)uVar20;
    FUN_140025760("--- Function call was: Take_Damage(%d, %f, %s(ID: %d, Owner: %S), %s, 0x%08X, 0x%08X, %s)\n"
                  ,param_2,(double)param_3,puVar16,uVar32,pwVar19,puVar12,param_6,param_7,pcVar15);
    if (puVar13 == (undefined *)0x0) {
      pwVar24 = L"None";
    }
    else {
      pwVar24 = (wchar_t *)(puVar13 + 0x28);
      if (7 < *(ulonglong *)(puVar13 + 0x40)) {
        pwVar24 = *(wchar_t **)pwVar24;
      }
    }
    if (puVar9 == (undefined *)0x0) {
      puVar16 = (undefined8 *)&DAT_14085af28;
    }
    else {
      uVar21 = *(undefined4 *)(puVar9 + 0x50);
      puVar16 = (undefined8 *)(*(longlong *)(puVar9 + 0x298) + 0xf8);
      if (0xf < *(ulonglong *)(*(longlong *)(puVar9 + 0x298) + 0x110)) {
        puVar16 = (undefined8 *)*puVar16;
      }
    }
    FUN_140025760("--- Attacker Object was %s(ID: %d, Owner: %S)\n",puVar16,uVar21,pwVar24,uVar32,
                  pwVar19,puVar12,param_6,param_7,pcVar15);
  }
LAB_1403ab582:
  if (0xf < local_f8) {
    if (0xfff < local_f8 + 1) {
      if (0x1f < (ulonglong)((longlong)local_110[0] + (-8 - (longlong)local_110[0][-1]))) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(local_110[0][-1],local_f8 + 0x28);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}



