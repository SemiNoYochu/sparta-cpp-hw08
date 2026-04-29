// Fill out your copyright notice in the Description page of Project Settings.


#include "spartaCppHW08/Public/MyBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMyBaseCharacter::AMyBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MaxHealth = 100.0f;
	bIsEliminated = false;
}

// Called when the game starts or when spawned
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

// Called every frame
void AMyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyBaseCharacter::OnEliminated()
{
	// 중복 트리거 방지
	if (bIsEliminated) return;
	
	bIsEliminated = true;
	
	// 1. 캐릭터 움직임 컴포넌트 중지 (중력, NavMesh, 가속도 계산 차단)
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
	}
 
	// 2. 캡슐 충돌 제거 (캐릭터끼리 밀치는 현상 방지)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 
	// 3. 메쉬에 래그돌 물리 적용
	// "Ragdoll" 프로필은 Physics와 Query 반응이 최적으로 세팅되어 있습니다.
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
 
	// 4. 시체 자동 제거 (성능 최적화: 5초 후 월드에서 삭제)
	SetLifeSpan(5.0f);
 
	UE_LOG(LogTemp, Warning, TEXT("%s has been eliminated."), *GetName());
}

float AMyBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsEliminated) return 0.0f; // 이미 제거된 상태라면 무시
	
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageToApply, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
	
	if (0.0f >= CurrentHealth)
	{
		OnEliminated();
	}
	
	return DamageToApply;
}

