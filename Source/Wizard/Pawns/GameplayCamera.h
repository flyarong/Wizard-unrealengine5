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

#pragma region CameraMovementFunctions
	/// <summary>
	/// Function for setting the Camera zoom position
	/// with Mouse wheel
	/// </summary>
	void SetPositionWithMouseWheel(float Value);

	/// <summary>
	/// Rotating the camera with the mouse
	/// Input was setup in Controller
	/// </summary>
	/// <param name="Value">Base amount to rotate</param>
	void MouseRotate(float Value);

	/// <summary>
	/// Function for moving the camera forward/backward with keys
	/// </summary>
	void KeyMoveForwardOrBackward(float Value);

	/// <summary>
	/// Function for moving the camera left/right with keys
	/// </summary>
	void KeyMoveLeftOrRight(float Value);
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
	/// movement with mouse
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement") 
	bool bEnableMouseMovement = true;
	
	FVector Bound;

	/// <summary>
	/// Camera Position multiplier
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	int32 CameraPosition = 1;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	int32 MaxCameraPosition = 5;

	/// <summary>
	/// Variable holding the amount to zoom in/out
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float CameraZoomDiff = 150.f;

	/// <summary>
	/// CameraBoom default length
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float DefaultSpringArmLength = 800.f;

	/// <summary>
	/// Speed multiplier when Camera is moving
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float CameraMovementSpeed = 20.f;

	/// <summary>
	/// How fast the camera should rotate
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float CameraRotationMultiplier = 3.f;
#pragma endregion

	/// <summary>
	/// Function for handling camera zooming in/out
	/// </summary>
	void CameraZoom(float DeltaTime);

	/// <summary>
	/// Camera Movement function for keys
	/// </summary>
	/// <param name="Value">Input value</param>
	/// <param name="RotationY">Rotates the vector depending on movement direction</param>
	void KeyMove(float Value, float RotationY);

	/**
	* Functions for moving the camera with the mouse
	*/
	///
	void MouseMoveLeft();
	void MouseMoveRight();
	void MouseMoveForward();
	void MouseMoveBack();

	/// <summary>
	/// Function for checking whether or not we are
	/// at the border of the Navmesh bound
	/// </summary>
	bool CheckCameraMovementBounds(FVector DeltaLocation);

public:
	FORCEINLINE void SetEnableCameraMovementWithMouse(bool bIsMovementEnabled) { bEnableMouseMovement = bIsMovementEnabled; }
};
