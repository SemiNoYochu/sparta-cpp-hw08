// Fill out your copyright notice in the Description page of Project Settings.


#include "spartaCppHW08/Public/MyBaseCharacter.h"

#include "Components/CapsuleComponent.h"


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
	bIsEliminated = true;
	
	// 1. 충돌 비활성화 (시체가 길을 막지 않도록)
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	// 2. 물리 시뮬레이션 활성화 (Ragdoll 효과 - 리소스 없을 때 가장 효과적)
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    
	// 3. AI나 컨트롤러 입력 중지 로직은 다음 단계에서 상세 구현
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

