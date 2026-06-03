/* TelekinesisTargetBehaviorClass::vfunc_6  RVA 0x63f210  size 607  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 TelekinesisTargetBehaviorClass::vfunc_6(undefined8 param_1,longlong param_2)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 local_90;
  undefined4 local_88;
  undefined4 local_84;
  float local_80;
  undefined4 local_78;
  undefined4 local_74;
  float local_70;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined8 local_58;
  undefined4 local_50;
  undefined1 local_48 [64];
  
  LocomotorCommonClass::vfunc_6();
  lVar2 = *(longlong *)(param_2 + 0x160);
  if (lVar2 == 0) {
    uVar3 = 0x80650001;
  }
  else {
    iVar1 = *(int *)(lVar2 + 8);
    if (iVar1 == 1) {
      local_80 = DAT_140b15ac0 + *(float *)(lVar2 + 0x14);
      local_98 = 0x43b40000;
      local_88 = *(undefined4 *)(lVar2 + 0xc);
      local_84 = *(undefined4 *)(lVar2 + 0x10);
      local_90 = *(undefined4 *)(lVar2 + 0x20);
      if (*(float *)(lVar2 + 0x34) <= DAT_1408524f8) {
        local_98 = 0;
      }
      uStack_94 = 0x43b40000;
      if (*(float *)(lVar2 + 0x38) <= DAT_1408524f8) {
        uStack_94 = 0;
      }
      fVar5 = (float)DAT_140b0a320 / (float)DAT_140b0a340;
      fVar4 = (fVar5 - *(float *)(lVar2 + 0x24)) / _DAT_140b15ac4;
      fVar6 = 0.0;
      if (0.0 <= fVar4) {
        fVar6 = fVar4;
      }
      if (DAT_1407ffaf8 <= fVar6) {
        FUN_1403a8f90(param_2,&local_88,0);
        FUN_1403a8710(param_2,&local_98);
        *(undefined4 *)(lVar2 + 0x28) = local_88;
        *(float *)(lVar2 + 0x30) = local_80;
        *(undefined4 *)(lVar2 + 0x2c) = local_84;
        *(undefined4 *)(lVar2 + 0x38) = uStack_94;
        *(undefined4 *)(lVar2 + 8) = 2;
        *(undefined4 *)(lVar2 + 0x34) = local_98;
        *(float *)(lVar2 + 0x24) = fVar5;
        *(undefined4 *)(lVar2 + 0x3c) = local_90;
        if (0.0 < *(float *)(lVar2 + 0x40)) {
          *(float *)(lVar2 + 0x48) = fVar5;
        }
      }
      else {
        local_70 = (local_80 - *(float *)(lVar2 + 0x30)) * fVar6 + *(float *)(lVar2 + 0x30);
        local_78 = local_88;
        local_74 = local_84;
        FUN_1403a8f90(param_2,&local_78,0);
        local_58 = CONCAT44(uStack_94,local_98);
        local_64 = *(undefined4 *)(lVar2 + 0x38);
        local_68 = *(undefined4 *)(lVar2 + 0x34);
        local_60 = *(undefined4 *)(lVar2 + 0x3c);
        local_50 = local_90;
        FUN_14063ea50(local_48,&local_68,&local_58,fVar6);
        FUN_1403a8710(param_2,local_48);
      }
    }
    else if (iVar1 == 2) {
      FUN_14063f730(param_1,param_2,lVar2);
    }
    else if (iVar1 == 3) {
      FUN_14063f470(param_1,param_2,lVar2);
    }
    uVar3 = 0x650000;
  }
  return uVar3;
}


