// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoroBotPawn.h"
#include "PoroBotProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

const FName APoroBotPawn::MoveForwardBinding("MoveForward");
const FName APoroBotPawn::MoveRightBinding("MoveRight");

APoroBotPawn::APoroBotPawn()
{	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Game/TwinStick/Poro/PoroBot_Run.PoroBot_Run"));
	// Create the mesh component
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	MeshComponent->SetSkeletalMesh(Mesh.Object);

	// Movement
	MoveSpeed = 1000.0f;
}

void APoroBotPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
}

void APoroBotPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector MoveDirection = GetActorForwardVector();

	// Calculate  movement
	FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;
	
	// Get the location of the agent
	FVector AgentLocation = GetActorLocation();
	
	// Get the direction the agent is facing
	FVector Direction = GetActorForwardVector();
	FVector DirectionLeft = -GetActorRightVector();
	FVector DirectionRight = GetActorRightVector();

	FVector newLocation = AgentLocation + Direction * 200;
	FVector newLocationLeft = AgentLocation + DirectionLeft * 200;
	FVector newLocationRight = AgentLocation + DirectionRight * 200;

	// Default trace params
	FCollisionQueryParams TraceParams(TEXT("LineOfSight_Trace"), false, this);


	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FRotator NewRotation = Movement.Rotation();
		FHitResult HitM(1.f);
		FHitResult HitForward(1.f);
		FHitResult HitLeft(1.f);
		FHitResult HitRight(1.f);
		UWorld* World = GetWorld();
		DrawDebugLine(World, AgentLocation, newLocation, FColor::Yellow, false, 0, 0, 10);
		DrawDebugLine(World, AgentLocation, newLocationLeft, FColor::Red, false, 0, 0, 10);
		DrawDebugLine(World, AgentLocation, newLocationRight, FColor::Green, false, 0, 0, 10);
		//Raycast Forward
		GetWorld()->LineTraceSingleByChannel(HitForward, AgentLocation, newLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Left
		GetWorld()->LineTraceSingleByChannel(HitLeft, AgentLocation, newLocationLeft, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Right
		GetWorld()->LineTraceSingleByChannel(HitRight, AgentLocation, newLocationRight, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		AActor* HitActor = HitForward.GetActor();
		AActor* HitActorLeft = HitLeft.GetActor();
		AActor* HitActorRight = HitRight.GetActor();
		Movement = getMovement(HitActor, HitActorLeft, HitActorRight, DeltaSeconds, Movement);
		NewRotation = Movement.Rotation();
		RootComponent->MoveComponent(Movement, NewRotation, true, &HitM);
	}
}

FVector APoroBotPawn::getMovement(AActor* actorForward, AActor* actorLeft, AActor* actorRight, float DeltaSeconds, FVector initialMovement) {
	FVector Movement = initialMovement;

	const FVector MoveDirectionLeft = -GetActorRightVector();
	const FVector MoveDirectionRight = GetActorRightVector();
	
	if (actorForward != NULL)
	{
		
		if (actorLeft != NULL) {
			Movement = MoveDirectionRight * MoveSpeed * DeltaSeconds;
		}
		else if (actorRight != NULL) {
			Movement = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
		}
		else {
			float rand = FMath::FRandRange(0, 1);
			if (rand < 0.5) {
				Movement = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
			}
			else {
				Movement = MoveDirectionRight * MoveSpeed * DeltaSeconds;
			}

		}
		

	}
	return Movement;
}
