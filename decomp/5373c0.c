/* RevealBehaviorClass::vfunc_6  RVA 0x5373c0  size 666  params 0  ret undefined */


undefined8 RevealBehaviorClass::vfunc_6(longlong param_1,longlong param_2,undefined4 param_3)

{
  float fVar1;
  float fVar2;
  char cVar3;
  longlong lVar4;
  ulonglong uVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  longlong local_48;
  longlong lStack_40;
  longlong local_38;
  
  if (((*(char *)(*(longlong *)(param_2 + 0x298) + 0x58) != '\0') && (DAT_140b15418 != 0)) &&
     (cVar3 = FUN_14035f790(), cVar3 == '\0')) {
    return 0;
  }
  LocomotorCommonClass::vfunc_6(param_1,param_2,param_3);
  if (*(char *)(param_2 + 0x3a7) == '\x01') {
    if (*(longlong *)(param_1 + 0x50) == *(longlong *)(param_1 + 0x58)) {
      return 0;
    }
    iVar6 = 0;
    if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
      do {
        lVar4 = FUN_140294bc0(&DAT_140a16fd0,iVar6);
        if ((lVar4 != 0) && (cVar3 = FUN_14039a2c0(param_2,lVar4,0), cVar3 == '\x01')) {
          FUN_140365760(DAT_140b15418,*(undefined4 *)(lVar4 + 0x4c));
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
    }
    FUN_1401584e0(param_1 + 0x50,0);
    return 0;
  }
  fVar1 = *(float *)(param_2 + 0x78);
  fVar8 = fVar1 - *(float *)(param_1 + 0x40);
  fVar2 = *(float *)(param_2 + 0x7c);
  fVar7 = fVar2 - *(float *)(param_1 + 0x44);
  if (*(char *)(param_1 + 0x68) == '\x01') {
    *(undefined1 *)(param_1 + 0x68) = 0;
  }
  else if (fVar8 * fVar8 + fVar7 * fVar7 < *(float *)(param_1 + 0x48)) goto LAB_14053762f;
  iVar6 = 0;
  local_38 = 0;
  local_48 = 0;
  lStack_40 = 0;
  if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
    do {
      lVar4 = FUN_140294bc0(&DAT_140a16fd0,iVar6);
      if ((lVar4 != 0) && (cVar3 = FUN_14039a2c0(param_2,lVar4,0), cVar3 == '\x01')) {
        if (*(longlong *)(param_1 + 0x50) != *(longlong *)(param_1 + 0x58)) {
          FUN_140365760(DAT_140b15418,*(undefined4 *)(lVar4 + 0x4c));
        }
        if (lStack_40 - local_48 >> 2 != 0) {
          lStack_40 = local_48;
        }
        FUN_14035d1b0(DAT_140b15418,*(undefined4 *)(lVar4 + 0x4c),param_2,&local_48,0);
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  FUN_1401584e0(param_1 + 0x50,0);
  FUN_140536e60(param_1 + 0x50,local_48,lStack_40);
  *(float *)(param_1 + 0x40) = fVar1;
  *(float *)(param_1 + 0x44) = fVar2;
  if (local_48 != 0) {
    uVar5 = local_38 - local_48 & 0xfffffffffffffffc;
    if (0xfff < uVar5) {
      if (0x1f < (local_48 - *(longlong *)(local_48 + -8)) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(*(longlong *)(local_48 + -8),uVar5 + 0x27);
      }
    }
    thunk_FUN_1407864b8();
  }
LAB_14053762f:
  *(undefined1 *)(param_1 + 0x69) = 1;
  return 0;
}


