// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoroBotPawn.h"
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
const FName APoroBotPawn::Spawn("Fire");
bool APoroBotPawn::isStarted = false;
APoroBotPawn::APoroBotPawn()
{	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Game/TwinStick/Poro/PoroBot_Run.PoroBot_Run"));
	// Create the mesh component
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	MeshComponent->SetSkeletalMesh(Mesh.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Anim(TEXT("/Game/TwinStick/Poro/PoroBot_Run_Anim.PoroBot_Run_Anim"));
	MeshComponent->AnimationData.AnimToPlay = Anim.Object;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Game/TwinStick/Poro/Sphere.Sphere"));
    SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
    SphereComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    SphereComponent->SetStaticMesh(Sphere.Object);
    //RootComponent = SphereComponent;
	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Movement
	MoveSpeed = 1000.0f;
}

void APoroBotPawn::BeginPlay() {
    Super::BeginPlay();
    SetActorLocation(FVector(-1739,-1620, 270));
	
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//InputComponent->BindAction("MouseLeftClicked", IE_Pressed, this, &APoroBotPawn::MouseClick);
	SphereComponent->SetWorldLocation(this->GetActorLocation());
	APoroSnax::nbSnax = 0;
}

void APoroBotPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// set up gameplay key bindings

	PlayerInputComponent->BindAction(Spawn, IE_Pressed, this, &APoroBotPawn::MouseClick);
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
}

void APoroBotPawn::Tick(float DeltaSeconds)
{
	SphereComponent->SetWorldLocation(this->GetActorLocation());
	if (isStarted) {
		Move(DeltaSeconds);
	}
}

void APoroBotPawn::MouseClick() {
	if (APoroSnax::nbSnax < 3) {
		FHitResult hit;
		const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
		const float RightValue = GetInputAxisValue(MoveRightBinding);
		const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, hit); // Raycast de la souris sur le sol
		AActor* actor = hit.GetActor();
		if (actor != NULL && actor->GetName().Contains("Floor")) {
			FVector rotation = FVector();
			//Up
			if (MoveDirection.X > 0) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, "Up");
				rotation = FVector(0, 0, 0);
			}
			//Down
			if (MoveDirection.X < 0) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, "Down");
				rotation = FVector(0, 0, 180);
			}
			//Left
			if (MoveDirection.Y < 0) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, "Left");
				rotation = FVector(0, 0, -90);

			}
			if (MoveDirection.Y > 0) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, "Right");
				rotation = FVector(0, 0, 90);
			}
			APoroSnax* poroSnax = GetWorld()->SpawnActor<APoroSnax>(FVector(hit.Location.X, hit.Location.Y, hit.Location.Z + 100), FRotator()); // Spawn d'un porosnax
			
			FQuat baseRotation(FRotator::MakeFromEuler(rotation));
			poroSnax->SetActorRelativeRotation(baseRotation);
			poroSnax->InitValue();
			FQuat rotationSnax = FQuat::MakeFromEuler(FVector(rotation.X, rotation.Y, rotation.Z + 90));
			poroSnax->SetSnaxRotation(rotationSnax);
			FVector origin = poroSnax->GetComponentsBoundingBox().GetCenter();
			FVector extent = FVector(156, 40, 100);
			//poroSnax->GetCompo() = poroSnax->GetComponentsBoundingBox().BuildAABB(origin, extent);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, "x: " + FString::SanitizeFloat(ForwardValue) + "; y:" + FString::SanitizeFloat(RightValue));
		}
		APoroSnax::nbSnax += 1;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, "Plus de PoroSnax Disponibles");
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

	FVector newLocation = AgentLocation + Direction * 150;
	FVector newLocationLeft = AgentLocation + DirectionLeft * 300;
	FVector newLocationRight = AgentLocation + DirectionRight * 300;

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
		TArray<FHitResult> Arr_HitForward;
		TArray<FHitResult> Arr_HitLeft;
		TArray<FHitResult> Arr_HitRight;
		UWorld* World = GetWorld();
		DrawDebugLine(World, AgentLocation, newLocation, FColor::Yellow, false, 0, 0, 10);
		DrawDebugLine(World, AgentLocation, newLocationLeft, FColor::Red, false, 0, 0, 10);
		DrawDebugLine(World, AgentLocation, newLocationRight, FColor::Green, false, 0, 0, 10);
		GetWorld()->LineTraceSingleByChannel(HitForward, AgentLocation, newLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Left
		GetWorld()->LineTraceSingleByChannel(HitLeft, AgentLocation, newLocationLeft, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Right
		GetWorld()->LineTraceSingleByChannel(HitRight, AgentLocation, newLocationRight, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);

		/*//Raycast Forward
		GetWorld()->LineTraceMultiByChannel(Arr_HitForward, AgentLocation, newLocation, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Left
		GetWorld()->LineTraceMultiByChannel(Arr_HitLeft, AgentLocation, newLocationLeft, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		//Raycast Right
		GetWorld()->LineTraceMultiByChannel(Arr_HitRight, AgentLocation, newLocationRight, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		
		for ( FHitResult Hit : Arr_HitForward)
		{
			AActor* hitActor = Hit.GetActor();
			if (hitActor != NULL && hitActor->GetName().Contains("Obstacle")) {
				HitForward = Hit;
			}
		}
		for  (FHitResult Hit : Arr_HitLeft)
		{
			AActor* hitActor = Hit.GetActor();
			if (hitActor != NULL && hitActor->GetName().Contains("Obstacle")) {
				HitLeft = Hit;
			}
		}
		for  (FHitResult Hit : Arr_HitRight)
		{
			AActor* hitActor = Hit.GetActor();
			if (hitActor != NULL && hitActor->GetName().Contains("Obstacle")) {
				HitRight = Hit;
			}
		}
		*/
		AActor* HitActor = HitForward.GetActor();
		AActor* HitActorLeft = HitLeft.GetActor();
		AActor* HitActorRight = HitRight.GetActor();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(timeCoroutine));
		if (timeCoroutine < 0 || (HitActor != NULL && HitActor->GetName().Contains("Obstacle"))) {
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
	// Devant et Gauche = Obstacle -> Direction Droite
	/*
	if(actorForward != NULL && actorLeft != NULL && actorForward->GetName().Contains("Obstacle")){
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, actorForward->GetName());
		Move = MoveDirectionRight * MoveSpeed * DeltaSeconds;
	}
	else if (actorForward != NULL && actorRight != NULL && actorRight->GetName().Contains("Obstacle")) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, actorRight->GetName());
		Move = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
	}
	else if (actorLeft != NULL && actorRight != NULL && actorLeft->GetName().Contains("Obstacle")) {
        GEngine->AddOnScreenDebugMessage(-1,0.5f, FColor::Blue, actorLeft->GetName());
		Move = initialMovement;
	}
	else if (actorRight != NULL && actorRight->GetName().Contains("Obstacle")) {
        GEngine->AddOnScreenDebugMessage(-1,0.5f, FColor::Magenta, actorRight->GetName());
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
	}*/

	if (actorForward != NULL && actorForward->GetName().Contains("Obstacle")) {
		if (actorLeft != NULL && actorLeft->GetName().Contains("Obstacle")) {
			if (actorRight == NULL) {
				Move = MoveDirectionRight * MoveSpeed * DeltaSeconds;
			}
			else {
				Move = -Move;
			}
		}
		else {
			Move = MoveDirectionLeft * MoveSpeed * DeltaSeconds;
		}
	}
	return Move;
}
