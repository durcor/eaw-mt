/* FUN_140381dc0  RVA 0x381dc0  size 558  params 0  ret undefined */


float * FUN_140381dc0(longlong param_1,float *param_2,float param_3,float *param_4,
                     undefined8 param_5,undefined4 param_6)

{
  longlong lVar1;
  uint uVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  
  if (DAT_140b3934d == '\x01') {
    *param_2 = *param_4;
    param_2[1] = param_4[1];
    param_2[2] = param_4[2];
  }
  else {
    fVar3 = (float)FUN_140374890(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x298),param_6,
                                 *(longlong *)(param_1 + 0x10));
    uVar2 = DAT_140800860;
    if ((*(char *)(*(longlong *)(param_1 + 0x10) + 0x38a) != -1) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x230), lVar1 != 0)) {
      fVar3 = *(float *)(lVar1 + 0x14);
    }
    if (fVar3 <= 0.0) {
      fVar5 = (float)FUN_14053ff30(*(undefined8 *)(param_1 + 0x20),param_5,param_6);
      fVar3 = *param_4;
      fVar7 = param_4[1];
      fVar4 = param_4[2];
      if ((0.0 < fVar5) && (0.0 < param_3)) {
        fVar6 = (float)FUN_1403857d0(param_1);
        uVar2 = DAT_140800860;
        if (fVar6 <= 0.0) {
          *param_2 = *param_4;
          param_2[1] = param_4[1];
          param_2[2] = param_4[2];
          return param_2;
        }
        fVar6 = (fVar5 * param_3) / fVar6;
        fVar5 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar6 ^ DAT_140800860,fVar6);
        fVar3 = fVar3 + fVar5;
        fVar5 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar6 ^ uVar2,fVar6);
        fVar7 = fVar7 + fVar5;
        fVar5 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar6 ^ uVar2,fVar6);
        fVar4 = fVar4 + fVar5;
      }
      *param_2 = fVar3;
      param_2[1] = fVar7;
      param_2[2] = fVar4;
    }
    else {
      fVar4 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar3 ^ DAT_140800860,fVar3);
      fVar5 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar3 ^ uVar2,fVar3);
      fVar6 = (float)FUN_1401ffbb0(&DAT_140a13e24,(uint)fVar3 ^ uVar2,fVar3);
      fVar3 = param_4[1];
      fVar7 = *param_4;
      param_2[2] = fVar6 + param_4[2];
      param_2[1] = fVar5 + fVar3;
      *param_2 = fVar4 + fVar7;
    }
  }
  return param_2;
}


