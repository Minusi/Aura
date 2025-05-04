// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Aura|Widget")
	void SetWidgetController(UObject* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura|Widget")
	void OnWidgetControllerSet();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Aura|Widget", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UObject> WidgetController;
};
