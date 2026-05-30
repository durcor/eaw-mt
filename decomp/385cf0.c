/* FUN_140385cf0  RVA 0x385cf0  size 376  params 0  ret undefined */


longlong FUN_140385cf0(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  undefined1 *puVar5;
  char ****ppppcVar6;
  char ***local_138 [2];
  longlong local_128;
  ulonglong local_120;
  char local_118 [272];
  
  if ((((*(longlong *)(param_1 + 0x20) != 0) &&
       (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4e) != '\0')) &&
      (*(longlong *)(param_1 + 0x10) != 0)) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2a0) != 0)) {
    lVar4 = FUN_1402648b0();
    if (lVar4 != 0) {
      FUN_14001e680(local_138,*(longlong *)(param_1 + 0x20) + 0x60);
      if (local_128 != 0) {
        iVar3 = *(int *)(param_1 + 0x90);
        if (iVar3 < 0) {
          ppppcVar6 = local_138;
          if (0xf < local_120) {
            ppppcVar6 = (char ****)local_138[0];
          }
          lVar2 = -(longlong)ppppcVar6;
          do {
            cVar1 = *(char *)ppppcVar6;
            *(char *)((longlong)ppppcVar6 + (longlong)(local_118 + lVar2)) = cVar1;
            ppppcVar6 = (char ****)((longlong)ppppcVar6 + 1);
          } while (cVar1 != '\0');
          _strupr(local_118);
          puVar5 = (undefined1 *)FUN_14076c698(local_118,&DAT_140819540);
          if (puVar5 != (undefined1 *)0x0) {
            *puVar5 = 0;
          }
          iVar3 = FUN_14012d520(lVar4,local_118);
          *(int *)(param_1 + 0x90) = iVar3;
          if (iVar3 < 0) {
            lVar4 = 0;
            goto LAB_140385df9;
          }
        }
        lVar4 = FUN_14012d6f0(lVar4,iVar3);
      }
LAB_140385df9:
      if (0xf < local_120) {
        ppppcVar6 = (char ****)local_138[0];
        if (0xfff < local_120 + 1) {
          ppppcVar6 = (char ****)local_138[0][-1];
          if ((char *)0x1f < (char *)((longlong)local_138[0] + (-8 - (longlong)ppppcVar6))) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
        }
        thunk_FUN_1407864b8(ppppcVar6);
      }
      return lVar4;
    }
  }
  return 0;
}


