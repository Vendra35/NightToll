// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TollVehicle.generated.h"

class ATollManager;
class USplineComponent;
class UStaticMeshComponent;

UCLASS()
class NIGHTTOLL_API ATollVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATollVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	UStaticMeshComponent* VehicleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	float VehicleSpeed = 500.0f; // Default speed, can be adjusted in the editor

	// This variable will keep track of how far along the spline the vehicle is, in terms of distance.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float DistanceAlongSpline;

	// This variable will hold a reference to the spline component that the vehicle should follow.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USplineComponent* PathToFollow;

	// This variable will hold a reference to the toll manager that owns this vehicle.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	ATollManager* OwningManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USceneComponent* RootComp;

	// This function will be called to set up the vehicle with the path it should follow and the manager that owns it.
	void SetupVehicle(USplineComponent* InPath, ATollManager* InManager);

	// This variable will be used to determine if the vehicle has reached the end of the spline and arrived at its destination.
	bool bHasArrived = false;

	// This function will be called when the vehicle is ready to depart, either after approval or rejection of the document.
	void Depart(bool bIsApproved, USplineComponent* NewExitPath = nullptr);

	// This variable will be used to determine if the vehicle is currently departing along the exit path.
	bool bIsDeparting = false;

	// This variable will be used to determine if the vehicle is approved to exit or not.
	bool bIsApprovedExit = false;
};
