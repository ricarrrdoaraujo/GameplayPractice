// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	
	EMS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	
	ESS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class FIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	// UPROPERTY(EditDefaultsOnly, Category="SavedData")
	// TSubclassOf<class AItemStorage> WeaponStorage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
	FVector CombatTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat")
	class USoundCue* HitSound;

	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Enums")
	EMovementStatus MovementStatus;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	float MinSprintStamina;

	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat")
	class AEnemy* CombatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; };

	FRotator GetLookAtRotationYaw(FVector Target);

	// Set movement status and running speed
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Running")
	float SprintingSpeed;

	bool bShiftKeyDown;

	void ShiftKeyDown();

	void ShiftKeyUp();
	
	/* Camera Boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/*Follow Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/* Base turn rates to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookupRate;

	/**
	 * Player Stats
	 * 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerStats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerStats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerStats")
	float MaxStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerStats")
	float Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerStats")
	int32 Coins;

	void DecrementHealth(float Amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	void IncrementCoins(int32 Amount);

	void Die();

	virtual void Jump() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Called for forwards/backwards input*/
	void MoveForward(float Value);
	/*Called for side to side input*/
	void MoveRight(float Value);

	bool bMovingForward;
	bool bMovingRight;

	/** Called via input to turn at a given rate
	 * @param Rate This is a normalized rate, i.e. 1.0 means
	 * 100% of desired rate
	 */
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	* @param Rate This is a normalized rate, i.e. 1.0 means
	* 100% of desired rate
	*/
	void LookupAtRate(float Rate);

	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE class UCameraComponent* GetFollowCamera() const {return FollowCamera;}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Items")
	class AItem* ActiveOvelappingItem;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() {return EquippedWeapon;};
	FORCEINLINE void SetActiveOvelappingItem(AItem* Item) {ActiveOvelappingItem = Item;};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void SwitchLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);
};
