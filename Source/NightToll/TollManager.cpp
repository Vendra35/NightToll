// Fill out your copyright notice in the Description page of Project Settings.


#include "TollManager.h"

#include "TollBarrier.h"
#include "TollDocument.h"
#include "TollVehicle.h"
#include "Components/SplineComponent.h"

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

		if (bIsApproved && ControlledBarrier && CurrentVehicle)
		{
			ControlledBarrier->OpenBarrier();
			CurrentVehicle->Depart(bIsApproved, ExitPath);
		}
		else if (!bIsApproved && CurrentVehicle)
		{
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
	SpawnDocumentForCurrentDriver();
}

void ATollManager::SpawnVehicle()
{
	if (VehicleClassToSpawn)
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
	else
	{
		UE_LOG(LogTemp, Display, TEXT("All drivers have been processed for the day."));
	}
}

