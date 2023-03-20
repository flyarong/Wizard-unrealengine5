// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/AttributeComponent.h"

bool FItemDataTable::operator==(const FItemDataTable& Rhs) const
{
	return ItemName == Rhs.ItemName;
}

uint32 GetTypeHash(const FItemDataTable& ItemRow)
{
	return GetTypeHash(ItemRow.ItemName);
}

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetSphereRadius(75.f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ItemEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemEffectComponent"));
	ItemEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ItemRow = GetItemRow();

		// Need timer because binding to OnComponentBeginOverlap shouldn't happen immediately
		FTimerHandle BindOverlapTimer;
		GetWorldTimerManager().SetTimer(
			BindOverlapTimer,
			this,
			&AItem::BindOverlapTimerFinished,
			.25f
		);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::BindOverlapTimerFinished()
{
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(OtherActor);
	if (WCharacter && !ItemRow.ItemName.IsEmpty()) {
		if (ItemRow.BoostType == EBoost::EB_XP) {
			if (WCharacter->GetAttribute()) WCharacter->GetAttribute()->AddXP(ItemRow.BoostAmount);
		}
		else {
			WCharacter->AddNewItem(ItemRow);
		}
		WCharacter->ServerInterruptMovement();
		WCharacter->MulticastPlayInteractMontage(FName("PickUp"));
	}

	Destroy();
}

FItemDataTable AItem::GetItemRow()
{
	if (ItemTableObject) {
		TArray<FItemDataTable*> ItemRows;
		ItemTableObject->GetAllRows<FItemDataTable>(TEXT(""), ItemRows);
		if (ItemRows.Num() > 0) {
			int32 Selection = FMath::RandRange(0, ItemRows.Num() - 1);
			return *ItemRows[Selection];
		}
	}

	return FItemDataTable();
}

void AItem::Destroyed()
{
	Super::Destroyed();

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}