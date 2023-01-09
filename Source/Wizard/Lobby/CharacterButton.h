// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/HUD/LobbyHUD.h"
#include "CharacterButton.generated.h"

/// <summary>
/// Delegate to set the selected character
/// when it's button is clicked
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterSelected, UCharacterButton*, ButtonSelected);

/**
 * Widget class for character selection
 * in the lobby
 */
UCLASS()
class WIZARD_API UCharacterButton : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Delegate broadcasting information to the
	/// callback function about the selected character
	/// </summary>
	FCharacterSelected CharacterSelectedDelegate;

	/// <summary>
	/// Function triggered when the character's button is clicked
	/// CharacterSelectedDelegate gets broadcasted
	/// </summary>
	UFUNCTION()
	void OnSelectButtonClicked();

private:

	/// <summary>
	/// Vertical box storing the button
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CharacterSelectBox;

	/// <summary>
	/// The character's button
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* CharacterSelectButton;

	/// <summary>
	/// Text holding the character's name
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;

	/// <summary>
	/// Data table row index
	/// </summary>
	UPROPERTY()
	int32 Index;

	/// <summary>
	/// RowName of the data table row
	/// </summary>
	UPROPERTY()
	FName RowName;

	/// <summary>
	/// Character's datatable row struct
	/// </summary>
	UPROPERTY()
	FCharacterDataTable CharacterStruct;

public:
	FORCEINLINE UButton* GetCharacterSelectButton() const { return CharacterSelectButton; }
	FORCEINLINE int32 GetDataTableRowIndex() const { return Index; }
	FORCEINLINE void SetDataTableRowIndex(int32 RowIndex) { Index = RowIndex; };
	FORCEINLINE FName GetDataTableRowName() const { return RowName; }
	FORCEINLINE void SetDataTableRowName(FName Name) { RowName = Name; };
	FORCEINLINE void SetCharacterNameText(FString CharacterName) { CharacterNameText->SetText(FText::FromString(*CharacterName)); };
	FORCEINLINE FCharacterDataTable GetCharacterStruct() { return CharacterStruct; }
	FORCEINLINE void SetCharacterStruct(FCharacterDataTable Character) { CharacterStruct = Character; }
};
