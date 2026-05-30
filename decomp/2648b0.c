/* FUN_1402648b0  RVA 0x2648b0  size 88  params 0  ret undefined */


/* WARNING: Removing unreachable block (ram,0x0001402648fb) */

longlong * FUN_1402648b0(longlong param_1)

{
  longlong *plVar1;
  undefined *puVar2;
  
  plVar1 = *(longlong **)(param_1 + 0x48);
  if (plVar1 != (longlong *)0x0) {
    for (puVar2 = (undefined *)(**(code **)(*plVar1 + 0x20))(plVar1); puVar2 != (undefined *)0x0;
        puVar2 = *(undefined **)(puVar2 + 8)) {
      if (puVar2 == &DAT_140a12370) {
        return plVar1;
      }
    }
  }
  return (longlong *)0x0;
}


