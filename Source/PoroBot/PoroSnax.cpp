// Fill out your copyright notice in the Description page of Project Settings.


#include "PoroSnax.h"
#include "PoroBotPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"
int32 APoroSnax::nbSnax = 0;
APoroSnax::APoroSnax() {
    OnActorBeginOverlap.AddDynamic(this, &APoroSnax::OverlapBegin);
    SetActorHiddenInGame(false); 
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/TwinStick/Poro/PoroSnax.PoroSnax"));
    SnaxComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SnaxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    SnaxComponent->SetStaticMesh(Mesh.Object);
    SnaxComponent->SetVisibility(true);
    //SnaxComponent->SetRelativeRotation(FQuat(0, 0, 90, 0));
}

void APoroSnax::BeginPlay(){
    Super::BeginPlay();
    APoroSnax::InitValue();
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

void APoroSnax::InitValue() {
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Init Value");
    SnaxComponent->SetWorldLocation(this->GetActorLocation());
}

void APoroSnax::SetSnaxRotation(FQuat quat) {
    SnaxComponent->SetRelativeRotation(quat);
}

void APoroSnax::OverlapBegin(AActor *OverlappedComponent, AActor *OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "POROOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");

        OtherActor->SetActorRotation(this->GetActorRotation());
    }
}