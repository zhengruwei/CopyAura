// Copyright CopyAura zrw


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
  DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
  DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
  DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)

  DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
  DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

  TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
  
  AuraDamageStatics()
  {
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);

    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
    DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);

    const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,ArmorDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,ArmorPenetrationDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,BlockChanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance,CriticalHitChanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,CriticalHitResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,CriticalHitDamageDef);

    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane,ArcaneResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,FireResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lighting,LightningResistanceDef);
    TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,PhysicalResistanceDef);
    
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
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

  RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
  RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
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
  float Damage = 0.f;
  for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
  {
    const FGameplayTag DamageTypeTag = Pair.Key;
    const FGameplayTag ResistanceTag = Pair.Value;

    checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag),
           TEXT("TagsToCaptureDefs doesn't contains Tag: [%s] in  ExecCalc_Damage!"), *ResistanceTag.ToString());
    const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

    float DamageTypeValue = OwningSpec.GetSetByCallerMagnitude(DamageTypeTag);
    
    float Resistance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluationParameters,Resistance);
    Resistance = FMath::Clamp(Resistance,0.f,100.f);

    DamageTypeValue *= (100.f - Resistance) / 100.f;
    
    Damage += DamageTypeValue;
  }

  float TargetBlockChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluationParameters,TargetBlockChance);
  TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

  const bool bBlocked = FMath::RandRange(1,100) < TargetBlockChance;

  FGameplayEffectContextHandle EffectContextHandle = OwningSpec.GetEffectContext();
  UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bBlocked);
  
  Damage = bBlocked ? Damage / 2.f : Damage;
  
  float TargetArmor = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluationParameters,TargetArmor);
  TargetArmor = FMath::Max<float>(0.f, TargetArmor);
  
  float SourceArmorPenetration = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluationParameters,SourceArmorPenetration);
  SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

  const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatarActor);
  const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
  const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
  const float EffectiveArmor = TargetArmor *  FMath::Max<float>(0.f,(100 - SourceArmorPenetration * ArmorPenetrationCoefficient)) / 100.f;
  
  const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
  const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
  Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

  float SourceCriticalHitChance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluationParameters,SourceCriticalHitChance);
  SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

  float TargetCriticalHitResistance = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluationParameters,TargetCriticalHitResistance);
  TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);
  
  float SourceCriticalHitDamage = 0.f;
  ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluationParameters,SourceCriticalHitDamage);
  SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

  const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"),FString());
  const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
  const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
  const bool bCriticalHit = FMath::RandRange(1,100) < EffectiveCriticalHitChance;

  UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);

  Damage = bCriticalHit ? 2.f*Damage + SourceCriticalHitDamage : Damage;

  const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
