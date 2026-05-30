/* GameObjectClass::FUN_1403893e0  RVA 0x3893e0  size 3168  params 0  ret undefined */


void GameObjectClass::FUN_1403893e0(undefined8 *param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined4 uVar4;
  int iVar5;
  longlong lVar6;
  HANDLE hHeap;
  longlong lVar7;
  ulonglong uVar8;
  uint uVar9;
  undefined8 *puVar10;
  longlong *plVar11;
  ulonglong uVar12;
  
  *param_1 = vftable;
  param_1[1] = vftable;
  param_1[3] = vftable;
  param_1[5] = vftable;
  param_1[7] = vftable;
  if ((*(char *)((longlong)param_1 + 0x33a) == -1) && (*(char *)((longlong)param_1 + 0x33e) == -1))
  {
    lVar6 = FUN_140294bc0(&DAT_140a16fd0,*(undefined4 *)(param_1 + 0xb));
    if ((lVar6 != 0) &&
       (((lVar6 = *(longlong *)(lVar6 + 0x360), lVar6 != 0 && (param_1[0x57] != 0)) &&
        (plVar11 = *(longlong **)(param_1[0x57] + 0x20), plVar11 != (longlong *)0x0)))) {
      uVar4 = (**(code **)(*plVar11 + 0xe0))();
      lVar6 = FUN_1404b06d0(lVar6,uVar4);
      if (((lVar6 != 0) && (*(longlong *)(lVar6 + 0x50) != 0)) &&
         (plVar11 = *(longlong **)(*(longlong *)(lVar6 + 0x50) + 0x38), plVar11 != (longlong *)0x0))
      {
        (**(code **)(*plVar11 + 0xa0))(plVar11,param_1);
      }
    }
  }
  if (*(char *)((longlong)param_1 + 0x36b) != -1) {
    FUN_1405031a0(param_1);
  }
  uVar8 = 0;
  if (param_1[0x53] != 0) {
    do {
      iVar5 = FUN_140374b50(param_1[0x53],uVar8);
      if (iVar5 == 0x20) {
        FUN_1404d07e0(param_1);
        break;
      }
      uVar9 = (int)uVar8 + 1;
      uVar8 = (ulonglong)uVar9;
    } while ((int)uVar9 < 2);
    if ((0.0 < *(float *)(param_1[0x53] + 0x23e8)) ||
       ((param_1[0x20] != 0 && (0 < *(int *)(param_1[0x20] + 0x378))))) {
      FUN_1404d07e0(param_1);
    }
  }
  FUN_1403a52f0(param_1);
  if (DAT_140b15418 != 0) {
    iVar5 = (**(code **)(**(longlong **)(param_1[0x57] + 0x20) + 0xe0))();
    if ((((iVar5 == 1) && (*(longlong *)(param_1[0x57] + 0x20) != 0)) &&
        (*(longlong *)(*(longlong *)(param_1[0x57] + 0x20) + 0x338) != 0)) && (param_1[0x54] != 0))
    {
      FUN_140264f00();
    }
    FUN_1403a5840(param_1);
  }
  if ((longlong *)param_1[0x54] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x54] + 8))();
    param_1[0x54] = 0;
  }
  lVar6 = param_1[0x55];
  if (lVar6 != 0) {
    (**(code **)(*(longlong *)(lVar6 + 0x40) + 8))(lVar6 + 0x40);
    param_1[0x55] = 0;
  }
  param_1[0x56] = 0;
  if ((param_1[0x35] != 0) && (lVar6 = *(longlong *)(param_1[0x35] + 0x30), lVar6 != 0)) {
    FUN_140391310(lVar6,1);
    *(undefined8 *)(param_1[0x35] + 0x30) = 0;
  }
  puVar10 = (undefined8 *)param_1[0x5a];
  if (puVar10 == (undefined8 *)0x0) goto LAB_140389693;
  if (*(int *)(puVar10 + 2) < 1) {
LAB_14038967f:
    (**(code **)*puVar10)(puVar10,1);
  }
  else {
    do {
      puVar2 = *(undefined8 **)puVar10[1];
      if (puVar2 != (undefined8 *)0x0) {
        (**(code **)*puVar2)(puVar2,1);
        puVar10 = (undefined8 *)param_1[0x5a];
      }
      if (0 < *(int *)(puVar10 + 2)) {
        uVar9 = *(int *)(puVar10 + 2) - 1;
        if (uVar9 != 0) {
          if ((*(uint *)((longlong)puVar10 + 0x14) & 0x80000000) == 0) {
            if (0 < (int)uVar9) {
              uVar12 = (ulonglong)uVar9;
              uVar8 = 0;
              do {
                uVar8 = uVar8 + 8;
                *(undefined8 *)(puVar10[1] + -8 + uVar8) = *(undefined8 *)(puVar10[1] + uVar8);
                uVar12 = uVar12 - 1;
              } while (uVar12 != 0);
            }
          }
          else {
            FUN_14076bd80(puVar10[1],puVar10[1] + 8,(longlong)(int)uVar9 << 3);
          }
        }
        *(int *)(puVar10 + 2) = *(int *)(puVar10 + 2) + -1;
      }
      puVar10 = (undefined8 *)param_1[0x5a];
    } while (0 < *(int *)(puVar10 + 2));
    if (puVar10 != (undefined8 *)0x0) goto LAB_14038967f;
  }
  param_1[0x5a] = 0;
