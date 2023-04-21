// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UMenuWidget::SetMenu(bool bGameHasEnded)
{
	if (bGameHasEnded) {
		ResumeButton->RemoveFromParent();
		if (RestartButton && !RestartButton->OnClicked.IsBound()) RestartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnRestartButtonClicked);
	}
	else {
		RestartButton->RemoveFromParent();
		if (ResumeButton && !ResumeButton->OnClicked.IsBound()) ResumeButton->OnClicked.AddDynamic(this, &UMenuWidget::ResumeGame);
	}

	if (ExitButton && !ExitButton->OnClicked.IsBound()) ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnExitButtonClicked);
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UMenuWidget::OnDestroySession);
		}
	}
}

void UMenuWidget::ResumeGame()
{
	AWizardPlayerController* WController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (WController) WController->ToggleHUDInGameMenu();

	if (RestartButton && RestartButton->OnClicked.IsBound()) RestartButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnRestartButtonClicked);
	if (ResumeButton && ResumeButton->OnClicked.IsBound()) ResumeButton->OnClicked.RemoveDynamic(this, &UMenuWidget::ResumeGame);
	if (ExitButton && ExitButton->OnClicked.IsBound()) ExitButton->OnClicked.RemoveDynamic(this, &UMenuWidget::OnExitButtonClicked);

	if (MultiplayerSessionsSubsystem && MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UMenuWidget::OnDestroySession);
	}
}

void UMenuWidget::OnRestartButtonClicked()
{
	// TODO travel to lobby
}

void UMenuWidget::OnExitButtonClicked()
{
	ExitButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* FirstPlayerController = World->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			AWizardCharacter* WCharacter = Cast<AWizardCharacter>(FirstPlayerController->GetPawn());
			if (WCharacter)
			{
				WCharacter->LeaveGame();
				WCharacter->OnLeftGameDelegate.AddDynamic(this, &UMenuWidget::OnPlayerLeftGame);
			}
			else
			{
				ExitButton->SetIsEnabled(true);
			}
		}
	}
}

void UMenuWidget::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ExitButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
		if (GameMode)
		{
			GameMode->ReturnToMainMenuHost();
		}
		else
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
			}
		}
	}
}

void UMenuWidget::OnPlayerLeftGame()
{
	if (MultiplayerSessionsSubsystem) MultiplayerSessionsSubsystem->DestroySession();
}
