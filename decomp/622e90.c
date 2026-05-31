/* StarshipLocomotorBehaviorClass::FUN_140622e90  RVA 0x622e90  size 1148  params 0  ret undefined */


void StarshipLocomotorBehaviorClass::FUN_140622e90
               (longlong param_1,longlong param_2,longlong param_3)

{
  int iVar1;
  longlong *plVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  float *pfVar6;
  longlong lVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  double dVar12;
  float fVar13;
  float fVar14;
  float local_118;
  float local_114;
  float local_110;
  undefined4 local_108;
  undefined4 local_104;
  undefined4 local_100;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  float local_e8;
  float local_e4;
  float local_e0;
  undefined1 local_d8 [4];
  undefined4 local_d4;
  undefined4 local_d0;
  undefined1 local_c8 [16];
  undefined1 local_b8 [144];
  
  lVar7 = *(longlong *)(param_2 + 0x298);
  iVar1 = *(int *)(param_3 + 0x48);
  if (lVar7 == 0) {
    return;
  }
  local_118 = *(float *)(param_2 + 0x78);
  local_114 = *(float *)(param_2 + 0x7c);
  fVar14 = 0.0;
  local_110 = *(float *)(param_2 + 0x80);
  puVar4 = (undefined4 *)FUN_140623340();
  local_108 = *puVar4;
  local_104 = puVar4[1];
  local_100 = *(undefined4 *)(lVar7 + 0xdfc);
  FUN_14020acd0(&local_f8,&local_118,&local_108);
  *(undefined4 *)(param_3 + 0x38) = local_f8;
  *(undefined4 *)(param_3 + 0x40) = local_f0;
  *(undefined4 *)(param_3 + 0x3c) = local_f4;
  fVar8 = (float)FUN_14020b6d0(*(undefined4 *)(param_2 + 0x84));
  uVar3 = DAT_140800860;
  if (fVar8 < *(float *)(param_3 + 0x44) || fVar8 == *(float *)(param_3 + 0x44)) {
    fVar9 = 0.0;
    if (fVar8 < *(float *)(param_3 + 0x44)) {
      fVar9 = (float)FUN_1403724d0(lVar7);
      fVar13 = *(float *)(param_3 + 0x44) - fVar8;
      if (fVar13 <= fVar9) {
        fVar9 = fVar13;
      }
    }
  }
  else {
    fVar9 = (float)FUN_1403724d0(lVar7);
    fVar13 = fVar8 - *(float *)(param_3 + 0x44);
    if (fVar13 <= fVar9) {
      fVar9 = fVar13;
    }
    fVar9 = (float)((uint)fVar9 ^ uVar3);
  }
  fVar10 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
  fVar13 = *(float *)(lVar7 + 0x39c);
  if (iVar1 == 3) {
    fVar13 = (float)((uint)fVar13 ^ uVar3);
  }
  fVar13 = ((float)((uint)fVar8 ^ uVar3) * fVar10) / fVar13;
  fVar10 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
  if (fVar10 <= fVar13) {
    fVar13 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
  }
  fVar11 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
  fVar10 = DAT_1408509e8;
  if (fVar13 <= fVar11 / DAT_1408509e8) {
    fVar13 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
    fVar13 = fVar13 / fVar10;
  }
  FUN_14020b100(param_3 + 0x38,local_d8,param_2 + 0x84);
  fVar10 = (float)FUN_14020b6d0(local_d0);
  if (fVar10 != 0.0) {
    fVar14 = fVar13;
    if (fVar10 <= fVar13) {
      fVar14 = fVar10;
    }
    if (iVar1 == 3) {
      fVar14 = (float)((uint)fVar14 ^ uVar3);
    }
    fVar11 = (float)FUN_1403724d0(lVar7);
    if (fVar11 <= DAT_1407ffaf8) {
      fVar11 = DAT_1407ffaf8;
    }
    if ((DAT_140819104 / fVar11 <= fVar10) && (fVar10 <= DAT_1408007f4 - DAT_140819104 / fVar11)) {
      *(undefined4 *)(param_3 + 0x54) = 0x3e800000;
      goto LAB_140623130;
    }
    *(undefined4 *)(param_3 + 0x44) = 0;
    *(undefined4 *)(param_3 + 0x54) = 0x3f000000;
    dVar12 = (double)FUN_1407760bc((double)fVar8);
    fVar8 = (float)FUN_1403724d0(lVar7);
    if (fVar8 < (float)dVar12) goto LAB_140623130;
  }
  FUN_140559250(param_3,1);
LAB_140623130:
  fVar8 = (float)FUN_14020b6d0(local_d4);
  local_e4 = 0.0;
  if (fVar8 != 0.0) {
    if (fVar8 <= 0.0) {
      local_e4 = (float)((uint)fVar13 ^ uVar3);
      if ((float)((uint)fVar13 ^ uVar3) <= fVar8) {
        local_e4 = fVar8;
      }
    }
    else {
      local_e4 = fVar13;
      if (fVar8 <= fVar13) {
        local_e4 = fVar8;
      }
    }
  }
  local_e8 = fVar9;
  local_e0 = fVar14;
  FUN_14020b070(param_2 + 0x84,local_c8,&local_e8);
  FUN_1403a8710(param_2,local_c8);
  if (*(float *)(param_3 + 0x44) == 0.0) {
    uVar5 = FUN_140623340(param_1);
    FUN_14020acd0(local_b8,param_2 + 0x78,uVar5);
    FUN_14020b100(local_b8,&local_e8,param_2 + 0x84);
    if (((DAT_14085c064 < local_e4) && (local_e4 < DAT_14085c068)) ||
       ((DAT_14085c064 < local_e0 && (local_e0 < DAT_14085c068)))) {
      *(undefined4 *)(param_3 + 0x54) = 0x3ecccccd;
      pfVar6 = (float *)FUN_140623340(param_1);
      fVar14 = (float)FUN_140776d48((pfVar6[1] - local_114) * (pfVar6[1] - local_114) +
                                    (*pfVar6 - local_118) * (*pfVar6 - local_118) +
                                    (pfVar6[2] - local_110) * (pfVar6[2] - local_110));
      fVar8 = (float)FUN_140370f00(lVar7,param_2);
      if (fVar8 * DAT_1408007e8 < fVar14) {
        FUN_140623fe0(param_1,param_2,param_3);
      }
      plVar2 = *(longlong **)(param_1 + 0x28);
      if (((char)plVar2[0x69] != -1) &&
         (lVar7 = (**(code **)(*plVar2 + 0x10))(plVar2,0x16), lVar7 != 0)) {
        FUN_1404061f0(lVar7,0);
      }
    }
  }
  return;
}


