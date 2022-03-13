// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "ShooterCharacter.h"


AGun::AGun()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	F
    DropMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DropMesh"));
	SetRootComponent(DropMesh);

	
   
	
	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	YouShoot = true;
	CurrentAmmo = AmmoQuantity;

	ShootingAccuracyLenght = DefaultShootingAccuracyLenght;
	CrosshairShrinkDelay = DefalutCrosshairShrinkDelay;
	GunIsOwned(false);

}



// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float CrosshairGoal;

	if (GunOwner )
	{
		

		if(CrosshairHasReachedGoal == false&& HasShot)
		{
           CrosshairGoal = AfterShootingAccuracyLenght;
		CrosshairGrowth(DeltaTime, ShootingAccuracyLenght, CrosshairGoal);

        }
		if (CrosshairHasReachedGoal == true)
		{
			if(CrosshairTimerFinished==true)
			{ 

			GetWorldTimerManager().SetTimer(CrosshairRelieveDelay, this, &AGun::CanCallCrosshairShrink, CrosshairShrinkDelay);
			CrosshairTimerFinished = false;

            }
			 CrosshairGoal = DefaultShootingAccuracyLenght;
			 if(bCanCallCrosshairShrink)
			 CrosshairShrink(DeltaTime, ShootingAccuracyLenght, CrosshairGoal);
			
		}
		if (IsAiming==false && !TargetDistanceReached)
		{
			AimingAccuracyGrower=FMath::FInterpConstantTo(AimingAccuracyGrower,0, DeltaTime, 2500.f);
			UE_LOG(LogTemp, Warning, TEXT("AimLenght: %f "), AimingAccuracyGrower);
			if (AimingAccuracyGrower==0)
			{
				TargetDistanceReached = true;
			}
		}
		if (IsAiming==true && !TargetDistanceReached)
		{
			AimingAccuracyGrower=FMath::FInterpConstantTo(AimingAccuracyGrower, DefaultAimingAccuracyGrower, DeltaTime, 2500.f);
			UE_LOG(LogTemp, Warning, TEXT("AimLenght: %f "), AimingAccuracyGrower);
			if (AimingAccuracyGrower == DefaultAimingAccuracyGrower)
			{
				TargetDistanceReached = true;
			}

		}




	    SpeedMultiplier = sqrt(pow(GunOwner->GetVelocity().X, 2) + pow(GunOwner->GetVelocity().Y, 2) + pow(GunOwner->GetVelocity().Z, 2)*3  )
		                     	/ 600;
	//	UE_LOG(LogTemp, Warning, TEXT("AccuracyLenght %f"), SpeedMultiplier);
		GunOwner->EventDispatcherChangeCrosshairAccuracy();


	}
    

	//UE_LOG(LogTemp, Warning, TEXT("AccuracyLenght %f"), ShootingAccuracyLenght);
	
	
}
void AGun::CrosshairGrowth(float DeltaTime, float AccuracyLenght, float  GoalAccuracyLenght)
{

	ShootingAccuracyLenght = FMath::FInterpTo(AccuracyLenght, GoalAccuracyLenght, DeltaTime,10);
	
	if (ShootingAccuracyLenght >= GoalAccuracyLenght - 1)
	{
		CrosshairHasReachedGoal = true;
		bCanCallCrosshairShrink = false;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Growing"));

   // UE_LOG(LogTemp, Warning, TEXT("AccuracyLenght %f"), ShootingAccuracyLenght);
//	UE_LOG(LogTemp, Error, TEXT("AfterAccuracyLenght %f"), AfterAccuracyLenght);

}

void AGun::CrosshairShrink(float DeltaTime, float AccuracyLenght, float GoalAccuracyLenght)
{
	ShootingAccuracyLenght = FMath::FInterpTo(AccuracyLenght, GoalAccuracyLenght, DeltaTime, 7);

	if (ShootingAccuracyLenght <= GoalAccuracyLenght + 1)
	{
		CrosshairHasReachedGoal = false;
		HasShot = false;
	}
	//UE_LOG(LogTemp, Error, TEXT("Shrinking"));
	//UE_LOG(LogTemp, Warning, TEXT("AccuracyLenght %f"), ShootingAccuracyLenght);

}

void AGun::CanCallCrosshairShrink()
{
	bCanCallCrosshairShrink = true;
	CrosshairTimerFinished = true;
//	UE_LOG(LogTemp, Error, TEXT("CallCrosshairShrink"));
}


void AGun::PullTrigger()
{
	
	if(YouShoot && CurrentAmmo>0 && !bGunIsCurrentlyReloading)
	{
		TriggerIsPressed = true;
		HasShot = true;
		CrosshairHasReachedGoal = false;
		CrosshairTimerFinished = true;
		CrosshairRelieveDelay.Invalidate();
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, DropMesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, DropMesh, TEXT("MuzzleFlashSocket"));  
		FHitResult Hit;
		FHitResult Gun_Hit;
		FHitResult RandomHit;

		FVector ShotDirection;
		FVector RandomShotDirection;
	 
		
        // GunTrace(Hit, ShotDirection);
		bool bSuccess =GenerateReandomHitLocationInArea(RandomHit, RandomShotDirection);
		if (bSuccess)
		{

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, RandomHit.Location, RandomShotDirection.Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, RandomHit.Location);


			Gun_RayCast(Gun_Hit, RandomHit.Location);
			 

			AActor* HitActor = RandomHit.GetActor();
			HitLocation = RandomHit.Location;
			if (HitActor)
			{
				FPointDamageEvent DamageEvent(Damage, RandomHit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);

			}
         

		}
		ShootingTimer.Invalidate();
		ReloadTimer.Invalidate();
		YouShoot = false;
		 CurrentAmmo--;
		//UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo : %i"),CurrentAmmo);

		if (CurrentAmmo > 0)
		{

			GetWorldTimerManager().SetTimer(ShootingTimer, this, &AGun::YouCanShoot, ShootingDelay);
		//	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
			
		}
	
		

	}
  


}

