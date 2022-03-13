// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Level.h"
#include "CollectableItems.h"
#include "SimpleShooterGameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ShooterPlayerController.h"


AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));

	CharacterSpringArm->SetupAttachment(GetMesh());
    CharacterCamera->SetupAttachment(CharacterSpringArm,USpringArmComponent::SocketName);

	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoInInventory = MaxAmmoCapacity;
	Health = MaxHealth;

    GetMesh()->HideBoneByName(TEXT("weapon_r"),EPhysBodyOp::PBO_None);

   Weapon1 = GetWorld()->SpawnActor<AGun>(GunClass1);
   Weapon2 = GetWorld()->SpawnActor<AGun>(GunClass2);
   Weapon2->GunIsOwned(true);

   GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooterCharacter::OnOverlapBegin);

   CurrentAmmoInInventory_AR = MaxAmmoCapacity_AR;
   CurrentAmmoInInventory_SMG = MaxAmmoCapacity_SMG;

   if (Weapon1->GetAmmoType() == EAmmoType::AMMO_AR)
   {
	    CurrentAmmoInInventory =CurrentAmmoInInventory_AR;
	  
   }
   else if (Weapon1->GetAmmoType() == EAmmoType::AMMO_SMG)
   {
	     CurrentAmmoInInventory=CurrentAmmoInInventory_SMG;
	  
   }
  
   CharacterSpringArm->TargetArmLength = 87;
   CharacterSpringArm->SocketOffset.Y =-250;

   ChangeToWeapon2();
   ChangeToWeapon1();
  
 

}







// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (bAimDownSight )//&& !TargetDistanceReached)
		{
			CharacterSpringArm->TargetArmLength = FMath::FInterpConstantTo(CharacterSpringArm->TargetArmLength, 50.f, DeltaTime, 700.f);
			CharacterSpringArm->SocketOffset.Y = FMath::FInterpConstantTo(CharacterSpringArm->SocketOffset.Y, -100.f, DeltaTime, 2500.f);
			//UE_LOG(LogTemp, Warning, TEXT("AimTargetArmLenght: %f "),CharacterSpringArm->TargetArmLength);
			//UE_LOG(LogTemp, Error, TEXT("AimSocketOffset: %f "), CharacterSpringArm->SocketOffset.Y);
			if (CharacterSpringArm->TargetArmLength == 50.f && CharacterSpringArm->SocketOffset.Y==-100.f)
		    {
				TargetDistanceReached = true;
		    }
			//UE_LOG(LogTemp, Warning, TEXT("Aiming "));
		}
	    if (!bAimDownSight)// && !TargetDistanceReached)
		{
			CharacterSpringArm->TargetArmLength = FMath::FInterpConstantTo(CharacterSpringArm->TargetArmLength, 87.f, DeltaTime, 700.f);
			CharacterSpringArm->SocketOffset.Y = FMath::FInterpConstantTo(CharacterSpringArm->SocketOffset.Y, -250.f, DeltaTime, 2500.f);
			//UE_LOG(LogTemp, Warning, TEXT("AimTargetArmLenght: %f "), CharacterSpringArm->TargetArmLength);
			//UE_LOG(LogTemp, Error, TEXT("AimSocketOffset: %f "), CharacterSpringArm->SocketOffset.Y);
			if (CharacterSpringArm->TargetArmLength == 87.f && CharacterSpringArm->SocketOffset.Y == -250.f)
			{
				TargetDistanceReached = true;
			}
		//	UE_LOG(LogTemp, Warning, TEXT("NOTAiming "));
        }



}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this,&AShooterCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::PitchCamera);
    PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::YawCamera);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("AimDownSight"), EInputEvent::IE_Pressed, this, &AShooterCharacter::AimDownSight);
	PlayerInputComponent->BindAction(TEXT("AimDownSight"), EInputEvent::IE_Released, this, &AShooterCharacter::ReleaseAimDownSight);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AShooterCharacter::Release);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CharacterReloadGun);
	PlayerInputComponent->BindAction(TEXT("Weapon1"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ChangeToWeapon1);
	PlayerInputComponent->BindAction(TEXT("Weapon2"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ChangeToWeapon2);
	PlayerInputComponent->BindAction(TEXT("PickupWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PickUpWeapon);
	PlayerInputComponent->BindAction(TEXT("GameMenu"), EInputEvent::IE_Pressed, this, &AShooterCharacter::OpenGameMenu);
	
}

void AShooterCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACollectableItems* ItemToCollect;
	ItemToCollect = Cast<ACollectableItems>(Other);

	if (ItemToCollect && GetController()->GetClass()->IsChildOf<APlayerController>()  )
	{
	//	UE_LOG(LogTemp, Error, TEXT("%s"), *ItemToCollect->GetName());
		if (ItemToCollect->GetCollectableType() == ECollectableType::AMMO_FOR_AR)
		{
			if (CurrentAmmoInInventory_AR + ItemToCollect->GetAR_AmmoStored() > MaxAmmoCapacity_AR)
			{
				ItemToCollect->SetAR_AmmoStored((CurrentAmmoInInventory_AR + ItemToCollect->GetAR_AmmoStored())-MaxAmmoCapacity_AR);
				CurrentAmmoInInventory_AR = MaxAmmoCapacity_AR;
				CurrentAmmoInInventory = CurrentAmmoInInventory_AR;
				MaxAmmoIsReached();
			}
			else
			{
              CurrentAmmoInInventory_AR = CurrentAmmoInInventory_AR + ItemToCollect->GetAR_AmmoStored();
			  CurrentAmmoInInventory = CurrentAmmoInInventory_AR;
              ItemToCollect->Destroy();

			  UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoInInventory_AR+ItemToCollect :%i"), CurrentAmmoInInventory_AR + ItemToCollect->GetAR_AmmoStored());
			  UE_LOG(LogTemp, Error, TEXT("CurrentAmmoInInventory_AR:%i"), CurrentAmmoInInventory_AR);
			}

		}
		else if (ItemToCollect->GetCollectableType() == ECollectableType::AMMO_FOR_SMG)
		{
			if (CurrentAmmoInInventory_SMG + ItemToCollect->GetSMG_AmmoStored() > MaxAmmoCapacity_SMG)
			{
				ItemToCollect->SetSMG_AmmoStored((CurrentAmmoInInventory_SMG + ItemToCollect->GetSMG_AmmoStored()) - MaxAmmoCapacity_SMG);
				CurrentAmmoInInventory_SMG = MaxAmmoCapacity_SMG;
				CurrentAmmoInInventory = CurrentAmmoInInventory_SMG;
				MaxAmmoIsReached();
			}
			else
			{
				CurrentAmmoInInventory_SMG = CurrentAmmoInInventory_SMG + ItemToCollect->GetSMG_AmmoStored();
				CurrentAmmoInInventory = CurrentAmmoInInventory_SMG;
				ItemToCollect->Destroy();

				
			}

		}

		else if (ItemToCollect->GetCollectableType() == ECollectableType::MEDKIT)
		{
			if (Health + ItemToCollect->GetMed_Health() > MaxHealth)
			{
				float AuxHealth = Health;
				ItemToCollect->SetMed_Health((Health + ItemToCollect->GetMed_Health()) - MaxHealth);
				Health = MaxHealth;
				if(AuxHealth<MaxHealth)
				ItemToCollect->Destroy();

			}
			else
			{
				Health = Health + ItemToCollect->GetMed_Health();
				ItemToCollect->Destroy();


			}

		}

		

	}

}
void AShooterCharacter::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;

	
	FRotator NewRotation = CharacterSpringArm->GetComponentRotation();
		NewRotation.Roll = FMath::Clamp(NewRotation.Roll + CameraInput.Y, -80.0f, 80.0f);
		NewRotation.Roll+= CameraInput.Y;
		CharacterSpringArm->SetWorldRotation(NewRotation);
        AddControllerPitchInput(AxisValue);

}
void AShooterCharacter::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
	FRotator NewRotation = this->GetActorRotation();
	NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;
    AddControllerYawInput(AxisValue);
	
}
void AShooterCharacter::MoveForward(float AxisValue)
{

	AddMovementInput(GetActorForwardVector() * AxisValue);

}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
//	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
	
		//* RotationRate * GetWorld()->GetDeltaSeconds();


}

void AShooterCharacter::LookRightRate(float AxisValue)
{
//	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
	//CameraInput.X = AxisValue;
	//	* RotationRate * GetWorld()->GetDeltaSeconds();


}

void AShooterCharacter::ChangeToWeapon1()
{
	
	if (Weapon2)
	Weapon2->SetActorHiddenInGame(true);
	if (Weapon1)
	{
		Weapon1->SetActorHiddenInGame(false);
		Weapon1->SetPhysics(false);
		Gun = Weapon1;
		
	}
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		Gun->GunIsOwned(true);
	}
	if (Gun->GetAmmoType() == EAmmoType::AMMO_AR)
	{
		MaxAmmoCapacity = MaxAmmoCapacity_AR;
		CurrentAmmoInInventory = CurrentAmmoInInventory_AR;
	//	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoInInventory_SMG:%i"), CurrentAmmoInInventory_SMG);
	}
	else if (Gun->GetAmmoType() == EAmmoType::AMMO_SMG) 
	{ 
		MaxAmmoCapacity = MaxAmmoCapacity_SMG;
		CurrentAmmoInInventory = CurrentAmmoInInventory_SMG;
	//	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoInInventory_AR:%i"), CurrentAmmoInInventory_AR);
	}
}

