/* FUN_14035dec0  RVA 0x35dec0  size 1133  params 0  ret undefined */


undefined4 FUN_14035dec0(undefined8 param_1,longlong param_2,longlong param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  undefined4 local_res10;
  undefined4 local_res14;
  undefined1 local_res20 [8];
  undefined1 local_58 [64];
  
  if (param_2 == 0) {
    return 0;
  }
  FUN_14035b2f0(param_3,param_2 + 0x78);
  if (*(char *)(*(longlong *)(param_2 + 0x298) + 0x87) == '\x01') {
    if (*(int *)(param_3 + 0x14) * 2 < 0x10) {
      FUN_14019f6a0(param_3,8,0,1);
    }
    FUN_14038de80(param_2,local_58);
    FUN_14020d1c0(local_58,&local_res10);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar6 / 10 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = local_res10;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = local_res14;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d210(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d260(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d2b0(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d310(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d3b0(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d340(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    local_res10 = uVar1;
    local_res14 = uVar2;
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
    puVar5 = (undefined4 *)FUN_14020d380(local_58,local_res20);
    iVar7 = *(int *)(param_3 + 0x10);
    iVar6 = *(int *)(param_3 + 0x14) * 2 >> 1;
    uVar1 = *puVar5;
    uVar2 = puVar5[1];
    if (iVar6 <= iVar7) {
      iVar7 = iVar6 / 10;
      if (iVar7 < 10) {
        iVar7 = 10;
      }
      local_res10 = uVar1;
      local_res14 = uVar2;
      FUN_14019f6a0(param_3,iVar7 + iVar6,0,1);
      iVar7 = *(int *)(param_3 + 0x10);
    }
    lVar3 = *(longlong *)(param_3 + 8);
    lVar4 = (longlong)iVar7;
    *(int *)(param_3 + 0x10) = iVar7 + 1;
    *(undefined4 *)(lVar3 + lVar4 * 0xc) = uVar1;
    *(undefined4 *)(lVar3 + 4 + lVar4 * 0xc) = uVar2;
    *(undefined4 *)(lVar3 + 8 + lVar4 * 0xc) = 0;
  }
  return *(undefined4 *)(param_3 + 0x10);
}


