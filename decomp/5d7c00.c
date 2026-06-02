/* FUN_1405d7c00  RVA 0x5d7c00  size 721  params 0  ret undefined */


undefined8 FUN_1405d7c00(longlong *param_1,longlong param_2)

{
  int iVar1;
  longlong lVar2;
  uint uVar3;
  float fVar4;
  double dVar5;
  uint uVar6;
  uint uVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  undefined8 local_res8;
  undefined8 local_res18;
  
  if (*(char *)((longlong)param_1 + 0x24) != '\0') {
    FUN_1405d8140();
  }
  dVar5 = DAT_140854f30;
  fVar4 = DAT_1407ffaf8;
  if ((*(char *)((longlong)param_1 + 0x31) != '\0') || ((char)param_1[6] != '\0')) {
    uVar8 = FUN_1406b24f0(*(undefined8 *)*param_1,param_2,param_2 + 0x10,param_2 + 0x18);
    return uVar8;
  }
  fVar14 = *(float *)(param_2 + 0x20);
  if (fVar14 < *(float *)(param_2 + 0x24) || fVar14 == *(float *)(param_2 + 0x24)) {
    uVar9 = param_1[1] - *param_1 >> 3;
    fVar11 = fVar14 - (float)*(uint *)((longlong)param_1 + 0x1c);
    if (0.0 <= fVar11) {
      iVar1 = (int)param_1[3];
      uVar3 = (uint)uVar9;
      fVar10 = *(float *)(param_2 + 0x24) - fVar14;
      uVar7 = (uint)(longlong)(fVar11 / (float)(int)param_1[5]);
      uVar6 = uVar3 - 1;
      if (uVar7 < uVar3) {
        uVar6 = uVar7;
      }
      uVar9 = (ulonglong)(uVar6 + iVar1) % (uVar9 & 0xffffffff);
      uVar6 = iVar1 - 1;
      if (iVar1 == 0) {
        uVar6 = uVar3 - 1;
      }
      if (fVar10 == 0.0) {
        fVar10 = DAT_1407ffaf8;
      }
      fVar10 = DAT_1407ffaf8 / fVar10;
      if (fVar14 <= *(float *)(param_2 + 0x24)) {
        do {
          lVar2 = *(longlong *)(*param_1 + uVar9 * 8);
          fVar13 = (fVar14 - *(float *)(param_2 + 0x20)) * fVar10;
          fVar11 = (float)*(uint *)(lVar2 + 0x28);
          fVar14 = *(float *)(param_2 + 0x24);
          if (fVar11 <= *(float *)(param_2 + 0x24)) {
            fVar14 = fVar11;
          }
          fVar11 = (fVar14 - *(float *)(param_2 + 0x20)) * fVar10;
          if (*(float *)(param_2 + 0x24) == *(float *)(param_2 + 0x20)) {
            fVar11 = fVar4;
          }
          if ((*(float *)(param_2 + 0x10) == *(float *)(param_2 + 0x18)) &&
             (*(float *)(param_2 + 0x14) == *(float *)(param_2 + 0x1c))) {
            local_res18 = *(undefined8 *)(param_2 + 0x10);
            local_res8 = *(undefined8 *)(param_2 + 0x18);
          }
          else {
            fVar12 = (float)(dVar5 - (double)fVar13);
            local_res18 = CONCAT44(fVar13 * *(float *)(param_2 + 0x1c) +
                                   fVar12 * *(float *)(param_2 + 0x14),
                                   fVar12 * *(float *)(param_2 + 0x10) +
                                   fVar13 * *(float *)(param_2 + 0x18));
            fVar13 = (float)(dVar5 - (double)fVar11);
            local_res8 = CONCAT44(fVar11 * *(float *)(param_2 + 0x1c) +
                                  fVar13 * *(float *)(param_2 + 0x14),
                                  fVar13 * *(float *)(param_2 + 0x10) +
                                  fVar11 * *(float *)(param_2 + 0x18));
          }
          uVar8 = FUN_1406b24f0(lVar2,param_2,&local_res18,&local_res8);
          if ((int)uVar8 != 0) {
            return uVar8;
          }
          fVar14 = fVar14 + fVar4;
          if ((uint)uVar9 == uVar6) {
            return 0;
          }
          uVar7 = (uint)uVar9 + 1;
          uVar9 = 0;
          if (uVar7 < uVar3) {
            uVar9 = (ulonglong)uVar7;
          }
        } while (fVar14 <= *(float *)(param_2 + 0x24));
      }
      return 0;
    }
  }
  return 0;
}


