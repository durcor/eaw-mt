/* FUN_14020a9b0  RVA 0x20a9b0  size 168  params 3  ret undefined */


/* WARNING: Unknown calling convention */

ulonglong FUN_14020a9b0(ReferenceListClass *param_1,longlong param_2,char param_3)

{
  ulonglong uVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong *plVar4;
  
  if (param_3 != '\0') {
    for (uVar1 = *(ulonglong *)(param_2 + 8); uVar1 != 0; uVar1 = *(ulonglong *)(uVar1 + 0x10)) {
      if (*(ReferenceListClass **)(uVar1 + 0x20) == param_1) {
        return uVar1 & 0xffffffffffffff00;
      }
    }
  }
  plVar4 = (longlong *)FUN_140769c58(0x28);
  if (plVar4 != (longlong *)0x0) {
    plVar4[2] = 0;
    plVar4[1] = 0;
    *plVar4 = 0;
    plVar4[4] = 0;
  }
  plVar4[3] = param_2;
  plVar4[2] = *(longlong *)(param_2 + 8);
  *(longlong **)(param_2 + 8) = plVar4;
  *plVar4 = (longlong)&param_1->sentinel;
  puVar2 = param_1->head;
  plVar4[1] = (longlong)puVar2;
  *puVar2 = plVar4;
  lVar3 = *plVar4;
  *(longlong **)(lVar3 + 8) = plVar4;
  plVar4[4] = (longlong)param_1;
  param_1->count = param_1->count + 1;
  param_1->cursor_cache = -999;
  return CONCAT71((int7)((ulonglong)lVar3 >> 8),1);
}


