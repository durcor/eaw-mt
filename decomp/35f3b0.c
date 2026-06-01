/* FUN_14035f3b0  RVA 0x35f3b0  size 189  params 0  ret undefined */


ulonglong FUN_14035f3b0(longlong *param_1,int param_2,undefined8 param_3,char param_4)

{
  longlong lVar1;
  int iVar2;
  uint uVar3;
  ulonglong uVar4;
  
  uVar4 = (**(code **)(*param_1 + 0x240))();
  if (((char)uVar4 != '\0') && (DAT_140a284c4 != '\0')) {
    if ((param_4 == '\0') &&
       (uVar4 = FUN_140294bc0(&DAT_140a16fd0,param_2), *(char *)(uVar4 + 0x62) == '\0')) {
      iVar2 = (**(code **)(*param_1 + 0xe0))(param_1);
      if (iVar2 == 0) {
        uVar4 = (ulonglong)DAT_140b15aec;
      }
      else {
        uVar3 = iVar2 - 1;
        uVar4 = (ulonglong)uVar3;
        if (uVar3 == 0) {
          uVar4 = (ulonglong)DAT_140b15aee;
        }
        else {
          if (uVar3 != 1) goto LAB_14035f456;
          uVar4 = (ulonglong)DAT_140b15aed;
        }
      }
      if ((char)uVar4 == '\0') goto LAB_14035f456;
    }
    if ((param_1[0x33] != 0) &&
       (lVar1 = *(longlong *)(param_1[0x33] + (longlong)param_2 * 8), lVar1 != 0)) {
      uVar4 = FUN_1404c0e00(lVar1,param_3);
      return uVar4;
    }
  }
LAB_14035f456:
  return uVar4 & 0xffffffffffffff00;
}


