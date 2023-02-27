// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"
#include "Wizard/Wizard.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"

// Sets default values
AStore::AStore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create Store Mesh and set it as Root
	StoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoreMesh"));
	StoreMesh->SetupAttachment(RootComponent);

	// Create Overhead widget
	OverheadComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadComponent->SetupAttachment(RootComponent);
	OverheadComponent->SetTwoSided(true);
}

// Called when the game starts or when spawned
void AStore::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup Events
	if (HasAuthority()) {
		CreateCatalog();
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AStore::OnStoreBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AStore::OnStoreEndOverlap);
	}
	AreaSphere->OnClicked.AddDynamic(this, &AStore::OnStoreClicked);
}

// Called every frame
void AStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Init Overhead widget
	if (OverheadWidget == nullptr && OverheadComponent) {
		OverheadWidget = Cast<UOverheadWidget>(OverheadComponent->GetWidget());
		if (OverheadWidget) {
			OverheadWidget->SetDisplayText(StoreName);
		}
	}
}

void AStore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStore, Catalog);
}

#pragma region Catalog
void AStore::CreateCatalog()
{
	if (Products.Num() == 0) {
		GetProducts();
	}

	if (Products.Num() > NumOfCatalogItems) {
		for (int32 i = 0; i < NumOfCatalogItems; i++) {
			AddItemToCatalog();
		}
	}
}

void AStore::AddItemToCatalog()
{
	int32 Index = FMath::RandRange(0, Products.Num() - 1);
	Catalog.Add(Products[Index]);
	Products.RemoveAt(Index);
}

void AStore::GetProducts()
{
	TArray<UDataTable*> Tables = GetProductTables();

	for (auto& Table : Tables) {
		if (Table) {
			for (FName RowName : Table->GetRowNames()) {
				FItemDataTable* ItemRow = Table->FindRow<FItemDataTable>(RowName, TEXT(""));
				Products.Add(*ItemRow);
			}
		}
	}
}

TArray<UDataTable*> AStore::GetProductTables()
{
	TArray<UDataTable*> TableObjects;
	const FString HealthItemSelectorTablePath{ HEALTH_ITEM_DATA_TABLE_PATH };
	UDataTable* HealthItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *HealthItemSelectorTablePath));
	const FString PowerItemSelectorTablePath{ POWER_ITEM_DATA_TABLE_PATH };
	UDataTable* PowerItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *PowerItemSelectorTablePath));
	const FString DefenseItemSelectorTablePath{ DEFENSE_ITEM_DATA_TABLE_PATH };
	UDataTable* DefenseItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DefenseItemSelectorTablePath));
	const FString AgilityItemSelectorTablePath{ AGILITY_ITEM_DATA_TABLE_PATH };
	UDataTable* AgilityItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *AgilityItemSelectorTablePath));
	const FString OffenseItemSelectorTablePath{ OFFENSE_ITEM_DATA_TABLE_PATH };
	UDataTable* OffenseItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *OffenseItemSelectorTablePath));
	const FString IntelligenceItemSelectorTablePath{ INTELLIGENCE_ITEM_DATA_TABLE_PATH };
	UDataTable* IntelligenceItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *IntelligenceItemSelectorTablePath));
	const FString WisdomItemSelectorTablePath{ WISDOM_ITEM_DATA_TABLE_PATH };
	UDataTable* WisdomItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WisdomItemSelectorTablePath));

	switch (StoreType)
	{
	case EStore::ES_General:
		if (HealthItemSelectorTableObject) {
			TableObjects.Add(HealthItemSelectorTableObject);
		}
		if (PowerItemSelectorTableObject) {
			TableObjects.Add(PowerItemSelectorTableObject);
		}
		break;
	case EStore::ES_Forge:
		if (OffenseItemSelectorTableObject) {
			TableObjects.Add(OffenseItemSelectorTableObject);
		}
		if (DefenseItemSelectorTableObject) {
			TableObjects.Add(DefenseItemSelectorTableObject);
		}
		break;
	case EStore::EA_MAX: // TODO need third store type for these
		if (AgilityItemSelectorTableObject) {
			TableObjects.Add(AgilityItemSelectorTableObject);
		}
		if (IntelligenceItemSelectorTableObject) {
			TableObjects.Add(IntelligenceItemSelectorTableObject);
		}
		if (WisdomItemSelectorTableObject) {
			TableObjects.Add(WisdomItemSelectorTableObject);
		}
		break;
	default:
		break;
	}

	return TableObjects;
}
#pragma endregion

#pragma region Events
void AStore::OnStoreBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->SetCurrentStore(this);
	}
}

void AStore::OnStoreEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveStore();
	}	
}

void AStore::OnStoreClicked(UPrimitiveComponent* TouchedComp, FKey ButtonPressed)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		AWizardCharacter* Character = PlayerController->GetWizardCharacter() ? PlayerController->GetWizardCharacter() :
			Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (Character && Character->GetAction() &&
			Character->GetAction()->GetCurrentStore() == this) {
			Character->GetAction()->OpenCatalog();
		}
	}
}
#pragma endregion
