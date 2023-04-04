// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NetworkProjectCharacter.generated.h"


UCLASS(config = Game)
class ANetworkProjectCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* playerInfoUI;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReleaseAction;


public:
	ANetworkProjectCharacter();


protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire();
	void ReleaseWeapon();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerFire();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire(bool bHasAmmo);

	UFUNCTION(Client, Unreliable)
	void ClientFire();

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class ABulletActor> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	UAnimMontage* fireMontage;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	UAnimMontage* hitMontage;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	UAnimMontage* noAmmoMontage;

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	int32 maxHP = 100;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = MySettings)
	int32 curHP;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = MySettings)
	int32 ammo;

	UPROPERTY(VisibleDefaultsOnly, Category = MySettings)
	class UPlayerInfoWidget* infoWidget;

	UPROPERTY()
	class AWeaponActor* owningWeapon;

	UFUNCTION()
	void SetHealth(int32 value);

	UFUNCTION()
	void AddHealth(int32 value);

	UFUNCTION(Server, Unreliable)
	void ServerDamageProcess(int32 value);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDamageProcess();

	UFUNCTION(Server, Unreliable)
	void ServerSetName(const FString& name);

	FORCEINLINE int32 GetHealth() { return curHP; };
	FORCEINLINE int32 GetAmmo() { return ammo; };
	FORCEINLINE bool IsDead() { return bIsDead; };


private:
	FString PrintInfo();
	int32 number;
	bool bIsDead = false;
	class UPlayerAnimInstance* playerAnim;
	class UServerGameInstance* gameInstance;

	UPROPERTY(Replicated)
	int32 repNumber;

	UPROPERTY(Replicated)
	FString myName;
	
	UPROPERTY(Replicated)
	bool bFireDelay = false;

	void DieProcess();
	void ChangeSpectatorMode();
};

