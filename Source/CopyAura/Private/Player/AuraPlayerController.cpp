// Copyright CopyAura zrw


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 *追踪鼠标悬停位置，有以下几种情况：
	 *一、LastActor为空，并且ThisActor为空，即鼠标还没有在敌人上空悬停。
	 *	——什么都不需要做
	 *二、LastActor为空，但ThisActor不为空，即鼠标第一次在敌人上空悬停。
	 *	——高亮当前敌人（Highlight ThisActor)
	 *三、LastActor不为空，但ThisActor为空，即鼠标从上一个敌人上空移开，不再悬停在敌人上空。
	 *	——取消上次高亮的敌人(UnHighlight LastActor)
	 *四、LastActor和ThisActor都不为空，且二者不相等，即鼠标从一个敌人上空转移到另一个敌人上空
	 *	——取消高亮上一个敌人，高亮当前敌人（UnHighlight LastActor,Highlight ThisActor)
	 *五、LastActor和ThisActor都不为空，且二者相等，即鼠标悬停在同一个敌人上空。
	 *	——什么都不需要做。
	 */
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			//情况二
			ThisActor->HighlightActor();
		}
	}
	else//LastActor 不为空
	{
		if(ThisActor == nullptr)
		{
			//情况三
			LastActor->UnHighlightActor();
		}
		else
		{
			if(LastActor != ThisActor)
			{
				//情况四
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
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


