/* TelekinesisTargetBehaviorClass::FUN_14063f470  RVA 0x63f470  size 704  params 0  ret undefined */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void TelekinesisTargetBehaviorClass::FUN_14063f470
               (undefined8 param_1,longlong *param_2,longlong param_3)

{
  char cVar1;
  ulonglong uVar2;
  longlong *plVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  undefined4 local_68;
  undefined4 local_64;
  float local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined1 local_38 [48];
  
  fVar5 = (float)DAT_140b0a340;
  uVar2 = (ulonglong)DAT_140b0a320;
  fVar6 = ((float)uVar2 / fVar5 - *(float *)(param_3 + 0x24)) / _DAT_140b15ac4;
  fVar7 = 0.0;
  if (0.0 <= fVar6) {
    fVar7 = fVar6;
  }
  if (DAT_1407ffaf8 <= fVar7) {
    FUN_1403a8f90(param_2,param_3 + 0xc,0);
    FUN_1403a8710(param_2,param_3 + 0x18);
    *(float *)(param_3 + 0x24) = (float)uVar2 / fVar5;
    *(undefined8 *)(param_3 + 8) = 0;
    *(undefined8 *)(param_3 + 0x28) = 0;
    *(undefined8 *)(param_3 + 0x30) = 0;
    *(undefined8 *)(param_3 + 0x38) = 0;
    *(undefined8 *)(param_3 + 0x10) = 0;
    *(undefined8 *)(param_3 + 0x18) = 0;
    *(undefined4 *)(param_3 + 0x20) = 0;
    *(undefined8 *)(param_3 + 0x40) = 0;
    *(undefined4 *)(param_3 + 0x48) = 0;
    *(undefined4 *)(param_3 + 0x4c) = 0x3fffff;
    if ((param_2 != (longlong *)0x0) && ((*(byte *)(param_2 + 0x74) & 2) == 0)) {
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,1);
      if (plVar3 != (longlong *)0x0) {
        cVar1 = FUN_14039b1a0(param_2);
        if (cVar1 == '\0') {
          lVar4 = (**(code **)(*param_2 + 0x10))(param_2,0x51);
        }
        else {
          lVar4 = (**(code **)(*(longlong *)param_2[0x56] + 0x10))();
        }
        if ((lVar4 == 0) || (cVar1 = FUN_140443cc0(lVar4), cVar1 == '\0')) {
          (**(code **)(*plVar3 + 0x208))(plVar3);
        }
      }
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,9);
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0xb8))(plVar3);
      }
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,10);
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0xb8))(plVar3);
      }
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,0xb);
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0xb8))(plVar3);
      }
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,7);
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0xb8))(plVar3);
      }
      plVar3 = (longlong *)(**(code **)(*param_2 + 0x10))(param_2,0x20);
      if (plVar3 != (longlong *)0x0) {
        (**(code **)(*plVar3 + 0xb8))(plVar3);
      }
    }
    if (DAT_140b30298 != 0) {
      FUN_1402d5320(&DAT_140b27e60,DAT_140b30298,param_2,0,0);
    }
    return;
  }
  local_68 = *(undefined4 *)(param_3 + 0xc);
  local_64 = *(undefined4 *)(param_3 + 0x10);
  local_60 = (*(float *)(param_3 + 0x14) - *(float *)(param_3 + 0x30)) * fVar7 +
             *(float *)(param_3 + 0x30);
  FUN_1403a8f90(param_2,&local_68,0);
  local_58 = *(undefined4 *)(param_3 + 0x18);
  local_54 = *(undefined4 *)(param_3 + 0x1c);
  local_50 = *(undefined4 *)(param_3 + 0x20);
  local_48 = *(undefined4 *)(param_3 + 0x34);
  local_44 = *(undefined4 *)(param_3 + 0x38);
  local_40 = *(undefined4 *)(param_3 + 0x3c);
  FUN_14063ea50(local_38,&local_48,&local_58,fVar7);
  FUN_1403a8710(param_2,local_38);
  return;
}


