// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerStartButton.generated.h"

/**
 * Widget class for the server to start the game
 * from the lobby
 */
UCLASS()
class WIZARD_API UServerStartButton : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/// <summary>
	/// Start game button
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

public:
	FORCEINLINE UButton* GetStartGameButton() const { return StartGameButton; }
};
