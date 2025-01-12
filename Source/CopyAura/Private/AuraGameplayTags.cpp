// Copyright CopyAura zrw


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
  /*
   * 主要属性
   */
  GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Primary.Strength"), FString("增加物理伤害"));//力量
  GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Primary.Intelligence"), FString("增加魔法伤害"));//智力
  GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Primary.Resilience"), FString("增加护甲和护甲穿透"));//韧性
  GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Primary.Vigor"), FString("增加健康值"));//活力
  /*
   * 次要属性
   */
  GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.Armor"), FString("减少所受伤害，提高格挡能力"));
  GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.ArmorPenetration"), FString("无视敌人护甲百分比，增加暴击率"));
  GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.BlockChance"), FString("格挡一次，伤害减半"));
  GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.CriticalHitChance"), FString("触发暴击伤害的几率"));
  GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.CriticalHitDamage"), FString("触发暴击时对敌人产生额外伤害"));
  GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.CriticalHitResistance"), FString("降低敌人的暴击率"));
  GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.HealthRegeneration"), FString("每秒恢复的健康值"));
  GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.ManaRegeneration"), FString("每秒恢复的魔法值"));
  GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.MaxHealth"), FString("最大健康值"));
  GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("Attributes.Secondary.MaxMana"), FString("最大魔法值"));
  /*
   * 输入标签
   */
  GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("InputTag.LMB"), FString("鼠标左键输入标签"));
  GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("InputTag.RMB"), FString("鼠标右键输入标签"));
  GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
  FName("InputTag.1"), FString("1键输入标签"));
  GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("InputTag.2"), FString("2键输入标签"));
  GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("InputTag.3"), FString("3键输入标签"));
 GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("InputTag.4"), FString("4键输入标签"));
 
 GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Damage"), FString("伤害标签"));

/*
 * 伤害类型
 */
 GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Damage.Fire"), FString("火属性伤害类型标签"));
 GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Damage.Lightning"), FString("电属性伤害类型标签"));
 GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Damage.Arcane"), FString("奥术伤害类型标签"));
 GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Damage.Physical"), FString("物理伤害类型标签"));

 /*
 * 伤害抗性
 */
 GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Attributes.Resistance.Arcane"), FString("奥术伤害抗性标签"));
 GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Attributes.Resistance.Fire"), FString("火属性伤害抗性标签"));
 GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Attributes.Resistance.Physical"), FString("物理伤害抗性标签"));
 GameplayTags.Attributes_Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Attributes.Resistance.Lighting"), FString("电属性伤害抗性标签"));

 /*
  * 伤害类型和伤害抗性之间的映射
  */
 GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Resistance_Arcane);
 GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
 GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,GameplayTags.Attributes_Resistance_Physical);
 GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resistance_Lighting);

 /*
  * 游戏效果
  */
 GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
 FName("Effects.HitReact"), FString("受击反应标签"));
}
