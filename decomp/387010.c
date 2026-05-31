/* FUN_140387010  RVA 0x387010  size 344  params 2  ret undefined */


/* WARNING: Unknown calling convention */

void FUN_140387010(HardPointClass *param_1,int param_2)

{
  char cVar1;
  int iVar2;
  longlong lVar3;
  HardPointOwnerRecord *pHVar4;
  undefined1 *puVar5;
  int *piVar6;
  uint uVar7;
  
  if (param_1->owner_record == (HardPointOwnerRecord *)0x0) {
    return;
  }
  if (param_1->context == (void *)0x0) {
    return;
  }
  if ((*(byte *)((longlong)param_1->context + 0x3a0) & 2) != 0) {
    return;
  }
  if ((DAT_140b15418 != 0) && (cVar1 = FUN_14035f790(), cVar1 != '\0')) {
    return;
  }
  pHVar4 = param_1->owner_record;
  uVar7 = param_2 - param_1->last_serviced_tick;
  param_1->last_serviced_tick = param_2;
  if ((pHVar4->motion_state - 5U < 6) && (param_1->enable_flag == '\x01')) {
    FUN_140387400(param_1,uVar7);
    pHVar4 = param_1->owner_record;
  }
  if ((((pHVar4->render_node_gate != '\x01') || (param_1->enable_flag != '\x01')) ||
      (param_1->context == (void *)0x0)) || (param_1->fire_rate_weight <= 0.0)) goto LAB_140387148;
  if (param_1->cached_bone_idx < 0) {
    lVar3 = FUN_140385cf0(param_1);
    if (lVar3 == 0) goto LAB_140387148;
    puVar5 = &param_1->owner_record[3].field_0x28;
    if (0xf < *(ulonglong *)&param_1->owner_record[3].field_0x40) {
      puVar5 = *(undefined1 **)puVar5;
    }
    iVar2 = FUN_14012d2a0(lVar3,puVar5);
    if (iVar2 < 0) goto LAB_140387148;
    param_1->cached_bone_idx = iVar2;
    FUN_14012d430(lVar3,iVar2,1);
    pHVar4 = param_1->owner_record;
    if (*(longlong *)&pHVar4[3].field_0x58 != 0) {
      piVar6 = &pHVar4[3].motion_state;
      if (0xf < *(ulonglong *)pHVar4[3].node_name._Bx) {
        piVar6 = *(int **)piVar6;
      }
      iVar2 = FUN_14012d2a0(lVar3,piVar6);
      param_1->cached_bone_idx2 = iVar2;
      FUN_14012d430(lVar3,iVar2,1);
    }
  }
  FUN_140381ff0(param_1);
LAB_140387148:
  if (param_1->field_0x6f == '\x01') {
    FUN_140387170(param_1,uVar7);
  }
  return;
}


