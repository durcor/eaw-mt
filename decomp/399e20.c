/* FUN_140399e20  RVA 0x399e20  size 736  params 0  ret undefined */


float * FUN_140399e20(undefined8 param_1,float *param_2,longlong param_3,float *param_4,
                     float *param_5,float param_6)

{
  float *pfVar1;
  longlong lVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  
  fVar5 = (float)DAT_140b0a340;
  fVar9 = *param_4 - *(float *)(param_3 + 0x78);
  fVar10 = param_4[2] - *(float *)(param_3 + 0x80);
  fVar11 = param_4[1] - *(float *)(param_3 + 0x7c);
  lVar2 = *(longlong *)(param_3 + 0xa8) + 0x164;
  if (*(longlong *)(param_3 + 0xa8) == 0) {
    lVar2 = param_3 + 0x248;
  }
  fVar12 = *param_4 - *param_5;
  fVar7 = param_4[2] - param_5[2];
  fVar6 = param_4[1] - param_5[1];
  fVar8 = (*(float *)(param_3 + 0x264) - *(float *)(lVar2 + 0x1c)) * fVar5;
  fVar3 = (*(float *)(param_3 + 0x254) - *(float *)(lVar2 + 0xc)) * fVar5;
  fVar4 = (*(float *)(param_3 + 0x274) - *(float *)(lVar2 + 0x2c)) * fVar5;
  fVar13 = (fVar8 * fVar8 + fVar3 * fVar3 + fVar4 * fVar4) - fVar5 * param_6 * fVar5 * param_6;
  fVar5 = (fVar8 * fVar6 + fVar12 * fVar3 + fVar4 * fVar7) * DAT_1408007d4;
  fVar3 = fVar12 * fVar12 + fVar6 * fVar6 + fVar7 * fVar7;
  if (0.0 <= fVar5 * fVar5 - fVar13 * DAT_1408007e0 * fVar3) {
    if (fVar13 == 0.0) {
      if ((fVar5 != 0.0) && (fVar5 = (float)((uint)fVar3 ^ DAT_140800860) / fVar5, 0.0 <= fVar5)) {
        pfVar1 = (float *)FUN_140393b70(param_3,fVar5);
        fVar5 = pfVar1[1];
        fVar3 = *pfVar1;
        param_2[2] = fVar10 + pfVar1[2];
        param_2[1] = fVar11 + fVar5;
        *param_2 = fVar9 + fVar3;
        return param_2;
      }
    }
    else {
      fVar4 = (float)FUN_140776d48(fVar5 * fVar5 - fVar13 * DAT_1408007e0 * fVar3);
      fVar3 = (fVar4 - fVar5) / (fVar13 * DAT_1408007d4);
      fVar5 = ((float)((uint)fVar5 ^ DAT_140800860) - fVar4) / (fVar13 * DAT_1408007d4);
      if ((0.0 < fVar3) || (0.0 < fVar5)) {
        if (0.0 < fVar3) {
          if (0.0 < fVar5) {
            if (fVar5 <= fVar3) {
              fVar3 = fVar5;
            }
LAB_14039a059:
            pfVar1 = (float *)FUN_140393b70(param_3,fVar3);
            fVar5 = pfVar1[1];
            fVar3 = *pfVar1;
            param_2[2] = fVar10 + pfVar1[2];
            param_2[1] = fVar11 + fVar5;
            *param_2 = fVar9 + fVar3;
            return param_2;
          }
          if (0.0 < fVar3) goto LAB_14039a059;
        }
        if (0.0 < fVar5) {
          pfVar1 = (float *)FUN_140393b70(param_3,fVar5);
          fVar5 = pfVar1[1];
          fVar3 = *pfVar1;
          param_2[2] = fVar10 + pfVar1[2];
          param_2[1] = fVar11 + fVar5;
          *param_2 = fVar9 + fVar3;
          return param_2;
        }
      }
    }
  }
  param_2[0] = 0.0;
  param_2[1] = 0.0;
  param_2[2] = 0.0;
  return param_2;
}


