// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "../NetworkProjectCharacter.h"


AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(FName("WeaponPreset"));
	boxComp->SetSimulatePhysics(true);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnOverlap);

}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ANetworkProjectCharacter* player = Cast<ANetworkProjectCharacter>(OtherActor);

		// 만일, 총을 쥔 플레이어가 아니라면...
		if (player != nullptr && player->owningWeapon == nullptr)
		{
			player->ammo = ammo;
			ServerGrabWeapon(player);
		}
	}
}

void AWeaponActor::ServerGrabWeapon_Implementation(ANetworkProjectCharacter* player)
{
	SetOwner(player);
	MulticastGrabWeapon(player);
}

void AWeaponActor::MulticastGrabWeapon_Implementation(ANetworkProjectCharacter* player)
{
	if (player != nullptr)
	{
		player->owningWeapon = this;
		boxComp->SetSimulatePhysics(false);
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}


void AWeaponActor::ServerReleaseWeapon_Implementation(class ANetworkProjectCharacter* player)
{
	MulticastReleaseWeapon(player);
	SetOwner(nullptr);
	player->ammo = 0;
}

void AWeaponActor::MulticastReleaseWeapon_Implementation(class ANetworkProjectCharacter* player)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	boxComp->SetSimulatePhysics(true);

	FTimerHandle releaseHandle;
	GetWorldTimerManager().SetTimer(releaseHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		}), 3.0f, false);

	player->owningWeapon = nullptr;
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponActor, ammo);
	DOREPLIFETIME(AWeaponActor, reloadTime);
	DOREPLIFETIME(AWeaponActor, damagePower);

}