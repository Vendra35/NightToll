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

	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
	RootComponent = VehicleMesh;
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

	if (PathToFollow && !bHasArrived)
	{
		DistanceAlongSpline = DistanceAlongSpline + VehicleSpeed * DeltaTime;
		FVector NewLocation = PathToFollow->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator NewRotation = PathToFollow->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(NewLocation, NewRotation);

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
}

void ATollVehicle::SetupVehicle(USplineComponent* InPath, ATollManager* InManager)
{
	PathToFollow = InPath;
	OwningManager = InManager;
	DistanceAlongSpline = 0.0f; // Start at the beginning of the spline
	bHasArrived = false; // Reset arrival status

	if (PathToFollow)
	{
		FVector NewLocation = PathToFollow->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator NewRotation = PathToFollow->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
}

