// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
#pragma region WizardStats
	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Wisdom;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Intelligence;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Combat;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Agility;
#pragma endregion

public:
	FORCEINLINE void SetWisdom(int32 WisdomToSet) { Wisdom = WisdomToSet; }
	FORCEINLINE void SetIntelligence(int32 IntelligenceToSet) { Intelligence = IntelligenceToSet; }
	FORCEINLINE void SetCombat(int32 CombatToSet) { Combat = CombatToSet; }
	FORCEINLINE void SetAgility(int32 AgilityToSet) { Agility = AgilityToSet; }
};
