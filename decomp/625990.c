/* SimpleSpaceLocomotorBehaviorClass::vfunc_59  RVA 0x625990  size 2354  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
SimpleSpaceLocomotorBehaviorClass::vfunc_59
          (longlong param_1,longlong param_2,uint param_3,float *param_4,float *param_5,
          float param_6,float *param_7,float *param_8,float *param_9)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  float *pfVar4;
  float *pfVar5;
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
  float local_res10;
  float local_res14;
  undefined8 local_158;
  float local_150;
  float local_148;
  float local_144;
  float local_140;
  float local_13c;
  float local_138;
  float local_134;
  float local_130;
  float local_128;
  float local_124;
  float local_120;
  float local_118;
  float local_114;
  float local_110;
  float local_10c;
  float local_108;
  float local_104;
  undefined1 local_100 [16];
  undefined1 local_f0 [184];
  
  if (param_2 != 0) {
    if (*(int *)(param_2 + 0x48) == 5) {
      uVar2 = FUN_140627930(param_1,param_2,param_3);
      return uVar2;
    }
    if (*(longlong *)(*(longlong *)(param_1 + 0x28) + 0xa8) != 0) {
      FUN_14041c000(param_5,&local_res10);
      cVar1 = FUN_1405c5910(param_2);
      if (cVar1 == '\0') {
        lVar3 = FUN_14054fc00(param_2 + 0x18,*(int *)(param_2 + 0x60) + 1);
        fVar8 = *(float *)(lVar3 + 0x30);
        while (fVar8 <= (float)param_3) {
          FUN_1405c4920(param_2);
          cVar1 = FUN_1405c5910(param_2);
          if (cVar1 != '\0') {
            fVar8 = param_4[1];
            fVar7 = *param_4;
            param_7[2] = param_4[2] + 0.0;
            param_7[1] = local_res14 * param_6 + fVar8;
            *param_7 = local_res10 * param_6 + fVar7;
            *param_8 = *param_5;
            param_8[1] = param_5[1];
            param_8[2] = param_5[2];
            *param_9 = param_6;
            return 1;
          }
          lVar3 = FUN_14054fc00(param_2 + 0x18,*(int *)(param_2 + 0x60) + 1);
          fVar8 = *(float *)(lVar3 + 0x30);
        }
        pfVar4 = (float *)FUN_14054fc00(param_2 + 0x18,*(undefined4 *)(param_2 + 0x60));
        pfVar5 = (float *)FUN_14054fc00(param_2 + 0x18,*(int *)(param_2 + 0x60) + 1);
        fVar8 = pfVar5[0xc] - pfVar4[0xc];
        local_104 = fVar8;
        if (0.0 < fVar8) {
          fVar6 = ((float)param_3 - pfVar4[0xc]) / fVar8;
          fVar7 = 0.0;
          if ((0.0 <= fVar6) && (fVar7 = DAT_1407ffaf8, fVar6 <= DAT_1407ffaf8)) {
            fVar7 = fVar6;
          }
          local_130 = fVar7;
          if (((*pfVar4 == *pfVar5) && (pfVar4[1] == pfVar5[1])) && (pfVar4[2] == pfVar5[2])) {
            *param_7 = *param_4;
            param_7[1] = param_4[1];
            param_7[2] = param_4[2];
            local_158 = 0;
            local_150 = 0.0;
            *param_9 = 0.0;
            FUN_14020acd0(local_100,&local_158,pfVar4 + 3);
            local_158 = 0;
            local_150 = 0.0;
            FUN_14020acd0(local_f0,&local_158,pfVar5 + 3);
            lVar3 = FUN_14020b100(local_f0,&local_158,local_100);
            local_150 = (float)FUN_14020b6d0(*(undefined4 *)(lVar3 + 8));
            local_150 = fVar7 * local_150;
            local_158 = 0;
            FUN_14020b070(local_100,&local_128,&local_158);
            *param_8 = local_128;
            param_8[2] = local_120;
            param_8[1] = local_124;
            FUN_140624c10(param_1,param_5,param_8);
          }
          else {
            local_110 = fVar7 * fVar7;
            fVar6 = *pfVar4;
            local_108 = fVar7 * local_110;
            local_158._4_4_ = pfVar4[4];
            local_res10 = pfVar4[1];
            local_144 = *pfVar5;
            local_13c = pfVar5[1];
            local_158._0_4_ = pfVar4[3];
            local_150 = pfVar4[5];
            local_148 = fVar6;
            FUN_140139800(&local_158);
            local_138 = pfVar4[6] * local_158._4_4_ * fVar8;
            fVar7 = pfVar4[6] * (float)local_158 * fVar8;
            local_158._0_4_ = pfVar5[3];
            local_150 = pfVar5[5];
            local_118 = fVar7;
            local_158._4_4_ = pfVar5[4];
            FUN_140139800(&local_158);
            local_134 = pfVar5[6] * local_158._4_4_ * fVar8;
            fVar8 = pfVar5[6] * (float)local_158 * fVar8;
            local_10c = fVar6 * _DAT_140b2f220 + local_144 * _DAT_140b2f224 + fVar7 * _DAT_140b2f228
                        + fVar8 * _DAT_140b2f22c;
            local_114 = fVar6 * _DAT_140b2f210 + local_144 * DAT_140b2f214 + fVar7 * DAT_140b2f218 +
                        fVar8 * DAT_140b2f21c;
            fVar7 = fVar6 * DAT_140b2f200 + local_144 * DAT_140b2f204 + fVar7 * DAT_140b2f208 +
                    fVar8 * DAT_140b2f20c;
            fVar6 = local_res10 * DAT_140b2f200;
            local_148 = local_148 * DAT_140b2f1f0 + local_144 * DAT_140b2f1f4 +
                        local_118 * DAT_140b2f1f8 + fVar8 * DAT_140b2f1fc;
            local_140 = local_res10 * _DAT_140b2f220 + local_13c * _DAT_140b2f224;
            fVar15 = local_res10 * DAT_140b2f1f0;
            fVar16 = local_140 + local_138 * _DAT_140b2f228 + local_134 * _DAT_140b2f22c;
            local_res10 = local_res10 * _DAT_140b2f210 + local_13c * DAT_140b2f214 +
                          local_138 * DAT_140b2f218 + local_134 * DAT_140b2f21c;
            fVar14 = local_13c * DAT_140b2f1f4;
            fVar11 = local_134 * DAT_140b2f1fc;
            fVar13 = local_138 * DAT_140b2f1f8;
            fVar10 = fVar6 + local_13c * DAT_140b2f204 + local_138 * DAT_140b2f208 +
                     local_134 * DAT_140b2f20c;
            fVar12 = local_114 * DAT_1408007d4;
            fVar9 = local_res10 * DAT_1408007d4;
            fVar8 = param_4[2];
            fVar6 = local_10c * DAT_1408007d8;
            fVar17 = fVar16 * DAT_1408007d8;
            *param_7 = fVar7 * local_130 + local_148 + local_114 * local_110 + local_10c * local_108
            ;
            param_7[2] = fVar8;
            fVar6 = (fVar12 * local_130 + fVar7 + fVar6 * local_110) / local_104;
            fVar7 = (fVar9 * local_130 + fVar10 + fVar17 * local_110) / local_104;
            param_7[1] = fVar10 * local_130 + fVar15 + fVar14 + fVar13 + fVar11 +
                         local_res10 * local_110 + fVar16 * local_108;
            fVar8 = (float)FUN_140776d48(fVar7 * fVar7 + fVar6 * fVar6);
            if (fVar8 <= DAT_140815de4) {
              *param_8 = *param_5;
              param_8[1] = param_5[1];
              fVar8 = param_5[2];
            }
            else {
              local_120 = 0.0;
              local_158._0_4_ = 0.0;
              local_158._4_4_ = 0.0;
              local_150 = 0.0;
              local_128 = fVar6;
              local_124 = fVar7;
              pfVar4 = (float *)FUN_14020acd0(local_f0,&local_158,&local_128);
              *param_8 = *pfVar4;
              param_8[1] = pfVar4[1];
              fVar8 = pfVar4[2];
            }
            param_8[2] = fVar8;
            fVar8 = (float)FUN_140776d48(fVar7 * fVar7 + fVar6 * fVar6 + 0.0);
            *param_9 = fVar8;
            FUN_140624c10(param_1,param_5,param_8);
          }
        }
        else {
          FUN_1405c4920(param_2);
          fVar8 = param_4[1];
          fVar7 = *param_4;
          param_7[2] = param_4[2] + 0.0;
          param_7[1] = local_res14 * param_6 + fVar8;
          *param_7 = local_res10 * param_6 + fVar7;
          *param_8 = *param_5;
          param_8[1] = param_5[1];
          param_8[2] = param_5[2];
          *param_9 = param_6;
        }
      }
      else {
        *param_7 = *param_4;
        param_7[1] = param_4[1];
        param_7[2] = param_4[2];
        *param_8 = *param_5;
        param_8[1] = param_5[1];
        param_8[2] = param_5[2];
        *param_9 = param_6;
      }
      return 1;
    }
  }
  return 0;
}


