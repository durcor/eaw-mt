/* FUN_140384740  RVA 0x384740 */


void FUN_140384740(longlong param_1)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  iVar3 = *(int *)(*(longlong *)(param_1 + 0x20) + 0x48);
  if ((5 < iVar3 - 5U) || (iVar3 == 10)) {
    *(undefined4 *)(param_1 + 0x88) = 0;
    return;
  }
  iVar3 = 0;
  fVar6 = 0.0;
  while( true ) {
    lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2d0);
    if (lVar1 == 0) {
      iVar2 = 0;
    }
    else {
      iVar2 = *(int *)(lVar1 + 0x10);
    }
    if (iVar2 <= iVar3) break;
    lVar1 = FUN_140396cb0(*(longlong *)(param_1 + 0x10),iVar3);
    if (lVar1 != 0) {
      iVar2 = *(int *)(*(longlong *)(lVar1 + 0x20) + 0x48);
      if (((iVar2 - 5U < 6) && (iVar2 != 10)) &&
         (*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0x210) != 0)) {
        fVar4 = (float)FUN_140370e60();
        fVar6 = fVar6 + fVar4;
      }
    }
    iVar3 = iVar3 + 1;
  }
  if (*(longlong *)(*(longlong *)(param_1 + 0x20) + 0x210) == 0) {
    *(undefined4 *)(param_1 + 0x88) = 0;
    return;
  }
  fVar4 = (float)FUN_140370e60();
  fVar5 = (float)FUN_140370e60(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298));
  *(float *)(param_1 + 0x88) = fVar5 * (fVar4 / fVar6);
  return;
}

