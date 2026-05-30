/* FUN_140381ff0  RVA 0x381ff0  size 746  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_140381ff0(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  undefined1 local_98 [4];
  float local_94;
  float local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  float local_78;
  undefined4 uStack_74;
  float fStack_70;
  undefined4 uStack_6c;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  float local_48;
  undefined4 uStack_44;
  float fStack_40;
  undefined4 uStack_3c;
  undefined8 local_38;
  undefined8 uStack_30;
  
  lVar2 = FUN_140385cf0();
  uVar3 = 0;
  if (lVar2 != 0) {
    FUN_140383650(param_1,local_98);
    cVar1 = WalkLocomotorBehaviorClass::FUN_140386170(local_98,param_1 + 0x9c);
    if (cVar1 != '\x01') {
      fVar4 = *(float *)(param_1 + 0xa0);
      fVar6 = *(float *)(*(longlong *)(param_1 + 0x20) + 0x4c0);
      if (local_94 <= fVar4) {
        fVar5 = fVar4;
        if ((local_94 < fVar4) && (fVar5 = local_94, local_94 <= fVar4 - fVar6)) {
          fVar5 = fVar4 - fVar6;
        }
      }
      else {
        fVar5 = local_94;
        if (fVar4 + fVar6 <= local_94) {
          fVar5 = fVar4 + fVar6;
        }
      }
      fVar4 = *(float *)(param_1 + 0xa4);
      *(float *)(param_1 + 0xa0) = fVar5;
      if (local_90 <= fVar4) {
        fVar5 = fVar4;
        if ((local_90 < fVar4) && (fVar5 = local_90, local_90 <= fVar4 - fVar6)) {
          fVar5 = fVar4 - fVar6;
        }
      }
      else {
        fVar5 = local_90;
        if (fVar4 + fVar6 <= local_90) {
          fVar5 = fVar4 + fVar6;
        }
      }
      *(float *)(param_1 + 0xa4) = fVar5;
      fVar4 = (float)FUN_14020b6d0(local_90 - fVar5);
      if (fVar4 != 0.0) {
        if (fVar4 <= 0.0) {
          fVar5 = (float)((uint)fVar6 ^ DAT_140800860);
          if ((float)((uint)fVar6 ^ DAT_140800860) <= fVar4) {
            fVar5 = fVar4;
          }
        }
        else {
          fVar5 = fVar6;
          if (fVar4 <= fVar6) {
            fVar5 = fVar4;
          }
        }
        *(float *)(param_1 + 0xa4) = fVar5 + *(float *)(param_1 + 0xa4);
      }
      fVar5 = (float)FUN_14020b6d0(*(undefined4 *)(param_1 + 0xa4));
      uStack_3c = _UNK_14080083c;
      fStack_40 = _UNK_140800838;
      uStack_44 = _UNK_140800834;
      local_48 = _DAT_140800830;
      fVar6 = DAT_1408007dc;
      fVar4 = DAT_1408007d4;
      local_58 = _DAT_140800820;
      uStack_50 = _UNK_140800828;
      local_38 = _DAT_140800840;
      uStack_30 = _UNK_140800848;
      *(float *)(param_1 + 0xa4) = fVar5;
      FUN_1400480f0(&local_58,((fVar6 * fVar4) / DAT_1408007f4) * fVar5);
      HolsterWeaponBehaviorClass::FUN_14012d480(lVar2,*(undefined4 *)(param_1 + 0x94),&local_58);
      if (-1 < *(int *)(param_1 + 0x98)) {
        local_88 = _DAT_140800820;
        uStack_80 = _UNK_140800828;
        fVar6 = ((fVar6 * fVar4) / DAT_1408007f4) * *(float *)(param_1 + 0xa0);
        local_78 = _DAT_140800830;
        uStack_74 = _UNK_140800834;
        fStack_70 = _UNK_140800838;
        uStack_6c = _UNK_14080083c;
        local_68 = _DAT_140800840;
        uStack_60 = _UNK_140800848;
        fVar4 = fVar6;
        if (0x1d000000 < ((uint)fVar6 & 0x7f800000)) {
          fVar4 = (float)FUN_140776650(fVar6);
        }
        fVar5 = DAT_1407ffaf8;
        if (0x1d000000 < ((uint)fVar6 & 0x7f800000)) {
          fVar5 = (float)FUN_140776150(fVar6);
        }
        fVar6 = (float)local_88 * fVar4;
        local_88 = CONCAT44(local_88._4_4_,(float)local_88 * fVar5 - (float)uStack_80 * fVar4);
        uStack_80 = CONCAT44(uStack_80._4_4_,(float)uStack_80 * fVar5 + fVar6);
        fVar6 = fStack_70 * fVar4;
        fStack_70 = fStack_70 * fVar5 + local_78 * fVar4;
        local_78 = local_78 * fVar5 - fVar6;
        fVar6 = (float)local_68 * fVar4;
        local_68 = CONCAT44(local_68._4_4_,(float)local_68 * fVar5 - (float)uStack_60 * fVar4);
        uStack_60 = CONCAT44(uStack_60._4_4_,(float)uStack_60 * fVar5 + fVar6);
        HolsterWeaponBehaviorClass::FUN_14012d480(lVar2,*(undefined4 *)(param_1 + 0x98),&local_88);
      }
    }
    uVar3 = 1;
  }
  return uVar3;
}


