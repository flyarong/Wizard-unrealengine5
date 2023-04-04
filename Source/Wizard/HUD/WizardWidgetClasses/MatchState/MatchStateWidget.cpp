// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchStateWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameStates/WizardGameState.h"
#include "Wizard/GameModes/WizardGameMode.h"

void UMatchStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController && PlayerController->HasAuthority()) {
		EndDelegate.BindDynamic(this, &UMatchStateWidget::OnMatchStateAnimationFinished);
		BindToAnimationFinished(MatchStateFade, EndDelegate);
	}
}

void UMatchStateWidget::SetMatchState(const FName& StateOfMatch)
{
	CurrentState = StateOfMatch;

	if (CurrentState == MatchState::Enemy) MatchStateText->SetText(FText::FromString(TEXT("Enemies' turn")));
	else if (CurrentState == MatchState::Trial) MatchStateText->SetText(FText::FromString(TEXT("Trial state")));
	else if (CurrentState == MatchState::Prepare) MatchStateText->SetText(FText::FromString(TEXT("Prepare state")));

	PlayAnimation(MatchStateFade);
}

void UMatchStateWidget::OnMatchStateAnimationFinished()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
	AWizardGameState* WGameState = Cast<AWizardGameState>(UGameplayStatics::GetGameState(this));
	if (PlayerController && PlayerController->HasAuthority() && WGameState) {
		if (CurrentState == MatchState::Enemy) {
			WGameState->DisableWizardActors();
			WGameState->MoveEnemies();
		}
		else if (CurrentState == MatchState::Trial) {
			WGameState->StartTrial();
		}
		else if (CurrentState == MatchState::Prepare) {
			WGameState->PrepareTurn();
		}
	}

	RemoveFromParent();
}
