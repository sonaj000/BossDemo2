// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"

#include "MCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class UHealthComp;
class UCurveFloat;

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

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		UAudioComponent* FreezeAudio;
	
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

	// dodge implementation
	UFUNCTION()
		void Dodge();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		bool bcanDash;

	UPROPERTY(EditAnywhere)
		UCurveFloat* FloatTimer;

	UPROPERTY()
		float CurveFloatValue;

	UPROPERTY()
		float TimelineValue;

	UPROPERTY()
		FTimeline Time;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		bool bisDash;
protected:
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanFreeze;

	UFUNCTION()
		void DashOverLap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TimeReset();
	UFUNCTION()
		void FreezeReset();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UHealthComp* HealthBar;
	UFUNCTION()
		void OnHealthChanged(UHealthComp* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

};
