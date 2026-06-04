/* SimpleSpaceLocomotorBehaviorClass::FUN_1407d8310  RVA 0x7d8310  size 620  params 0  ret undefined */


float SimpleSpaceLocomotorBehaviorClass::FUN_1407d8310(float *param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  fVar4 = floorf(*param_1);
  fVar5 = floorf(param_1[1]);
  fVar6 = floorf(param_1[2]);
  fVar9 = *param_1 - fVar4;
  fVar7 = param_1[1] - fVar5;
  fVar8 = param_1[2] - fVar6;
  iVar2 = (int)fVar6;
  if (0.0 < fVar9) {
    fVar6 = DAT_1407ffaf8;
    if (fVar9 < DAT_1407ffaf8) {
      fVar6 = (fVar9 - 0.0) / (DAT_1407ffaf8 - 0.0);
      fVar6 = (DAT_1408007d8 - fVar6 * DAT_1408007d4) * fVar6 * fVar6;
    }
  }
  else {
    fVar6 = 0.0;
  }
  if (0.0 < fVar7) {
    fVar9 = DAT_1407ffaf8;
    if (fVar7 < DAT_1407ffaf8) {
      fVar7 = (fVar7 - 0.0) / (DAT_1407ffaf8 - 0.0);
      fVar9 = (DAT_1408007d8 - fVar7 * DAT_1408007d4) * fVar7 * fVar7;
    }
  }
  else {
    fVar9 = 0.0;
  }
  fVar7 = 0.0;
  if ((0.0 < fVar8) && (fVar7 = DAT_1407ffaf8, fVar8 < DAT_1407ffaf8)) {
    fVar7 = (fVar8 - 0.0) / (DAT_1407ffaf8 - 0.0);
    fVar7 = (DAT_1408007d8 - fVar7 * DAT_1408007d4) * fVar7 * fVar7;
  }
  uVar1 = (uint)(byte)(&DAT_140a45d20)[(int)fVar4 & 0xffU] + ((int)fVar5 & 0xffU);
  uVar3 = ((int)fVar5 & 0xffU) + (uint)(byte)(&DAT_140a45d21)[(int)fVar4 & 0xffU];
  fVar5 = (*(float *)(&DAT_140a45f20 +
                     (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar3] + iVar2 + 1 & 0xff) * 4) -
          *(float *)(&DAT_140a45f20 +
                    (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar1] + iVar2 + 1 & 0xff) * 4)) *
          fVar6 + *(float *)(&DAT_140a45f20 +
                            (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar1] + iVar2 + 1 & 0xff) * 4)
  ;
  fVar4 = (*(float *)(&DAT_140a45f20 +
                     (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar3] + iVar2 & 0xff) * 4) -
          *(float *)(&DAT_140a45f20 +
                    (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar1] + iVar2 & 0xff) * 4)) * fVar6 +
          *(float *)(&DAT_140a45f20 +
                    (ulonglong)((uint)(byte)(&DAT_140a45d20)[uVar1] + iVar2 & 0xff) * 4);
  fVar4 = (((*(float *)(&DAT_140a45f20 +
                       (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar3] + iVar2 & 0xff) * 4) -
            *(float *)(&DAT_140a45f20 +
                      (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar1] + iVar2 & 0xff) * 4)) * fVar6
           + *(float *)(&DAT_140a45f20 +
                       (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar1] + iVar2 & 0xff) * 4)) - fVar4
          ) * fVar9 + fVar4;
  return (((((*(float *)(&DAT_140a45f20 +
                        (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar3] + iVar2 + 1 & 0xff) * 4) -
             *(float *)(&DAT_140a45f20 +
                       (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar1] + iVar2 + 1 & 0xff) * 4)) *
             fVar6 + *(float *)(&DAT_140a45f20 +
                               (ulonglong)((uint)(byte)(&DAT_140a45d21)[uVar1] + iVar2 + 1 & 0xff) *
                               4)) - fVar5) * fVar9 + fVar5) - fVar4) * fVar7 + fVar4;
}


