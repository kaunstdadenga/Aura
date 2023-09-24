// Copyright kaunstdadenga


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController() {
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay() {
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	CursorTrace();
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue) {
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace() {
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) {
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *    - Do nothing
	 *  B. LastActor is null && ThisActor is valid
	 *    - Highlight ThisActor
	 *  C. LastActor is valid && ThisActor is null
	 *    - UnHighlight LastActor
	 *  D. Both actors are valid, but LastActor != ThisActor
	 *    - UnHighlight LastActor and Highlight ThisActor
	 *  E. Both actors are valid, but LastActor == ThisActor
	 *    - Do nothing
	 */
	if (LastActor == nullptr) {
		if (ThisActor != nullptr) {
			// Case B
			ThisActor->HighlightActor();
		}
		else {
			// Case A - Do nothing
		}
	}
	else {
		if (ThisActor == nullptr) {
			// Case C
			LastActor->UnHighlightActor();
		}
		else {
			if (LastActor != ThisActor) {
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else {
				// Case E - do nothing
			}
		}
	}
}
