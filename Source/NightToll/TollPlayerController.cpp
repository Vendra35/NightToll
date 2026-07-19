// Fill out your copyright notice in the Description page of Project Settings.


#include "TollPlayerController.h"

#include "TollDocumentWidget.h"
#include "Blueprint/UserWidget.h"
#include "TollHUDWidget.h"

void ATollPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create and add the Document Widget to the viewport and hide it initially
	if (DocumentWidgetClass)
	{
		DocumentWidgetInstance = CreateWidget<UTollDocumentWidget>(this, DocumentWidgetClass);
		if (DocumentWidgetInstance)
		{
			DocumentWidgetInstance->AddToViewport();
			HideDocumentUI();
		}
	}
	// Create and add the HUD Widget to the viewport
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UTollHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
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

void ATollPlayerController::ShowEndOfShiftUI()
{
	if (EndOfShiftWidgetClass) // Check if the End of Shift widget class is set
	{
		EndOfShiftWidgetInstance = CreateWidget<UUserWidget>(this, EndOfShiftWidgetClass); // Create the End of Shift widget instance

		if (EndOfShiftWidgetInstance) // Check if the widget instance was created successfully
		{
			EndOfShiftWidgetInstance->AddToViewport(); // Add the widget to the viewport
			bShowMouseCursor = true; // Show the mouse cursor
			FInputModeUIOnly InputMode; // Set the input mode to UI only
			SetInputMode(InputMode); // Apply the input mode
			FlushPressedKeys(); // Flush any pressed keys to prevent unintended input after showing the UI
		}
	}
}

void ATollPlayerController::UpdateHUDMoney(int32 NewMoney)
{
	// Check if the HUD widget instance is valid before attempting to update the money display
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateMoney(NewMoney); // Call the UpdateMoney function on the HUD widget instance to update the displayed money amount
	}
}
