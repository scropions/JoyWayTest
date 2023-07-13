// Copyright Epic Games, Inc. All Rights Reserved.
#include "BaseAbilityComponent.h"
#include "Ability/BaseAbility.h"


UBaseAbilityComponent::UBaseAbilityComponent()
{
	//UBaseAbilityComponent.bCanEverTick = false;

}

void UBaseAbilityComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if (Cast<UAbilityBase>(AbilitySpec.Ability) &&
		Cast<UAbilityBase>(AbilitySpec.Ability)->KeyInput != EAbilityInputID::None)
	{
		AbilitySpec.InputID = static_cast<int32>(Cast<UAbilityBase>(AbilitySpec.Ability)->KeyInput);
	}

	UAbilitySystemComponent::OnGiveAbility(AbilitySpec);
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilityComponent::OnEffectApplied);
}

FActiveGameplayEffectHandle UBaseAbilityComponent::ApplyGameplayEffectSpecToSelf(
	const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	if (GetWorld())
	{
		return UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
	}

	return FActiveGameplayEffectHandle();
}

void UBaseAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (const auto DefaultAbility : DefaultAbilities)
	{
		GiveAbility(DefaultAbility);
	}

	for (const auto Effect : StartupEffects)
	{
		FGameplayEffectContextHandle EffectContext = MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, 1, EffectContext);
		if (!SpecHandle.IsValid()) break;

		FActiveGameplayEffectHandle GEHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(), FPredictionKey());
	}

}

void UBaseAbilityComponent::OnTagUpdated(const FGameplayTag& Tag, bool TagExists)
{
	Super::OnTagUpdated(Tag, TagExists);

	if (OnTagChanged.IsBound())
	{
		OnTagChanged.Broadcast(Tag, TagExists);
	}
	if (OnTagChangedWithASC.IsBound())
	{
		OnTagChangedWithASC.Broadcast(this, Tag, TagExists);
	}
}

void UBaseAbilityComponent::OnEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied,
	FActiveGameplayEffectHandle ActiveHandle)
{
	//We only handle UBaseEffect yet cause only need to link animations. May be changed later. 
	if (!Cast<UGameplayEffect>(SpecApplied.Def))
		return;

}

void UBaseAbilityComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Consume the input if this InputID is overloaded with GenericConfirm/Cancel and the GenericConfim/Cancel callback is bound
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	// ---------------------------------------------------------

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					UAbilityBase* GA = Cast<UAbilityBase>(Spec.Ability);
					if (GA && GA->bActivateOnInput)
					{
						// Ability is not active, so try to activate it
						TryActivateAbility(Spec.Handle);
					}
				}
			}
		}
	}
}
