/* FUN_1404907c0  RVA 0x4907c0  size 225  params 0  ret undefined */


void FUN_1404907c0(longlong param_1,longlong param_2)

{
  undefined4 uVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  
  plVar3 = (longlong *)(param_1 + 0x20);
  lVar4 = -1;
  do {
    lVar4 = lVar4 + 1;
  } while (*(char *)(param_2 + lVar4) != '\0');
  FUN_140022730(plVar3);
  if (0xf < *(ulonglong *)(param_1 + 0x38)) {
    plVar3 = (longlong *)*plVar3;
  }
  uVar1 = FUN_140215a30(plVar3,*(undefined4 *)(param_1 + 0x30),0);
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  *(undefined4 *)(param_1 + 0x48) = 0;
  iVar2 = 0;
  *(undefined1 *)(param_1 + 0x44) = 1;
  if (0 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3)) {
    do {
      lVar4 = FUN_140294bc0(&DAT_140a16fd0,iVar2);
      if (*(char *)(lVar4 + 0x62) != '\0') {
        *(undefined4 *)(*(longlong *)(param_1 + 0x18) + (longlong)*(int *)(param_1 + 0x48) * 0xc) =
             *(undefined4 *)(lVar4 + 0x4c);
        *(undefined4 *)
         (*(longlong *)(param_1 + 0x18) + 4 + (longlong)*(int *)(param_1 + 0x48) * 0xc) = 0;
        *(undefined4 *)
         (*(longlong *)(param_1 + 0x18) + 8 + (longlong)*(int *)(param_1 + 0x48) * 0xc) = 0;
        *(int *)(param_1 + 0x48) = *(int *)(param_1 + 0x48) + 1;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < (int)(DAT_140a16fd8 - DAT_140a16fd0 >> 3));
  }
  return;
}


