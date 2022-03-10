// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER427_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	// Update animation properties evert frame similar as Tick function
	void UpdateAnimationProperties(float DeltaTime);

	// Write and access the member variable
	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	/** The speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
	float speed;

	/** Whether or not the character is in the Air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/** Whether or not the character is moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
};
