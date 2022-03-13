// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"
 


class AShooterCharacter; 

UENUM(BlueprintType)
		enum EAmmoType
	{
		AMMO_AR   UMETA(DisplayName="AR_Ammo"),
		AMMO_SMG  UMETA(DisplayName = "SMG_Ammo"),

	};

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void ReleaseTrigger();
	void ReloadGun();
	void SetPhysics(bool Physics);
	void GunIsOwned(bool GunOwned);
	bool GetGunIsOwned();
	USkeletalMeshComponent* GetDropMesh();
	UFUNCTION()
	int32 GetMagSize();
	UFUNCTION(BlueprintPure)
		int32 ReturnCurrentAmmo()const;

	float GetShootingAccuracyLenght();

	bool GetbGunIsCurrentlyReloading();

	void SetAmmoAvailableToReload(int32 AmmoToReload);
	EAmmoType GetAmmoType();

	FVector GetHitLocation();

	void  Aim();
	void  ReleaseAim();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EAmmoType> AmmoType=AMMO_AR;


	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* DropMesh;
	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
		USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere)
		USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere)
		UParticleSystem* ImpactEffect;
    UPROPERTY(EditAnywhere)
		float MaxRange = 1000;
	UPROPERTY(EditAnywhere)
		float Damage = 10;
    UPROPERTY(EditAnywhere)
		float ShootingDelay = 1;
	UPROPERTY(EditAnywhere)
		int32 AmmoQuantity = 30;
	UPROPERTY(EditAnywhere)
		float ReloadTime = 3;
	UPROPERTY(EditAnywhere)
		float DefaultShootingAccuracyLenght = 50;
	
	UPROPERTY(EditAnywhere)
		float AfterShootingAccuracyLenght = 85;
	UPROPERTY(EditAnywhere)
		float MovingAccuracyLenghtAdd = 15;

	UPROPERTY(EditAnywhere)
		float DefaultAimingAccuracyGrower = 20  ;

	bool TargetDistanceReached = true;
	bool IsAiming=false;

	UPROPERTY(EditAnywhere)
		bool GunIsAutomatic = false;

	UPROPERTY()
		float ShootingAccuracyLenght;
	UPROPERTY()
		float AimingAccuracyGrower=0 ;

	UPROPERTY()
		bool HasShot=false;
    UPROPERTY()
	    AShooterCharacter* GunOwner;


	UFUNCTION(BlueprintPure)
		FTimerHandle GetReloadHandle()const;
	

	void CrosshairGrowth(float DeltaTime, float AccuracyLenght,float  GoalAccuracyLenght);
	void CrosshairShrink(float DeltaTime, float AccuracyLenght, float GoalAccuracyLenght);
	void CanCallCrosshairShrink();
	bool CrosshairHasReachedGoal=false;
	bool CrosshairTimerFinished=true;
	bool bCanCallCrosshairShrink;

	UPROPERTY(EditAnywhere)
	float DefalutCrosshairShrinkDelay=0.1;
	UPROPERTY(EditAnywhere)
		float AimingDefalutCrosshairShrinkDelay = 0.04;
	UPROPERTY()
		float CrosshairShrinkDelay;

	int32 AmmoAvailableToReload;


    bool Gun_RayCast(FHitResult &Hit,FVector RayEnd);
	bool GunTrace(FHitResult &Hit,FVector& ShotDirection);
	bool GenerateReandomHitLocationInArea(FHitResult& Hit, FVector RayEnd);

	float SpeedMultiplier;

    bool YouShoot;
	bool TriggerIsPressed=false;

	AController* GetOwnerController()const;

	FTimerHandle ShootingTimer;
	FTimerHandle ReloadTimer;
	FTimerHandle CrosshairRelieveDelay;

	void YouCanShoot();
	void Reload();
	bool bGunIsOwned=false;
	int32 CurrentAmmo;

	bool bGunIsCurrentlyReloading = false;

	FVector HitLocation;
};
