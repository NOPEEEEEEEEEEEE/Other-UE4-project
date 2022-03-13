// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IfPlayerSeen.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_IfPlayerSeen::UBTService_IfPlayerSeen()
{
	NodeName = "Is player Seen ";

}

void  UBTService_IfPlayerSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr)return;

	if (OwnerComp.GetAIOwner() == nullptr)return;
    OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
	{

		

	}

}