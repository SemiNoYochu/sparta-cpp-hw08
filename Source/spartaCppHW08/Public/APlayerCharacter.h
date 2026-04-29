#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "APlayerCharacter.generated.h"

class ABaseWeapon;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SPARTACPPHW08_API APlayerCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Fire(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<ABaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<ABaseWeapon> CurrentWeapon;
	
	void SpawnDefaultWeapon();
};
