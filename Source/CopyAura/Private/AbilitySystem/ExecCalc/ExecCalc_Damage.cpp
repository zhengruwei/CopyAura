// Copyright CopyAura zrw


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
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
  
  float Armor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluationParameters,Armor);
  Armor = FMath::Max<float>(0.f,Armor);
  ++Armor;

  const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty,EGameplayModOp::Additive,Armor);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
