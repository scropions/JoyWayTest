// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseAbility.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	Confirm			UMETA(DisplayName = "Confirm"),
	Cancel			UMETA(DisplayName = "Cancel"),
	Fire			UMETA(DisplayName = "Fire"),
	NextWeapon		UMETA(DisplayName = "NextWeapon"),
};

UCLASS()
class EXAMPLE_API UAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityBase")
	EAbilityInputID KeyInput = EAbilityInputID::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActivateOnInput = true;

public:
	//This function check if attribute exist on Instigator. If not -> Always true, if exist perform commit
	UFUNCTION(BlueprintCallable)
	bool CommitAbilityWithAttributeCheck(FGameplayAttribute Attribute, TSubclassOf<UGameplayEffect> CostEffect);

	UFUNCTION(BlueprintNativeEvent)
	float GetCostValue() const;

	UFUNCTION(BlueprintCallable)
	void CancelAllASCAbilities(UAbilitySystemComponent* ASC, const FGameplayTagContainer WithTags, const FGameplayTagContainer WithoutTags);

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
};
