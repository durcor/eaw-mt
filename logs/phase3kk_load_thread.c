
// ================================================================
// STRING SEARCH: LoadThread / PacketHandler / NATUtils thread names
// ================================================================

// Sym: "s_LoadThread_140805150" at RVA 0x805150
// Sym: "s_NATUtilsThread_140853530" at RVA 0x853530
// Sym: "s_LuaScriptThread:_Main_State_140855cd0" at RVA 0x855cd0
// Sym: "s_LuaScriptThread:_%s_140855cf0" at RVA 0x855cf0
// Sym: "s_LuaScriptThread:_Main_State_1408567b8" at RVA 0x8567b8
// Sym: "s_LuaScriptThread:_1408567d8" at RVA 0x8567d8

// ================================================================
// MEMORY SCAN: thread name strings → callers
// ================================================================

// String "LoadThread" not found in range 0x860000-0x8d0000
// (wider range) String "LoadThread" at RVA 0x805150
//   Referenced by fn RVA 0x8df00  FUN_14008df00  2340b
// String "PacketHandler" not found in range 0x860000-0x8d0000
// (wider range) String "PacketHandler" at RVA 0x851d50
//   Referenced by fn RVA 0x2059f0  FUN_1402059f0  807b
//   Referenced by fn RVA 0x2059f0  FUN_1402059f0  807b
//   Referenced by fn RVA 0x2059f0  FUN_1402059f0  807b
//   Referenced by fn RVA 0x205de0  FUN_140205de0  707b
//   Referenced by fn RVA 0x205de0  FUN_140205de0  707b
//   Referenced by fn RVA 0x205de0  FUN_140205de0  707b
//   Referenced by fn RVA 0x2056d0  FUN_1402056d0  404b
//   Referenced by fn RVA 0x2056d0  FUN_1402056d0  404b
//   Referenced by fn RVA 0x2056d0  FUN_1402056d0  404b

// ================================================================
// FUN_14022e490 — thread creation wrapper (5 call sites)
// ================================================================

// RVA: 0x22e490  Name: FUN_14022e490  Size: 199 bytes

void FUN_14022e490(longlong param_1)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  int iVar4;
  uintptr_t uVar5;
  int iVar6;
  longlong local_res8 [4];
  undefined4 uStack_c;
  
  *(undefined1 *)(param_1 + 9) = 0;
  *(undefined1 *)(param_1 + 8) = 0;
  local_res8[0] = param_1;
  uVar5 = _beginthreadex((void *)0x0,0,FUN_14022e400,local_res8,0,(uint *)0x0);
  if (uVar5 != 0) {
    *(uintptr_t *)(param_1 + 0x18) = uVar5;
    do {
    } while (*(char *)(param_1 + 8) == '\0');
    uVar3 = *(undefined8 *)(param_1 + 0x10);
    uVar2 = *(undefined4 *)(param_1 + 0x20);
    iVar6 = DAT_140a155ac * 2 >> 1;
    if (iVar6 <= DAT_140a155a8) {
      iVar4 = iVar6 / 10;
      if (iVar6 / 10 < 10) {
        iVar4 = 10;
      }
      FUN_14022eb30(&PTR_vftable_140a15598,iVar4 + iVar6,0,1);
    }
    puVar1 = (undefined8 *)(DAT_140a155a0 + (longlong)DAT_140a155a8 * 0x10);
    *puVar1 = uVar3;
    puVar1[1] = CONCAT44(uStack_c,uVar2);
    DAT_140a155a8 = DAT_140a155a8 + 1;
  }
  return;
}


// ================================================================
// CALL SITES OF FUN_14022e490
// ================================================================

// Call site fn: RVA 0x205de0  FUN_140205de0  707b
// RVA: 0x205de0  Name: FUN_140205de0  Size: 707 bytes

undefined8 FUN_140205de0(longlong param_1,longlong *param_2)

