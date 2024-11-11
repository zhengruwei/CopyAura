// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COPYAURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
  UFUNCTION(BlueprintCallable)
  void  SetWidgetController(UObject* InWidgetController);
  
  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<UObject> WidgetController;
protected:
  UFUNCTION(BlueprintImplementableEvent)
  void WidgetControllerSet();
};
