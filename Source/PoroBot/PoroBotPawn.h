// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PoroBotPawn.generated.h"

UCLASS(Blueprintable)
class APoroBotPawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshComponent;

public:
	APoroBotPawn();

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	FVector getMovement(AActor* actorForward, AActor* actorLeft, AActor* actorRight, float DeltaSeconds, FVector initialMovement);

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	FVector Movement;
	float timeCoroutine = 1;

private:

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

public:
	/** Returns MeshComponent subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMeshComponent() const { return MeshComponent; }
};

