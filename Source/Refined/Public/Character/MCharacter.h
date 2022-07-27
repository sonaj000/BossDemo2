// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class UHealthComp;
class AWeapon;
class UDamageType;
class UNiagaraFunctionLibrary;
class UNiagaraSystem;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponComp;

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
		void Attack();

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		int ComboCounter;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bCanAttack;

	UFUNCTION()
		void AttackReset();

	UPROPERTY()
		FTimerHandle AR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType>NAD;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* SkillMontage;
	//skills
	UFUNCTION()
		void SkillLunge();
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bCanLunge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* Cone;
	UFUNCTION()
		void ChargeSweep();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* SweepEffect;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bCanSweep;
	UFUNCTION()
		void Finisher();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* SpinEffect;
	UPROPERTY(VisibleAnywhere, Category = "Attack")
		bool bCanFinish;
	//skill reset cd function for all skills
	UFUNCTION()
		void SkillReset(int skillnum);
	//movement
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

	UPROPERTY()
		TArray<AActor*>TimeStore;

	UFUNCTION()
		void Dodge();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
		bool bcanDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UNiagaraSystem* DashEffect;
protected:
	UPROPERTY()
		AWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<AWeapon>SpawnWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponAttachSocketName;

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
