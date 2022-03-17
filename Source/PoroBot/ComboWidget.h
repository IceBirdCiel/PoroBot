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

	void RIPPoro();

	void WinPoro();
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTCombo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTDefeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TXTWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* StartGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		float time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		float maxTime;
	UFUNCTION(meta = (BindWidget))
		void startGame();

};
