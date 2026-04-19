
// ================================================================
// FUN_1404854e0 — called inside FUN_1404889d0 (object init check)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4854e0  Name: FUN_1404854e0  Size: 536 bytes
// ------------------------------------------------------------

undefined8 FUN_1404854e0(longlong param_1,longlong param_2,char param_3,undefined1 param_4)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_38;
  longlong local_30 [2];
  undefined8 local_20;
  ulonglong local_18;
  
  FUN_14027d370(8,"%s -- name: %s\n","TheGameScoringManagerClass::Attach_Lua_Script",param_2);
  lVar4 = -1;
  do {
    lVar4 = lVar4 + 1;
  } while (*(char *)(param_2 + lVar4) != '\0');
  local_18 = 0xf;
  local_30[0] = 0;
  local_20 = 0;
  FUN_140022730(local_30,param_2);
  if (param_3 == '\0') {
    plVar3 = (longlong *)FUN_140244170(&local_38,local_30,param_4);
    plVar1 = *(longlong **)(param_1 + 0x10);
    plVar2 = (longlong *)*plVar3;
    if (plVar1 != plVar2) {
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 8))(plVar1);
        plVar2 = (longlong *)*plVar3;
      }
      *(longlong **)(param_1 + 0x10) = plVar2;
      if (plVar2 != (longlong *)0x0) {
        (**(code **)*plVar2)();
      }
    }
    if (local_38 != (longlong *)0x0) {
      (**(code **)(*local_38 + 8))();
    }
    if (0xf < local_18) {
      if (0xfff < local_18 + 1) {
        if (0x1f < (local_30[0] - *(longlong *)(local_30[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_30[0] + -8),local_18 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
    if (*(longlong *)(param_1 + 0x10) == 0) {
      return 0;
    }
    FUN_1402567b0();
    FUN_140546c70(*(undefined8 *)(param_1 + 0x10));
    if ((*(char *)(*(longlong *)(param_1 + 0x10) + 0x122) != '\0') &&
       (FUN_1402437c0(*(longlong *)(param_1 + 0x10),&local_38,"Base_Definitions",0,0),
       local_38 != (longlong *)0x0)) {
      (**(code **)(*local_38 + 8))();
    }
    FUN_140244740(*(undefined8 *)(param_1 + 0x10),&DAT_140866674,0,0);
  }
  else {
    plVar2 = (longlong *)FUN_1402440f0(local_30);
    plVar1 = *(longlong **)(param_1 + 0x10);
    if (plVar1 != plVar2) {
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 8))();
      }
      *(longlong **)(param_1 + 0x10) = plVar2;
      if (plVar2 != (longlong *)0x0) {
        (**(code **)*plVar2)(plVar2);
      }
    }
    if (0xf < local_18) {
      if (0xfff < local_18 + 1) {
        if (0x1f < (local_30[0] - *(longlong *)(local_30[0] + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
          FUN_1407711cc(*(longlong *)(local_30[0] + -8),local_18 + 0x28);
        }
      }
      thunk_FUN_1407864b8();
    }
    FUN_1402567b0(*(undefined8 *)(param_1 + 0x10));
    FUN_140546c70(*(undefined8 *)(param_1 + 0x10));
  }
  return 1;
}




// ================================================================
// ALL CALLERS OF FUN_1404889d0 (subscribe to event 0x23)
// ================================================================

// Caller: RVA 0x4b250  Name: FUN_14004b250  Size: 10192 bytes


// ================================================================
// Caller RVA 0x4b250 (10192 bytes)
// ================================================================

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
// ... [truncated at 60 lines, total 1509]

