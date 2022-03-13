// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "ShooterPlayerController.h"
#include "ShooterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeCrosshairAccuracy, float, Accuracy);

class USpringArmComponent;
class UCameraComponent;
class AGun;
class ACollectableItems;
UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);


public:	
	UFUNCTION(BlueprintPure)
		bool IsDead()const;
	UFUNCTION(BlueprintPure)
		float GetHealthPrecent()const;
	UFUNCTION(BlueprintPure)
		bool ShootAnimation()const;

	UFUNCTION(BlueprintPure)
		int32 ReturnCurrentAmmoInInventory()const;

	UFUNCTION(BlueprintPure)
		bool GetOpenMenu();

	UFUNCTION(BlueprintPure)
		float GetDamageDealt();
	UFUNCTION(BlueprintPure)
		FVector GetDamageWidgetPosition();
	UFUNCTION(BlueprintImplementableEvent)
		void MaxAmmoIsReached();

	UPROPERTY(BlueprintAssignable, Category = "ChangeCrosshairAccuracy")
		FChangeCrosshairAccuracy CrosshairAccuracy;
	
	UFUNCTION(BlueprintImplementableEvent)
		void OpenGameMenuEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void CreateDamageWidget();
	

	void CallCreateDamageWidgetOnPlayer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

    void Shoot();
	void Release();

    void EventDispatcherChangeCrosshairAccuracy();
	void SetDamageDealtandPosition(float Damage,FVector Position);

private:
	
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void ChangeToWeapon1();
	void ChangeToWeapon2();
	bool ViewTrace(FHitResult& Hit, FVector& ShotDirection);
	void PickUpWeapon();
	void CharacterReloadGun();
	
	void AimDownSight();
	void ReleaseAimDownSight();
	void SaveTest();
	void LoadTest();

	

	UPROPERTY(EditAnyWhere)
		float RotationRate = 10;
	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;
	UPROPERTY(VisibleAnyWhere)
		float Health;
	UPROPERTY(EditAnyWhere)
		float MaxPickRange=10000;

	UPROPERTY()
		int32 MaxAmmoCapacity ;
	UPROPERTY(EditAnyWhere)
		int32 MaxAmmoCapacity_AR = 210;
	UPROPERTY(EditAnyWhere)
		int32 MaxAmmoCapacity_SMG = 150;
	UPROPERTY()
		int32 CurrentAmmoInInventory_AR;
	UPROPERTY()
		int32 CurrentAmmoInInventory_SMG;
	UPROPERTY()
		int32 CurrentAmmoInInventory;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass1;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass2;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACollectableItems> ItemToFall;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* CharacterSpringArm ;
	UPROPERTY(EditAnywhere)
		UCameraComponent* CharacterCamera;
	
	FVector2D CameraInput;

	UPROPERTY()
		AGun* Gun;
	UPROPERTY()
		AGun* Weapon1;
	UPROPERTY()
		AGun* Weapon2;
	

	UFUNCTION(BlueprintPure)
		 AGun* GetGun();

	void OpenGameMenu();
	bool OpenMenu=false;

	float DamageDealt;
	FVector DamageWidgetPosition;

	bool bAimDownSight=false;
	bool TargetDistanceReached = true;
	//bool bAimDownSightReleased;


};
