
// ================================================================
// FUN_1401f92b0 — sub-object destructor A (GameObjectClass+8)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1f92b0  Name: FUN_1401f92b0  Size: 9 bytes
// ------------------------------------------------------------

void FUN_1401f92b0(longlong param_1)

{
  *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
  return;
}




// ================================================================
// FUN_1401f92c0 — sub-object destructor B (GameObjectClass+8)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1f92c0  Name: FUN_1401f92c0  Size: 26 bytes
// ------------------------------------------------------------

undefined4 FUN_1401f92c0(longlong *param_1)

{
  longlong *plVar1;
  undefined4 uVar2;
  
  plVar1 = param_1 + 1;
  *(int *)plVar1 = (int)*plVar1 + -1;
  uVar2 = (undefined4)param_1[1];
  if ((int)*plVar1 == 0) {
    (**(code **)(*param_1 + 0x18))();
    uVar2 = 0;
  }
  return uVar2;
}




// ================================================================
// FUN_140220e90 — signal register/connect (SetupPhaseManager subscribes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x220e90  Name: FUN_140220e90  Size: 25 bytes
// ------------------------------------------------------------

void FUN_140220e90(undefined8 param_1,longlong param_2,longlong param_3,undefined4 param_4)

{
  if ((param_2 != 0) && (param_3 != 0)) {
    FUN_1402406c0(param_2,param_4);
    return;
  }
  return;
}




// ================================================================
// FUN_1402acac0 — add_object (registers object into GOM, likely subscribes)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2aca60  Name: FUN_1402aca60  Size: 169 bytes
// ------------------------------------------------------------

undefined8 FUN_1402aca60(longlong param_1,uint param_2)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  int iVar4;
  longlong *plVar5;
  uint uVar6;
  
  uVar6 = (param_2 ^ 0xdeadbeef) & 0x7fffffff;
  lVar1 = *(longlong *)(param_1 + 0x88);
  iVar4 = (uVar6 % 0x1f31d) * 0x41a7 + (uVar6 / 0x1f31d) * -0xb14;
  iVar2 = iVar4 + 0x7fffffff;
  if (-1 < iVar4) {
    iVar2 = iVar4;
  }
  plVar5 = (longlong *)
           (((longlong)iVar2 & *(ulonglong *)(param_1 + 0xb0)) * 0x10 +
           *(longlong *)(param_1 + 0x98));
  lVar3 = plVar5[1];
  if (lVar3 != lVar1) {
    iVar2 = *(int *)(lVar3 + 0x10);
    iVar4 = iVar2 - param_2;
    if ((int)param_2 < iVar2) {
      do {
        if (lVar3 == *plVar5) {
          return 0;
        }
        lVar3 = *(longlong *)(lVar3 + 8);
      } while ((int)param_2 < *(int *)(lVar3 + 0x10));
      iVar2 = *(int *)(lVar3 + 0x10);
      iVar4 = iVar2 - param_2;
    }
    if (SBORROW4(iVar2,param_2) == iVar4 < 0) {
      if (lVar3 == 0) {
        lVar3 = lVar1;
      }
      if (lVar3 != lVar1) {
        return *(undefined8 *)(lVar3 + 0x18);
      }
    }
  }
  return 0;
}




// ================================================================
// FUN_14028a5d0 — singleton getter (for combat/signal manager)
// ================================================================

// ------------------------------------------------------------
// RVA: 0x28a5d0  Name: FUN_14028a5d0  Size: 137 bytes
// ------------------------------------------------------------

longlong FUN_14028a5d0(void)

{
  longlong lVar1;
  undefined **local_res8 [4];
  
  if (DAT_140a16d90 == 0) {
    lVar1 = FUN_140769c58(0x10);
    if (lVar1 == 0) {
      DAT_140a16d90 = 0;
    }
    else {
      DAT_140a16d90 = FUN_1404d9c80(lVar1);
    }
    local_res8[0] = &PTR_vftable_140a16d88;
    if (DAT_140a7bc10 != DAT_140a7bc18) {
      *DAT_140a7bc10 = &PTR_vftable_140a16d88;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
      return DAT_140a16d90;
    }
    FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,local_res8);
  }
  return DAT_140a16d90;
}




// ================================================================
// FUN_140058570 — singleton getter B
// ================================================================

// ------------------------------------------------------------
// RVA: 0x58570  Name: FUN_140058570  Size: 137 bytes
// ------------------------------------------------------------

longlong FUN_140058570(void)

{
  longlong lVar1;
  undefined **local_res8 [4];
  
  if (DAT_140a10b50 == 0) {
    lVar1 = FUN_140769c58(0x78);
    if (lVar1 == 0) {
      DAT_140a10b50 = 0;
    }
    else {
      DAT_140a10b50 = FUN_140221820(lVar1);
    }
    local_res8[0] = &PTR_vftable_140a10b48;
    if (DAT_140a7bc10 != DAT_140a7bc18) {
      *DAT_140a7bc10 = &PTR_vftable_140a10b48;
      DAT_140a7bc10 = DAT_140a7bc10 + 1;
      return DAT_140a10b50;
    }
    FUN_14001ded0(&DAT_140a7bc08,DAT_140a7bc10,local_res8);
  }
  return DAT_140a10b50;
}




// ================================================================
// CALLERS OF FUN_1401f92b0 (sub-obj destructor A)
// ================================================================

