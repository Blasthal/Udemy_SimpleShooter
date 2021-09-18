// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass = nullptr;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.0f;

	FTimerHandle RestartTimer;

	UPROPERTY()
	UUserWidget* HUD = nullptr;
};
