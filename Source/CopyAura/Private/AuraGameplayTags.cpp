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
}