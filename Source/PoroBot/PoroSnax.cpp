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
}

void APoroSnax::BeginPlay(){
    Super::BeginPlay();
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

void APoroSnax::OverlapBegin(AActor *OverlappedComponent, AActor *OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "POROOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");

        OtherActor->SetActorRotation(this->GetActorRotation());
    }
}