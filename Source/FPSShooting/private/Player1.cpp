// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "PlayerAnim.h"
#include "Bullet.h"
#include "UI_SniperZoom.h"
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>

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

	//camera 컴포넌트
	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("fpsCamComp"));
	camComp->SetupAttachment(GetMesh(), TEXT("head"));
	camComp->bUsePawnControlRotation = true;		//카메라 폰 제어

	bUseControllerRotationYaw = true;					//클래스디폴트 Yaw 설정

	//라이플mesh 컴포넌트
	rifleMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMeshComp"));
	rifleMeshComp->SetupAttachment(GetMesh(), TEXT("RifleSocket_r"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RifleMesh(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (RifleMesh.Succeeded())
	{
		rifleMeshComp->SetSkeletalMesh(RifleMesh.Object);
		rifleMeshComp->SetRelativeLocationAndRotation(FVector(-3, 3, 2), FRotator(80, -90, 101));
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
	}
	//arrow 컴포넌트
	bulletArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletArrow"));
	bulletArrow->SetupAttachment(rifleMeshComp, TEXT("Muzzle"));

	//체력 초기화
	playerMaxHealth = 100;
	playerHealth = playerMaxHealth;
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();
	
	//초기 속도 걷기속도
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//시작 시 소총으로 시작
	rifleMeshComp->SetVisibility(true);
	sniperMeshComp->SetVisibility(false);
	//일반 크로스헤어, 스코프 위젯 생성
	if (IsValid(zoomWidget))
	{
		_zoomWidget = Cast<UUI_SniperZoom>(CreateWidget(GetWorld(), zoomWidget));
		_zoomWidget->SetOwnerPlayer(this);
	}
	if (IsValid(crosshairWidget))
		_crosshairWidget = CreateWidget(GetWorld(), crosshairWidget);

	if (_crosshairWidget != nullptr)
		_crosshairWidget->AddToViewport();
		
}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();					//캐릭터 이동 함수
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
	//달리기 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayer1::InputJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayer1::OutputJump);
	//총 발사 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayer1::Fire);
	//저격총 스코프 사용/해제 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("ZoomInOut"), IE_Pressed, this, &APlayer1::ZoomInOut);
	//총 변경 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Swap1"), IE_Pressed, this, &APlayer1::Swap1);
	PlayerInputComponent->BindAction(TEXT("Swap2"), IE_Pressed, this, &APlayer1::Swap2);
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

void APlayer1::InputJump()
{
	bPressedJump = true;
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayJumpMontage();
}

void APlayer1::OutputJump()
{
	bPressedJump = false;
}

void APlayer1::Fire()
{
	FVector startPos = camComp->GetComponentLocation();
	FVector endPos = startPos + camComp->GetForwardVector() * 5000;

	FHitResult hitInfo;					//충돌 정보
	FCollisionQueryParams params;		//충돌옵션 설정변수
	params.AddIgnoredActor(this);		//본인 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	
	//라인트레이스가 히트되었을 경우
	if (bHit)
	{
		if (bUsingRifle)
		{
			//총구 위치
			FVector muzzle = bulletArrow->GetComponentLocation();
			//타겟으로의 방향
			FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(muzzle, hitInfo.ImpactPoint);
			//타겟 위치
			FTransform target = UKismetMathLibrary::MakeTransform(muzzle, targetRot, FVector(1, 1, 1));
			GetWorld()->SpawnActor<ABullet>(bulletFactory, target);
		}
		else if (bUsingSniper)
		{

		}
	}
	else
	{
		if (bUsingRifle)
		{
			//총구 위치
			FVector muzzle = bulletArrow->GetComponentLocation();
			//타겟으로의 방향	(타겟이 없으므로 트레이스 끝 부분 지정)
			FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(muzzle, hitInfo.TraceEnd);
			//타겟 위치
			FTransform target = UKismetMathLibrary::MakeTransform(muzzle, targetRot, FVector(1, 1, 1));
			GetWorld()->SpawnActor<ABullet>(bulletFactory, target);
		}
		else if (bUsingSniper)
		{

		}
	}
}

void APlayer1::ZoomInOut()
{
	if (bUsingSniper)
	{
		if (!isZooming)
		{
			isZooming = true;
			//스코프 UI 화면 출력
			_zoomWidget->AddToViewport();
			camComp->SetFieldOfView(45);
			_crosshairWidget->RemoveFromParent();
		}

		else
		{
			isZooming = false;
			//크로스헤어 UI 화면 출력
			_zoomWidget->RemoveFromParent();
			camComp->SetFieldOfView(90);
			_crosshairWidget->AddToViewport();
		}
	}
}

void APlayer1::Swap1()
{
	rifleMeshComp->SetVisibility(true);
	sniperMeshComp->SetVisibility(false);
	bUsingRifle = true;
	bUsingSniper = false;
}

void APlayer1::Swap2()
{
	rifleMeshComp->SetVisibility(false);
	sniperMeshComp->SetVisibility(true);
	bUsingSniper = true;
	bUsingRifle = false;
}
