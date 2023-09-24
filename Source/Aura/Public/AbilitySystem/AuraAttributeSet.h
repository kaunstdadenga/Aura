// Copyright kaunstdadenga

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet {
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	// MaxHealth
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	// MaxMana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
