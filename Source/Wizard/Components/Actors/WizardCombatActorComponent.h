// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "WizardCombatActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UWizardCombatActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWizardCombatActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function to setup this component's Owner and
	/// event bindings
	/// </summary>
	/// <param name="OwningActor">Owning Actor</param>
	/// <param name="Sphere">Owning Actor's AreaSphere</param>
	void SetupComponent(class AActor* OwningActor, class USphereComponent* Sphere);

	/// <summary>
	/// Function to damage the Actor after Combat
	/// </summary>
	void ReceiveDamage(int32 Damage);

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// This component's Owning Actor
	/// </summary>
	UPROPERTY()
	AActor* Owner;

	/// <summary>
	/// Item classes that can be spawned and picked up
	/// after this Actor is destroyed
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickups")
	TArray<TSubclassOf<class AItem>> PickupClasses;

	/// <summary>
	/// Variable to determine how many Items
	/// should be spawned after this Actor
	/// is destroyed
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickups")
	int32 NumOfSelections = 1;

	/// <summary>
	/// Cost of the Actor interaction
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Cost")
	float Cost = 25.f;

	/// <summary>
	/// Actor's Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Health = 1;

	/// <summary>
	/// Actor's Combat Type
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	ECombat CombatType;

#pragma region Callbacks
	/// <summary>
	/// Callback function to spawn Item(s)
	/// after this Actor is destroyed
	/// </summary>
	UFUNCTION()
	void SpawnPickupItem(AActor* DestroyedActor);

	UFUNCTION()
	void OnActorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnActorEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

	// <summary>
	/// Boolean for whether the Actor
	/// can be interacted with or it is already
	/// being interacted with by another Player
	/// </summary>
	UPROPERTY(Replicated)
	bool bCanInteract = true;		

public:
	FORCEINLINE void SetCanInteract(bool bIsInteractable) { bCanInteract = bIsInteractable; }
	FORCEINLINE bool GetCanInteract() const { return bCanInteract; }
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE ECombat GetCombatType() const { return CombatType; }
};
