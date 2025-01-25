// Copyright CopyAura zrw


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CausedDamage(AActor* TargetActor)
{
  FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1.f);
  for (TTuple<FGameplayTag,FScalableFloat> Pair : DamageTypes)
  {
    const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,ScaledDamage);
  }
  GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
