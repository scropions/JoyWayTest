// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleGun.h"
#include "ExampleProjectile.h"
#include "Kismet/GameplayStatics.h"

AExampleGun::AExampleGun()
{
	// Create a gun mesh component
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	Mesh->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = false;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Mesh);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void AExampleGun::Fire_Implementation()
{
	// try and play the sound if specified
		// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{
			auto OwnerActor = Cast<APawn>(GetAttachParentActor());
				const FRotator SpawnRotation = OwnerActor->GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : OwnerActor->GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AExampleProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}

		CurrentAmmo -= PriceAmmo;

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}

}

void AExampleGun::Show()
{
	Mesh->SetVisibility(true);
}

void AExampleGun::Hide()
{
	Mesh->SetVisibility(false);
}