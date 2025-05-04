// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/AuraWidgetController.h"

FWidgetControllerParams::FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
	: PlayerController(InPlayerController)
	, PlayerState(InPlayerState)
	, AbilitySystemComponent(InAbilitySystemComponent)
	, AttributeSet(InAttributeSet)
{
}

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetConrollerParams)
{
	PlayerController = InWidgetConrollerParams.PlayerController;
	PlayerState = InWidgetConrollerParams.PlayerState;
	AbilitySystemComponent = InWidgetConrollerParams.AbilitySystemComponent;
	AttributeSet = InWidgetConrollerParams.AttributeSet;
}
