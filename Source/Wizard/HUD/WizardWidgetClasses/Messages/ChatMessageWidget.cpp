// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMessageWidget.h"
#include "Components/TextBlock.h"

void UChatMessageWidget::ConstructMessage(const FText& Message)
{
	MessageText->SetText(Message);
}
