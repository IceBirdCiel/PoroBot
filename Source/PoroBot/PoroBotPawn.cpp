// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoroBotPawn.h"
#include "PoroBotProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "DrawDebugHelpers.h"

const FName APoroBotPawn::MoveForwardBinding("MoveForward");
const FName APoroBotPawn::MoveRightBinding("MoveRight");

APoroBotPawn::APoroBotPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

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
	const FVector MoveDirectionLeft = -GetActorRightVector();
	const FVector MoveDirectionRight = GetActorRightVector();

	// Calculate  movement
	FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;
	
	// Get the location of the agent
	FVector AgentLocation = GetActorLocation();
	
	// Get the direction the agent is facing
	FVector Direction = GetActorForwardVector();
	GetActorRightVector();

	FVector newLocation = AgentLocation + Direction * 100;

	// Default trace params
	FCollisionQueryParams TraceParams(TEXT("LineOfSight_Trace"), false, this);


	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FRotator NewRotation = Movement.Rotation();
		FHitResult HitM(1.f);
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &HitM);
		UWorld* World = GetWorld();
		DrawDebugLine(World, AgentLocation, newLocation, FColor::Yellow, true, 0, 0, 10);

		GetWorld()->LineTraceSingleByChannel(Hit, AgentLocation, newLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != NULL)
		{
			Movement = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
			NewRotation = Movement.Rotation();
			RootComponent->MoveComponent(Movement, NewRotation, true, &HitM);
		}
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
}
