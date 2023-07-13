// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseAbility.h"
#include "AbilitySystemComponent.h"


bool UAbilityBase::CommitAbilityWithAttributeCheck(FGameplayAttribute Attribute, TSubclassOf<UGameplayEffect> CostEffect)
{
	if (GetAbilitySystemComponentFromActorInfo()->HasAttributeSetForAttribute(Attribute))
	{
		//CostGameplayEffectClass = CostEffect;
		return CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}

	return true;
}

void UAbilityBase::CancelAllASCAbilities(UAbilitySystemComponent* ASC, const FGameplayTagContainer WithTags, const FGameplayTagContainer WithoutTags)
{
	ASC->CancelAbilities(&WithTags, &WithoutTags, this);
}

bool UAbilityBase::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (!CostGE) return true;

	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent) return false;

	for (const auto Modifier : CostGE->Modifiers)
	{
		if (!AbilitySystemComponent->HasAttributeSetForAttribute(Modifier.Attribute))
		{
			return true;
		}
	}

	return UGameplayAbility::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

float UAbilityBase::GetCostValue_Implementation() const
{
	return 0;
}
