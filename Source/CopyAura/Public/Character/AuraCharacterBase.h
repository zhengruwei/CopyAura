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
    
    AAuraCharacterBase();
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const {return AttributeSet;}
    
    /** 战斗接口 */
    virtual FVector GetCombatSocketLocation_Implementation() override;
    virtual UAnimMontage*  GetHitReactMontage_Implementation() override;
    virtual void Die() override;
    virtual bool IsDead_Implementation() const override;
    virtual AActor* GetAvatar_Implementation() override;
    virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
    /** 战斗接口结束 */

    UFUNCTION(NetMulticast,Reliable)
    virtual void MultiCastHandleDeath();

    UPROPERTY(EditAnywhere,Category="Combat")
    TArray<FTaggedMontage> AttackMontages;
    
protected:
   
    virtual void BeginPlay() override;
    
    UPROPERTY(EditAnywhere,Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    UPROPERTY(EditAnywhere,Category = "Combat")
    FName WeaponTipSocketName;

    bool bDead = false;

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

    /*Dissolve Effects 溶解效果*/
    void Dissolve();

    UFUNCTION(BlueprintImplementableEvent)
    void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

    UFUNCTION(BlueprintImplementableEvent)
    void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
    
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
private:
    UPROPERTY(EditAnywhere,Category="Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere,Category="Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
