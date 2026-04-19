
// ================================================================
// RAW BYTES AT DAT_140b2b3a8 AND DAT_140b2b2f0
// ================================================================

// DAT_140b2b3a8 @ RVA 0xb2b3a8
//   raw value = 0x0
//   as RVA    = 0xfffffffec0000000

// DAT_140b2b2f0 @ RVA 0xb2b2f0
//   raw value = 0x0
//   as RVA    = 0xfffffffec0000000


// ================================================================
// GET+TICK PAIR A
// ================================================================

// ------------------------------------------------------------
// RVA: 0x25770  Name: FUN_140025770  Size: 139 bytes
// ------------------------------------------------------------

longlong FUN_140025770(void)

{
  longlong lVar1;
  undefined **local_res8 [4];
  
  if (DAT_1409cf058 == 0) {
    lVar1 = FUN_140769c58(0x350);
    if (lVar1 == 0) {
      DAT_1409cf058 = 0;
    }
    else {
      DAT_1409cf058 = FUN_14006ca10(lVar1);
    }
    local_res8[0] = &PTR_vftable_1409cf050;
    if (DAT_140a7bc10 != DAT_140a7bc18) {
      *DAT_140a7bc10 = &PTR_vftable_1409cf050;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
      return DAT_1409cf058;
    }
    FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,local_res8);
  }
  return DAT_1409cf058;
}



// ------------------------------------------------------------
// RVA: 0x6d1d0  Name: FUN_14006d1d0  Size: 643 bytes
// ------------------------------------------------------------

