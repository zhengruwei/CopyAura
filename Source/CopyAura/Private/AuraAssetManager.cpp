// Copyright CopyAura zrw


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
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

  //要使用目标数据（TargetData）必须加上这句
  UAbilitySystemGlobals::Get().InitGlobalData();
}
