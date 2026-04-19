
// ================================================================
// GOM CONSTRUCTOR — FUN_14029a0c0 (sets vftable)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x29a0c0  Name: FUN_14029a0c0  Size: 1753 bytes
// ------------------------------------------------------------

undefined8 * FUN_14029a0c0(undefined8 *param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  
  FUN_140240610();
  *param_1 = GameObjectManagerClass::vftable;
  puVar1 = param_1 + 8;
  *(undefined4 *)(param_1 + 3) = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[9] = puVar1;
  param_1[0xc] = 0;
  *puVar1 = puVar1;
  param_1[5] = 0;
  param_1[6] = 0;
  *(undefined4 *)(param_1 + 0xd) = 0;
  *(undefined4 *)((longlong)param_1 + 0x6c) = 1;
  *(undefined4 *)(param_1 + 0xe) = 0xfffffc19;
  param_1[0xf] = 0;
  param_1[0xb] = 0;
  param_1[10] = 0;
  param_1[7] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined4 *)((longlong)param_1 + 0x84) = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  lVar2 = FUN_140769c58(0x20);
  *(longlong *)lVar2 = lVar2;
  *(longlong *)(lVar2 + 8) = lVar2;
  param_1[0x11] = lVar2;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  param_1[0x16] = 7;
  param_1[0x17] = 8;
  *(undefined4 *)((longlong)param_1 + 0x84) = 0x3f800000;
  FUN_140048250(param_1 + 0x13,0x10,param_1[0x11]);
  puVar1 = param_1 + 0x1e;
  param_1[0x18] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  param_1[0x21] = 0;
  param_1[0x1f] = puVar1;
  param_1[0x22] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x27;
  param_1[0x28] = 0;
  param_1[0x29] = 0;
  param_1[0x2a] = 0;
  param_1[0x28] = puVar1;
  param_1[0x2b] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x30;
  param_1[0x32] = 0;
  param_1[0x31] = 0;
  param_1[0x33] = 0;
  param_1[0x34] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  *(undefined4 *)(param_1 + 0x23) = 0;
  *(undefined4 *)((longlong)param_1 + 0x11c) = 1;
  *(undefined4 *)(param_1 + 0x24) = 0xfffffc19;
  param_1[0x25] = 0;
  param_1[0x21] = 0;
  param_1[0x20] = 0;
  param_1[0x1d] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 0x2c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x164) = 1;
  *(undefined4 *)(param_1 + 0x2d) = 0xfffffc19;
  param_1[0x2e] = 0;
  param_1[0x2a] = 0;
  param_1[0x29] = 0;
  param_1[0x26] = ReferenceListClass<class_GameObjectClass>::vftable;
  *puVar1 = puVar1;
  param_1[0x31] = puVar1;
  puVar1 = param_1 + 0x3a;
  param_1[0x3b] = 0;
  param_1[0x3c] = 0;
  param_1[0x3d] = 0;
  param_1[0x3b] = puVar1;
  param_1[0x3e] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x43;
  param_1[0x44] = 0;
  param_1[0x45] = 0;
  param_1[0x46] = 0;
  param_1[0x44] = puVar1;
  param_1[0x47] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x4c;
  param_1[0x4d] = 0;
  param_1[0x4e] = 0;
  param_1[0x4f] = 0;
  param_1[0x4d] = puVar1;
  param_1[0x50] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x55;
  param_1[0x56] = 0;
  param_1[0x57] = 0;
  param_1[0x58] = 0;
  param_1[0x56] = puVar1;
  param_1[0x59] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x5e;
  param_1[0x5f] = 0;
  param_1[0x60] = 0;
  param_1[0x61] = 0;
  param_1[0x5f] = puVar1;
  param_1[0x62] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x67;
  *(undefined4 *)(param_1 + 0x35) = 0;
  *(undefined4 *)((longlong)param_1 + 0x1ac) = 1;
  *(undefined4 *)(param_1 + 0x36) = 0xfffffc19;
  param_1[0x37] = 0;
  param_1[0x33] = 0;
  param_1[0x32] = 0;
  param_1[0x2f] = ReferenceListClass<class_GameObjectClass>::vftable;
  param_1[0x38] = 0;
  *(undefined4 *)(param_1 + 0x3f) = 0;
  *(undefined4 *)((longlong)param_1 + 0x1fc) = 1;
  *(undefined4 *)(param_1 + 0x40) = 0xfffffc19;
  param_1[0x41] = 0;
  param_1[0x3d] = 0;
  param_1[0x3c] = 0;
  param_1[0x39] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 0x48) = 0;
  *(undefined4 *)((longlong)param_1 + 0x244) = 1;
  *(undefined4 *)(param_1 + 0x49) = 0xfffffc19;
  param_1[0x4a] = 0;
  param_1[0x46] = 0;
  param_1[0x45] = 0;
  param_1[0x42] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 0x51) = 0;
  *(undefined4 *)((longlong)param_1 + 0x28c) = 1;
  *(undefined4 *)(param_1 + 0x52) = 0xfffffc19;
  param_1[0x53] = 0;
  param_1[0x4f] = 0;
  param_1[0x4e] = 0;
  param_1[0x4b] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 0x5a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x2d4) = 1;
  *(undefined4 *)(param_1 + 0x5b) = 0xfffffc19;
  param_1[0x5c] = 0;
  param_1[0x58] = 0;
  param_1[0x57] = 0;
  param_1[0x54] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 99) = 0;
  *(undefined4 *)((longlong)param_1 + 0x31c) = 1;
  *(undefined4 *)(param_1 + 100) = 0xfffffc19;
  param_1[0x65] = 0;
  param_1[0x61] = 0;
  param_1[0x60] = 0;
  param_1[0x5d] = ReferenceListClass<class_GameObjectClass>::vftable;
  param_1[0x68] = 0;
  param_1[0x69] = 0;
  param_1[0x6a] = 0;
  param_1[0x68] = puVar1;
  param_1[0x6b] = 0;
  *puVar1 = puVar1;
  puVar1 = param_1 + 0x70;
  param_1[0x71] = 0;
  param_1[0x72] = 0;
  param_1[0x73] = 0;
  param_1[0x71] = puVar1;
  param_1[0x74] = 0;
  *puVar1 = puVar1;
  param_1[0x78] = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  *(undefined4 *)(param_1 + 0x6c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x364) = 1;
  *(undefined4 *)(param_1 + 0x6d) = 0xfffffc19;
  param_1[0x6e] = 0;
  param_1[0x6a] = 0;
  param_1[0x69] = 0;
  param_1[0x66] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)(param_1 + 0x75) = 0;
  *(undefined4 *)((longlong)param_1 + 0x3ac) = 1;
  *(undefined4 *)(param_1 + 0x76) = 0xfffffc19;
  param_1[0x77] = 0;
  param_1[0x73] = 0;
  param_1[0x72] = 0;
  param_1[0x6f] = ReferenceListClass<class_GameObjectClass>::vftable;
  param_1[0x79] = 0;
  param_1[0x7a] = 0;
  param_1[0x7b] = DynamicVectorRefClass<class_GameObjectClass*___ptr64>::vftable;
  puVar1 = param_1 + 0x82;
  param_1[0x7c] = 0;
  param_1[0x7d] = 0;
  param_1[0x7e] = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  param_1[0x7f] = 0;
  param_1[0x80] = 0;
  param_1[0x83] = 0;
  param_1[0x84] = 0;
  param_1[0x85] = 0;
  param_1[0x83] = puVar1;
  param_1[0x86] = 0;
  *puVar1 = puVar1;
  param_1[0x81] = MultiLinkedListClass<class_GameObjectClass>::vftable;
  param_1[0x8a] = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
  *(undefined4 *)(param_1 + 0x87) = 0;
  *(undefined4 *)((longlong)param_1 + 0x43c) = 1;
  *(undefined4 *)(param_1 + 0x88) = 0xfffffc19;
  param_1[0x89] = 0;
  param_1[0x85] = 0;
  param_1[0x84] = 0;
  param_1[0x8b] = 0;
  param_1[0x8c] = 0;
  param_1[0x8d] = DynamicVectorClass<struct_ConflictInfoStruct>::vftable;
  param_1[0x8e] = 0;
  param_1[0x8f] = 0;
  param_1[0x90] = DynamicVectorClass<struct_ConflictInfoStruct>::vftable;
  param_1[0x91] = 0;
  param_1[0x92] = 0;
  FUN_1404f2ec0(param_1 + 0x93);
  puVar1 = param_1 + 0xad;
  param_1[0xa8] = 0;
  param_1[0xae] = 0;
  param_1[0xb0] = 0;
  param_1[0xae] = puVar1;
  param_1[0xb0] = 0;
  param_1[0xa9] = 0;
  param_1[0xaa] = 0;
  param_1[0xab] = 0;
  param_1[0xaf] = 0;
  param_1[0xb1] = 0;
  *(undefined4 *)(param_1 + 0xb2) = 0;
  *(undefined4 *)((longlong)param_1 + 0x594) = 1;
  *(undefined4 *)(param_1 + 0xb3) = 0xfffffc19;
  param_1[0xb4] = 0;
  *puVar1 = puVar1;
  param_1[0xaf] = 0;
  puVar1 = param_1 + 0xb6;
  *puVar1 = puVar1;
  *(undefined4 *)(param_1 + 0xbb) = 0;
  param_1[0xbd] = 0;
  param_1[0xbf] = 0;
  param_1[0xc0] = 0;
  param_1[0xb7] = 0;
  param_1[0xb7] = puVar1;
  param_1[0xb8] = 0;
  param_1[0xb8] = 0;
  param_1[0xb9] = 0;
  param_1[0xb9] = 0;
  param_1[0xac] = ReferenceListClass<class_GameObjectClass>::vftable;
  *(undefined4 *)((longlong)param_1 + 0x5dc) = 1;
  *(undefined4 *)(param_1 + 0xbc) = 0xfffffc19;
  param_1[0xb5] = ReferenceListClass<class_GameObjectClass>::vftable;
  param_1[0xba] = 0;
  param_1[0xbe] = DynamicVectorClass<struct_GameObjectManagerClass::CreationParamsStruct>::vftable;
  param_1[0xc1] = 0;
  param_1[0xc2] = 0;
  *(undefined4 *)((longlong)param_1 + 0x61c) = 0;
  *(undefined2 *)((longlong)param_1 + 0x634) = 0;
  *(undefined2 *)((longlong)param_1 + 0x63c) = 0;
  *(undefined4 *)(param_1 + 0xc3) = 0xffffffff;
  *(undefined4 *)(param_1 + 0xc4) = 0x4ffffff;
  *(undefined4 *)((longlong)param_1 + 0x624) = 0xffffffff;
  *(undefined4 *)(param_1 + 0xc5) = 0x5d5e0b6b;
  *(undefined4 *)((longlong)param_1 + 0x62c) = 0x5d5e0b6b;
  *(undefined4 *)(param_1 + 0xc6) = 0x5d5e0b6b;
  *(undefined4 *)(param_1 + 199) = 0xffffffff;
  *(undefined1 *)((longlong)param_1 + 0x63e) = 1;
  return param_1;
}




