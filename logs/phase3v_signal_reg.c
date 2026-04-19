
// ================================================================
// FUN_1402406c0 — signal subscribe/register
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2406c0  Name: FUN_1402406c0  Size: 505 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1402406c0(longlong param_1,undefined4 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  longlong local_18;
  char local_10;
  
  puVar5 = (undefined8 *)0x0;
  if (*(longlong *)(param_1 + 0x10) == 0) {
    puVar1 = (undefined8 *)FUN_140769c58(0x40);
    puVar4 = puVar5;
    if (puVar1 != (undefined8 *)0x0) {
      *(undefined4 *)((longlong)puVar1 + 4) = 0;
      *(undefined1 *)puVar1 = 0;
      puVar1[1] = 0;
      puVar1[2] = 0;
      lVar2 = FUN_140769c58(0x20);
      *(longlong *)lVar2 = lVar2;
      *(longlong *)(lVar2 + 8) = lVar2;
      puVar1[1] = lVar2;
      puVar1[3] = 0;
      puVar1[4] = 0;
      puVar1[5] = 0;
      puVar1[6] = 7;
      puVar1[7] = 8;
      *(undefined4 *)((longlong)puVar1 + 4) = 0x3f800000;
      FUN_140048250(puVar1 + 3,0x10,puVar1[1]);
      puVar4 = puVar1;
    }
    *(undefined8 **)(param_1 + 0x10) = puVar4;
    uVar3 = FUN_140058570();
    FUN_140221410(uVar3,param_1);
  }
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
               0xd4) < DAT_140b09d28) && (_Init_thread_header(&DAT_140b09d28), DAT_140b09d28 == -1))
  {
    atexit(FUN_1407edcf0);
    _Init_thread_footer(&DAT_140b09d28);
  }
  _DAT_140b09d18 = param_2;
  FUN_140240f50(*(undefined8 *)(param_1 + 0x10),&local_18,&DAT_140b09d18);
  if (local_10 != '\0') {
    puVar4 = (undefined8 *)FUN_140769c58(0x60);
    if (puVar4 != (undefined8 *)0x0) {
      puVar5 = puVar4 + 1;
      *(undefined4 *)(puVar4 + 6) = 0;
      *(undefined4 *)((longlong)puVar4 + 0x34) = 1;
      *(undefined4 *)(puVar4 + 7) = 0xfffffc19;
      puVar4[8] = 0;
      *(undefined4 *)(puVar4 + 10) = 0;
      puVar4[3] = 0;
      puVar4[3] = 0;
      puVar4[2] = 0;
      puVar4[2] = puVar5;
      puVar4[4] = 0;
      puVar4[4] = 0;
      *puVar4 = SignalListClass::vftable;
      puVar4[9] = SignalListClass::vftable;
      puVar4[5] = 0;
      *puVar5 = puVar5;
      puVar4[0xb] = param_1;
      puVar5 = puVar4;
    }
    puVar4 = *(undefined8 **)(local_18 + 0x18);
    if (puVar4 != puVar5) {
      if (puVar4 != (undefined8 *)0x0) {
        (**(code **)(puVar4[9] + 8))(puVar4 + 9);
      }
      *(undefined8 **)(local_18 + 0x18) = puVar5;
      if (puVar5 != (undefined8 *)0x0) {
        (**(code **)puVar5[9])(puVar5 + 9);
      }
    }
  }
  FUN_14020aa60(*(undefined8 *)(local_18 + 0x18),param_3,1);
  return;
}




// ================================================================
// FUN_1401cacb0 — signal lookup (used in FUN_140240940 dispatcher)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1cacb0  Name: FUN_1401cacb0  Size: 193 bytes
// ------------------------------------------------------------

longlong * FUN_1401cacb0(longlong param_1,longlong *param_2,uint *param_3)

{
  uint uVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  int iVar5;
  longlong *plVar6;
  uint uVar7;
  
  uVar1 = *param_3;
  uVar7 = (uVar1 ^ 0xdeadbeef) & 0x7fffffff;
  lVar2 = *(longlong *)(param_1 + 8);
  iVar5 = (uVar7 % 0x1f31d) * 0x41a7 + (uVar7 / 0x1f31d) * -0xb14;
  iVar3 = iVar5 + 0x7fffffff;
  if (-1 < iVar5) {
    iVar3 = iVar5;
  }
  plVar6 = (longlong *)
           (((longlong)iVar3 & *(ulonglong *)(param_1 + 0x30)) * 0x10 +
           *(longlong *)(param_1 + 0x18));
  lVar4 = plVar6[1];
  if (lVar4 != lVar2) {
    iVar3 = *(int *)(lVar4 + 0x10);
    while( true ) {
      if (iVar3 <= (int)uVar1) {
        if (*(int *)(lVar4 + 0x10) < (int)uVar1) {
          lVar4 = 0;
        }
        if (lVar4 == 0) {
          lVar4 = lVar2;
        }
        *param_2 = lVar4;
        return param_2;
      }
      if (lVar4 == *plVar6) break;
      lVar4 = *(longlong *)(lVar4 + 8);
      iVar3 = *(int *)(lVar4 + 0x10);
    }
  }
  *param_2 = lVar2;
  return param_2;
}




// ================================================================
// ALL CALLERS OF FUN_1402406c0 (signal subscription)
// ================================================================

// Caller: RVA 0x2215b0  Name: FUN_1402215b0  Size: 602 bytes
// Caller: RVA 0x220e90  Name: FUN_140220e90  Size: 25 bytes


// ================================================================
// WinMain FUN_14005d990 — last section (should contain game loop)
// ================================================================

// Function size: 7440 bytes, showing lines 400 to 800
// Total lines: 941, outputting 400 to 800
  local_518.hCursor = (HCURSOR)0x0;
  local_518.lpszClassName = "TESTWINCLASS";
  local_518.hIconSm = LoadIconA(local_518.hInstance,&DAT_00000084);
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
// ... [end of window]

