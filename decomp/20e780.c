/* FUN_14020e780  RVA 0x20e780  size 615  params 0  ret undefined */


void FUN_14020e780(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  ulonglong uVar4;
  uint uVar5;
  longlong *plVar6;
  undefined8 uVar7;
  uint uVar8;
  longlong lVar9;
  longlong local_res18;
  longlong local_58;
  longlong *plStack_50;
  longlong *local_48;
  
  lVar3 = *(longlong *)(param_1 + 0x10);
  local_58 = 0;
  plStack_50 = (longlong *)0x0;
  lVar9 = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  local_48 = (longlong *)0x0;
  if (((*(longlong *)(lVar3 + 0x18) == *(longlong *)(lVar3 + 0x20)) &&
      (*(longlong *)(lVar3 + 0x30) == 0)) && (*(longlong *)(lVar3 + 0x38) == 0)) goto LAB_14020e950;
  iVar2 = FUN_14020bb60(param_2,lVar3);
  if (iVar2 == 2) {
    plVar6 = *(longlong **)(lVar3 + 0x18);
    if (plVar6 != *(longlong **)(lVar3 + 0x20)) {
      do {
        if (plStack_50 == local_48) {
          FUN_140045580(&local_58,plStack_50,plVar6);
        }
        else {
          *plStack_50 = *plVar6;
          plStack_50 = plStack_50 + 1;
        }
        plVar6 = plVar6 + 1;
      } while (plVar6 != *(longlong **)(lVar3 + 0x20));
    }
    if (*(longlong *)(lVar3 + 0x30) != 0) {
      FUN_14020fef0(*(longlong *)(lVar3 + 0x30),param_2,&local_58,1);
    }
    lVar3 = *(longlong *)(lVar3 + 0x38);
    if (lVar3 == 0) goto LAB_14020e950;
    uVar7 = 1;
  }
  else {
    if (iVar2 == 1) goto LAB_14020e950;
    uVar4 = *(longlong *)(lVar3 + 0x20) - *(longlong *)(lVar3 + 0x18) >> 3;
    uVar8 = (uint)uVar4;
    if (uVar8 != 0) {
      uVar5 = 1;
      uVar4 = uVar4 & 0xffffffff;
      do {
        lVar1 = *(longlong *)(lVar9 + *(longlong *)(lVar3 + 0x18));
        local_res18 = lVar1;
        if (uVar5 < uVar8) {
          FUN_140216840(*(longlong *)(*(longlong *)(lVar3 + 0x18) + (ulonglong)uVar5 * 8) + 0x2c);
        }
        iVar2 = FUN_14020bb60(param_2,lVar1 + 0x2c);
        if (iVar2 != 1) {
          if (plStack_50 == local_48) {
            FUN_140045580(&local_58,plStack_50,&local_res18);
          }
          else {
            *plStack_50 = lVar1;
            plStack_50 = plStack_50 + 1;
          }
        }
        uVar5 = uVar5 + 1;
        lVar9 = lVar9 + 8;
        uVar4 = uVar4 - 1;
      } while (uVar4 != 0);
    }
    if (*(longlong *)(lVar3 + 0x30) != 0) {
      FUN_14020fef0(*(longlong *)(lVar3 + 0x30),param_2,&local_58,0);
    }
    lVar3 = *(longlong *)(lVar3 + 0x38);
    if (lVar3 == 0) goto LAB_14020e950;
    uVar7 = 0;
  }
  FUN_14020fef0(lVar3,param_2,&local_58,uVar7);
LAB_14020e950:
  uVar4 = 0;
  if ((longlong)plStack_50 - local_58 >> 3 != 0) {
    do {
      lVar3 = *(longlong *)(local_58 + uVar4 * 8);
      uVar4 = (ulonglong)((int)uVar4 + 1);
      *(undefined8 *)(lVar3 + 0x18) = *(undefined8 *)(param_1 + 8);
      *(longlong *)(param_1 + 8) = lVar3;
    } while (uVar4 < (ulonglong)((longlong)plStack_50 - local_58 >> 3));
  }
  DAT_140a7c66c = DAT_140a7c66c + 1;
  if (local_58 != 0) {
    uVar4 = (longlong)local_48 - local_58 & 0xfffffffffffffff8;
    if (0xfff < uVar4) {
      if (0x1f < (local_58 - *(longlong *)(local_58 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_58 + -8),uVar4 + 0x27);
      }
    }
    thunk_FUN_1407864b8();
  }
  return;
}


