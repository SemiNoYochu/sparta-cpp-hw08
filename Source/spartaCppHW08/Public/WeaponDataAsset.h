// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SPARTACPPHW08_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UWeaponDataAsset();
	
	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<USkeletalMesh> WeaponMesh;
 
	UPROPERTY(EditAnywhere, Category = "Stats")
	float DamageAmount;
 
	UPROPERTY(EditAnywhere, Category = "Stats")
	float TraceDistance;
 
	UPROPERTY(EditAnywhere, Category = "Visual")
	TObjectPtr<UParticleSystem> MuzzleFlash; // 총구 화염 등 (선택 사항)
};
