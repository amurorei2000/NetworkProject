
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

private:
	FString PrintInfo();

};
