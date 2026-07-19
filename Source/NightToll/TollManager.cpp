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

	// Get the player controller to update the HUD for the initial money amount
	ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (TollPlayerController) // Check if the cast was successful
	{
		TollPlayerController->UpdateHUDMoney(CurrentMoney); // Update the HUD with the initial money amount
	}
}

// Called every frame
void ATollManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATollManager::ProcessDocumentDecision(bool bIsApproved)
{
		UE_LOG(LogTemp, Display, TEXT("Document decision processed: %s"), bIsApproved ? TEXT("Approved") : TEXT("Rejected"));

		bool bIsAnomaly = DailyDrivers[CurrentDriverIndex].bHasAnomaly;
		bool bIsInvalid = DailyDrivers[CurrentDriverIndex].bHasInvalidDocument;

		// 1. Game rules and economy (completely independent of the barrier or vehicle)
		if (bIsApproved)
		{
			if (bIsAnomaly)
			{
				UE_LOG(LogTemp, Warning, TEXT("Anomaly detected for driver: %s"), *DailyDrivers[CurrentDriverIndex].DriverName);
				// Handle anomaly case here, e.g., log it, notify the player, etc.
				UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
			}
			else if (bIsInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid document detected for driver: %s"), *DailyDrivers[CurrentDriverIndex].DriverName);
				// Handle invalid document case here, e.g., log it, notify the player, etc.
				CurrentMoney -= 10; // Deduct money for invalid document
				UGameplayStatics::PlaySound2D(this, MoneyLoseSound);
			}
			else
			{
				CurrentMoney += 10; // Reward for approving a valid document
				UGameplayStatics::PlaySound2D(this, MoneyGainSound);
			}
		}
		else // (!bIsApproved)
		{
			if (bIsAnomaly)
			{
				UE_LOG(LogTemp, Warning, TEXT("Anomaly detected for driver: %s"), *DailyDrivers[CurrentDriverIndex].DriverName);
				// Handle anomaly case here, e.g., log it, notify the player, etc.
				CurrentMoney += 20; // Reward for catching an anomaly
				UGameplayStatics::PlaySound2D(this, MoneyGainSound);
			}
			else if (bIsInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid document detected for driver: %s"), *DailyDrivers[CurrentDriverIndex].DriverName);
				CurrentMoney += 10; // Reward for catching an invalid document
				UGameplayStatics::PlaySound2D(this, MoneyGainSound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Valid document rejected for driver: %s"), *DailyDrivers[CurrentDriverIndex].DriverName);
				CurrentMoney -= 10; // Deduct money for rejecting a valid document
				UGameplayStatics::PlaySound2D(this, MoneyLoseSound);
			}
		}
		// Check for game over condition
		if (CurrentMoney < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("GAME OVER: YOUR ARE POOR!"));
			// Here you can implement additional game over logic, such as showing a game over screen, stopping the game, etc.
		}

		// Get the player controller to update the HUD for the current money amount
		ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

		if (TollPlayerController) // Check if the cast was successful
		{
			TollPlayerController->UpdateHUDMoney(CurrentMoney); // Update the HUD with the new money amount
		}

		// 2. Trigger world objects (Just do null-checks for these)
		if (CurrentVehicle)
		{
			if (bIsApproved)
			{
				if (ControlledBarrier)
				{
					ControlledBarrier->OpenBarrier(); // Open the barrier for approved vehicles
				}
				CurrentVehicle->Depart(bIsApproved, ExitPath); // Pass the ExitPath to the vehicle for it to follow after departure
			}
			else
			{
				CurrentVehicle->Depart(bIsApproved, nullptr); // No exit path for rejected vehicles, they will just reverse back to the start
			}
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

