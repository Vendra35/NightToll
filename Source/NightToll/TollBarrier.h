// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TollBarrier.generated.h"

class USoundBase;

UCLASS()
class NIGHTTOLL_API ATollBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATollBarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barrier")
	UStaticMeshComponent* BarrierBaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barrier")
	USceneComponent* BarrierHinge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barrier")
	UStaticMeshComponent* BarrierArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Barrier")
	USceneComponent* RootComp;

	// This variable will be used to determine if the barrier is currently open or closed.
	bool bIsBarrierOpen = false;

	// This function will be called to open the barrier.
	void OpenBarrier();

	// This function will be called to close the barrier.
	void CloseBarrier();

	// Sound effect for opening the barrier
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* BarrierOpenSound;

	// Sound effect for closing the barrier
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* BarrierCloseSound;

};
