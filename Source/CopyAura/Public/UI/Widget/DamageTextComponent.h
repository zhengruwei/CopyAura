// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class COPYAURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent,blueprintcallable)
	void SetDamageText(float Damage,bool bBlockedHit,bool bCriticalHit);
};
