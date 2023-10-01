// Copyright kaunstdadenga

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy {
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy {
	RemoveOnEndOverlap,
	DoNotRemove,
};


USTRUCT(BlueprintType)
struct FInstantGameplayEffect {
	GENERATED_BODY()

	FInstantGameplayEffect() {}

	FInstantGameplayEffect(const TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                       const bool bDestroyOnEffectRemoval,
	                       const EEffectApplicationPolicy InstantEffectApplicationPolicy) :
		bDestroyOnEffectRemoval(bDestroyOnEffectRemoval), GameplayEffectClass(GameplayEffectClass),
		InstantEffectApplicationPolicy(InstantEffectApplicationPolicy) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
};

USTRUCT(BlueprintType)
struct FDurationGameplayEffect {
	GENERATED_BODY()

	FDurationGameplayEffect() {}

	FDurationGameplayEffect(const TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                        const bool bDestroyOnEffectRemoval,
	                        const EEffectApplicationPolicy DurationEffectApplicationPolicy) :
		bDestroyOnEffectRemoval(bDestroyOnEffectRemoval), GameplayEffectClass(GameplayEffectClass),
		DurationEffectApplicationPolicy(DurationEffectApplicationPolicy) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
};

USTRUCT(BlueprintType)
struct FInfiniteGameplayEffect {
	GENERATED_BODY()

	FInfiniteGameplayEffect() {}

	FInfiniteGameplayEffect(const TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                        const bool bDestroyOnEffectRemoval,
	                        const EEffectApplicationPolicy InfiniteEffectApplicationPolicy,
	                        const EEffectRemovalPolicy InfiniteEffectRemovalPolicy) :
		bDestroyOnEffectRemoval(bDestroyOnEffectRemoval), GameplayEffectClass(GameplayEffectClass),
		InfiniteEffectApplicationPolicy(InfiniteEffectApplicationPolicy),
		InfiniteEffectRemovalPolicy(InfiniteEffectRemovalPolicy) {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor {
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToTarget(UAbilitySystemComponent* TargetASC,
	                                                TSubclassOf<UGameplayEffect> GameplayEffectClass,
	                                                float EffectLevel);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FInstantGameplayEffect> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FDurationGameplayEffect> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TArray<FInfiniteGameplayEffect> InfiniteGameplayEffects;

private:
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
