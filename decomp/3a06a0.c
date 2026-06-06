/* FUN_1403a06a0  RVA 0x3a06a0  size 617  params 0  ret undefined */


void FUN_1403a06a0(longlong *param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 local_18;
  undefined4 local_10;
  
  plVar2 = (longlong *)(**(code **)(*param_1 + 0x10))(param_1,0x18);
  if (plVar2 != (longlong *)0x0) {
    (**(code **)(*plVar2 + 0xd0))(plVar2,param_1,param_2,param_3);
  }
  (**(code **)(*DAT_140b15418 + 0x260))(DAT_140b15418,param_1,param_2);
  lVar4 = param_1[0x20];
  if (lVar4 == 0) {
LAB_1403a0723:
    if (lVar4 != 0) goto LAB_1403a0728;
  }
  else {
    if (*(char *)(lVar4 + 0x2a4) != '\0') {
      FUN_140592d00(param_1,DAT_140800908);
      lVar4 = param_1[0x20];
      goto LAB_1403a0723;
    }
LAB_1403a0728:
    FUN_14037d1f0(lVar4 + 8,param_2,param_3);
  }
  cVar1 = FUN_14039b950(param_1,0x29,1);
  if (cVar1 == '\0') goto LAB_1403a0828;
  plVar2 = param_1;
  if ((longlong *)param_1[0x56] != (longlong *)0x0) {
    plVar2 = (longlong *)param_1[0x56];
  }
  if ((plVar2[0x20] == 0) || (*(longlong *)(plVar2[0x20] + 0x270) != param_2)) goto LAB_1403a0828;
  cVar1 = FUN_14043fa90(&DAT_140b2f2e0,0x29);
  if ((cVar1 == '\0') && ((char)plVar2[0x69] != -1)) {
    if (plVar2[0x22] != 0) {
      uVar3 = FUN_1404ec820();
      lVar4 = FUN_140397d90(uVar3,0x29);
      goto LAB_1403a07c7;
    }
  }
  else {
    lVar4 = plVar2[0xcc];
LAB_1403a07c7:
    if ((((lVar4 != 0) && (0 < *(int *)(lVar4 + 0xf8))) &&
        (lVar4 = *(longlong *)(*(longlong *)(lVar4 + 0xf0) + 8), lVar4 != 0)) &&
       (lVar4 != *(longlong *)(param_3 + 0x298))) goto LAB_1403a0828;
  }
  local_18 = 0;
  local_10 = 0;
  FUN_14038a350(param_1,0x29,0,&local_18,0xffffffff,0x3fffff);
LAB_1403a0828:
  if (((((*(char *)((longlong)param_1 + 0x34f) != -1) &&
        (lVar4 = (**(code **)(*param_1 + 0x10))(param_1,0x1d), lVar4 != 0)) &&
       ((cVar1 = FUN_1404c3700(lVar4), cVar1 != '\0' &&
        ((lVar4 = param_1[0x2f], lVar4 != 0 && (param_2 != 0)))))) &&
      (*(int *)(lVar4 + 8) == *(int *)(param_2 + 0x50))) && (*(char *)(lVar4 + 0xc) == '\0')) {
    *(undefined1 *)(lVar4 + 0xc) = 1;
    local_18 = *(undefined8 *)(param_2 + 0x78);
    local_10 = *(undefined4 *)(param_2 + 0x80);
    FUN_14038a350(param_1,5,1,&local_18,0xffffffff,0x3fffff);
    if (((char)param_1[0x6f] != -1) &&
       (lVar4 = (**(code **)(*param_1 + 0x10))(param_1,0x46), lVar4 != 0)) {
      FUN_14042fde0(lVar4,5);
    }
  }
  return;
}


