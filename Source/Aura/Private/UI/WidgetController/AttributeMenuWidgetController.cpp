// Copyright kaunstdadenga


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues() {
	Super::BroadcastInitialValues();

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(
		FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();

	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {
	Super::BindCallbacksToDependencies();
}
