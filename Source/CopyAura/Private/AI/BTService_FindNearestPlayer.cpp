// Copyright CopyAura zrw


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

  GEngine->AddOnScreenDebugMessage(1,1,FColor::Red,*AIOwner->GetName());
  GEngine->AddOnScreenDebugMessage(2,1,FColor::Green,*ActorOwner->GetName());
}