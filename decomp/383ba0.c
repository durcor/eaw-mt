/* FUN_140383ba0  RVA 0x383ba0  size 963  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_140383ba0(longlong param_1,float *param_2)

{
  double dVar1;
  uint uVar2;
  ulonglong uVar3;
  longlong *plVar4;
  undefined *puVar5;
  float fVar6;
  undefined8 local_d8;
  undefined4 local_d0;
  float local_c8;
  float local_c4;
  float local_c0;
  float local_b8;
  float local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  float local_a4;
  float local_a0;
  float local_9c;
  float local_98;
  float local_94;
  float local_90;
  float local_8c;
  undefined1 local_88 [4];
  float local_84;
  float local_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  uVar2 = *(int *)(*(longlong *)(param_1 + 0x20) + 0x48) - 5;
  uVar3 = (ulonglong)uVar2;
  if ((uVar2 < 6) && (*(longlong *)(param_1 + 0x10) != 0)) {
    if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4e) == '\0') {
      local_48 = _DAT_140800820;
      uStack_40 = _UNK_140800828;
      local_28 = _DAT_140800840;
      uStack_24 = _UNK_140800844;
      uStack_20 = _UNK_140800848;
      uStack_1c = _UNK_14080084c;
      local_d8 = 0;
      local_d0 = 0;
      local_38 = _DAT_140800830;
      uStack_34 = _UNK_140800834;
      uStack_30 = _UNK_140800838;
      uStack_2c = _UNK_14080083c;
      uVar3 = FUN_140385e70(param_1,&local_d8,&local_48,0);
      if ((char)uVar3 != '\0') {
        local_78 = local_48;
        local_68 = local_38;
        uStack_64 = uStack_34;
        uStack_60 = uStack_30;
        local_58 = local_28;
        uStack_54 = uStack_24;
        uStack_50 = uStack_20;
        uStack_70 = CONCAT44((undefined4)local_d8,(int)uStack_40);
        uStack_5c = local_d8._4_4_;
        uStack_4c = local_d0;
        FUN_14022d410(&local_78,&local_b8);
        local_d8 = 0;
        local_d0 = 0;
        local_c8 = local_b8 * *param_2 + local_b4 * param_2[1] + local_b0 * param_2[2] + local_ac;
        local_c4 = local_a8 * *param_2 + local_a4 * param_2[1] + local_a0 * param_2[2] + local_9c;
        local_c0 = local_98 * *param_2 + local_94 * param_2[1] + local_90 * param_2[2] + local_8c;
        FUN_14020acd0(local_88,&local_d8,&local_c8);
        if (DAT_1408524f8 < local_80) {
          local_80 = local_80 - DAT_1408007f4;
        }
        dVar1 = (double)FUN_1407760bc(SUB84((double)local_80,0));
        uVar3 = *(ulonglong *)(param_1 + 0x20);
        if ((float)dVar1 <= *(float *)(uVar3 + 0x200) / DAT_1408007d4) {
          dVar1 = (double)FUN_1407760bc(SUB84((double)local_84,0));
          uVar3 = *(ulonglong *)(param_1 + 0x20);
          if ((float)dVar1 <= *(float *)(uVar3 + 0x204) / DAT_1408007d4) goto LAB_140383f5c;
        }
      }
    }
    else {
      plVar4 = (longlong *)
               ActionDrivenHeroClashClass::vfunc_4
                         (*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2a0));
      if (plVar4 != (longlong *)0x0) {
        for (puVar5 = (undefined *)(**(code **)(*plVar4 + 0x20))(plVar4); puVar5 != (undefined *)0x0
            ; puVar5 = *(undefined **)(puVar5 + 8)) {
          if (puVar5 == &DAT_140a12370) goto LAB_140383e36;
        }
      }
      plVar4 = (longlong *)0x0;
LAB_140383e36:
      local_78 = _DAT_140800820;
      uStack_70 = _UNK_140800828;
      local_58 = _DAT_140800840;
      uStack_54 = _UNK_140800844;
      uStack_50 = _UNK_140800848;
      uStack_4c = _UNK_14080084c;
      local_68 = _DAT_140800830;
      uStack_64 = _UNK_140800834;
      uStack_60 = _UNK_140800838;
      uStack_5c = _UNK_14080083c;
      FUN_14012d2c0(plVar4,*(undefined4 *)(param_1 + 0x2c),&local_78);
      FUN_14022d410(&local_78,&local_b8);
      local_d8 = 0;
      local_d0 = 0;
      local_c8 = local_b8 * *param_2 + local_b4 * param_2[1] + local_b0 * param_2[2] + local_ac;
      local_c4 = local_a8 * *param_2 + local_a4 * param_2[1] + local_a0 * param_2[2] + local_9c;
      local_c0 = local_98 * *param_2 + local_94 * param_2[1] + local_90 * param_2[2] + local_8c;
      FUN_14020acd0(local_88,&local_d8,&local_c8);
      fVar6 = (float)FUN_14020b6d0(local_80 + DAT_1408007ec);
      uVar3 = *(ulonglong *)(param_1 + 0x20);
      if (ABS(fVar6) < *(float *)(uVar3 + 0x4c4) || ABS(fVar6) == *(float *)(uVar3 + 0x4c4)) {
LAB_140383f5c:
        return CONCAT71((int7)(uVar3 >> 8),1);
      }
    }
  }
  return uVar3 & 0xffffffffffffff00;
}


