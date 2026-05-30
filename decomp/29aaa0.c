/* GameObjectManagerClass::FUN_14029aaa0  RVA 0x29aaa0  size 2069  params 0  ret undefined */


void GameObjectManagerClass::FUN_14029aaa0(undefined8 *param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  LPVOID pvVar3;
  undefined8 *puVar4;
  HANDLE pvVar5;
  longlong lVar6;
  undefined1 local_res8 [8];
  
  *param_1 = vftable;
  for (puVar1 = (undefined8 *)param_1[9]; puVar1 != param_1 + 8; puVar1 = (undefined8 *)puVar1[1]) {
    lVar6 = puVar1[3] + -0x18;
    if (puVar1[3] == 0) {
      lVar6 = 0;
    }
    FUN_1403a8380(lVar6);
  }
  plVar2 = (longlong *)param_1[0x19];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0x1a];
  if (puVar1 != (undefined8 *)0x0) {
    if (puVar1[-1] == 0) {
      thunk_FUN_1407864b8(puVar1 + -1,8);
    }
    else {
      (**(code **)*puVar1)(puVar1,3);
    }
  }
  plVar2 = (longlong *)param_1[0x38];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0xa8];
  if (puVar1 != (undefined8 *)0x0) {
    if (puVar1[-1] == 0) {
      thunk_FUN_1407864b8(puVar1 + -1,8);
    }
    else {
      (**(code **)*puVar1)(puVar1,3);
    }
  }
  puVar1 = (undefined8 *)param_1[0xab];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  puVar1 = (undefined8 *)param_1[0x1b];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  puVar1 = (undefined8 *)param_1[0x1c];
  if (puVar1 != (undefined8 *)0x0) {
    (**(code **)*puVar1)(puVar1,1);
  }
  plVar2 = (longlong *)param_1[0xaa];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  plVar2 = (longlong *)param_1[0xa9];
  if (plVar2 != (longlong *)0x0) {
    if (plVar2[-1] == 0) {
      thunk_FUN_1407864b8(plVar2 + -1,8);
    }
    else {
      (**(code **)(*plVar2 + 8))(plVar2,3);
    }
  }
  param_1[0x1b] = 0;
  param_1[0x38] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0xa8] = 0;
  param_1[0xab] = 0;
  param_1[0x1c] = 0;
  param_1[0xa9] = 0;
  *(undefined4 *)(param_1 + 3) = 0;
  pvVar3 = (LPVOID)param_1[0xbf];
  if (pvVar3 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar3);
    }
    else {
      pvVar5 = GetProcessHeap();
      HeapFree(pvVar5,0,pvVar3);
    }
    *(uint *)((longlong)param_1 + 0x604) = *(uint *)((longlong)param_1 + 0x604) & 0x80000000;
    param_1[0xbf] = 0;
    *(undefined4 *)(param_1 + 0xc0) = 0;
  }
  pvVar3 = (LPVOID)param_1[0xbf];
  param_1[0xbe] = DynamicVectorClass<struct_GameObjectManagerClass::CreationParamsStruct>::vftable;
  if (pvVar3 != (LPVOID)0x0) {
    if ((*(uint *)((longlong)param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar3);
    }
    else {
      pvVar5 = GetProcessHeap();
      HeapFree(pvVar5,0,pvVar3);
    }
    *(uint *)((longlong)param_1 + 0x604) = *(uint *)((longlong)param_1 + 0x604) & 0x80000000;
    param_1[0xbf] = 0;
    *(undefined4 *)(param_1 + 0xc0) = 0;
  }
  param_1[0xb5] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0xb5);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0xb5,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0xb5);
  }
  FUN_14020a960(param_1 + 0xb5);
  param_1[0xac] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0xac);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0xac,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0xac);
  }
  FUN_14020a960(param_1 + 0xac);
  FUN_1404f2f90(param_1 + 0x93);
  FUN_14029a8f0(param_1 + 0x90);
  FUN_14029a8f0(param_1 + 0x8d);
  FUN_140046490(param_1 + 0x8a);
  puVar1 = param_1 + 0x81;
  *puVar1 = MultiLinkedListClass<class_GameObjectClass>::vftable;
  puVar4 = (undefined8 *)param_1[0x83];
  while (puVar4 != param_1 + 0x82) {
    FUN_14020ac80(puVar1,local_res8);
    puVar4 = (undefined8 *)param_1[0x83];
  }
  FUN_14020a960(puVar1);
  FUN_140046490(param_1 + 0x7e);
  param_1[0x7b] = DynamicVectorRefClass<class_GameObjectClass*___ptr64>::vftable;
  FUN_14029c770(param_1 + 0x7b);
  FUN_140046490(param_1 + 0x7b);
  FUN_140046490(param_1 + 0x78);
  param_1[0x6f] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x6f);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x6f,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x6f);
  }
  FUN_14020a960(param_1 + 0x6f);
  param_1[0x66] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x66);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x66,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x66);
  }
  FUN_14020a960(param_1 + 0x66);
  param_1[0x5d] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x5d);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x5d,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x5d);
  }
  FUN_14020a960(param_1 + 0x5d);
  param_1[0x54] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x54);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x54,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x54);
  }
  FUN_14020a960(param_1 + 0x54);
  param_1[0x4b] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x4b);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x4b,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x4b);
  }
  FUN_14020a960(param_1 + 0x4b);
  param_1[0x42] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x42);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x42,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x42);
  }
  FUN_14020a960(param_1 + 0x42);
  param_1[0x39] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x39);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x39,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x39);
  }
  FUN_14020a960(param_1 + 0x39);
  param_1[0x2f] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x2f);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x2f,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x2f);
  }
  FUN_14020a960(param_1 + 0x2f);
  param_1[0x26] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x26);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x26,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x26);
  }
  FUN_14020a960(param_1 + 0x26);
  param_1[0x1d] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 0x1d);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 0x1d,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 0x1d);
  }
  FUN_14020a960(param_1 + 0x1d);
  FUN_14007b260(param_1 + 0x10);
  param_1[7] = ReferenceListClass<class_GameObjectClass>::vftable;
  lVar6 = FUN_1402ac2f0(param_1 + 7);
  while (lVar6 != 0) {
    lVar6 = FUN_14020ac80(param_1 + 7,local_res8);
    if ((lVar6 != 0) && ((longlong *)(lVar6 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar6 + -0x18) + 8))();
    }
    lVar6 = FUN_1402ac2f0(param_1 + 7);
  }
  FUN_14020a960(param_1 + 7);
  FUN_140240640(param_1);
  return;
}


