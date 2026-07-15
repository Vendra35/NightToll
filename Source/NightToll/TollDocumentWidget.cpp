// Fill out your copyright notice in the Description page of Project Settings.


#include "TollDocumentWidget.h"

#include "InspectorCharacter.h"
#include "TollManager.h"
#include "TollPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTollDocumentWidget::UpdateDocumentUI(FString InDriverName, FString InLicensePlate, int32 InExpirationYear)
{
	if (DriverNameText && LicensePlateText && ExpirationYearText)
	{
		DriverNameText->SetText(FText::FromString(InDriverName));
		LicensePlateText->SetText(FText::FromString(InLicensePlate));
		ExpirationYearText->SetText(FText::FromString(FString::FromInt(InExpirationYear)));
	}
}

void UTollDocumentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ApproveButton && RejectButton)
	{
		ApproveButton->OnClicked.AddDynamic(this, &UTollDocumentWidget::OnApproveClicked);
		RejectButton->OnClicked.AddDynamic(this, &UTollDocumentWidget::OnRejectClicked);
	}
}

void UTollDocumentWidget::OnApproveClicked()
{
	ATollPlayerController* TollController = Cast<ATollPlayerController>(GetOwningPlayer());
	if (TollController)
	{
		// Arayüz patronunu buldu. Artýk TollController üzerinden her komutu verebiliriz.
		TollController->HideDocumentUI();
		AInspectorCharacter* InspectorCharacter = Cast<AInspectorCharacter>(TollController->GetCharacter());
		if (InspectorCharacter)
		{
			InspectorCharacter->DisposeDocument();
		}
		ATollManager* TollManager = Cast<ATollManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATollManager::StaticClass()));
		if (TollManager)
		{
			TollManager->ProcessDocumentDecision(true);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Approve button clicked"));
}

void UTollDocumentWidget::OnRejectClicked()
{
	ATollPlayerController* TollController = Cast<ATollPlayerController>(GetOwningPlayer());
	if (TollController)
	{
		// Arayüz patronunu buldu. Artýk TollController üzerinden her komutu verebiliriz.
		TollController->HideDocumentUI();
		AInspectorCharacter* InspectorCharacter = Cast<AInspectorCharacter>(TollController->GetCharacter());
		if (InspectorCharacter)
		{
			InspectorCharacter->DisposeDocument();
		}
		ATollManager* TollManager = Cast<ATollManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATollManager::StaticClass()));
		if (TollManager)
		{
			TollManager->ProcessDocumentDecision(false);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Reject button clicked"));
}
