// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "PlayerAnim.h"
#include "Bullet.h"
#include "Grenade.h"
#include "UI_SniperZoom.h"
#include "UI_Player.h"
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Math/UnrealMathUtility.h>
#include <Perception/AISense_Hearing.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>

// Sets default values
APlayer1::APlayer1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 데이터
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Player1/Mesh/Swat_UE.Swat_UE'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//컴포넌트 위치 (0, 0, -90) 회전 (0, -90, 0)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	//카메라 설정
	//SpringArm 컴포넌트
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 20, 120));
	springArmComp->TargetArmLength = 250;
	springArmComp->bUsePawnControlRotation = true;		//암컴포넌트 폰 제어
	springArmComp->bDoCollisionTest = false;			//카메라 시야 가릴 때 자동 카메라 위치조정 비활성화
	springArmComp->bEnableCameraLag = true;
	//TPScamera 컴포넌트
	TPScamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	TPScamComp->SetupAttachment(springArmComp);
	TPScamComp->bUsePawnControlRotation = false;		//카메라 폰 제어
	TPScamComp->SetAutoActivate(false);

	//FPScamera 컴포넌트
	FPScamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("fpsCamComp"));
	FPScamComp->SetupAttachment(GetMesh(), TEXT("head"));
	FPScamComp->bUsePawnControlRotation = true;		//카메라 폰 제어
	FPScamComp->SetAutoActivate(true);				//FPS카메라부터 활성화		

	bUseControllerRotationYaw = true;					//클래스디폴트 Yaw 설정

	//라이플mesh 컴포넌트
	rifleMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMeshComp"));
	rifleMeshComp->SetupAttachment(GetMesh(), TEXT("RifleSocket_r"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RifleMesh(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (RifleMesh.Succeeded())
	{
		rifleMeshComp->SetSkeletalMesh(RifleMesh.Object);
		rifleMeshComp->SetRelativeLocationAndRotation(FVector(-3, 3, 2), FRotator(80, -90, 101));
		rifleMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//스나이프mesh 컴포넌트
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SnipeMeshComp"));
	sniperMeshComp->SetupAttachment(GetMesh(), TEXT("RifleSocket_r"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> SnipeMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Sniper/sniper_rifle.sniper_rifle'"));
	if (SnipeMesh.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(SnipeMesh.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector(-5, -30, -4), FRotator(78, -55, 135));
		sniperMeshComp->SetWorldScale3D(FVector(0.1f, 0.15f, 0.15f));
		sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//수류탄mesh 컴포넌트
	grenadeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMeshComp"));
	grenadeMeshComp->SetupAttachment(GetMesh(), TEXT("RifleSocket_r"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> GrenadeMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67.SM_G67'"));
	if (GrenadeMesh.Succeeded())
	{
		grenadeMeshComp->SetStaticMesh(GrenadeMesh.Object);
		grenadeMeshComp->SetRelativeLocationAndRotation(FVector(-1, -2, 3), FRotator(19, -442, 111));
		grenadeMeshComp->SetWorldScale3D(FVector(0.1f, 0.15f, 0.15f));
		grenadeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//arrow 컴포넌트
	bulletArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletArrow"));
	bulletArrow->SetupAttachment(rifleMeshComp, TEXT("Muzzle"));

	//플레이어 앉기 세팅
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(60);
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();
	playerHealth = playerMaxHealth;
	playerArmor = playerMaxArmor;
	//초기 속도 걷기속도
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = crouchSpeed;
	//시작 시 소총으로 시작
	rifleMeshComp->SetVisibility(true);
	sniperMeshComp->SetVisibility(false);
	grenadeMeshComp->SetVisibility(false);
	//일반 크로스헤어, 스코프 위젯 생성
	if (IsValid(zoomWidget))
	{
		_zoomWidget = Cast<UUI_SniperZoom>(CreateWidget(GetWorld(), zoomWidget));
		_zoomWidget->SetOwnerPlayer(this);
		_zoomWidget->AddToViewport();
		_zoomWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(playerWidget))
	{
		_playerWidget = Cast<UUI_Player>(CreateWidget(GetWorld(), playerWidget));
		_playerWidget->SetOwnerPlayer(this);
		_playerWidget->AddToViewport();
		_playerWidget->UpdateHealthBar();
		_playerWidget->UpdateAmmoBySwap();
	}
}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();					//캐릭터 이동 함수
	//달리기 시 오디오 겹침 방지
	if (GetCharacterMovement()->MaxWalkSpeed == runSpeed && !stepSoundOverlapControl)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, baseStepSound, GetActorLocation());
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 0.0f, TEXT("Noise"));
		stepSoundOverlapControl = true;
		GetWorld()->GetTimerManager().SetTimer(StepSoundTimerHandle, this, &APlayer1::StepSoundControl, 0.4f, false);
	}

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	//소총 사격 연사 속도 제어
	if (isLeftMouseReleased && bUsingRifle && !rifleFireSpeedControl && !anim->isDeath)
	{
		//장전중이면 장전취소
		if (isReloading) isReloading = false;
		RifleFire();
		rifleFireSpeedControl = true;
		GetWorld()->GetTimerManager().SetTimer(RifleFireSpeedTimerHandle, this, &APlayer1::RifleFireSpeedControl, 0.1f, false);
	}
	if (anim->isDeath)	//죽었을 경우 카메라를 (0, 0, 300)만큼 멀리한다
	{
		TPScamComp->SetRelativeLocation(FMath::VInterpTo(TPScamComp->GetRelativeLocation(),
			FVector(0, 0, 500), GetWorld()->GetDeltaSeconds(), 3.0f));
		TPScamComp->SetRelativeRotation(FMath::RInterpTo(TPScamComp->GetRelativeRotation(),
			FRotator(-70, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
	}
	else
	{
		TPScamComp->SetRelativeLocation(FMath::VInterpTo(TPScamComp->GetRelativeLocation(),
			FVector(0, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
		TPScamComp->SetRelativeRotation(FMath::RInterpTo(TPScamComp->GetRelativeRotation(),
			FRotator(0, 0, 0), GetWorld()->GetDeltaSeconds(), 3.0f));
	}
}

// Called to bind functionality to input
void APlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//좌우 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayer1::Turn);
	//상하 축 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayer1::LookUp);
	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayer1::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayer1::InputVertical);
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &APlayer1::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &APlayer1::OutputRun);
	//점프 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer1::InputJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayer1::OutputJump);
	//앉기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &APlayer1::InputCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &APlayer1::OutputCrouch);
	//총 발사 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayer1::InputLeftMouse);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayer1::OutputLeftMouse);
	//장전 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &APlayer1::Reload);
	//카메라 시점 변경 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("ChangePerspective"), IE_Pressed, this, &APlayer1::ChangePerspective);
	//저격총 스코프 사용/해제 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("ZoomInOut"), IE_Pressed, this, &APlayer1::ZoomInOut);
	//총 변경 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Swap1"), IE_Pressed, this, &APlayer1::Swap1);
	PlayerInputComponent->BindAction(TEXT("Swap2"), IE_Pressed, this, &APlayer1::Swap2);
	PlayerInputComponent->BindAction(TEXT("Swap3"), IE_Pressed, this, &APlayer1::Swap3);
}

void APlayer1::Turn(float value)
{
	AddControllerYawInput(value);
}

void APlayer1::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void APlayer1::InputHorizontal(float value)
{
	direction.Y = value;
}

void APlayer1::InputVertical(float value)
{
	direction.X = value;
}

void APlayer1::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector; 
}

void APlayer1::InputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void APlayer1::OutputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void APlayer1::StepSoundControl()
{
	GetWorld()->GetTimerManager().ClearTimer(StepSoundTimerHandle);
	stepSoundOverlapControl = false;
}

void APlayer1::InputJump()
{
	bPressedJump = true;
}

void APlayer1::OutputJump()
{
	bPressedJump = false;
}

void APlayer1::InputCrouch()
{
	Crouch();
}

void APlayer1::OutputCrouch()
{
	UnCrouch();
}

void APlayer1::InputLeftMouse()
{
	//장전중이면
	if (isReloading) return;

	isLeftMouseReleased = true;

	if (bUsingSniper)
		SniperFire();
	else
		InputThrowGrenade();
}

void APlayer1::RifleFire()
{
	if (rifleBullet <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("rifleBullet Empty"));
		return;
	}

	FVector startPos;
	FVector endPos;
	if (FPScamComp->IsActive())	//FPS시점일 경우
	{
		startPos = FPScamComp->GetComponentLocation();
		endPos = startPos + FPScamComp->GetForwardVector() * 5000;
	}
	else						//TPS시점일 경우
	{
		startPos = TPScamComp->GetComponentLocation();
		endPos = startPos + TPScamComp->GetForwardVector() * 5000;
	}
	 

	FHitResult hitInfo;					//충돌 정보
	FCollisionQueryParams params;		//충돌옵션 설정변수
	params.AddIgnoredActor(this);		//본인 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	//총구 위치
	FVector muzzle = bulletArrow->GetComponentLocation();
	//타겟으로의 방향
	FRotator targetRot;
	//타겟 위치
	FTransform target;

	//라인트레이스가 히트되었을 경우
	if (bHit)
	{
		targetRot = UKismetMathLibrary::FindLookAtRotation(muzzle, hitInfo.ImpactPoint);
		target = UKismetMathLibrary::MakeTransform(muzzle, targetRot, FVector(1, 1, 1));
		//DrawDebugLine(GetWorld(), muzzle, hitInfo.ImpactPoint, FColor::Red, false, 3, (uint8)0U, 1.5f);
	}
	else
	{
		//타겟이 없으므로 트레이스 끝부분 지정
		targetRot = UKismetMathLibrary::FindLookAtRotation(muzzle, hitInfo.TraceEnd);
		target = UKismetMathLibrary::MakeTransform(muzzle, targetRot, FVector(1, 1, 1));
		//DrawDebugLine(GetWorld(), muzzle, hitInfo.TraceEnd, FColor::Green, false, 3, (uint8)0U, 1.5f);
	}
	GetWorld()->SpawnActor<ABullet>(bulletFactory, target);
	rifleBullet--;
	//반동 구현
	AddControllerPitchInput(FMath::FRandRange(-0.5f, -0.3f));
	AddControllerYawInput(FMath::FRandRange(-0.5f, 0.5f));

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	_playerWidget->UpdateAmmoByFire();
	anim->PlayFireMontage();
	UGameplayStatics::SpawnSoundAtLocation(this, rifleFireSound, muzzle);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 200.0f, TEXT("Noise"));
}

void APlayer1::RifleFireSpeedControl()
{
	GetWorld()->GetTimerManager().ClearTimer(RifleFireSpeedTimerHandle);
	rifleFireSpeedControl = false;
}

void APlayer1::SniperFire()
{
	if (sniperFireSpeedControl)
	{
		UE_LOG(LogTemp, Warning, TEXT("bullet load..."));
		return;
	}
	if (sniperBullet <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("sniperBullet: %f"), sniperBullet);
		return;
	}

	FVector startPos;
	FVector endPos;
	if (FPScamComp->IsActive())	//FPS시점일 경우
	{
		startPos = FPScamComp->GetComponentLocation();
		endPos = startPos + FPScamComp->GetForwardVector() * 5000;
	}
	else						//TPS시점일 경우
	{
		startPos = TPScamComp->GetComponentLocation();
		endPos = startPos + TPScamComp->GetForwardVector() * 5000;
	}
	FHitResult hitInfo;					//충돌 정보
	FCollisionQueryParams params;		//충돌옵션 설정변수
	params.AddIgnoredActor(this);		//본인 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	//라인트레이스가 히트되었을 경우
	if (bHit)
	{
		auto hitComp = hitInfo.GetActor();
		UGameplayStatics::ApplyPointDamage(hitComp, 50.0f, hitComp->GetActorLocation(), hitInfo, nullptr, this, nullptr);
		DrawDebugLine(GetWorld(), FPScamComp->GetComponentLocation(), hitInfo.ImpactPoint, FColor::Red, false, 3, (uint8)0U, 1.5f);
	}
	else
	{
		DrawDebugLine(GetWorld(), FPScamComp->GetComponentLocation(), hitInfo.TraceEnd, FColor::Green, false, 3, (uint8)0U, 1.5f);
	}
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(cameraShake);
	sniperBullet--;
	sniperFireSpeedControl = true;
	GetWorld()->GetTimerManager().SetTimer(SniperFireSpeedTimerHandle, this, &APlayer1::SniperFireSpeedControl, 2.0f, false);

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	_playerWidget->UpdateAmmoByFire();
	anim->PlayFireMontage();
	UGameplayStatics::SpawnSoundAtLocation(this, sniperFireSound, GetActorLocation());
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 500.0f, TEXT("Noise"));
}

void APlayer1::SniperFireSpeedControl()
{
	GetWorld()->GetTimerManager().ClearTimer(SniperFireSpeedTimerHandle);
	sniperFireSpeedControl = false;
}

void APlayer1::Reload()
{
	if (isReloading || bUsingGrenade)
	{
		return;
	}
	isReloading = true;
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayReloadMontage();
}

void APlayer1::ChangePerspective()
{
	//스코프 사용중에는 시점변경불가
	if (isZooming) return;

	if (FPScamComp->IsActive())
	{
		FPScamComp->SetActive(false);
		TPScamComp->SetActive(true);
		isFPSPerspective = false;
	}
	else
	{
		FPScamComp->SetActive(true);
		TPScamComp->SetActive(false);
		isFPSPerspective = true;
	}
}

void APlayer1::InputThrowGrenade()
{
	if (bUsingGrenade)
	{
		if (grenadeAmmo <= 0)	//수류탄없으면 X
			return;
		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		anim->PlayThrowGrenadeMontage();
		//anim->StopAllMontages(0);
	}
}

void APlayer1::OutputLeftMouse()
{
	isLeftMouseReleased = false;
	if (bUsingGrenade)
	{
		if (isStandbyGrenade && !isLeftMouseReleased)
		{
			auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
			anim->Montage_Resume(anim->ThrowGrenadeMontage);
			if (grenadeAmmo > 0)
			{
				grenadeAmmo--;
				if (keepGrenadeAmmo > 0)
				{
					grenadeAmmo += 1;
					keepGrenadeAmmo -= 1;
				}
			}
			_playerWidget->UpdateAmmoBySwap();
			isStandbyGrenade = false;
		}
	}
}

void APlayer1::Die()
{
	DisableInput(GetWorld()->GetFirstPlayerController());
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->isDeath = true;

	if (isFPSPerspective)		//FPS카메라 상태이면 TPS로 변경
	{
		FPScamComp->SetActive(false);
		TPScamComp->SetActive(true);
	}
	//UI 모두 지우기
	if (_zoomWidget != nullptr)
	{
		_zoomWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (_playerWidget != nullptr)
	{
		_playerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (bUsingGrenade)
	{
		anim->PlayDieGrenadeMontage();
		anim->Montage_JumpToSection("Start", anim->DieGrenadeMontage);
	}
		
	else
		anim->PlayDieBaseMontage();
	GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &APlayer1::DestroyTemp, 3.0f, false);
}

void APlayer1::DestroyTemp()
{
	//게임 일시정지
	GetWorld()->GetFirstPlayerController()->SetPause(true);
}

void APlayer1::ZoomInOut()
{
	if (bUsingSniper)
	{
		if (!isZooming)
		{
			if (!isFPSPerspective)		//TPS카메라 상태이면 줌인하는동안 FPS로 변경
			{
				FPScamComp->SetActive(true);
				TPScamComp->SetActive(false);
			}
			isZooming = true;
			//스코프 UI 화면 출력
			if (_zoomWidget != nullptr)
			{
				_zoomWidget->SetVisibility(ESlateVisibility::Visible);
			}
			if (FPScamComp)
			{
				FPScamComp->FieldOfView = 45.0f;
			}
			if (_playerWidget != nullptr)
			{
				_playerWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			if (FPScamComp)
			{
				FPScamComp->FieldOfView = 90.0f;
			}
			if (!isFPSPerspective)		//TPS카메라 상태이면 다시 TPS카메라 시점으로 변경
			{
				FPScamComp->SetActive(false);
				TPScamComp->SetActive(true);
			}
			isZooming = false;
			//크로스헤어 UI 화면 출력
			if (_zoomWidget != nullptr)
			{
				_zoomWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			if (_playerWidget != nullptr)
			{
				_playerWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void APlayer1::Swap1()
{
	//장전중이면 장전취소
	if (isReloading) isReloading = false;

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->StopAllMontages(0);
	bUsingRifle = true;
	bUsingSniper = false;
	bUsingGrenade = false;

	rifleMeshComp->SetVisibility(true);
	sniperMeshComp->SetVisibility(false);
	grenadeMeshComp->SetVisibility(false);
	if (isZooming)
	{
		if (!isFPSPerspective)		//TPS카메라 상태이면 다시 TPS카메라 시점으로 변경
		{
			FPScamComp->SetActive(false);
			TPScamComp->SetActive(true);
		}
		isZooming = false;
		//크로스헤어 UI 화면 출력
		_zoomWidget->RemoveFromParent();
		FPScamComp->FieldOfView = 90.0f;
		_playerWidget->AddToViewport();
	}
	_playerWidget->UpdateAmmoBySwap();
}

void APlayer1::Swap2()
{
	//장전중이면 장전취소
	if (isReloading) isReloading = false;

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->StopAllMontages(0);
	bUsingSniper = true;
	bUsingRifle = false;
	bUsingGrenade = false;

	rifleMeshComp->SetVisibility(false);
	sniperMeshComp->SetVisibility(true);
	grenadeMeshComp->SetVisibility(false);
	_playerWidget->UpdateAmmoBySwap();
}

void APlayer1::Swap3()
{
	//장전중이면 장전취소
	if (isReloading) isReloading = false;

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->StopAllMontages(0);
	bUsingSniper = false;
	bUsingRifle = false;
	bUsingGrenade = true;

	anim->PlayReadyGrenadeMontage();

	rifleMeshComp->SetVisibility(false);
	sniperMeshComp->SetVisibility(false);
	if (grenadeAmmo > 0)
	{
		grenadeMeshComp->SetVisibility(true);
	}
	if (isZooming)
	{
		if (!isFPSPerspective)		//TPS카메라 상태이면 다시 TPS카메라 시점으로 변경
		{
			FPScamComp->SetActive(false);
			TPScamComp->SetActive(true);
		}
		isZooming = false;
		//크로스헤어 UI 화면 출력
		_zoomWidget->RemoveFromParent();
		FPScamComp->FieldOfView = 90.0f;
	}
	_playerWidget->UpdateAmmoBySwap();
}

void APlayer1::AnimNotify_ThrowEnd()
{
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayReadyGrenadeMontage();
	if (grenadeAmmo <= 0)
		grenadeMeshComp->SetVisibility(false);
}

void APlayer1::AnimNotify_ThrowGrenade()
{
	FVector startPos;
	FVector endPos;
	if (FPScamComp->IsActive())	//FPS시점일 경우
	{
		startPos = FPScamComp->GetComponentLocation();
		endPos = startPos + FPScamComp->GetForwardVector() * 5000;
	}
	else						//TPS시점일 경우
	{
		startPos = TPScamComp->GetComponentLocation();
		endPos = startPos + TPScamComp->GetForwardVector() * 5000;
	}

	endPos.Z += 1000;		//트레이스보다 위쪽으로 던짐

	FHitResult hitInfo;					//충돌 정보
	FCollisionQueryParams params;		//충돌옵션 설정변수
	params.AddIgnoredActor(this);		//본인 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	//오른손 위치
	FVector rightHand = GetMesh()->GetSocketLocation(TEXT("RifleSocket_r"));
	FRotator targetRot;
	if (bHit)
		//타겟으로의 방향
		targetRot = UKismetMathLibrary::FindLookAtRotation(rightHand, hitInfo.ImpactPoint);
	else
		//트레이스 끝 방향
		targetRot = UKismetMathLibrary::FindLookAtRotation(rightHand, hitInfo.TraceEnd);
	//타겟 위치
	FTransform target = UKismetMathLibrary::MakeTransform(rightHand, targetRot, FVector(1, 1, 1));
	GetWorld()->SpawnActor<AGrenade>(grenadeFactory, target);
}

void APlayer1::AnimNotify_ThrowDivisionAction()
{
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->Montage_Pause(anim->ThrowGrenadeMontage);
	isStandbyGrenade = true;

	if (isStandbyGrenade && !isLeftMouseReleased)
	{
		anim->Montage_Resume(anim->ThrowGrenadeMontage);
		if (grenadeAmmo > 0)
		{
			grenadeAmmo--;
			if (keepGrenadeAmmo > 0)
			{
				grenadeAmmo += 1;
				keepGrenadeAmmo -= 1;
			}
		}
		_playerWidget->UpdateAmmoBySwap();
		isStandbyGrenade = false;
	}
}

void APlayer1::AnimNotify_ReloadComplete()
{
	if (bUsingRifle)
	{
		if (keepRifleBullet <= 0) return;
		float needBullet = 30 - rifleBullet;
		if (needBullet >= keepRifleBullet)
		{
			rifleBullet += keepRifleBullet;
			keepRifleBullet = 0;
		}
		else
		{
			rifleBullet += needBullet;
			keepRifleBullet -= needBullet;
		}
	}
	else if (bUsingSniper)
	{
		if (keepSniperBullet <= 0) return;
		float needBullet = 5 - sniperBullet;
		if (needBullet >= keepSniperBullet)
		{
			sniperBullet += keepSniperBullet;
			keepSniperBullet = 0;
		}
		else
		{
			sniperBullet += needBullet;
			keepSniperBullet -= needBullet;
		}
	}
	isReloading = false;
	_playerWidget->UpdateAmmoBySwap();
}

void APlayer1::AnimNotify_PlayReloadSound()
{
	UGameplayStatics::SpawnSoundAtLocation(this, rifleReloadSound, GetActorLocation());
}

void APlayer1::AnimNotify_PlayPinPullSound()
{
	UGameplayStatics::SpawnSoundAtLocation(this, pinPullSound, GetActorLocation());
}

float APlayer1::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		for (auto bName : criticalBone)
		{
			if (PointDamageEvent->HitInfo.BoneName == bName)
			{
				if (bName == "head")
					ActualDamage *= 3; // 맞은 부위가 머리면, 데미지 3배
				else
					ActualDamage *= 2; // 맞은 부위가 치명부위이면, 데미지 2배
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("BoneName :: %s"), *PointDamageEvent->HitInfo.BoneName.ToString());
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
	}

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (ActualDamage > 0.0f)
	{
		float tempDamage = ActualDamage;
		if (playerArmor > tempDamage)			//아머가 대미지보다 많을 경우
		{
			playerArmor -= tempDamage;
		}
		else									//아머보다 대미지가 많을 경우
		{
			tempDamage -= playerArmor;
			playerArmor = 0;
			playerHealth -= tempDamage;
		}

		if (!bUsingGrenade)
		{
			anim->PlayHitReactionMontage();
		}
		else if((bUsingGrenade && !isLeftMouseReleased))
		{
			anim->PlayGrenadeHitReactionMontage();
			UE_LOG(LogTemp, Warning, TEXT("Montage Check"));
		}

		if (IsValid(playerWidget))
		{
			_playerWidget->UpdateArmorBar();
			_playerWidget->UpdateHealthBar();
		}
	}

	if (playerHealth <= 0)
	{
		SetActorEnableCollision(false);
		Die();
	}

	return ActualDamage;
}
