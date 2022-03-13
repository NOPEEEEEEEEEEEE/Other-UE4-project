// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"
#include "CharacterSpawner.generated.h"

UCLASS()
class SIMPLESHOOTER_API ACharacterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterCharacter>CharacterToSpawn;


	UFUNCTION(BlueprintCallable)
		void Spawn();

	UFUNCTION(BlueprintCallable)
		void CallCharacterSpawn(int32 LevelIndex, int32 DifficultyIndex);
private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY()
		AShooterCharacter* ShooterAI;
	UPROPERTY()
	float SpawnTime;
	UPROPERTY()
	int32 UnitsToSpawn;
	UPROPERTY()
	FTimerHandle SpawnTimer;

	int32 LowGrowthFunction(int32 x);
	int32 MediumGrowthFunction(int32 x);
	int32 FastGrowthFunction(int32 x);
	int32 FastAsFuckBoiGrowthFunction(int32 x);

};
