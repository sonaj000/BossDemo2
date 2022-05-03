// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DelayedOrb.h"
#include "Character/MCharacter.h"

ADelayedOrb::ADelayedOrb()
{

}
void ADelayedOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorRotation(GetActorRotation() + FRotator(0, 15, 0));

}

void ADelayedOrb::BeginOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != NULL && OtherActor != this && OtherActor->IsA(AMCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		FRotator LaunchDirection = OtherActor->GetActorRotation();
		LaunchDirection.Pitch = 90.0f;
		FVector LaunchVelocity = OtherActor->GetActorForwardVector() * -1750;

		AMCharacter* Recasted = Cast<AMCharacter>(OtherActor);
		if (Recasted)
		{
			UE_LOG(LogTemp, Warning, TEXT("destroy"));
			Recasted->LaunchCharacter(LaunchVelocity, true, true);
			Destroy();
		}

	}
}
