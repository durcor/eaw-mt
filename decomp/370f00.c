/* FUN_140370f00  RVA 0x370f00  size 101  params 0  ret undefined */


void FUN_140370f00(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  float fVar2;
  float fVar3;
  
  if (param_2 == (longlong *)0x0) {
    fVar3 = *(float *)(param_1 + 0x37c);
  }
  else {
    lVar1 = (**(code **)(*param_2 + 0x10))(param_2,0x16);
    if (lVar1 == 0) {
      fVar3 = *(float *)(param_1 + 0x37c);
    }
    else {
      fVar2 = (float)FUN_1404049e0(lVar1);
      fVar3 = *(float *)(param_1 + 0x37c);
      if (fVar2 <= *(float *)(param_1 + 0x37c)) {
        fVar3 = fVar2;
      }
    }
  }
  FUN_140376990(param_1,param_2,fVar3);
  return;
}


