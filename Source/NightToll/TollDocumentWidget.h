// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TollDocumentWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class NIGHTTOLL_API UTollDocumentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DriverNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LicensePlateText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ExpirationYearText;

	UFUNCTION(BlueprintCallable, Category = "Document UI")
	void UpdateDocumentUI(FString InDriverName, FString InLicensePlate, int32 InExpirationYear, UTexture2D* InDriverPhoto);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ApproveButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* RejectButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* DriverPhotoImage;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnApproveClicked();

	UFUNCTION()
	void OnRejectClicked();
	
};
