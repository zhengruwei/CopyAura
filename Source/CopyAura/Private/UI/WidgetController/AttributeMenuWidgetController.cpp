// Copyright CopyAura zrw


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
  check(AttributeInfo);
  for (auto& Pair: AS->TagsToAttributes)
  {
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
      [this,Pair](const FOnAttributeChangeData& Data)
      {
        BroadcastAttributeInfo(Pair.Key, Pair.Value());
      }
    );
  }
}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
  UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
  check(AttributeInfo);
  for (auto& Pair : AS->TagsToAttributes)
  {
    BroadcastAttributeInfo(Pair.Key, Pair.Value());
  }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag,
  const FGameplayAttribute& Attribute) const
{
  FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(GameplayTag);
  Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
  AttributeInfoDelegate.Broadcast(Info);
}
