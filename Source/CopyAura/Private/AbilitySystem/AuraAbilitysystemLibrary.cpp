// Copyright CopyAura zrw


#include "AbilitySystem/AuraAbilitysystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitysystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
  if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
  {
    if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
    {
      AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
      UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
      UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
      const FWidgetControllerParams WidgetControllerParams(PlayerController,PlayerState,AbilitySystemComponent, AttributeSet);
      return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
    }
  }
  return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitysystemLibrary::GetAttributeMenuWidgetController(
  const UObject* WorldContextObject)
{
  if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
  {
    if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
    {
      AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
      UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
      UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
      const FWidgetControllerParams WidgetControllerParams(PlayerController,PlayerState,AbilitySystemComponent, AttributeSet);
      return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
    }
  }
  return nullptr;
}
