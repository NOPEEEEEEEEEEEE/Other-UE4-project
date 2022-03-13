// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTaskNode_ReleaseTrigger.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTaskNode_ReleaseTrigger::UBTTaskNode_ReleaseTrigger()
{

	NodeName = "Release";
}

EBTNodeResult::Type UBTTaskNode_ReleaseTrigger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)return EBTNodeResult::Failed;

	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)return EBTNodeResult::Failed;

	Character->Release();

	return EBTNodeResult::Succeeded;
}