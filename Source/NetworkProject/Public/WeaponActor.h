// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class NETWORKPROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category=MySettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category=MySettings)
	class USkeletalMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Replicated, Category=MySettings)
	int32 ammo;

	UPROPERTY(EditDefaultsOnly, Replicated, Category=MySettings)
	float reloadTime;

	UPROPERTY(EditDefaultsOnly, Replicated, Category=MySettings)
	float damagePower;

	UFUNCTION(Server, Unreliable)
	void ServerGrabWeapon(class ANetworkProjectCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastGrabWeapon(class ANetworkProjectCharacter* player);

	UFUNCTION(Server, Unreliable)
	void ServerReleaseWeapon(class ANetworkProjectCharacter* player);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastReleaseWeapon(class ANetworkProjectCharacter* player);

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
