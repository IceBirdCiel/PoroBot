// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAlert.h"
#include "ComboWidget.h"
#include "Components/PointLightComponent.h"

// Sets default values
ALightAlert::ALightAlert()
{
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	light->SetLightColor(FColor::Red);
	light->Intensity = 0;
	RootComponent = light;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void ALightAlert::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UComboWidget::externTime > 30) {

		light->SetIntensity(100000);
	}
}

