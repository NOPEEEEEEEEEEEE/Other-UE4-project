// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSpawner.h"
#include "GameFramework/Controller.h"
#include "ShooterCharacter.h"
#include "TimerManager.h"
// Sets default values
ACharacterSpawner::ACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);


}

// Called when the game starts or when spawned
void ACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
	UnitsToSpawn = 0;
	//CallCharacterSpawn();

}

// Called every frame
void ACharacterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  

}

void ACharacterSpawner::Spawn()
{
	

	if (CharacterToSpawn)
	{
		UWorld* World = GetWorld();
		UE_LOG(LogTemp, Error, TEXT("%s"), *CharacterToSpawn->GetName());
		if (World)
		{
		   
			
			FActorSpawnParameters spawnParams;
			FRotator Rotation = {0,0,0};
			FVector SpawnLocation =this->GetActorLocation();
          spawnParams.Owner = this;
	
			if(CharacterToSpawn)
				World->SpawnActor<AShooterCharacter>(CharacterToSpawn, SpawnLocation, Rotation);
          
			UnitsToSpawn--;
		}

	}
if (UnitsToSpawn == 0)
		GetWorldTimerManager().ClearTimer(SpawnTimer);


}

void ACharacterSpawner::CallCharacterSpawn(int32 LevelIndex,int32 DifficultyIndex)
{
	if (DifficultyIndex == 1)
		UnitsToSpawn = LowGrowthFunction(LevelIndex);

	else if (DifficultyIndex == 2)
		UnitsToSpawn = MediumGrowthFunction(LevelIndex);

	else if (DifficultyIndex == 3)
		UnitsToSpawn = FastGrowthFunction(LevelIndex);

	else if (DifficultyIndex == 4)
		UnitsToSpawn = FastAsFuckBoiGrowthFunction(LevelIndex);
	
	
	SpawnTime = LevelIndex;

    GetWorldTimerManager().SetTimer(SpawnTimer, this, &ACharacterSpawner::Spawn, SpawnTime,true,0.f);
	
	
		
}

int32 ACharacterSpawner::LowGrowthFunction(int32 x)
{
	int32 Units = 2 * x / 5;
	if (Units == 0)return 1;
	else
	return Units;
}

int32 ACharacterSpawner::MediumGrowthFunction(int32 x)
{
	int32 Units =  x / 2;
	if (Units == 0)return 1;
	else
		return Units;
}

int32 ACharacterSpawner::FastGrowthFunction(int32 x)
{

	int32 Units = 3 * x / 4;
	if (Units == 0)return 1;
	else
		return Units;
}

int32 ACharacterSpawner::FastAsFuckBoiGrowthFunction(int32 x)
{
	int32 Units =  x ;
	if (Units == 0)return 1;
	else
		return Units;
}



