// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player1.generated.h"

UCLASS()
class FPSSHOOTING_API APlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	APlayer1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* TPScamComp;
	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* FPScamComp;
	UPROPERTY(EditDefaultsOnly)
		class USkeletalMeshComponent* rifleMeshComp;				//라이플메시 컴포넌트 등록
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* sniperMeshComp;				//스나이프메시 컴포넌트 등록
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* grenadeMeshComp;				//수류탄메시 컴포넌트 등록
	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* bulletArrow;							//총알 발사 방향, 위치
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* rifleFireSound;							//소총 사격 사운드
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* rifleReloadSound;							//소총 장전 사운드
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* sniperFireSound;							//저격총 사격 사운드
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* pinPullSound;							//수류탄 핀 사운드


	//좌우 입력 처리
	void Turn(float value);
	//상하 입력 처리
	void LookUp(float value);

	//이동 방향
	FVector direction;
	//좌우 이동 입력
	void InputHorizontal(float value);
	//상하 이동 입력
	void InputVertical(float value);

	void Move();					//이동 처리
	void InputRun();				//달리기 Press
	void OutputRun();				//달리기 Release
	void InputJump();				//점프 Press
	void OutputJump();				//점프 Release
	void InputCrouch();				//앉기 Press
	void OutputCrouch();			//앉기 Release
	void InputLeftMouse();			//왼쪽 마우스 클릭
	void RifleFire();				//소총 발사
	void FireSpeedControl();		//소총 연사 속도 제어
	void SniperFire();				//저격총 발사
	void Reload();					//장전
	void ChangePerspective();		//카메라 시점 변경
	void InputThrowGrenade();			//수류탄 투척 준비동작
	void OutputLeftMouse();			//수류탄 투척
	//저격총 스코프 사용, 해제
	void ZoomInOut();
	//총 스왑 함수
	void Swap1();
	void Swap2();
	void Swap3();

	class UUI_Player* _playerWidget;
	class UUI_SniperZoom* _zoomWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> playerWidget;			//Crosshair UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> zoomWidget;			//Zoom UI

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class ABullet> bulletFactory;		//총알액터생성팩토리
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class AGrenade> grenadeFactory;		//수류탄액터생성팩토리
	
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;

	FTimerHandle FireSpeedTimerHandle;					//연사속도제어타이머

	//이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 300;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 600;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float crouchSpeed = 150;

	float playerMaxHealth;			//최대체력
	float playerHealth;				//현재체력
	float rifleBullet = 30;
	float keepRifleBullet = 360;		//소총 총알 여분
	float sniperBullet = 5;
	float keepSniperBullet = 25;			//저격총 총알 여분
	float grenadeAmmo = 1;
	float keepGrenadeAmmo = 1;			//수류탄 여분

	bool isFPSPerspective = true;			//FPS카메라 시점 상태인가?
	bool isZooming = false;					//저격총 스코프를 사용하고 있는 상태인가?
	bool bUsingRifle = true;				//소총을 든 상태인가?
	bool bUsingSniper = false;				//저격총을 든 상태인가?
	bool bUsingGrenade = false;				//수류탄을 든 상태인가?
	bool isLeftMouseReleased = false;		//왼쪽마우스를 누르고 있는 상태인가?
	bool isStandbyGrenade = false;			//수류탄 던질 준비 완료한 상태
	bool rifleFireSpeedControl = false;		//소총 연사 속도 제어 변수

	//노티파이 호출 함수
	UFUNCTION(BluePrintCallable)
		void AnimNotify_ThrowEnd();
	UFUNCTION(BluePrintCallable)
		void AnimNotify_ThrowGrenade();
	UFUNCTION(BluePrintCallable)
		void AnimNotify_ThrowDivisionAction();
	UFUNCTION(BluePrintCallable)
		void AnimNotify_ReloadComplete();
	UFUNCTION(BluePrintCallable)
		void AnimNotify_PlayReloadSound();
	UFUNCTION(BluePrintCallable)
		void AnimNotify_PlayPinPullSound();
};
