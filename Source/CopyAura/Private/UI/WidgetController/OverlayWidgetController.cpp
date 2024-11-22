// Copyright CopyAura zrw


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
  const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
  
  OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
  OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
  OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
  OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
  const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
    AuraAttributeSet->GetHealthAttribute()).AddLambda(
    [this] (const FOnAttributeChangeData& Data){OnHealthChanged.Broadcast(Data.NewValue);});
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
  AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
  [this] (const FOnAttributeChangeData& Data){OnMaxHealthChanged.Broadcast(Data.NewValue);});
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
  AuraAttributeSet->GetManaAttribute()).AddLambda(
  [this] (const FOnAttributeChangeData& Data){OnManaChanged.Broadcast(Data.NewValue);});
  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
  AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
  [this] (const FOnAttributeChangeData& Data){OnMaxManaChanged.Broadcast(Data.NewValue);});
  Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
  [this](const FGameplayTagContainer& AssetTags)
      {
        for (const FGameplayTag& Tag : AssetTags)
        {
          //例如，假设Tag = Message.HealthPotion
          //"Message.HealthPotion".MatchesTag("Message")将会返回true,"Message".MatchesTag("Message.HealthPotion")将会返回false
          FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
          if (Tag.MatchesTag(MessageTag))
          {
            const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
            MessageWidgetRowDelegate.Broadcast(*Row);
          }
        }
      }
  );
}