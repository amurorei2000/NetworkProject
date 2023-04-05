// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "../NetworkProjectCharacter.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	player = Cast<ANetworkProjectCharacter>(GetOwningActor());

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 애니메이션 미리보기 예외 처리
	if (player == nullptr)
	{
		return;
	}

	moveSpeed = player->GetVelocity().Length();

#pragma region Calculate Angle
	////FVector forwarVec = player->GetActorForwardVector();
	//FVector forwarVec = FRotationMatrix(player->GetActorRotation()).GetUnitAxis(EAxis::X);
	//FVector normalVelocity = player->GetVelocity().GetSafeNormal2D();

	//// 회전 각도 계산
	//double cosTheta = FVector::DotProduct(forwarVec, normalVelocity);
	//double rad = FMath::Acos(cosTheta);
	//float degree = FMath::RadiansToDegrees(rad);

	//FVector rightVec = FRotationMatrix(player->GetActorRotation()).GetUnitAxis(EAxis::Y);

	//// 회전 방향 계산
	//cosTheta = FVector::DotProduct(rightVec, normalVelocity);
	//rotAngle = cosTheta >=0 ? degree : degree * -1.0f;
#pragma endregion

	//rotAngle = CalculateDirection(player->GetVelocity(), player->GetActorRotation());
	rotAngle = UKismetAnimationLibrary::CalculateDirection(player->GetVelocity(), player->GetActorRotation());

	bIsJumping = player->GetCharacterMovement()->IsFalling();

	FRotator viewRot = player->GetBaseAimRotation();
	FRotator playerRot = player->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	pitch = FMath::Clamp(deltaRot.GetNormalized().Pitch, -45.0f, 45.0f);
	
	// 죽었는지 확인
	bIsDead = player->IsDead();
}

void UPlayerAnimInstance::AnimNotify_WalkSound()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), source, player->GetActorLocation(), 1, 1, 0, attenu);
}
