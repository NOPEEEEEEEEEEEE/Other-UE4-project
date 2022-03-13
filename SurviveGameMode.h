// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "CharacterSpawner.h"
#include "SurviveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASurviveGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled)override;
	UFUNCTION(BlueprintCallable)
	int32 GetWave();
	UFUNCTION(BlueprintCallable)
		void SetDifficulty(int32 Difficulty);

	UFUNCTION(BlueprintImplementableEvent)
		void WaveChange();
	
protected:
	virtual void BeginPlay() override;
private:

	void EndGame(bool bIsPlayerWinner);
	
	void NextWave();

	UPROPERTY()
		int32 Wave=1;
	UPROPERTY()
		int32 DifficultyIndex =2; //1=easy  
	                              //2=Medium
	                              //3=Hard
	                              //4=Impossible
	 
	UPROPERTY(EditAnywhere)
		float WaveBeginTime=5;

	FTimerHandle WaveBeginTimer;

};
