// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BigBox.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MCharacter.h"

ABigBox::ABigBox()
{
	ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMove->SetUpdatedComponent(RootComponent);
	ProjectileMove->InitialSpeed = InitialS;
	ProjectileMove->MaxSpeed = 3000.0f;
	ProjectileMove->bRotationFollowsVelocity = false;
	ProjectileMove->bShouldBounce = false;
	ProjectileMove->bIsHomingProjectile = true;
	ProjectileMove->HomingAccelerationMagnitude = 500.0f;
	ProjectileMove->ProjectileGravityScale = 0.0f;

	DelayMove = 1.5f;
}

void ABigBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABigBox::LaunchBox()
{
}

void ABigBox::BeginPlay()
{
	Super::BeginPlay();
	APawn* target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (target)
	{
		ProjectileMove->HomingTargetComponent = target->GetRootComponent();
	}

	FTimerHandle Wait;
	GetWorld()->GetTimerManager().SetTimer(Wait, this, &ABigBox::LaunchBox, DelayMove, false);
}
