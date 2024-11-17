// Copyright CopyAura zrw


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
  InitHealth(50.f);
  InitMaxHealth(100.f);
  InitMana(10.f);
  InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
  Super::PreAttributeChange(Attribute, NewValue);

  if (Attribute == GetHealthAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
  }
 
  if (Attribute == GetManaAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
  }
}

void UAuraAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
  FEffectProperties& Properties) const
{
  //Source = causer of the effect, Target = target of the effect (owner of this AS)
  
  Properties.EffectContextHandle = Data.EffectSpec.GetContext();
  Properties.SourceASC = Properties.EffectContextHandle.GetInstigatorAbilitySystemComponent();

  if (IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid() && Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
  {
    Properties.SourceAvatarActor = Properties.SourceASC->AbilityActorInfo->AvatarActor.Get();
    Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get();
    if (Properties.SourceController == nullptr && Properties.SourceAvatarActor != nullptr)
    {
      if (const APawn* Pawn = Cast<APawn>(Properties.SourceAvatarActor))
      {
        Properties.SourceController = Pawn->GetController();
      }
    }
    if (Properties.SourceController != nullptr)
    {
       Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
    }
  }
  if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
  {
    Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
    Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);
    Properties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
  }
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
  Super::PostGameplayEffectExecute(Data);

  FEffectProperties Properties;
  SetEffectProperties(Data,Properties);
  
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}
