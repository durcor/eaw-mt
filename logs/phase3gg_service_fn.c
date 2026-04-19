
// ================================================================
// REFERENCES TO DEBUG STRINGS
// ================================================================

// String ::Service at RVA 0x874358:
//   Ref from fn RVA 0x488660  FUN_140488660  555 bytes
//   Ref from fn RVA 0x488660  FUN_140488660  555 bytes
// String ::Attach_Lua_Script at RVA 0x874398:
//   Ref from fn RVA 0x4854e0  FUN_1404854e0  536 bytes
// String ::Reset at RVA 0x874450:
//   Ref from fn RVA 0x487810  FUN_140487810  84 bytes
// String ::Get2 at RVA 0x8743d8:
//   Ref from fn RVA 0x485700  FUN_140485700  89 bytes


// ================================================================
// Function referencing ::Service string: RVA 0x488660
// ================================================================

// ------------------------------------------------------------
// RVA: 0x488660  Name: FUN_140488660  Size: 555 bytes
// ------------------------------------------------------------

void FUN_140488660(longlong param_1)

{
  longlong *plVar1;
  undefined *puVar2;
  ulonglong uVar3;
  longlong *plVar4;
  longlong *plVar5;
  float fVar6;
  double dVar7;
  
  if (*(longlong *)(param_1 + 0x10) == 0) {
    return;
  }
  FUN_14027d370(8,&DAT_140874300,"TheGameScoringManagerClass::Service");
  plVar1 = (longlong *)FUN_140246fb0(*(undefined8 *)(param_1 + 0x10),"ServiceRate",0);
  if (plVar1 == (longlong *)0x0) {
LAB_1404886ff:
    plVar5 = (longlong *)0x0;
    if (plVar1 != (longlong *)0x0) goto LAB_140488706;
  }
  else {
    (**(code **)*plVar1)(plVar1);
    puVar2 = (undefined *)(**(code **)(*plVar1 + 0x20))(plVar1);
    if (puVar2 == (undefined *)0x0) goto LAB_1404886ff;
    do {
      if (puVar2 == &DAT_140a157c0) {
        (**(code **)*plVar1)(plVar1);
        plVar5 = plVar1;
        goto LAB_140488706;
      }
      puVar2 = *(undefined **)(puVar2 + 8);
    } while (puVar2 != (undefined *)0x0);
    plVar5 = (longlong *)0x0;
LAB_140488706:
    (**(code **)(*plVar1 + 8))(plVar1);
  }
  plVar1 = (longlong *)FUN_140246fb0(*(undefined8 *)(param_1 + 0x10),"LastService",0);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)*plVar1)(plVar1);
    for (puVar2 = (undefined *)(**(code **)(*plVar1 + 0x20))(plVar1); puVar2 != (undefined *)0x0;
        puVar2 = *(undefined **)(puVar2 + 8)) {
      if (puVar2 == &DAT_140a157c0) {
        (**(code **)*plVar1)(plVar1);
        plVar4 = plVar1;
        goto LAB_14048875c;
      }
    }
  }
  plVar4 = (longlong *)0x0;
  if (plVar1 != (longlong *)0x0) {
LAB_14048875c:
    (**(code **)(*plVar1 + 8))(plVar1);
  }
  uVar3 = (ulonglong)DAT_140b0a320;
  fVar6 = (float)DAT_140b0a340;
  if (plVar5 == (longlong *)0x0) {
LAB_140488855:
    if (plVar4 != (longlong *)0x0) goto LAB_14048885a;
  }
  else {
    if ((plVar4 == (longlong *)0x0) ||
       (dVar7 = (double)((float)uVar3 / fVar6) - (double)plVar4[2],
       (double)plVar5[2] <= dVar7 && dVar7 != (double)plVar5[2])) {
      FUN_14027d370(8,"%s -- Pump_Threads\n","TheGameScoringManagerClass::Service");
      FUN_140247a90(*(undefined8 *)(param_1 + 0x10));
      plVar1 = *(longlong **)(param_1 + 0x10);
      if (*(char *)((longlong)plVar1 + 0x121) != '\0') {
        if (plVar1 != (longlong *)0x0) {
          (**(code **)(*plVar1 + 8))();
          *(undefined8 *)(param_1 + 0x10) = 0;
        }
        if (plVar4 != (longlong *)0x0) {
          (**(code **)(*plVar4 + 8))(plVar4);
        }
        goto LAB_140488868;
      }
      if ((plVar4 == (longlong *)0x0) &&
         (plVar1 = (longlong *)FUN_140769c58(0x18), plVar1 != (longlong *)0x0)) {
        *(undefined4 *)(plVar1 + 1) = 0;
        *plVar1 = (longlong)LuaValue<double,4>::vftable;
        (*(code *)LuaValue<double,4>::vftable[0])(plVar1);
        plVar4 = plVar1;
      }
      plVar4[2] = (longlong)(double)((float)uVar3 / fVar6);
      FUN_140247000(*(undefined8 *)(param_1 + 0x10),plVar4,"LastService");
      goto LAB_140488855;
    }
LAB_14048885a:
    (**(code **)(*plVar4 + 8))(plVar4);
  }
  if (plVar5 == (longlong *)0x0) {
    return;
  }
LAB_140488868:
  (**(code **)(*plVar5 + 8))(plVar5);
  return;
}




// ================================================================
// SYMBOL TABLE: TheGameScoringManagerClass vtable entries
// ================================================================


// ================================================================
// VTABLE SEARCH VIA RTTI: scanning .rdata for fn addresses
// ================================================================

// Scanning FUN_140485340 for data references (vtable addr):
//   DataRef to RVA 0x8742b8  (vftable)
//   vtable[0] unreadable
//   vtable[1] RVA 0x7069483ec1000000  ?  0b
//   vtable[2] RVA 0x70dc173ec1000000  ?  0b
//   vtable[3] RVA 0x54686546216d6553  ?  0b
//   vtable[4] RVA 0x636f72682e674d61  ?  0b
//   vtable[5] RVA 0x6e61676432436c61  ?  0b
//   DataRef to RVA 0x8742b8  (vftable)
//   vtable[0] unreadable
//   vtable[1] RVA 0x7069483ec1000000  ?  0b
//   vtable[2] RVA 0x70dc173ec1000000  ?  0b
//   vtable[3] RVA 0x54686546216d6553  ?  0b
//   vtable[4] RVA 0x636f72682e674d61  ?  0b
//   vtable[5] RVA 0x6e61676432436c61  ?  0b
