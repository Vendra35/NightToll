// Fill out your copyright notice in the Description page of Project Settings.


#include "TollVehicle.h"

#include "TollManager.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/AudioComponent.h"

// Sets default values
ATollVehicle::ATollVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root scene component for the vehicle and set it as the root component of the actor.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp; // RootComp->SetupAttachment(RootComponent); Is wrong because when RootComponent is first time created in the RAM , it is null. So we need to set the RootComponent to RootComp instead of attaching RootComp to RootComponent.

	// Create a static mesh component for the vehicle and attach it to the root component.
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
	VehicleMesh->SetupAttachment(RootComp);

	// Create an audio component for the engine sound and attach it to the root component.
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSoundComponent"));
	EngineSoundComponent->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ATollVehicle::BeginPlay()
{
	Super::BeginPlay();

	// Create a dynamic material instance for the vehicle mesh to allow for runtime modifications.
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(VehicleMesh->GetMaterial(0), this);

	if (DynMaterial) // Check if the dynamic material instance was successfully created.
	{
		// Set a random color for the vehicle's body by modifying the "BodyColor" parameter of the dynamic material.
		DynMaterial->SetVectorParameterValue(FName("BodyColor"), FLinearColor::MakeRandomColor());
		VehicleMesh->SetMaterial(0, DynMaterial);
	}
}

// Called every frame
void ATollVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PathToFollow && !bHasArrived && !bIsDeparting) // Only move the vehicle if it has a path, hasn't arrived, and isn't departing.
	{
		DistanceAlongSpline = DistanceAlongSpline + VehicleSpeed * DeltaTime; // Update the distance along the spline based on the vehicle's speed and the time elapsed since the last frame.

		if (DistanceAlongSpline >= PathToFollow->GetSplineLength()) // Check if we've reached the end of the spline.
		{
			DistanceAlongSpline = PathToFollow->GetSplineLength(); // Align the distance exactly with the end of the track (so that the vehicle doesn't go off the tracks).
			bHasArrived = true; // Mark that we've arrived to prevent further movement.

			if (EngineSoundComponent) // Check if the engine sound component exists before trying to modify it.
			{
				// Reduce the engine sound pitch and volume to indicate that the vehicle has stopped.
				EngineSoundComponent->SetPitchMultiplier(0.5f);
				EngineSoundComponent->SetVolumeMultiplier(0.6f);
			}

			if (OwningManager)
			{
				OwningManager->OnVehicleArrived(); // Notify the Toll Manager that the vehicle has arrived at its destination.
			}
		}
	}
	else if (PathToFollow && bIsDeparting && bIsApprovedExit) // Only move the vehicle if it has a path, is departing, and is approved to exit.
	{
		DistanceAlongSpline = DistanceAlongSpline + VehicleSpeed * DeltaTime; // Update the distance along the spline based on the vehicle's speed and the time elapsed since the last frame.

		if (DistanceAlongSpline >= PathToFollow->GetSplineLength()) // Check if we've reached the end of the spline.
		{
			DistanceAlongSpline = PathToFollow->GetSplineLength(); // Align the distance exactly with the end of the track (so that the vehicle doesn't go off the tracks).
			if (OwningManager)
			{
				OwningManager->CurrentVehicle = nullptr; // Clear the reference to the current vehicle in the Toll Manager.
				OwningManager->OnVehicleExited(); // Notify the Toll Manager that the vehicle has exited.
				Destroy(); // Destroy the vehicle actor since it has successfully exited.
				return; // Exit the function to avoid further processing after destruction.
			}
		}
	}
	else if (PathToFollow && bIsDeparting && !bIsApprovedExit) // Only move the vehicle if it has a path, is departing, and is NOT approved to exit (i.e., rejected).
	{
		DistanceAlongSpline = DistanceAlongSpline - VehicleSpeed * DeltaTime; // Move the vehicle backward along the spline for rejection.

		if (DistanceAlongSpline <= 0.0f) // Check if we've reached the start of the spline.
		{
			DistanceAlongSpline = 0.0f; // Align the distance exactly with the start of the track (so that the vehicle doesn't go off the tracks).
			if (OwningManager)
			{
				OwningManager->CurrentVehicle = nullptr; // Clear the reference to the current vehicle in the Toll Manager.
				OwningManager->OnVehicleExited(); // Notify the Toll Manager that the vehicle has exited.
				Destroy(); // Destroy the vehicle actor since it has been rejected and returned to the start.
				return; // Exit the function to avoid further processing after destruction.
			}
		}
	}
	// Update the vehicle's position and rotation based on the current distance along the spline.
	FVector NewLocation = PathToFollow->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator NewRotation = PathToFollow->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void ATollVehicle::SetupVehicle(USplineComponent* InPath, ATollManager* InManager)
{
	PathToFollow = InPath; // Set the spline path for the vehicle to follow
	OwningManager = InManager; // Set the owning manager reference
	DistanceAlongSpline = 0.0f; // Start at the beginning of the spline
	bHasArrived = false; // Reset arrival status

	if (PathToFollow)
	{
		FVector NewLocation = PathToFollow->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator NewRotation = PathToFollow->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
}

void ATollVehicle::Depart(bool bIsApproved, USplineComponent* NewExitPath)
{
	bIsApprovedExit = bIsApproved; // Store the approval status for later use

	PendingExitPath = NewExitPath; // Store the new exit path for later use

	// Start a timer to delay the departure, allowing for any necessary animations or effects before the vehicle starts moving.
	GetWorld()->GetTimerManager().SetTimer(DepartTimerHandle, this, &ATollVehicle::StartDriving, 2.0f, false);
}

void ATollVehicle::StartDriving()
{
	bIsDeparting = true; // Mark the vehicle as departing
	bHasArrived = false; // Reset arrival status for departure

	if (bIsApprovedExit) // If the vehicle is approved to exit, set the path to follow to the new exit path.
	{
		DistanceAlongSpline = 0.0f; // Reset distance along the spline for departure
		PathToFollow = PendingExitPath; // Set the path to follow to the new exit path
	}

	if (EngineSoundComponent) // Check if the engine sound component exists before trying to modify it.
	{
		// Increase the engine sound pitch and volume to indicate that the vehicle is starting to drive.
		EngineSoundComponent->SetPitchMultiplier(1.2f);
		EngineSoundComponent->SetVolumeMultiplier(1.0f);
	}
}

