// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class COPYAURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	/*敌人接口类*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*敌人接口类结束*/

	/*战斗接口类*/
	FORCEINLINE virtual int32 GetPlayerLevel() override{return Level;}
	/*战斗接口类结束*/
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level = 1;
};
