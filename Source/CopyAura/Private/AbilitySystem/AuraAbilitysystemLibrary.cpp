// Copyright CopyAura zrw


#include "AbilitySystem/AuraAbilitysystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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

void UAuraAbilitysystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
  AActor* AvatarActor = ASC->GetAvatarActor();
  
  UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
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

void UAuraAbilitysystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
  UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
  if (CharacterClassInfo == nullptr) return;
  for (TSubclassOf<UGameplayAbility> AbilityClass :CharacterClassInfo->CommonAbilities)
  {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
    ASC->GiveAbility(AbilitySpec);
  }
  const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
  for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartUpAbilities)
  {
    if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
    {
      FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,CombatInterface->GetPlayerLevel());
      ASC->GiveAbility(AbilitySpec);
    }
  }
}

UCharacterClassInfo* UAuraAbilitysystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
  AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (AuraGameMode == nullptr ) return nullptr;
  return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitysystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
  if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    return AuraEffectContext->IsBlockedHit();
  }
  return false;
}

bool UAuraAbilitysystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
  if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    return AuraEffectContext->IsCriticalHit();
  }
  return false;
}

void UAuraAbilitysystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockHit)
{
  if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    AuraEffectContext->SetIsBlockedHit(bInIsBlockHit);
  }
}

void UAuraAbilitysystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
  bool bInIsCriticalHit)
{
  if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
  }
}
