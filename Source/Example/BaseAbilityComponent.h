// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagChanged, const FGameplayTag&, UpdatedTag, bool, Exists);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTagChangedWithASC, UAbilitySystemComponent*, ASC, const FGameplayTag&, UpdatedTag, bool, Exists);

UCLASS()
class EXAMPLE_API UBaseAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UBaseAbilityComponent();

	UPROPERTY(EditDefaultsOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayEffect>> StartupEffects;
	UPROPERTY(BlueprintAssignable)
		FOnTagChanged OnTagChanged;
	//Same as On tag changed, but with ASC ref. Not changed default one, so won't need to change everything already bound. 
	UPROPERTY(BlueprintAssignable)
		FOnTagChangedWithASC OnTagChangedWithASC;

public:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;

	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey) override;

	virtual void InitializeComponent() override;
	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void OnTagUpdated(const FGameplayTag& Tag, bool TagExists) override;
	virtual void OnEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

};