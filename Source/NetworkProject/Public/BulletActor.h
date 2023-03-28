// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class NETWORKPROJECT_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category=BulletSettings)
	class USphereComponent* sphereComp;

	UPROPERTY(EditDefaultsOnly, Category=BulletSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = BulletSettings)
	float moveSpeed = 300;

	UPROPERTY(EditDefaultsOnly, Category = BulletSettings)
	class UParticleSystem* explosion;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Unreliable)
	void ServerSpawnEffect();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnEffect();
};
