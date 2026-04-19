
// ================================================================
// CALLERS OF FUN_14035cc10 (slot-4 per-frame, ALL modes)
// ================================================================

// (no callers found via direct references)


// ================================================================
// CALLERS OF FUN_14035cb10 (slot-3 per-frame, Space+Land)
// ================================================================

// RVA 0x3b5e10  RefType: UNCONDITIONAL_JUMP  Name: thunk_FUN_14035cb10  Size: 5 bytes


// ================================================================
// FUN_14008ddc0 — post-mode-exit (called when mode ends/transitions)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x8ddc0  Name: FUN_14008ddc0  Size: 312 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14008ddc0(void)

{
  longlong lVar1;
  char cVar2;
  undefined8 uVar3;
  
  if (DAT_140a61d38 != '\0') {
    if (DAT_140a61de0 != 0) {
      FUN_1402d5a30(&DAT_140b27e60,DAT_140a61de0,DAT_1408007c0);
      DAT_140a61de0 = 0;
    }
    if (DAT_140a61df8 != (longlong *)0x0) {
      *(longlong *)(DAT_140b15418 + 0x2a8) = DAT_140a61df8[0x53];
    }
    FUN_1401c1d20(&DAT_140a78510,DAT_140a110d4,1);
    DAT_140a110d4 = 0xffffffff;
    if (DAT_140a61df8 != (longlong *)0x0) {
      (**(code **)(*DAT_140a61df8 + 8))();
      DAT_140a61df8 = (longlong *)0x0;
    }
    if (DAT_140a61d40 != 0) {
      FUN_140271a10();
      lVar1 = DAT_140a61d40;
      if (DAT_140a61d40 != 0) {
        FUN_140271360(DAT_140a61d40);
        thunk_FUN_1407864b8(lVar1,0x20);
      }
      DAT_140a61d40 = 0;
    }
    cVar2 = FUN_1402cd0c0(&DAT_140b27a10);
    if (cVar2 != '\0') {
      uVar3 = FUN_14004fba0();
      FUN_140262490(uVar3,0);
    }
    DAT_140b0a354 = 0;
    DAT_140a61de8 = 0;
    DAT_140a61d38 = '\0';
    DAT_140a61dd4 = 0;
    _DAT_140b27ef8 = 0x3f800000;
    DAT_140a110e0 = 0x3f800000;
  }
  return;
}




// ================================================================
// FUN_1404d9c80 — per-player service object ctor
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4d9c80  Name: FUN_1404d9c80  Size: 28 bytes
// ------------------------------------------------------------

undefined8 * FUN_1404d9c80(undefined8 *param_1)

{
  *(undefined4 *)((longlong)param_1 + 0xc) = 1;
  *param_1 = TheAIClass::vftable;
  *(undefined4 *)(param_1 + 1) = 0x1010101;
  return param_1;
}



