// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboWidget.h"

UComboWidget::UComboWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

}

void UComboWidget::NativeConstruct() {
	//Super::NativeConstruct();
}

void UComboWidget::UpdateComboCount(float Value) {
	if (TXTCombo && Value > 1) {
		if (TXTCombo->Visibility == ESlateVisibility::Hidden) {
			TXTCombo->SetVisibility(ESlateVisibility::Visible);
		}
	}
	time = Value;
	TXTCombo->SetText(FText::FromString("Time : " + FString::FromInt((int32)time)));
}

void UComboWidget::ResetCombo() {
	if(TXTCombo)
		TXTCombo->SetVisibility(ESlateVisibility::Hidden);
}

