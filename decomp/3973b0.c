/* FUN_1403973b0  RVA 0x3973b0  size 156  params 0  ret undefined */


longlong FUN_1403973b0(longlong *param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (param_1 == (longlong *)0x0) {
    return 0;
  }
  lVar1 = (**(code **)(*param_1 + 0x10))(param_1,4);
  lVar2 = (**(code **)(*param_1 + 0x10))(param_1,5);
  lVar3 = (**(code **)(*param_1 + 0x10))(param_1,0x16);
  if (lVar1 == 0) {
    if (lVar2 == 0) {
      if (lVar3 == 0) goto LAB_140397435;
      lVar1 = FUN_140404c20(lVar3);
    }
    else {
      lVar1 = FUN_1404f3bb0(lVar2,param_1);
    }
  }
  else {
    lVar1 = FUN_140432780(lVar1,param_1);
  }
  if (lVar1 != 0) {
    return lVar1;
  }
LAB_140397435:
  return param_1[0x53];
}


