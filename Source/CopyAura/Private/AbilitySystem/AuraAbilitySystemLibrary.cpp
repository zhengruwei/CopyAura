// Copyright CopyAura zrw


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
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

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
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

void UAuraAbilitySystemLibrary::GiveStartUpAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
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

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
  AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
  if (AuraGameMode == nullptr ) return nullptr;
  return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
  if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    return AuraEffectContext->IsBlockedHit();
  }
  return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
  if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    return AuraEffectContext->IsCriticalHit();
  }
  return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockHit)
{
  if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    AuraEffectContext->SetIsBlockedHit(bInIsBlockHit);
  }
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
  bool bInIsCriticalHit)
{
  if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
  {
    AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
  }
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
  TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
  const FVector& SphereOrigin)
{
  FCollisionQueryParams SphereParams;
  SphereParams.AddIgnoredActors(ActorsToIgnore);

  TArray<FOverlapResult> Overlaps;
  if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
  {
    World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
    for (FOverlapResult& Overlap : Overlaps)
    {
      if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
      {
        OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
      }
    }
  }
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
  const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
  const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
  const bool bFriend = bBothArePlayers || bBothAreEnemies;
  return !bFriend;
}
