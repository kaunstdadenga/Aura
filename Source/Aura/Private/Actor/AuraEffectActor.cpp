// Copyright kaunstdadenga


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor() {
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay() {
	Super::BeginPlay();
}

FActiveGameplayEffectHandle AAuraEffectActor::ApplyEffectToTarget(UAbilitySystemComponent* TargetASC,
                                                                  TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                                                  float EffectLevel) {
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	check(GameplayEffectClass);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, EffectLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	return ActiveEffectHandle;
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor) {
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) {
		return;
	}

	for (const auto InstantGameplayEffect : InstantGameplayEffects) {
		if (InstantGameplayEffect.InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
			ApplyEffectToTarget(TargetASC, InstantGameplayEffect.GameplayEffectClass,
			                    InstantGameplayEffect.EffectLevel);
		}
	}
	for (const auto DurationGameplayEffect : DurationGameplayEffects) {
		if (DurationGameplayEffect.DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
			ApplyEffectToTarget(TargetASC, DurationGameplayEffect.GameplayEffectClass,
			                    DurationGameplayEffect.EffectLevel);
		}
	}
	for (const auto InfiniteGameplayEffect : InfiniteGameplayEffects) {
		if (InfiniteGameplayEffect.InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
			FActiveGameplayEffectHandle ActiveEffectHandle = ApplyEffectToTarget(
				TargetASC, InfiniteGameplayEffect.GameplayEffectClass, InfiniteGameplayEffect.EffectLevel);

			if (InfiniteGameplayEffect.InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
				ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
			}
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor) {
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) {
		return;
	}

	for (const auto InstantGameplayEffect : InstantGameplayEffects) {
		if (InstantGameplayEffect.InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			ApplyEffectToTarget(TargetASC, InstantGameplayEffect.GameplayEffectClass,
			                    InstantGameplayEffect.EffectLevel);
		}
	}
	for (const auto DurationGameplayEffect : DurationGameplayEffects) {
		if (DurationGameplayEffect.DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			ApplyEffectToTarget(TargetASC, DurationGameplayEffect.GameplayEffectClass,
			                    DurationGameplayEffect.EffectLevel);
		}
	}
	for (const auto InfiniteGameplayEffect : InfiniteGameplayEffects) {
		if (InfiniteGameplayEffect.InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
			ApplyEffectToTarget(TargetASC, InfiniteGameplayEffect.GameplayEffectClass,
			                    InfiniteGameplayEffect.EffectLevel);
		}
		else if (InfiniteGameplayEffect.InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (auto HandlePair : ActiveEffectHandles) {
				if (TargetASC == HandlePair.Value) {
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}
			for (auto& Handle : HandlesToRemove) {
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}
