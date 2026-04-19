// _beginthreadex called from FUN_0x22e490 (site 0x22e4c1)

// Total callers: 1

// ============================================================
// RVA: 0x22e490  Size: 199 bytes
// ============================================================

void FUN_14022e490(longlong param_1)

{
  undefined8 *puVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  int iVar4;
  uintptr_t uVar5;
  int iVar6;
  longlong local_res8 [4];
  undefined4 uStack_c;
  
  *(undefined1 *)(param_1 + 9) = 0;
  *(undefined1 *)(param_1 + 8) = 0;
  local_res8[0] = param_1;
  uVar5 = _beginthreadex((void *)0x0,0,FUN_14022e400,local_res8,0,(uint *)0x0);
  if (uVar5 != 0) {
    *(uintptr_t *)(param_1 + 0x18) = uVar5;
    do {
    } while (*(char *)(param_1 + 8) == '\0');
    uVar3 = *(undefined8 *)(param_1 + 0x10);
    uVar2 = *(undefined4 *)(param_1 + 0x20);
    iVar6 = DAT_140a155ac * 2 >> 1;
    if (iVar6 <= DAT_140a155a8) {
      iVar4 = iVar6 / 10;
      if (iVar6 / 10 < 10) {
        iVar4 = 10;
      }
      FUN_14022eb30(&PTR_vftable_140a15598,iVar4 + iVar6,0,1);
    }
    puVar1 = (undefined8 *)(DAT_140a155a0 + (longlong)DAT_140a155a8 * 0x10);
    *puVar1 = uVar3;
    puVar1[1] = CONCAT44(uStack_c,uVar2);
    DAT_140a155a8 = DAT_140a155a8 + 1;
  }
  return;
}



