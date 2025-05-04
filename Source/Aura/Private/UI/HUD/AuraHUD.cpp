// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/Controller/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindDependencyCallbacks();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not set, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not set, please fill out BP_AuraHUD"));

	auto widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(widget);
	if (OverlayWidget == nullptr)
	{
		return;
	}

	FWidgetControllerParams params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	auto widgetController = GetOverlayWidgetController(params);
	if (widgetController == nullptr)
	{
		return;
	}

	OverlayWidget->SetWidgetController(widgetController);
	widgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}