// Fill out your copyright notice in the Description page of Project Settings.


#include "SurviveGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterPlayerController.h"
#include "TimerManager.h"



void ASurviveGameMode::BeginPlay()
{
	Wave = 1;
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->SetActualGameMode(false, false, true);
	}
	WaveChange();
	NextWave();
}

void ASurviveGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerController != nullptr)
	{
		EndGame(false);
	}
	for (AShooterAIController* AIController : TActorRange< AShooterAIController>( GetWorld() ) )
	{
		if (!AIController->IsDead()) return;
	}
	
	GetWorldTimerManager().SetTimer(WaveBeginTimer, this, &ASurviveGameMode::NextWave, WaveBeginTime);
	
}

void ASurviveGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}

}

void ASurviveGameMode::NextWave()
{
	if(Wave!=1)WaveChange();
	for (ACharacterSpawner* Spawner : TActorRange< ACharacterSpawner>(GetWorld()))
		{
		
		Spawner->CallCharacterSpawn(Wave, DifficultyIndex);

		}

	UE_LOG(LogTemp, Warning, TEXT("Wave: %i"), Wave);
Wave++;

}
int32 ASurviveGameMode::GetWave()
{
	return Wave;
}

void ASurviveGameMode::SetDifficulty(int32 Difficulty)
{
	DifficultyIndex = Difficulty;
  UE_LOG(LogTemp, Error, TEXT("Difficulty: %i"), Difficulty);
}



