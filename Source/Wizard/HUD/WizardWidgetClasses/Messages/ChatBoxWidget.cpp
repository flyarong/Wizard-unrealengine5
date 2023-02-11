// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatBoxWidget.h"
#include "ChatMessageWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"

void UChatBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChatTextBox->OnTextCommitted.AddDynamic(this, &UChatBoxWidget::OnMessageSent);
}

void UChatBoxWidget::AddMessage(const FText& Message)
{
	OwnerController = OwnerController == nullptr ?
		Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController()) : OwnerController;
	if (OwnerController) {
		UChatMessageWidget* ChatMessage = CreateWidget<UChatMessageWidget>(OwnerController, ChatMessageWidgetClass);
		if (ChatMessage) {
			ChatMessage->ConstructMessage(Message);
			MessageBox->AddChildToVerticalBox(ChatMessage);
		}
	}
}

void UChatBoxWidget::OnMessageSent(const FText& Text, ETextCommit::Type CommitMethod)
{
	OwnerController = OwnerController == nullptr ?
		Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController()) : OwnerController;
	if (OwnerController && CommitMethod == ETextCommit::OnEnter) {
		OwnerController->ServerSendChatMessage(OwnerController->GetPlayerState<AWizardPlayerState>()->GetPlayerName(), Text);
		ChatTextBox->SetText(FText::GetEmpty());
	}
}
