// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "JoinedPlayer.generated.h"

/**
 * Widget class to show the recently joined
 * player's name in the lobby
 */
UCLASS()
class WIZARD_API UJoinedPlayer : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/// <summary>
	/// Text holding the player's name
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinedPlayerText;

	/// <summary>
	/// Image showing the status of the players
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UImage* ReadyStatusImage;

public:
	FORCEINLINE void SetJoinedPlayerText(FString PlayerName) { JoinedPlayerText->SetText(FText::FromString(*PlayerName)); };
	FORCEINLINE void SetReadyStatusImage(UTexture2D* ReadyStatus) { ReadyStatusImage->SetBrushFromTexture(ReadyStatus); };
};
