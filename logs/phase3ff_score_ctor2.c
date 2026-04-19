
// ================================================================
// FUN_140485340 — TheGameScoringManagerClass constructor
// ================================================================

// ------------------------------------------------------------
// RVA: 0x485340  Name: FUN_140485340  Size: 28 bytes
// ------------------------------------------------------------

undefined8 * FUN_140485340(undefined8 *param_1)

{
  *param_1 = TheGameScoringManagerClass::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  *(undefined2 *)(param_1 + 3) = 0;
  return param_1;
}




// ================================================================
// FUN_140488ab0 — GameScoringManager shutdown (called in deinit)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x488ab0  Name: FUN_140488ab0  Size: 56 bytes
// ------------------------------------------------------------

void FUN_140488ab0(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x10) != 0) {
    FUN_1402488e0();
    if (*(longlong **)(param_1 + 0x10) != (longlong *)0x0) {
      (**(code **)(**(longlong **)(param_1 + 0x10) + 8))();
      *(undefined8 *)(param_1 + 0x10) = 0;
    }
  }
  *(undefined1 *)(param_1 + 0x18) = 0;
  return;
}




// ================================================================
// SEARCHING FOR TheGameScoringManagerClass VTABLE
// ================================================================

// Symbol: FUN_140485340 at RVA 0x485340
// Symbol: FUN_140488ab0 at RVA 0x488ab0
// Symbol: s_TheGameScoringManagerClass::Get(_140801fc0 at RVA 0x801fc0
// Symbol: s_TheGameScoringManagerClass::Relo_1408742d0 at RVA 0x8742d0
// Symbol: s_TheGameScoringManagerClass::Save_140874308 at RVA 0x874308
// Symbol: s_TheGameScoringManagerClass::Load_140874330 at RVA 0x874330
// Symbol: s_TheGameScoringManagerClass::Serv_140874358 at RVA 0x874358
// Symbol: s_TheGameScoringManagerClass::Atta_140874398 at RVA 0x874398
// Symbol: s_TheGameScoringManagerClass::Get__1408743d8 at RVA 0x8743d8
// Symbol: s_GameScoring_--_Listening_to_new_O_140874418 at RVA 0x874418
// Symbol: s_TheGameScoringManagerClass::Rese_140874450 at RVA 0x874450
// Symbol: s_TheGameScoringManagerClass::Get__1408744d8 at RVA 0x8744d8
// Symbol: s_TheGameScoringManagerClass::Send_140874518 at RVA 0x874518
// Symbol: s_TheGameScoringManagerClass::Send_140874570 at RVA 0x874570
// Symbol: s_TheGameScoringManagerClass::Send_1408745b0 at RVA 0x8745b0
// Symbol: s_TheGameScoringManagerClass::Play_140874620 at RVA 0x874620
// Symbol: s_TheGameScoringManagerClass::Send_140874650 at RVA 0x874650
// Symbol: s_TheGameScoringManagerClass::Send_1408746c0 at RVA 0x8746c0
// Symbol: s_TheGameScoringManagerClass::Send_140874720 at RVA 0x874720
// Symbol: s_TheGameScoringManagerClass::Send_1408747c8 at RVA 0x8747c8
// Symbol: s_TheGameScoringManagerClass::Get__140874880 at RVA 0x874880
// Symbol: s_TheGameScoringManagerClass::Get__140874940 at RVA 0x874940
// Symbol: s_Get_Game_Scoring_Type_14089a928 at RVA 0x89a928


// ================================================================
// CALLEES OF FUN_140485340 (constructor callees)
// ================================================================


