// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedOrb.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MCharacter.h"


// Sets default values
ARangedOrb::ARangedOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Orb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb"));
	SetRootComponent(Orb);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("The HitBox"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->SetBoundsScale(1.5);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ARangedOrb::BeginOverLap);

	LifeTime = 1.0f;
	ExplosionDelay = 0.0f;

}

// Called when the game starts or when spawned
void ARangedOrb::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	FTimerHandle Explode;
	GetWorld()->GetTimerManager().SetTimer(Explode, this, &ARangedOrb::SpawnExplosion, ExplosionDelay, false);
}

void ARangedOrb::SpawnExplosion()
{
	if (OrbExplosion)
	{
		UNiagaraComponent* WeaponLaser = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrbExplosion, GetActorLocation(), GetActorRotation());
	}
}

// Called every frame
void ARangedOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARangedOrb::BeginOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != NULL && OtherActor != this && OtherActor->IsA(AMCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("overlap"));
		FRotator LaunchDirection = OtherActor->GetActorRotation();
		LaunchDirection.Pitch = 90.0f;
		FVector LaunchVelocity = OtherActor->GetActorForwardVector() * -2500;

		AMCharacter* Recasted = Cast<AMCharacter>(OtherActor);
		if (Recasted)
		{
			UE_LOG(LogTemp, Warning, TEXT("destroy"));
			Recasted->LaunchCharacter(LaunchVelocity, true, true);
			FTimerHandle Kill;
			GetWorld()->GetTimerManager().SetTimer(Kill, this, &ARangedOrb::End, 0.5f, false);
		}

	}

}

void ARangedOrb::End()
{
	Destroy();
}


