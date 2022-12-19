// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player1.h"
#include "UI_SniperZoom.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class FPSSHOOTING_API UUI_SniperZoom : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TWeakObjectPtr<APlayer1> OwnerPlayer;

	UPROPERTY(meta = (BindWidget))
		class UImage* ZoomImage;

public:
	void SetOwnerPlayer(APlayer1* Player) { OwnerPlayer = Player; }
};
