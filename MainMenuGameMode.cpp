// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterSaveGameObject.h"

void AMainMenuGameMode::BeginPlay()
{

	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->SetActualGameMode(true, false, false);
		DisableInput(PlayerController);
	}
	Load();

}

int32 AMainMenuGameMode::GetTotalKills()
{

	return TotalKills;
}

void AMainMenuGameMode::Load()
{
	if (UShooterSaveGameObject* LoadedGame = Cast<UShooterSaveGameObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerKillCountSaveSlot"), 0)))
	{
		TotalKills = LoadedGame->AllTimeKillCount;
		//UE_LOG(LogTemp, Warning, TEXT("LOADED: %i"), LoadedGame->AllTimeKillCount);

	}
}