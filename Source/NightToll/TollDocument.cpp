// Fill out your copyright notice in the Description page of Project Settings.

#include "TollDocument.h"

#include "InspectorCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

#include "TollPlayerController.h"
#include "TollDocumentWidget.h"
#include "TollManager.h"
#include "Kismet/GameplayStatics.h" // PlayerController'ý dünyadan çekmek için

// Sets default values
ATollDocument::ATollDocument()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DocumentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DocumentMesh"));
	RootComponent = DocumentMesh;

}

// Called when the game starts or when spawned
void ATollDocument::BeginPlay()
{
	Super::BeginPlay();
	OriginalDeskTransform = GetActorTransform(); // Doðduðum yeri aklýma yazdým.
}

// Called every frame
void ATollDocument::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATollDocument::Interact_Implementation(AActor* Interactor)
{
	// 1. Oyuncunun beynine (Player Controller) dünyadan doðrudan ulaþ
	ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	// 2. Güvenlik Zýrhý: Controller var mý ve Arayüz (Widget) RAM'de yaratýlmýþ mý?
	if (TollPlayerController && TollPlayerController->DocumentWidgetInstance)
	{
		// 3. Kendi içindeki verileri doðrudan UI'ýn fonksiyonuna ateþle
		TollPlayerController->DocumentWidgetInstance->UpdateDocumentUI(DocumentDetails.DriverName, DocumentDetails.LicensePlate, DocumentDetails.ExpirationYear);
		TollPlayerController->ShowDocumentUI();
	}

	AInspectorCharacter* OurCharacter = Cast<AInspectorCharacter>(Interactor);
	if (OurCharacter)
	{
		DocumentMesh->SetSimulatePhysics(false);
		SetActorEnableCollision(false);
		AttachToComponent(OurCharacter->FirstPersonCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRelativeLocation(FVector(40.0f, 20.0f, -15.0f));
		SetActorRelativeRotation(FRotator(0.0f, 90.0f, 45.0f));
	}
}

void ATollDocument::StopInteract_Implementation(AActor* Interactor)
{
	// DocumentMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// Belgeyi elimize alýrken Interact_Implementation içinde DocumentMesh->AttachToComponent(...) yazmamýþtýk; doðrudan aktörün kendisine AttachToComponent(...) demiþtik. 
	// Kodun simetrik ve okunaklý olmasý için, alýrken aktörü bütünüyle yapýþtýrdýysak, býrakýrken de bütünüyle sökmemiz gerekir.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	DocumentMesh->SetSimulatePhysics(false);
	// Hafýzadaki masanýn o ilk konumuna sýfýr hata ile geri dön.
	SetActorTransform(OriginalDeskTransform);
	// 1. Oyuncunun beynine (Player Controller) dünyadan doðrudan ulaþ
	ATollPlayerController* TollPlayerController = Cast<ATollPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (TollPlayerController)
	{
		TollPlayerController->HideDocumentUI();
	}
}

void ATollDocument::SetupDocumentData(const FDriverData& InDriverData)
{
	DocumentDetails.DriverName = InDriverData.DriverName;

	DocumentDetails.LicensePlate = InDriverData.LicensePlate;

	DocumentDetails.ExpirationYear = InDriverData.ExpirationYear;

	DocumentDetails.bIsFake = InDriverData.bHasAnomaly;
}

