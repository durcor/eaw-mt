/* FUN_14039a540  RVA 0x39a540  size 112  params 0  ret undefined */


ulonglong FUN_14039a540(longlong param_1,longlong param_2)

{
  ulonglong in_RAX;
  longlong lVar1;
  longlong *plVar2;
  ulonglong uVar3;
  
  if (((param_2 != 0) && (*(char *)(param_1 + 0x3b4) != '\0')) &&
     (*(char *)(param_2 + 0x3b4) != '\0')) {
    lVar1 = (**(code **)(*DAT_140b15418 + 0x210))();
    in_RAX = 0;
    if (lVar1 != 0) {
      plVar2 = (longlong *)FUN_14056c720(lVar1,param_1);
      in_RAX = 0;
      if (plVar2 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00014039a59f. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        uVar3 = (**(code **)(*plVar2 + 0x18))(plVar2,param_2);
        return uVar3;
      }
    }
  }
  return in_RAX & 0xffffffffffffff00;
}


