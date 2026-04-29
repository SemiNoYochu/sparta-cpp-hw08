#include "AEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// 1. 부모(BaseCharacter)의 체력 감소 및 Elimination 로직을 먼저 실행
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 2. 적 캐릭터만의 추가 로직 (예: 피격 사운드 재생 또는 AI 반응)
	
	return ActualDamage;
}
