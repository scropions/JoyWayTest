// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExampleGun.generated.h"


UCLASS()
class AExampleGun : public AActor
{
	GENERATED_BODY()
public:
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AExampleProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int PriceAmmo;
public:
	AExampleGun();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void Show();
	UFUNCTION(BlueprintCallable)
		void Hide();

	virtual void Fire_Implementation();
protected:

private:
};



