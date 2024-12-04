// Copyright CopyAura zrw


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
  bool bLogNotFound) const
{
  for (const FAuraInputAction& Action : AbilityInputActions)
  {
    if (Action.InputAction && Action.InputTag == InputTag)
    {
      return Action.InputAction;
    }
  }
  if (bLogNotFound)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for Tag [%s],on InputTag [%s]"), *InputTag.ToString(),
           *GetNameSafe(this));
  }
  return nullptr;
}
