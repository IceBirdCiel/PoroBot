// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboWidget.h"
#include "Engine/TriggerBox.h"
#include "SnaxMountain.generated.h"

/**
 *
 */
UCLASS()
class POROBOT_API ASnaxMountain : public ATriggerBox
{
	GENERATED_BODY()
		ASnaxMountain();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapBegin(class AActor* OverlappedComp, class AActor* OtherActor);
	class UComboWidget* comboWidget;
};
