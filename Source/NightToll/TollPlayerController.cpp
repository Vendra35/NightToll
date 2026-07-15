// Fill out your copyright notice in the Description page of Project Settings.


#include "TollPlayerController.h"

#include "TollDocumentWidget.h"
#include "Blueprint/UserWidget.h"

void ATollPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DocumentWidgetClass)
	{
		DocumentWidgetInstance = CreateWidget<UTollDocumentWidget>(this, DocumentWidgetClass);
		if (DocumentWidgetInstance)
		{
			DocumentWidgetInstance->AddToViewport();
			HideDocumentUI();
		}
	}
}

void ATollPlayerController::ShowDocumentUI()
{
	if (DocumentWidgetInstance)
	{
		DocumentWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
}

void ATollPlayerController::HideDocumentUI()
{
	if (DocumentWidgetInstance)
	{
		DocumentWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}
