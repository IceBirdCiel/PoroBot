// Fill out your copyright notice in the Description page of Project Settings.


#include "PoroSnax.h"
#include "PoroBotPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"

APoroSnax::APoroSnax() {
    OnActorBeginOverlap.AddDynamic(this, &APoroSnax::OverlapBegin);
    SetActorHiddenInGame(false); 
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TwinStick/Poro/PoroSnax.PoroSnax"));
    SnaxComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SnaxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    SnaxComponent->SetStaticMesh(Mesh.Object);
    SnaxComponent->SetVisibility(true);
}

void APoroSnax::BeginPlay(){
    Super::BeginPlay();
    SnaxComponent->SetRelativeLocation(FVector(0, 0, 0));
    FQuat rotation(FRotator::MakeFromEuler(FVector(0, 0, 90)));
    SnaxComponent->SetRelativeRotation(rotation);
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

void APoroSnax::OverlapBegin(AActor *OverlappedComponent, AActor *OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "POROOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");

        OtherActor->SetActorRotation(this->GetActorRotation());
    }
}