void FUN_14006d1d0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 ****ppppuVar5;
  undefined1 local_res8 [32];
  undefined8 ***local_48 [2];
  longlong local_38;
  ulonglong local_30;
  longlong local_28 [3];
  ulonglong local_10;
  
  FUN_1400736c0();
  if (*(char *)(param_1 + 0x347) != '\0') {
    FUN_140077410(param_1);
    *(undefined1 *)(param_1 + 0x347) = 0;
  }
  iVar1 = *(int *)(param_1 + 0x254);
  if (iVar1 == 2) {
    FUN_14006dc90(param_1);
    goto LAB_14006d426;
  }
  if (iVar1 == 4) {
    lVar2 = SteamInternal_ContextInit(&PTR_FUN_140a10e00);
    puVar4 = (undefined8 *)
             (**(code **)(**(longlong **)(lVar2 + 8) + 0x10))(*(longlong **)(lVar2 + 8),local_res8);
    FUN_140073f20(param_1,*puVar4);
    FUN_1400271b0(*(undefined1 *)(param_1 + 0x344),*(undefined4 *)(param_1 + 0x210));
    *(undefined4 *)(param_1 + 0x254) = 7;
    FUN_140200870(local_28,param_1 + 0x1d0);
    FUN_14002b0e0(local_28,1);
    if (local_10 < 8) goto LAB_14006d426;
    if (0xfff < local_10 * 2 + 2) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 * 2 + 0x29);
      }
    }
  }
  else {
    if (iVar1 != 6) goto LAB_14006d426;
    FUN_1400a7890(3 - (uint)(*(char *)(param_1 + 0x344) != '\0'));
    lVar2 = FUN_14007bfb0();
    if (*(char *)(lVar2 + 0xc) != '\0') {
      uVar3 = FUN_14007bfb0();
      FUN_1400261e0(uVar3);
    }
    FUN_1400271b0(*(undefined1 *)(param_1 + 0x344),*(undefined4 *)(param_1 + 0x210));
    *(undefined4 *)(param_1 + 0x254) = 7;
    FUN_140200870(local_28,param_1 + 0x1d0);
    FUN_14002b0e0(local_28,1);
    if (*(char *)(param_1 + 0x344) == '\0') {
      FUN_140053c10(&PTR_vftable_140a10b18);
      puVar4 = (undefined8 *)FUN_140054ed0(&PTR_vftable_140a10b18);
      if (7 < (ulonglong)puVar4[3]) {
        puVar4 = (undefined8 *)*puVar4;
      }
      FUN_140200710(local_48,puVar4);
      if (local_38 != 0) {
        ppppuVar5 = local_48;
        if (0xf < local_30) {
          ppppuVar5 = (undefined8 ****)local_48[0];
        }
        FUN_140025760("%s -- Broadcasting LAN SaveGame List [%s]\n",
                      "SteamPeerLobbyClass::Send_Save_Game_Info",ppppuVar5);
        FUN_140078040(param_1,9,0,local_48);
      }
      if (0xf < local_30) {
        if (0xfff < local_30 + 1) {
          if (0x1f < (ulonglong)((longlong)local_48[0] + (-8 - (longlong)local_48[0][-1]))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(local_48[0][-1],local_30 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
    }
    if (local_10 < 8) goto LAB_14006d426;
    if (0xfff < local_10 * 2 + 2) {
      if (0x1f < (local_28[0] - *(longlong *)(local_28[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_28[0] + -8),local_10 * 2 + 0x29);
      }
    }
  }
  thunk_FUN_1407864b8();
LAB_14006d426:
  if (*(char *)(param_1 + 0x346) != '\0') {
    FUN_140077070(param_1);
    *(undefined1 *)(param_1 + 0x346) = 0;
  }
  return;
}




// ================================================================
// GET+TICK PAIR B
// ================================================================

// ------------------------------------------------------------
// RVA: 0x67c80  Name: FUN_140067c80  Size: 137 bytes
// ------------------------------------------------------------

longlong FUN_140067c80(void)

{
  longlong lVar1;
  undefined **local_res8 [4];
  
  if (DAT_1409cf0a0 == 0) {
    lVar1 = FUN_140769c58(0x18);
    if (lVar1 == 0) {
      DAT_1409cf0a0 = 0;
    }
    else {
      DAT_1409cf0a0 = FUN_14006a000(lVar1);
    }
    local_res8[0] = &PTR_vftable_1409cf098;
    if (DAT_140a7bc10 != DAT_140a7bc18) {
      *DAT_140a7bc10 = &PTR_vftable_1409cf098;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
      return DAT_1409cf0a0;
    }
    FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,local_res8);
  }
  return DAT_1409cf0a0;
}



// ------------------------------------------------------------
// RVA: 0x6a1e0  Name: FUN_14006a1e0  Size: 37 bytes
// ------------------------------------------------------------

void FUN_14006a1e0(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  longlong *plVar3;
  char cVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong lStackX_8;
  
  lVar1 = *(longlong *)(param_1 + 0x10);
  if (lVar1 == 0) {
    if ((DAT_140a10df8 != '\0') && (DAT_140a61b38 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x00014006bdd0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      SteamAPI_RunCallbacks();
      return;
    }
    return;
  }
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140a61b68);
  lVar5 = SteamInternal_ContextInit(&PTR_FUN_140a10e00);
  if (*(longlong *)(lVar5 + 8) != 0) {
    lVar5 = SteamInternal_ContextInit(&PTR_FUN_140a10e00);
    cVar4 = (**(code **)(**(longlong **)(lVar5 + 8) + 8))();
    if (cVar4 != '\0') {
      cVar4 = '\x01';
      goto LAB_14006b9a1;
    }
  }
  cVar4 = '\0';
LAB_14006b9a1:
  if (DAT_140a61b94 != cVar4) {
    DAT_140a61b94 = cVar4;
  }
  SteamAPI_RunCallbacks();
  EnterCriticalSection((LPCRITICAL_SECTION)&DAT_140a61b68);
  plVar6 = (longlong *)**(longlong **)(lVar1 + 0x10);
  if (plVar6 != *(longlong **)(lVar1 + 0x10)) {
    do {
      lVar5 = plVar6[4];
      FUN_140202370(&lStackX_8,0,0);
      lStackX_8 = lVar5;
      if (plVar6[4] != *(longlong *)(lVar1 + 0x20)) {
        lVar5 = SteamInternal_ContextInit(&PTR_FUN_140a10e00);
        cVar4 = (**(code **)(**(longlong **)(lVar5 + 0x40) + 0x30))();
        if (cVar4 == '\0') {
          FUN_140206d90(DAT_140a7c490,&lStackX_8);
          FUN_14006bc00(lVar1,plVar6[4]);
          break;
        }
      }
      plVar2 = (longlong *)plVar6[2];
      if (*(char *)((longlong)plVar2 + 0x19) == '\0') {
        cVar4 = *(char *)(*plVar2 + 0x19);
        plVar6 = plVar2;
        plVar2 = (longlong *)*plVar2;
        while (cVar4 == '\0') {
          cVar4 = *(char *)(*plVar2 + 0x19);
          plVar6 = plVar2;
          plVar2 = (longlong *)*plVar2;
        }
      }
      else {
        cVar4 = *(char *)(plVar6[1] + 0x19);
        plVar3 = (longlong *)plVar6[1];
        plVar2 = plVar6;
        while ((plVar6 = plVar3, cVar4 == '\0' && (plVar2 == (longlong *)plVar6[2]))) {
          cVar4 = *(char *)(plVar6[1] + 0x19);
          plVar3 = (longlong *)plVar6[1];
          plVar2 = plVar6;
        }
      }
    } while (plVar6 != *(longlong **)(lVar1 + 0x10));
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140a61b68);
                    /* WARNING: Could not recover jumptable at 0x00014006bacb. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_140a61b68);
  return;
}




// ================================================================
// NEW PER-FRAME TICK (FUN_140051d10)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x51d10  Name: FUN_140051d10  Size: 1852 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_140051d10(char *param_1)

{
  char *pcVar1;
  int iVar2;
  undefined8 *puVar3;
  undefined4 uVar4;
  DWORD DVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  uint uVar9;
  undefined *puVar10;
  ulonglong uVar11;
  int iVar12;
  longlong local_res8;
  LARGE_INTEGER local_res10;
  longlong local_res18;
  longlong local_res20;
  longlong local_1c8 [2];
  undefined8 local_1b8;
  ulonglong local_1b0;
  undefined1 local_1a8 [16];
  longlong local_198 [3];
  ulonglong local_180;
  undefined1 local_178 [16];
  char local_168 [32];
  CHAR local_148;
  char acStack_147 [263];
  
  if (*param_1 != '\0') {
    DVar5 = timeGetTime();
    uVar4 = DAT_1407ffaf8;
    iVar6 = DVar5 - _DAT_140a7bec0;
    if (999 < (uint)(iVar6 - *(int *)(param_1 + 0x2c))) {
      param_1[0x28] = '\0';
      param_1[0x29] = '\0';
      param_1[0x2a] = '\0';
      param_1[0x2b] = '\0';
      *(int *)(param_1 + 0x2c) = iVar6;
      FUN_1401f9340(local_1a8,0,uVar4,0);
      local_1c8[0] = 0;
      local_1b8 = 0;
      local_1b0 = 7;
      FUN_1400228a0(local_1c8,L"Profile Timings:",0x10);
      FUN_1401f9320(local_178,local_1a8);
      FUN_140052460(param_1,local_1c8,8);
      lVar7 = *(longlong *)(param_1 + 8);
      iVar6 = *(int *)(lVar7 + 0xc);
      if (7 < local_1b0) {
        if (0xfff < local_1b0 * 2 + 2) {
          lVar7 = local_1c8[0] - *(longlong *)(local_1c8[0] + -8);
          if (0x1e < lVar7 - 8U && lVar7 != 0x27) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_1c8[0] + -8),local_1b0 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
        lVar7 = *(longlong *)(param_1 + 8);
      }
      iVar6 = iVar6 + 8 + *(int *)(lVar7 + 0xc);
      iVar12 = 0;
      lVar7 = FUN_1402163c0(&DAT_140a61a60,0);
      local_res18 = 0;
      local_res8 = 0;
      local_res20 = 0;
      if ((lVar7 != 0) && (local_res18 = *(longlong *)(lVar7 + 8), local_res18 != 0)) {
        local_res20 = *(longlong *)(local_res18 + 0x28);
      }
      wsprintfA(&local_148," #      %%par %%tot  Samples  Total   Description");
      local_1c8[0] = 0;
      local_1b8 = 0;
      local_1b0 = 0xf;
      lVar8 = -1;
      do {
        pcVar1 = acStack_147 + lVar8;
        lVar8 = lVar8 + 1;
      } while (*pcVar1 != '\0');
      FUN_140022730(local_1c8,&local_148);
      FUN_140200870(local_198,local_1c8);
      if (0xf < local_1b0) {
        if (0xfff < local_1b0 + 1) {
          lVar8 = local_1c8[0] - *(longlong *)(local_1c8[0] + -8);
          if (0x1e < lVar8 - 8U && lVar8 != 0x27) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_1c8[0] + -8),local_1b0 + 0x28);
          }
        }
        thunk_FUN_1407864b8();
      }
      FUN_1401f9320(local_178,local_1a8);
      FUN_140052460(param_1,local_198,8,iVar6);
      lVar8 = *(longlong *)(param_1 + 8);
      iVar2 = *(int *)(lVar8 + 0xc);
      if (7 < local_180) {
        if (0xfff < local_180 * 2 + 2) {
          lVar8 = local_198[0] - *(longlong *)(local_198[0] + -8);
          if (0x1e < lVar8 - 8U && lVar8 != 0x27) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc(*(longlong *)(local_198[0] + -8),local_180 * 2 + 0x29);
          }
        }
        thunk_FUN_1407864b8();
        lVar8 = *(longlong *)(param_1 + 8);
      }
      iVar6 = iVar6 + iVar2 + *(int *)(lVar8 + 0xc);
      while (lVar7 != 0) {
        local_res10.QuadPart = 0;
        QueryPerformanceFrequency(&local_res10);
        if (iVar12 < 10) {
          FUN_140048ec0(local_168,&DAT_140802468,iVar12);
        }
        else {
          local_168[0] = (char)iVar12 + '7';
          local_168[1] = 0;
        }
        FUN_140216450(lVar7);
        FUN_140215b40(lVar7);
        puVar10 = &DAT_140802640;
        if (*(longlong *)(lVar7 + 0x10) == 0) {
          puVar10 = &DAT_14080263c;
        }
        wsprintfA(&local_148,"%s%s    %3d%% %3d%%    %5d  %5d    %s",local_168,puVar10);
        local_1b0 = 0xf;
        local_1c8[0] = 0;
        local_1b8 = 0;
        lVar8 = -1;
        do {
          pcVar1 = acStack_147 + lVar8;
          lVar8 = lVar8 + 1;
        } while (*pcVar1 != '\0');
        FUN_140022730(local_1c8,&local_148);
        FUN_140200870(local_198,local_1c8);
        if (0xf < local_1b0) {
          if (0xfff < local_1b0 + 1) {
            lVar8 = local_1c8[0] - *(longlong *)(local_1c8[0] + -8);
            if (0x1e < lVar8 - 8U && lVar8 != 0x27) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_1c8[0] + -8),local_1b0 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        FUN_1401f9320(local_178,local_1a8);
        FUN_140052460(param_1,local_198,8,iVar6);
        iVar6 = iVar6 + *(int *)(*(longlong *)(param_1 + 8) + 0xc);
        if (7 < local_180) {
          if (0xfff < local_180 * 2 + 2) {
            lVar8 = local_198[0] - *(longlong *)(local_198[0] + -8);
            if (0x1e < lVar8 - 8U && lVar8 != 0x27) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_198[0] + -8),local_180 * 2 + 0x29);
            }
          }
          thunk_FUN_1407864b8();
        }
        if (*(int *)(lVar7 + 0x20) != 0) {
          local_res8 = local_res8 + *(longlong *)(lVar7 + 0x28);
        }
        iVar12 = iVar12 + 1;
        lVar7 = FUN_1402163c0(&DAT_140a61a60,iVar12);
      }
      if ((local_res18 != 0) && (local_res20 != 0)) {
        iVar12 = *(int *)(*(longlong *)(param_1 + 8) + 0xc);
        wsprintfA(&local_148,"       %3d%%                         Unknown",
                  (ulonglong)(100 - (int)((local_res8 * 100) / local_res20)));
        local_1c8[0] = 0;
        local_1b8 = 0;
        local_1b0 = 0xf;
        lVar7 = -1;
        do {
          lVar8 = lVar7 + 1;
          pcVar1 = acStack_147 + lVar7;
          lVar7 = lVar8;
        } while (*pcVar1 != '\0');
        FUN_140022730(local_1c8,&local_148,lVar8);
        FUN_140200870(local_198,local_1c8);
        if (0xf < local_1b0) {
          if (0xfff < local_1b0 + 1) {
            lVar7 = local_1c8[0] - *(longlong *)(local_1c8[0] + -8);
            if (0x1e < lVar7 - 8U && lVar7 != 0x27) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_1c8[0] + -8),local_1b0 + 0x28);
            }
          }
          thunk_FUN_1407864b8();
        }
        FUN_1401f9320(local_178,local_1a8);
        FUN_140052460(param_1,local_198,8,iVar6 + iVar12);
        if (7 < local_180) {
          if (0xfff < local_180 * 2 + 2) {
            lVar7 = local_198[0] - *(longlong *)(local_198[0] + -8);
            if (0x1e < lVar7 - 8U && lVar7 != 0x27) {
                    /* WARNING: Subroutine does not return */
              FUN_1407711cc(*(longlong *)(local_198[0] + -8),local_180 * 2 + 0x29);
            }
          }
          thunk_FUN_1407864b8();
        }
      }
      iVar6 = *(int *)(param_1 + 0x20);
      iVar12 = *(int *)(param_1 + 0x28);
      if (iVar12 < iVar6) {
        do {
          puVar3 = *(undefined8 **)(*(longlong *)(param_1 + 0x18) + (longlong)iVar12 * 8);
          if (puVar3 != (undefined8 *)0x0) {
            (**(code **)*puVar3)(puVar3,1);
            iVar12 = *(int *)(param_1 + 0x28);
            iVar6 = *(int *)(param_1 + 0x20);
          }
          if ((-1 < iVar12) && (iVar12 < iVar6)) {
            uVar9 = (iVar6 - iVar12) - 1;
            if (uVar9 != 0) {
              if ((*(uint *)(param_1 + 0x24) & 0x80000000) == 0) {
                if (0 < (int)uVar9) {
                  uVar11 = (ulonglong)uVar9;
                  if (iVar12 + 1 < iVar12) {
                    lVar8 = (longlong)(int)(iVar12 + 1 + uVar9) + -1;
                    lVar7 = lVar8 * 8;
                    do {
                      lVar7 = lVar7 + -8;
                      *(undefined8 *)
                       (*(longlong *)(param_1 + 0x18) + ((int)(iVar12 + uVar9) - lVar8) * 8 + lVar7)
                           = *(undefined8 *)(*(longlong *)(param_1 + 0x18) + 8 + lVar7);
                      uVar11 = uVar11 - 1;
                    } while (uVar11 != 0);
                  }
                  else {
                    lVar7 = ((longlong)iVar12 + 1) * 8;
                    do {
                      lVar7 = lVar7 + 8;
                      *(undefined8 *)
                       (*(longlong *)(param_1 + 0x18) +
                        ((longlong)iVar12 - ((longlong)iVar12 + 1)) * 8 + -8 + lVar7) =
                           *(undefined8 *)(*(longlong *)(param_1 + 0x18) + -8 + lVar7);
                      uVar11 = uVar11 - 1;
                    } while (uVar11 != 0);
                  }
                }
              }
              else {
                FUN_14076bd80(*(longlong *)(param_1 + 0x18) + (longlong)iVar12 * 8,
                              *(longlong *)(param_1 + 0x18) + ((longlong)iVar12 + 1) * 8,
                              (longlong)(int)uVar9 << 3);
              }
            }
            *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + -1;
            iVar6 = *(int *)(param_1 + 0x20);
          }
          iVar12 = *(int *)(param_1 + 0x28);
        } while (iVar12 < iVar6);
      }
      FUN_140215d70();
      FUN_1401f9440(local_1a8);
    }
  }
  return;
}




