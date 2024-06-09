// Copyright kaunstdadenga


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level) {
	return FString::Printf(
		TEXT("<Default>%s, </><Level>%d</>"),
		L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum",
		Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level) {
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level) {
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: </><Level>%d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(const float InLevel) const {
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect()) {
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers) {
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute()) {
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(const float InLevel) const {
	float CoolDown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect()) {
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CoolDown);
	}
	return CoolDown;
}
