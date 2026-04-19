
// ================================================================
// GalacticMode ctor (FUN_1404b1270) — vtable base + slot 43
// ================================================================

// RVA: 0x4b1270  Name: FUN_1404b1270  Size: 122 bytes

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
// SpaceMode ctor (FUN_1403b5210) — first 40 lines
// ================================================================

// RVA: 0x3b5210  Name: FUN_1403b5210  Size: 355 bytes

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
// ... [truncated at 40 / 51 lines]


// ================================================================
// LandMode ctor (FUN_1404d6ba0) — first 40 lines
// ================================================================

// RVA: 0x4d6ba0  Name: FUN_1404d6ba0  Size: 761 bytes

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
// ... [truncated at 40 / 69 lines]


// ================================================================
// VTABLE SLOT 43 VIA CTOR DATA REFS
// ================================================================

// GalacticMode ctor data refs:
// SpaceMode ctor data refs:
// LandMode ctor data refs:

// ================================================================
// LoadingThreadClass vtable → vftable[1] = actual LoadThread body
// ================================================================


// ================================================================
// FUN_1400c5250 — LoadThread trigger caller (caller of FUN_14008df00)
// ================================================================

// RVA: 0xc5250  Name: FUN_1400c5250  Size: 1553 bytes

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1400c5250(int *param_1)

