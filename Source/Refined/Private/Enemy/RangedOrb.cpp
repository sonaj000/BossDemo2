// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedOrb.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"



// Sets default values
ARangedOrb::ARangedOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Orb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb"));
	RootComponent = Orb;

	HitBox = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere HitBox"));
	HitBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARangedOrb::BeginPlay()
{
	Super::BeginPlay();
	if (OrbExplosion)
	{
		UNiagaraComponent* WeaponLaser = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrbExplosion, GetActorLocation(), GetActorRotation());
	}
	SetLifeSpan(1.5f);
}

// Called every frame
void ARangedOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

