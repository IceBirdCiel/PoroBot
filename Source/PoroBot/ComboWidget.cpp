// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboWidget.h"
#include "PoroBotPawn.h"


bool UComboWidget::win = false;
void UComboWidget::startGame() {
	APoroBotPawn::startGame();
	UGameplayStatics::SetGamePaused(GetWorld(), !APoroBotPawn::isStarted);
}
UComboWidget::UComboWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	maxTime = 50;
	
	
}

void UComboWidget::NativeConstruct() {
	Super::NativeConstruct();
	StartGame->OnClicked.AddDynamic(this, &UComboWidget::startGame);
	
}

void UComboWidget::UpdateComboCount(float Value) {
	if (TXTCombo && Value > 1) {
		if (TXTCombo->Visibility == ESlateVisibility::Hidden) {
			TXTCombo->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (APoroBotPawn::isStarted) {
		time = Value;
		float percent = 1 - time / maxTime;
		TXTCombo->SetText(FText::FromString("Time : " + FString::FromInt((int32)time) + " s"));
		HPBar->SetPercent(percent);
		if (percent < 0) {
			RIPPoro();
		}
	}

	if (win) {
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, "win");
		WinPoro();
	}

	
}
void UComboWidget::RIPPoro() {
	TXTDefeat->SetText(FText::FromString("Le Poro est mort de faim"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	APoroBotPawn::isStarted = false;
}

void UComboWidget::WinPoro() {
	TXTWin->SetText(FText::FromString("Poro est content,\nil mange des PoroSnax !"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	APoroBotPawn::isStarted = false;
}

void UComboWidget::ResetCombo() {
	if(TXTCombo)
		TXTCombo->SetVisibility(ESlateVisibility::Hidden);
}

