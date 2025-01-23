// Copyright CopyAura zrw


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
  bReplicates = true;

  Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
  Super::PlayerTick(DeltaTime);
  CursorTrace();
  AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount,ACharacter* TargetCharacter,bool bBlockedHit,bool bCriticalHit)
{
  if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
  {
    UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
    DamageText->RegisterComponent();
    DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    DamageText->SetDamageText(DamageAmount,bBlockedHit,bCriticalHit);
  }
}

void AAuraPlayerController::CursorTrace()
{
  GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
  if (!CursorHit.bBlockingHit)
  {
    return;
  }

  LastActor = ThisActor;
  ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
  if (LastActor != ThisActor)
  {
    if (LastActor) LastActor->UnHighlightActor();
    if (ThisActor) ThisActor->HighlightActor();
  }
}

void AAuraPlayerController::AutoRun()
{
  if (!bAutoRunning) return;
  if (APawn* ControlledPawn = GetPawn())
  {
    const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
      ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
    const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
    ControlledPawn->AddMovementInput(Direction);

    const float DistanceToDestination =(LocationOnSpline - CachedDestination).Length();
    if (DistanceToDestination <= AutoRunAcceptanceRadius)
    {
      bAutoRunning = false;
    }
  }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
  if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
  {
    bTargeting = ThisActor ? true : false;
    bAutoRunning = false;
  }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
  if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
  {
    if (GetAuraASC())
    {
      GetAuraASC()->AbilityInputTagReleased(InputTag);
    }
    return;
  }
  if (GetAuraASC())
  {
    GetAuraASC()->AbilityInputTagReleased(InputTag);
  }
  if (!bTargeting && !bIsShiftKeyDown)
  {
    const APawn* ControlledPawn = GetPawn();
    if (FollowTime <= ShortPressThreshold && ControlledPawn)
    {
      if (UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
        this, ControlledPawn->GetActorLocation(), CachedDestination))
      {
        Spline->ClearSplinePoints();
        for (const FVector& PointLoc : NavigationPath->PathPoints)
        {
          Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
        }
        if (NavigationPath->PathPoints.Num() > 0)
        {
          CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num()-1];
          bAutoRunning = true;
        }
      }
    }
    FollowTime = 0.f;
    bTargeting = false;
  }
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
  if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
  {
    if (GetAuraASC())
    {
      GetAuraASC()->AbilityInputTagHeld(InputTag);
    }
    return;
  }
  if (bTargeting || bIsShiftKeyDown)
  {
    if (GetAuraASC())
    {
      GetAuraASC()->AbilityInputTagHeld(InputTag);
    }
  }
  else
  {
    FollowTime += GetWorld()->GetDeltaSeconds();
    
    if (CursorHit.bBlockingHit)
    {
      CachedDestination = CursorHit.ImpactPoint;
    }
    if (APawn* ControlledPawn = GetPawn())
    {
      const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
      ControlledPawn->AddMovementInput(WorldDirection);
    }
  }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
  if (AuraAbilitySystemComponent == nullptr)
  {
    AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
      UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
  }
  return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
  Super::BeginPlay();
  check(AuraContext);
  if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
  {
    Subsystem->AddMappingContext(AuraContext,0);
  }
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;

  FInputModeGameAndUI InputModeData;
  InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputModeData.SetHideCursorDuringCapture(false);
  SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
  AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
  AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftPressed);
  AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftReleased);
  AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed,
                                         &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
  const FVector2D InputAxisVector2D = InputActionValue.Get<FVector2D>();
  const FRotator Rotation = GetControlRotation();
  const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

  const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  if(APawn* ControlledPawn = GetPawn<APawn>())
  {
    ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector2D.Y);
    ControlledPawn->AddMovementInput(RightDirection,InputAxisVector2D.X);
  }
}