{
  undefined8 *puVar1;
  longlong lVar2;
  DWORD DVar3;
  BOOL BVar4;
  undefined8 uVar5;
  longlong *plVar6;
  longlong *plVar7;
  undefined8 *puVar8;
  char *pcVar9;
  undefined8 *local_res8;
  
  if (*(longlong *)(param_1 + 0x98) != 0) {
    puVar1 = (undefined8 *)(param_1 + 0xd0);
    puVar8 = local_res8;
    if (puVar1 != (undefined8 *)0x0) {
      DVar3 = WaitForSingleObject((HANDLE)*puVar1,10000);
      puVar8 = puVar1;
      if (DVar3 == 0) {
        *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Rebind";
      }
      else {
        pcVar9 = "Unknown";
        if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
          pcVar9 = *(char **)(param_1 + 0xd8);
        }
        FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                      ,"PacketHandlerClass::Rebind",pcVar9);
      }
    }
    if (*(longlong **)(param_1 + 0x98) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x98) + 8))();
      *(undefined8 *)(param_1 + 0x98) = 0;
    }
    if (puVar8 != (undefined8 *)0x0) {
      uVar5 = puVar8[1];
      puVar8[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*puVar8);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",uVar5);
      }
    }
  }
  FUN_14022e560(param_1);
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  lVar2 = *(longlong *)(param_1 + 0x90);
  while (lVar2 != 0) {
    FUN_140209540(&DAT_140a14b80,*(undefined8 *)(**(longlong **)(param_1 + 0x88) + 0x10));
    plVar6 = (longlong *)**(longlong **)(param_1 + 0x88);
    lVar2 = *plVar6;
    *(longlong *)(param_1 + 0x90) = *(longlong *)(param_1 + 0x90) + -1;
    *(longlong *)plVar6[1] = lVar2;
    *(longlong *)(lVar2 + 8) = plVar6[1];
    lVar2 = plVar6[2];
    if (lVar2 != 0) {
// ... [truncated at 60 / 132 lines]

// Call site fn: RVA 0x2056d0  FUN_1402056d0  404b
// RVA: 0x2056d0  Name: FUN_1402056d0  Size: 404 bytes

bool FUN_1402056d0(longlong param_1,short param_2,char param_3)

{
  longlong *plVar1;
  longlong lVar2;
  DWORD DVar3;
  int iVar4;
  BOOL BVar5;
  longlong *plVar6;
  longlong *plVar7;
  longlong *plVar8;
  char *pcVar9;
  bool bVar10;
  longlong *local_res8;
  int local_28;
  short local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_18;
  
  plVar1 = (longlong *)(param_1 + 0xd0);
  plVar8 = local_res8;
  if (plVar1 != (longlong *)0x0) {
    DVar3 = WaitForSingleObject((HANDLE)*plVar1,10000);
    plVar8 = plVar1;
    if (DVar3 == 0) {
      *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Init";
    }
    else {
      pcVar9 = "Unknown";
      if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
        pcVar9 = *(char **)(param_1 + 0xd8);
      }
      FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                    ,"PacketHandlerClass::Init",pcVar9);
    }
  }
  FUN_140202370(&local_28,0,0);
  local_28 = 0;
  local_18 = 0x101;
  if (param_2 == 0) {
    param_2 = 0x4d2;
  }
  local_20 = 0x8000;
  *(short *)(param_1 + 0xa0) = param_2;
  local_1c = 0x2000;
  local_24 = param_2;
  if ((param_3 != '\0') && (iVar4 = FUN_140208b10(), iVar4 != 0)) {
    local_28 = iVar4;
  }
  plVar6 = (longlong *)FUN_140227030(&local_res8,&local_28);
  plVar1 = *(longlong **)(param_1 + 0x98);
  plVar7 = (longlong *)*plVar6;
  if (plVar1 != plVar7) {
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))(plVar1);
      plVar7 = (longlong *)*plVar6;
    }
    *(longlong **)(param_1 + 0x98) = plVar7;
// ... [truncated at 60 / 83 lines]

// Call site fn: RVA 0x2059f0  FUN_1402059f0  807b
// RVA: 0x2059f0  Name: FUN_1402059f0  Size: 807 bytes

undefined8 FUN_1402059f0(longlong param_1,short param_2)

