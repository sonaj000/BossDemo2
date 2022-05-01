// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DelayedOrb.h"

ADelayedOrb::ADelayedOrb()
{

}
void ADelayedOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorRotation(GetActorRotation() + FRotator(0, 15, 0));

}
