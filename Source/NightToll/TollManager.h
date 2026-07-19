// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TollManager.generated.h"

class ATollVehicle;
class USplineComponent;
class ATollDocument;
class ATollBarrier;
class UTexture2D;

USTRUCT(BlueprintType)
struct FDriverData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DriverPhoto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverLookDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverLicense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpirationYear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LicensePlate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bHasInvalidDocument;

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

	// This array will hold the data for each driver that will be processed by the toll manager. We can set this up in the editor to define the drivers for the day.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDriverData> DailyDrivers;

	// This index will keep track of which driver we are currently processing in the DailyDrivers array.
	int32 CurrentDriverIndex = 0;

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

	// This will be the barrier that we will control to open or close based on the document decision. We can set this in the editor to link to the specific barrier actor in the scene.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Toll Setup")
	ATollBarrier* ControlledBarrier;

	// This will be the current vehicle that is moving along the spline. We can use this to check if a vehicle is currently in the toll area and to manage its behavior.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Toll Setup")
	ATollVehicle* CurrentVehicle;

	// This will be the spline that the vehicle will follow to exit the toll area. We can set this in the editor to define the exit path.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Toll Setup")
	USplineComponent* ExitPath;

	// This function will be called when the vehicle has exited the toll area, either after approval or rejection of the document. We can use this to reset the state and prepare for the next vehicle.
	void OnVehicleExited();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Economy") 
	int32 CurrentMoney = 10;

	// Sound to play when money is gained
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* MoneyGainSound;

	// Sound to play when money is lost
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* MoneyLoseSound;
};
