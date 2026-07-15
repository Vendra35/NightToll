// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"

#include "TollDocument.generated.h"

class UStaticMeshComponent;

struct FDriverData;

USTRUCT(BlueprintType)
struct FDocumentData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpirationYear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LicensePlate;
};

UCLASS()
class NIGHTTOLL_API ATollDocument : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATollDocument();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Interactor) override;

	virtual void StopInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Document Data")
	FDocumentData DocumentDetails;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Document")
	UStaticMeshComponent* DocumentMesh;

	void SetupDocumentData(const FDriverData& InDriverData);

	FTransform OriginalDeskTransform;

};
