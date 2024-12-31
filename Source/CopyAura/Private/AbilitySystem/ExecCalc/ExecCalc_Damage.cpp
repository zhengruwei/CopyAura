// Copyright CopyAura zrw


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitysystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
  DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
  }
};

static const AuraDamageStatics& DamageStatics()
{
  static AuraDamageStatics DStatics;
  return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
  RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
  RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
  RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
  const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
  const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

  AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
  AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
  ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatarActor);
  ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatarActor);

  const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();

  const FGameplayTagContainer* SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();
  FAggregatorEvaluateParameters EvaluationParameters;
  EvaluationParameters.SourceTags = SourceTags;
  EvaluationParameters.TargetTags = TargetTags;

  // Get Damage Set by Caller Magnitude
  float Damage = OwningSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluationParameters,TargetBlockChance);
  TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

  const bool bBlocked = FMath::RandRange(1,100) < TargetBlockChance;
  Damage = bBlocked ? Damage / 2.f : Damage;
  
  float TargetArmor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluationParameters,TargetArmor);
  TargetArmor = FMath::Max<float>(0.f, TargetArmor);
  
  float SourceArmorPenetration = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluationParameters,SourceArmorPenetration);
  SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

  const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitysystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
  const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
  const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
  const float EffectiveArmor = TargetArmor *  FMath::Max<float>(0.f,(100 - SourceArmorPenetration * ArmorPenetrationCoefficient)) / 100.f;
  
  const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
  const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
  Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

  const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
