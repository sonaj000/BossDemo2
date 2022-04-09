// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedOrb.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"



// Sets default values
ARangedOrb::ARangedOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Orb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb"));
	RootComponent = Orb;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Cube HitBox"));
	HitBox->SetupAttachment(RootComponent);

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

