
// ================================================================
// FUN_140240940 — signal dispatcher (how it calls listener vftable[1])
// ================================================================

// ------------------------------------------------------------
// RVA: 0x240940  Name: FUN_140240940  Size: 516 bytes
// ------------------------------------------------------------

void FUN_140240940(longlong param_1,undefined4 param_2,undefined8 param_3)

{
  longlong *plVar1;
  int iVar2;
  __uint64 _Var3;
  longlong lVar4;
  ulonglong uVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  undefined1 uVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong *local_res8;
  undefined4 local_res10 [2];
  undefined8 local_res18;
  longlong local_58;
  undefined8 *puStack_50;
  undefined8 *local_48;
  
  lVar4 = *(longlong *)(param_1 + 0x10);
  if ((lVar4 != 0) &&
     (local_res10[0] = param_2, local_res18 = param_3, FUN_1401cacb0(lVar4,&local_res8,local_res10),
     plVar1 = local_res8, local_res8 != *(longlong **)(lVar4 + 8))) {
    local_48 = (undefined8 *)0x0;
    local_58 = 0;
    puStack_50 = (undefined8 *)0x0;
    _Var3 = (__uint64)*(int *)(local_res8[3] + 0x30);
    if (_Var3 != 0) {
      if (0x1fffffffffffffff < _Var3) {
                    /* WARNING: Subroutine does not return */
        FUN_140022630();
      }
      std::vector<unsigned___int64,class_std::allocator<unsigned___int64>_>::_Reallocate_exactly
                ((vector<unsigned___int64,class_std::allocator<unsigned___int64>_> *)&local_58,_Var3
                );
    }
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + 1;
    iVar6 = 0;
    uVar9 = *(undefined1 *)(param_1 + 8);
    *(undefined1 *)(param_1 + 8) = 0;
    lVar10 = plVar1[3];
    lVar8 = *(longlong *)(lVar10 + 0x10);
    lVar4 = lVar10 + 8;
    if (lVar8 != lVar4) {
      lVar7 = 0;
      do {
        plVar11 = *(longlong **)(lVar8 + 0x18);
        iVar2 = (int)((longlong)puStack_50 - local_58 >> 3);
        if (iVar6 < iVar2) {
          do {
            if (*(longlong **)(local_58 + lVar7 * 8) == plVar11) {
              lVar8 = *(longlong *)(lVar8 + 8);
              if (lVar8 == lVar4) break;
              plVar11 = *(longlong **)(lVar8 + 0x18);
            }
            iVar6 = iVar6 + 1;
            lVar7 = lVar7 + 1;
          } while (lVar7 < iVar2);
        }
        local_res8 = plVar11;
        if (lVar8 == lVar4) break;
        if (puStack_50 == local_48) {
          FUN_140045580(&local_58,puStack_50,&local_res8);
        }
        else {
          *puStack_50 = plVar11;
          puStack_50 = puStack_50 + 1;
        }
        (**(code **)(*plVar11 + 8))(plVar11,param_1,local_res10[0],local_res18);
        if (*(char *)(param_1 + 8) == '\0') {
          lVar8 = *(longlong *)(lVar8 + 8);
        }
        else {
          lVar10 = plVar1[3];
          iVar6 = 0;
          lVar7 = 0;
          uVar9 = 1;
          lVar8 = *(longlong *)(lVar10 + 0x10);
          *(undefined1 *)(param_1 + 8) = 0;
        }
        lVar4 = lVar10 + 8;
      } while (lVar8 != lVar4);
    }
    *(int *)(param_1 + 0xc) = *(int *)(param_1 + 0xc) + -1;
    *(undefined1 *)(param_1 + 8) = uVar9;
    if (local_58 != 0) {
      uVar5 = (longlong)local_48 - local_58 & 0xfffffffffffffff8;
      if (0xfff < uVar5) {
        if (0x1f < (local_58 - *(longlong *)(local_58 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_58 + -8),uVar5 + 0x27);
        }
      }
      thunk_FUN_1407864b8();
    }
  }
  return;
}




// ================================================================
// STRING SEARCH: functions containing 'Scoring' references
// ================================================================

// Symbol: SingletonInstance<class_TheGameScoringManagerClass> at RVA 0xfffffffec0000000 (no fn)
// Symbol: TheGameScoringManagerClass at RVA 0xfffffffec0000000 (no fn)
// Symbol: Singleton<class_TheGameScoringManagerClass> at RVA 0xfffffffec0000000 (no fn)


// ================================================================
// VTABLE SEARCH: 'Scoring' or 'GameObjectManager' vtables
// ================================================================


// ================================================================
// MODE CLASS VTABLES (SpaceMode, LandMode, CinematicMode, GalacticMode)
// ================================================================

