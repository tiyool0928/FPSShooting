// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player1.h"
#include "UI_Player.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTING_API UUI_Player : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<APlayer1> OwnerPlayer;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* healthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* currentHealth;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* maxHealth;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* currentAmmo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* maxAmmo;

public:
	FNumberFormattingOptions Opts;

	void UpdeateHealthBar();	//체력 업데이트
	void UpdateAmmoBySwap();	//총기교체로 인한 총알 업데이트
	void UpdateAmmoByFire();	//사격으로 인한 총알 업데이트
	void SetOwnerPlayer(APlayer1* InPlayer) { OwnerPlayer = InPlayer; }
};
