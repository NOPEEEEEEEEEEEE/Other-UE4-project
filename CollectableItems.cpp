// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItems.h"

// Sets default values
ACollectableItems::ACollectableItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
   CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
   ARAmmo_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AR_Ammo_Mesh"));
   SMGAmmo_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMG_Ammo_Mesh"));
   MED_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MEDKIT_Mesh"));
   ARAmmo_Mesh -> SetupAttachment(CollisionCapsule);
   SMGAmmo_Mesh -> SetupAttachment(CollisionCapsule);
   MED_Mesh->SetupAttachment(CollisionCapsule);


}


int32 ACollectableItems::GetAR_AmmoStored()
{
	return AR_AmmoStored;
}

int32 ACollectableItems::GetSMG_AmmoStored()
{
	return SMG_AmmoStored;
}

float ACollectableItems::GetMed_Health()
{
	return Med_Health;
}

void ACollectableItems::SetAR_AmmoStored(int32 Ammo)
{
	AR_AmmoStored = Ammo;
}

void ACollectableItems::SetSMG_AmmoStored(int32 Ammo)
{
	SMG_AmmoStored = Ammo;
}

void ACollectableItems::SetMed_Health(float Health)
{
	Med_Health = Health;
}

ECollectableType ACollectableItems::GetCollectableType()
{
	return CollectableType;
}

// Called when the game starts or when spawned
void ACollectableItems::BeginPlay()
{
	Super::BeginPlay();

	
    CollectableType = static_cast<ECollectableType>(FMath::RandRange(0, 2));

	if (CollectableType == ECollectableType::AMMO_FOR_AR)
   {
	   SMGAmmo_Mesh->SetVisibility(false);
	   MED_Mesh->SetVisibility(false);
	   UE_LOG(LogTemp, Warning, TEXT("1"));
   }
   else if (CollectableType == ECollectableType::AMMO_FOR_SMG)
   {
		ARAmmo_Mesh ->SetVisibility(false);
		MED_Mesh->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("2"));
   }

   else if (CollectableType == ECollectableType::MEDKIT)
   {
		ARAmmo_Mesh->SetVisibility(false);
		SMGAmmo_Mesh->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("3"));
   }


}

// Called every frame
void ACollectableItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

