// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MaxManaMMC.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMaxManaMMC::UMaxManaMMC()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
}

float UMaxManaMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	auto sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	auto targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evlautionParameters;
	evlautionParameters.SourceTags = sourceTags;
	evlautionParameters.TargetTags = targetTags;

	float intelligence;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, evlautionParameters, intelligence);

	auto combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	auto playerLevel = combatInterface->GetPlayerLevel();

	return 50.f + 2.5f * intelligence + 15.f * playerLevel;
}
