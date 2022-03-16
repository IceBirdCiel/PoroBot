// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RunTime/UMG/Public/UMG.h"

#include "ComboWidget.generated.h"

/**
 *
 */
UCLASS()
class POROBOT_API UComboWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UComboWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	void UpdateComboCount(float Value);

	void ResetCombo();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTCombo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		float time;
};
