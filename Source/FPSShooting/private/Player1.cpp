// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

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
	springArmComp->SetRelativeLocation(FVector(0, 0, 90));
	springArmComp->TargetArmLength = 500;
	springArmComp->bUsePawnControlRotation = true;		//암컴포넌트 폰 제어
	springArmComp->bDoCollisionTest = false;			//카메라 시야 가릴 때 자동 카메라 위치조정 비활성화
	//camera 컴포넌트
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;		//카메라 폰 제어

	bUseControllerRotationYaw = true;					//클래스디폴트 Yaw 설정

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
}

void APlayer1::OutputJump()
{
	bPressedJump = false;
}