LAB_140389693:
  if (param_1[0x5b] != 0) {
    FUN_1402488e0();
    if ((longlong *)param_1[0x5b] != (longlong *)0x0) {
      (**(code **)(*(longlong *)param_1[0x5b] + 8))();
      param_1[0x5b] = 0;
    }
  }
  lVar6 = param_1[0x15];
  if (lVar6 != 0) {
    FUN_14038a050(lVar6);
    thunk_FUN_1407864b8(lVar6,0x2c0);
    param_1[0x15] = 0;
  }
  lVar6 = param_1[0x16];
  if (lVar6 != 0) {
    FUN_14055be70(lVar6);
    thunk_FUN_1407864b8(lVar6,0x68);
    param_1[0x16] = 0;
  }
  lVar6 = param_1[0x17];
  if (lVar6 != 0) {
    FUN_1404b5fb0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x358);
    param_1[0x17] = 0;
  }
  lVar6 = param_1[0x18];
  if (lVar6 != 0) {
    FUN_140552970(lVar6);
    thunk_FUN_1407864b8(lVar6,0x98);
    param_1[0x18] = 0;
  }
  lVar6 = param_1[0x19];
  if (lVar6 != 0) {
    FUN_1404ed540(lVar6);
    thunk_FUN_1407864b8(lVar6,0xd0);
    param_1[0x19] = 0;
  }
  lVar6 = param_1[0x1a];
  if (lVar6 != 0) {
    FUN_1405597e0(lVar6);
    thunk_FUN_1407864b8(lVar6,200);
    param_1[0x1a] = 0;
  }
  lVar6 = param_1[0x1b];
  if (lVar6 != 0) {
    FUN_1405532b0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x60);
    param_1[0x1b] = 0;
  }
  lVar6 = param_1[0x1c];
  if (lVar6 != 0) {
    FUN_140560960(lVar6);
    thunk_FUN_1407864b8(lVar6,0x28);
    param_1[0x1c] = 0;
  }
  lVar6 = param_1[0x23];
  if (lVar6 != 0) {
    FUN_14055c590(lVar6);
    thunk_FUN_1407864b8(lVar6,0x68);
    param_1[0x23] = 0;
  }
  lVar6 = param_1[0x24];
  if (lVar6 != 0) {
    FUN_14055c110(lVar6);
    thunk_FUN_1407864b8(lVar6,0x40);
    param_1[0x24] = 0;
  }
  lVar6 = param_1[0x25];
  if (lVar6 != 0) {
    FUN_140551b50(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x25] = 0;
  }
  lVar6 = param_1[0x26];
  if (lVar6 != 0) {
    FUN_140552670(lVar6);
    thunk_FUN_1407864b8(lVar6,0x18);
    param_1[0x26] = 0;
  }
  lVar6 = param_1[0x2a];
  if (lVar6 != 0) {
    FUN_1405612a0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x30);
    param_1[0x2a] = 0;
  }
  lVar6 = param_1[0x27];
  if (lVar6 != 0) {
    FUN_140561800(lVar6);
    thunk_FUN_1407864b8(lVar6,0x20);
    param_1[0x27] = 0;
  }
  lVar6 = param_1[0x28];
  if (lVar6 != 0) {
    FUN_140561670(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x28] = 0;
  }
  lVar6 = param_1[0x29];
  if (lVar6 != 0) {
    FUN_1405622e0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x58);
    param_1[0x29] = 0;
  }
  lVar6 = param_1[0x1d];
  if (lVar6 != 0) {
    FUN_14055b570(lVar6);
    thunk_FUN_1407864b8(lVar6,0xb8);
    param_1[0x1d] = 0;
  }
  lVar6 = param_1[0x1e];
  if (lVar6 != 0) {
    FUN_140380750(lVar6);
    thunk_FUN_1407864b8(lVar6,0x1a0);
    param_1[0x1e] = 0;
  }
  lVar6 = param_1[0x1f];
  if (lVar6 != 0) {
    (*(code *)**(undefined8 **)(lVar6 + 8))(lVar6 + 8,1);
    param_1[0x1f] = 0;
  }
  lVar6 = param_1[0x20];
  if (lVar6 != 0) {
    FUN_14055da50(lVar6);
    thunk_FUN_1407864b8(lVar6,0x3b8);
    param_1[0x20] = 0;
  }
  lVar6 = param_1[0x21];
  if (lVar6 != 0) {
    FUN_140551090(lVar6);
    thunk_FUN_1407864b8(lVar6,0x98);
    param_1[0x21] = 0;
  }
  lVar6 = param_1[0x22];
  if (lVar6 != 0) {
    FUN_1404ec7f0(lVar6);
    thunk_FUN_1407864b8(lVar6,200);
    param_1[0x22] = 0;
  }
  lVar6 = param_1[0x2b];
  if (lVar6 != 0) {
    FUN_140491460(lVar6);
    thunk_FUN_1407864b8(lVar6,0x50);
    param_1[0x2b] = 0;
  }
  lVar6 = param_1[0x2c];
  if (lVar6 != 0) {
    FUN_140561a80(lVar6);
    thunk_FUN_1407864b8(lVar6,0x58);
    param_1[0x2c] = 0;
  }
  lVar6 = param_1[0x2d];
  if (lVar6 != 0) {
    FUN_140563790(lVar6);
    thunk_FUN_1407864b8(lVar6,0x58);
    param_1[0x2d] = 0;
  }
  lVar6 = param_1[0x30];
  if (lVar6 != 0) {
    FUN_140562040(lVar6);
    thunk_FUN_1407864b8(lVar6,0x18);
    param_1[0x30] = 0;
  }
  lVar6 = param_1[0x31];
  if (lVar6 != 0) {
    FUN_140552870(lVar6);
    thunk_FUN_1407864b8(lVar6,0x50);
    param_1[0x31] = 0;
  }
  lVar6 = param_1[0x32];
  if (lVar6 != 0) {
    FUN_140551cb0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x18);
    param_1[0x32] = 0;
  }
  lVar6 = param_1[0x33];
  if (lVar6 != 0) {
    FUN_140551dd0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x33] = 0;
  }
  lVar6 = param_1[0x3c];
  if (lVar6 != 0) {
    FUN_140562960(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x3c] = 0;
  }
  puVar10 = (undefined8 *)param_1[0x34];
  if (puVar10 != (undefined8 *)0x0) {
    *puVar10 = DamageTrackingDataPackClass::vftable;
    lVar6 = puVar10[5];
    uVar8 = puVar10[6] - lVar6 & 0xfffffffffffffff8;
    lVar7 = lVar6;
    if (0xfff < uVar8) {
      lVar7 = *(longlong *)(lVar6 + -8);
      if (0x1f < (lVar6 - lVar7) - 8U) {
                    /* WARNING: Subroutine does not return */
        FUN_1407711cc(lVar6 - lVar7,uVar8 + 0x27);
      }
    }
    thunk_FUN_1407864b8(lVar7);
    puVar10[5] = 0;
    puVar10[6] = 0;
    puVar10[7] = 0;
    puVar2 = (undefined8 *)puVar10[3];
    *(undefined8 *)puVar2[1] = 0;
    puVar2 = (undefined8 *)*puVar2;
    while (puVar2 != (undefined8 *)0x0) {
      puVar3 = (undefined8 *)*puVar2;
      thunk_FUN_1407864b8(puVar2,0x18);
      puVar2 = puVar3;
    }
    thunk_FUN_1407864b8(puVar10[3],0x18);
    thunk_FUN_1407864b8(puVar10,0x58);
    param_1[0x34] = 0;
  }
  lVar6 = param_1[0x35];
  if ((lVar6 != 0) && (DAT_140b2c37b == '\0')) {
    FUN_1404ee450(lVar6);
    thunk_FUN_1407864b8(lVar6,0x40);
    param_1[0x35] = 0;
  }
  lVar6 = param_1[0x36];
  if (lVar6 != 0) {
    FUN_140564f00(lVar6);
    thunk_FUN_1407864b8(lVar6,0x38);
    param_1[0x36] = 0;
  }
  lVar6 = param_1[0x37];
  if (lVar6 != 0) {
    FUN_1405628d0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x28);
    param_1[0x37] = 0;
  }
  lVar6 = param_1[0x38];
  if (lVar6 != 0) {
    FUN_1404ee270(lVar6);
    thunk_FUN_1407864b8(lVar6,0x28);
    param_1[0x38] = 0;
  }
  lVar6 = param_1[0x39];
  if (lVar6 != 0) {
    FUN_1405635a0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x18);
    param_1[0x39] = 0;
  }
  lVar6 = param_1[0x3a];
  if (lVar6 != 0) {
    FUN_14058c000(lVar6);
    thunk_FUN_1407864b8(lVar6,0x48);
    param_1[0x3a] = 0;
  }
  lVar6 = param_1[0x3b];
  if (lVar6 != 0) {
    FUN_14058c480(lVar6);
    thunk_FUN_1407864b8(lVar6,0x20);
    param_1[0x3b] = 0;
  }
  lVar6 = param_1[0x3d];
  if (lVar6 != 0) {
    FUN_1405629b0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x2c8);
    param_1[0x3d] = 0;
  }
  lVar6 = param_1[0x2e];
  if (lVar6 != 0) {
    FUN_14053e110(lVar6);
    thunk_FUN_1407864b8(lVar6,0x20);
    param_1[0x2e] = 0;
  }
  lVar6 = param_1[0x2f];
  if (lVar6 != 0) {
    FUN_140562ed0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x2f] = 0;
  }
  lVar6 = param_1[0x3e];
  if (lVar6 != 0) {
    FUN_1405630a0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x60);
    param_1[0x3e] = 0;
  }
  lVar6 = param_1[0x3f];
  if (lVar6 != 0) {
    FUN_140565cb0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x10);
    param_1[0x3f] = 0;
  }
  lVar6 = param_1[0x40];
  if (lVar6 != 0) {
    FUN_1405329b0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x20);
    param_1[0x40] = 0;
  }
  plVar11 = (longlong *)param_1[0x41];
  if (plVar11 != (longlong *)0x0) {
    (**(code **)(*plVar11 + 0x10))(plVar11,1);
    param_1[0x41] = 0;
  }
  lVar6 = param_1[0x42];
  if (lVar6 != 0) {
    FUN_140563b80(lVar6);
    thunk_FUN_1407864b8(lVar6,0x38);
    param_1[0x42] = 0;
  }
  lVar6 = param_1[0x43];
  if (lVar6 != 0) {
    FUN_140563ed0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x38);
    param_1[0x43] = 0;
  }
  lVar6 = param_1[0x44];
  if (lVar6 != 0) {
    FUN_140564270(lVar6);
    thunk_FUN_1407864b8(lVar6,0x28);
    param_1[0x44] = 0;
  }
  lVar6 = param_1[0x45];
  if (lVar6 != 0) {
    FUN_1405645f0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x40);
    param_1[0x45] = 0;
  }
  lVar6 = param_1[0x46];
  if (lVar6 != 0) {
    FUN_140564a10(lVar6);
    thunk_FUN_1407864b8(lVar6,0x18);
    param_1[0x46] = 0;
  }
  lVar6 = param_1[0x48];
  if (lVar6 != 0) {
    FUN_140564cb0(lVar6);
    thunk_FUN_1407864b8(lVar6,0x60);
    param_1[0x48] = 0;
  }
  lVar6 = 0x4d;
  plVar11 = param_1 + 0x114;
  do {
    plVar1 = plVar11 + -2;
    lVar6 = lVar6 + -1;
    *(undefined1 *)(plVar11 + -1) = 0;
    lVar7 = *plVar1;
    if (lVar7 != 0) {
      (**(code **)(*(longlong *)(lVar7 + 8) + 8))(lVar7 + 8);
      *plVar1 = 0;
    }
    plVar11 = plVar1;
  } while (lVar6 != 0);
  if ((longlong *)param_1[0x5b] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x5b] + 8))();
  }
  lVar6 = param_1[0x55];
  if (lVar6 != 0) {
    (**(code **)(*(longlong *)(lVar6 + 0x40) + 8))(lVar6 + 0x40);
  }
  if ((longlong *)param_1[0x54] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x54] + 8))();
  }
  if ((longlong *)param_1[0x53] != (longlong *)0x0) {
    (**(code **)(*(longlong *)param_1[0x53] + 8))();
  }
  puVar10 = (undefined8 *)param_1[0x4f];
  if (puVar10 != (undefined8 *)0x0) {
    if (puVar10 != param_1 + 0x50) {
      hHeap = GetProcessHeap();
      HeapFree(hHeap,0,puVar10);
      param_1[0x4f] = param_1 + 0x50;
    }
    *(undefined2 *)(param_1 + 0x51) = 0x100;
  }
  FUN_140389330(param_1 + 0xc);
  FUN_140240640(param_1 + 7);
  param_1[5] = CullObjectClass::vftable;
  FUN_14020a820(param_1 + 3);
  param_1[1] = RefCountClass::vftable;
  return;
}


