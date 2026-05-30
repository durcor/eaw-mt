/* HideWhenFoggedBehaviorClass::vfunc_6  RVA 0x53ddc0  size 814  params 0  ret undefined */


undefined8 HideWhenFoggedBehaviorClass::vfunc_6(longlong param_1,longlong param_2)

{
  float *pfVar1;
  longlong lVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  float fVar6;
  char cVar7;
  char cVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  double dVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  
  LocomotorCommonClass::vfunc_6();
  if (*(char *)(param_2 + 0x3a7) == '\x01') {
    return 0;
  }
  cVar7 = FUN_1404ad350(param_1 + 0x58);
  if ((cVar7 == '\0') &&
     (dVar11 = (double)FUN_1407760bc((double)(*(float *)(param_1 + 0x44) -
                                             *(float *)(param_1 + 0x40))),
     (float)dVar11 < DAT_140815de4)) {
    return 0;
  }
  FUN_1404ad380(param_1 + 0x58,0x1e);
  cVar7 = *(char *)(param_1 + 0x54);
  uVar9 = FUN_140294a70(&DAT_140a16fd0);
  cVar8 = FUN_14039b950(param_2,3,1);
  if ((((cVar8 == '\0') && (cVar8 = FUN_14035f470(DAT_140b15418,uVar9,param_2,0), cVar8 != '\0')) &&
      (DAT_140a284c4 != '\0')) && (DAT_140b1574e == '\0')) {
    *(undefined1 *)(param_1 + 0x54) = 0;
    cVar8 = FUN_1402cbce0(&DAT_140b27a10);
    if (cVar8 == '\0') {
      *(undefined4 *)(param_1 + 0x44) = 0;
      goto LAB_14053dedf;
    }
  }
  else {
    *(undefined1 *)(param_1 + 0x54) = 1;
  }
  cVar8 = FUN_14039a450(param_2,uVar9,0);
  if ((cVar8 == '\0') || (cVar8 = FUN_14039b140(param_2), cVar8 == '\0')) {
    *(undefined4 *)(param_1 + 0x44) = 0x3f800000;
  }
  else {
    *(undefined4 *)(param_1 + 0x44) = 0;
  }
LAB_14053dedf:
  lVar2 = DAT_140b15418[0x54];
  if ((lVar2 != 0) &&
     ((cVar8 = *(char *)(param_1 + 0x54), cVar7 != cVar8 || (*(char *)(param_1 + 0x56) != '\0')))) {
    if (cVar8 == '\0') {
      thunk_FUN_1404eb020(lVar2,param_2);
    }
    else if (cVar8 == '\x01') {
      FUN_1404eacf0(lVar2,param_2);
    }
  }
  fVar6 = DAT_140803510;
  fVar15 = *(float *)(param_1 + 0x50) * DAT_1408007d4;
  bVar3 = DAT_140803510 != *(float *)(param_1 + 0x40);
  bVar4 = *(float *)(param_1 + 0x40) <= DAT_140803510;
  fVar12 = fVar15 * DAT_140b0a344;
  fVar13 = *(float *)(param_1 + 0x40) - *(float *)(param_1 + 0x44);
  fVar14 = DAT_1407ffaf8 /
           (fVar12 * DAT_140819c54 * fVar12 + fVar12 + DAT_1407ffaf8 +
           fVar12 * DAT_140819c50 * fVar12 * fVar12);
  fVar12 = (fVar13 * fVar15 + *(float *)(param_1 + 0x48)) * DAT_140b0a344;
  *(float *)(param_1 + 0x40) = (fVar12 + fVar13) * fVar14 + *(float *)(param_1 + 0x44);
  *(float *)(param_1 + 0x48) = (*(float *)(param_1 + 0x48) - fVar12 * fVar15) * fVar14;
  if (*(char *)(*(longlong *)(param_2 + 0x298) + 0x88) != '\0') {
    *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_1 + 0x44);
    *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(param_1 + 0x44);
    *(undefined4 *)(param_1 + 0x48) = 0;
  }
  pfVar1 = (float *)(param_1 + 0x40);
  fVar14 = *pfVar1;
  fVar13 = *pfVar1;
  fVar12 = *pfVar1;
  bVar5 = fVar12 <= fVar6 && fVar6 != fVar13;
  if ((bVar5 != (bVar4 && bVar3)) || (*(char *)(param_1 + 0x56) != '\0')) {
    if (*(longlong *)(param_2 + 0x2a0) != 0) {
      FUN_140264c80(*(longlong *)(param_2 + 0x2a0),bVar5);
    }
    if (lVar2 != 0) {
      if ((NAN(fVar6) || NAN(fVar14)) || (fVar12 > fVar6 || fVar6 == fVar13)) {
        _guard_check_icall(lVar2,param_2);
      }
      else {
        _guard_check_icall(lVar2,param_2);
      }
    }
  }
  fVar6 = DAT_140803514;
  if (*(float *)(param_1 + 0x44) <= DAT_1408007c0 && DAT_1408007c0 != *(float *)(param_1 + 0x44)) {
    fVar6 = DAT_1408054d0;
  }
  *(bool *)(param_1 + 0x55) = fVar6 < *(float *)(param_1 + 0x40);
  if ((*(char *)(param_1 + 0x54) == '\x01') &&
     (((*(char *)(*(longlong *)(param_2 + 0x298) + 0x8f) == '\0' ||
       (cVar7 = (**(code **)(*DAT_140b15418 + 0x160))(), cVar7 == '\0')) &&
      (*(char *)(param_2 + 0x33a) == -1)))) {
    uVar10 = FUN_140294a40(&DAT_140a16fd0);
    cVar7 = FUN_1402824d0(uVar10,*(undefined4 *)(param_2 + 0x58));
    if (cVar7 == '\x01') {
      (**(code **)(*DAT_140b15418 + 0x168))(DAT_140b15418,param_2);
    }
  }
  *(undefined1 *)(param_1 + 0x56) = 0;
  return 0;
}


