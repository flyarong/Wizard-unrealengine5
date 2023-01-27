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
}

// Called when the game starts or when spawned
void AStore::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	if (HasAuthority()) {
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AStore::OnStoreBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AStore::OnStoreEndOverlap);
	}
	AreaSphere->OnClicked.AddDynamic(this, &AStore::OnStoreClicked);

	POI->SetupPOI(this);

	CreateCatalog();
}

void AStore::CreateCatalog()
{
	const FString ItemSelectorTablePath{ ITEM_DATA_TABLE_PATH };
	UDataTable* ItemSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ItemSelectorTablePath));

	if (ItemSelectorTableObject) {
		for (FName RowName : ItemSelectorTableObject->GetRowNames()) {
			FItemDataTable* ItemRow = ItemSelectorTableObject->FindRow<FItemDataTable>(RowName, TEXT(""));
			Catalog.Add(*ItemRow);
		}
	}
}

void AStore::OnStoreBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->SetCurrentStore(this);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Session created successfully!")));
		}
	}
}

void AStore::OnStoreEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveStore();
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Session created successfully!")));
		}
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
			Character->GetAction()->ShowStoreCatalog();
		}
	}
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

