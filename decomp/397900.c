/* FUN_140397900  RVA 0x397900  size 199  params 0  ret undefined */


int FUN_140397900(longlong *param_1,char param_2)

{
  longlong lVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  longlong *plVar6;
  
  lVar1 = param_1[0x53];
  if (lVar1 == 0) {
    return 0;
  }
  plVar6 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,1);
  cVar2 = '\0';
  if (plVar6 != (longlong *)0x0) {
    cVar2 = (**(code **)(*plVar6 + 0x1a8))(plVar6);
  }
  iVar5 = *(int *)(lVar1 + 0x16e0);
  if ((((*(char *)(lVar1 + 0x74) != '\0') && (plVar6 != (longlong *)0x0)) &&
      (cVar3 = (**(code **)(*plVar6 + 0x240))(plVar6), cVar3 != '\0')) &&
     (iVar4 = (**(code **)(*plVar6 + 0x238))(plVar6), iVar4 == 2)) {
    return 4;
  }
  if (((cVar2 != '\0') || (param_2 != '\0')) || (iVar5 == 0)) {
    iVar5 = *(int *)(lVar1 + 0x16dc);
  }
  return iVar5;
}


