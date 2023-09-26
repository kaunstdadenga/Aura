// Copyright kaunstdadenga


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams) {
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitiySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}
