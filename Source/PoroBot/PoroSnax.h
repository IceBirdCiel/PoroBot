// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PoroSnax.generated.h"

//class UStaticMeshComponent;

UCLASS()

class POROBOT_API APoroSnax : public ATriggerBox
{
	GENERATED_BODY()
    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* SnaxComponent;

protected:
    virtual void BeginPlay() override;

public:
	APoroSnax();

    UFUNCTION()
    void OverlapBegin(class AActor* OverlappedComp, class AActor* OtherActor);//, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
