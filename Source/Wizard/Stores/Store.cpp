// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"
#include "Wizard/Wizard.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"

// Sets default values
AStore::AStore()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the Point Of Interest Component
	POI = CreateDefaultSubobject<UPointOfInterestComponent>(TEXT("PointOfInterest"));
	POI->SetIsReplicated(true);

	// Create Store Mesh
	StoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoreMesh"));
	SetRootComponent(StoreMesh);

	// Create AreaSphere
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create Overhead widget
	OverheadComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadComponent->SetupAttachment(RootComponent);
	OverheadComponent->SetTwoSided(true);

	// Create Interact widget
	InteractComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStore::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup Store
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	if (HasAuthority()) {
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AStore::OnStoreBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AStore::OnStoreEndOverlap);
	}
	AreaSphere->OnClicked.AddDynamic(this, &AStore::OnStoreClicked);

	if (InteractComponent) InteractComponent->SetVisibility(false);

	POI->SetupPOI(this);

	CreateCatalog();
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

	// Init Interact widget
	if (InteractWidget == nullptr && InteractComponent) {
		InteractWidget = Cast<UOverheadWidget>(InteractComponent->GetWidget());
		if (InteractWidget) {
			InteractWidget->SetDisplayText(FString::Printf(TEXT("[Left Click] to Interact")));
		}
	}
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
	int32 index = FMath::RandRange(0, Products.Num() - 1);
	CatalogItems.Add(ProductKeys[index]);
}

TArray<FItemDataTable> AStore::GetStoreCatalog()
{
	TArray<FItemDataTable> Catalog;

	for (auto& ItemName : CatalogItems) {
		Catalog.Add(Products[ItemName]);
	}

	return Catalog;
}

void AStore::RemoveItemFromCatalog(FItemDataTable Item)
{
	if (CatalogItems.Contains(Item.ItemName)) CatalogItems.Remove(Item.ItemName);
}

void AStore::GetProducts()
{
	TArray<UDataTable*> Tables = GetProductTables();

	for (auto& Table : Tables) {
		if (Table) {
			for (FName RowName : Table->GetRowNames()) {
				FItemDataTable* ItemRow = Table->FindRow<FItemDataTable>(RowName, TEXT(""));
				ProductKeys.Add(*ItemRow->ItemName);
				Products.Add(*ItemRow->ItemName, *ItemRow);
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
	const FString EnergyItemSelectorTablePath{ ENERGY_ITEM_DATA_TABLE_PATH };
	UDataTable* EnergyItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *EnergyItemSelectorTablePath));
	const FString AgilityItemSelectorTablePath{ AGILITY_ITEM_DATA_TABLE_PATH };
	UDataTable* AgilityItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *AgilityItemSelectorTablePath));
	const FString CombatItemSelectorTablePath{ COMBAT_ITEM_DATA_TABLE_PATH };
	UDataTable* CombatItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *CombatItemSelectorTablePath));
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
		if (EnergyItemSelectorTableObject) {
			TableObjects.Add(EnergyItemSelectorTableObject);
		}
		break;
	case EStore::ES_Forge:
		if (AgilityItemSelectorTableObject) {
			TableObjects.Add(AgilityItemSelectorTableObject);
		}
		if (CombatItemSelectorTableObject) {
			TableObjects.Add(CombatItemSelectorTableObject);
		}
		if (IntelligenceItemSelectorTableObject) {
			TableObjects.Add(IntelligenceItemSelectorTableObject);
		}
		if (WisdomItemSelectorTableObject) {
			TableObjects.Add(WisdomItemSelectorTableObject);
		}
		break;
	case EStore::EA_MAX:
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

void AStore::ShowInteractWidget(bool bShowInteractWidget)
{
	if (InteractComponent) {
		InteractComponent->SetVisibility(bShowInteractWidget);
	}
}
#pragma endregion
