// Fill out your copyright notice in the Description page of Project Settings.


#include "District.h"
#include "Components/BoxComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

// Sets default values
ADistrict::ADistrict()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DistrictBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DistrictBox"));
	DistrictBox->SetupAttachment(RootComponent);
	DistrictBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DistrictBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DistrictBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void ADistrict::BeginPlay()
{
	Super::BeginPlay();
	
	// Wait until Component becomes valid
	GetWorldTimerManager().SetTimer(
		BindOverlapTimer,
		this,
		&ADistrict::BindOverlapTimerFinished,
		BindOverlapTime
	);
}

void ADistrict::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(OtherActor->GetInstigatorController());
	if (PlayerController && Character && Character->GetAction() &&
		Character->GetAction()->GetCurrentDistrict() != DistrictName) { // if travelling to a new district
		PlayerController->ShowHUDTravelPopUp(DistrictName);
	}
}

void ADistrict::BindOverlapTimerFinished()
{
	DistrictBox->OnComponentBeginOverlap.AddDynamic(this, &ADistrict::OnBoxOverlap);
}

// Called every frame
void ADistrict::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

