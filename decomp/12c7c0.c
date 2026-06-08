/* alHModel::vfunc_15  RVA 0x12c7c0  size 497  params 0  ret undefined */


void alHModel::vfunc_15(longlong param_1)

{
  longlong *plVar1;
  undefined8 uVar2;
  undefined *puVar3;
  ulonglong uVar4;
  longlong lVar5;
  uint uVar6;
  int iVar7;
  
  if (*(longlong **)(param_1 + 0xb0) != (longlong *)0x0) {
    (**(code **)(**(longlong **)(param_1 + 0xb0) + 0x78))();
  }
  if ((*(byte *)(param_1 + 0x30) & 0x20) != 0) {
    *(byte *)(param_1 + 0x30) = *(byte *)(param_1 + 0x30) & 0xdf;
    uVar2 = FUN_14013fe60(param_1);
    iVar7 = *(int *)(param_1 + 0x170);
    if (iVar7 == 0) {
      FUN_140146310(*(undefined8 *)(param_1 + 0xe8),uVar2,param_1 + 0x38);
    }
    else if (iVar7 == 1) {
      FUN_1401467b0(*(undefined8 *)(param_1 + 0xe8),uVar2,param_1 + 0x38,
                    *(undefined8 *)(param_1 + 0x178),*(undefined4 *)(param_1 + 0x180));
    }
    else if (iVar7 == 2) {
      FUN_140146d50(*(undefined8 *)(param_1 + 0xe8),uVar2,param_1 + 0x38,
                    *(undefined8 *)(param_1 + 0x178),*(undefined4 *)(param_1 + 0x180),
                    *(undefined8 *)(param_1 + 0x188),*(undefined4 *)(param_1 + 400),
                    *(undefined4 *)(param_1 + 0x194));
    }
    lVar5 = *(longlong *)(param_1 + 0xf0);
    uVar6 = 0;
    if (*(longlong *)(param_1 + 0xf8) - lVar5 >> 4 != 0) {
      uVar4 = 0;
      do {
        plVar1 = *(longlong **)(lVar5 + 8 + uVar4 * 0x10);
        iVar7 = *(int *)(lVar5 + uVar4 * 0x10);
        (**(code **)(*plVar1 + 0x58))
                  (plVar1,(longlong)iVar7 * 0x30 +
                          *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x28));
        if (iVar7 == 0) {
          for (puVar3 = (undefined *)(**(code **)(*plVar1 + 0x20))(plVar1);
              puVar3 != (undefined *)0x0; puVar3 = *(undefined **)(puVar3 + 8)) {
            if (puVar3 == &DAT_140a12540) {
              if ((plVar1 != (longlong *)0x0) && ((*(byte *)((longlong)plVar1 + 0x31) & 8) != 0)) {
                iVar7 = (int)plVar1[0x23];
              }
              break;
            }
          }
        }
        uVar6 = uVar6 + 1;
        uVar4 = (ulonglong)uVar6;
        *(byte *)(plVar1 + 6) =
             *(byte *)(plVar1 + 6) ^
             (~(*(byte *)((longlong)iVar7 + *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x10)) >>
               1) ^ *(byte *)(plVar1 + 6)) & 2;
        lVar5 = *(longlong *)(param_1 + 0xf0);
      } while (uVar4 < (ulonglong)(*(longlong *)(param_1 + 0xf8) - lVar5 >> 4));
    }
  }
  return;
}


