// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBaseCharacter.generated.h"

UCLASS()
class SPARTACPPHW08_API AMyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyBaseCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// EditDefaultsOnly: 블루프린트 디테일창에서 수정 가능하게 함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentHealth;

	// 캐릭터가 제거될 때, 실행될 로직
	virtual void OnEliminated();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bIsEliminated;
	
public:
	// 엔진의 표준 데미지 시스템 함수 오버라이드
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
};
