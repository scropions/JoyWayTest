// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttr.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#define LOW_HP 20.0f;

void UBaseAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, MaxHealth.GetCurrentValue());
		if (NewValue < 1)
		{
			if (GetOwningActor()->Implements<UDieInterface>())
			{
				IDieInterface::Execute_Die(GetOwningActor());
			}
		}
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		if (NewValue < 0.f)
		{
			NewValue = 0.f;
		}

		if (Health.GetCurrentValue() > NewValue)
			Health.SetCurrentValue(NewValue);
	}
	
}

bool UBaseAttributes::IsLowHp() const
{
	return Health.GetCurrentValue() <= MaxHealth.GetCurrentValue() * LOW_HP;
}

void UBaseAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp<float>(GetHealth(), 0.f, MaxHealth.GetCurrentValue()));
	}
}
