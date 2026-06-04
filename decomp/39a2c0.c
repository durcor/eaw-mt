/* FUN_14039a2c0  RVA 0x39a2c0  size 89  params 0  ret undefined */


ulonglong FUN_14039a2c0(longlong param_1,longlong param_2,char param_3)

{
  ulonglong uVar1;
  undefined4 uVar2;
  
  if (*(char *)(param_1 + 0x335) == -1) {
    uVar2 = *(undefined4 *)(param_1 + 0x58);
  }
  else if (param_3 == '\0') {
    uVar2 = *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x6c);
  }
  else {
    uVar2 = *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x74);
  }
  uVar1 = FUN_140294bc0(&DAT_140a16fd0,uVar2);
  if ((param_2 != 0) && (uVar1 != 0)) {
    uVar1 = FUN_1402823e0(param_2,uVar1);
    return uVar1;
  }
  return uVar1 & 0xffffffffffffff00;
}


