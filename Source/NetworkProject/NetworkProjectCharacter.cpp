// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "BulletActor.h"
#include "Components/WidgetComponent.h"
#include "PlayerInfoWidget.h"
#include "PlayerAnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// ANetworkProjectCharacter

ANetworkProjectCharacter::ANetworkProjectCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	playerInfoUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Info UI"));
	playerInfoUI->SetupAttachment(GetMesh());

}

void ANetworkProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HasAuthority())
	{
		SetHealth(maxHP);
	}

	infoWidget = Cast<UPlayerInfoWidget>(playerInfoUI->GetWidget());
	playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

void ANetworkProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bIsDead)
	return;


	// 상태 정보를 출력한다.
	DrawDebugString(GetWorld(), GetActorLocation(), PrintInfo(), nullptr, FColor::White, 0.0f, true, 1.0f);

	if (HasAuthority())
	{
		number++;
		repNumber++;
	}

	infoWidget->SetHealthBar(curHP);

	if (curHP <= 0)
	{
		bIsDead = true;
		GetCharacterMovement()->DisableMovement();
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bUseControllerRotationYaw = false;
		FollowCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
	}

}

FString ANetworkProjectCharacter::PrintInfo()
{
	FString infoText = "";

#pragma region RoleInfo
	/*FString myLocalRole = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
	FString myRemoteRole = UEnum::GetValueAsString<ENetRole>(GetRemoteRole());
	FString myConnection = GetNetConnection() != nullptr ? TEXT("Valid") : TEXT("Invalid");
	FString myOwner = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	FString name = this->GetName();

	infoText = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nNet Connection: %s\nOwner: %s\nName: %s"), *myLocalRole, *myRemoteRole, *myConnection, *myOwner, *name);*/
#pragma endregion

#pragma region RepOrNot
	//infoText = FString::Printf(TEXT("Number: %d\nReplicated Number: %d"), number, repNumber);
#pragma endregion

	return infoText;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANetworkProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANetworkProjectCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANetworkProjectCharacter::Look);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ANetworkProjectCharacter::Fire);

	}

}

void ANetworkProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANetworkProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANetworkProjectCharacter::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Query Fire!"));
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString("Query Fire!"), true, FVector2D(1.2f));

	ServerFire();
}


// 서버에 요청하는 함수
void ANetworkProjectCharacter::ServerFire_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Server Fire!"));
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString("Server Fire!"), true, FVector2D(1.2f));

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 100, GetActorRotation(), param);
	bullet->SetOwner(this);
	//bullet->SetOwner(nullptr);

	MulticastFire();
	//ClientFire();

}

bool ANetworkProjectCharacter::ServerFire_Validate()
{
	return true;
}

// 서버로부터 전달되는 함수
void ANetworkProjectCharacter::MulticastFire_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Multicast Fire!"));
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString("Multicast Fire!"), true, FVector2D(1.2f));

	if (fireMontage != nullptr)
	{
		PlayAnimMontage(fireMontage);
	}
}


void ANetworkProjectCharacter::ClientFire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Client Fire!"));
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString("Client Fire!"), true, FVector2D(1.2f));


}

void ANetworkProjectCharacter::SetHealth(int32 value)
{
	curHP = FMath::Min(maxHP, value);
}

void ANetworkProjectCharacter::AddHealth(int32 value)
{
	curHP = FMath::Clamp(curHP + value, 0, maxHP);
}

void ANetworkProjectCharacter::ServerDamageProcess_Implementation(int32 value)
{
	AddHealth(value);
	MulticastDamageProcess();
}

void ANetworkProjectCharacter::MulticastDamageProcess_Implementation()
{
	if (curHP > 0)
	{
		if(hitMontage != nullptr)
		{
			PlayAnimMontage(hitMontage);
		}

	}
	else
	{
		// 죽음 변수 변경
		
		if (playerAnim != nullptr)
		{
			playerAnim->bIsDead = true;
		}
	}
}

void ANetworkProjectCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ANetworkProjectCharacter, repNumber);
	//DOREPLIFETIME_CONDITION(ANetworkProjectCharacter, repNumber, COND_OwnerOnly);

	DOREPLIFETIME(ANetworkProjectCharacter, curHP);
	DOREPLIFETIME(ANetworkProjectCharacter, ammo);
	DOREPLIFETIME(ANetworkProjectCharacter, myName);
	DOREPLIFETIME(ANetworkProjectCharacter, bIsDead);

}




