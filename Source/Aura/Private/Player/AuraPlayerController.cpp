// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "InputMappingContext.h"
#include "Interaction/EnemyInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TraceCursor();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	auto enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (enhancedInputSubsystem)
	{
		enhancedInputSubsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	auto enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	auto inputAxisVector = Value.Get<FVector2D>();
	auto rotation = GetControlRotation();
	auto yawRotation = FRotator(0.f, rotation.Yaw, 0.f);

	auto forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	auto rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	auto controlledPawn = GetPawn<APawn>();
	if (controlledPawn == nullptr)
	{
		return;
	}

	controlledPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
	controlledPawn->AddMovementInput(rightDirection, inputAxisVector.X);
}

void AAuraPlayerController::TraceCursor()
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
	if (hitResult.bBlockingHit == false)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = hitResult.GetActor();

	if (LastActor.GetInterface())
	{ 
		if (CurrentActor != LastActor)
		{
			LastActor->UnhighlightActor();
		}
	}
	if (CurrentActor.GetInterface())
	{
		if (CurrentActor != LastActor)
		{
			CurrentActor->HighlightActor();
		}
	}
}
