// Copyright CopyAura zrw


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy ==
		EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);
	}

	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	if (InstantApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if (InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	if (InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

