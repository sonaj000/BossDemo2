// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CloseBox.h"
#include "Character/MCharacter.h"

ACloseBox::ACloseBox()
{
	bCanMove = false;
	Delay = 1.0f;
}

void ACloseBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanMove)
	{
		FVector Curr = GetActorLocation();
		FVector New = Curr + GetActorForwardVector() * 4000.0f * DeltaTime;
		this->SetActorLocation(New);
	}
}

void ACloseBox::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Change;
	GetWorld()->GetTimerManager().SetTimer(Change, this, &ACloseBox::Move, Delay, false);
}

void ACloseBox::Move()
{
	bCanMove = true;
}

void ACloseBox::BeginOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			FTimerHandle Kill;
			GetWorld()->GetTimerManager().SetTimer(Kill, this, &ACloseBox::D, 0.1f, false);
		}

	}
}

void ACloseBox::D()
{
	Destroy();
}