// ================================================================
// GOM DESTRUCTOR/CLEANUP — FUN_14029c800
// ================================================================

// ------------------------------------------------------------
// RVA: 0x29c800  Name: FUN_14029c800  Size: 1971 bytes
// ------------------------------------------------------------

void FUN_14029c800(longlong param_1)

{
  uint *puVar1;
  LPVOID pvVar2;
  longlong lVar3;
  HANDLE pvVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  undefined1 local_res8 [8];
  undefined1 local_res10 [8];
  undefined1 local_res18 [16];
  
  *(undefined1 *)(param_1 + 0x63d) = 1;
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x18);
  if (param_1 == lVar3) {
    lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x20);
  }
  uVar8 = 0;
  lVar5 = *(longlong *)(param_1 + 0x48);
  while (lVar5 != param_1 + 0x40) {
    uVar9 = *(longlong *)(lVar5 + 0x18) - 0x18;
    if (*(longlong *)(lVar5 + 0x18) == 0) {
      uVar9 = uVar8;
    }
    FUN_1403a8380(uVar9);
    if (lVar3 != 0) {
      FUN_1402a6a00(lVar3,uVar9,0);
    }
    FUN_1402a6a00(param_1,uVar9,0);
    lVar5 = *(longlong *)(param_1 + 0x48);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x408);
  while (lVar3 != 0) {
    FUN_14020ac80(param_1 + 0x408,local_res8);
    lVar3 = FUN_1402ac2f0(param_1 + 0x408);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x38);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x38,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x38);
  }
  FUN_14029c770(param_1 + 0x3d8);
  lVar3 = FUN_1402ac2f0(param_1 + 0xe8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0xe8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0xe8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x130);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x130,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x130);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x178);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x178,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x178);
  }
  pvVar2 = *(LPVOID *)(param_1 + 0x488);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x494) & 0x80000000) == 0) {
      FUN_14029b5c0(pvVar2,3);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x494) = *(uint *)(param_1 + 0x494) & 0x80000000;
    *(undefined8 *)(param_1 + 0x488) = 0;
    *(undefined4 *)(param_1 + 0x490) = 0;
  }
  pvVar2 = *(LPVOID *)(param_1 + 0x470);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x47c) & 0x80000000) == 0) {
      FUN_14029b5c0(pvVar2,3);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x47c) = *(uint *)(param_1 + 0x47c) & 0x80000000;
    *(undefined8 *)(param_1 + 0x470) = 0;
    *(undefined4 *)(param_1 + 0x478) = 0;
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x1c8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x1c8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x1c8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x210);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x210,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x210);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 600);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 600,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 600);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x2a0);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x2a0,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x2a0);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x2e8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x2e8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x2e8);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x330);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x330,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x330);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x378);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x378,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x378);
  }
  FUN_140139170(param_1 + 0x3c0);
  pvVar2 = *(LPVOID *)(param_1 + 0x5f8);
  if (pvVar2 != (LPVOID)0x0) {
    if ((*(uint *)(param_1 + 0x604) & 0x80000000) == 0) {
      thunk_FUN_1407864b8(pvVar2);
    }
    else {
      pvVar4 = GetProcessHeap();
      HeapFree(pvVar4,0,pvVar2);
    }
    *(uint *)(param_1 + 0x604) = *(uint *)(param_1 + 0x604) & 0x80000000;
    *(undefined8 *)(param_1 + 0x5f8) = 0;
    *(undefined4 *)(param_1 + 0x600) = 0;
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x560);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x560,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x560);
  }
  lVar3 = FUN_1402ac2f0(param_1 + 0x5a8);
  while (lVar3 != 0) {
    lVar3 = FUN_14020ac80(param_1 + 0x5a8,local_res8);
    if ((lVar3 != 0) && ((longlong *)(lVar3 + -0x18) != (longlong *)0x0)) {
      (**(code **)(*(longlong *)(lVar3 + -0x18) + 8))();
    }
    lVar3 = FUN_1402ac2f0(param_1 + 0x5a8);
  }
  uVar9 = uVar8;
  if (0 < *(int *)(param_1 + 0x18)) {
    do {
      iVar7 = (int)uVar8;
      if (*(longlong *)(param_1 + 200) != 0) {
        lVar3 = *(longlong *)(param_1 + 200) + (longlong)iVar7 * 0x48;
        lVar5 = FUN_1402ac2f0(lVar3);
        while (lVar5 != 0) {
          lVar5 = FUN_14020ac80(lVar3,local_res8);
          if ((lVar5 != 0) && ((longlong *)(lVar5 + -0x18) != (longlong *)0x0)) {
            (**(code **)(*(longlong *)(lVar5 + -0x18) + 8))();
          }
          lVar5 = FUN_1402ac2f0(lVar3);
        }
// ... [truncated at 200 lines, total 258]


// ================================================================
// GOM PLAYER REGISTRATION — FUN_1402af130
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2af130  Name: FUN_1402af130  Size: 947 bytes
// ------------------------------------------------------------

void FUN_1402af130(longlong param_1,int param_2,undefined8 param_3,undefined1 param_4)

{
  ulonglong *puVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  int iVar8;
  ulonglong uVar9;
  longlong lVar10;
  ulonglong *puVar11;
  ulonglong *puVar12;
  ulonglong *puVar13;
  ulonglong *puVar14;
  ulonglong uVar15;
  
  *(int *)(param_1 + 0x18) = param_2;
  uVar15 = (ulonglong)param_2;
  auVar2._8_8_ = 0;
  auVar2._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar2,0);
  if (SUB168(ZEXT816(0x48) * auVar2,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  *(undefined8 *)(param_1 + 0x20) = param_3;
  lVar10 = uVar9 + 8;
  *(undefined1 *)(param_1 + 0x63e) = param_4;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  puVar13 = (ulonglong *)0x0;
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    }
  }
  *(ulonglong **)(param_1 + 0x1c0) = puVar12;
  auVar3._8_8_ = 0;
  auVar3._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar3,0);
  if (SUB168(ZEXT816(0x48) * auVar3,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    }
  }
  *(ulonglong **)(param_1 + 200) = puVar12;
  auVar4._8_8_ = 0;
  auVar4._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x18) * auVar4,0);
  if (SUB168(ZEXT816(0x18) * auVar4,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar11 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar11 = puVar11 + 1;
    puVar12 = puVar11;
    for (uVar9 = uVar15; uVar9 != 0; uVar9 = uVar9 - 1) {
      *puVar12 = (ulonglong)DynamicVectorClass<struct_CompanyStruct*___ptr64>::vftable;
      puVar12[1] = 0;
      puVar12[2] = 0;
      puVar12 = puVar12 + 3;
    }
  }
  *(ulonglong **)(param_1 + 0x540) = puVar11;
  auVar5._8_8_ = 0;
  auVar5._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x38) * auVar5,0);
  if (SUB168(ZEXT816(0x38) * auVar5,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar12 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  puVar11 = puVar13;
  if (puVar12 != (ulonglong *)0x0) {
    puVar11 = puVar12 + 1;
    *puVar12 = uVar15;
    puVar12 = puVar11;
    for (uVar9 = uVar15; uVar9 != 0; uVar9 = uVar9 - 1) {
      FUN_14020e370(puVar12);
      puVar12 = puVar12 + 7;
    }
  }
  *(ulonglong **)(param_1 + 0xd0) = puVar11;
  lVar10 = FUN_140769c58(0x38);
  puVar11 = puVar13;
  if (lVar10 != 0) {
    puVar11 = (ulonglong *)FUN_14020e370(lVar10);
  }
  *(ulonglong **)(param_1 + 0xd8) = puVar11;
  lVar10 = FUN_140769c58(0x38);
  puVar11 = puVar13;
  if (lVar10 != 0) {
    puVar11 = (ulonglong *)FUN_14020e370(lVar10);
  }
  *(ulonglong **)(param_1 + 0xe0) = puVar11;
  auVar6._8_8_ = 0;
  auVar6._0_8_ = uVar15;
  uVar9 = SUB168(ZEXT816(0x48) * auVar6,0);
  if (SUB168(ZEXT816(0x48) * auVar6,8) != 0) {
    uVar9 = 0xffffffffffffffff;
  }
  lVar10 = uVar9 + 8;
  if (0xfffffffffffffff7 < uVar9) {
    lVar10 = -1;
  }
  puVar11 = (ulonglong *)thunk_FUN_140769c58(lVar10);
  if (puVar11 == (ulonglong *)0x0) {
    puVar12 = (ulonglong *)0x0;
  }
  else {
    *puVar11 = uVar15;
    puVar12 = puVar11 + 1;
    if (uVar15 != 0) {
      puVar11 = puVar11 + 3;
      puVar14 = puVar12;
      uVar9 = uVar15;
      do {
        puVar11[3] = 0;
        puVar1 = puVar11 + -1;
        *puVar11 = (ulonglong)puVar1;
        *(undefined4 *)(puVar11 + 4) = 0;
        *(undefined4 *)((longlong)puVar11 + 0x24) = 1;
        *(undefined4 *)(puVar11 + 5) = 0xfffffc19;
        puVar11[6] = 0;
        puVar11[2] = 0;
        puVar11[1] = 0;
        puVar11 = puVar11 + 9;
        *puVar14 = (ulonglong)ReferenceListClass<class_GameObjectClass>::vftable;
        puVar14 = puVar14 + 9;
        *puVar1 = (ulonglong)puVar1;
// ... [truncated at 200 lines, total 255]


// ================================================================
// GOM SKIRMISH SETUP — FUN_1402a74f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2a74f0  Name: FUN_1402a74f0  Size: 129 bytes
// ------------------------------------------------------------

void FUN_1402a74f0(longlong param_1,undefined1 param_2)

{
  longlong lVar1;
  longlong lVar2;
  
  lVar2 = *(longlong *)(param_1 + 0x48);
  if (lVar2 == param_1 + 0x40) {
    *(undefined1 *)(param_1 + 0x63c) = param_2;
  }
  else {
    do {
      lVar1 = *(longlong *)(lVar2 + 0x18) + -0x18;
      if (*(longlong *)(lVar2 + 0x18) == 0) {
        lVar1 = 0;
      }
      FUN_1403935b0(lVar1,param_2,0,1);
      lVar2 = *(longlong *)(lVar2 + 8);
    } while (lVar2 != param_1 + 0x40);
    *(undefined1 *)(param_1 + 0x63c) = param_2;
  }
  return;
}




// ================================================================
// LIGHT_LOOP ENDPOINT — FUN_14030ac40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x30ac40  Name: FUN_14030ac40  Size: 86 bytes
// ------------------------------------------------------------

void FUN_14030ac40(longlong param_1)

{
  if (DAT_140b30690 != 0) {
    FUN_14044f130(&DAT_140b306dd);
  }
  FUN_14025c490(*(undefined8 *)(param_1 + 0xff0),*(undefined8 *)(param_1 + 0xdd0));
  *(undefined1 *)(param_1 + 0x26b1) = 0;
  *(undefined1 *)(param_1 + 0x26d8) = 0;
  FUN_140201910(param_1 + 0x26b8,0,0);
  return;
}




// ================================================================
// Find_All_By_Type — FUN_1402a9ff0 (for vtable offset reference)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2a9ff0  Name: FUN_1402a9ff0  Size: 521 bytes
// ------------------------------------------------------------

longlong FUN_1402a9ff0(longlong param_1,int param_2,int param_3,int param_4,undefined8 param_5,
                      float param_6,int param_7,int param_8)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  double dVar7;
  
  *(undefined4 *)(param_1 + 0x460) = 0;
  if (param_3 == -1) {
    if (param_2 == 2) {
      lVar5 = param_1 + 0x2a0;
    }
    else if (param_2 == 3) {
      lVar5 = param_1 + 0x560;
    }
    else {
      lVar5 = param_1 + 0x38;
    }
  }
  else {
    lVar5 = *(longlong *)(param_1 + 200) + (longlong)param_3 * 0x48;
  }
  for (lVar1 = *(longlong *)(lVar5 + 0x10); lVar1 != lVar5 + 8; lVar1 = *(longlong *)(lVar1 + 8)) {
    lVar6 = *(longlong *)(lVar1 + 0x18) + -0x18;
    if (*(longlong *)(lVar1 + 0x18) == 0) {
      lVar6 = 0;
    }
    if ((((*(int *)(lVar6 + 0x58) != param_4) &&
         ((param_8 == -1 || (cVar2 = FUN_14039a450(lVar6,param_8,0), cVar2 != '\0')))) &&
        ((param_2 == -1 || (*(char *)((longlong)param_2 + 0x332 + lVar6) != -1)))) &&
       ((param_6 == 0.0 ||
        (dVar7 = (double)FUN_140397640(lVar6), dVar7 < (double)param_6 * (double)param_6)))) {
      iVar3 = *(int *)(param_1 + 0x460);
      iVar4 = *(int *)(param_1 + 0x464) * 2 >> 1;
      if (iVar4 <= iVar3) {
        iVar3 = iVar4 / 10;
        if (iVar4 / 10 < 10) {
          iVar3 = 10;
        }
        FUN_140047aa0(param_1 + 0x450,iVar3 + iVar4,0,1);
        iVar3 = *(int *)(param_1 + 0x460);
      }
      *(longlong *)(*(longlong *)(param_1 + 0x458) + (longlong)iVar3 * 8) = lVar6;
      *(int *)(param_1 + 0x460) = *(int *)(param_1 + 0x460) + 1;
      if (param_7 <= *(int *)(param_1 + 0x460)) break;
    }
  }
  return param_1 + 0x450;
}




// ================================================================
// GOM NEIGHBOR — FUN_1402a87d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2a87d0  Name: FUN_1402a87d0  Size: 280 bytes
// ------------------------------------------------------------

longlong FUN_1402a87d0(longlong param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  
  *(undefined4 *)(param_1 + 0x400) = 0;
  if (param_2 != 0) {
    if (param_3 == 0) {
      param_3 = param_1 + 0x38;
    }
    for (lVar1 = *(longlong *)(param_3 + 0x10); lVar1 != param_3 + 8;
        lVar1 = *(longlong *)(lVar1 + 8)) {
      lVar4 = *(longlong *)(lVar1 + 0x18) + -0x18;
      if (*(longlong *)(lVar1 + 0x18) == 0) {
        lVar4 = 0;
      }
      if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x298) == param_2)) {
        iVar2 = *(int *)(param_1 + 0x400);
        iVar3 = *(int *)(param_1 + 0x404) * 2 >> 1;
        if (iVar3 <= iVar2) {
          iVar2 = iVar3 / 10;
          if (iVar3 / 10 < 10) {
            iVar2 = 10;
          }
          FUN_140047aa0(param_1 + 0x3f0,iVar2 + iVar3,0,1);
          iVar2 = *(int *)(param_1 + 0x400);
        }
        *(longlong *)(*(longlong *)(param_1 + 0x3f8) + (longlong)iVar2 * 8) = lVar4;
        *(int *)(param_1 + 0x400) = *(int *)(param_1 + 0x400) + 1;
      }
    }
    return param_1 + 0x3f0;
  }
  return param_1 + 0x3f0;
}




// ================================================================
// GOM NEIGHBOR after A9FF0 — FUN_1402aaca0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2aa8e0  Name: FUN_1402aa8e0  Size: 1202 bytes
// ------------------------------------------------------------

void FUN_1402aa8e0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  longlong *plVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  HANDLE hHeap;
  longlong lVar11;
  longlong lVar12;
  undefined **local_58;
  LPVOID local_50;
  ulonglong local_48;
  
  if ((param_2 == (longlong *)0x0) ||
     (lVar6 = (**(code **)(*param_2 + 0x10))(param_2,3), lVar6 == 0)) goto LAB_1402aacd9;
  iVar3 = *(int *)(param_1 + 0x61c);
  if (iVar3 == 1) {
    uVar4 = *(undefined4 *)(*(longlong *)(param_1 + 0x470) + 4);
    lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
    if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) != '\0')) {
      FUN_1402ba6c0(param_1,param_2,uVar4);
    }
    uVar4 = *(undefined4 *)(*(longlong *)(param_1 + 0x470) + 8);
    lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
    if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) != '\0')) {
      FUN_1402ba6c0(param_1,param_2,uVar4);
    }
    iVar3 = FUN_1403f9520(lVar6,param_2);
    if (iVar3 == -1) goto LAB_1402aacd9;
    uVar4 = FUN_1403f9520(lVar6,param_2);
    lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
    if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) == '\0')) goto LAB_1402aacd9;
  }
  else {
    if (iVar3 == 2) {
      uVar4 = *(undefined4 *)(*(longlong *)(param_1 + 0x470) + 4);
      lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
      if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) == '\0')) {
        FUN_1402ba6c0(param_1,param_2,uVar4);
      }
      uVar4 = *(undefined4 *)(*(longlong *)(param_1 + 0x470) + 8);
      lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
      if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) == '\0')) {
        FUN_1402ba6c0(param_1,param_2,uVar4);
      }
      iVar3 = FUN_1403f9520(lVar6,param_2);
    }
    else {
      if (iVar3 != 3) goto LAB_1402aacd9;
      lVar11 = param_2[0x17];
      iVar3 = *(int *)(lVar11 + 0xd0) + -1;
      lVar12 = (longlong)iVar3;
      if (-1 < iVar3) {
        do {
          lVar1 = *(longlong *)(*(longlong *)(lVar11 + 200) + lVar12 * 8);
          if ((lVar1 != 0) &&
             (lVar7 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(lVar1 + 0x58)),
             *(char *)(lVar7 + 0x62) == '\0')) {
            local_58 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
            local_50 = (LPVOID)0x0;
            local_48 = 0;
            FUN_1403965f0(lVar1,&local_58,0);
            lVar7 = (longlong)((int)local_48 + -1);
            if (-1 < (int)local_48 + -1) {
              do {
                plVar2 = *(longlong **)((longlong)local_50 + lVar7 * 8);
                if (plVar2 != (longlong *)0x0) {
                  uVar8 = FUN_1403973b0(plVar2);
                  iVar3 = FUN_1403711c0(uVar8,0);
                  if (0 < iVar3) {
                    uVar9 = FUN_140294bc0(&DAT_140a16fd0,(int)plVar2[0xb]);
                    FUN_1403711c0(uVar8,0);
                    FUN_14027f370(uVar9);
                  }
                  if (*(char *)((longlong)plVar2 + 0x337) != -1) {
                    lVar10 = (**(code **)(*plVar2 + 0x10))(plVar2,5);
                    if (lVar10 != 0) {
                      FUN_1404f3a20(lVar10,plVar2);
                    }
                  }
                  if (*(char *)((longlong)plVar2 + 0x336) != -1) {
                    lVar10 = (**(code **)(*plVar2 + 0x10))(plVar2,4);
                    if (lVar10 != 0) {
                      FUN_140431860(lVar10,plVar2);
                    }
                  }
                  FUN_140391310(plVar2);
                }
                lVar7 = lVar7 + -1;
              } while (-1 < lVar7);
            }
            FUN_140391310(lVar1,1);
            local_58 = DynamicVectorClass<class_GameObjectClass*___ptr64>::vftable;
            if (local_50 != (LPVOID)0x0) {
              if ((local_48 & 0x8000000000000000) == 0) {
                thunk_FUN_1407864b8();
              }
              else {
                hHeap = GetProcessHeap();
                HeapFree(hHeap,0,local_50);
              }
            }
          }
          lVar12 = lVar12 + -1;
        } while (-1 < lVar12);
      }
      iVar3 = FUN_1403f9520(lVar6,param_2);
    }
    if (iVar3 == -1) goto LAB_1402aacd9;
    uVar4 = FUN_1403f9520(lVar6,param_2);
    lVar11 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
    if ((lVar11 != 0) && (*(char *)(lVar11 + 0x62) != '\0')) goto LAB_1402aacd9;
    lVar11 = FUN_1403f94e0(lVar6,param_2);
    iVar3 = 0;
    if (0 < *(int *)(lVar11 + 0x10)) {
      lVar12 = 0;
      do {
        uVar8 = *(undefined8 *)(*(longlong *)(lVar12 + *(longlong *)(lVar11 + 8)) + 0x298);
        iVar5 = FUN_1403711c0(uVar8,param_2);
        if (0 < iVar5) {
          uVar9 = FUN_140294bc0(&DAT_140a16fd0,
                                *(undefined4 *)
                                 (*(longlong *)(lVar12 + *(longlong *)(lVar11 + 8)) + 0x58));
          FUN_1403711c0(uVar8,0);
          FUN_14027f370(uVar9);
        }
        iVar3 = iVar3 + 1;
        lVar12 = lVar12 + 8;
      } while (iVar3 < *(int *)(lVar11 + 0x10));
    }
    lVar11 = FUN_1403f8b40(lVar6,*(undefined8 *)(lVar6 + 0x28));
    if ((lVar11 != 0) && (iVar3 = FUN_1403711c0(lVar11,0), 0 < iVar3)) {
      uVar4 = FUN_1403f9520(lVar6,param_2);
      uVar8 = FUN_140294bc0(&DAT_140a16fd0,uVar4);
      FUN_1403711c0(lVar11,0);
      FUN_14027f370(uVar8);
    }
  }
  FUN_1403f7440(lVar6,param_2);
  FUN_1403fe750(lVar6,param_2,0,0);
