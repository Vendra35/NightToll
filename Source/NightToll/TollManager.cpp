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
		if (bIsApproved && ControlledBarrier)
		{
			ControlledBarrier->OpenBarrier();
		}
}

void ATollManager::SpawnDocumentForCurrentDriver()
{
	if (DocumentClassToSpawn && DailyDrivers.Num() > 0)
	{
		ATollDocument* SpawnedDocument = GetWorld()->SpawnActor<ATollDocument>(DocumentClassToSpawn, DocumentSpawnTransform);
		if (SpawnedDocument)
		{
			SpawnedDocument->SetupDocumentData(DailyDrivers[0]);
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
		ATollVehicle* SpawnedVehicle = GetWorld()->SpawnActor<ATollVehicle>(VehicleClassToSpawn);
		if (SpawnedVehicle)
		{
			SpawnedVehicle->SetupVehicle(VehiclePath, this);
		}
	}
}

