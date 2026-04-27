// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "WeaponDataAsset.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 에셋이 설정되어 있다면 메쉬를 자동으로 할당
	if (WeaponData && WeaponData->WeaponMesh)
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->WeaponMesh);
	}
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Fire()
{
	if (!WeaponData) return;
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	
	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController) return;
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	
	FVector End = Location + (Rotation.Vector() * WeaponData->TraceDistance);
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerPawn);
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, 
		Location,
		End,
		ECC_Visibility,
		Params)
		)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UGameplayStatics::ApplyDamage(
				HitActor, 
				WeaponData->DamageAmount, 
				OwnerController, 
				this, 
				nullptr);
			
			DrawDebugSphere(
				GetWorld(), 
				Hit.ImpactPoint, 
				10.0f, 
				12, 
				FColor::Red, 
				false, 
				2.0f);
		}
	}
	
}

