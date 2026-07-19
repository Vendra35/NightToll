// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TollPlayerController.generated.h"

class UTollDocumentWidget;
class UTollHUDWidget;

/**
 * 
 */
UCLASS()
class NIGHTTOLL_API ATollPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Document Widget Class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTollDocumentWidget> DocumentWidgetClass;

	// End of Shift Widget Class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EndOfShiftWidgetClass;

	// HUD Widget Class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTollHUDWidget> HUDWidgetClass;

public:
	
	// Document Widget Instance
	UPROPERTY(BlueprintReadOnly)
	UTollDocumentWidget* DocumentWidgetInstance;

	// End of Shift Widget Instance
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* EndOfShiftWidgetInstance;

	// HUD Widget Instance
	UPROPERTY(BlueprintReadOnly)
	UTollHUDWidget* HUDWidgetInstance;

	// Show the Document UI
	void ShowDocumentUI();

	// Hide the Document UI
	void HideDocumentUI();

	// Show the End of Shift UI
	void ShowEndOfShiftUI();

	// Update the HUD with the new money amount
	void UpdateHUDMoney(int32 NewMoney); 
	
};
