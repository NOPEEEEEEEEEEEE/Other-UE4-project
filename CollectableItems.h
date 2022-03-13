// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "CollectableItems.generated.h"

UENUM(BlueprintType)
enum ECollectableType
{
	AMMO_FOR_AR   UMETA(DisplayName = "AR_Ammo"),
	AMMO_FOR_SMG  UMETA(DisplayName = "SMG_Ammo"),
	MEDKIT        UMETA(DisplayName = "MedKit"),
};

UCLASS()
class SIMPLESHOOTER_API ACollectableItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableItems();
	UFUNCTION()
	int32 GetAR_AmmoStored();
	UFUNCTION()
	int32 GetSMG_AmmoStored();
	UFUNCTION()
	float GetMed_Health();
	UFUNCTION()
		void SetAR_AmmoStored(int32 Ammo);
	UFUNCTION()
		void SetSMG_AmmoStored(int32 Ammo);
	UFUNCTION()
		void SetMed_Health(float Health);


	ECollectableType GetCollectableType();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	
	UPROPERTY(EditAnywhere)
		TEnumAsByte<ECollectableType> CollectableType = AMMO_FOR_AR;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionCapsule;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ARAmmo_Mesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* SMGAmmo_Mesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MED_Mesh;

	UPROPERTY(EditAnywhere)
		int32 AR_AmmoStored;
	UPROPERTY(EditAnywhere)
		int32 SMG_AmmoStored;
	UPROPERTY(EditAnywhere)
		float Med_Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
