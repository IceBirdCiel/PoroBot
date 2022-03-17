// Fill out your copyright notice in the Description page of Project Settings.


#include "Snax.h"
#include "PoroBotPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "Components/BoxComponent.h"

// Sets default values
ASnax::ASnax()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/TwinStick/Poro/PoroSnax.PoroSnax"));

	PoroSnaxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoroSnaxMesh"));
	PoroSnaxMesh->SetStaticMesh(MeshAsset.Object);
	PoroSnaxMesh->SetupAttachment(RootComponent);
	PoroSnaxMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	/*Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollider"));
	Trigger->SetGenerateOverlapEvents(true);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	 */
    OnActorBeginOverlap.AddDynamic(this, &ASnax::OnOverlapBegin);
}

void ASnax::OnOverlapBegin(class AActor* OverlappedActor, class AActor* otherActor){
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Contains Poro");
}