/* SimpleSpaceLocomotorBehaviorClass::FUN_1406269f0  RVA 0x6269f0  size 653  params 0  ret undefined */


undefined8
SimpleSpaceLocomotorBehaviorClass::FUN_1406269f0
          (longlong param_1,longlong param_2,undefined4 *param_3)

{
  byte *pbVar1;
  int iVar2;
  char cVar3;
  longlong *plVar4;
  longlong lVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float local_58;
  float local_54;
  float local_50;
  
  *param_3 = 0x650000;
  iVar2 = *(int *)(param_2 + 0x5c);
  iVar6 = iVar2 + 1;
  *(int *)(param_2 + 0x5c) = iVar6;
  if (iVar6 < 0x19) {
    if (DAT_140b1574e == '\0') {
      FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),1);
      pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
      *pbVar1 = *pbVar1 | 1;
    }
    plVar4 = (longlong *)
             (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                       (*(longlong **)(param_1 + 0x28),0x18);
    if (plVar4 != (longlong *)0x0) {
      cVar3 = FUN_1404c36f0(plVar4);
      if (cVar3 == '\0') {
        (**(code **)(*plVar4 + 0xc0))(plVar4);
      }
    }
  }
  else if (iVar6 < 0x96) {
    if (iVar6 == 0x2d) {
      lVar5 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58));
      lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x68) + 0x6b0);
      if (lVar5 != 0) {
        FUN_1402d5320(&DAT_140b27e60,lVar5,*(undefined8 *)(param_1 + 0x28),0,0);
      }
    }
    else if (iVar6 == 0x78) {
      plVar4 = (longlong *)
               (**(code **)(**(longlong **)(param_1 + 0x28) + 0x10))
                         (*(longlong **)(param_1 + 0x28),iVar2 + -0x5f);
      if (plVar4 != (longlong *)0x0) {
        (**(code **)(*plVar4 + 0xb8))(plVar4);
      }
    }
    fVar8 = *(float *)(param_2 + 0x14);
    fVar9 = *(float *)(param_2 + 0x18);
    fVar10 = *(float *)(param_2 + 0x1c);
    fVar7 = (float)FUN_140776d48(fVar8 * fVar8 + fVar9 * fVar9 + fVar10 * fVar10);
    if (0.0 < fVar7) {
      fVar7 = DAT_1407ffaf8 / fVar7;
      fVar8 = fVar8 * fVar7;
      fVar9 = fVar9 * fVar7;
      fVar10 = fVar10 * fVar7;
    }
    fVar7 = (float)FUN_14049d400(&DAT_140b27a10,iVar6);
    lVar5 = *(longlong *)(param_1 + 0x28);
    local_58 = *(float *)(lVar5 + 0x78) + fVar8 * fVar7;
    local_54 = *(float *)(lVar5 + 0x7c) + fVar9 * fVar7;
    local_50 = *(float *)(lVar5 + 0x80) + fVar10 * fVar7;
    FUN_1403a8f90(lVar5,&local_58,0);
    cVar3 = SceneClass::FUN_140264cb0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0));
    if (cVar3 != '\0') {
      fVar8 = local_58 - *(float *)(param_2 + 0x2c);
      fVar9 = local_54 - *(float *)(param_2 + 0x30);
      fVar10 = local_50 - *(float *)(param_2 + 0x34);
      fVar8 = (float)FUN_140776d48(fVar8 * fVar8 + fVar9 * fVar9 + fVar10 * fVar10);
      if (fVar8 < DAT_140867698) {
        FUN_140264c80(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x2a0),0);
        pbVar1 = (byte *)(*(longlong *)(param_1 + 0x28) + 0x3a2);
        *pbVar1 = *pbVar1 & 0xfe;
      }
    }
  }
  else if (*(char *)(param_2 + 0x260) == '\x01') {
    FUN_140624dc0();
  }
  return 1;
}


