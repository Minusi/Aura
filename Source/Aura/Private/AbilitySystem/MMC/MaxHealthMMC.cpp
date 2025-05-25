// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MaxHealthMMC.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMaxHealthMMC::UMaxHealthMMC()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMaxHealthMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	auto sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	auto targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evlautionParameters;
	evlautionParameters.SourceTags = sourceTags;
	evlautionParameters.TargetTags = targetTags;

	float vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, evlautionParameters, vigor);
	vigor = FMath::Max(vigor, 0.f);

	auto combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	auto playerLevel = combatInterface->GetPlayerLevel();

	return 80.f + 2.5f * vigor + 10.f * playerLevel;
}