void AShooterCharacter::ChangeToWeapon2()
{
	
	if (Weapon1)
	Weapon1->SetActorHiddenInGame(true);
	if (Weapon2)
	{
		Weapon2->SetActorHiddenInGame(false);
		Weapon2->SetPhysics(false);
		Gun = Weapon2;
	}
	if (Gun)
	{
        Gun->GunIsOwned(true);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		
	
	}
	if (Gun->GetAmmoType() == EAmmoType::AMMO_AR)
	{
		MaxAmmoCapacity = MaxAmmoCapacity_AR;
		CurrentAmmoInInventory = CurrentAmmoInInventory_AR;
		UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoInInventory_SMG:%i"), CurrentAmmoInInventory_SMG);
	}
	else if (Gun->GetAmmoType() == EAmmoType::AMMO_SMG)
	{
		MaxAmmoCapacity = MaxAmmoCapacity_SMG;
		CurrentAmmoInInventory = CurrentAmmoInInventory_SMG;
		UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoInInventory_AR:%i"), CurrentAmmoInInventory_AR);
	}
	
}



AGun* AShooterCharacter::GetGun()
{
	return Gun;
}

void AShooterCharacter::OpenGameMenu()
{
	if (AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController()))
	{
		OpenMenu = !OpenMenu;
		PlayerController->OpenGameMenu();
	//	PlayerController->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
          
		if (OpenMenu)
		{
			UE_LOG(LogTemp, Warning, TEXT("True"));
			 PlayerController->bShowMouseCursor=true;
		}
		else if (!OpenMenu)
		{
			UE_LOG(LogTemp, Warning, TEXT("False"));
			PlayerController->bShowMouseCursor = false;
		}
        OpenGameMenuEvent();
	}


	//OpenGameMenuDelegate.Broadcast();
}


void AShooterCharacter::Shoot()
{

	if (Gun)Gun->PullTrigger();
	if (Gun->ReturnCurrentAmmo() <= 0 && CurrentAmmoInInventory > 0)CharacterReloadGun();
	ShootAnimation();
	EventDispatcherChangeCrosshairAccuracy();
	
}
void AShooterCharacter::Release()
{
	if (Gun)
	Gun->ReleaseTrigger();
}
void AShooterCharacter::CallCreateDamageWidgetOnPlayer()
{
	
 CreateDamageWidget();
}
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health = Health - DamageToApply;
	AShooterCharacter* DamageCauserCharacter = Cast<AShooterCharacter>(DamageCauser->GetOwner());
	if (AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(DamageCauserCharacter->GetController()))
	{
		
	if (AGun* GunCausingDamage = Cast<AGun>(DamageCauser))
		DamageCauserCharacter->SetDamageDealtandPosition(DamageAmount, GunCausingDamage->GetHitLocation());
        DamageCauserCharacter->CallCreateDamageWidgetOnPlayer();

    }
	if (IsDead())
	{
        ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
        if(ItemToFall)
		{
			UWorld* World = GetWorld();
			if (World)
			{


				FActorSpawnParameters spawnParams;
				FRotator Rotation = { 0,0,0 };
				FVector SpawnLocation = this->GetActorLocation();
				World->SpawnActor<ACollectableItems>(ItemToFall, SpawnLocation, Rotation);
				
		
			}

		}
        if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		

		UE_LOG(LogTemp, Warning, TEXT("Owner : %s"), *this->GetName());

		DetachFromControllerPendingDestroy();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Weapon1->GunIsOwned(false);
		Weapon2->GunIsOwned(false);
		

		//AShooterCharacter* DamageCauserCharacter = Cast<AShooterCharacter>(DamageCauser->GetOwner());
		if (AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(DamageCauserCharacter->GetController()))
		if ( PlayerController)
		PlayerController->IncreaseKillCount();

	}
	return DamageToApply;
}

bool AShooterCharacter::ViewTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetController();
	if (OwnerController == nullptr)return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxPickRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel2, Params);

}

