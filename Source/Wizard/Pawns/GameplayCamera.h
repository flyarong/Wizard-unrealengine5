// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayCamera.generated.h"

UCLASS()
class WIZARD_API AGameplayCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameplayCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region AxisMappingCallbacks
	/// <summary>
	/// Callback function to Mouse Wheel Axis Mapping
	/// </summary>
	void MouseWheelAxis(float Value);

	/// <summary>
	/// Rotating the camera with the mouse
	/// Input was setup in Controller
	/// </summary>
	/// <param name="Value">Base amount to rotate</param>
	void MouseRotate(float Value);
#pragma endregion

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

#pragma region CameraProperties
	/// <summary>
	/// Boolean for enabling/disabling camera
	/// movement with mouse - mainly used for testing in PIE
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement") 
	bool bEnableMouseMovement = true;
	
	/// <summary>
	/// Camera Position multiplier
	/// </summary>
	UPROPERTY()
	int32 CameraPosition = 0;

	UPROPERTY()
	int32 MaxCameraPosition = 8;

	/// <summary>
	/// Variable holding the amount to zoom in/out
	/// </summary>
	UPROPERTY()
	float CameraZoomDiff = 150.f;

	/// <summary>
	/// CameraBoom default length
	/// </summary>
	UPROPERTY()
	float DefaultSpringArmLength = 800.f;

	/// <summary>
	/// Speed multiplier when Camera is moving
	/// </summary>
	UPROPERTY()
	float CameraMovementSpeed = 15.f;

	/// <summary>
	/// How fast the camera should rotate
	/// </summary>
	UPROPERTY()
	float CameraRotationMultiplier = 3.f;
#pragma endregion

#pragma region CameraMovementFunctions
	/// <summary>
	/// Function for handling camera zooming in/out
	/// </summary>
	void CameraZoom(float DeltaTime);

	/**
	* Functions for moving the camera with the mouse
	*/
	///
	void MouseMoveLeft();
	void MouseMoveRight();
	void MouseMoveForward();
	void MouseMoveBack();
#pragma endregion

public:
	FORCEINLINE float GetCameraMovementSpeed() const { return CameraMovementSpeed; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
