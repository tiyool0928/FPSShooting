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
		class UArrowComponent* bulletArrow;							//총알 발사 방향, 위치

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
	void Fire();					//총 발사
	void ChangePerspective();			//카메라 시점 변경
	//저격총 스코프 사용, 해제
	void ZoomInOut();
	//총 스왑 함수
	void Swap1();
	void Swap2();
	

	class UUserWidget* _crosshairWidget;
	class UUI_SniperZoom* _zoomWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> crosshairWidget;			//Crosshair UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> zoomWidget;			//Zoom UI

	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class ABullet> bulletFactory;		//총알액터생성팩토리

	//이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 300;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 600;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float crouchSpeed = 150;

	float playerMaxHealth;			//최대체력
	float playerHealth;				//현재체력

	bool isFPSPerspective = true;			//FPS카메라 시점 상태인가?
	bool isZooming = false;					//저격총 스코프를 사용하고 있는 상태인가?
	bool bUsingRifle = true;				//소총을 든 상태인가?
	bool bUsingSniper = false;				//저격총을 든 상태인가?
};
