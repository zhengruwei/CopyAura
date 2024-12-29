// Copyright CopyAura zrw


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
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
  RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
  const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
  const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

  const AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
  const AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

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

  const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
