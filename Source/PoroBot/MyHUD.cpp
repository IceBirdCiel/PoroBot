// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "PoroBotPawn.h"

AMyHUD::AMyHUD() {

}
void AMyHUD::BeginPlay() {
	Super::BeginPlay();
	APoroBotPawn::isStarted = false;
	UComboWidget::win = false;
	if (ComboWidgetClass) {
		ComboWidget = CreateWidget<UComboWidget>(GetWorld(), ComboWidgetClass);
		if (ComboWidget) {
			ComboWidget->AddToViewport();
		}
	}
}

void AMyHUD::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	ComboWidget->UpdateComboCount(DeltaSeconds * 1.2);
	
}

void AMyHUD::DrawHUD() {
	//Super::DrawHUD();
}

void AMyHUD::UpdateComboCount(int32 Value) {
	if (ComboWidget) {
		ComboWidget->UpdateComboCount(Value);
	}
}

void AMyHUD::ResetCombo() {
	if (ComboWidget) {
		ComboWidget->ResetCombo();
	}
}
