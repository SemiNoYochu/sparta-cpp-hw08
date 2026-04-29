#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "AEnemyCharacter.generated.h"

UCLASS()
class SPARTACPPHW08_API AEnemyCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();
	
	// 데미지 처리 오버라이드
	virtual float TakeDamage(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser) override;
};
