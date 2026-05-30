/* FUN_14028d400  RVA 0x28d400  size 654  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14028d400(longlong param_1,undefined4 param_2)

{
  longlong *plVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  DWORD DVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined8 ****ppppuVar8;
  uint uVar9;
  undefined8 ***local_38 [2];
  longlong local_28;
  ulonglong local_20;
  
  if (DAT_140b0a355 == '\0') {
    if ((0 < *(int *)(param_1 + 0x1f0)) &&
       (iVar2 = *(int *)(param_1 + 0x1f0) + -1, *(int *)(param_1 + 0x1f0) = iVar2, iVar2 == 0)) {
      FUN_14028a4d0();
    }
    if (*(longlong *)(param_1 + 0x38) != 0) {
      uVar6 = FUN_14004fd50();
      FUN_140488660(uVar6);
      (**(code **)(**(longlong **)(param_1 + 0x38) + 0xb0))(*(longlong **)(param_1 + 0x38),param_2);
      FUN_1404908c0(param_1 + 0x180);
      if (*(char *)(param_1 + 0x1c4) != '\0') {
        FUN_14001e680(local_38,param_1 + 0x1a0);
        ppppuVar8 = local_38;
        if (0xf < local_20) {
          ppppuVar8 = (undefined8 ****)local_38[0];
        }
        if ((local_28 == 0x1c) &&
           (iVar2 = memcmp(ppppuVar8,"PENDING_TACTICAL_BATTLE_VOTE",0x1c), iVar2 == 0)) {
          iVar2 = FUN_140490750(param_1 + 0x180,2,0);
          iVar3 = FUN_140490750(param_1 + 0x180,2,1);
          iVar4 = FUN_140294b80(&DAT_140a16fd0);
          if (iVar3 + iVar2 == iVar4) {
            lVar7 = FUN_140769c58(0x50);
            if (lVar7 == 0) {
              uVar6 = 0;
            }
            else {
              uVar6 = FUN_1404adfd0(lVar7);
            }
            FUN_1404ae100(uVar6,iVar2 < 1);
            OutgoingEventQueueClass::vfunc_1(&DAT_140b2ed18,uVar6);
            FUN_1404908b0(param_1 + 0x180);
          }
        }
        if (0xf < local_20) {
          ppppuVar8 = (undefined8 ****)local_38[0];
          if ((0xfff < local_20 + 1) &&
             (ppppuVar8 = (undefined8 ****)local_38[0][-1],
             0x1f < (ulonglong)((longlong)local_38[0] + (-8 - (longlong)ppppuVar8)))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
          thunk_FUN_1407864b8(ppppuVar8);
        }
      }
      if ((((DAT_140b270a1 != '\0') &&
           (plVar1 = *(longlong **)(param_1 + 0x38), plVar1 != (longlong *)0x0)) &&
          (0 < *(int *)(param_1 + 0x2c))) && (*(int *)(plVar1[3] + 0x618) == 0)) {
        if (((*(int *)(param_1 + 0x18) - 1U & 0xfffffffc) == 0) && (*(int *)(param_1 + 0x18) != 3))
        {
          (**(code **)(*plVar1 + 0x270))
                    (plVar1,*(undefined8 *)(param_1 + 0x1d0),*(undefined1 *)(param_1 + 0x1d8),
                     *(undefined4 *)(param_1 + 0x1dc));
          uVar9 = DAT_140b27628 * 1000;
          if ((*(int *)(param_1 + 0x1dc) != 0) &&
             (DVar5 = timeGetTime(), uVar9 < (DVar5 - _DAT_140a7bec0) - *(int *)(param_1 + 0x1dc)))
          {
            FUN_140490ba0(param_1 + 0x180,2);
            *(undefined4 *)(param_1 + 0x1dc) = 0;
          }
        }
        else if ((*(char *)(param_1 + 0x1e2) != '\0') &&
                (lVar7 = DAT_140b2b368 - DAT_140b2b360 >> 3, (int)lVar7 == 0)) {
          *(char *)(param_1 + 0x1e2) = (char)lVar7;
          FUN_140291330(param_1);
        }
      }
    }
  }
  return;
}


