// Fill out your copyright notice in the Description page of Project Settings.


#include "TollBarrier.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ATollBarrier::ATollBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	BarrierBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierBaseMesh"));
	BarrierBaseMesh->SetupAttachment(RootComp);

	BarrierHinge = CreateDefaultSubobject<USceneComponent>(TEXT("BarrierHinge"));
	BarrierHinge->SetupAttachment(BarrierBaseMesh);

	BarrierArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierArmMesh"));
	BarrierArmMesh->SetupAttachment(BarrierHinge);

}

// Called when the game starts or when spawned
void ATollBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATollBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator TargetRotation;

	if (bIsBarrierOpen) // If the barrier is open, we want to rotate the barrier arm to a vertical position (90 degrees pitch).
	{
		TargetRotation = FRotator(90.0f, 0.0f, 0.0f);
	}
	else // If the barrier is closed, we want to rotate the barrier arm back to a horizontal position (0 degrees pitch).
	{
		TargetRotation = FRotator(0.0f, 0.0f, 0.0f);
	}
	// Smoothly interpolate the rotation of the barrier hinge towards the target rotation
	FRotator CurrentRotation = BarrierHinge->GetRelativeRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);
	BarrierHinge->SetRelativeRotation(NewRotation);
}

void ATollBarrier::OpenBarrier()
{
	// Set the barrier state to open
	bIsBarrierOpen = true;
	
	if (BarrierOpenSound)
	{
		// Play the barrier opening sound at the barrier's location
		UGameplayStatics::PlaySoundAtLocation(this, BarrierOpenSound, GetActorLocation());
	}
}

void ATollBarrier::CloseBarrier()
{
	// Set the barrier state to closed
	bIsBarrierOpen = false;
	
	if (BarrierCloseSound)
	{
		// Play the barrier closing sound at the barrier's location
		UGameplayStatics::PlaySoundAtLocation(this, BarrierCloseSound, GetActorLocation());
	}
}
