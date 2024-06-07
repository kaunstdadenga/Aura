// Copyright kaunstdadenga


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues() {
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies() {
	GetAuraASC()->AbilitiesStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel) {
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag)) {
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoint = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoint, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.
					Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
			}

			if (AbilityInfo) {
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](const int32 SpellPoints) {
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		bool bEnableSpendPoint = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoint, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag) {
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid) {
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else {
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoint, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed() {
	if (GetAuraASC()) {
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton,
                                                     bool& bShouldEnableEquipButton) {
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)) {
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible)) {
		if (SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked)) {
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	}
}
