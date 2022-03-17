// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PoroBotPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "Snax.generated.h"

UCLASS()
class POROBOT_API ASnax : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PoroSnax, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PoroSnaxMesh;
	
public:	
	// Sets default values for this actor's properties
	ASnax();

	FORCEINLINE UStaticMeshComponent* GetMesh() const { return PoroSnaxMesh; }
    UFUNCTION()
    void OnOverlapBegin(class AActor* OverlappedActor, class AActor* otherActor);
};