{
  ulonglong uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char cVar5;
  DWORD DVar6;
  int iVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong *plVar10;
  longlong *plVar11;
  char *local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  cVar5 = FUN_1401c1e90(&DAT_140a78510,DAT_140a118b8);
  if (cVar5 != '\0') {
    iVar7 = *param_1;
    if (iVar7 == 0) {
      FUN_1400c5200();
      FUN_1400e9450();
      return;
    }
    if (iVar7 == 1) {
      if (DAT_140a61fc0 == 0) {
        uVar1 = *(ulonglong *)(param_1 + 8);
        plVar11 = (longlong *)(param_1 + 2);
        plVar10 = plVar11;
        if (0xf < uVar1) {
          plVar10 = (longlong *)*plVar11;
        }
        lVar8 = *(longlong *)(param_1 + 6);
        if ((lVar8 != 0x1d) ||
           (iVar7 = memcmp(plVar10,"IDC_PLAY_FACTION_A_BUTTON_BIG",0x1d), iVar7 != 0)) {
          plVar10 = plVar11;
          if (0xf < uVar1) {
            plVar10 = (longlong *)*plVar11;
          }
          if ((lVar8 != 0x17) ||
             (iVar7 = memcmp(plVar10,"IDC_MAIN_CAMPAIGN_START",0x17), iVar7 != 0)) {
            if (0xf < uVar1) {
              plVar11 = (longlong *)*plVar11;
            }
            if (lVar8 != 0x11) {
              return;
            }
            if (*plVar11 != 0x434e41435f434449) {
              return;
            }
            if (plVar11[1] != 0x4f545455425f4c45) {
              return;
            }
            if ((char)plVar11[2] != 'N') {
              return;
            }
            FUN_1400c5200();
            FUN_1400e9450();
            return;
          }
        }
        DAT_140a61fc0 = FUN_140479210(&DAT_140b31160,&DAT_140a118c0);
        if (DAT_140a61fc0 != 0) {
          local_28[0] = (char *)FUN_140769c58(0x20);
          uVar4 = s_IDC_CANCEL_BUTTON_140815840._12_4_;
          uVar3 = s_IDC_CANCEL_BUTTON_140815840._8_4_;
          uVar2 = s_IDC_CANCEL_BUTTON_140815840._4_4_;
          local_18 = 0x11;
          local_10 = 0x1f;
          *(undefined4 *)local_28[0] = s_IDC_CANCEL_BUTTON_140815840._0_4_;
          *(undefined4 *)(local_28[0] + 4) = uVar2;
          *(undefined4 *)(local_28[0] + 8) = uVar3;
          *(undefined4 *)(local_28[0] + 0xc) = uVar4;
          local_28[0][0x10] = s_IDC_CANCEL_BUTTON_140815840[0x10];
          local_28[0][0x11] = '\0';
// ... [truncated at 80 / 226 lines]


// ================================================================
// FUN_140247310 — yield case 2 handler (Pump_Threads callee)
// ================================================================

// RVA: 0x247310  Name: FUN_140247310  Size: 199 bytes

void FUN_140247310(undefined8 param_1,longlong param_2)

{
  int iVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  uint uVar4;
  
  FUN_1407b9140();
  uVar2 = 0;
  uVar3 = uVar2;
  if (0 < (int)(*(longlong *)(param_2 + 0x18) - *(longlong *)(param_2 + 0x10) >> 3)) {
    do {
      uVar4 = (int)uVar3 + 1;
      FUN_1407b9520(param_1,(double)(int)uVar4);
      FUN_140247700(param_1,*(undefined8 *)(uVar2 + *(longlong *)(param_2 + 0x10)));
      iVar1 = (**(code **)(**(longlong **)(uVar2 + *(longlong *)(param_2 + 0x10)) + 0x28))();
      if (iVar1 == 2) {
        FUN_14024a4b0(*(longlong *)(param_2 + 0x10) + (longlong)(int)uVar3 * 8);
      }
      FUN_1407b9a60(param_1,0xfffffffd);
      uVar2 = uVar2 + 8;
      uVar3 = (ulonglong)uVar4;
    } while ((int)uVar4 < (int)(*(longlong *)(param_2 + 0x18) - *(longlong *)(param_2 + 0x10) >> 3))
    ;
  }
  return;
}


// ================================================================
// Pump_Threads callees (for shared-state survey)
// ================================================================

// Direct callees of Pump_Threads:
//   RVA 0x246940  FUN_140246940  395b
// RVA: 0x246940  Name: FUN_140246940  Size: 395 bytes

undefined8 FUN_140246940(undefined8 param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 ***pppuVar3;
  longlong lVar4;
  undefined8 ****ppppuVar5;
  ulonglong uVar6;
  longlong lVar7;
  int iVar8;
  undefined8 ***local_28 [2];
  undefined8 local_18;
  ulonglong local_10;
  
  if (DAT_140b09d40 != (code *)0x0) {
    iVar1 = FUN_1407b8ef0();
    iVar8 = 1;
    if (0 < iVar1) {
      lVar7 = 0;
      do {
        lVar2 = FUN_1407b9cc0(param_1,iVar8);
        if (lVar2 != 0) {
          FUN_140252be0(param_1,0);
          local_28[0] = (undefined8 ****)0x0;
          local_18 = 0;
          local_10 = 0xf;
          pppuVar3 = (undefined8 ***)FUN_140769c58(0x2037);
          if (pppuVar3 == (undefined8 ***)0x0) {
// ... [truncated at 30 / 76 lines]

//   RVA 0x7b9310  thunk_FUN_1407bc310  5b
// RVA: 0x7b9310  Name: thunk_FUN_1407bc310  Size: 5 bytes

int thunk_FUN_1407bc310(longlong param_1,int param_2,undefined8 param_3)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  char *pcVar5;
  undefined8 uVar6;
  int aiStackX_10 [2];
  
  puVar2 = *(undefined8 **)(param_1 + 0x58);
  aiStackX_10[0] = param_2;
  if (puVar2 == *(undefined8 **)(param_1 + 0x80)) {
    if ((longlong)param_2 < *(longlong *)(param_1 + 0x10) - *(longlong *)(param_1 + 0x18) >> 4) {
LAB_1407bc3d4:
      uVar1 = *(undefined1 *)(param_1 + 0x8d);
      uVar6 = *(undefined8 *)(param_1 + 200);
      *(undefined8 *)(param_1 + 200) = param_3;
      iVar4 = FUN_1407bc0d0(param_1,FUN_1407bc5e0,aiStackX_10);
      if (iVar4 != 0) {
        *(undefined8 **)(param_1 + 0x58) = *(undefined8 **)(param_1 + 0x80);
        *(undefined8 *)(param_1 + 0x18) = **(undefined8 **)(param_1 + 0x80);
        *(undefined2 *)(param_1 + 0x8a) = 0;
        FUN_1407b9f90(param_1);
        FUN_1407bc690(param_1,iVar4,*(undefined8 *)(param_1 + 0x18));
        *(undefined1 *)(param_1 + 0x8d) = uVar1;
        FUN_1407bc580(param_1);
      }
// ... [truncated at 30 / 58 lines]

//   RVA 0x582a0  FUN_1400582a0  138b
// RVA: 0x582a0  Name: FUN_1400582a0  Size: 138 bytes

longlong * FUN_1400582a0(longlong *param_1,longlong *param_2)

{
  ulonglong uVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (param_1 != param_2) {
    uVar1 = param_1[3];
    if (0xf < uVar1) {
      lVar2 = *param_1;
      lVar3 = lVar2;
      if (0xfff < uVar1 + 1) {
        lVar3 = *(longlong *)(lVar2 + -8);
        if (0x1f < (lVar2 - lVar3) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(lVar2 - lVar3,uVar1 + 0x28);
        }
      }
      thunk_FUN_1407864b8(lVar3);
    }
    param_1[3] = 0xf;
    param_1[2] = 0;
    *(undefined1 *)param_1 = 0;
    lVar2 = param_2[1];
    *param_1 = *param_2;
    param_1[1] = lVar2;
    lVar2 = param_2[3];
    param_1[2] = param_2[2];
// ... [truncated at 30 / 37 lines]

//   RVA 0x248d10  FUN_140248d10  131b
// RVA: 0x248d10  Name: FUN_140248d10  Size: 131 bytes

void FUN_140248d10(longlong param_1,undefined8 param_2)

{
  int iVar1;
  
  FUN_1407b94a0(*(undefined8 *)(param_1 + 0x58),"LuaThreadTable",0xe);
  FUN_1407b8e90(*(undefined8 *)(param_1 + 0x58),0xffffd8ef);
  iVar1 = FUN_1407b9e00(*(undefined8 *)(param_1 + 0x58),0xffffffff);
  if (iVar1 == 5) {
    FUN_1407b95e0(*(undefined8 *)(param_1 + 0x58),param_2);
    FUN_1407b9510(*(undefined8 *)(param_1 + 0x58));
    FUN_1407b9a60(*(undefined8 *)(param_1 + 0x58),0xfffffffd);
    FUN_1407b9ab0(*(undefined8 *)(param_1 + 0x58),0xfffffffe);
  }
  return;
}

//   RVA 0x7711cc  FUN_1407711cc  46b
// RVA: 0x7711cc  Name: FUN_1407711cc  Size: 46 bytes

void FUN_1407711cc(void)

{
  FUN_1407710fc();
                    /* WARNING: Subroutine does not return */
  _invoke_watson((wchar_t *)0x0,(wchar_t *)0x0,(wchar_t *)0x0,0,0);
}

//   RVA 0x247700  FUN_140247700  622b
// RVA: 0x247700  Name: FUN_140247700  Size: 622 bytes

void FUN_140247700(undefined8 param_1,longlong *param_2)

{
  undefined8 *puVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  char cVar5;
  undefined4 uVar6;
  int iVar7;
  longlong lVar8;
  undefined8 *puVar9;
  undefined8 uVar10;
  longlong *plVar11;
  longlong lVar12;
  
  uVar6 = (**(code **)(*param_2 + 0x28))(param_2);
  switch(uVar6) {
  case 1:
    FUN_1407b9140(param_1);
    puVar9 = (undefined8 *)param_2[2];
    puVar1 = (undefined8 *)*puVar9;
    while (puVar1 != puVar9) {
      plVar11 = (longlong *)puVar1[4];
      if (plVar11 != (longlong *)0x0) {
        (**(code **)*plVar11)(plVar11);
      }
      plVar2 = (longlong *)puVar1[5];
      if (plVar2 != (longlong *)0x0) {
// ... [truncated at 30 / 123 lines]

//   RVA 0x769c94  thunk_FUN_1407864b8  5b
// RVA: 0x769c94  Name: thunk_FUN_1407864b8  Size: 5 bytes

void thunk_FUN_1407864b8(void)

{
  _free_base();
  return;
}

//   RVA 0x2473e0  FUN_1402473e0  754b
// RVA: 0x2473e0  Name: FUN_1402473e0  Size: 754 bytes

undefined8 * FUN_1402473e0(undefined8 param_1,char param_2,char param_3,undefined1 param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  
  uVar1 = FUN_1407b9e00(param_1,0xffffffff);
  switch(uVar1) {
  default:
    puVar6 = (undefined8 *)FUN_140769c58(0x18);
    if (puVar6 != (undefined8 *)0x0) {
      *(undefined4 *)(puVar6 + 1) = 0;
      puVar6[2] = 0;
      *puVar6 = LuaValue<void*___ptr64,3>::vftable;
      goto LAB_1402476a8;
    }
    break;
  case 1:
    puVar6 = (undefined8 *)FUN_140769c58(0x18);
    if (puVar6 != (undefined8 *)0x0) {
      iVar2 = FUN_1407b9b70(param_1,0xffffffff);
      *(undefined4 *)(puVar6 + 1) = 0;
      *puVar6 = LuaValue<bool,5>::vftable;
      *(bool *)(puVar6 + 2) = iVar2 != 0;
      goto LAB_1402476a8;
// ... [truncated at 30 / 127 lines]

//   RVA 0x2488e0  FUN_1402488e0  754b
// RVA: 0x2488e0  Name: FUN_1402488e0  Size: 754 bytes

void FUN_1402488e0(longlong param_1)

{
  undefined8 uVar1;
  undefined **ppuVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined *puVar6;
  undefined **local_res8;
  
  if (*(longlong *)(param_1 + 0x58) != 0) {
    *(undefined1 *)(param_1 + 0x121) = 1;
    FUN_1402490c0(param_1 + 0x40,0);
    FUN_1407b94a0(*(undefined8 *)(param_1 + 0x58),"LuaThreadTable",0xe);
    FUN_1407b9510(*(undefined8 *)(param_1 + 0x58));
    FUN_1407b9a60(*(undefined8 *)(param_1 + 0x58),0xffffd8ef);
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
// ... [truncated at 30 / 105 lines]

