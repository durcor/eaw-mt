/* StarshipLocomotorBehaviorClass::FUN_140622b80  RVA 0x622b80  size 778  params 0  ret undefined */


void StarshipLocomotorBehaviorClass::FUN_140622b80
               (longlong param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  char cVar2;
  float *pfVar3;
  undefined4 *puVar4;
  float fVar5;
  float fVar6;
  double dVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float local_78;
  float local_74;
  float local_70;
  undefined4 local_68;
  undefined4 local_64;
  float local_60;
  undefined8 local_58;
  undefined4 local_50;
  
  lVar1 = *(longlong *)(param_2 + 0x298);
  if (lVar1 != 0) {
    local_78 = *(float *)(param_2 + 0x78);
    local_74 = *(float *)(param_2 + 0x7c);
    local_70 = *(float *)(param_2 + 0x80);
    pfVar3 = (float *)FUN_140623340();
    fVar9 = pfVar3[2] - local_70;
    fVar10 = pfVar3[1] - local_74;
    fVar8 = *pfVar3 - local_78;
    cVar2 = FUN_140623450(param_1,param_2,param_3);
    if (((cVar2 == '\0') || (*(float *)(lVar1 + 0x39c) == 0.0)) ||
       (fVar5 = (float)FUN_1403724d0(lVar1), fVar5 == 0.0)) {
      puVar4 = (undefined4 *)FUN_140623340(param_1);
      fVar5 = DAT_1408007d4;
      local_68 = *puVar4;
      local_64 = puVar4[1];
      local_60 = (float)puVar4[2];
      if (local_60 < *(float *)(lVar1 + 0xdfc)) {
        dVar7 = (double)FUN_1407760bc((double)(local_60 - local_70));
        if ((float)dVar7 < *(float *)(lVar1 + 0x3a8)) {
          local_60 = *(float *)(lVar1 + 0xdfc);
        }
        dVar7 = (double)FUN_1407760bc((double)(local_60 - local_70));
        if (*(float *)(lVar1 + 0x3a8) * fVar5 < (float)dVar7) {
          local_60 = *(float *)(lVar1 + 0xdfc);
        }
      }
      FUN_14020acd0(&local_58,&local_78,&local_68);
      fVar6 = (float)FUN_140372560(lVar1);
      cVar2 = FUN_14020ae40(&local_58,param_2 + 0x84,fVar6 / fVar5);
      if (cVar2 == '\0') {
        fVar8 = (float)FUN_140776d48(fVar8 * fVar8 + fVar10 * fVar10 + fVar9 * fVar9);
        fVar9 = (float)FUN_140370f00(lVar1,param_2);
        if (fVar9 * DAT_140805898 < fVar8) {
          *(undefined4 *)(param_3 + 0x38) = (undefined4)local_58;
          *(undefined4 *)(param_3 + 0x40) = local_50;
          *(undefined4 *)(param_3 + 0x3c) = local_58._4_4_;
          *(undefined4 *)(param_3 + 0x54) = 0x3f4ccccd;
          fVar8 = (float)FUN_140372560(*(undefined8 *)(param_2 + 0x298));
          FUN_1403abe40(param_2,param_3 + 0x38,fVar8 / DAT_1408007d8,1,1);
        }
        *(undefined4 *)(param_3 + 0x58) = 0x3c23d70a;
      }
      else {
        *(undefined4 *)(param_3 + 0x54) = 0x3f800000;
        *(undefined4 *)(param_3 + 0x58) = 0x3ba3d70a;
      }
    }
    else {
      *(undefined4 *)(param_3 + 0x54) = 0x3ecccccd;
      local_58 = 0;
      local_50 = 0;
      fVar5 = (float)FUN_140372560(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x298));
      FUN_1403abf80(*(undefined8 *)(param_1 + 0x28),&local_58,fVar5 / DAT_1408007d8,0,1);
      fVar8 = (float)FUN_140776d48(fVar8 * fVar8 + fVar10 * fVar10 + fVar9 * fVar9);
      if (*(float *)(lVar1 + 0x3a8) <= fVar8 && fVar8 != *(float *)(lVar1 + 0x3a8)) {
        FUN_140623fe0(param_1,param_2,param_3);
      }
    }
  }
  return;
}


