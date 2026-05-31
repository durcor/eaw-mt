/* FUN_140396df0  RVA 0x396df0  size 98  params 0  ret undefined */


ulonglong FUN_140396df0(longlong *param_1)

{
  longlong lVar1;
  float fVar2;
  ulonglong uVar3;
  float fVar4;
  
  if ((char)param_1[0x69] == -1) {
    fVar2 = (float)FUN_1403727a0(param_1[0x53],param_1);
    fVar4 = *(float *)((longlong)param_1 + 0x5c);
    if (fVar2 <= *(float *)((longlong)param_1 + 0x5c)) {
      fVar4 = fVar2;
    }
    *(float *)((longlong)param_1 + 0x5c) = fVar4;
    return (ulonglong)(uint)(fVar4 / fVar2);
  }
  lVar1 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
  if (lVar1 == 0) {
    return 0;
  }
  uVar3 = FUN_140405230(lVar1);
  return uVar3;
}


