// Fill out your copyright notice in the Description page of Project Settings.


#include "spartaCppHW08/Public/PlayerControllerBase.h"

#include "EnhancedInputSubsystems.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
