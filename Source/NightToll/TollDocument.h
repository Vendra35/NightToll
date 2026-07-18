// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"

#include "TollDocument.generated.h"

class UStaticMeshComponent;
class UTexture2D;
class USoundBase;

struct FDriverData;

USTRUCT(BlueprintType)
struct FDocumentData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DriverName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DriverPhoto;

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

	// Implement the Interact function from the InteractableInterface
	virtual void Interact_Implementation(AActor* Interactor) override;

	// Implement the StopInteract function from the InteractableInterface
	virtual void StopInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Document Data")
	FDocumentData DocumentDetails;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Document")
	UStaticMeshComponent* DocumentMesh;

	// Function to set up the document data based on the provided driver data
	void SetupDocumentData(const FDriverData& InDriverData);

	// Original transform of the document on the desk, used to reset its position when dropped
	FTransform OriginalDeskTransform;

	// Sound effect for picking up the document
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* DocumentPickupSound;

	// Sound effect for dropping the document
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* DocumentDropSound;

};
