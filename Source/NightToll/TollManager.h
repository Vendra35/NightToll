// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TollManager.generated.h"

class ATollVehicle;
class USplineComponent;
class ATollDocument;

USTRUCT(BlueprintType)
struct FDriverData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverLookDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverLicense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpirationYear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LicensePlate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasAnomaly;
};

UCLASS()
class NIGHTTOLL_API ATollManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATollManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDriverData> DailyDrivers;

	// Document is approved or rejected, this function will be called to process the decision
	void ProcessDocumentDecision(bool bIsApproved);

	// Spawns a document for the current driver in the DailyDrivers array
	void SpawnDocumentForCurrentDriver();

	// Called by the vehicle when it reaches the end of the spline
	void OnVehicleArrived();

	// Spawns a vehicle and sets it up to move along the spline. This can be called after processing the document decision, for example.
	void SpawnVehicle();

	UPROPERTY(EditDefaultsOnly, Category = "Toll Setup")
	TSubclassOf<ATollDocument> DocumentClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Toll Setup")
	FTransform DocumentSpawnTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Toll Setup")
	USplineComponent* VehiclePath;

	// This will be the class of the vehicle that we will spawn and move along the spline. We can set this in the editor to use different types of vehicles.
	UPROPERTY(EditDefaultsOnly, Category = "Toll Setup")
	TSubclassOf<ATollVehicle> VehicleClassToSpawn;
};
