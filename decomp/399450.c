/* FUN_140399450  RVA 0x399450  size 2512  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float * FUN_140399450(undefined8 param_1,float *param_2,longlong param_3,float *param_4,
                     float *param_5,float param_6)

{
  longlong lVar1;
  bool bVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  char cVar6;
  longlong lVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  undefined1 local_res18 [8];
  undefined8 in_stack_fffffffffffffe38;
  undefined4 uVar21;
  float local_1a8;
  float local_1a4;
  float local_1a0;
  float local_198;
  float local_194;
  float local_190;
  float local_188;
  float local_180;
  float local_178;
  float local_174;
  float local_170;
  float local_168;
  float local_164;
  float local_160;
  float local_15c;
  float local_158;
  float local_150;
  float local_14c;
  float local_148;
  float local_144;
  float local_140;
  float local_13c;
  float local_138;
  float local_134;
  float local_130;
  float local_12c;
  float local_128;
  float local_124;
  float local_120;
  float local_11c;
  float local_118;
  float local_114;
  float local_110;
  float local_10c;
  float local_108;
  float local_104;
  float local_100;
  float local_f8;
  float local_f4;
  float local_f0;
  float local_ec;
  float local_e8;
  float local_e4;
  float local_e0;
  float local_dc;
  float local_d8;
  float local_d4;
  float local_d0;
  float local_cc;
  float local_c8;
  int local_c4;
  
  uVar21 = (undefined4)((ulonglong)in_stack_fffffffffffffe38 >> 0x20);
  lVar1 = *(longlong *)(param_3 + 0xa8);
  fVar13 = *(float *)(param_3 + 0x254);
  local_104 = (float)DAT_140b0a340;
  local_144 = *(float *)(param_3 + 0x264);
  fVar20 = *(float *)(param_3 + 0x274);
  lVar7 = lVar1 + 0x164;
  lVar3 = lVar1 + 0x194;
  if (lVar1 == 0) {
    lVar7 = param_3 + 0x248;
    lVar3 = param_3 + 0x248;
  }
  local_14c = *(float *)(lVar3 + 0x1c);
  local_150 = *(float *)(lVar3 + 0x2c);
  fVar17 = local_144 - local_14c;
  local_148 = *(float *)(lVar7 + 0x2c);
  fVar19 = fVar20 - local_150;
  local_168 = *(float *)(lVar3 + 0xc);
  local_13c = *(float *)(lVar7 + 0xc);
  fVar15 = local_148 - local_150;
  local_140 = *(float *)(lVar7 + 0x1c);
  fVar16 = fVar13 - local_168;
  fVar14 = local_140 - local_14c;
  fVar12 = local_13c - local_168;
  local_108 = fVar20;
  local_180 = (float)FUN_140776d48(fVar14 * fVar14 + fVar12 * fVar12 + fVar15 * fVar15);
  fVar9 = DAT_1407ffaf8;
  if (0.0 < local_180) {
    fVar10 = DAT_1407ffaf8 / local_180;
    fVar12 = fVar12 * fVar10;
    fVar14 = fVar14 * fVar10;
    fVar15 = fVar15 * fVar10;
  }
  fVar8 = (float)FUN_140776d48(fVar17 * fVar17 + fVar16 * fVar16 + fVar19 * fVar19);
  fVar18 = local_144;
  fVar5 = local_14c;
  fVar4 = local_150;
  fVar10 = DAT_14080350c;
  if (0.0 < fVar8) {
    fVar11 = fVar9 / fVar8;
    fVar16 = fVar16 * fVar11;
    fVar17 = fVar11 * fVar17;
    fVar19 = fVar11 * fVar19;
  }
  fVar17 = fVar12 * fVar16 + fVar14 * fVar17 + fVar15 * fVar19;
  if ((DAT_14080350c <= fVar17 - fVar9) || (DAT_14080350c <= fVar9 - fVar17)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if (((((DAT_14080350c <= local_180 - 0.0) || (DAT_14080350c <= 0.0 - local_180)) ||
       (DAT_14080350c <= fVar8 - 0.0)) || (DAT_14080350c <= 0.0 - fVar8)) && (!bVar2)) {
    local_178 = local_13c;
    local_174 = local_140;
    local_170 = local_148;
    local_198 = local_168;
    local_1a4 = local_144;
    local_194 = local_14c;
    local_190 = local_150;
    local_1a8 = fVar13;
    local_1a0 = fVar20;
    cVar6 = FUN_14038ee10(&local_198,&local_178,&local_1a8,&local_160);
    if (cVar6 != '\0') {
      fVar18 = fVar18 - local_15c;
      fVar20 = fVar20 - local_158;
      fVar13 = fVar13 - local_160;
      local_178 = (fVar4 - local_158) * fVar18 - (fVar5 - local_15c) * fVar20;
      local_174 = (float)((uint)((fVar4 - local_158) * fVar13 - fVar20 * (local_168 - local_160)) ^
                         DAT_140800860);
      local_170 = (fVar5 - local_15c) * fVar13 - fVar18 * (local_168 - local_160);
      local_1a8 = local_170 * fVar18 - local_174 * fVar20;
      local_1a0 = local_174 * fVar13 - local_178 * fVar18;
      local_1a4 = (float)((uint)(local_170 * fVar13 - local_178 * fVar20) ^ DAT_140800860);
      local_180 = fVar13;
      fVar17 = (float)FUN_140776d48(fVar13 * fVar13 + fVar18 * fVar18 + fVar20 * fVar20);
      if (0.0 < fVar17) {
        fVar19 = fVar9 / fVar17;
        fVar18 = fVar18 * fVar19;
        fVar13 = fVar19 * fVar13;
        fVar20 = fVar20 * fVar19;
        local_180 = fVar13;
      }
      if (((fVar17 != 0.0) && (fVar17 = (float)FUN_140139800(&local_1a8), fVar17 != 0.0)) &&
         (fVar19 = (float)FUN_140139800(&local_178), fVar17 = local_1a8, fVar19 != 0.0)) {
        local_130 = local_178;
        local_12c = local_160;
        local_124 = local_1a4;
        local_11c = local_15c;
        local_114 = local_1a0;
        local_120 = local_174;
        local_10c = local_158;
        local_134 = local_1a8;
        local_110 = local_170;
        local_138 = fVar13;
        local_128 = fVar18;
        local_118 = fVar20;
        FUN_14022d410(&local_138,&local_f8);
        local_138 = local_f8;
        local_134 = local_f4;
        local_130 = local_f0;
        local_124 = local_e4;
        local_114 = local_d4;
        local_128 = local_e8;
        local_164 = local_f4 * param_5[1] + local_f8 * *param_5 + local_f0 * param_5[2] + local_ec;
        local_120 = local_e0;
        local_118 = local_d8;
        local_110 = local_d0;
        local_10c = local_cc;
        local_12c = local_ec;
        local_11c = local_dc;
        local_100 = local_e4 * param_5[1] + local_e8 * *param_5 + local_e0 * param_5[2] + local_dc;
        local_188 = local_d4 * param_5[1] + local_d8 * *param_5 + local_d0 * param_5[2] + local_cc;
        DAT_140b2c380 =
             (float)FUN_140776d48((local_14c - local_15c) * (local_14c - local_15c) +
                                  (local_168 - local_160) * (local_168 - local_160) +
                                  (local_150 - local_158) * (local_150 - local_158));
        if (DAT_1408007f8 < DAT_140b2c380) {
          local_198 = *param_5;
          local_194 = param_5[1];
          local_190 = param_5[2];
          local_1a4 = param_4[1];
          local_1a8 = *param_4;
          local_1a0 = param_4[2];
          FUN_140399e20(param_1,param_2,param_3,&local_1a8,&local_198,param_6);
          return param_2;
        }
        fVar13 = (float)FUN_140776d48();
        DAT_140b2c384 = fVar13 * local_104;
        DAT_140b2c394 = local_104 * param_6;
        _DAT_140b2c388 = local_164;
        _DAT_140b2c38c = local_100;
        DAT_140b2c390 = local_188;
        FUN_1407d6720(local_res18);
        FUN_1407d68e0(local_res18,&local_c8,FUN_1403aba90,DAT_1408007b4,
                      CONCAT44(uVar21,DAT_1408007f8),fVar10,0xf);
        if (local_c4 == 0) {
          fVar19 = (local_c8 * DAT_140b2c384) / DAT_140b2c380;
          fVar13 = fVar19;
          if (0x1d000000 < ((uint)fVar19 & 0x7f800000)) {
            local_188 = fVar19;
            fVar13 = (float)FUN_140776650();
          }
          local_188 = fVar19;
          if (0x1d000000 < ((uint)fVar19 & 0x7f800000)) {
            fVar9 = (float)FUN_140776150();
          }
          fVar19 = param_4[2];
          fVar12 = *(float *)(param_3 + 0x80);
          fVar13 = fVar13 * DAT_140b2c380;
          fVar9 = fVar9 * DAT_140b2c380;
          fVar14 = *param_4;
          fVar15 = *(float *)(param_3 + 0x78);
          param_2[1] = (param_4[1] - *(float *)(param_3 + 0x7c)) +
                       local_1a4 * fVar13 + fVar9 * fVar18 + local_174 * 0.0 + local_15c;
          *param_2 = fVar17 * fVar13 + fVar9 * local_180 + local_178 * 0.0 + local_160 +
                     (fVar14 - fVar15);
          param_2[2] = (fVar19 - fVar12) +
                       local_1a0 * fVar13 + fVar9 * fVar20 + local_170 * 0.0 + local_158;
        }
        else {
          local_198 = *param_5;
          local_194 = param_5[1];
          local_190 = param_5[2];
          local_1a8 = *param_4;
          local_1a4 = param_4[1];
          local_1a0 = param_4[2];
          FUN_140399e20(param_1,param_2,param_3,&local_1a8,&local_198,param_6);
        }
        FUN_1407d6730(local_res18);
        return param_2;
      }
    }
  }
  local_198 = *param_5;
  local_194 = param_5[1];
  local_190 = param_5[2];
  local_1a4 = param_4[1];
  local_1a8 = *param_4;
  local_1a0 = param_4[2];
  FUN_140399e20(param_1,param_2,param_3,&local_1a8,&local_198,param_6);
  return param_2;
}


