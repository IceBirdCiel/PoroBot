// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

AMyHUD::AMyHUD() {

}
void AMyHUD::BeginPlay() {
	Super::BeginPlay();
	if (ComboWidgetClass) {
		ComboWidget = CreateWidget<UComboWidget>(GetWorld(), ComboWidgetClass);
		if (ComboWidget) {
			ComboWidget->AddToViewport();
		}
	}
}

void AMyHUD::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	ComboWidget->UpdateComboCount(GetWorld()->GetTimeSeconds());
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
