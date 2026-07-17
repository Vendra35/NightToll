// Fill out your copyright notice in the Description page of Project Settings.


#include "TollVehicle.h"

#include "TollManager.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATollVehicle::ATollVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp; // RootComp->SetupAttachment(RootComponent); Is wrong because when RootComponent is first time created in the RAM , it is null. So we need to set the RootComponent to RootComp instead of attaching RootComp to RootComponent.

	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
	VehicleMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ATollVehicle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATollVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathToFollow && !bHasArrived && !bIsDeparting)
	{
		DistanceAlongSpline = DistanceAlongSpline + VehicleSpeed * DeltaTime;

		if (DistanceAlongSpline >= PathToFollow->GetSplineLength()) // Check if we've reached the end of the spline.
		{
			DistanceAlongSpline = PathToFollow->GetSplineLength(); // Align the distance exactly with the end of the track (so that the vehicle doesn't go off the tracks).
			bHasArrived = true; // Mark that we've arrived to prevent further movement.
			if (OwningManager)
			{
				OwningManager->OnVehicleArrived();
			}
		}
	}
	else if (PathToFollow && bIsDeparting && bIsApprovedExit) 
	{
		DistanceAlongSpline = DistanceAlongSpline + VehicleSpeed * DeltaTime;

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
	else if (PathToFollow && bIsDeparting && !bIsApprovedExit)
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
	bIsDeparting = true; // Mark the vehicle as departing
	bHasArrived = false; // Reset arrival status for departure

	if (bIsApproved)
	{
		PathToFollow = NewExitPath; // Set the new exit path for the vehicle to follow
		DistanceAlongSpline = 0.0f; // Reset distance for the new path
	}
}

