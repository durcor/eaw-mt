/* FUN_140387f50  RVA 0x387f50  size 424  params 0  ret undefined */


float FUN_140387f50(longlong param_1,longlong param_2,undefined4 param_3,float param_4)

{
  float fVar1;
  longlong lVar2;
  char cVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  char *pcVar7;
  int local_res8 [2];
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar8;
  int local_48;
  undefined1 local_44 [60];
  
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  fVar1 = *(float *)(param_1 + 0x28);
  if (*(char *)(*(longlong *)(param_1 + 0x20) + 0x4d) == '\0') {
    return fVar1;
  }
  lVar2 = *(longlong *)(param_1 + 0x10);
  if (param_2 != lVar2) {
    return fVar1;
  }
  if (fVar1 <= 0.0) {
    return fVar1;
  }
  if (param_4 <= 0.0) {
    return fVar1;
  }
  local_48 = 0;
  if (*(char *)(lVar2 + 0x38e) != -1) {
    FUN_140397540(lVar2,&local_48);
  }
  *(float *)(param_1 + 0x28) = *(float *)(param_1 + 0x28) - param_4;
  FUN_140386660(param_1,param_3,0);
  if (*(float *)(param_1 + 0x28) <= 0.0) {
    local_res8[0] = 0;
    if (*(char *)(*(longlong *)(param_1 + 0x10) + 0x38e) != -1) {
      FUN_140397540(*(longlong *)(param_1 + 0x10),local_res8);
    }
    if (local_res8[0] != local_48) {
      uVar4 = FUN_140294a70(&DAT_140a16fd0);
      cVar3 = FUN_14039a230(*(undefined8 *)(param_1 + 0x10),uVar4,0);
      if (cVar3 == '\0') {
        uVar4 = FUN_140294a70(&DAT_140a16fd0);
        cVar3 = FUN_14039a450(*(undefined8 *)(param_1 + 0x10),uVar4,0);
        if (cVar3 == '\0') goto LAB_1403880db;
        uVar5 = FUN_1401f93e0(local_44,0xff,0x18,0x27,CONCAT44(uVar8,0xff));
        pcVar7 = "TEXT_ENEMY_SPACE_STATION_LEVEL_LOST";
      }
      else {
        uVar5 = FUN_1401f93e0(local_44,0xff,0x18,0x27,CONCAT44(uVar8,0xff));
        pcVar7 = "TEXT_FRIENDLY_SPACE_STATION_LEVEL_LOST";
      }
      uVar6 = FUN_1401fa460(&DAT_140a7bc58,pcVar7);
      FUN_1402dd630(&DAT_140b27f60,uVar6,uVar5,1,0);
      FUN_1401f9440(local_44);
    }
  }
LAB_1403880db:
  return *(float *)(param_1 + 0x28);
}


