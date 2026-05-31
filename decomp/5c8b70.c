/* FighterLocomotorBehaviorClass::FUN_1405c8b70  RVA 0x5c8b70  size 1073  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FighterLocomotorBehaviorClass::FUN_1405c8b70
          (longlong param_1,float *param_2,undefined4 param_3,undefined4 param_4)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  undefined4 local_res18 [2];
  undefined4 local_res20 [2];
  float local_120;
  float local_11c;
  float local_118;
  float local_110;
  float local_10c;
  float local_108;
  float local_104;
  float local_100;
  float local_fc;
  undefined2 local_f8 [2];
  undefined8 local_f4;
  undefined4 local_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  undefined8 local_dc;
  undefined2 *local_c8;
  undefined8 local_c0;
  undefined1 local_b8;
  undefined4 local_b4;
  undefined1 local_b0 [136];
  
  lVar1 = *(longlong *)(*(longlong *)(DAT_140b15418 + 0x18) + 0x558);
  if (lVar1 != 0) {
    lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8);
    if (lVar2 != 0) {
      local_res18[0] = param_3;
      local_res20[0] = param_4;
      fVar5 = (float)FUN_140372560(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
      fVar6 = DAT_1408007d4;
      lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2b0);
      fVar5 = DAT_1408007d4 / (fVar5 * ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4));
      if (((lVar4 != 0) && (*(longlong *)(lVar4 + 0x110) != 0)) &&
         (lVar4 = FUN_1404ec820(), lVar4 == *(longlong *)(param_1 + 0x28))) {
        fVar5 = fVar5 * fVar6;
      }
      fVar6 = (float)FUN_140776d48(*(float *)(lVar2 + 0x14) * *(float *)(lVar2 + 0x14) +
                                   *(float *)(lVar2 + 0x18) * *(float *)(lVar2 + 0x18) +
                                   *(float *)(lVar2 + 0x1c) * *(float *)(lVar2 + 0x1c));
      lVar4 = *(longlong *)(param_1 + 0x28);
      fVar10 = fVar6 + *(float *)(lVar4 + 0x78);
      fVar9 = *(float *)(lVar4 + 0x80) - fVar6;
      fVar12 = fVar6 + *(float *)(lVar4 + 0x80);
      fVar11 = fVar6 + *(float *)(lVar4 + 0x7c);
      fVar8 = *(float *)(lVar4 + 0x7c) - fVar6;
      fVar6 = *(float *)(lVar4 + 0x78) - fVar6;
      local_108 = (fVar9 + fVar12) * DAT_1408007c0;
      local_10c = (fVar8 + fVar11) * DAT_1408007c0;
      local_104 = ABS(fVar10 - fVar6) * DAT_1408007c0;
      local_110 = (fVar10 + fVar6) * DAT_1408007c0;
      local_100 = ABS(fVar11 - fVar8) * DAT_1408007c0;
      local_fc = ABS(fVar12 - fVar9) * DAT_1408007c0;
      FUN_14020e780(lVar1,&local_110);
      lVar4 = *(longlong *)(param_1 + 0x28);
      local_11c = fVar5 * *(float *)(lVar2 + 0x18) + *(float *)(lVar4 + 0x7c);
      local_118 = fVar5 * *(float *)(lVar2 + 0x1c) + *(float *)(lVar4 + 0x80);
      local_120 = fVar5 * *(float *)(lVar2 + 0x14) + *(float *)(lVar4 + 0x78);
      local_ec = _DAT_140800840;
      uStack_e8 = _UNK_140800844;
      uStack_e4 = _UNK_140800848;
      uStack_e0 = _UNK_14080084c;
      local_f8[0] = 0;
      local_f4 = 0x3f800000;
      local_c8 = local_f8;
      local_dc = 0;
      local_c0 = 0;
      local_b8 = 0;
      local_b4 = 1;
      FUN_14014e100(local_b0,(float *)(lVar4 + 0x78),&local_120);
      fVar6 = DAT_140805898;
      for (lVar1 = *(longlong *)(lVar1 + 8); DAT_140805898 = fVar6, lVar1 != 0;
          lVar1 = *(longlong *)(lVar1 + 0x18)) {
        lVar4 = *(longlong *)(lVar1 + 8) + -0x28;
        if (*(longlong *)(lVar1 + 8) == 0) {
          lVar4 = 0;
        }
        cVar3 = FUN_14038f900(lVar4,&local_c8);
        if ((cVar3 != '\0') && (*(char *)((longlong)local_c8 + 1) == '\0')) {
          lVar1 = *(longlong *)(param_1 + 0x28);
          local_120 = *(float *)(lVar1 + 0x78) - *(float *)(lVar4 + 0x78);
          local_118 = *(float *)(lVar1 + 0x80) - *(float *)(lVar4 + 0x80);
          local_11c = *(float *)(lVar1 + 0x7c) - *(float *)(lVar4 + 0x7c);
          if ((local_120 == 0.0) && ((local_11c == 0.0 && (local_118 == 0.0)))) {
            local_11c = 0.0;
            local_118 = 0.0;
            local_120 = DAT_1407ffaf8;
          }
          local_118 = local_118 + *(float *)(lVar1 + 0x80);
          local_11c = local_11c + *(float *)(lVar1 + 0x7c);
          local_120 = local_120 + *(float *)(lVar1 + 0x78);
          uVar7 = FUN_1405ca430(param_1);
          FUN_1405caaf0(param_1,&local_120,uVar7,local_res18,local_res20,&stack0x00000028,1);
          *(undefined4 *)(lVar2 + 0x2b4) = 0xf;
          return 1;
        }
        fVar6 = DAT_140805898;
      }
      if (0 < *(int *)(lVar2 + 0x2b4)) {
        *(int *)(lVar2 + 0x2b4) = *(int *)(lVar2 + 0x2b4) + -1;
        lVar1 = *(longlong *)(param_1 + 0x28);
        fVar5 = *(float *)(lVar2 + 0x18);
        fVar8 = *(float *)(lVar2 + 0x14);
        fVar9 = *(float *)(lVar1 + 0x7c);
        fVar10 = *(float *)(lVar1 + 0x78);
        param_2[2] = *(float *)(lVar2 + 0x1c) * fVar6 + *(float *)(lVar1 + 0x80);
        param_2[1] = fVar5 * fVar6 + fVar9;
        *param_2 = fVar8 * fVar6 + fVar10;
      }
    }
  }
  return 0;
}


