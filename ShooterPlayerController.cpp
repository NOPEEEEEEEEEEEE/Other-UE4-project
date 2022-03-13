// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterSaveGameObject.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
    HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
	Load();
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner )
{
	Super::GameHasEnded(EndGameFocus,bIsWinner);
	HUD->RemoveFromViewport();
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();

		}

	}
	else
	{
       UUserWidget*LoseScreen=CreateWidget(this, LoseScreenClass);
	   if (LoseScreen != nullptr)
	   {
		LoseScreen->AddToViewport();
	   }

	}
	Save();
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel,RestartDelay);

}

void AShooterPlayerController::Save()
{
	if (UShooterSaveGameObject* SaveGameInstance = Cast<UShooterSaveGameObject>(UGameplayStatics::CreateSaveGameObject(UShooterSaveGameObject::StaticClass())))
	{
		
       
		SaveGameInstance->AllTimeKillCount = TotalKillsEver + KillCount ;


		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerKillCountSaveSlot"), 0))
		{
			
			UE_LOG(LogTemp, Error, TEXT("Kills: %i"),KillCount);
			UE_LOG(LogTemp, Error, TEXT("Save succeeded"));
			
			
		}

	}


}

void AShooterPlayerController::Load()
{
	if (UShooterSaveGameObject* LoadedGame = Cast<UShooterSaveGameObject>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerKillCountSaveSlot"), 0)))
	{
		TotalKillsEver = LoadedGame->AllTimeKillCount;
		UE_LOG(LogTemp, Warning, TEXT("LOADED: %i"), LoadedGame->AllTimeKillCount);

	}
}



void AShooterPlayerController::CallGameHasEnded()
{
	GameHasEnded(this,false);
}

void AShooterPlayerController::SetActualGameMode(bool MainMenu, bool KillEmAll, bool Survive)
{
	 bMainMenu = MainMenu;
	 bKillEmAll =KillEmAll;
	 bSurvive=Survive;
}

bool AShooterPlayerController::GetbMainMenu()
{
	return bMainMenu;
}

bool AShooterPlayerController::GetbKillEmAll()
{
	return bKillEmAll;
}

bool AShooterPlayerController::GetbSurvive()
{
	return bSurvive;
}

int32 AShooterPlayerController::GetKillCount()
{
	return KillCount;
}

void AShooterPlayerController::IncreaseKillCount()
{
	KillCount++;
	
}

void AShooterPlayerController::OpenGameMenu()
{
	Load();
	//UE_LOG(LogTemp, Error, TEXT("GameMenu"));

}