// ================================================================
// SCENE-MGR CALL FROM CAMERA (FUN_1402e7430)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2e7430  Name: FUN_1402e7430  Size: 112 bytes
// ------------------------------------------------------------

undefined8 FUN_1402e7430(longlong param_1)

{
  undefined4 uVar1;
  char cVar2;
  ulonglong uVar3;
  undefined4 local_res8;
  undefined4 local_resc;
  ulonglong local_res10 [3];
  
  local_res8 = *(undefined4 *)(param_1 + 0x212c);
  local_resc = *(undefined4 *)(param_1 + 0x2130);
  local_res10[0] = 0;
  cVar2 = FUN_1402fbc70(local_res8,&local_res8,local_res10);
  if (cVar2 != '\0') {
    uVar1 = 1;
    if (*(int *)(param_1 + 0x211c) != 1) {
      uVar1 = 0xe2;
    }
    uVar3 = FUN_140347b50(uVar1);
    if (local_res10[0] == uVar3) {
      return 1;
    }
  }
  return 0;
}




// ================================================================
// PLAYER REGISTRY TICK (FUN_1402089e0)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2089e0  Name: FUN_1402089e0  Size: 12 bytes
// ------------------------------------------------------------

void FUN_1402089e0(void)

{
  FUN_140207e50(&DAT_140a7c4e0);
  return;
}




