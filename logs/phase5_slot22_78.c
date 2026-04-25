/* =================================================================
 * Vtable slot 22 (offset 0xb0) for each GameMode subclass
 * Base no-op = FUN_14076af38 (RVA 0x76af38)
 * =================================================================*/

// GalacticModeClass
//   slot 22 (0xb0) → 0x000000014045e030  RVA 0x45e030  FUN_14045e030  (507 bytes)
//   slot 78 (0x270) → 0x000000014045e020  RVA 0x45e020  thunk_FUN_14045db20  (5 bytes)

// SpaceModeClass
//   slot 22 (0xb0) → 0x00000001404d95a0  RVA 0x4d95a0  FUN_1404d95a0  (358 bytes)
//   slot 78 (0x270) → 0x0000000140364f30  RVA 0x364f30  FUN_140364f30  (529 bytes)

// LandModeClass
//   slot 22 (0xb0) → 0x00000001403bb440  RVA 0x3bb440  FUN_1403bb440  (598 bytes)
//   slot 78 (0x270) → 0x0000000140364f30  RVA 0x364f30  FUN_140364f30  (529 bytes)


/* =================================================================
 * FUN_140364f30  RVA 0x364f30  slot 78 base-class — all modes
 * Caller has timeGetTime watchdog immediately after this dispatch,
 * implying the engine expects this can be slow.
 * =================================================================*/

// size: 529 bytes  params: 0  return: undefined


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140364f30(undefined8 param_1,longlong param_2,char param_3,int param_4)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  DWORD DVar4;
  longlong *plVar5;
  longlong lVar6;
  undefined8 *puVar7;
  uint uVar8;
  char *pcVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  int iVar12;
  longlong local_68 [2];
  undefined8 local_58;
  ulonglong local_50;
  longlong local_48 [8];
  
  if (DAT_140b28f48 != 0) {
    local_50 = 7;
    local_68[0] = 0;
    local_58 = 0;
    if (param_2 != 0) {
      lVar1 = *(longlong *)(param_2 + 0x298);
      lVar6 = lVar1 + 0x140;
      if (7 < *(ulonglong *)(lVar1 + 0x158)) {
        lVar6 = *(longlong *)(lVar1 + 0x140);
      }
      FUN_14002d480(local_68,lVar6,*(undefined8 *)(lVar1 + 0x150));
    }
    cVar2 = FUN_14028af60(&DAT_140b153e0);
    if (cVar2 == '\0') {
      pcVar9 = "TEXT_TACTICAL_LAND_BATTLE_IS_PENDING";
      if (param_3 != '\x01') {
        pcVar9 = "TEXT_TACTICAL_SPACE_BATTLE_IS_PENDING";
      }
      plVar5 = (longlong *)FUN_1401fa460(&DAT_140a7bc58,pcVar9);
      plVar11 = plVar5;
      if (7 < (ulonglong)plVar5[3]) {
        plVar11 = (longlong *)*plVar5;
      }
      lVar6 = plVar5[2];
    }
    else {
      FUN_14002d480(local_68,&DAT_1408008b4,2);
      uVar3 = FUN_140294a70(&DAT_140a16fd0);
      lVar6 = FUN_140490780(&DAT_140b15560,uVar3);
      if ((lVar6 == 0) || (*(int *)(lVar6 + 4) == 0)) {
        pcVar9 = "TEXT_MP_TACTICAL_LAND_BATTLE_IS_PENDING";
        if (param_3 == '\0') {
          pcVar9 = "TEXT_MP_TACTICAL_SPACE_BATTLE_IS_PENDING";
        }
      }
      else {
        pcVar9 = "TEXT_WAITING_FOR_OPPONENT";
      }
      puVar7 = (undefined8 *)FUN_1401fa460(&DAT_140a7bc58,pcVar9);
      puVar10 = puVar7;
      if (7 < (ulonglong)puVar7[3]) {
        puVar10 = (undefined8 *)*puVar7;
      }
      FUN_14002d480(local_68,puVar10,puVar7[2]);
      DVar4 = timeGetTime();
      uVar8 = ((DVar4 - _DAT_140a7bec0) - param_4) / 1000;
      if (DAT_140b27628 <= uVar8) {
        uVar8 = DAT_140b27628;
      }
      iVar12 = DAT_140b27628 - uVar8;
      FUN_14002d480(local_68,&DAT_1408005e4,1);
      FUN_140022b30(local_48,0x20,&DAT_1407ffa20,iVar12);
      lVar6 = -1;
      do {
        lVar6 = lVar6 + 1;
      } while (*(short *)((longlong)local_48 + lVar6 * 2) != 0);
      plVar11 = local_48;
    }
    FUN_14002d480(local_68,plVar11,lVar6);
    FUN_1403deac0(&DAT_140b27f60,local_68);
    if (7 < local_50) {
      if (0xfff < local_50 * 2 + 2) {
        if (0x1f < (local_68[0] - *(longlong *)(local_68[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_68[0] + -8),local_50 * 2 + 0x29);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}


