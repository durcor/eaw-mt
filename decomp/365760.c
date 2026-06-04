/* FUN_140365760  RVA 0x365760  size 94  params 0  ret undefined */


void FUN_140365760(longlong *param_1,int param_2,undefined8 param_3)

{
  longlong lVar1;
  char cVar2;
  
  cVar2 = (**(code **)(*param_1 + 0x240))();
  if ((((cVar2 != '\0') && (param_1[0x33] != 0)) && (-1 < param_2)) &&
     ((param_2 < (int)param_1[0x32] &&
      (lVar1 = *(longlong *)(param_1[0x33] + (longlong)param_2 * 8), lVar1 != 0)))) {
    FUN_1404c19b0(lVar1,param_3);
  }
  return;
}


