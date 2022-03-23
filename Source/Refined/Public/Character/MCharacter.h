// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class REFINED_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComp;
public:
	AMCharacter();

	UPROPERTY(EditAnywhere, Category = "Jump")
		float JumpHeight;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION()
		void RunStart();
	UFUNCTION()
		void RunEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run")
		float RunSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run")
		float WalkSpeed;

	UFUNCTION()
		void DoubleJump();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
		int JumpCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Jump")
		bool binAir;

	void virtual Landed(const FHitResult& Hit) override;

	UFUNCTION()
		void Dodge();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		bool bcanDash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		bool bisDash;
	UPROPERTY(EditAnywhere, Category = "Dash")
		float DashDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		float DashCD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		float DashR;

	UFUNCTION()
		void DashStop();
	UFUNCTION()
		void DashReset();


	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

};
