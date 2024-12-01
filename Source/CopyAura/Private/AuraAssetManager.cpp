// Copyright CopyAura zrw


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
  check(GEngine);
  UAuraAssetManager* AuraAssetManagerPtr = Cast<UAuraAssetManager>(GEngine->AssetManager);
  return *AuraAssetManagerPtr;
}

void UAuraAssetManager::StartInitialLoading()
{
  Super::StartInitialLoading();
  FAuraGameplayTags::InitializeNativeGameplayTags();
}
