// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float rotAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float moveSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	bool bIsJumping = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float pitch = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	bool bIsDead = false;

	UFUNCTION()
	void AnimNotify_WalkSound();

private:
	class ANetworkProjectCharacter* player;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings, meta=(AllowPrivateAccess))
	class USoundBase* source;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings, meta=(AllowPrivateAccess))
	class USoundAttenuation* attenu;

};