// ================================================================
// PER-FRAME TICK FUN_1403b08c0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x3b08c0  Name: FUN_1403b08c0  Size: 257 bytes
// ------------------------------------------------------------

void FUN_1403b08c0(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  char cVar4;
  longlong *plVar5;
  
  if (DAT_140b153f8 == 0) {
LAB_1403b092f:
    iVar1 = *(int *)(param_1 + 0x18);
    while (0 < iVar1) {
      plVar2 = *(longlong **)(param_1 + 8);
      if (plVar2 != (longlong *)0x0) {
        plVar5 = plVar2;
        if (plVar2[2] != 0) {
          *(undefined8 *)(plVar2[2] + 0x18) = 0;
          plVar5 = *(longlong **)(param_1 + 8);
        }
        lVar3 = plVar5[2];
        *(longlong *)(param_1 + 8) = lVar3;
        if (lVar3 == 0) {
          *(undefined8 *)(param_1 + 0x10) = 0;
        }
        plVar2[2] = 0;
        plVar2[3] = 0;
        *(int *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + -1;
        cVar4 = (**(code **)(*plVar2 + 0x38))(plVar2);
        if (cVar4 != '\0') {
          *(undefined4 *)(plVar2 + 5) = DAT_140b0a320;
        }
      }
      FUN_1404b3a80(&DAT_140b15690,plVar2);
      (**(code **)(*plVar2 + 8))(plVar2);
      iVar1 = *(int *)(param_1 + 0x18);
    }
  }
  else {
    if ((DAT_140b153f8 != 1) && (DAT_140b153f8 != 2)) {
      if (DAT_140b153f8 == 3) goto LAB_1403b092f;
      if (DAT_140b153f8 != 4) {
        return;
      }
    }
    if (((0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) && (DAT_140b15418 != 0)) &&
       (*(char *)(DAT_140b15418 + 0x98) != '\0')) {
      FUN_1403b03e0(param_1);
      return;
    }
  }
  return;
}



