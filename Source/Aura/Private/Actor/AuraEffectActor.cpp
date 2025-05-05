// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectTypes.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	auto sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(sceneComponent);
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (IsValid(targetASC) == false)
	{
		return;
	}

	check(GameplayEffectClass);
	auto effectContextHandle = targetASC->MakeEffectContext();
	effectContextHandle.AddSourceObject(this);
	const auto effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel
		, effectContextHandle);
	const auto activeEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());

	if (effectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite
		&& InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(activeEffectHandle, targetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		auto targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(targetASC) == false)
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> handlesToRemove;
		for (auto& activeEffectHandle : ActiveEffectHandles)
		{
			if (activeEffectHandle.Value == targetASC)
			{
				targetASC->RemoveActiveGameplayEffect(activeEffectHandle.Key, 1);
				handlesToRemove.Add(activeEffectHandle.Key);
			}
		}

		for (auto& handle : handlesToRemove)
		{
			ActiveEffectHandles.Remove(handle);
		}
	}
}
