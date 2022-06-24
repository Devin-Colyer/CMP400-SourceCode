// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "InteractableBase.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentLine = -1;
}

void UDialogueWidget::NextLine()
{
	if (CurrentLine < DialogueLines.Num()-1) {
		CurrentLine++;
	}
	else {
		RemoveFromParent();
	}
}
