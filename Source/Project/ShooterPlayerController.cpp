// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// HUDì¬
	HUD = CreateWidget(this, HUDClass);
	if (HUD)
	{
		HUD->AddToViewport();
	}
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	UE_LOG(LogTemp, Warning, TEXT("We have finished."));
	

	// HUDœŠO
	if (HUD)
	{
		HUD->RemoveFromViewport();
	}


	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel,  RestartDelay);
}