LAB_1402aacd9:
  *(undefined4 *)(param_1 + 0x61c) = 0;
  return;
}




// ================================================================
// GOM NEIGHBOR before A9FF0 — FUN_1402a8ff0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2a8fb0  Name: FUN_1402a8fb0  Size: 1469 bytes
// ------------------------------------------------------------

longlong FUN_1402a8fb0(longlong param_1,int param_2,int param_3,float *param_4,float param_5,
                      int param_6,char param_7)

{
  float fVar1;
  REFMINIPDB2 *pRVar2;
  int iVar3;
  undefined8 *puVar4;
  REFMINIPDB2 **ppRVar5;
  int iVar6;
  int iVar7;
  REFMINIPDB2 **ppRVar8;
  REFMINIPDB2 *pRVar9;
  ulonglong uVar10;
  longlong lVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  REFMINIPDB2 **local_res8;
  int local_res10;
  float local_res18 [2];
  float local_d0;
  float local_cc;
  float local_c8;
  float local_c4;
  float local_c0;
  float local_bc;
  float local_b8;
  float local_b4;
  float local_b0;
  float local_ac;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  
  ppRVar5 = (REFMINIPDB2 **)0x0;
  *(undefined4 *)(param_1 + 0x460) = 0;
  fVar1 = DAT_1408007c0;
  if (param_3 == -1) {
    lVar11 = *(longlong *)(param_1 + 0xd8);
  }
  else {
    lVar11 = (longlong)param_3 * 0x38 + *(longlong *)(param_1 + 0xd0);
  }
  if (lVar11 != 0) {
    fVar14 = param_4[2] - DAT_140800804;
    fVar17 = param_4[2] + DAT_140800804;
    fVar12 = *param_4 - param_5;
    fVar13 = param_4[1] - param_5;
    local_c4 = param_5;
    fVar15 = *param_4 + param_5;
    fVar16 = param_4[1] + param_5;
    local_res18[0] = fVar17 - fVar14;
    local_b8 = (fVar17 + fVar14) * DAT_1408007c0;
    local_bc = (fVar16 + fVar13) * DAT_1408007c0;
    local_b4 = ABS(fVar15 - fVar12) * DAT_1408007c0;
    local_b0 = ABS(fVar16 - fVar13) * DAT_1408007c0;
    local_c0 = (fVar15 + fVar12) * DAT_1408007c0;
    local_cc = param_4[1];
    local_ac = ABS(local_res18[0]) * DAT_1408007c0;
    local_d0 = *param_4;
    local_c8 = param_4[2];
    local_res10 = param_2;
    FUN_14020e780(lVar11,&local_c0);
    for (lVar11 = *(longlong *)(lVar11 + 8); lVar11 != 0; lVar11 = *(longlong *)(lVar11 + 0x18)) {
      ppRVar8 = (REFMINIPDB2 **)(*(longlong *)(lVar11 + 8) + -0x28);
      if (*(longlong *)(lVar11 + 8) == 0) {
        ppRVar8 = ppRVar5;
      }
      if ((*(char *)((longlong)ppRVar8 + 0x3a7) == '\0') &&
         ((param_2 == -1 || (*(char *)((longlong)param_2 + 0x332 + (longlong)ppRVar8) != -1)))) {
        fVar12 = *(float *)((longlong)ppRVar8 + 0x7c) - param_4[1];
        if (param_5 * param_5 <
            fVar12 * fVar12 +
            (*(float *)(ppRVar8 + 0xf) - *param_4) * (*(float *)(ppRVar8 + 0xf) - *param_4)) {
          puVar4 = (undefined8 *)(**(code **)(ppRVar8[5] + 8))(ppRVar8 + 5);
          if (*(char *)((longlong)ppRVar8 + 0x36b) != -1) {
            fVar12 = *(float *)(ppRVar8 + 0xf);
            fVar13 = *(float *)((longlong)ppRVar8 + 0x7c);
            FUN_140396850(ppRVar8,&local_res8,local_res18);
            fVar14 = *(float *)(puVar4 + 1) - *(float *)((longlong)puVar4 + 0x14);
            fVar16 = fVar12 - local_res8._0_4_;
            fVar12 = fVar12 + local_res8._0_4_;
            fVar15 = *(float *)((longlong)puVar4 + 0x14) + *(float *)(puVar4 + 1);
            fVar17 = fVar13 - local_res18[0];
            fVar13 = fVar13 + local_res18[0];
            uStack_a0 = (REFMINIPDB2 **)
                        CONCAT44(ABS(fVar16 - fVar12) * fVar1,(fVar14 + fVar15) * fVar1);
            local_a8 = (REFMINIPDB2 *)CONCAT44((fVar17 + fVar13) * fVar1,(fVar16 + fVar12) * fVar1);
            local_98 = (REFMINIPDB2 **)
                       CONCAT44(ABS(fVar14 - fVar15) * fVar1,ABS(fVar17 - fVar13) * fVar1);
            puVar4 = &local_a8;
          }
          iVar3 = FUN_14020c000(puVar4,ppRVar8 + 0x49,&local_d0);
          param_2 = local_res10;
          if (iVar3 != 8) goto LAB_1402a9394;
          iVar3 = *(int *)(param_1 + 0x460);
          iVar6 = *(int *)(param_1 + 0x464) * 2 >> 1;
          if (iVar6 <= iVar3) {
            iVar3 = iVar6 / 10;
            if (iVar3 < 10) {
              iVar3 = 10;
            }
            goto LAB_1402a9367;
          }
        }
        else {
          iVar3 = *(int *)(param_1 + 0x460);
          iVar6 = *(int *)(param_1 + 0x464) * 2 >> 1;
          if (iVar6 <= iVar3) {
            iVar3 = iVar6 / 10;
            if (iVar3 < 10) {
              iVar3 = 10;
            }
LAB_1402a9367:
            FUN_140047aa0(param_1 + 0x450,iVar3 + iVar6,0,1);
            iVar3 = *(int *)(param_1 + 0x460);
          }
        }
        *(REFMINIPDB2 ***)(*(longlong *)(param_1 + 0x458) + (longlong)iVar3 * 8) = ppRVar8;
        *(int *)(param_1 + 0x460) = *(int *)(param_1 + 0x460) + 1;
        param_2 = local_res10;
        if (param_6 <= *(int *)(param_1 + 0x460)) break;
      }
LAB_1402a9394:
    }
    if ((param_7 != '\0') && (iVar3 = *(int *)(param_1 + 0x460), 0 < iVar3)) {
      DAT_140b15670._0_4_ = *param_4;
      DAT_140b15670._4_4_ = param_4[1];
      DAT_140b15678 = param_4[2];
      local_a8 = (REFMINIPDB2 *)0x0;
      uStack_a0 = (REFMINIPDB2 **)0x0;
      local_98 = (REFMINIPDB2 **)0x0;
      ppRVar8 = ppRVar5;
      local_res8 = ppRVar5;
      do {
        if (uStack_a0 == local_res8) {
          FUN_140045580(&local_a8);
          local_res8 = local_98;
        }
        else {
          *uStack_a0 = *(REFMINIPDB2 **)(*(longlong *)(param_1 + 0x458) + (longlong)ppRVar8 * 8);
          uStack_a0 = uStack_a0 + 1;
        }
        pRVar2 = local_a8;
        ppRVar8 = (REFMINIPDB2 **)((longlong)ppRVar8 + 1);
      } while ((longlong)ppRVar8 < (longlong)iVar3);
      std::
      _Sort_unchecked<struct_REFMINIPDB2*___ptr64*___ptr64,bool_(__cdecl*)(struct_REFMINIPDB2_const*___ptr64,struct_REFMINIPDB2_const*___ptr64)>
                ((REFMINIPDB2 **)local_a8,uStack_a0,(longlong)uStack_a0 - (longlong)local_a8 >> 3,
                 FUN_1402c0720);
      *(undefined4 *)(param_1 + 0x460) = 0;
      ppRVar8 = ppRVar5;
      do {
        iVar7 = *(int *)(param_1 + 0x464) * 2 >> 1;
        iVar6 = (int)ppRVar5;
        if (iVar7 <= iVar6) {
          iVar6 = iVar7 / 10;
          if (iVar7 / 10 < 10) {
            iVar6 = 10;
          }
          FUN_140047aa0(param_1 + 0x450,iVar6 + iVar7,0,1);
          iVar6 = *(int *)(param_1 + 0x460);
        }
        lVar11 = (longlong)ppRVar8 * 8;
        ppRVar8 = (REFMINIPDB2 **)((longlong)ppRVar8 + 1);
        *(undefined8 *)(*(longlong *)(param_1 + 0x458) + (longlong)iVar6 * 8) =
             *(undefined8 *)(pRVar2 + lVar11);
        *(int *)(param_1 + 0x460) = *(int *)(param_1 + 0x460) + 1;
        ppRVar5 = (REFMINIPDB2 **)(ulonglong)*(uint *)(param_1 + 0x460);
      } while ((longlong)ppRVar8 < (longlong)iVar3);
      if (pRVar2 != (REFMINIPDB2 *)0x0) {
        uVar10 = (longlong)local_res8 - (longlong)pRVar2 & 0xfffffffffffffff8;
        pRVar9 = pRVar2;
        if (0xfff < uVar10) {
          pRVar9 = *(REFMINIPDB2 **)(pRVar2 + -8);
          uVar10 = uVar10 + 0x27;
          if ((REFMINIPDB2 *)0x1f < pRVar2 + (-8 - (longlong)pRVar9)) {
                    /* WARNING: Subroutine does not return */
            FUN_1407711cc();
          }
        }
        thunk_FUN_1407864b8(pRVar9,uVar10);
      }
    }
  }
  return param_1 + 0x450;
}




