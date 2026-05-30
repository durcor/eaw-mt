/* MovementCoordinatorClass::vfunc_8  RVA 0x4f7360  size 450  params 0  ret undefined */


void MovementCoordinatorClass::vfunc_8(longlong param_1)

{
  longlong lVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong *plVar4;
  longlong lVar5;
  uint uVar6;
  ulonglong uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  if (((*(longlong *)(param_1 + 0x30) - *(longlong *)(param_1 + 0x28) & 0xfffffffffffffff8U) != 0)
     && (lVar3 = FUN_1404f6fb0(), lVar3 != 0)) {
    lVar5 = *(longlong *)(param_1 + 0x30);
    plVar4 = *(longlong **)(param_1 + 0x28);
    uVar6 = 0;
    fVar9 = 0.0;
    fVar10 = 0.0;
    fVar11 = 0.0;
    if (lVar5 - (longlong)plVar4 >> 3 != 0) {
      uVar7 = 0;
      do {
        FUN_14040e0f0(lVar3,plVar4[uVar7],1);
        plVar4 = *(longlong **)(param_1 + 0x28);
        uVar6 = uVar6 + 1;
        lVar5 = *(longlong *)(param_1 + 0x30);
        lVar1 = plVar4[uVar7];
        uVar7 = (ulonglong)uVar6;
        fVar9 = fVar9 + *(float *)(lVar1 + 0x78);
        fVar11 = fVar11 + *(float *)(lVar1 + 0x7c);
        fVar10 = fVar10 + *(float *)(lVar1 + 0x80);
      } while (uVar7 < (ulonglong)(lVar5 - (longlong)plVar4 >> 3));
    }
    FUN_14027d370(0x10,"Issue_Degenerate_Move, FID: %u, FOID: %d, OC: %d\n",
                  *(undefined4 *)(lVar3 + 0x288),*(undefined4 *)(*plVar4 + 0x50),
                  (int)(lVar5 - (longlong)plVar4 >> 3));
    *(undefined4 *)(param_1 + 100) = 1;
    fVar8 = DAT_1407ffaf8 /
            (float)(ulonglong)(*(longlong *)(param_1 + 0x30) - *(longlong *)(param_1 + 0x28) >> 3);
    plVar4 = (longlong *)**(undefined8 **)(param_1 + 0x28);
    lVar5 = *plVar4;
    *(float *)(param_1 + 0x44) = fVar11 * fVar8;
    *(float *)(param_1 + 0x48) = fVar10 * fVar8;
    *(float *)(param_1 + 0x40) = fVar9 * fVar8;
    plVar4 = (longlong *)(**(code **)(lVar5 + 0x10))(plVar4,1);
    if (plVar4 != (longlong *)0x0) {
      uVar2 = (**(code **)(*plVar4 + 0x238))(plVar4);
      FUN_1404241a0(lVar3,uVar2);
    }
    FUN_140423c10(lVar3,param_1 + 0x40,0);
    *(undefined4 *)(param_1 + 0xa8) = 0;
    FUN_140419ee0(lVar3);
  }
  return;
}


