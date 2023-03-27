// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnershipActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "../NetworkProjectCharacter.h"


AOwnershipActor::AOwnershipActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ���� on/off �Լ�
	//SetReplicates(true);
	//SetReplicateMovement(true);
}

void AOwnershipActor::BeginPlay()
{
	Super::BeginPlay();

	// �޽��� ������ ���̳��� ��Ƽ���� �ν��Ͻ��� �����Ѵ�.
	UMaterialInterface* base_mat = meshComp->GetMaterial(0);
	if (base_mat != nullptr)
	{
		mat = UMaterialInstanceDynamic::Create(base_mat, this);
		meshComp->SetMaterial(0, mat);
	}

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AOwnershipActor::OnOverlap);

	// ����, ���� ���Ͷ��...
	//if (GetLocalRole() == ROLE_Authority)
	if (HasAuthority())
	{
		// ������ ������ �����Ѵ�.
		moveDirection = FMath::VRand();
		moveDirection.Z = FMath::Abs(moveDirection.Z);

		// 5�ʿ� �� ���� testNumber�� ������ �����Ѵ�.
		FTimerHandle repeatTimer;
		GetWorldTimerManager().SetTimer(repeatTimer, FTimerDelegate::CreateLambda([&]() {testNumber = FMath::Rand(); }), 5.0f, true);
	}
}

void AOwnershipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ������ ����Ѵ�.
	DrawDebugString(GetWorld(), GetActorLocation(), PrintInfo(), nullptr, FColor::White, 0.0f, true, 1.0f);

	//SetActorLocation(GetActorLocation() + moveDirection * 100 * DeltaTime);

	if (HasAuthority())
	{
		CheckOwner();
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 500.0f, 30, FColor::Cyan, false, 0.0f, 0, 1);

	//ChangeColor();
}

void AOwnershipActor::ScreenLog()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::FromInt(testNumber), true, FVector2D(1.5f));
}

FString AOwnershipActor::PrintInfo()
{
#pragma region RoleInfo
	FString myLocalRole = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
	FString myRemoteRole = UEnum::GetValueAsString<ENetRole>(GetRemoteRole());
	FString myConnection = GetNetConnection() != nullptr ? TEXT("Valid") : TEXT("Invalid");
	FString myOwner = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");

	FString infoText = FString::Printf(TEXT("Local Role: %s\nRemote Role: %s\nNet Connection: %s\nOwner: %s"), *myLocalRole, *myRemoteRole, *myConnection, *myOwner);
#pragma endregion

#pragma region RandomVector
	//FString infoText = FString::Printf(TEXT("%.2f, %.2f, %.2f"), moveDirection.X, moveDirection.Y, moveDirection.Z);
#pragma endregion

	return infoText;
}

// ������ ���� �ȿ� �÷��̾ �ִٸ� ���� ����� �÷��̾� ĳ���͸� Owner�� �����Ѵ�.
void AOwnershipActor::CheckOwner()
{
	float maxDistance = 500;
	ANetworkProjectCharacter* player = nullptr;

	for (TActorIterator<ANetworkProjectCharacter> it(GetWorld()); it; ++it)
	{
		float distance = this->GetDistanceTo(*it);

		if (distance < maxDistance)
		{
			maxDistance = distance;
			player = *it;
		}
	}

	if (player != nullptr && GetOwner() != player)
	{
		SetOwner(player);
	}
}

void AOwnershipActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		// �浹�� ����� �÷��̾���...
		if (OtherActor->IsA<ANetworkProjectCharacter>())
		{
			// ��Ƽ���� ������ �����ϰ� �����Ѵ�.
			matColor = FMath::VRand();
			matColor = matColor.GetAbs();
			ChangeColor();
		}
	}
}

void AOwnershipActor::ChangeColor()
{
	UE_LOG(LogTemp, Warning, TEXT("Change!!!"));

	// "MyColor" �Ķ������ ���� matColor ������ �����Ѵ�.
	mat->SetVectorParameterValue(FName("MyColor"), (FLinearColor)matColor);
}

// ������ ���� ����ϱ� ���� �Լ�
void AOwnershipActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOwnershipActor, moveDirection);
	DOREPLIFETIME(AOwnershipActor, testNumber);
	DOREPLIFETIME(AOwnershipActor, matColor);
}


// 1. �÷��̾� ĳ���Ͱ� �浹�ϸ� ������ �������� �����Ѵ�.
// 2. ���� ������ ��� Ŭ���̾�Ʈ���� �����ϰ� ������ �Ѵ�.