{
  longlong *plVar1;
  longlong lVar2;
  DWORD DVar3;
  BOOL BVar4;
  longlong *plVar5;
  longlong *plVar6;
  longlong *plVar7;
  char *pcVar8;
  longlong *local_res8;
  undefined4 local_38;
  short local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined2 local_28;
  
  if (param_2 == *(short *)(param_1 + 0xa0)) {
    return 1;
  }
  if (*(longlong *)(param_1 + 0x98) != 0) {
    plVar1 = (longlong *)(param_1 + 0xd0);
    plVar7 = local_res8;
    if (plVar1 != (longlong *)0x0) {
      DVar3 = WaitForSingleObject((HANDLE)*plVar1,10000);
      plVar7 = plVar1;
      if (DVar3 == 0) {
        *(char **)(param_1 + 0xd8) = "PacketHandlerClass::Rebind";
      }
      else {
        pcVar8 = "Unknown";
        if (*(char **)(param_1 + 0xd8) != (char *)0x0) {
          pcVar8 = *(char **)(param_1 + 0xd8);
        }
        FUN_140025760("ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)\n"
                      ,"PacketHandlerClass::Rebind",pcVar8);
      }
    }
    if (*(longlong **)(param_1 + 0x98) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x98) + 8))();
      *(undefined8 *)(param_1 + 0x98) = 0;
    }
    if (plVar7 != (longlong *)0x0) {
      lVar2 = plVar7[1];
      plVar7[1] = 0;
      BVar4 = ReleaseMutex((HANDLE)*plVar7);
      if (BVar4 == 0) {
        DVar3 = GetLastError();
        FUN_140025760("ReleaseMutex failed with error code %d - %s (mutex owner is %s)\n",DVar3,
                      "RunDebugBuild",lVar2);
      }
    }
  }
  FUN_14022e560(param_1);
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa8));
  lVar2 = *(longlong *)(param_1 + 0x90);
  while (lVar2 != 0) {
    FUN_140209540(&DAT_140a14b80,*(undefined8 *)(**(longlong **)(param_1 + 0x88) + 0x10));
// ... [truncated at 60 / 142 lines]

// Call site fn: RVA 0x225bf0  FUN_140225bf0  386b
// RVA: 0x225bf0  Name: FUN_140225bf0  Size: 386 bytes

void FUN_140225bf0(longlong param_1,undefined4 param_2,undefined4 param_3,undefined8 param_4)

{
  int iVar1;
  undefined8 *puVar2;
  int iVar3;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa0));
  iVar1 = *(int *)(param_1 + 0xd8);
  iVar3 = *(int *)(param_1 + 0xdc) * 2 >> 1;
  if (iVar3 <= iVar1) {
    iVar1 = iVar3 / 10;
    if (iVar3 / 10 < 10) {
      iVar1 = 10;
    }
    FUN_140021ec0(param_1 + 200,iVar1 + iVar3,0,1);
    iVar1 = *(int *)(param_1 + 0xd8);
  }
  *(undefined4 *)(*(longlong *)(param_1 + 0xd0) + (longlong)iVar1 * 4) = param_2;
  *(int *)(param_1 + 0xd8) = *(int *)(param_1 + 0xd8) + 1;
  iVar1 = *(int *)(param_1 + 0xf0);
  iVar3 = *(int *)(param_1 + 0xf4) * 2 >> 1;
  if (iVar3 <= iVar1) {
    iVar1 = iVar3 / 10;
    if (iVar3 / 10 < 10) {
      iVar1 = 10;
    }
    FUN_140021ec0(param_1 + 0xe0,iVar1 + iVar3,0,1);
    iVar1 = *(int *)(param_1 + 0xf0);
  }
  *(undefined4 *)(*(longlong *)(param_1 + 0xe8) + (longlong)iVar1 * 4) = param_3;
  *(int *)(param_1 + 0xf0) = *(int *)(param_1 + 0xf0) + 1;
  FUN_1402268e0(param_1 + 0xf8,param_4);
  if (*(longlong *)(param_1 + 0x130) == 0) {
    puVar2 = (undefined8 *)FUN_140769c58(0x28);
    if (puVar2 == (undefined8 *)0x0) {
      puVar2 = (undefined8 *)0x0;
      *(undefined8 *)(param_1 + 0x130) = 0;
    }
    else {
      FUN_14022e310(puVar2,"NATUtilsThread");
      *puVar2 = NATUtilsClass::NATUtilsThreadClass::vftable;
      *(undefined8 **)(param_1 + 0x130) = puVar2;
    }
  }
  else {
    if (*(char *)(*(longlong *)(param_1 + 0x130) + 9) == '\0') goto LAB_140225d46;
    FUN_14022e560(*(undefined8 *)(param_1 + 0x130));
    puVar2 = *(undefined8 **)(param_1 + 0x130);
  }
  FUN_14022e490(puVar2);
