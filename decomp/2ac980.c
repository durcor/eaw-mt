/* FUN_1402ac980  RVA 0x2ac980  size 53  params 0  ret undefined */


void FUN_1402ac980(longlong param_1)

{
  int iVar1;
  
  if (*(char *)(param_1 + 0x63e) != '\0') {
    FUN_14028a9e0(&DAT_140b153e0);
    return;
  }
  iVar1 = *(int *)(param_1 + 0x620);
  if (iVar1 == 0x3ffffe) {
    iVar1 = 0x3fffff;
  }
  *(int *)(param_1 + 0x620) = iVar1 + 1;
  return;
}


