// Fill out your copyright notice in the Description page of Project Settings.

#include "TollDocument.h"

#include "InspectorCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

#include "TollPlayerController.h"
#include "TollDocumentWidget.h"
#include "TollManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATollDocument::ATollDocument()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DocumentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DocumentMesh"));
	RootComponent = DocumentMesh;

}

// Called when the game starts or when spawned
void ATollDocument::BeginPlay()
{
	Super::BeginPlay();

	// Store the original transform of the document on the desk for later use when dropping it back.
	OriginalDeskTransform = GetActorTransform(); 
}

// Called every frame
void ATollDocument::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATollDocument::Interact_Implementation(AActor* Interactor)
{
	if (DocumentPickupSound)
	{
		// Play the sound at the location of the document when it is picked up.
		UGameplayStatics::PlaySoundAtLocation(this, DocumentPickupSound, GetActorLocation());
	}

	// 1. Directly access the player's brain (Player Controller) from the world
	ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	// 2. Security Armor: Is the Controller present and is the Interface (Widget) created in RAM?
	if (TollPlayerController && TollPlayerController->DocumentWidgetInstance)
	{
		// 3. Fire the data inside directly to the UI's function
		TollPlayerController->DocumentWidgetInstance->UpdateDocumentUI(DocumentDetails.DriverName, DocumentDetails.LicensePlate, DocumentDetails.ExpirationYear, DocumentDetails.DriverPhoto);
		TollPlayerController->ShowDocumentUI();
	}

	AInspectorCharacter* OurCharacter = Cast<AInspectorCharacter>(Interactor);
	if (OurCharacter)
	{
		DocumentMesh->SetSimulatePhysics(false);
		SetActorEnableCollision(false);
		AttachToComponent(OurCharacter->FirstPersonCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRelativeLocation(FVector(40.0f, 20.0f, -15.0f));
		SetActorRelativeRotation(FRotator(0.0f, 90.0f, 45.0f));
	}
}

void ATollDocument::StopInteract_Implementation(AActor* Interactor)
{
	if (DocumentDropSound)
	{
		// Play the sound at the location of the document when it is dropped.
		UGameplayStatics::PlaySoundAtLocation(this, DocumentDropSound, GetActorLocation());
	}

	// DocumentMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// When we picked up the document, we didn't write DocumentMesh->AttachToComponent(...) in Interact_Implementation; we directly said AttachToComponent(...) to the actor itself.
	// For symmetry and readability of the code, if we glued the whole actor when picking it up, we need to detach the whole actor when dropping it.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	DocumentMesh->SetSimulatePhysics(false);
	// Reset to the original position of the desk in memory with zero error.
	SetActorTransform(OriginalDeskTransform);

	// Directly access the player's brain (Player Controller) from the world
	ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (TollPlayerController)
	{
		TollPlayerController->HideDocumentUI();
	}
}

void ATollDocument::SetupDocumentData(const FDriverData& InDriverData)
{
	DocumentDetails.DriverName = InDriverData.DriverName;

	DocumentDetails.DriverPhoto = InDriverData.DriverPhoto;

	DocumentDetails.LicensePlate = InDriverData.LicensePlate;

	DocumentDetails.ExpirationYear = InDriverData.ExpirationYear;

	DocumentDetails.bIsFake = InDriverData.bHasAnomaly;

}

