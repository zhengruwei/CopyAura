// Copyright CopyAura zrw


#include "AbilitySystem/Abilities/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
  const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
  const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
  const float DeltaSpread = SpawnSpread / NumMinions;
  
  const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f,FVector::UpVector);
  TArray<FVector> SpawnLocations;
  for (int32 i = 0; i < NumMinions; i++)
  {
    const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
    const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
    SpawnLocations.Add(ChosenSpawnLocation);
    
    DrawDebugSphere(GetWorld(),ChosenSpawnLocation,15,12,FColor::Green,false,2);
    UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location,Location+Direction*MaxSpawnDistance,4,FColor::Red,2);
    DrawDebugSphere(GetWorld(),Location + Direction*MinSpawnDistance,10,12,FColor::Red,false,2);
    DrawDebugSphere(GetWorld(),Location + Direction*MaxSpawnDistance,10,12,FColor::Red,false,2);
  }
  return SpawnLocations;
}
