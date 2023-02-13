// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Wizard/Characters/WizardCharacter.h"

// Sets default values
AGameplayCamera::AGameplayCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;
	SetReplicateMovement(false);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = DefaultSpringArmLength;
	CameraBoom->SetRelativeLocation(FVector(-5.f, 35.f, 190.f));
	CameraBoom->SetRelativeRotation(FRotator(-35.f, -120.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 5.f;
	CameraBoom->CameraRotationLagSpeed = 5.f;

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	TopDownCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TopDownCameraComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AGameplayCamera::BeginPlay()
{
	Super::BeginPlay();
	
	ANavMeshBoundsVolume* NavMesh = Cast<ANavMeshBoundsVolume>(UGameplayStatics::GetActorOfClass(this, ANavMeshBoundsVolume::StaticClass()));
	
	FVector Origin;
	FVector BoxExtent;
	NavMesh->GetActorBounds(false, Origin, BoxExtent, false);
	Bound = Origin + BoxExtent;
}

// Called every frame
void AGameplayCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraZoom(DeltaTime);
	if (bEnableMouseMovement) {
		MouseMoveLeft();
		MouseMoveForward();
		MouseMoveRight();
		MouseMoveBack();
	}
	FollowWizardWithCamera();
}

// Called to bind functionality to input
void AGameplayCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGameplayCamera::SetCameraFocusOnWizard()
{
	if (WCharacter) {
		if (!bFollowWizard) bFollowWizard = true;
		SetActorLocation(WCharacter->GetActorLocation());
	}
}

void AGameplayCamera::SetPositionWithMouseWheel(float Value)
{
	if (Value > 0.f) {
		CameraPosition = FMath::Clamp(CameraPosition + 1, -MaxCameraPosition, MaxCameraPosition);
	}
	else if (Value < 0.f) {
		CameraPosition = FMath::Clamp(CameraPosition - 1, -MaxCameraPosition, MaxCameraPosition);
	}
}

void AGameplayCamera::CameraZoom(float DeltaTime)
{
	float NewTargetLength = DefaultSpringArmLength + (-1 * (CameraZoomDiff * CameraPosition));
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, NewTargetLength, DeltaTime, 2.f);
}

void AGameplayCamera::MouseRotate(float Value)
{
	if (CameraBoom) {
		FRotator CameraRotation(0.f, Value * CameraRotationMultiplier, 0.f);
		CameraBoom->AddWorldRotation(CameraRotation);
	}
}

void AGameplayCamera::KeyMoveForwardOrBackward(float Value)
{
	KeyMove(Value, -90.f + CameraBoom->GetComponentRotation().Yaw);
}

void AGameplayCamera::KeyMoveLeftOrRight(float Value)
{
	KeyMove(Value, CameraBoom->GetComponentRotation().Yaw);
}

void AGameplayCamera::KeyMove(float Value, float RotationY)
{
	if (CameraBoom && Value != 0) {
		if (bFollowWizard) bFollowWizard = false;
		FVector DeltaLocation = FRotator(0.f, RotationY, 0.f)
			.RotateVector(FVector(0.f, Value * CameraMovementSpeed, 0.f));
		if (CheckCameraMovementBounds(DeltaLocation)) CameraBoom->AddRelativeLocation(DeltaLocation);
	}
}

void AGameplayCamera::MouseMoveLeft()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(this);

	if (CameraBoom && MousePosition.X <= ViewPortSize.X * 0.03f) {
		FVector DeltaLocation = FRotator(0.f, CameraBoom->GetComponentRotation().Yaw, 0.f)
			.RotateVector(
				FVector(0.f, -CameraMovementSpeed, 0.f) *
				(1 - FMath::Clamp(UKismetMathLibrary::NormalizeToRange(MousePosition.X, 0.f, ViewPortSize.X * 0.03f), 0.f, 1.f))
			);
		if (CheckCameraMovementBounds(DeltaLocation)) {
			if (bFollowWizard) bFollowWizard = false;
			CameraBoom->AddRelativeLocation(DeltaLocation);
		}
	}
}

void AGameplayCamera::MouseMoveRight()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(this);

	if (CameraBoom && MousePosition.X >= ViewPortSize.X * 0.97f) {
		FVector DeltaLocation = FRotator(0.f, CameraBoom->GetComponentRotation().Yaw, 0.f)
			.RotateVector(
				FVector(0.f, CameraMovementSpeed, 0.f) *
				FMath::Clamp(UKismetMathLibrary::NormalizeToRange(MousePosition.X, ViewPortSize.X * 0.97f, ViewPortSize.X), 0.f, 1.f)
			);
		if (CheckCameraMovementBounds(DeltaLocation)) {
			if (bFollowWizard) bFollowWizard = false;
			CameraBoom->AddRelativeLocation(DeltaLocation);
		}
	}
}

void AGameplayCamera::MouseMoveForward()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(this);

	if (CameraBoom && MousePosition.Y <= ViewPortSize.Y * 0.03f) {
		FVector DeltaLocation = FRotator(0.f, 90.f + CameraBoom->GetComponentRotation().Yaw, 0.f)
			.RotateVector(
				FVector(0.f, -CameraMovementSpeed, 0.f) *
				(1 - FMath::Clamp(UKismetMathLibrary::NormalizeToRange(MousePosition.Y, 0.f, ViewPortSize.Y * 0.03f), 0.f, 1.f))
			);
		if (CheckCameraMovementBounds(DeltaLocation)) {
			if (bFollowWizard) bFollowWizard = false;
			CameraBoom->AddRelativeLocation(DeltaLocation);
		}
	}
}

void AGameplayCamera::MouseMoveBack()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(this);

	if (CameraBoom && MousePosition.Y >= ViewPortSize.Y * 0.97f) {
		FVector DeltaLocation = FRotator(0.f, 90.f + CameraBoom->GetComponentRotation().Yaw, 0.f)
			.RotateVector(
				FVector(0.f, CameraMovementSpeed, 0.f) *
				FMath::Clamp(UKismetMathLibrary::NormalizeToRange(MousePosition.Y, ViewPortSize.Y * 0.97f, ViewPortSize.Y), 0.f, 1.f)
			);
		if (CheckCameraMovementBounds(DeltaLocation)) {
			if (bFollowWizard) bFollowWizard = false;
			CameraBoom->AddRelativeLocation(DeltaLocation);
		}
	}
}

void AGameplayCamera::FollowWizardWithCamera()
{
	if (bFollowWizard) {
		SetCameraFocusOnWizard();
	}
}

bool AGameplayCamera::CheckCameraMovementBounds(FVector DeltaLocation)
{
	FVector CachedLocation = CameraBoom->GetRelativeLocation() + DeltaLocation;
	if ((FMath::Abs(CachedLocation.X) >= FMath::Abs(Bound.X)) || 
		(FMath::Abs(CachedLocation.Y) >= FMath::Abs(Bound.Y))) 
		return false;

	return true;
}