// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COPYAURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
  AAuraCharacter();
  virtual void PossessedBy(AController* NewController) override;
  virtual void OnRep_PlayerState() override;

  /*战斗接口类*/
  virtual int32 GetPlayerLevel() override;
  /*战斗接口类结束*/
private:
   virtual void InitAbilityActorInfo() override;
};
