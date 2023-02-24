// Fill out your copyright notice in the Description page of Project Settings.


#include "District.h"
#include "Components/BoxComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/AttributeComponent.h"

// Sets default values
ADistrict::ADistrict()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DistrictBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DistrictBox"));
	DistrictBox->SetupAttachment(RootComponent);
	DistrictBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DistrictBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

// Called when the game starts or when spawned
void ADistrict::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup overlap on server
	if (HasAuthority()) {
		DistrictBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DistrictBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		DistrictBox->OnComponentBeginOverlap.AddDynamic(this, &ADistrict::OnBoxBeginOverlap);
		DistrictBox->OnComponentEndOverlap.AddDynamic(this, &ADistrict::OnBoxEndOverlap);
	}
}

void ADistrict::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction() && 
		Character->GetAction()->GetCurrentDistrict() != this) {
		Character->GetAction()->SetCurrentDistrict(this);
	}
}

void ADistrict::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAttribute()) {
		Character->GetAttribute()->SpendPower(PowerCost, EAction::EA_Movement);
	}
}

// Called every frame
void ADistrict::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

