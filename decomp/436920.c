/* AsteroidFieldDamageBehaviorClass::FUN_140436920  RVA 0x436920  size 1962  params 0  ret undefined */


/* WARNING: Removing unreachable block (ram,0x000140436e0a) */
/* WARNING: Removing unreachable block (ram,0x000140436e1a) */
/* WARNING: Removing unreachable block (ram,0x000140436e4d) */
/* WARNING: Removing unreachable block (ram,0x000140436e8c) */
/* WARNING: Removing unreachable block (ram,0x000140436f50) */
/* WARNING: Removing unreachable block (ram,0x000140437012) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void AsteroidFieldDamageBehaviorClass::FUN_140436920(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  undefined *puVar6;
  float *pfVar7;
  longlong *plVar8;
  uint uVar9;
  longlong *plVar10;
  longlong *plVar11;
  float fVar12;
  ulonglong in_stack_fffffffffffffde8;
  ulonglong uVar13;
  float local_1c0;
  float fStack_1bc;
  float local_1b8;
  float local_1b0;
  float local_1ac;
  float local_1a8;
  undefined8 local_1a0;
  float local_198;
  longlong *local_190;
  undefined4 local_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 uStack_168;
  undefined4 local_160;
  undefined4 uStack_15c;
  undefined4 uStack_158;
  undefined4 uStack_154;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined4 local_130;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined8 local_120;
  float local_118;
  undefined1 local_110 [232];
  
  plVar10 = (longlong *)0x0;
  FUN_140395190(param_2);
  lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x298) + 0x13d0);
  if (lVar1 != 0) {
    FUN_1402d5320(&DAT_140b27e60,lVar1,param_2,0,in_stack_fffffffffffffde8 & 0xffffffffffffff00);
  }
  uVar13 = 0;
  FUN_1403a9e30(param_2,10);
  lVar1 = *(longlong *)(param_2 + 0x2a0);
  local_1c0 = 0.0;
  fStack_1bc = 0.0;
  local_1b8 = 0.0;
  plVar3 = (longlong *)FUN_1402648b0(lVar1);
  local_1a0 = 0;
  local_198 = 0.0;
  local_190 = plVar3;
  puVar4 = (undefined8 *)(**(code **)(*plVar3 + 0x60))(plVar3);
  local_180 = *puVar4;
  uStack_178 = puVar4[1];
  local_170 = puVar4[2];
  uStack_168 = puVar4[3];
  local_160 = *(undefined4 *)(puVar4 + 4);
  uStack_15c = *(undefined4 *)((longlong)puVar4 + 0x24);
  uStack_158 = *(undefined4 *)(puVar4 + 5);
  uStack_154 = *(undefined4 *)((longlong)puVar4 + 0x2c);
  FUN_14022d390(&local_180,&local_150);
  local_180 = local_150;
  uStack_178 = uStack_148;
  local_160 = local_130;
  uStack_15c = uStack_12c;
  uStack_158 = uStack_128;
  uStack_154 = uStack_124;
  if (*(int *)(lVar1 + 0xa8) == -1) {
    local_198 = *(float *)(param_2 + 0x8c);
    local_1a0 = *(undefined8 *)(param_2 + 0x84);
    local_120 = local_1a0;
    local_118 = local_198;
    iVar2 = FUN_14012d6d0(plVar3);
    plVar5 = plVar10;
    plVar11 = plVar10;
    if (iVar2 < 0) {
      return;
    }
    do {
      plVar8 = (longlong *)FUN_14012d6f0(plVar3,plVar5);
      if (plVar8 != (longlong *)0x0) {
        for (puVar6 = (undefined *)(**(code **)(*plVar8 + 0x20))(plVar8); puVar6 != (undefined *)0x0
            ; puVar6 = *(undefined **)(puVar6 + 8)) {
          if (puVar6 == &DAT_140a12540) {
            if ((char)plVar8[6] < '\0') {
              plVar11 = (longlong *)(ulonglong)((int)plVar11 + 1);
            }
            break;
          }
        }
      }
      uVar9 = (int)plVar5 + 1;
      plVar5 = (longlong *)(ulonglong)uVar9;
    } while ((int)uVar9 <= iVar2);
    if ((int)plVar11 == 0) {
      return;
    }
    fVar12 = (float)FUN_1401ffbb0(&DAT_140a13e24,0);
    fVar12 = floorf(fVar12);
    plVar3 = plVar10;
    do {
      plVar5 = (longlong *)FUN_14012d6f0(local_190,plVar10);
      if (plVar5 != (longlong *)0x0) {
        for (puVar6 = (undefined *)(**(code **)(*plVar5 + 0x20))(plVar5); puVar6 != (undefined *)0x0
            ; puVar6 = *(undefined **)(puVar6 + 8)) {
          if (puVar6 == &DAT_140a12540) {
            if ((char)plVar5[6] < '\0') {
              if ((int)plVar3 == (int)fVar12) {
                FUN_14014d010(plVar5,&local_1c0,&local_1b0,1);
                fVar12 = (float)FUN_140776d48(local_1ac * local_1ac + local_1b0 * local_1b0 +
                                              local_1a8 * local_1a8);
                if (0.0 < fVar12) {
                  fVar12 = DAT_1407ffaf8 / fVar12;
                  local_1b0 = local_1b0 * fVar12;
                  local_1ac = local_1ac * fVar12;
                  local_1a8 = local_1a8 * fVar12;
                }
                fStack_1bc = fStack_1bc + local_1ac * DAT_1408007b4;
                local_1c0 = local_1c0 + local_1b0 * DAT_1408007b4;
                local_1b8 = local_1b8 + local_1a8 * DAT_1408007b4;
                goto LAB_140436d88;
              }
              plVar3 = (longlong *)(ulonglong)((int)plVar3 + 1);
            }
            break;
          }
        }
      }
      uVar9 = (int)plVar10 + 1;
      plVar10 = (longlong *)(ulonglong)uVar9;
    } while ((int)uVar9 <= iVar2);
  }
  else {
    plVar5 = (longlong *)FUN_14012d6f0();
    plVar3 = plVar10;
    if (plVar5 != (longlong *)0x0) {
      for (puVar6 = (undefined *)(**(code **)(*plVar5 + 0x20))(plVar5);
          (plVar3 = plVar10, puVar6 != (undefined *)0x0 &&
          (plVar3 = plVar5, puVar6 != &DAT_140a12540)); puVar6 = *(undefined **)(puVar6 + 8)) {
      }
    }
    FUN_14014ce10(plVar3,&local_1c0,&local_1b0,1);
    local_190 = (longlong *)0x0;
    local_188 = 0;
    pfVar7 = (float *)FUN_14020acd0(local_110,&local_190,&local_1b0);
    local_198 = pfVar7[2] + 0.0;
    local_1a0 = CONCAT44(pfVar7[1] + DAT_1408007ec,*pfVar7 + 0.0);
    local_120 = CONCAT44(pfVar7[1] + DAT_1408007ec,*pfVar7 + 0.0);
    local_118 = local_198;
  }
LAB_140436d88:
  iVar2 = *(int *)(*(longlong *)(param_2 + 0x298) + 0xca8);
  if (0 < iVar2) {
    iVar2 = FUN_1401ffb40(&DAT_140a13e24,0,iVar2 + -1);
    lVar1 = *(longlong *)
             (*(longlong *)(*(longlong *)(param_2 + 0x298) + 0xca0) + 8 + (longlong)iVar2 * 0x10);
    if (lVar1 != 0) {
      FUN_14029f810(*(undefined8 *)(DAT_140b15418 + 0x18),lVar1,*(undefined4 *)(param_2 + 0x58),
                    &local_1c0,&local_120,1,uVar13 & 0xffffffffffffff00);
    }
  }
  return;
}


