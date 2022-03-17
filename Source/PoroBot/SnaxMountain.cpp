// Fill out your copyright notice in the Description page of Project Settings.


#include "SnaxMountain.h"

ASnaxMountain::ASnaxMountain() {
    OnActorBeginOverlap.AddDynamic(this, &ASnaxMountain::OverlapBegin);
}

void ASnaxMountain::BeginPlay() {
    Super::BeginPlay();
    UComboWidget::win = false;
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Red, true, -1, 0, 5);
}

void ASnaxMountain::OverlapBegin(AActor* OverlappedComponent, AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PLEINS DE POROSNAX");
        UComboWidget::win = true;
    }
}