void AShooterCharacter::PickUpWeapon()
{

	FHitResult Hit;
	FVector ShotDirection;

	if (ViewTrace(Hit, ShotDirection))
	{
		AGun* HitActorGun = Cast<AGun>(Hit.GetActor());

		if (HitActorGun && !HitActorGun->GetGunIsOwned())
		{
			if (Gun == Weapon1)
			{       
				Weapon1 = HitActorGun;
				HitActorGun = Gun;

				Weapon1->GunIsOwned(true);
				Weapon1->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Weapon1->SetActorRelativeLocation({ 0,0,0 });
				Weapon1->SetActorRelativeRotation({ 0,0,0 });
				
				ChangeToWeapon1();

				HitActorGun->GunIsOwned(false);
				HitActorGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
				
				
			}

			else if (Gun == Weapon2)
			{
				Weapon2 = HitActorGun;
				HitActorGun = Gun;

				Weapon2->GunIsOwned(true);
				Weapon2->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Weapon2->SetActorRelativeLocation({ 0,0,0 });
				Weapon2->SetActorRelativeRotation({ 0,0,0 });
				
				ChangeToWeapon2();

				HitActorGun->GunIsOwned(false);
				HitActorGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			
			}
		}
	}
}

void AShooterCharacter::CharacterReloadGun()
{

	if (Gun && CurrentAmmoInInventory>0)
	{
		if (!Gun->GetbGunIsCurrentlyReloading())
		{
			if (Gun->ReturnCurrentAmmo() < Gun->GetMagSize())
			{
				

				int32 AmmoToReload = Gun->GetMagSize() - Gun->ReturnCurrentAmmo();
			//	CurrentAmmoInInventory = CurrentAmmoInInventory - AmmoToReload;

				if (Gun->GetAmmoType() == EAmmoType::AMMO_AR)
				{
					
				


				 if (CurrentAmmoInInventory_AR >= AmmoToReload)
				  {
                     CurrentAmmoInInventory_AR= CurrentAmmoInInventory_AR - AmmoToReload;

					 Gun->SetAmmoAvailableToReload(AmmoToReload);
				  }
				 else
				 { 
					 Gun->SetAmmoAvailableToReload(CurrentAmmoInInventory_AR); 
					 CurrentAmmoInInventory_AR = 0;
				 }

                  CurrentAmmoInInventory = CurrentAmmoInInventory_AR;
				}


				else if (Gun->GetAmmoType() == EAmmoType::AMMO_SMG)
				{
					

					if (CurrentAmmoInInventory_SMG >= AmmoToReload)
					{
						CurrentAmmoInInventory_SMG = CurrentAmmoInInventory_SMG - AmmoToReload;

						Gun->SetAmmoAvailableToReload(AmmoToReload);
					}
					else
					{
						Gun->SetAmmoAvailableToReload(CurrentAmmoInInventory_SMG);
						CurrentAmmoInInventory_SMG = 0;
					}

					CurrentAmmoInInventory = CurrentAmmoInInventory_SMG;


				}

			
              Gun->ReloadGun();
				
			}
		}
		


	}

}

void AShooterCharacter::AimDownSight()
{
	//UE_LOG(LogTemp, Warning, TEXT("Aim"));
	bAimDownSight = true;
	TargetDistanceReached = false;
	Gun->Aim();
//	CharacterSpringArm->TargetArmLength = 87;
//	CharacterSpringArm->SocketOffset.Y = -100;
}

void AShooterCharacter::ReleaseAimDownSight()
{
	//UE_LOG(LogTemp, Warning, TEXT("Release"));
	bAimDownSight = false;
	TargetDistanceReached = false;
	Gun->ReleaseAim();
	//CharacterSpringArm->TargetArmLength = 87;
	//CharacterSpringArm->SocketOffset.Y = -250;

}


void AShooterCharacter::EventDispatcherChangeCrosshairAccuracy()
{
	if(Gun)
	CrosshairAccuracy.Broadcast( Gun->GetShootingAccuracyLenght() );
	

}

void AShooterCharacter::SetDamageDealtandPosition(float Damage, FVector Position)
{
	DamageDealt=Damage;
	DamageWidgetPosition=Position;
//	UE_LOG(LogTemp, Warning, TEXT("Position:%s"), *DamageWidgetPosition.ToString());
}

bool AShooterCharacter::IsDead() const
{
	if (Health <= 0)
	{
        if(Gun)
        Gun->ReleaseTrigger();
		return true;
	}
	else return false;
	
	


}

float AShooterCharacter::GetHealthPrecent() const
{
	return Health/MaxHealth;
}

bool AShooterCharacter::ShootAnimation() const
{
	return true;
}

int32 AShooterCharacter::ReturnCurrentAmmoInInventory() const
{
	return CurrentAmmoInInventory;
}

bool AShooterCharacter::GetOpenMenu()
{
	return OpenMenu;
}

float AShooterCharacter::GetDamageDealt()
{
	return DamageDealt;
}

FVector AShooterCharacter::GetDamageWidgetPosition()
{
	return DamageWidgetPosition;
}



