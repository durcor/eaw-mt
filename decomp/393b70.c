/* FUN_140393b70  RVA 0x393b70  size 2191  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_140393b70(longlong param_1,float param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  longlong lVar7;
  float *pfVar8;
  float *pfVar9;
  int iVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float local_190 [16];
  float local_150 [15];
  float local_114;
  undefined8 local_110;
  undefined8 uStack_108;
  float local_100;
  float local_fc;
  float local_f8;
  float local_f4;
  float local_f0;
  float local_ec;
  float local_e8;
  float local_e4;
  float local_e0;
  float local_d8 [52];
  
  pfVar9 = (float *)(param_1 + 0x248);
  pfVar8 = (float *)(*(longlong *)(param_1 + 0xa8) + 0x164);
  if (*(longlong *)(param_1 + 0xa8) == 0) {
    pfVar8 = pfVar9;
  }
  fVar11 = (float)DAT_140b0a340 * param_2;
  _DAT_140b2ec18 = (*(float *)(param_1 + 0x254) - pfVar8[3]) * fVar11 + *(float *)(param_1 + 0x78);
  _DAT_140b2ec1c = (*(float *)(param_1 + 0x264) - pfVar8[7]) * fVar11 + *(float *)(param_1 + 0x7c);
  _DAT_140b2ec20 = (*(float *)(param_1 + 0x274) - pfVar8[0xb]) * fVar11 + *(float *)(param_1 + 0x80)
  ;
  if (DAT_140b2c37c != 0) {
    pfVar8 = (float *)(*(longlong *)(param_1 + 0xa8) + 0x164);
    if (*(longlong *)(param_1 + 0xa8) == 0) {
      pfVar8 = pfVar9;
    }
    local_150[0] = *pfVar9;
    local_150[0xe] = *pfVar8;
    local_114 = pfVar8[1];
    fVar11 = pfVar8[3];
    local_110 = *(undefined8 *)(pfVar8 + 2);
    uStack_108 = *(undefined8 *)(pfVar8 + 4);
    local_100 = pfVar8[6];
    fVar1 = pfVar8[7];
    local_f8 = pfVar8[8];
    local_f4 = pfVar8[9];
    local_f0 = pfVar8[10];
    fVar2 = pfVar8[0xb];
    local_190[0xe] = *(float *)(param_1 + 0x268);
    local_150[1] = *(float *)(param_1 + 0x24c);
    local_150[2] = *(float *)(param_1 + 0x250);
    fVar13 = *(float *)(param_1 + 0x25c);
    fVar15 = *(float *)(param_1 + 0x26c);
    fVar3 = *(float *)(param_1 + 0x260);
    fVar4 = *(float *)(param_1 + 0x270);
    fVar16 = *(float *)(param_1 + 0x254);
    fVar12 = *(float *)(param_1 + 0x264);
    fVar14 = *(float *)(param_1 + 0x274);
    fVar5 = *(float *)(param_1 + 600);
    local_150[3] = fVar16;
    local_150[4] = fVar5;
    local_150[5] = fVar13;
    local_150[6] = fVar3;
    local_150[7] = fVar12;
    local_150[8] = local_190[0xe];
    local_150[9] = fVar15;
    local_150[10] = fVar4;
    local_150[0xb] = fVar14;
    local_fc = fVar1;
    local_ec = fVar2;
    local_e8 = local_150[0];
    local_e4 = local_150[1];
    local_e0 = local_150[2];
    FUN_14022d390(local_150 + 0xe);
    fVar6 = local_e8;
    local_114 = local_190[1];
    pfVar9 = local_d8 + 1;
    fVar16 = fVar16 - fVar11;
    fVar12 = fVar12 - fVar1;
    fVar14 = fVar14 - fVar2;
    lVar7 = 0;
    local_fc = local_190[7];
    local_f4 = local_190[9];
    local_ec = local_190[0xb];
    local_f0 = local_190[10];
    local_f8 = local_190[8];
    local_100 = local_190[6];
    local_190[0xc] = local_190[1] * fVar12 + fVar16 * local_190[0] + local_190[2] * fVar14;
    local_150[0xc] = local_190[5] * fVar12 + fVar16 * local_190[4] + local_190[6] * fVar14;
    local_190[0xd] = fVar16 * local_190[8] + local_190[9] * fVar12 + local_190[10] * fVar14;
    do {
      fVar11 = *(float *)((longlong)local_150 + lVar7 + 4);
      fVar1 = *(float *)((longlong)local_150 + lVar7);
      fVar2 = *(float *)((longlong)local_150 + lVar7 + 8);
      fVar16 = *(float *)((longlong)local_150 + lVar7);
      pfVar9[-1] = fVar11 * local_190[4] + fVar1 * local_190[0] + fVar2 * local_190[8];
      fVar12 = *(float *)((longlong)local_150 + lVar7 + 4);
      pfVar9[1] = fVar11 * local_190[6] + fVar1 * local_190[2] + fVar2 * local_190[10];
      fVar14 = *(float *)((longlong)local_150 + lVar7 + 8);
      *pfVar9 = fVar11 * local_190[5] + fVar1 * local_190[1] + fVar2 * local_190[9];
      pfVar9 = pfVar9 + 4;
      *(float *)((longlong)local_d8 + lVar7 + 0xc) =
           local_190[7] * fVar12 + local_190[3] * fVar16 + local_190[0xb] * fVar14 +
           *(float *)((longlong)local_150 + lVar7 + 0xc);
      lVar7 = lVar7 + 0x10;
    } while (lVar7 < 0x30);
    iVar10 = 0;
    local_190[8] = local_190[0xe];
    local_190[1] = local_e4;
    local_190[2] = local_e0;
    _DAT_140b2ec2c = local_150[7];
    _DAT_140b2ec30 = local_150[0xb];
    _DAT_140b2ec28 = local_150[3];
    local_190[4] = fVar5;
    local_190[5] = fVar13;
    local_190[6] = fVar3;
    local_190[9] = fVar15;
    local_190[10] = fVar4;
    if (0.0 < (float)DAT_140b0a340 * param_2) {
      while( true ) {
        pfVar9 = local_190 + 1;
        lVar7 = 0;
        local_150[0] = fVar6;
        local_150[1] = local_190[1];
        local_150[2] = local_190[2];
        local_190[3] = local_150[3] +
                       local_150[0xc] * local_190[1] + local_190[0xc] * fVar6 +
                       local_190[0xd] * local_190[2];
        local_150[3] = local_190[3];
        local_190[7] = local_150[7] +
                       local_150[0xc] * local_190[5] + local_190[0xc] * local_190[4] +
                       local_190[0xd] * local_190[6];
        local_150[8] = local_190[0xe];
        local_150[4] = local_190[4];
        local_150[5] = local_190[5];
        local_150[6] = local_190[6];
        local_150[9] = local_190[9];
        local_190[0xb] =
             local_150[0xb] +
             local_150[0xc] * local_190[9] + local_190[0xc] * local_190[0xe] +
             local_190[0xd] * local_190[10];
        local_150[7] = local_190[7];
        local_150[10] = local_190[10];
        local_150[0xb] = local_190[0xb];
        local_190[8] = local_190[0xe];
        do {
          fVar11 = *(float *)((longlong)local_150 + lVar7 + 4);
          fVar1 = *(float *)((longlong)local_150 + lVar7);
          fVar2 = *(float *)((longlong)local_150 + lVar7 + 8);
          fVar15 = local_d8[1] * fVar1;
          pfVar9[-1] = local_d8[0] * fVar1 + local_d8[4] * fVar11 + local_d8[8] * fVar2;
          fVar13 = local_d8[2] * fVar1;
          *pfVar9 = fVar15 + local_d8[5] * fVar11 + local_d8[9] * fVar2;
          pfVar9[1] = fVar13 + local_d8[6] * fVar11 + local_d8[10] * fVar2;
          pfVar9 = pfVar9 + 4;
          *(float *)((longlong)local_190 + lVar7 + 0xc) =
               fVar11 * 0.0 + fVar1 * 0.0 + fVar2 * 0.0 +
               *(float *)((longlong)local_150 + lVar7 + 0xc);
          lVar7 = lVar7 + 0x10;
        } while (lVar7 < 0x30);
        iVar10 = iVar10 + 1;
        _DAT_140b2ec2c = local_190[7];
        _DAT_140b2ec30 = local_190[0xb];
        _DAT_140b2ec28 = local_190[3];
        if ((float)DAT_140b0a340 * param_2 <= (float)iVar10) break;
        local_190[0xe] = local_190[8];
        local_150[7] = local_190[7];
        local_150[0xb] = local_190[0xb];
        local_150[3] = local_190[3];
      }
    }
    FUN_140776d48();
    return &DAT_140b2ec28;
  }
  return &DAT_140b2ec18;
}


