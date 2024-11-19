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
	/*敌人类接口*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*敌人类接口结束*/
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
