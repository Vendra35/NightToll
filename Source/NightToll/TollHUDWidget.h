// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TollHUDWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class NIGHTTOLL_API UTollHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Text block to display the player's money
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoneyText; 

	// Function to update the displayed money amount
	void UpdateMoney(int32 NewAmount);
	
};
