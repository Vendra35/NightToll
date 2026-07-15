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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USplineComponent* PathToFollow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	ATollManager* OwningManager;

	void SetupVehicle(USplineComponent* InPath, ATollManager* InManager);

	bool bHasArrived = false;
};
