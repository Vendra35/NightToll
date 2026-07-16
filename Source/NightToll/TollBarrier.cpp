// Fill out your copyright notice in the Description page of Project Settings.


#include "TollBarrier.h"

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

	if (bIsBarrierOpen)
	{
		FRotator TargetRotation(90.0f, 0.0f, 0.0f);
		FRotator CurrentRotation = BarrierHinge->GetRelativeRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
		BarrierHinge->SetRelativeRotation(NewRotation);
	}

}

void ATollBarrier::OpenBarrier()
{
	bIsBarrierOpen = true;
}

