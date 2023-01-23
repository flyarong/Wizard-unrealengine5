// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerIconWidget.generated.h"

/**
 * Widget class for the player camera's icon
 * on the MiniMap
 */
UCLASS()
class WIZARD_API UPlayerIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	
	/// <summary>
	/// Image of the player on the MiniMap
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerIcon;
};
