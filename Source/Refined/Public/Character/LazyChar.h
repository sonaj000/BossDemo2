// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Refined/RefinedCharacter.h"
#include "LazyChar.generated.h"

/**
 * 
 */
UCLASS()
class REFINED_API ALazyChar : public ARefinedCharacter
{
	GENERATED_BODY()

public:
	ALazyChar();

	UPROPERTY(EditAnywhere, Category = "Jump")
		float JumpHeight;

protected:

	UFUNCTION()
		void DoubleJump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		int JumpCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
		bool binAir;

	void virtual Landed(const FHitResult& Hit) override;

	UFUNCTION()
		void Dodge();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;



	
};
