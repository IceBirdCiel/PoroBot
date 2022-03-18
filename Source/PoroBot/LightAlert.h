// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "LightAlert.generated.h"

UCLASS()
class POROBOT_API ALightAlert : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Light")
	class UPointLightComponent* light;

public:	
	// Sets default values for this actor's properties
	ALightAlert();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