// Caller: RVA 0x4c3520  Name: FUN_1404c3520  Size: 240 bytes
// Caller: RVA 0x1aa10  Name: FUN_14001aa10  Size: 92 bytes
// Caller: RVA 0x1aa70  Name: FUN_14001aa70  Size: 92 bytes
// Caller: RVA 0x1aad0  Name: FUN_14001aad0  Size: 106 bytes
// Caller: RVA 0x1ab40  Name: FUN_14001ab40  Size: 92 bytes
// Caller: RVA 0x4c3610  Name: FUN_1404c3610  Size: 159 bytes
// Caller: RVA 0x52f920  Name: FUN_14052f920  Size: 275 bytes
// Caller: RVA 0x71fc10  Name: FUN_14071fc10  Size: 180 bytes
// Caller: RVA 0x66fcc0  Name: FUN_14066fcc0  Size: 2414 bytes
// Caller: RVA 0x68f270  Name: FUN_14068f270  Size: 411 bytes
// Caller: RVA 0x6af410  Name: FUN_1406af410  Size: 202 bytes
// Caller: RVA 0x68f410  Name: FUN_14068f410  Size: 411 bytes
// Caller: RVA 0x68f5b0  Name: FUN_14068f5b0  Size: 2092 bytes
// Caller: RVA 0x52f720  Name: FUN_14052f720  Size: 510 bytes
// Caller: RVA 0x6ceaa0  Name: FUN_1406ceaa0  Size: 314 bytes
// Caller: RVA 0x60ed40  Name: FUN_14060ed40  Size: 433 bytes
// Caller: RVA 0x60efb0  Name: FUN_14060efb0  Size: 212 bytes
// Caller: RVA 0x69e1b0  Name: FUN_14069e1b0  Size: 1320 bytes
// Caller: RVA 0x60e3f0  Name: FUN_14060e3f0  Size: 356 bytes
// Caller: RVA 0x5fe5d0  Name: FUN_1405fe5d0  Size: 2483 bytes
// Caller: RVA 0x69e6e0  Name: FUN_14069e6e0  Size: 217 bytes
// Caller: RVA 0x6ae6a0  Name: FUN_1406ae6a0  Size: 620 bytes
// Caller: RVA 0x5fd970  Name: FUN_1405fd970  Size: 218 bytes
// Caller: RVA 0x6adbb0  Name: FUN_1406adbb0  Size: 162 bytes
// Caller: RVA 0x1dc60  Name: FUN_14001dc60  Size: 549 bytes
// Caller: RVA 0x6bd020  Name: FUN_1406bd020  Size: 187 bytes
// Caller: RVA 0x6bd0e0  Name: FUN_1406bd0e0  Size: 187 bytes
// Caller: RVA 0x19d200  Name: FUN_14019d200  Size: 1862 bytes
// Caller: RVA 0x6ed230  Name: FUN_1406ed230  Size: 952 bytes
// Caller: RVA 0x5fd320  Name: FUN_1405fd320  Size: 183 bytes
// Caller: RVA 0x6bd300  Name: FUN_1406bd300  Size: 267 bytes
// Caller: RVA 0x57d3a0  Name: FUN_14057d3a0  Size: 179 bytes
// Caller: RVA 0x73d6b0  Name: FUN_14073d6b0  Size: 165 bytes
// Caller: RVA 0x73d760  Name: FUN_14073d760  Size: 187 bytes
// Caller: RVA 0x5fca20  Name: FUN_1405fca20  Size: 489 bytes
// Caller: RVA 0x73cae0  Name: FUN_14073cae0  Size: 241 bytes
// Caller: RVA 0x5fcda0  Name: FUN_1405fcda0  Size: 281 bytes
// Caller: RVA 0x5fcec0  Name: FUN_1405fcec0  Size: 278 bytes
// Caller: RVA 0x57c030  Name: FUN_14057c030  Size: 215 bytes
// Caller: RVA 0x57c110  Name: FUN_14057c110  Size: 215 bytes
// Caller: RVA 0x57c2e0  Name: FUN_14057c2e0  Size: 253 bytes
// Caller: RVA 0x6ec2b0  Name: FUN_1406ec2b0  Size: 341 bytes
// Caller: RVA 0x57c3e0  Name: FUN_14057c3e0  Size: 229 bytes
// Caller: RVA 0x57c4d0  Name: FUN_14057c4d0  Size: 292 bytes
// Caller: RVA 0x5fc4d0  Name: FUN_1405fc4d0  Size: 1012 bytes
// Caller: RVA 0x57c7d0  Name: FUN_14057c7d0  Size: 166 bytes
// Caller: RVA 0x57b950  Name: FUN_14057b950  Size: 215 bytes
// Caller: RVA 0x57bab0  Name: FUN_14057bab0  Size: 165 bytes
// Caller: RVA 0x57bb60  Name: FUN_14057bb60  Size: 714 bytes
// Caller: RVA 0x24bb80  Name: FUN_14024bb80  Size: 351 bytes
// Caller: RVA 0x5fbcb0  Name: FUN_1405fbcb0  Size: 856 bytes
// Caller: RVA 0x57be30  Name: FUN_14057be30  Size: 215 bytes
// Caller: RVA 0x57bf10  Name: FUN_14057bf10  Size: 287 bytes
// Caller: RVA 0x73b3f0  Name: FUN_14073b3f0  Size: 187 bytes
// Caller: RVA 0x6b550  Name: FUN_14006b550  Size: 751 bytes
// Caller: RVA 0x69b670  Name: FUN_14069b670  Size: 1095 bytes
// Caller: RVA 0x73a8d0  Name: FUN_14073a8d0  Size: 200 bytes
// Caller: RVA 0x69aa80  Name: FUN_14069aa80  Size: 275 bytes
// Caller: RVA 0x24ab30  Name: FUN_14024ab30  Size: 1839 bytes
// Caller: RVA 0x73ade0  Name: FUN_14073ade0  Size: 162 bytes
// Caller: RVA 0x69afd0  Name: FUN_14069afd0  Size: 1693 bytes
// Caller: RVA 0x5fa090  Name: FUN_1405fa090  Size: 190 bytes
// Caller: RVA 0x57a170  Name: FUN_14057a170  Size: 228 bytes
// Caller: RVA 0x57a260  Name: FUN_14057a260  Size: 459 bytes
// Caller: RVA 0x48a3e0  Name: FUN_14048a3e0  Size: 648 bytes
// Caller: RVA 0x579820  Name: FUN_140579820  Size: 347 bytes
// Caller: RVA 0x5f9890  Name: FUN_1405f9890  Size: 499 bytes
// Caller: RVA 0x579980  Name: FUN_140579980  Size: 387 bytes
// Caller: RVA 0x579bf0  Name: FUN_140579bf0  Size: 421 bytes
// Caller: RVA 0x699eb0  Name: FUN_140699eb0  Size: 349 bytes
// Caller: RVA 0x579f10  Name: FUN_140579f10  Size: 597 bytes
// Caller: RVA 0x5791f0  Name: FUN_1405791f0  Size: 234 bytes
// Caller: RVA 0x7d92c0  Name: FUN_1407d92c0  Size: 224 bytes
// Caller: RVA 0x699430  Name: FUN_140699430  Size: 1465 bytes
// Caller: RVA 0x7394f0  Name: FUN_1407394f0  Size: 294 bytes
// Caller: RVA 0x579480  Name: FUN_140579480  Size: 234 bytes
// Caller: RVA 0x6a95e0  Name: FUN_1406a95e0  Size: 1767 bytes
// Caller: RVA 0x578b60  Name: FUN_140578b60  Size: 202 bytes
// Caller: RVA 0x578c30  Name: FUN_140578c30  Size: 266 bytes
// Caller: RVA 0x578e00  Name: FUN_140578e00  Size: 848 bytes
// Caller: RVA 0x6e8370  Name: FUN_1406e8370  Size: 680 bytes
// Caller: RVA 0x578300  Name: FUN_140578300  Size: 362 bytes
// Caller: RVA 0x5784f0  Name: FUN_1405784f0  Size: 354 bytes
// Caller: RVA 0x578660  Name: FUN_140578660  Size: 387 bytes
// Caller: RVA 0x488660  Name: FUN_140488660  Size: 555 bytes
// Caller: RVA 0x2586d0  Name: FUN_1402586d0  Size: 152 bytes
// Caller: RVA 0x258770  Name: FUN_140258770  Size: 166 bytes
// Caller: RVA 0x487cd0  Name: FUN_140487cd0  Size: 482 bytes
// Caller: RVA 0x487ec0  Name: FUN_140487ec0  Size: 712 bytes
// Caller: RVA 0x227030  Name: FUN_140227030  Size: 157 bytes
// Caller: RVA 0x4e7170  Name: FUN_1404e7170  Size: 3252 bytes
// Caller: RVA 0x577230  Name: FUN_140577230  Size: 967 bytes
// Caller: RVA 0x147760  Name: FUN_140147760  Size: 782 bytes
// Caller: RVA 0x6e77e0  Name: FUN_1406e77e0  Size: 341 bytes
// Caller: RVA 0x6968d0  Name: FUN_1406968d0  Size: 180 bytes
// Caller: RVA 0x486970  Name: FUN_140486970  Size: 3349 bytes
// Caller: RVA 0x576a40  Name: FUN_140576a40  Size: 292 bytes
// Caller: RVA 0x4e6b20  Name: FUN_1404e6b20  Size: 1112 bytes
// Caller: RVA 0x3a6b80  Name: FUN_1403a6b80  Size: 2862 bytes
// Caller: RVA 0x6e6f80  Name: FUN_1406e6f80  Size: 379 bytes
// Caller: RVA 0x5460a0  Name: FUN_1405460a0  Size: 262 bytes
// Caller: RVA 0x606130  Name: FUN_140606130  Size: 400 bytes
// Caller: RVA 0x5461b0  Name: FUN_1405461b0  Size: 961 bytes
// Caller: RVA 0x6a6210  Name: FUN_1406a6210  Size: 186 bytes
// Caller: RVA 0x6063c0  Name: FUN_1406063c0  Size: 340 bytes
// Caller: RVA 0x606520  Name: FUN_140606520  Size: 259 bytes
// Caller: RVA 0x6c6720  Name: FUN_1406c6720  Size: 437 bytes
// Caller: RVA 0x726710  Name: FUN_140726710  Size: 2081 bytes
// Caller: RVA 0x605810  Name: FUN_140605810  Size: 179 bytes
// Caller: RVA 0x6058e0  Name: FUN_1406058e0  Size: 182 bytes
// Caller: RVA 0x6059a0  Name: FUN_1406059a0  Size: 183 bytes
// Caller: RVA 0x605a60  Name: FUN_140605a60  Size: 185 bytes
// Caller: RVA 0x4d5a70  Name: FUN_1404d5a70  Size: 2184 bytes
// Caller: RVA 0x145ab0  Name: FUN_140145ab0  Size: 494 bytes
// Caller: RVA 0x485b50  Name: FUN_140485b50  Size: 1735 bytes
// Caller: RVA 0x605bc0  Name: FUN_140605bc0  Size: 193 bytes
// Caller: RVA 0x245ca0  Name: FUN_140245ca0  Size: 152 bytes
// Caller: RVA 0x255c90  Name: FUN_140255c90  Size: 426 bytes
// Caller: RVA 0x605c90  Name: FUN_140605c90  Size: 184 bytes
// Caller: RVA 0x605d50  Name: FUN_140605d50  Size: 401 bytes
// Caller: RVA 0x605ef0  Name: FUN_140605ef0  Size: 434 bytes
// Caller: RVA 0x605320  Name: FUN_140605320  Size: 185 bytes
// Caller: RVA 0x6053e0  Name: FUN_1406053e0  Size: 482 bytes
// Caller: RVA 0x195430  Name: FUN_140195430  Size: 87 bytes
// Caller: RVA 0x255560  Name: FUN_140255560  Size: 175 bytes
// Caller: RVA 0x6055d0  Name: FUN_1406055d0  Size: 185 bytes
// Caller: RVA 0x255690  Name: FUN_140255690  Size: 470 bytes
// Caller: RVA 0x605690  Name: FUN_140605690  Size: 180 bytes
// Caller: RVA 0x605750  Name: FUN_140605750  Size: 179 bytes
// Caller: RVA 0x485760  Name: FUN_140485760  Size: 576 bytes
// Caller: RVA 0x2457f0  Name: FUN_1402457f0  Size: 537 bytes
// Caller: RVA 0x665780  Name: FUN_140665780  Size: 686 bytes
// Caller: RVA 0x4d48c0  Name: FUN_1404d48c0  Size: 471 bytes
// Caller: RVA 0x6a4980  Name: FUN_1406a4980  Size: 454 bytes
// Caller: RVA 0x6a4b50  Name: FUN_1406a4b50  Size: 454 bytes
// Caller: RVA 0x254cb0  Name: FUN_140254cb0  Size: 179 bytes
// Caller: RVA 0x6a4d20  Name: FUN_1406a4d20  Size: 454 bytes
// Caller: RVA 0x574fb0  Name: FUN_140574fb0  Size: 235 bytes
// Caller: RVA 0x5741c0  Name: FUN_1405741c0  Size: 227 bytes
// Caller: RVA 0x6a44e0  Name: FUN_1406a44e0  Size: 822 bytes
// Caller: RVA 0x4d4690  Name: FUN_1404d4690  Size: 546 bytes
// Caller: RVA 0x603960  Name: FUN_140603960  Size: 214 bytes
// Caller: RVA 0x603a40  Name: FUN_140603a40  Size: 221 bytes
// Caller: RVA 0x5f3aa0  Name: FUN_1405f3aa0  Size: 956 bytes
// Caller: RVA 0x723d00  Name: FUN_140723d00  Size: 353 bytes
// Caller: RVA 0x603de0  Name: FUN_140603de0  Size: 375 bytes
// Caller: RVA 0x573e80  Name: FUN_140573e80  Size: 827 bytes
// Caller: RVA 0x603040  Name: FUN_140603040  Size: 226 bytes
// Caller: RVA 0x603560  Name: FUN_140603560  Size: 502 bytes
// Caller: RVA 0x7236f0  Name: FUN_1407236f0  Size: 292 bytes
// Caller: RVA 0x603760  Name: FUN_140603760  Size: 502 bytes
// Caller: RVA 0x253700  Name: FUN_140253700  Size: 612 bytes
// Caller: RVA 0x5428b0  Name: FUN_1405428b0  Size: 190 bytes
// Caller: RVA 0x602a00  Name: FUN_140602a00  Size: 220 bytes
// Caller: RVA 0x602ae0  Name: FUN_140602ae0  Size: 348 bytes
// Caller: RVA 0x5f2b80  Name: FUN_1405f2b80  Size: 1494 bytes
// Caller: RVA 0x602c40  Name: FUN_140602c40  Size: 345 bytes
// Caller: RVA 0x542d60  Name: FUN_140542d60  Size: 181 bytes
// Caller: RVA 0x572d90  Name: FUN_140572d90  Size: 251 bytes
// Caller: RVA 0x602da0  Name: FUN_140602da0  Size: 306 bytes
// Caller: RVA 0x602ee0  Name: FUN_140602ee0  Size: 345 bytes
// Caller: RVA 0x6a2f90  Name: FUN_1406a2f90  Size: 1100 bytes
// Caller: RVA 0x602060  Name: FUN_140602060  Size: 314 bytes
// Caller: RVA 0x512180  Name: FUN_140512180  Size: 180 bytes
// Caller: RVA 0x5423a0  Name: FUN_1405423a0  Size: 970 bytes
// Caller: RVA 0x6024d0  Name: FUN_1406024d0  Size: 166 bytes
// Caller: RVA 0x602690  Name: FUN_140602690  Size: 348 bytes
// Caller: RVA 0x542770  Name: FUN_140542770  Size: 311 bytes
// Caller: RVA 0x6027f0  Name: FUN_1406027f0  Size: 220 bytes
// Caller: RVA 0x6a1a50  Name: FUN_1406a1a50  Size: 315 bytes
// Caller: RVA 0x511a20  Name: FUN_140511a20  Size: 162 bytes
// Caller: RVA 0x511ce0  Name: FUN_140511ce0  Size: 162 bytes
// Caller: RVA 0x6a11d0  Name: FUN_1406a11d0  Size: 288 bytes
// Caller: RVA 0x7b12e0  Name: FUN_1407b12e0  Size: 244 bytes
// Caller: RVA 0x580c50  Name: FUN_140580c50  Size: 1218 bytes
// Caller: RVA 0x7b0ca0  Name: FUN_1407b0ca0  Size: 1557 bytes
// Caller: RVA 0x6b04a0  Name: FUN_1406b04a0  Size: 173 bytes
// Caller: RVA 0x540550  Name: FUN_140540550  Size: 166 bytes
// Caller: RVA 0x2406c0  Name: FUN_1402406c0  Size: 505 bytes
// Caller: RVA 0x5406b0  Name: FUN_1405406b0  Size: 189 bytes
// Caller: RVA 0x540770  Name: FUN_140540770  Size: 907 bytes
// Caller: RVA 0x33fb50  Name: FUN_14033fb50  Size: 9 bytes


// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x4c3520
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4c3520  Name: FUN_1404c3520  Size: 240 bytes
// ------------------------------------------------------------

longlong FUN_1404c3520(char *param_1)

{
  char *pcVar1;
  char cVar2;
  char cVar3;
  undefined4 uVar4;
  char *pcVar5;
  longlong lVar6;
  undefined **ppuVar7;
  int iVar8;
  char local_108 [256];
  
  pcVar1 = param_1 + 0x10;
  if (*(longlong *)(param_1 + 0x10) - 1U < 0xff) {
    pcVar5 = param_1;
    if (0xf < *(ulonglong *)(param_1 + 0x18)) {
      pcVar5 = *(char **)param_1;
    }
    lVar6 = -(longlong)pcVar5;
    do {
      cVar2 = *pcVar5;
      pcVar5[(longlong)(local_108 + lVar6)] = cVar2;
      pcVar5 = pcVar5 + 1;
    } while (cVar2 != '\0');
    _strupr(local_108);
    iVar8 = 0;
    ppuVar7 = &PTR_s_LAND_TEAM_CONTAINER_LOCOMOTOR_140a2ac90;
    do {
      pcVar5 = *ppuVar7;
      if (pcVar5 != (char *)0x0) {
        lVar6 = -(longlong)pcVar5;
        do {
          cVar2 = *pcVar5;
          cVar3 = pcVar5[(longlong)(local_108 + lVar6)];
          if (cVar2 != cVar3) break;
          pcVar5 = pcVar5 + 1;
        } while (cVar3 != '\0');
        if (cVar2 == cVar3) {
          lVar6 = (*(code *)(&PTR_FUN_140a2ac98)[(longlong)iVar8 * 4])();
          FUN_1401f92b0(lVar6 + 8);
          if (0xf < *(ulonglong *)(param_1 + 0x18)) {
            param_1 = *(char **)param_1;
          }
          uVar4 = FUN_140215a30(param_1,*(undefined4 *)pcVar1,0);
          *(undefined4 *)(lVar6 + 0x38) = uVar4;
          return lVar6;
        }
      }
      iVar8 = iVar8 + 1;
      ppuVar7 = ppuVar7 + 4;
    } while ((longlong)ppuVar7 < 0x140a2bb30);
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x1aa10
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1aa10  Name: FUN_14001aa10  Size: 92 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14001aa10(void)

{
  FUN_1405309a0(&DAT_140b39140);
  _DAT_140b39088 = &DAT_140b39140;
  _DAT_140b39140 = AILogWindow::vftable;
  FUN_140022730(&DAT_140b391c0,"AI Log",6);
  FUN_1401f92b0(&DAT_140b39140);
  atexit(FUN_1407f4ef0);
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x1aa70
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1aa70  Name: FUN_14001aa70  Size: 92 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14001aa70(void)

{
  FUN_1405309a0(&DAT_140b39280);
  DAT_140b39098 = &DAT_140b39280;
  _DAT_140b39280 = EventLogWindow::vftable;
  FUN_140022730(&DAT_140b39300,"Story Event Log",0xf);
  FUN_1401f92b0(&DAT_140b39280);
  atexit(FUN_1407f4f90);
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x1aad0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1aad0  Name: FUN_14001aad0  Size: 106 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14001aad0(void)

{
  FUN_1405309a0(&DAT_140b390a0);
  DAT_140b39080 = &DAT_140b390a0;
  _DAT_140b390a0 = MainLogWindow::vftable;
  FUN_140022730(&DAT_140b39120,"Main Log",8);
  FUN_1401f92b0(&DAT_140b390a0);
  DAT_140a7c7a8 = FUN_140530d30;
  atexit(FUN_1407f5030);
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x1ab40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x1ab40  Name: FUN_14001ab40  Size: 92 bytes
// ------------------------------------------------------------

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14001ab40(void)

{
  FUN_1405309a0(&DAT_140b391e0);
  DAT_140b39090 = &DAT_140b391e0;
  _DAT_140b391e0 = StoryLogWindow::vftable;
  FUN_140022730(&DAT_140b39260,"Story Log",9);
  FUN_1401f92b0(&DAT_140b391e0);
  atexit(FUN_1407f50d0);
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x4c3610
// ================================================================

// ------------------------------------------------------------
// RVA: 0x4c3610  Name: FUN_1404c3610  Size: 159 bytes
// ------------------------------------------------------------

longlong FUN_1404c3610(int param_1)

{
  undefined *puVar1;
  int iVar2;
  ulonglong uVar3;
  longlong lVar4;
  undefined **ppuVar5;
  int iVar6;
  
  ppuVar5 = &PTR_s_LAND_TEAM_CONTAINER_LOCOMOTOR_140a2ac90;
  iVar6 = 0;
  do {
    puVar1 = *ppuVar5;
    if (puVar1 != (undefined *)0x0) {
      uVar3 = 0xffffffffffffffff;
      do {
        uVar3 = uVar3 + 1;
      } while (puVar1[uVar3] != '\0');
      iVar2 = FUN_140215a30(puVar1,uVar3 & 0xffffffff,0);
      if (iVar2 == param_1) {
        lVar4 = (*(code *)(&PTR_FUN_140a2ac98)[(longlong)iVar6 * 4])();
        FUN_1401f92b0(lVar4 + 8);
        *(int *)(lVar4 + 0x38) = param_1;
        return lVar4;
      }
    }
    iVar6 = iVar6 + 1;
    ppuVar5 = ppuVar5 + 4;
    if (0x140a2bb2f < (longlong)ppuVar5) {
      return 0;
    }
  } while( true );
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x52f920
// ================================================================

// ------------------------------------------------------------
// RVA: 0x52f920  Name: FUN_14052f920  Size: 275 bytes
// ------------------------------------------------------------

void FUN_14052f920(longlong param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong *plVar3;
  longlong *local_res8;
  longlong *local_res18 [2];
  
  if (*(longlong *)(param_1 + 0x610) != 0) {
    plVar2 = (longlong *)FUN_140769c58(0x30);
    if (plVar2 == (longlong *)0x0) {
      local_res8 = (longlong *)0x0;
    }
    else {
      *(undefined4 *)(plVar2 + 1) = 0;
      *plVar2 = (longlong)
                LuaValue<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>,6>
                ::vftable;
      FUN_14001e680(plVar2 + 2,param_2);
      plVar3 = (longlong *)FUN_14024a250();
      local_res18[0] = plVar2;
      (**(code **)*plVar2)(plVar2);
      puVar1 = (undefined8 *)plVar3[3];
      if (puVar1 == (undefined8 *)plVar3[4]) {
        FUN_140144900(plVar3 + 2,puVar1,local_res18);
      }
      else {
        *puVar1 = plVar2;
        (**(code **)*plVar2)(plVar2);
        plVar3[3] = plVar3[3] + 8;
      }
      (**(code **)(*plVar2 + 8))(plVar2);
      local_res8 = plVar3;
      if (plVar3 != (longlong *)0x0) {
        (**(code **)*plVar3)(plVar3);
      }
    }
    FUN_1402437c0(*(undefined8 *)(param_1 + 0x610),local_res18,"Story_Event_Trigger",local_res8,0);
    if (local_res18[0] != (longlong *)0x0) {
      (**(code **)(*local_res18[0] + 8))();
    }
    FUN_14024a4b0(&local_res8);
    if (local_res8 != (longlong *)0x0) {
      (**(code **)(*local_res8 + 8))();
    }
  }
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x71fc10
// ================================================================

// ------------------------------------------------------------
// RVA: 0x71fc10  Name: FUN_14071fc10  Size: 180 bytes
// ------------------------------------------------------------

longlong FUN_14071fc10(longlong param_1)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    if (*(char *)(param_1 + 0x41) != '\0') {
      FUN_14071f710(param_1);
    }
    uVar1 = *(undefined1 *)(param_1 + 0x40);
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar1;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6af410
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6af410  Name: FUN_1406af410  Size: 202 bytes
// ------------------------------------------------------------

longlong FUN_1406af410(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 == (longlong *)0x0) {
    return 0;
  }
  if (*(char *)(param_1 + 0x41) != '\0') {
    FUN_1406aebf0(param_1);
  }
  if (*(char *)(param_1 + 0x40) == '\0') {
    lVar4 = FUN_1405791e0(*(undefined8 *)(param_1 + 0x48));
    if (lVar4 != 0) {
      uVar2 = 0;
      goto LAB_1406af460;
    }
  }
  uVar2 = 1;
LAB_1406af460:
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<bool,5>::vftable;
  *(undefined1 *)(plVar3 + 2) = uVar2;
  lVar4 = FUN_14024a250();
  local_res20 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar1 = *(undefined8 **)(lVar4 + 0x18);
  if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
  }
  else {
    *puVar1 = plVar3;
    (**(code **)*plVar3)(plVar3);
    *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
  }
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar4;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x60efb0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x60efb0  Name: FUN_14060efb0  Size: 212 bytes
// ------------------------------------------------------------

longlong FUN_14060efb0(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    if (((*(float *)(param_1 + 0x28) == DAT_140800804) &&
        (*(float *)(param_1 + 0x2c) == DAT_140800804)) &&
       (*(float *)(param_1 + 0x30) == DAT_140800804)) {
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x69e6e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x69e6e0  Name: FUN_14069e6e0  Size: 217 bytes
// ------------------------------------------------------------

longlong FUN_14069e6e0(void)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  int iVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *local_res20;
  
  plVar4 = (longlong *)FUN_140769c58(0x18);
  if (plVar4 == (longlong *)0x0) {
    return 0;
  }
  iVar3 = DAT_140b153fc;
  if (DAT_140b15418 != (longlong *)0x0) {
    iVar3 = (**(code **)(*DAT_140b15418 + 0xe0))(DAT_140b15418);
  }
  if (iVar3 != 0) {
    lVar5 = FUN_14028aa00(&DAT_140b153e0,DAT_140b15418);
    uVar2 = 0;
    if (lVar5 == 0) goto LAB_14069e744;
  }
  uVar2 = 1;
LAB_14069e744:
  *(undefined4 *)(plVar4 + 1) = 0;
  *plVar4 = (longlong)LuaValue<bool,5>::vftable;
  *(undefined1 *)(plVar4 + 2) = uVar2;
  lVar5 = FUN_14024a250();
  local_res20 = plVar4;
  (**(code **)*plVar4)(plVar4);
  puVar1 = *(undefined8 **)(lVar5 + 0x18);
  if (puVar1 == *(undefined8 **)(lVar5 + 0x20)) {
    FUN_140144900(lVar5 + 0x10,puVar1,&local_res20);
  }
  else {
    *puVar1 = plVar4;
    (**(code **)*plVar4)(plVar4);
    *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
  }
  (**(code **)(*plVar4 + 8))(plVar4);
  return lVar5;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5fd970
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5fd970  Name: FUN_1405fd970  Size: 218 bytes
// ------------------------------------------------------------

longlong FUN_1405fd970(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *(longlong *)(param_1 + 0x80);
    lVar1 = *(longlong *)(param_1 + 0x78);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)(ulonglong)(lVar4 - lVar1 >> 3);
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6adbb0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6adbb0  Name: FUN_1406adbb0  Size: 162 bytes
// ------------------------------------------------------------

longlong FUN_1406adbb0(longlong param_1)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(undefined1 *)(param_1 + 0x40);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6bd020
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6bd020  Name: FUN_1406bd020  Size: 187 bytes
// ------------------------------------------------------------

longlong FUN_1406bd020(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  float fVar4;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    fVar4 = (float)FUN_140692ca0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x88));
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<double,4>::vftable;
    plVar2[2] = (longlong)(double)fVar4;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6bd0e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6bd0e0  Name: FUN_1406bd0e0  Size: 187 bytes
// ------------------------------------------------------------

longlong FUN_1406bd0e0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  float fVar4;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    fVar4 = (float)FUN_140692cf0(*(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x88));
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<double,4>::vftable;
    plVar2[2] = (longlong)(double)fVar4;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5fd320
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5fd320  Name: FUN_1405fd320  Size: 183 bytes
// ------------------------------------------------------------

longlong FUN_1405fd320(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *(longlong *)(param_1 + 0x80);
    lVar1 = *(longlong *)(param_1 + 0x78);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar3 + 2) = lVar4 - lVar1 >> 3 != 0;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6bd300
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6bd300  Name: FUN_1406bd300  Size: 267 bytes
// ------------------------------------------------------------

longlong FUN_1406bd300(longlong param_1)

{
  undefined8 *puVar1;
  bool bVar2;
  longlong *plVar3;
  longlong lVar4;
  float fVar5;
  longlong *local_res8;
  
  if (*(char *)(param_1 + 0x30) == '\0') {
    plVar3 = (longlong *)FUN_140769c58(0x18);
    if (plVar3 == (longlong *)0x0) {
      return 0;
    }
    fVar5 = (float)FUN_140692cf0(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x88));
  }
  else {
    plVar3 = (longlong *)FUN_140769c58(0x18);
    if (plVar3 == (longlong *)0x0) {
      return 0;
    }
    fVar5 = (float)FUN_140692ca0(*(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x88));
  }
  bVar2 = DAT_1408bf110 <= fVar5 - *(float *)(param_1 + 0x40);
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<bool,5>::vftable;
  *(bool *)(plVar3 + 2) = bVar2;
  lVar4 = FUN_14024a250();
  local_res8 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar1 = *(undefined8 **)(lVar4 + 0x18);
  if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  *puVar1 = plVar3;
  (**(code **)*plVar3)(plVar3);
  *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar4;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57d3a0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57d3a0  Name: FUN_14057d3a0  Size: 179 bytes
// ------------------------------------------------------------

longlong FUN_14057d3a0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  bool bVar4;
  longlong *local_res8;
  
  bVar4 = false;
  if (*(longlong *)(param_1 + 0x60) != 0) {
    bVar4 = *(char *)(*(longlong *)(param_1 + 0x60) + 0x337) != -1;
  }
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar2 + 2) = bVar4;
    lVar3 = FUN_14024a250();
    local_res8 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res8);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73d6b0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73d6b0  Name: FUN_14073d6b0  Size: 165 bytes
// ------------------------------------------------------------

longlong FUN_14073d6b0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x48);
    *(undefined4 *)(plVar3 + 1) = 0;
    *(bool *)(plVar3 + 2) = iVar1 == 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73d760
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73d760  Name: FUN_14073d760  Size: 187 bytes
// ------------------------------------------------------------

longlong FUN_14073d760(longlong param_1)

{
  int iVar1;
  int iVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *local_res20;
  
  plVar4 = (longlong *)FUN_140769c58(0x18);
  if (plVar4 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x44);
    iVar2 = *(int *)(param_1 + 0x40);
    *(undefined4 *)(plVar4 + 1) = 0;
    *plVar4 = (longlong)LuaValue<double,4>::vftable;
    plVar4[2] = (longlong)(double)((float)iVar1 / (float)iVar2);
    lVar5 = FUN_14024a250();
    local_res20 = plVar4;
    (**(code **)*plVar4)(plVar4);
    puVar3 = *(undefined8 **)(lVar5 + 0x18);
    if (puVar3 == *(undefined8 **)(lVar5 + 0x20)) {
      FUN_140144900(lVar5 + 0x10,puVar3,&local_res20);
    }
    else {
      *puVar3 = plVar4;
      (**(code **)*plVar4)(plVar4);
      *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
    }
    (**(code **)(*plVar4 + 8))(plVar4);
    return lVar5;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73cae0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73cae0  Name: FUN_14073cae0  Size: 241 bytes
// ------------------------------------------------------------

longlong FUN_14073cae0(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  int iVar3;
  undefined8 uVar4;
  longlong *plVar5;
  longlong lVar6;
  longlong *local_res20;
  
  uVar4 = FUN_1406021a0();
  plVar5 = (longlong *)FUN_140769c58(0x18);
  if (plVar5 == (longlong *)0x0) {
    return 0;
  }
  if (*(char *)(param_1 + 0x55) != '\0') {
    FUN_14073bc50(param_1,0);
  }
  if (*(char *)(param_1 + 0x40) == '\0') {
    iVar3 = FUN_1405fca10(uVar4);
    if (iVar3 != 0) {
      uVar2 = 0;
      goto LAB_14073cb38;
    }
  }
  uVar2 = 1;
LAB_14073cb38:
  *(undefined4 *)(plVar5 + 1) = 0;
  *plVar5 = (longlong)LuaValue<bool,5>::vftable;
  *(undefined1 *)(plVar5 + 2) = uVar2;
  lVar6 = FUN_14024a250();
  local_res20 = plVar5;
  (**(code **)*plVar5)(plVar5);
  puVar1 = *(undefined8 **)(lVar6 + 0x18);
  if (puVar1 != *(undefined8 **)(lVar6 + 0x20)) {
    *puVar1 = plVar5;
    (**(code **)*plVar5)(plVar5);
    *(longlong *)(lVar6 + 0x18) = *(longlong *)(lVar6 + 0x18) + 8;
    (**(code **)(*plVar5 + 8))(plVar5);
    return lVar6;
  }
  FUN_140144900(lVar6 + 0x10,puVar1,&local_res20);
  (**(code **)(*plVar5 + 8))(plVar5);
  return lVar6;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5fcda0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5fcda0  Name: FUN_1405fcda0  Size: 281 bytes
// ------------------------------------------------------------

longlong FUN_1405fcda0(longlong param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  ulonglong uVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  longlong *local_res8;
  
  lVar2 = *(longlong *)(param_1 + 0x78);
  uVar5 = 0;
  fVar6 = 0.0;
  if (*(longlong *)(param_1 + 0x80) - lVar2 >> 3 != 0) {
    uVar4 = 0;
    fVar7 = fVar6;
    do {
      lVar2 = FUN_1405791e0(*(undefined8 *)(lVar2 + uVar4 * 8));
      fVar6 = fVar7;
      if (((lVar2 != 0) && (*(longlong *)(lVar2 + 0x298) != 0)) &&
         (fVar6 = (float)FUN_140370e60(), fVar6 <= fVar7)) {
        fVar6 = fVar7;
      }
      lVar2 = *(longlong *)(param_1 + 0x78);
      uVar5 = uVar5 + 1;
      uVar4 = (ulonglong)uVar5;
      fVar7 = fVar6;
    } while (uVar4 < (ulonglong)(*(longlong *)(param_1 + 0x80) - lVar2 >> 3));
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 == (longlong *)0x0) {
    return 0;
  }
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<double,4>::vftable;
  plVar3[2] = (longlong)(double)fVar6;
  lVar2 = FUN_14024a250();
  local_res8 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar1 = *(undefined8 **)(lVar2 + 0x18);
  if (puVar1 == *(undefined8 **)(lVar2 + 0x20)) {
    FUN_140144900(lVar2 + 0x10,puVar1,&local_res8);
  }
  else {
    *puVar1 = plVar3;
    (**(code **)*plVar3)(plVar3);
    *(longlong *)(lVar2 + 0x18) = *(longlong *)(lVar2 + 0x18) + 8;
  }
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar2;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5fcec0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5fcec0  Name: FUN_1405fcec0  Size: 278 bytes
// ------------------------------------------------------------

longlong FUN_1405fcec0(longlong param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  ulonglong uVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  longlong *local_res8;
  
  lVar2 = *(longlong *)(param_1 + 0x78);
  uVar5 = 0;
  fVar7 = 0.0;
  if (*(longlong *)(param_1 + 0x80) - lVar2 >> 3 != 0) {
    uVar4 = 0;
    do {
      lVar2 = FUN_1405791e0(*(undefined8 *)(lVar2 + uVar4 * 8));
      if ((lVar2 != 0) && (*(longlong *)(lVar2 + 0x298) != 0)) {
        fVar6 = (float)FUN_140370e60();
        fVar7 = fVar7 + fVar6;
      }
      lVar2 = *(longlong *)(param_1 + 0x78);
      uVar5 = uVar5 + 1;
      uVar4 = (ulonglong)uVar5;
    } while (uVar4 < (ulonglong)(*(longlong *)(param_1 + 0x80) - lVar2 >> 3));
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar7;
    lVar2 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar2 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar2 + 0x20)) {
      FUN_140144900(lVar2 + 0x10,puVar1,&local_res8);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar2 + 0x18) = *(longlong *)(lVar2 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar2;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c030
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c030  Name: FUN_14057c030  Size: 215 bytes
// ------------------------------------------------------------

longlong FUN_14057c030(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar2 = FUN_14039a840(*(undefined8 *)(param_1 + 0x60));
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c110
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c110  Name: FUN_14057c110  Size: 215 bytes
// ------------------------------------------------------------

longlong FUN_14057c110(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar2 = FUN_14039aa60(*(undefined8 *)(param_1 + 0x60));
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c2e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c2e0  Name: FUN_14057c2e0  Size: 253 bytes
// ------------------------------------------------------------

longlong FUN_14057c2e0(longlong param_1,undefined8 param_2)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) != 0) {
    if (*(char *)(*(longlong *)(param_1 + 0x60) + 0x335) != -1) {
      plVar3 = (longlong *)FUN_140769c58(0x18);
      if (plVar3 == (longlong *)0x0) {
        return 0;
      }
      uVar1 = *(undefined1 *)(*(longlong *)(*(longlong *)(param_1 + 0x60) + 0xb8) + 0x2c8);
      *(undefined4 *)(plVar3 + 1) = 0;
      *plVar3 = (longlong)LuaValue<bool,5>::vftable;
      *(undefined1 *)(plVar3 + 2) = uVar1;
      lVar4 = FUN_14024a250();
      local_res8 = plVar3;
      (**(code **)*plVar3)(plVar3);
      puVar2 = *(undefined8 **)(lVar4 + 0x18);
      if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
        *puVar2 = plVar3;
        (**(code **)*plVar3)(plVar3);
        *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
        (**(code **)(*plVar3 + 8))(plVar3);
        return lVar4;
      }
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140248190(param_2,
                  "GameObjectWrapper::Is_Planet_Destroyed -- this function is only valid on planet objects."
                 );
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c3e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c3e0  Name: FUN_14057c3e0  Size: 229 bytes
// ------------------------------------------------------------

longlong FUN_14057c3e0(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x60) + 0xb0);
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    if ((lVar4 == 0) || (*(char *)(lVar4 + 0x38) == '\0')) {
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c4d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c4d0  Name: FUN_14057c4d0  Size: 292 bytes
// ------------------------------------------------------------

longlong FUN_14057c4d0(longlong param_1,undefined8 param_2)

{
  undefined1 uVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 *puVar5;
  longlong *local_res8;
  
  plVar3 = *(longlong **)(param_1 + 0x60);
  if (plVar3 != (longlong *)0x0) {
    if (*(char *)((longlong)plVar3 + 0x35e) != -1) {
      uVar2 = (**(code **)(*plVar3 + 0x10))(plVar3,0x2c);
      plVar3 = (longlong *)FUN_140769c58(0x18);
      if (plVar3 == (longlong *)0x0) {
        return 0;
      }
      uVar1 = FUN_14042a6b0(uVar2);
      *(undefined1 *)(plVar3 + 2) = uVar1;
      *plVar3 = (longlong)LuaValue<bool,5>::vftable;
      *(undefined4 *)(plVar3 + 1) = 0;
      lVar4 = FUN_14024a250();
      local_res8 = plVar3;
      (**(code **)*plVar3)(plVar3);
      puVar5 = *(undefined8 **)(lVar4 + 0x18);
      if (puVar5 != *(undefined8 **)(lVar4 + 0x20)) {
        *puVar5 = plVar3;
        (**(code **)*plVar3)(plVar3);
        *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
        (**(code **)(*plVar3 + 8))(plVar3);
        return lVar4;
      }
      FUN_140144900(lVar4 + 0x10,puVar5,&local_res8);
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    puVar5 = (undefined8 *)(plVar3[0x53] + 0xf8);
    if (0xf < *(ulonglong *)(plVar3[0x53] + 0x110)) {
      puVar5 = (undefined8 *)*puVar5;
    }
    FUN_140248190(param_2,
                  "GameObjectWrapper::Fire_Tactical_Superweapon -- %s is not a tactical superweapon."
                  ,puVar5);
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57c7d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57c7d0  Name: FUN_14057c7d0  Size: 166 bytes
// ------------------------------------------------------------

longlong FUN_14057c7d0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    lVar3 = *(longlong *)(param_1 + 0x60);
    *(undefined4 *)(plVar2 + 1) = 0;
    *(bool *)(plVar2 + 2) = lVar3 != 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57b950
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57b950  Name: FUN_14057b950  Size: 215 bytes
// ------------------------------------------------------------

longlong FUN_14057b950(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar2 = FUN_1403a0910(*(undefined8 *)(param_1 + 0x60));
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57bab0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57bab0  Name: FUN_14057bab0  Size: 165 bytes
// ------------------------------------------------------------

longlong FUN_14057bab0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(param_1 + 0x40);
    *(undefined4 *)(plVar3 + 1) = 0;
    *(bool *)(plVar3 + 2) = iVar1 == 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57be30
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57be30  Name: FUN_14057be30  Size: 215 bytes
// ------------------------------------------------------------

longlong FUN_14057be30(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar2 = FUN_14039a620(*(undefined8 *)(param_1 + 0x60));
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57bf10
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57bf10  Name: FUN_14057bf10  Size: 287 bytes
// ------------------------------------------------------------

longlong FUN_14057bf10(longlong param_1)

{
  undefined8 *puVar1;
  char cVar2;
  undefined1 uVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  cVar2 = FUN_14039b1a0();
  if (cVar2 == '\0') {
    plVar4 = (longlong *)FUN_140769c58(0x18);
    if (plVar4 == (longlong *)0x0) {
      return 0;
    }
    lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x60) + 0x210);
  }
  else {
    lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x60) + 0x2b0);
    plVar4 = (longlong *)FUN_140769c58(0x18);
    if (plVar4 == (longlong *)0x0) {
      return 0;
    }
    lVar5 = *(longlong *)(lVar5 + 0x210);
  }
  if ((lVar5 == 0) || (*(int *)(lVar5 + 8) != 7)) {
    uVar3 = 0;
  }
  else {
    uVar3 = 1;
  }
  *(undefined4 *)(plVar4 + 1) = 0;
  *plVar4 = (longlong)LuaValue<bool,5>::vftable;
  *(undefined1 *)(plVar4 + 2) = uVar3;
  lVar5 = FUN_14024a250();
  local_res8 = plVar4;
  (**(code **)*plVar4)(plVar4);
  puVar1 = *(undefined8 **)(lVar5 + 0x18);
  if (puVar1 == *(undefined8 **)(lVar5 + 0x20)) {
    FUN_140144900(lVar5 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar4 + 8))(plVar4);
    return lVar5;
  }
  *puVar1 = plVar4;
  (**(code **)*plVar4)(plVar4);
  *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
  (**(code **)(*plVar4 + 8))(plVar4);
  return lVar5;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73b3f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73b3f0  Name: FUN_14073b3f0  Size: 187 bytes
// ------------------------------------------------------------

longlong FUN_14073b3f0(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *(longlong *)(param_1 + 0x40);
    if ((lVar4 == 0) || (*(longlong *)(lVar4 + 0x10) != lVar4 + 8)) {
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73a8d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73a8d0  Name: FUN_14073a8d0  Size: 200 bytes
// ------------------------------------------------------------

longlong FUN_14073a8d0(longlong param_1)

{
  undefined8 *puVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(char *)(param_1 + 0x45) == '\0') {
    iVar2 = 0;
    if (DAT_140b15418 != 0) {
      iVar2 = *(int *)(DAT_140b15418 + 0x10);
    }
    if (0x96 < iVar2 - *(int *)(param_1 + 0x40)) {
      FUN_14073b180(param_1,0);
    }
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = *(undefined1 *)(param_1 + 0x45);
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x69aa80
// ================================================================

// ------------------------------------------------------------
// RVA: 0x69aa80  Name: FUN_14069aa80  Size: 275 bytes
// ------------------------------------------------------------

longlong FUN_14069aa80(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 *puVar5;
  longlong *local_res8;
  
  uVar2 = (**(code **)(**(longlong **)(param_1 + 0x30) + 0x10))(*(longlong **)(param_1 + 0x30),3);
  if (*(char *)(param_1 + 0x38) == '\0') {
    iVar1 = FUN_1403f8b30(uVar2,*(undefined8 *)(param_1 + 0x30));
  }
  else {
    iVar1 = FUN_1403f8aa0();
  }
  if (iVar1 < *(int *)(param_1 + 0x3c)) {
    plVar3 = (longlong *)FUN_140769c58(0x18);
    if (plVar3 == (longlong *)0x0) {
      return 0;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar5 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar5 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar5 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      goto LAB_14069ab77;
    }
  }
  else {
    plVar3 = (longlong *)FUN_140769c58(0x18);
    if (plVar3 == (longlong *)0x0) {
      return 0;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = 1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar5 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar5 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar5 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      goto LAB_14069ab77;
    }
  }
  FUN_140144900(lVar4 + 0x10,puVar5,&local_res8);
LAB_14069ab77:
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar4;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x73ade0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x73ade0  Name: FUN_14073ade0  Size: 162 bytes
// ------------------------------------------------------------

longlong FUN_14073ade0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = *(undefined1 *)(param_1 + 0x44);
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5fa090
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5fa090  Name: FUN_1405fa090  Size: 190 bytes
// ------------------------------------------------------------

longlong FUN_1405fa090(longlong param_1)

{
  undefined8 *puVar1;
  char cVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    cVar2 = FUN_1406bffe0(*(undefined8 *)
                           (*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x88) + 0x68));
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar3 + 2) = cVar2 == '\0';
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x57a170
// ================================================================

// ------------------------------------------------------------
// RVA: 0x57a170  Name: FUN_14057a170  Size: 228 bytes
// ------------------------------------------------------------

longlong FUN_14057a170(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    lVar3 = FUN_140397a40(*(undefined8 *)(param_1 + 0x60),0);
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar2 + 1) = 0;
    *(bool *)(plVar2 + 2) = lVar3 != 0;
    lVar3 = FUN_14024a250();
    local_res8 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar3 + 0x20)) {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
      (**(code **)(*plVar2 + 8))(plVar2);
      return lVar3;
    }
    FUN_140144900(lVar3 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5791f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5791f0  Name: FUN_1405791f0  Size: 234 bytes
// ------------------------------------------------------------

longlong FUN_1405791f0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if ((*(longlong *)(param_1 + 0x60) == 0) ||
     (iVar1 = *(int *)(*(longlong *)(param_1 + 0x60) + 0x50), iVar1 == 0x3fffff)) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 == (longlong *)0x0) {
    return 0;
  }
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<double,4>::vftable;
  plVar3[2] = (longlong)(double)iVar1;
  lVar4 = FUN_14024a250();
  local_res8 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar2 = *(undefined8 **)(lVar4 + 0x18);
  if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
    *puVar2 = plVar3;
    (**(code **)*plVar3)(plVar3);
    *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar4;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x7d92c0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x7d92c0  Name: FUN_1407d92c0  Size: 224 bytes
// ------------------------------------------------------------

void FUN_1407d92c0(longlong param_1,longlong *param_2,undefined4 param_3,undefined4 param_4)

{
  longlong *plVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong *local_res8;
  
  plVar3 = (longlong *)FUN_140769c58(0x38);
  local_res8 = plVar3;
  if (plVar3 != (longlong *)0x0) {
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)CameraEffectTrackerClass::vftable;
    plVar3[2] = 0;
    *(undefined4 *)(plVar3 + 3) = param_3;
    *(undefined4 *)((longlong)plVar3 + 0x1c) = param_4;
    plVar3[4] = 0;
    plVar3[5] = 0;
    *(undefined4 *)(plVar3 + 6) = 0;
    plVar1 = (longlong *)plVar3[2];
    if (plVar1 != param_2) {
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 8))();
      }
      plVar3[2] = (longlong)param_2;
      if (param_2 != (longlong *)0x0) {
        (**(code **)*param_2)(param_2);
      }
    }
    (**(code **)*plVar3)(plVar3);
  }
  puVar2 = *(undefined8 **)(param_1 + 8);
  if (puVar2 == *(undefined8 **)(param_1 + 0x10)) {
    FUN_140144900(param_1,puVar2,&local_res8);
  }
  else {
    *puVar2 = plVar3;
    if (plVar3 != (longlong *)0x0) {
      (**(code **)*plVar3)(plVar3);
    }
    *(longlong *)(param_1 + 8) = *(longlong *)(param_1 + 8) + 8;
  }
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 8))(plVar3);
  }
  return;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x7394f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x7394f0  Name: FUN_1407394f0  Size: 294 bytes
// ------------------------------------------------------------

longlong FUN_1407394f0(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined1 uVar3;
  int iVar4;
  undefined8 uVar5;
  longlong *plVar6;
  longlong lVar7;
  longlong *local_res20;
  
  uVar5 = FUN_1406021a0();
  plVar6 = (longlong *)FUN_140769c58(0x18);
  if (plVar6 == (longlong *)0x0) {
    return 0;
  }
  lVar7 = *(longlong *)(param_1 + 0x58);
  if ((((lVar7 != 0) && (lVar1 = *(longlong *)(lVar7 + 0x20), lVar1 != 0)) &&
      (*(char *)(lVar1 + 0x4d) == '\x01')) && (*(float *)(lVar7 + 0x28) <= 0.0)) {
    if (*(char *)(param_1 + 0x42) == '\0') {
      *(undefined1 *)(param_1 + 0x40) = 1;
    }
    else {
      FUN_140738b90(param_1,*(undefined8 *)(param_1 + 0x50),0,*(undefined4 *)(lVar1 + 0x48));
    }
  }
  if (*(char *)(param_1 + 0x40) == '\0') {
    iVar4 = FUN_1405fca10(uVar5);
    if (iVar4 != 0) {
      uVar3 = 0;
      goto LAB_14073957d;
    }
  }
  uVar3 = 1;
LAB_14073957d:
  *(undefined4 *)(plVar6 + 1) = 0;
  *plVar6 = (longlong)LuaValue<bool,5>::vftable;
  *(undefined1 *)(plVar6 + 2) = uVar3;
  lVar7 = FUN_14024a250();
  local_res20 = plVar6;
  (**(code **)*plVar6)(plVar6);
  puVar2 = *(undefined8 **)(lVar7 + 0x18);
  if (puVar2 != *(undefined8 **)(lVar7 + 0x20)) {
    *puVar2 = plVar6;
    (**(code **)*plVar6)(plVar6);
    *(longlong *)(lVar7 + 0x18) = *(longlong *)(lVar7 + 0x18) + 8;
    (**(code **)(*plVar6 + 8))(plVar6);
    return lVar7;
  }
  FUN_140144900(lVar7 + 0x10,puVar2,&local_res20);
  (**(code **)(*plVar6 + 8))(plVar6);
  return lVar7;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x579480
// ================================================================

// ------------------------------------------------------------
// RVA: 0x579480  Name: FUN_140579480  Size: 234 bytes
// ------------------------------------------------------------

longlong FUN_140579480(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if ((*(longlong *)(param_1 + 0x60) == 0) ||
     (iVar1 = *(int *)(*(longlong *)(param_1 + 0x60) + 0x54), iVar1 == 0x3fffff)) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 == (longlong *)0x0) {
    return 0;
  }
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<double,4>::vftable;
  plVar3[2] = (longlong)(double)iVar1;
  lVar4 = FUN_14024a250();
  local_res8 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar2 = *(undefined8 **)(lVar4 + 0x18);
  if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
    *puVar2 = plVar3;
    (**(code **)*plVar3)(plVar3);
    *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar4;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x578b60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x578b60  Name: FUN_140578b60  Size: 202 bytes
// ------------------------------------------------------------

longlong FUN_140578b60(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res8;
  
  if ((*(longlong *)(param_1 + 0x60) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x60) + 0x1c0) != 0)) {
    plVar2 = (longlong *)FUN_140769c58(0x30);
    lVar3 = 0;
    if (plVar2 != (longlong *)0x0) {
      lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x60) + 0x1c0);
      *(undefined4 *)(plVar2 + 1) = 0;
      *plVar2 = (longlong)
                LuaValue<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>,6>
                ::vftable;
      FUN_14001e680(plVar2 + 2,lVar3 + 8);
      lVar3 = FUN_14024a250();
      local_res8 = plVar2;
      (**(code **)*plVar2)(plVar2);
      puVar1 = *(undefined8 **)(lVar3 + 0x18);
      if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
        FUN_140144900(lVar3 + 0x10,puVar1,&local_res8);
      }
      else {
        *puVar1 = plVar2;
        (**(code **)*plVar2)(plVar2);
        *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
      }
      (**(code **)(*plVar2 + 8))(plVar2);
    }
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x578c30
// ================================================================

// ------------------------------------------------------------
// RVA: 0x578c30  Name: FUN_140578c30  Size: 266 bytes
// ------------------------------------------------------------

longlong FUN_140578c30(longlong param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  char cVar2;
  longlong *plVar3;
  longlong lVar4;
  float fVar5;
  longlong *local_res20;
  
  cVar2 = (**(code **)(*DAT_140b15418 + 0xf0))();
  if (cVar2 == '\0') {
    FUN_140248190(param_2,
                  "GameObjectWrapper::Get_Hull -- this command is only supported in tactical modes."
                 );
  }
  else if (*(longlong *)(param_1 + 0x60) != 0) {
    plVar3 = (longlong *)FUN_140769c58(0x18);
    if (plVar3 == (longlong *)0x0) {
      return 0;
    }
    fVar5 = (float)FUN_140396470(*(undefined8 *)(param_1 + 0x60));
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    plVar3[2] = (longlong)(double)fVar5;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x2586d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x2586d0  Name: FUN_1402586d0  Size: 152 bytes
// ------------------------------------------------------------

longlong FUN_1402586d0(void)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = 0;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x258770
// ================================================================

// ------------------------------------------------------------
// RVA: 0x258770  Name: FUN_140258770  Size: 166 bytes
// ------------------------------------------------------------

longlong FUN_140258770(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    lVar3 = *(longlong *)(param_1 + 0x38);
    *(undefined4 *)(plVar2 + 1) = 0;
    *(bool *)(plVar2 + 2) = lVar3 != 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x227030
// ================================================================

// ------------------------------------------------------------
// RVA: 0x227030  Name: FUN_140227030  Size: 157 bytes
// ------------------------------------------------------------

undefined8 * FUN_140227030(undefined8 *param_1,undefined4 *param_2)

{
  longlong lVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  longlong *plVar5;
  
  plVar5 = (longlong *)FUN_140769c58(0x30);
  if (plVar5 == (longlong *)0x0) {
    plVar5 = (longlong *)0x0;
  }
  else {
    *(undefined4 *)(plVar5 + 1) = 0;
    plVar5[2] = -1;
    *plVar5 = (longlong)WinsockAsyncSocketImpl::vftable;
    uVar2 = param_2[1];
    uVar3 = param_2[2];
    uVar4 = param_2[3];
    *(undefined4 *)(plVar5 + 3) = *param_2;
    *(undefined4 *)((longlong)plVar5 + 0x1c) = uVar2;
    *(undefined4 *)(plVar5 + 4) = uVar3;
    *(undefined4 *)((longlong)plVar5 + 0x24) = uVar4;
    plVar5[5] = *(longlong *)(param_2 + 4);
    FUN_140227410(plVar5);
    (**(code **)*plVar5)(plVar5);
  }
  lVar1 = plVar5[2];
  *param_1 = 0;
  if (lVar1 != -1) {
    *param_1 = plVar5;
    (**(code **)*plVar5)(plVar5);
  }
  (**(code **)(*plVar5 + 8))(plVar5);
  return param_1;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6968d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6968d0  Name: FUN_1406968d0  Size: 180 bytes
// ------------------------------------------------------------

longlong FUN_1406968d0(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    if ((*(char *)(param_1 + 0x68) == '\0') || (*(char *)(param_1 + 0x6a) == '\0')) {
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x576a40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x576a40  Name: FUN_140576a40  Size: 292 bytes
// ------------------------------------------------------------

longlong FUN_140576a40(longlong param_1,undefined8 param_2)

{
  undefined1 uVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 *puVar5;
  longlong *local_res8;
  
  plVar3 = *(longlong **)(param_1 + 0x60);
  if (plVar3 != (longlong *)0x0) {
    if (*(char *)((longlong)plVar3 + 0x35e) != -1) {
      uVar2 = (**(code **)(*plVar3 + 0x10))(plVar3,0x2c);
      plVar3 = (longlong *)FUN_140769c58(0x18);
      if (plVar3 == (longlong *)0x0) {
        return 0;
      }
      uVar1 = FUN_14042a200(uVar2);
      *(undefined1 *)(plVar3 + 2) = uVar1;
      *plVar3 = (longlong)LuaValue<bool,5>::vftable;
      *(undefined4 *)(plVar3 + 1) = 0;
      lVar4 = FUN_14024a250();
      local_res8 = plVar3;
      (**(code **)*plVar3)(plVar3);
      puVar5 = *(undefined8 **)(lVar4 + 0x18);
      if (puVar5 != *(undefined8 **)(lVar4 + 0x20)) {
        *puVar5 = plVar3;
        (**(code **)*plVar3)(plVar3);
        *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
        (**(code **)(*plVar3 + 8))(plVar3);
        return lVar4;
      }
      FUN_140144900(lVar4 + 0x10,puVar5,&local_res8);
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    puVar5 = (undefined8 *)(plVar3[0x53] + 0xf8);
    if (0xf < *(ulonglong *)(plVar3[0x53] + 0x110)) {
      puVar5 = (undefined8 *)*puVar5;
    }
    FUN_140248190(param_2,
                  "GameObjectWrapper::Fire_Tactical_Superweapon -- %s is not a tactical superweapon."
                  ,puVar5);
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5460a0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5460a0  Name: FUN_1405460a0  Size: 262 bytes
// ------------------------------------------------------------

longlong FUN_1405460a0(void)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  float fVar4;
  longlong *local_res20;
  
  lVar2 = FUN_14028a950(&DAT_140b153e0,0);
  if (lVar2 == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar2 = FUN_14028a950(&DAT_140b153e0,0);
    fVar4 = (float)*(int *)(lVar2 + 0x10) * DAT_140b0a344;
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar4;
    lVar2 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar2 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar2 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar2 + 0x18) = *(longlong *)(lVar2 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar2;
    }
    FUN_140144900(lVar2 + 0x10,puVar1,&local_res20);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar2;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6a6210
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6a6210  Name: FUN_1406a6210  Size: 186 bytes
// ------------------------------------------------------------

longlong FUN_1406a6210(longlong param_1)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    if ((*(longlong *)(param_1 + 0x30) == 0) ||
       (*(char *)(*(longlong *)(param_1 + 0x30) + 0x166) != '\0')) {
      uVar2 = 1;
    }
    else {
      uVar2 = 0;
    }
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x606520
// ================================================================

// ------------------------------------------------------------
// RVA: 0x606520  Name: FUN_140606520  Size: 259 bytes
// ------------------------------------------------------------

longlong FUN_140606520(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  longlong *local_res8;
  
  lVar1 = FUN_1406c00c0(*(undefined8 *)(param_1 + 0x28));
  if ((lVar1 == 0) || ((*(char *)(lVar1 + 0x13) == '\0' && (*(char *)(lVar1 + 0x1e) == '\0')))) {
    plVar2 = (longlong *)FUN_140769c58(0x18);
    if (plVar2 == (longlong *)0x0) {
      return 0;
    }
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = 0;
    lVar1 = FUN_14024a250();
    local_res8 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar3 = *(undefined8 **)(lVar1 + 0x18);
    if (puVar3 != *(undefined8 **)(lVar1 + 0x20)) {
      *puVar3 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar1 + 0x18) = *(longlong *)(lVar1 + 0x18) + 8;
      goto LAB_140606607;
    }
  }
  else {
    plVar2 = (longlong *)FUN_140769c58(0x18);
    if (plVar2 == (longlong *)0x0) {
      return 0;
    }
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = 1;
    lVar1 = FUN_14024a250();
    local_res8 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar3 = *(undefined8 **)(lVar1 + 0x18);
    if (puVar3 != *(undefined8 **)(lVar1 + 0x20)) {
      *puVar3 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar1 + 0x18) = *(longlong *)(lVar1 + 0x18) + 8;
      goto LAB_140606607;
    }
  }
  FUN_140144900(lVar1 + 0x10,puVar3,&local_res8);
LAB_140606607:
  (**(code **)(*plVar2 + 8))(plVar2);
  return lVar1;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605810
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605810  Name: FUN_140605810  Size: 179 bytes
// ------------------------------------------------------------

longlong FUN_140605810(longlong param_1)

{
  float fVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    fVar1 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x504);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6058e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6058e0  Name: FUN_1406058e0  Size: 182 bytes
// ------------------------------------------------------------

longlong FUN_1406058e0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(*(longlong *)(param_1 + 0x28) + 0x878);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)iVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6059a0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6059a0  Name: FUN_1406059a0  Size: 183 bytes
// ------------------------------------------------------------

longlong FUN_1406059a0(longlong param_1)

{
  undefined8 *puVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar2 = FUN_1403743f0(*(undefined8 *)(param_1 + 0x28));
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)iVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605a60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605a60  Name: FUN_140605a60  Size: 185 bytes
// ------------------------------------------------------------

longlong FUN_140605a60(longlong param_1)

{
  uint uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(uint *)(*(longlong *)(param_1 + 0x28) + 0x89c);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)uVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605bc0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605bc0  Name: FUN_140605bc0  Size: 193 bytes
// ------------------------------------------------------------

longlong FUN_140605bc0(longlong param_1)

{
  uint uVar1;
  undefined8 *puVar2;
  undefined1 uVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *local_res20;
  
  plVar4 = (longlong *)FUN_140769c58(0x18);
  if (plVar4 != (longlong *)0x0) {
    uVar1 = *(uint *)(*(longlong *)(param_1 + 0x28) + 0x1fe8);
    if ((uVar1 < 7) && ((0x52U >> (uVar1 & 0x1f) & 1) != 0)) {
      uVar3 = 1;
    }
    else {
      uVar3 = 0;
    }
    *(undefined4 *)(plVar4 + 1) = 0;
    *plVar4 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar4 + 2) = uVar3;
    lVar5 = FUN_14024a250();
    local_res20 = plVar4;
    (**(code **)*plVar4)(plVar4);
    puVar2 = *(undefined8 **)(lVar5 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar5 + 0x20)) {
      FUN_140144900(lVar5 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar4;
      (**(code **)*plVar4)(plVar4);
      *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
    }
    (**(code **)(*plVar4 + 8))(plVar4);
    return lVar5;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x245ca0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x245ca0  Name: FUN_140245ca0  Size: 152 bytes
// ------------------------------------------------------------

longlong FUN_140245ca0(void)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = 1;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605c90
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605c90  Name: FUN_140605c90  Size: 184 bytes
// ------------------------------------------------------------

longlong FUN_140605c90(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(*(longlong *)(param_1 + 0x28) + 0x1fe8);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar3 + 2) = (iVar1 - 4U & 0xfffffffd) == 0;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605320
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605320  Name: FUN_140605320  Size: 185 bytes
// ------------------------------------------------------------

longlong FUN_140605320(longlong param_1)

{
  uint uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(uint *)(*(longlong *)(param_1 + 0x28) + 0x8a0);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)uVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x195430
// ================================================================

// ------------------------------------------------------------
// RVA: 0x195430  Name: FUN_140195430  Size: 87 bytes
// ------------------------------------------------------------

undefined8 * FUN_140195430(undefined8 *param_1,undefined4 param_2)

{
  char cVar1;
  
  param_1[3] = 0;
  *(undefined4 *)(param_1 + 1) = 0;
  *param_1 = alDrawPrimitive::vftable;
  param_1[2] = alDrawPrimitive::vftable;
  *(undefined4 *)(param_1 + 4) = param_2;
  *(undefined1 *)((longlong)param_1 + 0x24) = 0;
  cVar1 = FUN_14020aa60(&PTR_vftable_140a12ae0,param_1 + 2,1);
  if (cVar1 == '\x01') {
    (**(code **)*param_1)(param_1);
  }
  return param_1;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x255560
// ================================================================

// ------------------------------------------------------------
// RVA: 0x255560  Name: FUN_140255560  Size: 175 bytes
// ------------------------------------------------------------

longlong FUN_140255560(undefined8 param_1,longlong param_2)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(param_2 + 0x60);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)iVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6055d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6055d0  Name: FUN_1406055d0  Size: 185 bytes
// ------------------------------------------------------------

longlong FUN_1406055d0(longlong param_1)

{
  undefined8 *puVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar2 = FUN_1403711c0(*(undefined8 *)(param_1 + 0x28),0);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)iVar2;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605690
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605690  Name: FUN_140605690  Size: 180 bytes
// ------------------------------------------------------------

longlong FUN_140605690(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  float fVar4;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    fVar4 = (float)FUN_140370e60(*(undefined8 *)(param_1 + 0x28));
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<double,4>::vftable;
    plVar2[2] = (longlong)(double)fVar4;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x605750
// ================================================================

// ------------------------------------------------------------
// RVA: 0x605750  Name: FUN_140605750  Size: 179 bytes
// ------------------------------------------------------------

longlong FUN_140605750(longlong param_1)

{
  float fVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    fVar1 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x500);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar1;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x254cb0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x254cb0  Name: FUN_140254cb0  Size: 179 bytes
// ------------------------------------------------------------

longlong FUN_140254cb0(undefined8 param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x30);
  if (plVar2 != (longlong *)0x0) {
    uVar3 = FUN_140251840(param_2);
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)
              LuaValue<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>,6>
              ::vftable;
    FUN_14001e680(plVar2 + 2,uVar3);
    lVar4 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x574fb0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x574fb0  Name: FUN_140574fb0  Size: 235 bytes
// ------------------------------------------------------------

longlong FUN_140574fb0(longlong param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    FUN_140248190(param_2,"GameObjectWrapper::Contains_Hero -- this object is already dead.");
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar2 = FUN_14038fa20(*(undefined8 *)(param_1 + 0x60),0);
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5741c0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5741c0  Name: FUN_1405741c0  Size: 227 bytes
// ------------------------------------------------------------

longlong FUN_1405741c0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(*(longlong *)(*(longlong *)(param_1 + 0x60) + 0x298) + 0x2398);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar3 + 2) = 0 < iVar1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x603960
// ================================================================

// ------------------------------------------------------------
// RVA: 0x603960  Name: FUN_140603960  Size: 214 bytes
// ------------------------------------------------------------

longlong FUN_140603960(longlong param_1)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(undefined1 *)(*(longlong *)(param_1 + 0x28) + 0x62);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x603a40
// ================================================================

// ------------------------------------------------------------
// RVA: 0x603a40  Name: FUN_140603a40  Size: 221 bytes
// ------------------------------------------------------------

longlong FUN_140603a40(longlong param_1)

{
  undefined1 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(undefined1 *)(*(longlong *)(*(longlong *)(param_1 + 0x28) + 0x68) + 0x108);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar3 + 2) = uVar1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x603040
// ================================================================

// ------------------------------------------------------------
// RVA: 0x603040  Name: FUN_140603040  Size: 226 bytes
// ------------------------------------------------------------

longlong FUN_140603040(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar1 = *(int *)(*(longlong *)(param_1 + 0x28) + 0x84);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)iVar1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x7236f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x7236f0  Name: FUN_1407236f0  Size: 292 bytes
// ------------------------------------------------------------

longlong FUN_1407236f0(longlong param_1)

{
  undefined8 *puVar1;
  bool bVar2;
  bool bVar3;
  undefined1 uVar4;
  longlong *plVar5;
  longlong lVar6;
  longlong *local_res8;
  
  lVar6 = *(longlong *)(param_1 + 0x58);
  if (lVar6 - *(longlong *)(param_1 + 0x50) >> 2 != 0) {
    FUN_140722e80(param_1);
    lVar6 = *(longlong *)(param_1 + 0x58);
  }
  if ((*(int *)(param_1 + 0x74) < 0) || (DAT_140b0a320 < *(int *)(param_1 + 0x74))) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if (((lVar6 - *(longlong *)(param_1 + 0x50) & 0xfffffffffffffffcU) == 0) &&
     (*(int *)(param_1 + 0x78) < 1)) {
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  plVar5 = (longlong *)FUN_140769c58(0x18);
  if (plVar5 != (longlong *)0x0) {
    if ((bVar2) || (bVar3)) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
    *(undefined4 *)(plVar5 + 1) = 0;
    *plVar5 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar5 + 2) = uVar4;
    lVar6 = FUN_14024a250();
    local_res8 = plVar5;
    (**(code **)*plVar5)(plVar5);
    puVar1 = *(undefined8 **)(lVar6 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar6 + 0x20)) {
      *puVar1 = plVar5;
      (**(code **)*plVar5)(plVar5);
      *(longlong *)(lVar6 + 0x18) = *(longlong *)(lVar6 + 0x18) + 8;
      (**(code **)(*plVar5 + 8))(plVar5);
      return lVar6;
    }
    FUN_140144900(lVar6 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar5 + 8))(plVar5);
    return lVar6;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5428b0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5428b0  Name: FUN_1405428b0  Size: 190 bytes
// ------------------------------------------------------------

longlong FUN_1405428b0(void)

{
  undefined8 *puVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  float fVar5;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    iVar2 = 0;
    if (DAT_140b15418 != 0) {
      iVar2 = *(int *)(DAT_140b15418 + 0x10);
    }
    fVar5 = (float)iVar2 * DAT_140b0a344;
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar5;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x602a00
// ================================================================

// ------------------------------------------------------------
// RVA: 0x602a00  Name: FUN_140602a00  Size: 220 bytes
// ------------------------------------------------------------

longlong FUN_140602a00(longlong param_1)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x30);
  if (plVar3 != (longlong *)0x0) {
    uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x68);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)
              LuaValue<class_std::basic_string<char,struct_std::char_traits<char>,class_std::allocator<char>_>,6>
              ::vftable;
    FUN_14001e680(plVar3 + 2,uVar1);
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x542d60
// ================================================================

// ------------------------------------------------------------
// RVA: 0x542d60  Name: FUN_140542d60  Size: 181 bytes
// ------------------------------------------------------------

longlong FUN_140542d60(void)

{
  undefined8 *puVar1;
  char cVar2;
  undefined1 uVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong *local_res20;
  
  plVar4 = (longlong *)FUN_140769c58(0x18);
  if (plVar4 != (longlong *)0x0) {
    cVar2 = FUN_14028af60(&DAT_140b153e0);
    if ((cVar2 != '\0') || (uVar3 = 0, DAT_140b1574e != '\0')) {
      uVar3 = 1;
    }
    *(undefined4 *)(plVar4 + 1) = 0;
    *plVar4 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar4 + 2) = uVar3;
    lVar5 = FUN_14024a250();
    local_res20 = plVar4;
    (**(code **)*plVar4)(plVar4);
    puVar1 = *(undefined8 **)(lVar5 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar5 + 0x20)) {
      FUN_140144900(lVar5 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar4;
      (**(code **)*plVar4)(plVar4);
      *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
    }
    (**(code **)(*plVar4 + 8))(plVar4);
    return lVar5;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x572d90
// ================================================================

// ------------------------------------------------------------
// RVA: 0x572d90  Name: FUN_140572d90  Size: 251 bytes
// ------------------------------------------------------------

longlong FUN_140572d90(longlong param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined1 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x60) == 0) {
    FUN_140248190(param_2,"GameObjectWrapper::Are_Engines_Online -- this object is already dead.");
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x60) + 0xa8);
    if (lVar4 == 0) {
      uVar2 = 1;
    }
    else {
      uVar2 = *(undefined1 *)(lVar4 + 0xd0);
    }
    *(undefined1 *)(plVar3 + 2) = uVar2;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined4 *)(plVar3 + 1) = 0;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar1 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar1 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar1 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar1,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x512180
// ================================================================

// ------------------------------------------------------------
// RVA: 0x512180  Name: FUN_140512180  Size: 180 bytes
// ------------------------------------------------------------

longlong FUN_140512180(longlong param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res20;
  
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *(longlong *)(param_1 + 0x48);
    lVar1 = *(longlong *)(param_1 + 0x40);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<bool,5>::vftable;
    *(bool *)(plVar3 + 2) = (lVar4 - lVar1 & 0xfffffffffffffffcU) == 0;
    lVar4 = FUN_14024a250();
    local_res20 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 == *(undefined8 **)(lVar4 + 0x20)) {
      FUN_140144900(lVar4 + 0x10,puVar2,&local_res20);
    }
    else {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
    }
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6024d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6024d0  Name: FUN_1406024d0  Size: 166 bytes
// ------------------------------------------------------------

longlong FUN_1406024d0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    lVar3 = *(longlong *)(param_1 + 0x28);
    *(undefined4 *)(plVar2 + 1) = 0;
    *(bool *)(plVar2 + 2) = lVar3 != 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6027f0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6027f0  Name: FUN_1406027f0  Size: 220 bytes
// ------------------------------------------------------------

longlong FUN_1406027f0(longlong param_1)

{
  float fVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *local_res8;
  
  if (*(longlong *)(param_1 + 0x28) == 0) {
    return 0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 != (longlong *)0x0) {
    fVar1 = *(float *)(*(longlong *)(param_1 + 0x28) + 0x70);
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)LuaValue<double,4>::vftable;
    plVar3[2] = (longlong)(double)fVar1;
    lVar4 = FUN_14024a250();
    local_res8 = plVar3;
    (**(code **)*plVar3)(plVar3);
    puVar2 = *(undefined8 **)(lVar4 + 0x18);
    if (puVar2 != *(undefined8 **)(lVar4 + 0x20)) {
      *puVar2 = plVar3;
      (**(code **)*plVar3)(plVar3);
      *(longlong *)(lVar4 + 0x18) = *(longlong *)(lVar4 + 0x18) + 8;
      (**(code **)(*plVar3 + 8))(plVar3);
      return lVar4;
    }
    FUN_140144900(lVar4 + 0x10,puVar2,&local_res8);
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar4;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x511a20
// ================================================================

// ------------------------------------------------------------
// RVA: 0x511a20  Name: FUN_140511a20  Size: 162 bytes
// ------------------------------------------------------------

longlong FUN_140511a20(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = *(undefined1 *)(param_1 + 0x41);
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x511ce0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x511ce0  Name: FUN_140511ce0  Size: 162 bytes
// ------------------------------------------------------------

longlong FUN_140511ce0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = *(undefined1 *)(param_1 + 0x40);
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6a11d0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6a11d0  Name: FUN_1406a11d0  Size: 288 bytes
// ------------------------------------------------------------

longlong FUN_1406a11d0(longlong param_1)

{
  int iVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  undefined *puVar4;
  longlong lVar5;
  longlong *local_res8;
  
  iVar1 = *(int *)(param_1 + 0x40);
  if (iVar1 == 0) {
    FUN_1406a1930();
    plVar3 = (longlong *)FUN_1406021a0(param_1);
    if (plVar3 != (longlong *)0x0) {
      for (puVar4 = (undefined *)(**(code **)(*plVar3 + 0x20))(plVar3); puVar4 != (undefined *)0x0;
          puVar4 = *(undefined **)(puVar4 + 8)) {
        if (puVar4 == &DAT_140a441e0) {
          FUN_1405f8360(plVar3[0x1e],*(undefined1 *)(param_1 + 0x44));
          break;
        }
      }
    }
  }
  plVar3 = (longlong *)FUN_140769c58(0x18);
  if (plVar3 == (longlong *)0x0) {
    return 0;
  }
  *(undefined4 *)(plVar3 + 1) = 0;
  *plVar3 = (longlong)LuaValue<bool,5>::vftable;
  *(bool *)(plVar3 + 2) = iVar1 == 0;
  lVar5 = FUN_14024a250();
  local_res8 = plVar3;
  (**(code **)*plVar3)(plVar3);
  puVar2 = *(undefined8 **)(lVar5 + 0x18);
  if (puVar2 != *(undefined8 **)(lVar5 + 0x20)) {
    *puVar2 = plVar3;
    (**(code **)*plVar3)(plVar3);
    *(longlong *)(lVar5 + 0x18) = *(longlong *)(lVar5 + 0x18) + 8;
    (**(code **)(*plVar3 + 8))(plVar3);
    return lVar5;
  }
  FUN_140144900(lVar5 + 0x10,puVar2,&local_res8);
  (**(code **)(*plVar3 + 8))(plVar3);
  return lVar5;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x7b12e0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x7b12e0  Name: FUN_1407b12e0  Size: 244 bytes
// ------------------------------------------------------------

longlong * FUN_1407b12e0(longlong param_1,undefined4 *param_2)

{
  undefined8 *puVar1;
  DWORD DVar2;
  longlong *plVar3;
  longlong *local_res18 [2];
  
  DVar2 = GetCurrentThreadId();
  if ((DVar2 == DAT_140a155d0) && (DAT_140a62740 != '\0')) {
    return (longlong *)0x0;
  }
  plVar3 = (longlong *)FUN_140769c58(0x88);
  local_res18[0] = plVar3;
  if (plVar3 != (longlong *)0x0) {
    *(undefined4 *)(plVar3 + 0xe) = 0;
    *(undefined4 *)(plVar3 + 1) = 0;
    *plVar3 = (longlong)PlaneCollisionBodyClass::vftable;
    *(undefined4 *)(plVar3 + 0xf) = *param_2;
    *(undefined4 *)((longlong)plVar3 + 0x7c) = param_2[1];
    *(undefined4 *)(plVar3 + 0x10) = param_2[2];
    *(undefined4 *)((longlong)plVar3 + 0x84) = param_2[3];
    *(undefined4 *)(plVar3 + 0xe) = 2;
    (**(code **)*plVar3)(plVar3);
  }
  puVar1 = *(undefined8 **)(param_1 + 0x110);
  if (puVar1 == *(undefined8 **)(param_1 + 0x118)) {
    FUN_140045680(param_1 + 0x108,puVar1,local_res18);
  }
  else {
    *puVar1 = plVar3;
    if (plVar3 != (longlong *)0x0) {
      (**(code **)*plVar3)(plVar3);
    }
    *(longlong *)(param_1 + 0x110) = *(longlong *)(param_1 + 0x110) + 8;
  }
  if (plVar3 != (longlong *)0x0) {
    (**(code **)(*plVar3 + 8))(plVar3);
  }
  return plVar3;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x6b04a0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x6b04a0  Name: FUN_1406b04a0  Size: 173 bytes
// ------------------------------------------------------------

longlong FUN_1406b04a0(longlong param_1)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong *local_res8;
  
  if (*(char *)(param_1 + 0x50) != '\0') {
    FUN_1406b06f0();
  }
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    *(undefined1 *)(plVar2 + 2) = *(undefined1 *)(param_1 + 0x51);
    lVar3 = FUN_14024a250();
    local_res8 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res8);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x540550
// ================================================================

// ------------------------------------------------------------
// RVA: 0x540550  Name: FUN_140540550  Size: 166 bytes
// ------------------------------------------------------------

longlong FUN_140540550(void)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  bool bVar4;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    bVar4 = DAT_140b30690 == 0;
    *(undefined4 *)(plVar2 + 1) = 0;
    *(bool *)(plVar2 + 2) = bVar4;
    *plVar2 = (longlong)LuaValue<bool,5>::vftable;
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x5406b0
// ================================================================

// ------------------------------------------------------------
// RVA: 0x5406b0  Name: FUN_1405406b0  Size: 189 bytes
// ------------------------------------------------------------

longlong FUN_1405406b0(void)

{
  undefined8 *puVar1;
  longlong *plVar2;
  longlong lVar3;
  ulonglong uVar4;
  float fVar5;
  longlong *local_res20;
  
  plVar2 = (longlong *)FUN_140769c58(0x18);
  if (plVar2 != (longlong *)0x0) {
    uVar4 = (ulonglong)DAT_140b0a320;
    fVar5 = (float)DAT_140b0a340;
    *(undefined4 *)(plVar2 + 1) = 0;
    *plVar2 = (longlong)LuaValue<double,4>::vftable;
    plVar2[2] = (longlong)(double)((float)uVar4 / fVar5);
    lVar3 = FUN_14024a250();
    local_res20 = plVar2;
    (**(code **)*plVar2)(plVar2);
    puVar1 = *(undefined8 **)(lVar3 + 0x18);
    if (puVar1 == *(undefined8 **)(lVar3 + 0x20)) {
      FUN_140144900(lVar3 + 0x10,puVar1,&local_res20);
    }
    else {
      *puVar1 = plVar2;
      (**(code **)*plVar2)(plVar2);
      *(longlong *)(lVar3 + 0x18) = *(longlong *)(lVar3 + 0x18) + 8;
    }
    (**(code **)(*plVar2 + 8))(plVar2);
    return lVar3;
  }
  return 0;
}




// ================================================================
// Small caller of sub-obj-dtor-A: RVA 0x33fb50
// ================================================================

// ------------------------------------------------------------
// RVA: 0x33fb50  Name: FUN_14033fb50  Size: 9 bytes
// ------------------------------------------------------------

void FUN_14033fb50(longlong param_1)

{
  FUN_1401f92b0(param_1 + 8);
  return;
}



