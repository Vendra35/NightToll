// Fill out your copyright notice in the Description page of Project Settings.


#include "InspectorCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableInterface.h"
#include "TollDocument.h"
#include "Camera/CameraComponent.h"

// Sets default values
AInspectorCharacter::AInspectorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	HeldObject = nullptr;

}

// Called when the game starts or when spawned
void AInspectorCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer)
		{
			UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
			if (SubSystem)
			{
				SubSystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void AInspectorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInspectorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInspectorCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInspectorCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AInspectorCharacter::Interact);
	}

}

void AInspectorCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInspectorCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AInspectorCharacter::Interact()
{
	if (HeldObject == nullptr)
	{
		// Implement interaction logic here
		if (FirstPersonCamera)
		{
			FVector StartLocation = FirstPersonCamera->GetComponentLocation();
			// Perform a line trace or other interaction logic based on the camera's location and forward vector
			FVector ForwardVector = FirstPersonCamera->GetForwardVector();
			FVector EndLocation = StartLocation + (ForwardVector * InteractionDistance); // Example interaction range
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
			if (bHit)
			{
				DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Green, false, 2.0f);
				AActor* HitActor = HitResult.GetActor();
				if (HitActor)
				{
					//// Neden Cast Edip Çaðýrmadýk?
					//Kurslarda sana þunu öðretmiþlerdir :
					//IInteractableInterface * Interface = Cast<IInteractableInterface>(HitActor);
					//Interface->Interact();
					//Saf C++'ta bu kusursuz çalýþýr. Ancak biz arayüzümüzü BlueprintNativeEvent makrosuyla yarattýk. 
					//Eðer sen bu arayüzü C++'ta deðil de Blueprint'te (örneðin o radyoda) uygularsan, bu standart C++ Cast iþlemi baþarýsýz olur. 
					//Çünkü C++'ýn standart Sanal Tablosu(VTable), Blueprint'in görsel düðümlerini göremez.
					if (HitActor->Implements<UInteractableInterface>())
					{
						////. Sihirli Köprü: Execute_
						//Sen o makroyu(BlueprintNativeEvent) yazdýðýn an, Unreal Header Tool(UHT) senin için arka planda Execute_Interact adýnda Statik(Static) bir yönlendirici(Router) yaratýr.
						//Bu fonksiyon adeta bir trafik polisidir.Sen topu ona atarsýn(Execute_Interact(HitActor, this)), o arka planda þu sorguyu yapar :
						//"Bu HitActor arayüzü C++'ta mý eziyor, yoksa Blueprint'te mi? Blueprint'teyse görsel node'u çalýþtýr, C++'taysa _Implementation uzantýlý fonksiyonu çalýþtýr."

						//Yani Execute_, motorun iki farklý dünyasýný(C++ ve Blueprint) çöktürmeden konuþturan o nihai köprünün adýdýr.
						IInteractableInterface::Execute_Interact(HitActor, this);
						HeldObject = HitActor;
					}
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);
			}

		}
	}
	else
	{
		if (HeldObject->Implements<UInteractableInterface>())
		{
			IInteractableInterface::Execute_StopInteract(HeldObject, this);
			UE_LOG(LogTemp, Display, TEXT("The object we are holding is: %s being released"), *HeldObject->GetActorNameOrLabel());
			HeldObject = nullptr;
		}
		
	}
}

void AInspectorCharacter::DisposeDocument()
{
// Sana bir Baþ Programcý (Lead Programmer) sýrrý vereyim. 
// Destroy() fonksiyonu, Unreal Engine çekirdeðinde zaten en temel AActor sýnýfýna ait olan evrensel bir motor komutudur.
// Yani o kodda hiç Cast iþlemi yapmadan, doðrudan þu þekilde yazsaydýn da kod çalýþýr ve belgeyi silerdi:
// if (HeldObject)
//	{
//		HeldObject->Destroy();
//	}
// Neden Cast yaptýn peki? Çünkü "Zýrhlama" refleksin devreye girdi ve bu mükemmel bir alýþkanlýktýr. 
// Eðer Destroy() demek yerine belgenin içinden DocumentDetails verisini okumak isteseydin, o Cast iþlemine mecburdun (Çünkü standart bir Actor'ün içinde DocumentDetails yoktur). 
// Bu mühendislik refleksini kaybetme.
	ATollDocument* DocumentInHand = Cast<ATollDocument>(HeldObject);
	if (DocumentInHand)
	{
		DocumentInHand->Destroy();
	}
	HeldObject = nullptr;
}

