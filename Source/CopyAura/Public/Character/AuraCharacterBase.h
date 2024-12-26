// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAnimMontage;


UCLASS(Abstract)
class COPYAURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AAuraCharacterBase();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const {return AttributeSet;}

    virtual UAnimMontage*  GetHitReactMontage_Implementation() override;
    virtual void Die() override;

    UFUNCTION(NetMulticast,Reliable)
    virtual void MultiCastHandleDeath(); 
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere,Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    UPROPERTY(EditAnywhere,Category = "Combat")
    FName WeaponTipSocketName;

    virtual FVector GetCombatSocketLocation() override;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    virtual void InitAbilityActorInfo();

    UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

    UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

    UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "Attributes")
    TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

    void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass,const float Level) const;

    virtual void InitializeDefaultAttributes() const;

    void AddCharacterAbilities();
private:
    UPROPERTY(EditAnywhere,Category="Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere,Category="Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
