// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "ChatBoxWidget.generated.h"

/**
 * Widget class for Chat functionality
 */
UCLASS()
class WIZARD_API UChatBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	/// <summary>
	/// Function to add the message to the
	/// MessageBox
	/// </summary>
	/// <param name="Message">The message to add</param>
	void AddMessage(const FText& Message);

private:

	/// <summary>
	/// Widget class for the Message
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Chat")
	TSubclassOf<class UUserWidget> ChatMessageWidgetClass;

	class AWizardPlayerController* OwnerController;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatTextBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* MessageBox;

	/// <summary>
	/// Number of messages to show in the messagebox
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Message")
	int32 NumOfMessages = 5;

	/// <summary>
	/// Callback function to when the Player
	/// presses Enter and commits a message
	/// </summary>
	/// <param name="Text">The message</param>
	/// <param name="CommitMethod">Commit Type</param>
	UFUNCTION()
	void OnMessageSent(const FText& Text, ETextCommit::Type CommitMethod);
};
