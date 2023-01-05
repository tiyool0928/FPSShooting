// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

void UUI_Player::UpdateHealthBar()
{
	healthBar->SetPercent(OwnerPlayer->playerHealth / OwnerPlayer->playerMaxHealth);

	Opts.SetMaximumFractionalDigits(0);
	currentHealth->SetText(FText::AsNumber(OwnerPlayer->playerHealth, &Opts));
	maxHealth->SetText(FText::AsNumber(OwnerPlayer->playerMaxHealth, &Opts));
}

void UUI_Player::UpdateArmorBar()
{
	armorBar->SetPercent(OwnerPlayer->playerArmor / OwnerPlayer->playerMaxArmor);

	Opts.SetMaximumFractionalDigits(0);
	currentArmor->SetText(FText::AsNumber(OwnerPlayer->playerArmor, &Opts));
	maxArmor->SetText(FText::AsNumber(OwnerPlayer->playerMaxArmor, &Opts));
}

void UUI_Player::UpdateAmmoBySwap()
{
	Opts.SetMaximumFractionalDigits(0);
	if (OwnerPlayer->bUsingRifle)	//소총 사용중이면
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->rifleBullet, &Opts));
		maxAmmo->SetText(FText::AsNumber(OwnerPlayer->keepRifleBullet, &Opts));
	}
	else if (OwnerPlayer->bUsingSniper)
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->sniperBullet, &Opts));
		maxAmmo->SetText(FText::AsNumber(OwnerPlayer->keepSniperBullet, &Opts));
	}
	else if (OwnerPlayer->bUsingGrenade)
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->grenadeAmmo, &Opts));
		maxAmmo->SetText(FText::AsNumber(OwnerPlayer->keepGrenadeAmmo, &Opts));
	}
}

void UUI_Player::UpdateAmmoByFire()
{
	Opts.SetMaximumFractionalDigits(0);
	if (OwnerPlayer->bUsingRifle)	//소총 사용중이면
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->rifleBullet, &Opts));
	}
	else if (OwnerPlayer->bUsingSniper)
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->sniperBullet, &Opts));
	}
	else if (OwnerPlayer->bUsingGrenade)
	{
		currentAmmo->SetText(FText::AsNumber(OwnerPlayer->grenadeAmmo, &Opts));
	}
}
