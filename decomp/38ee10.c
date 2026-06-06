/* FUN_14038ee10  RVA 0x38ee10  size 1970  params 0  ret undefined */


undefined8 FUN_14038ee10(float *param_1,float *param_2,float *param_3,float *param_4)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float extraout_XMM0_Da;
  float extraout_XMM0_Da_00;
  float extraout_XMM0_Da_01;
  float fVar5;
  float fVar6;
  float fVar7;
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
  float local_res8;
  float local_138;
  float local_134;
  float local_130;
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
  float local_fc;
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
  
  fVar20 = DAT_1407ffaf8;
  local_138 = (param_2[1] - param_1[1]) * (param_3[2] - param_1[2]) -
              (param_2[2] - param_1[2]) * (param_3[1] - param_1[1]);
  local_130 = (*param_2 - *param_1) * (param_3[1] - param_1[1]) -
              (*param_3 - *param_1) * (param_2[1] - param_1[1]);
  local_134 = (float)((uint)((*param_2 - *param_1) * (param_3[2] - param_1[2]) -
                            (*param_3 - *param_1) * (param_2[2] - param_1[2])) ^ DAT_140800860);
  if (local_134 * local_134 + local_138 * local_138 + local_130 * local_130 <= 0.0) {
    fVar13 = 0.0;
    fVar15 = 0.0;
    fVar17 = DAT_1407ffaf8;
  }
  else {
    FUN_140139800(&local_138);
    fVar13 = local_138;
    fVar15 = local_134;
    fVar17 = local_130;
  }
  local_res8 = *param_2 - *param_1;
  fVar12 = param_2[2] - param_1[2];
  fVar11 = param_2[1] - param_1[1];
  fVar19 = fVar17 * fVar11 - fVar15 * fVar12;
  fVar10 = (float)((uint)(local_res8 * fVar17 - fVar13 * fVar12) ^ DAT_140800860);
  fVar8 = local_res8 * fVar15 - fVar13 * fVar11;
  FUN_140776d48(local_res8 * local_res8 + fVar11 * fVar11 + fVar12 * fVar12);
  if (0.0 < extraout_XMM0_Da) {
    fVar5 = fVar20 / extraout_XMM0_Da;
    local_res8 = local_res8 * fVar5;
    fVar11 = fVar11 * fVar5;
    fVar12 = fVar12 * fVar5;
  }
  if (extraout_XMM0_Da != 0.0) {
    FUN_140776d48(fVar10 * fVar10 + fVar19 * fVar19 + fVar8 * fVar8);
    if (0.0 < extraout_XMM0_Da_00) {
      fVar5 = fVar20 / extraout_XMM0_Da_00;
      fVar19 = fVar19 * fVar5;
      fVar10 = fVar10 * fVar5;
      fVar8 = fVar8 * fVar5;
    }
    if (extraout_XMM0_Da_00 != 0.0) {
      FUN_140776d48(fVar15 * fVar15 + fVar13 * fVar13 + fVar17 * fVar17);
      if (0.0 < extraout_XMM0_Da_01) {
        fVar20 = fVar20 / extraout_XMM0_Da_01;
        fVar13 = fVar13 * fVar20;
        fVar15 = fVar15 * fVar20;
        fVar17 = fVar17 * fVar20;
      }
      if (extraout_XMM0_Da_01 != 0.0) {
        fVar20 = *param_1;
        local_dc = param_1[1];
        fVar5 = param_1[1];
        fVar1 = param_1[2];
        local_f8 = local_res8;
        local_ec = *param_1;
        local_cc = param_1[2];
        local_f4 = fVar19;
        local_f0 = fVar13;
        local_e8 = fVar11;
        local_e4 = fVar10;
        local_e0 = fVar15;
        local_d8 = fVar12;
        local_d4 = fVar8;
        local_d0 = fVar17;
        FUN_14022d390(&local_f8,&local_128);
        fVar6 = *param_3;
        fVar14 = *param_1;
        fVar18 = param_1[1];
        fVar7 = param_3[2];
        fVar9 = param_1[2];
        fVar16 = *param_2;
        fVar2 = param_2[1];
        fVar3 = param_2[2];
        fVar4 = param_3[1];
        *param_1 = local_128 * fVar14 + local_124 * fVar18 + local_120 * fVar9 + local_11c;
        param_1[1] = local_118 * fVar14 + local_114 * fVar18 + local_110 * fVar9 + local_10c;
        param_1[2] = local_108 * fVar14 + local_104 * fVar18 + local_100 * fVar9 + local_fc;
        *param_2 = local_124 * fVar2 + local_128 * fVar16 + local_120 * fVar3 + local_11c;
        param_2[1] = local_114 * fVar2 + local_118 * fVar16 + local_110 * fVar3 + local_10c;
        param_2[2] = local_104 * fVar2 + local_108 * fVar16 + local_100 * fVar3 + local_fc;
        local_10c = local_114 * fVar4 + local_118 * fVar6 + local_110 * fVar7 + local_10c;
        local_11c = local_124 * fVar4 + local_128 * fVar6 + local_120 * fVar7 + local_11c;
        param_3[2] = local_104 * fVar4 + local_108 * fVar6 + local_100 * fVar7 + local_fc;
        param_3[1] = local_10c;
        *param_3 = local_11c;
        fVar6 = *param_2;
        fVar14 = param_2[1];
        fVar9 = *param_1 * *param_1 + param_1[1] * param_1[1];
        fVar16 = *param_2 * *param_2 + param_2[1] * param_2[1];
        fVar7 = local_10c * local_10c + local_11c * local_11c;
        fVar18 = (fVar6 * local_10c - fVar14 * local_11c) +
                 (((fVar14 - local_10c) * *param_1 + 0.0) - (fVar6 - local_11c) * param_1[1]);
        if (fVar18 != 0.0) {
          fVar2 = param_1[2];
          fVar18 = DAT_1408007c0 / fVar18;
          fVar14 = ((fVar16 * local_10c - fVar7 * fVar14) +
                   (((fVar14 - local_10c) * fVar9 + 0.0) - (fVar16 - fVar7) * param_1[1])) * fVar18;
          fVar6 = (float)((uint)fVar18 ^ DAT_140800860) *
                  ((fVar16 * local_11c - fVar7 * fVar6) +
                  (((fVar6 - local_11c) * fVar9 + 0.0) - (fVar16 - fVar7) * *param_1));
          *param_4 = fVar14 * local_res8 + fVar6 * fVar19 + fVar2 * fVar13 + fVar20;
          param_4[1] = fVar14 * fVar11 + fVar10 * fVar6 + fVar2 * fVar15 + fVar5;
          param_4[2] = fVar8 * fVar6 + fVar14 * fVar12 + fVar2 * fVar17 + fVar1;
          return 1;
        }
      }
    }
  }
  return 0;
}


