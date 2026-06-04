/* FUN_1405cd680  RVA 0x5cd680  size 600  params 0  ret undefined */


undefined8 FUN_1405cd680(longlong param_1,longlong param_2,undefined4 *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  char cVar5;
  longlong lVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float local_88;
  float local_84;
  float local_80;
  
  iVar4 = *(int *)(param_2 + 0x5c);
  lVar6 = *(longlong *)(param_1 + 0x28);
  fVar9 = ((DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4) * *(float *)(lVar6 + 0x98);
  fVar1 = *(float *)(lVar6 + 0x78);
  fVar2 = *(float *)(lVar6 + 0x7c);
  fVar3 = *(float *)(lVar6 + 0x80);
  fVar7 = fVar9;
  if (0x1d000000 < ((uint)fVar9 & 0x7f800000)) {
    fVar7 = (float)FUN_140776650(fVar9);
  }
  fVar8 = DAT_1407ffaf8;
  if (0x1d000000 < ((uint)fVar9 & 0x7f800000)) {
    fVar8 = (float)FUN_140776150(fVar9);
  }
  if (iVar4 == 0x5a) {
    lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x298) + 0x1270);
    if (lVar6 == 0) {
      lVar6 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58));
      lVar6 = *(longlong *)(*(longlong *)(lVar6 + 0x68) + 0x6c0);
      if (lVar6 != 0) goto LAB_1405cd79b;
    }
    else {
LAB_1405cd79b:
      FUN_1402d5240(&DAT_140b27e60,lVar6,*(longlong *)(param_1 + 0x28) + 0x78,0,0,0);
    }
    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x2a0);
    if (lVar6 != 0) {
      cVar5 = FUN_14039b950(*(longlong *)(param_1 + 0x28),9,1);
      if (cVar5 == '\0') {
        FUN_140265ae0(lVar6,0,1);
      }
      else {
        FUN_140265ae0(lVar6,1,1);
      }
    }
  }
  else {
    fVar9 = DAT_140875c1c;
    if (0x5a < iVar4) goto LAB_1405cd834;
  }
  lVar6 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(*(longlong *)(param_1 + 0x28) + 0x58));
  fVar9 = fRam00000000000020bc;
  if (lVar6 != 0) {
    fVar9 = *(float *)(*(longlong *)(lVar6 + 0x68) + 0x20bc);
  }
LAB_1405cd834:
  local_88 = fVar1 + fVar8 * fVar9;
  local_84 = fVar2 + fVar7 * fVar9;
  local_80 = fVar3 + fVar9 * 0.0;
  FUN_1403a8f90(*(undefined8 *)(param_1 + 0x28),&local_88,0);
  if (iVar4 + -1 == 0) {
    FUN_140391310(*(undefined8 *)(param_1 + 0x28),1);
  }
  else {
    *(int *)(param_2 + 0x5c) = iVar4 + -1;
  }
  *param_3 = 0x650000;
  return 1;
}


