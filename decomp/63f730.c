/* TelekinesisTargetBehaviorClass::FUN_14063f730  RVA 0x63f730  size 753  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void TelekinesisTargetBehaviorClass::FUN_14063f730
               (undefined8 param_1,longlong param_2,longlong param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  ulonglong uVar8;
  float local_68;
  float local_64;
  undefined4 local_60;
  undefined4 local_58;
  undefined4 local_54;
  float local_50;
  
  fVar3 = (float)DAT_140b0a340;
  uVar8 = (ulonglong)DAT_140b0a320;
  fVar5 = (float)uVar8 / fVar3 - *(float *)(param_3 + 0x24);
  fVar6 = 0.0;
  if (0.0 <= fVar5 / _DAT_140b15ac8) {
    fVar6 = fVar5 / _DAT_140b15ac8;
  }
  fVar7 = DAT_1407ffaf8;
  if (fVar5 < DAT_140b15acc) {
    fVar7 = fVar5 / DAT_140b15acc;
  }
  fVar5 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)(fVar6 * DAT_140819b3c) & 0x7f800000)) {
    fVar5 = (float)FUN_140776150();
  }
  local_60 = *(undefined4 *)(param_3 + 0x3c);
  local_64 = *(float *)(param_3 + 0x38);
  fVar4 = fVar6 * DAT_140819b3c;
  local_68 = *(float *)(param_3 + 0x34) + fVar5 * DAT_140b15ad0 * fVar7;
  if (0x1d000000 < ((uint)fVar4 & 0x7f800000)) {
    fVar4 = (float)FUN_140776650();
  }
  local_64 = local_64 + fVar4 * DAT_140b15ad0 * fVar7;
  local_68 = (float)FUN_14020b710();
  local_64 = (float)FUN_14020b710();
  FUN_1403a8710(param_2,&local_68);
  fVar6 = fVar6 * DAT_140819b3c;
  if (0x1d000000 < ((uint)fVar6 & 0x7f800000)) {
    fVar6 = (float)FUN_140776650();
  }
  local_58 = *(undefined4 *)(param_3 + 0xc);
  local_54 = *(undefined4 *)(param_3 + 0x10);
  local_50 = *(float *)(param_3 + 0x14) + fVar6 * _DAT_140b15ad4 + DAT_140b15ac0;
  FUN_1403a8f90(param_2,&local_58,0);
  if ((*(float *)(param_3 + 0x48) <= (float)uVar8 / fVar3) && (*(float *)(param_3 + 0x48) != 0.0)) {
    uVar1 = FUN_1402aca60(*(undefined8 *)(DAT_140b15418 + 0x18),*(undefined4 *)(param_3 + 0x4c));
    uVar8 = 0;
    FUN_1403a9e30(param_2,3,*(undefined4 *)(param_3 + 0x40),uVar1,0,0,0,0,0,0xffffffff,0,0,0,0);
    *(float *)(param_3 + 0x48) = *(float *)(param_3 + 0x44) + *(float *)(param_3 + 0x48);
    puVar2 = (undefined8 *)(*(longlong *)(param_2 + 0x298) + 0xf8);
    if (0xf < *(ulonglong *)(*(longlong *)(param_2 + 0x298) + 0x110)) {
      puVar2 = (undefined8 *)*puVar2;
    }
    FUN_140025760("Damaged %s for %f points of damage. %f remaning health.\n",puVar2,
                  (double)*(float *)(param_3 + 0x40),(double)*(float *)(param_2 + 0x5c));
    if (DAT_140b30288 != 0) {
      FUN_1402d5320(&DAT_140b27e60,DAT_140b30288,param_2,0,uVar8 & 0xffffffffffffff00);
    }
  }
  return;
}


