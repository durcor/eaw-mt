/* FUN_14078437c  RVA 0x78437c  size 1008  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_14078437c(float param_1,float param_2)

{
  double dVar1;
  double dVar2;
  uint uVar3;
  undefined8 uVar4;
  int iVar5;
  double dVar6;
  double dVar7;
  ulonglong uVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  uint uVar12;
  uint uVar13;
  float fVar14;
  uint uVar15;
  float local_res8;
  
  dVar9 = (double)param_2;
  uVar15 = (uint)((ulonglong)dVar9 >> 0x20);
  dVar1 = (double)param_1;
  uVar3 = (uint)((ulonglong)dVar1 >> 0x20);
  iVar5 = (uVar3 >> 0x14 & 0x7ff) - (uVar15 >> 0x14 & 0x7ff);
  dVar7 = ABS(dVar9);
  dVar6 = ABS(dVar1);
  local_res8 = param_2;
  if ((0x7ff0000000000000 < (ulonglong)dVar7) ||
     (local_res8 = param_1, 0x7ff0000000000000 < (ulonglong)dVar6)) {
    uVar8 = _handle_nanf(local_res8);
    return uVar8;
  }
  uVar8 = 0;
  uVar4 = 0x20;
  if (dVar6 == 0.0) {
    if (-1 < (longlong)dVar9) goto LAB_140784446;
LAB_1407845b6:
    fVar14 = DAT_1408007dc;
    if ((longlong)dVar1 < 0) {
      fVar14 = DAT_1408a8560;
    }
  }
  else if (dVar7 == 0.0) {
    if (-1 < (longlong)dVar1) {
      _set_statfp(0x20);
      goto LAB_14078446b;
    }
LAB_140784475:
    fVar14 = DAT_1408d4008;
  }
  else {
LAB_14078446b:
    if (iVar5 < 0x1b) {
      if ((iVar5 < -0xd) && (-1 < (longlong)dVar9)) {
        if (iVar5 < -0x96) {
          fVar14 = DAT_140819968;
          if (-1 < (longlong)dVar1) {
            fVar14 = 0.0;
          }
        }
        else {
          if (-0x7f < iVar5) {
            dVar1 = dVar1 / dVar9;
LAB_140784446:
            return (ulonglong)(uint)(float)dVar1;
          }
          dVar9 = (dVar1 * 1.2676506002282294e+30) / dVar9;
          uVar3 = (uint)((ulonglong)ABS(dVar9) >> 0x34);
          if (uVar3 < 0x65) {
            iVar5 = 0x65 - uVar3;
            if (iVar5 < 0x37) {
              uVar8 = ((ulonglong)dVar9 & 0x1fffffffffffff | 0x10000000000000) >>
                      ((char)iVar5 - 1U & 0x3f);
              uVar8 = (uVar8 >> 1) + (ulonglong)((uint)uVar8 & 1);
            }
          }
          else {
            uVar8 = ((ulonglong)ABS(dVar9) >> 0x34) - 100 << 0x34 |
                    (ulonglong)dVar9 & 0xfffffffffffff;
          }
          fVar14 = (float)(double)(uVar8 | (ulonglong)dVar9 & 0x8000000000000000);
          if ((uVar8 & 0x7ff0000000000000) != 0) goto LAB_14078448f;
        }
        uVar4 = 0x30;
      }
      else {
        if ((iVar5 < -0x1a) && ((longlong)dVar9 < 0)) goto LAB_1407845b6;
        if ((dVar6 != INFINITY) || (dVar7 != INFINITY)) {
          uVar12 = (uint)DAT_14086b840;
          uVar13 = (uint)((ulonglong)DAT_14086b840 >> 0x20);
          dVar6 = dVar9;
          if ((longlong)dVar9 < 0) {
            dVar6 = (double)CONCAT44(uVar15 ^ uVar13,SUB84(dVar9,0) ^ uVar12);
          }
          dVar7 = dVar1;
          if ((longlong)dVar1 < 0) {
            dVar7 = (double)CONCAT44(uVar3 ^ uVar13,SUB84(dVar1,0) ^ uVar12);
          }
          dVar10 = dVar6;
          dVar2 = dVar7;
          if (dVar6 < dVar7) {
            dVar10 = dVar7;
            dVar2 = dVar6;
          }
          dVar11 = dVar2 / dVar10;
          if (dVar11 <= _DAT_1408d47a8) {
            if (dVar11 < DAT_140899790) goto LAB_140784714;
            dVar10 = dVar11 * dVar11;
            dVar10 = (DAT_1408d47c8 - (DAT_1408d47b8 - dVar10 * _DAT_1408d47b0) * dVar10) *
                     dVar10 * dVar11;
          }
          else {
            uVar3 = (uint)(dVar11 * DAT_1408d47d8 + DAT_140854fb0);
            dVar11 = (double)uVar3;
            dVar10 = (dVar2 * DAT_1408d47d8 - dVar11 * dVar10) /
                     (dVar11 * dVar2 + dVar10 * DAT_1408d47d8);
            dVar11 = dVar10 + *(double *)(&DAT_1408d4020 + (ulonglong)(uVar3 - 0x10) * 8);
            dVar10 = dVar10 * dVar10 * dVar10 * _DAT_1408d47c0;
          }
          dVar11 = dVar11 - dVar10;
LAB_140784714:
          if (dVar6 < dVar7) {
            dVar11 = DAT_1408d36a8 - dVar11;
          }
          if ((longlong)dVar9 < 0) {
            dVar11 = DAT_14086b828 - dVar11;
          }
          if ((longlong)dVar1 < 0) {
            dVar11 = (double)CONCAT44((uint)((ulonglong)dVar11 >> 0x20) ^ uVar13,
                                      SUB84(dVar11,0) ^ uVar12);
          }
          return (ulonglong)(uint)(float)dVar11;
        }
        if ((longlong)dVar9 < 0) {
          fVar14 = DAT_1408d47d0;
          if ((longlong)dVar1 < 0) {
            fVar14 = DAT_1408d47e0;
          }
        }
        else {
          fVar14 = DAT_14081b96c;
          if ((longlong)dVar1 < 0) {
            fVar14 = DAT_14081b974;
          }
        }
      }
    }
    else {
      fVar14 = DAT_14081afc0;
      if ((longlong)dVar1 < 0) goto LAB_140784475;
    }
  }
  _set_statfp(uVar4);
LAB_14078448f:
  return (ulonglong)(uint)fVar14;
}