void AGun::ReleaseTrigger()
{
	//YouShoot = false;
	TriggerIsPressed = false;
}

void AGun::SetPhysics(bool Physics)
{

	DropMesh->SetSimulatePhysics(Physics);

}

void AGun::YouCanShoot() 
{
//	if (ShootingTimer.IsValid())
		YouShoot = true;
		
//	if (ReloadTimer.IsValid())YouShoot = true;
		if(GunIsAutomatic && TriggerIsPressed)
     	{

         PullTrigger();

        }

}

void AGun::ReloadGun()
{
	
	  GetWorldTimerManager().SetTimer(ReloadTimer, this, &AGun::Reload, ReloadTime);
	  bGunIsCurrentlyReloading = true;
	  YouShoot = false;

}

void AGun::Reload()
{
	CurrentAmmo = CurrentAmmo+AmmoAvailableToReload;

	YouCanShoot();
	bGunIsCurrentlyReloading = false;
}

void AGun::GunIsOwned(bool GunOwned)
{
	bGunIsOwned = GunOwned;

	if (!bGunIsOwned  && DropMesh)
	{
		
		SetPhysics(true);
		SetActorEnableCollision(true);
	}
	else if (bGunIsOwned)
	{
		if (DropMesh)
		{
			
			SetPhysics(false);
			SetActorEnableCollision(false);
		}
	    
		GunOwner = Cast<AShooterCharacter>(GetOwner());

	}


}
bool AGun::GetGunIsOwned()
{

	return bGunIsOwned;
}

USkeletalMeshComponent* AGun::GetDropMesh()
{
	return DropMesh;
}

int32 AGun::GetMagSize()
{
	return AmmoQuantity;
}

int32 AGun::ReturnCurrentAmmo()const
{
	return CurrentAmmo;
}
float AGun::GetShootingAccuracyLenght()
{

	return ShootingAccuracyLenght+ SpeedMultiplier* MovingAccuracyLenghtAdd- AimingAccuracyGrower;
}
bool AGun::GetbGunIsCurrentlyReloading()
{
	return bGunIsCurrentlyReloading;
}
void AGun::SetAmmoAvailableToReload(int32 AmmoToReload)
{
	AmmoAvailableToReload = AmmoToReload;

}
EAmmoType AGun::GetAmmoType()
{
	return AmmoType;
}
FVector AGun::GetHitLocation()
{
	return HitLocation;
}
void AGun::Aim()
{
	IsAiming = true;
	TargetDistanceReached = false;
	CrosshairShrinkDelay = AimingDefalutCrosshairShrinkDelay;
}
void AGun::ReleaseAim()
{
	IsAiming = false;
	TargetDistanceReached = false;
	CrosshairShrinkDelay = DefalutCrosshairShrinkDelay;
}
FTimerHandle AGun::GetReloadHandle()const
{
	return ReloadTimer;
}


bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)return false;
	
	FVector Location;
	FRotator Rotation;
	
	OwnerController->GetPlayerViewPoint(Location,Rotation);
    ShotDirection = -Rotation.Vector();
	
	FVector End = Location + Rotation.Vector()* MaxRange;
	FVector HitEnd = End + GetActorUpVector()* ShootingAccuracyLenght;


	FCollisionQueryParams Params;

	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, true, 1, 0, 1);

	//DrawDebugLine(GetWorld(),End,HitEnd , FColor::Blue, true, 1, 0, 1);

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
   
}

bool AGun::GenerateReandomHitLocationInArea(FHitResult& Hit, FVector RayEnd)
{
//	int32 VectorAngle;
	FVector GeneratedLocation;
	FVector HitGenLoc;

	float UpVectorLenght;
	float GenVectorLenght;
	FHitResult HitOut;
	FRotator Rotation;
	FVector UpVector= GetActorUpVector();
	
		//GetActorForwardVector();

	FVector Location;
	AController* OwnerController = GetOwnerController();

	if (OwnerController == nullptr)return false;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
    FVector ForwardVector = -Rotation.Vector();
	FVector End = Location + Rotation.Vector() * MaxRange;

	bool ConditionsVerified=false;

	UpVectorLenght = sqrt(  pow(UpVector.X, 2) + pow(UpVector.Y, 2) + pow(UpVector.Z, 2)  );///////////

	

	///----------------------------------------------------------------------------------------------------------------

	float valX = 1;
	float valY;
	while ((!ConditionsVerified) && valX>=0)
	{
     GeneratedLocation.X = FMath::RandRange(-valX, valX);     ////////
	
	 valY =sqrt( pow(UpVector.X, 2) + pow(UpVector.Y, 2) + pow(UpVector.Z, 2)- pow(GeneratedLocation.X, 2) )  ;

	  while ((!ConditionsVerified) && valY >= 0)
	  {
		 
	    //Generate Y on the wanted vector
		GeneratedLocation.Y = FMath::RandRange(-valY, valY);
		//Generate Z on the wanted vector
        GeneratedLocation.Z = (   -ForwardVector.Y * GeneratedLocation.Y- ForwardVector.X * GeneratedLocation.X   )
		                       / (ForwardVector.Z) ;
        //Lenght of the generated vector
        GenVectorLenght =sqrt(  pow(GeneratedLocation.X, 2) + pow(GeneratedLocation.Y, 2) + pow(GeneratedLocation.Z, 2)  ) ;

		if (GenVectorLenght <= UpVectorLenght)ConditionsVerified = true;
		else valY = valY - 0.05;

	  }
	 
	 valX = valX - 0.05;
     
	}

     HitGenLoc =End + GeneratedLocation* (ShootingAccuracyLenght + SpeedMultiplier * MovingAccuracyLenghtAdd- AimingAccuracyGrower);
  
	 FCollisionQueryParams Params;
	 Params.AddIgnoredActor(this);
	 Params.AddIgnoredActor(GetOwner());
	 //UE_LOG(LogTemp, Error, TEXT("%s"),*Rotation.ToString());
 //UE_LOG(LogTemp, Error, TEXT("SHOT DIRECTION:%s"),* ShotDirection.ToString());


	 return GetWorld()->LineTraceSingleByChannel(Hit, Location, HitGenLoc, ECollisionChannel::ECC_GameTraceChannel1, Params);

	
}


bool AGun::Gun_RayCast(FHitResult& Hit,FVector RayEnd)
{
	FHitResult HitOut;
	FVector Start = this->GetActorLocation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(GetOwner());
	//DrawDebugLine(GetWorld(),Start,RayEnd,FColor::Green,false,1,0,1);

	return GetWorld()->LineTraceSingleByChannel(Hit, Start, RayEnd, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);

}
AController* AGun::GetOwnerController() const
{
 APawn* OwnerPawn= Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)return nullptr;
	return OwnerPawn->GetController();

}


