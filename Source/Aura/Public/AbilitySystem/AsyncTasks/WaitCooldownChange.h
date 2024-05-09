// Copyright kaunstdadenga

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);


/**
 * 
 */
UCLASS()
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
	                                                  const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount) const;
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied,
	                         FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;
};
