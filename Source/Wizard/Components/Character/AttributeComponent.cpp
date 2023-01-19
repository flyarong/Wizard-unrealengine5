// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent, Name);
	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, Power);
	DOREPLIFETIME(UAttributeComponent, Wisdom);
	DOREPLIFETIME(UAttributeComponent, Intelligence);
	DOREPLIFETIME(UAttributeComponent, Combat);
	DOREPLIFETIME(UAttributeComponent, Agility);
}