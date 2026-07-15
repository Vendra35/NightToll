// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TollPlayerController.generated.h"

class UTollDocumentWidget;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTollDocumentWidget> DocumentWidgetClass;

public:
	UPROPERTY(BlueprintReadOnly)
	UTollDocumentWidget* DocumentWidgetInstance;

	void ShowDocumentUI();

	void HideDocumentUI();
	
};
