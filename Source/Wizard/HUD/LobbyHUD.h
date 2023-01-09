// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "LobbyHUD.generated.h"

/**
* Data structure for character selection
*/
USTRUCT(BlueprintType)
struct FCharacterDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Character's name
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	/// <summary>
	/// Character's class
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWizardCharacter> CharacterClass;

	/// <summary>
	/// Character's skeletal mesh
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh* CharacterMesh;

	/// <summary>
	/// Character's Magic Staff mesh
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* MagicStaff;

	/**
	* Wizard Stats
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Intelligence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Combat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Agility;
};

/**
 * HUD class for the Lobby
 */
UCLASS()
class WIZARD_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Add Lobby menu widget
	/// to the HUD
	/// </summary>
	void AddLobbyMenu();

	/// <summary>
	/// Function creating buttons
	/// for the character selector
	/// </summary>
	void CreateCharacterButtons();

	/// <summary>
	/// Function spawning the character buttons
	/// onto the menu
	/// </summary>
	/// <param name="SelectionStatus">Status of the selection</param>
	void SpawnCharacterSelector(TArray<bool> SelectionStatus);

	/// <summary>
	/// Function removing the character buttons
	/// from the menu
	/// </summary>
	void RemoveCharacterSelector();

	/// <summary>
	/// Function spawning the Ready button
	/// onto the menu
	/// </summary>
	void SpawnReadyButton();

	/// <summary>
	/// Function spawning the Start Game button
	/// onto the menu
	/// </summary>
	/// <param name="bIsButtonEnabled">Whether the button should be enabled or not</param>
	void SpawnStartGameButton(bool bIsButtonEnabled = false);

	/// <summary>
	/// Function spawning the Player List
	/// onto the menu
	/// </summary>
	/// <param name="BusyPlayers">List of players who are unready</param>
	void SpawnPlayerList(TArray<FString> BusyPlayers);

private:

	/// <summary>
	/// Controller owning this HUD
	/// </summary>
	UPROPERTY()
	class ALobbyController* OwningController;

	/// <summary>
	/// Character selector menu pointer
	/// </summary>
	UPROPERTY()
	class ULobbyMenu* LobbyMenu;

	/// <summary>
	/// Array containing the character
	/// buttons
	/// </summary>
	UPROPERTY()
	TArray<class UCharacterButton*> CharacterButtons;

	/// <summary>
	/// Map containing the player readiness status
	/// </summary>
	UPROPERTY()
	TMap<FString, bool> PlayerList;

#pragma region WidgetClasses
	/// <summary>
	/// Character Selector menu class
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	TSubclassOf<class UUserWidget> LobbyMenuClass;

	/// <summary>
	/// Widget class to create the button widget from
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	TSubclassOf<class UUserWidget> CharacterButtonWidget;

	/// <summary>
	/// Widget class to create the list element in the Player list
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	TSubclassOf<class UUserWidget> PlayerListElementWidget;

	/// <summary>
	/// Image to show when player is ready
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	class UTexture2D* PlayerReadyTexture;

	/// <summary>
	/// Image to show when player is not ready
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	UTexture2D* PlayerNotReadyTexture;

	/// <summary>
	/// Widget class to create the ready button on all machines
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	TSubclassOf<class UUserWidget> ReadyButtonWidget;

	/// <summary>
	/// Widget class to create the start game button on the server
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Selector")
	TSubclassOf<class UUserWidget> StartGameButtonWidget;
#pragma endregion
};
