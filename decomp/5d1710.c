/* SpaceMovementCoordinatorClass::vfunc_7  RVA 0x5d1710  size 622  params 0  ret undefined */


bool SpaceMovementCoordinatorClass::vfunc_7(longlong param_1,undefined8 param_2,longlong param_3)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  ulonglong uVar4;
  longlong lVar5;
  uint uVar6;
  ulonglong uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  undefined8 local_res8;
  float local_48;
  float local_44;
  float local_40;
  
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
               0xd4) < DAT_140b39660) && (_Init_thread_header(&DAT_140b39660), DAT_140b39660 == -1))
  {
    atexit(FUN_1407f5970);
    _Init_thread_footer(&DAT_140b39660);
  }
  if ((longlong)DAT_140b39650 - (longlong)DAT_140b39648 >> 3 != 0) {
    DAT_140b39650 = DAT_140b39648;
  }
  uVar6 = 0;
  lVar3 = *(longlong *)(param_1 + 0x10);
  if (*(longlong *)(param_1 + 0x18) - lVar3 >> 3 != 0) {
    uVar7 = 0;
    do {
      iVar1 = FUN_14041b470(*(undefined8 *)(lVar3 + uVar7 * 8));
      if (iVar1 != 0) {
        uVar2 = FUN_14041b450(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar7 * 8),0);
        iVar1 = FUN_140397900(uVar2);
        if ((iVar1 != 0) &&
           (lVar3 = FUN_14041b450(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar7 * 8),0),
           lVar3 != *(longlong *)(param_3 + 0x28))) {
          local_res8 = FUN_14041b450(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + uVar7 * 8),0);
          if (DAT_140b39650 == DAT_140b39658) {
            FUN_140045580(&DAT_140b39648,DAT_140b39650,&local_res8);
          }
          else {
            *DAT_140b39650 = local_res8;
            DAT_140b39650 = DAT_140b39650 + 1;
          }
        }
      }
      lVar3 = *(longlong *)(param_1 + 0x10);
      uVar6 = uVar6 + 1;
      uVar7 = (ulonglong)uVar6;
    } while (uVar7 < (ulonglong)(*(longlong *)(param_1 + 0x18) - lVar3 >> 3));
  }
  if (((longlong)DAT_140b39650 - (longlong)DAT_140b39648 & 0xfffffffffffffff8U) == 0) {
    return false;
  }
  FUN_14041a9b0();
  lVar3 = 0;
  uVar6 = 0;
  uVar7 = (longlong)DAT_140b39650 - (longlong)DAT_140b39648 >> 3;
  if (uVar7 != 0) {
    uVar4 = 0;
    lVar5 = lVar3;
    fVar11 = DAT_140800804;
    do {
      lVar3 = DAT_140b39648[uVar4];
      fVar10 = *(float *)(lVar3 + 0x7c) - local_44;
      fVar8 = *(float *)(lVar3 + 0x78) - local_48;
      fVar9 = *(float *)(lVar3 + 0x80) - local_40;
      fVar8 = fVar10 * fVar10 + fVar8 * fVar8 + fVar9 * fVar9;
      if (fVar11 <= fVar8) {
        lVar3 = lVar5;
        fVar8 = fVar11;
      }
      fVar11 = fVar8;
      uVar6 = uVar6 + 1;
      uVar4 = (ulonglong)uVar6;
      lVar5 = lVar3;
    } while (uVar4 < uVar7);
  }
  *(longlong *)(param_3 + 0x28) = lVar3;
  *(undefined8 *)(param_3 + 0x30) = 0;
  return lVar3 != 0;
}


