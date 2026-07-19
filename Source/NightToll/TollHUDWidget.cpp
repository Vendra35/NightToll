// Fill out your copyright notice in the Description page of Project Settings.


#include "TollHUDWidget.h"
#include "Components/TextBlock.h"

void UTollHUDWidget::UpdateMoney(int32 NewAmount)
{
	if (MoneyText)
	{
		MoneyText->SetText(FText::FromString(FString::Printf(TEXT("$ %d"), NewAmount))); // Update the text block with the new amount
	}
}
