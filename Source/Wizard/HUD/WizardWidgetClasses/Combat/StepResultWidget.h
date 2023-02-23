// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StepResultWidget.generated.h"

/**
 * Widget class for showing the result of
 * the Current Step
 */
UCLASS()
class WIZARD_API UStepResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to set the result on
	/// screen
	/// </summary>
	/// <param name="bIsSuccess">Whether the Step was successful or not</param>
	void SetResult(bool bIsSuccess);

private:

	UPROPERTY(EditAnywhere, Category = "Result Textures")
	class UTexture2D* SuccessTexture;

	UPROPERTY(EditAnywhere, Category = "Result Textures")
	UTexture2D* FailTexture;

	UPROPERTY(meta = (BindWidget))
	class UImage* ResultImage;
};