// ================================================================
// SCENE POST-CALL — FUN_140305890 (930 bytes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x305890  Name: FUN_140305890  Size: 930 bytes
// ------------------------------------------------------------

void FUN_140305890(longlong param_1,undefined4 param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  longlong *plVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined8 *puVar11;
  longlong *plVar12;
  longlong *plVar13;
  longlong lVar14;
  
  iVar8 = FUN_14025e960();
  iVar9 = FUN_14025e930();
  if (((float)iVar8 != *(float *)(param_1 + 0x1008)) ||
     ((float)iVar9 != *(float *)(param_1 + 0x100c))) {
    FUN_140304770(param_1,0);
  }
  cVar7 = FUN_1402cd0c0(&DAT_140b27a10);
  if (((cVar7 != '\0') && (*(char *)(param_1 + 0x21a8) != '\0')) &&
     (*(code **)(param_1 + 0x27d0) != (code *)0x0)) {
    (**(code **)(param_1 + 0x27d0))();
    *(undefined1 *)(param_1 + 0x21a8) = 0;
  }
  iVar8 = 0;
  if (*(char *)(param_1 + 0x2699) != '\0') {
    FUN_1403def00(param_1,0);
    *(undefined8 *)(param_1 + 0x21a0) = 0;
    *(undefined8 *)(param_1 + 0x2030) = 0;
  }
  if ((*(int *)(param_1 + 0x211c) != 0) && (*(char *)(param_1 + 0x2699) == '\0')) {
    plVar12 = (longlong *)**(longlong **)(param_1 + 0x640);
    if (plVar12 != *(longlong **)(param_1 + 0x640)) {
      do {
        (**(code **)(*(longlong *)plVar12[5] + 0x60))((longlong *)plVar12[5],param_2);
        plVar13 = (longlong *)plVar12[2];
        if (*(char *)((longlong)plVar13 + 0x19) == '\0') {
          cVar7 = *(char *)(*plVar13 + 0x19);
          plVar12 = plVar13;
          plVar13 = (longlong *)*plVar13;
          while (cVar7 == '\0') {
            cVar7 = *(char *)(*plVar13 + 0x19);
            plVar12 = plVar13;
            plVar13 = (longlong *)*plVar13;
          }
        }
        else {
          cVar7 = *(char *)(plVar12[1] + 0x19);
          plVar6 = (longlong *)plVar12[1];
          plVar13 = plVar12;
          while ((plVar12 = plVar6, cVar7 == '\0' && (plVar13 == (longlong *)plVar12[2]))) {
            cVar7 = *(char *)(plVar12[1] + 0x19);
            plVar6 = (longlong *)plVar12[1];
            plVar13 = plVar12;
          }
        }
      } while (plVar12 != *(longlong **)(param_1 + 0x640));
    }
    if (*(char *)(param_1 + 0x2268) != '\0') {
      FUN_140265280(*(undefined8 *)(param_1 + 0x2248 + (longlong)*(int *)(param_1 + 0x226c) * 8),
                    param_2,0);
    }
    if (*(char *)(param_1 + 0x2698) != '\0') {
      puVar11 = (undefined8 *)(param_1 + 0x2288);
      lVar14 = 10;
      do {
        FUN_140265280(*puVar11,param_2,0);
        puVar11 = puVar11 + 0xd;
        lVar14 = lVar14 + -1;
      } while (lVar14 != 0);
    }
    if ((*(char *)(param_1 + 4000) != '\0') && (*(longlong *)(param_1 + 0xfa8) != 0)) {
      FUN_140265280(*(longlong *)(param_1 + 0xfa8),param_2,0);
    }
    if (*(longlong *)(param_1 + 0x2278) != 0) {
      FUN_140265280(*(longlong *)(param_1 + 0x2278),param_2,0);
    }
    if ((*(longlong *)(param_1 + 0x20d0) != 0) &&
       (((*(int *)(DAT_140b2eea0 + 0x44) != 3 || (*(int *)(param_1 + 0x211c) != 1)) ||
        (lVar14 = FUN_14034c770(*(longlong *)(param_1 + 0x20d0),0), lVar14 == 0)))) {
      lVar14 = *(longlong *)(param_1 + 0x20d0);
      if (lVar14 != 0) {
        FUN_14034a770(lVar14);
        thunk_FUN_1407864b8(lVar14,200);
      }
      *(undefined8 *)(param_1 + 0x20d0) = 0;
      *(undefined8 *)(param_1 + 0x21a0) = 0;
    }
    fVar5 = DAT_14085d6a8;
    fVar4 = DAT_14085acb4;
    fVar3 = DAT_1408054e8;
    fVar2 = DAT_140804fb8;
    fVar1 = DAT_1407ffaf8;
    if (DAT_140b15418 != 0) {
      iVar8 = *(int *)(DAT_140b15418 + 0x10);
    }
    plVar12 = (longlong *)**(longlong **)(param_1 + 11000);
    if (plVar12 != *(longlong **)(param_1 + 11000)) {
      do {
        plVar13 = (longlong *)plVar12[9];
        if (plVar13 == (longlong *)0x0) {
LAB_140305b3f:
          plVar13 = (longlong *)*plVar12;
          *(longlong **)plVar12[1] = plVar13;
          *(longlong *)(*plVar12 + 8) = plVar12[1];
          *(longlong *)(param_1 + 0x2b00) = *(longlong *)(param_1 + 0x2b00) + -1;
          FID_conflict__Tidy_deallocate(plVar12 + 2);
          thunk_FUN_1407864b8(plVar12,0x50);
        }
        else {
          iVar9 = iVar8 - *(int *)((longlong)plVar12 + 0x44);
          if (0x3c < iVar9) {
            FUN_14034af40();
            thunk_FUN_1407864b8(plVar13,0x7e8);
            plVar12[9] = 0;
            goto LAB_140305b3f;
          }
          *(undefined4 *)((longlong)plVar12 + 0x3c) = 0;
          iVar10 = iVar9 + -0x1e;
          if (iVar9 + -0x1e < 1) {
            iVar10 = 0;
          }
          *(float *)(plVar12 + 8) = ((float)iVar9 / fVar2) * fVar5;
          (**(code **)(*plVar13 + 0x20))(plVar13,(int)((fVar1 - (float)iVar10 / fVar4) * fVar3));
          plVar13 = (longlong *)*plVar12;
        }
        plVar12 = plVar13;
      } while (plVar13 != *(longlong **)(param_1 + 11000));
    }
  }
  return;
}



