// Copyright CopyAura zrw

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class COPYAURA_API AAuraPlayerController : public APlayerController
{
  GENERATED_BODY()
public:
  AAuraPlayerController();
  virtual void PlayerTick(float DeltaTime) override;
protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;
private:
  UPROPERTY(EditAnywhere,Category = "Input")
  TObjectPtr<UInputMappingContext> AuraContext;

  UPROPERTY(EditAnywhere,Category = "Input")
  TObjectPtr<UInputAction> MoveAction;

  UPROPERTY(EditAnywhere,Category = "Input")
  TObjectPtr<UInputAction> ShiftAction;

  void ShiftPressed() {bIsShiftKeyDown = true;}
  void ShiftReleased(){bIsShiftKeyDown = false;}

  bool bIsShiftKeyDown = false;

  void Move(const FInputActionValue& InputActionValue);
  void CursorTrace();
  IEnemyInterface* LastActor = nullptr;
  IEnemyInterface* ThisActor = nullptr;
  FHitResult CursorHit;

  void AbilityInputTagPressed(FGameplayTag InputTag);
  void AbilityInputTagReleased(FGameplayTag InputTag);
  void AbilityInputTagHeld(FGameplayTag InputTag);
	
  UPROPERTY(EditDefaultsOnly, Category = "Input")
  TObjectPtr<UAuraInputConfig> InputConfig;

  UPROPERTY()
  TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

  UAuraAbilitySystemComponent* GetAuraASC();

  FVector CachedDestination = FVector::ZeroVector;
  float FollowTime = 0.0f;
  float ShortPressThreshold = 0.5f;
  bool bAutoRunning = false;
  bool bTargeting = false;

  UPROPERTY(EditDefaultsOnly)
  float AutoRunAcceptanceRadius = 50.0f;
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USplineComponent> Spline;

  void AutoRun();
};
