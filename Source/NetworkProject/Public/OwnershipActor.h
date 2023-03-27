
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnershipActor.generated.h"

UCLASS()
class NETWORKPROJECT_API AOwnershipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOwnershipActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=MyActor)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category=MyActor)
	class UStaticMeshComponent* meshComp;

	UFUNCTION()
	void ScreenLog();

private:
	FString PrintInfo();
	void CheckOwner();
	class UMaterialInstanceDynamic* mat;
	
	UPROPERTY(Replicated)
	FVector moveDirection;

	UPROPERTY(ReplicatedUsing = ScreenLog)
	int32 testNumber = 0;

	UPROPERTY(ReplicatedUsing = ChangeColor)
	FVector matColor;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ChangeColor();
};
