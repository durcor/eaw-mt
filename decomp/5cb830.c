/* FighterLocomotorBehaviorClass::vfunc_6  RVA 0x5cb830  size 928  params 0  ret undefined */


undefined4
FighterLocomotorBehaviorClass::vfunc_6(longlong *param_1,undefined8 param_2,undefined4 param_3)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float local_res8 [2];
  float local_48;
  float local_44;
  float local_40;
  
  lVar1 = *(longlong *)(param_1[5] + 0xa8);
  if (lVar1 == 0) {
    return 0x80650001;
  }
  cVar2 = (**(code **)(*param_1 + 0x210))();
  if (cVar2 != '\0') {
    local_res8[0] = 9.275385e-39;
    if (param_1[5] == 0) {
      return 0x80650001;
    }
    lVar1 = *(longlong *)(param_1[5] + 0xa8);
    if (lVar1 == 0) {
      return 0x80650001;
    }
    iVar7 = *(int *)(lVar1 + 0x48);
    if (iVar7 == 4) {
      FUN_1405cc970(param_1,lVar1,local_res8);
      return local_res8[0];
    }
    if (iVar7 != 5) {
      if (iVar7 == 0x28) {
        FUN_1405cd680(param_1,lVar1,local_res8);
        return local_res8[0];
      }
      if (iVar7 == 0x2c) {
        FUN_1405cbc10(param_1,lVar1,local_res8);
        return local_res8[0];
      }
      return 0x650000;
    }
    FUN_1405cd010(param_1,lVar1,local_res8);
    return local_res8[0];
  }
  LocomotorCommonClass::vfunc_6(param_1,param_2,param_3);
  lVar6 = param_1[5];
  local_48 = *(float *)(lVar6 + 0x84);
  local_44 = *(float *)(lVar6 + 0x88);
  local_40 = *(float *)(lVar6 + 0x8c);
  local_44 = (float)FUN_14020b6d0();
  fVar9 = ABS(local_44);
  local_res8[0] = fVar9;
  if (DAT_1408007ec <= fVar9) {
    local_48 = (float)FUN_14020b710(local_48 + DAT_1408524f8);
    fVar10 = DAT_1407ffaf8;
    if (local_44 < 0.0) {
      fVar10 = DAT_14080080c;
    }
    fVar9 = DAT_1408524f8 - fVar9;
    fVar8 = (float)FUN_140372440(*(undefined8 *)(param_1[5] + 0x298));
    local_44 = (float)FUN_14020b710((fVar9 - fVar8) * fVar10);
    local_40 = (float)FUN_14020b710(local_40 + DAT_1408524f8);
    FUN_1403a8710(param_1[5],&local_48);
  }
  if (*(longlong *)(param_1[5] + 0x2b0) != 0) {
    *(undefined1 *)(*(longlong *)(*(longlong *)(param_1[5] + 0x2b0) + 0x110) + 0xbc) = 1;
    lVar6 = param_1[5];
    lVar5 = *(longlong *)(lVar6 + 0xa8);
    iVar7 = *(int *)(lVar5 + 0x48);
    if ((iVar7 != 0x1c) && (iVar7 != 0x1d)) {
      uVar4 = FUN_140404950();
      FUN_1404f9d40(uVar4,lVar6);
      iVar7 = *(int *)(lVar5 + 0x48);
    }
    if (1 < iVar7 - 0x1dU) {
      lVar6 = param_1[5];
      uVar4 = FUN_140404950();
      thunk_FUN_1404f9d50(uVar4,*(undefined8 *)(lVar6 + 0x2b0));
    }
    lVar6 = *(longlong *)(*(longlong *)(param_1[5] + 0x2b0) + 0x110);
    if ((lVar6 != 0) &&
       (lVar5 = FUN_1404ec820(lVar6), *(int *)(*(longlong *)(lVar5 + 0xa8) + 0x48) != 0x1e)) {
      *(undefined1 *)(lVar6 + 0xbd) = 0;
    }
  }
  switch(*(undefined4 *)(lVar1 + 0x48)) {
  case 0:
    lVar6 = lVar1;
    if (*(longlong *)(param_1[5] + 0x2b0) != 0) {
      lVar6 = *(longlong *)(*(longlong *)(param_1[5] + 0x2b0) + 0xa8);
    }
    if ((*(longlong *)(lVar6 + 0x1f0) != 0) &&
       (lVar6 = FUN_14041a2e0(*(longlong *)(lVar6 + 0x1f0),0,0xffffffff), lVar6 != 0)) {
LAB_1405cbb7b:
      FUN_140559250(lVar1,0x1e);
      goto switchD_1405cbae5_caseD_1e;
    }
    FUN_1405c9ca0(param_1);
    FUN_140559250(lVar1,0x1b);
    goto LAB_1405cbb34;
  default:
    uVar3 = 0x80650002;
    break;
  case 0x1b:
    lVar6 = lVar1;
    if (*(longlong *)(param_1[5] + 0x2b0) != 0) {
      lVar6 = *(longlong *)(*(longlong *)(param_1[5] + 0x2b0) + 0xa8);
    }
    if ((*(longlong *)(lVar6 + 0x1f0) != 0) &&
       (lVar6 = FUN_14041a2e0(*(longlong *)(lVar6 + 0x1f0),0,0xffffffff), lVar6 != 0))
    goto LAB_1405cbb7b;
LAB_1405cbb34:
    FUN_1405ce010(param_1);
    uVar3 = 0x650000;
    break;
  case 0x1c:
    FUN_1405cd8e0(param_1);
    uVar3 = 0x650000;
    break;
  case 0x1d:
    goto switchD_1405cbae5_caseD_1d;
  case 0x1e:
switchD_1405cbae5_caseD_1e:
    FUN_1405cc220(param_1);
    uVar3 = 0x650000;
    break;
  case 0x1f:
    FUN_1405cbea0(param_1);
    goto switchD_1405cbae5_caseD_1d;
  }
  return uVar3;
switchD_1405cbae5_caseD_1d:
  return 0x650000;
}


