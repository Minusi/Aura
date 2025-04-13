// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	auto abilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (abilitySystemInterface == nullptr)
	{
		return;
	}

	auto abilitySystemComponent = abilitySystemInterface->GetAbilitySystemComponent();
	if (abilitySystemComponent == nullptr)
	{
		return;
	}

	// todo change this to apply a gameplay effect. For now, we will just set the health directly.
	auto auraAttributeSet = const_cast<UAuraAttributeSet*>(Cast<UAuraAttributeSet>(abilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass())));
	auraAttributeSet->SetHealth(auraAttributeSet->GetHealth() + 25.0f);
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}