LAB_140225d46:
                    /* WARNING: Could not recover jumptable at 0x000140225d6b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0xa0));
  return;
}

// Call site fn: RVA 0x8df00  FUN_14008df00  2340b
// RVA: 0x8df00  Name: FUN_14008df00  Size: 2340 bytes

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
// ... [truncated at 60 / 365 lines]


// ================================================================
// LoadThread-referencing functions — decompiled
// ================================================================


// ================================================================
// ThreadLockMutexClass — string search + callers
// ================================================================

// Mutex string at RVA 0x851bf0: "ThreadLockMutexClass -- %s failed to obtain mutex within 10 seco"
//   Ref from RVA 0x33a200  FUN_14033a200  471b
//   Ref from RVA 0x339bc0  FUN_140339bc0  1097b
//   Ref from RVA 0x339bc0  FUN_140339bc0  1097b
//   Ref from RVA 0x339190  FUN_140339190  444b
//   Ref from RVA 0x206940  FUN_140206940  1087b
//   Ref from RVA 0x336a50  FUN_140336a50  475b
//   Ref from RVA 0x336d50  FUN_140336d50  8419b
//   Ref from RVA 0x3360b0  FUN_1403360b0  426b
//   Ref from RVA 0x2062b0  FUN_1402062b0  431b
//   Ref from RVA 0x3362d0  FUN_1403362d0  472b
//   Ref from RVA 0x3365c0  FUN_1403365c0  613b
//   Ref from RVA 0x335830  FUN_140335830  675b
//   Ref from RVA 0x335830  FUN_140335830  675b
//   Ref from RVA 0x205870  FUN_140205870  373b
//   Ref from RVA 0x2059f0  FUN_1402059f0  807b
//   Ref from RVA 0x2059f0  FUN_1402059f0  807b
//   Ref from RVA 0x335ae0  FUN_140335ae0  544b
//   Ref from RVA 0x335d00  FUN_140335d00  942b
//   Ref from RVA 0x205de0  FUN_140205de0  707b
//   Ref from RVA 0x205de0  FUN_140205de0  707b
//   Ref from RVA 0x2054c0  FUN_1402054c0  511b
//   Ref from RVA 0x3355d0  FUN_1403355d0  268b
//   Ref from RVA 0x3356e0  FUN_1403356e0  325b
//   Ref from RVA 0x2056d0  FUN_1402056d0  404b
//   Ref from RVA 0x334ce0  FUN_140334ce0  746b
//   Ref from RVA 0x334110  FUN_140334110  269b
//   Ref from RVA 0x334220  FUN_140334220  266b
//   Ref from RVA 0x334330  FUN_140334330  815b
//   Ref from RVA 0x334660  FUN_140334660  890b
//   Ref from RVA 0x3333e0  FUN_1403333e0  257b
//   Ref from RVA 0x332eb0  FUN_140332eb0  175b
// Mutex string at RVA 0x851c50: "ThreadLockMutexClass -- Failed to obtain mutex within 10 seconds"
//   Ref from RVA 0x21b050  FUN_14021b050  426b
//   Ref from RVA 0x21b210  FUN_14021b210  231b
//   Ref from RVA 0x21a810  FUN_14021a810  498b
//   Ref from RVA 0x21aa10  FUN_14021aa10  447b
//   Ref from RVA 0x21abe0  FUN_14021abe0  417b
//   Ref from RVA 0x21adb0  FUN_14021adb0  238b
//   Ref from RVA 0x21aeb0  FUN_14021aeb0  402b
//   Ref from RVA 0x21a260  FUN_14021a260  952b
//   Ref from RVA 0x21a630  FUN_14021a630  465b
//   Ref from RVA 0x219a20  FUN_140219a20  246b
//   Ref from RVA 0x219b20  FUN_140219b20  263b
//   Ref from RVA 0x219c30  FUN_140219c30  238b
//   Ref from RVA 0x219d30  FUN_140219d30  238b
//   Ref from RVA 0x219e30  FUN_140219e30  209b
//   Ref from RVA 0x219f10  FUN_140219f10  215b
//   Ref from RVA 0x219ff0  FUN_140219ff0  601b
//   Ref from RVA 0x218e40  FUN_140218e40  2941b
//   Ref from RVA 0x217b00  FUN_140217b00  314b
//   Ref from RVA 0x217d50  FUN_140217d50  284b
//   Ref from RVA 0x217e80  FUN_140217e80  4006b
//   Ref from RVA 0x217030  FUN_140217030  389b
//   Ref from RVA 0x2171c0  FUN_1402171c0  273b
//   Ref from RVA 0x2172e0  FUN_1402172e0  444b
//   Ref from RVA 0x2174b0  FUN_1402174b0  238b
//   Ref from RVA 0x2175b0  FUN_1402175b0  392b
//   Ref from RVA 0x216b50  FUN_140216b50  457b
//   Ref from RVA 0x216de0  FUN_140216de0  423b
//   Ref from RVA 0x332eb0  FUN_140332eb0  175b

// ================================================================
// IMPORT CALLS: EnterCriticalSection / WaitForSingleObject
// ================================================================

// Import "EnterCriticalSection" at RVA 0xfffffffec000002a
//   Called by RVA 0x76a100  _Init_thread_wait  99b
//   Called by RVA 0x798fd4  __acrt_lowio_lock_fh  39b
//   Called by RVA 0x74831c  EnterCriticalSection  7b
//   Called by RVA 0x78651c  __acrt_lock  25b
//   Called by RVA 0x78658c  _lock_locales  27b
//   Called by RVA 0x784aa4  FUN_140784aa4  11b
//   Called by RVA 0x21e3e0  FUN_14021e3e0  74b
//   Called by RVA 0x21e430  FUN_14021e430  172b
//   Called by RVA 0x21e4f0  FUN_14021e4f0  172b
//   Called by RVA 0x21e5b0  FUN_14021e5b0  172b
//   Called by RVA 0x21e670  FUN_14021e670  172b
//   Called by RVA 0x21e7b0  FUN_14021e7b0  344b
//   Called by RVA 0x23de60  FUN_14023de60  449b
//   Called by RVA 0x23d1b0  FUN_14023d1b0  215b
//   Called by RVA 0x23d290  FUN_14023d290  128b
//   Called by RVA 0x23d370  FUN_14023d370  1494b
//   Called by RVA 0x23c9a0  FUN_14023c9a0  409b
//   Called by RVA 0x23cb40  FUN_14023cb40  195b
//   Called by RVA 0x23cc10  FUN_14023cc10  169b
//   Called by RVA 0x23ccc0  FUN_14023ccc0  861b
//   ... (truncated at 20)
// Import "EnterCriticalSection" at RVA 0x74831c
//   Called by RVA 0x740b0c  _Lockit  59b
// Import "WaitForSingleObject" at RVA 0xfffffffec00000a3
//   Called by RVA 0x21b050  FUN_14021b050  426b
//   Called by RVA 0x21b210  FUN_14021b210  231b
//   Called by RVA 0x21a810  FUN_14021a810  498b
//   Called by RVA 0x21aa10  FUN_14021aa10  447b
//   Called by RVA 0x21abe0  FUN_14021abe0  417b
//   Called by RVA 0x21adb0  FUN_14021adb0  238b
//   Called by RVA 0x21aeb0  FUN_14021aeb0  402b
//   Called by RVA 0x33a200  FUN_14033a200  471b
//   Called by RVA 0x21a260  FUN_14021a260  952b
//   Called by RVA 0x21a630  FUN_14021a630  465b
//   Called by RVA 0x219a20  FUN_140219a20  246b
//   Called by RVA 0x219b20  FUN_140219b20  263b
//   Called by RVA 0x339bc0  FUN_140339bc0  1097b
//   Called by RVA 0x339bc0  FUN_140339bc0  1097b
//   Called by RVA 0x219c30  FUN_140219c30  238b
//   Called by RVA 0x219d30  FUN_140219d30  238b
//   Called by RVA 0x219e30  FUN_140219e30  209b
//   Called by RVA 0x219f10  FUN_140219f10  215b
//   Called by RVA 0x219ff0  FUN_140219ff0  601b
//   Called by RVA 0x339190  FUN_140339190  444b
//   ... (truncated at 20)
// Import "LeaveCriticalSection" at RVA 0xfffffffec000002b
//   Called by RVA 0x21e3e0  FUN_14021e3e0  74b
//   Called by RVA 0x23de60  FUN_14023de60  449b
//   Called by RVA 0x6c0c0  FUN_14006c0c0  277b
//   Called by RVA 0x23c3a0  FUN_14023c3a0  115b
//   Called by RVA 0x6b950  FUN_14006b950  386b
//   Called by RVA 0x6bc00  FUN_14006bc00  244b
//   Called by RVA 0x76a054  _Init_thread_header  103b
//   Called by RVA 0x2090f0  FUN_1402090f0  447b
//   Called by RVA 0x7990bc  __acrt_lowio_unlock_fh  39b
//   Called by RVA 0x2092c0  FUN_1402092c0  238b
//   Called by RVA 0x4093c0  FUN_1404093c0  1282b
//   Called by RVA 0x209540  FUN_140209540  140b
//   Called by RVA 0x748324  LeaveCriticalSection  7b
//   Called by RVA 0x786570  __acrt_unlock  25b
//   Called by RVA 0x7865a8  FUN_1407865a8  14b
//   Called by RVA 0x205870  FUN_140205870  373b
//   Called by RVA 0x225bf0  FUN_140225bf0  386b
//   Called by RVA 0x205d20  FUN_140205d20  181b
//   Called by RVA 0x784ab0  FUN_140784ab0  11b
//   Called by RVA 0x21e430  FUN_14021e430  172b
//   ... (truncated at 20)
// Import "LeaveCriticalSection" at RVA 0x748324
//   Called by RVA 0x740b84  ~_Lockit  50b
// Import "SetEvent" at RVA 0xfffffffec000008d
//   Called by RVA 0x76a0bc  _Init_thread_notify  65b
