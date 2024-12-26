// Copyright CopyAura zrw


#include "AbilitySystem/AuraAbilitysystemLibrary.h"

#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitysystemLibrary::InitializeDefalutAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
  AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (AuraGameMode == nullptr ) return;

  AActor* AvatarActor = ASC->GetAvatarActor();
  
  UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
  FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

  FGameplayEffectContextHandle PrimaryAttributesHandle = ASC->MakeEffectContext();
  PrimaryAttributesHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

  FGameplayEffectContextHandle SecondaryAttributesHandle = ASC->MakeEffectContext();
  SecondaryAttributesHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,SecondaryAttributesHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());


  FGameplayEffectContextHandle VitalAttributesHandle = ASC->MakeEffectContext();
  VitalAttributesHandle.AddSourceObject(AvatarActor);
  const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalAttributesHandle);
  ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitysystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
  AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (AuraGameMode == nullptr ) return;

  UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
  for (TSubclassOf<UGameplayAbility> AbilityClass :CharacterClassInfo->CommonAbilities)
  {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass);
    ASC->GiveAbility(AbilitySpec);
  }
}
