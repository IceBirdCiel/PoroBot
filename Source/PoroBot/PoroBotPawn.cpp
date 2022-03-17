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
bool APoroBotPawn::isStarted = false;
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
	if (isStarted) {
		Move(DeltaSeconds);
	}
}
void APoroBotPawn::startGame() {
	isStarted = !isStarted;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Start Game");
}

void APoroBotPawn::Move(float DeltaSeconds) {
	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	//const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	const FVector MoveDirection = GetActorForwardVector();

	// Calculate  movement
	Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// Get the location of the agent
	FVector AgentLocation = GetActorLocation();

	// Get the direction the agent is facing
	FVector Direction = GetActorForwardVector();
	FVector DirectionLeft = -GetActorRightVector();
	FVector DirectionRight = GetActorRightVector();

	FVector newLocation = AgentLocation + Direction * 400;
	FVector newLocationLeft = AgentLocation + DirectionLeft * 400;
	FVector newLocationRight = AgentLocation + DirectionRight * 400;

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
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(timeCoroutine));
		if (timeCoroutine < 0) {
			Movement = getMovement(HitActor, HitActorLeft, HitActorRight, DeltaSeconds, Movement);
			timeCoroutine = 0.5;
		}
		timeCoroutine -= DeltaSeconds;
		NewRotation = Movement.Rotation();
		RootComponent->MoveComponent(Movement, NewRotation, true, &HitM);
	}
}

FVector APoroBotPawn::getMovement(AActor* actorForward, AActor* actorLeft, AActor* actorRight, float DeltaSeconds, FVector initialMovement) {
	FVector Move = initialMovement;

	const FVector MoveDirectionLeft = -GetActorRightVector();
	const FVector MoveDirectionRight = GetActorRightVector();
	if(actorForward != NULL && actorLeft != NULL){
		Move = MoveDirectionRight * MoveSpeed * DeltaSeconds;
	}
	else if (actorForward != NULL && actorRight != NULL) {
		Move = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
	}
	else if (actorLeft != NULL && actorRight != NULL) {
		Move = initialMovement;
	}
	else if (actorRight != NULL) {
		float rand = FMath::FRandRange(0, 1);
		if (rand < 0.5) {
			Move = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
		}
		else {
			Move = initialMovement;
		}
	}
	else {
		float rand = FMath::FRandRange(0, 1);
		if (rand < 0.5) {
			Move = initialMovement;
		}
		else {
			Move = MoveDirectionRight * MoveSpeed * DeltaSeconds;
		}
	}
	return Move;
}
