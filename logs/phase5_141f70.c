/* FUN_140141f70  RVA 0x141f70 */


ulonglong FUN_140141f70(longlong param_1,char *param_2)

{
  char cVar1;
  byte bVar2;
  ulonglong in_RAX;
  char *pcVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  undefined1 local_288 [64];
  undefined1 local_248 [144];
  undefined1 local_1b8 [159];
  undefined1 uStack_119;
  char local_118 [272];
  
  if (param_2 == (char *)0x0) {
    return in_RAX & 0xffffffffffffff00;
  }
  pcVar3 = param_2;
  do {
    cVar1 = *pcVar3;
    pcVar3[(longlong)(local_118 + -(longlong)param_2)] = cVar1;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  FUN_140213010(local_248);
  cVar1 = FUN_1402136f0(local_248,local_118,0,0,0);
  if (cVar1 == '\0') {
    iVar7 = 0;
    bVar2 = 0;
    if (0 < *(int *)(param_1 + 0x1c0)) {
      lVar6 = 0;
      do {
        pcVar3 = (char *)(*(longlong *)(param_1 + 0x1b8) + lVar6);
        if (0xf < *(ulonglong *)(pcVar3 + 0x18)) {
          pcVar3 = *(char **)pcVar3;
        }
        lVar5 = -(longlong)pcVar3;
        do {
          cVar1 = *pcVar3;
          pcVar3[(longlong)(local_118 + lVar5)] = cVar1;
          pcVar3 = pcVar3 + 1;
        } while (cVar1 != '\0');
        puVar4 = &uStack_119;
        do {
          pcVar3 = puVar4 + 1;
          puVar4 = puVar4 + 1;
        } while (*pcVar3 != '\0');
        lVar5 = 0;
        do {
          cVar1 = param_2[lVar5];
          puVar4[lVar5] = cVar1;
          lVar5 = lVar5 + 1;
        } while (cVar1 != '\0');
        cVar1 = FUN_1402136f0(local_248,local_118,0,0,0);
        if (cVar1 != '\0') goto LAB_1401420b3;
        iVar7 = iVar7 + 1;
        lVar6 = lVar6 + 0x20;
      } while (iVar7 < *(int *)(param_1 + 0x1c0));
    }
  }
  else {
LAB_1401420b3:
    FUN_140222830(local_1b8,param_2);
    FUN_140222c30(local_1b8,local_248);
    FUN_1402138d0(local_248);
    FUN_140220280(local_288,local_1b8);
    bVar2 = FUN_140142170(param_1,local_288,local_118);
    FUN_1402229e0(local_1b8);
    FUN_140220370(local_288);
    FUN_1402228d0(local_1b8);
  }
  FUN_1402132f0(local_248);
  return (ulonglong)bVar2;
}

