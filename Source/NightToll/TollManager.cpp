// Fill out your copyright notice in the Description page of Project Settings.


#include "TollManager.h"

#include "TollBarrier.h"
#include "TollDocument.h"
#include "TollVehicle.h"
#include "TollPlayerController.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATollManager::ATollManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Unreal Enginede yarattýðýn her AActor sýnýfý, arka planda motorun çekirdeðinden miras aldýðý varsayýlan bir USceneComponent* RootComponent; deðiþkenine halihazýrda sahiptir.
	// Sadece yerel (local) bir iþaretçiyle yaratýp, motorun kendi RootComponent'ine eþitliyoruz.
	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath"));
	VehiclePath->SetupAttachment(RootComponent);

	ExitPath = CreateDefaultSubobject<USplineComponent>(TEXT("ExitPath"));
	ExitPath->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATollManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnVehicle();
}

// Called every frame
void ATollManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATollManager::ProcessDocumentDecision(bool bIsApproved)
{
		UE_LOG(LogTemp, Display, TEXT("Document decision processed: %s"), bIsApproved ? TEXT("Approved") : TEXT("Rejected"));

		if (bIsApproved && ControlledBarrier && CurrentVehicle) // Check if the barrier and vehicle are valid before trying to open the barrier and depart
		{
			// Open the barrier and allow the vehicle to depart along the exit path
			ControlledBarrier->OpenBarrier();
			CurrentVehicle->Depart(bIsApproved, ExitPath);
		}
		else if (!bIsApproved && CurrentVehicle)
		{
			// If the document is rejected, we can still allow the vehicle to depart, but without opening the barrier. This could represent a scenario where the vehicle is sent back or redirected.
			CurrentVehicle->Depart(bIsApproved, nullptr);
		}
}

void ATollManager::SpawnDocumentForCurrentDriver()
{
	if (DocumentClassToSpawn && DailyDrivers.IsValidIndex(CurrentDriverIndex)) // Check if the index is valid before accessing the array
	{
		// Spawn the document actor at the specified transform
		ATollDocument* SpawnedDocument = GetWorld()->SpawnActor<ATollDocument>(DocumentClassToSpawn, DocumentSpawnTransform);
		if (SpawnedDocument)
		{
			// Set up the document data for the current driver
			SpawnedDocument->SetupDocumentData(DailyDrivers[CurrentDriverIndex]);
		}
	}
}

void ATollManager::OnVehicleArrived()
{
	// When the vehicle arrives, we can spawn the document for the current driver
	SpawnDocumentForCurrentDriver();
}

void ATollManager::SpawnVehicle()
{
	if (VehicleClassToSpawn) // Check if the vehicle class is set before trying to spawn
	{
		CurrentVehicle = GetWorld()->SpawnActor<ATollVehicle>(VehicleClassToSpawn);

		if (CurrentVehicle)
		{
			// Set the vehicle's initial position to the start of the spline
			CurrentVehicle->SetupVehicle(VehiclePath, this);
		}
	}
}

void ATollManager::OnVehicleExited()
{
	CurrentDriverIndex++; // Move to the next driver in the DailyDrivers array

	if (CurrentDriverIndex < DailyDrivers.Num()) // Check if there are more drivers to process
	{
		SpawnVehicle();
	}
	else // If all drivers have been processed, we can show the end of shift UI or perform any other end-of-day logic
	{
		ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0)); // Get the player controller to show the end of shift UI
		if (TollPlayerController)
		{
			TollPlayerController->ShowEndOfShiftUI(); // Call a function in the player controller to show the end of shift UI
		}
	}

	if (ControlledBarrier) // Ensure the barrier is closed after the vehicle exits
	{
		ControlledBarrier->CloseBarrier();
	}
}

