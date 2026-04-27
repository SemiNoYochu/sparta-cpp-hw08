#include "APlayerCharacter.h"

#include "BaseWeapon.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// 캐릭터 매쉬가 카메라 회전에 따라 돌아가지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// 대신, 이동하려는 방향으로 캐릭터의 몸을 부드럽게 회전시킴
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // 회전 속도 조정
	
	// 카메라 붐(셀카봉) 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;			// 캐릭터와의 거리
	CameraBoom->bUsePawnControlRotation = true;		// 컨트롤러 회전값 사용
	
	// 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;	// 컨트롤러 회전값 미사용
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultWeapon();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APlayerControllerBase* PC =
			Cast<APlayerControllerBase>(GetController()))
		{
			if (PC->MoveAction)
			{
				EnhancedInputComponent->BindAction(
					PC->MoveAction, 
					ETriggerEvent::Triggered, 
					this, 
					&APlayerCharacter::Move);
			}
			
			if (PC->LookAction)
			{
				EnhancedInputComponent->BindAction(
					PC->LookAction, 
					ETriggerEvent::Triggered, 
					this, 
					&APlayerCharacter::Look);
			}
		}
		
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// 입력값(W: Y=1, S: Y=-1, D: X=1, A: X=-1) 가져오기
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// 1. 카메라가 바라보는 방향(Rotation)을 가져옴
		const FRotator Rotation = Controller->GetControlRotation();
		
		// 2. 바닥면과 평행한 방향만 계산하기 위해 Yaw(좌우회전)값만 남김 (Pitch, Roll 제거)
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// 3. 이 회전값을 기준으로 월드의 '앞'과 '오른쪽' 벡터를 계산
		// FRotationMatrix는 해당 회전이 적용된 좌표계를 만듦
		const FVector ForwardDirection = 
			FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// 4. 계산된 방향벡터에 플레이어의 입력 크기(1 or -1)를 곱해 이동시킴
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::SpawnDefaultWeapon()
{
	// WeaponClass가 에디터에서 할당되었는지 확인
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;	// 무기의 Owner를 이 캐릭터로 설정
		
		// 무기 스폰
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParams);
		if (CurrentWeapon)
		{
			// 캐릭터 손 소켓에 부착
			CurrentWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("weapon_attach_r"));	// 에디터에서 만든 소켓 이름과 일치해야 함
		}
	}
}
