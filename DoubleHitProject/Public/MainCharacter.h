// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


class UCameraComponent;
UCLASS()
class DOUBLEHITPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float value);

	void MoveRight(float value);
	
	FRotator LookAtTarget(FVector location,AActor* targetActor);
	FVector GetCameraTargetPos(FVector target,  UCameraComponent* camera, float offset);

	FVector RotateAxis(FVector centerPos, FVector aroundPos, float moveSpeed, FVector Axis);
	FVector ProjectOnPlane(FVector target, FVector noraml);

public:
	UStaticMeshComponent* staticMeshCom;
	UPROPERTY(Editanywhere)
	class UAnimInstance* anim;

	UPROPERTY(EditDefaultsOnly)
	 UCameraComponent* CameraCom;
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* CameraBoom;


	UPROPERTY(Editanywhere)
		AActor* targetActor;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	bool IsShot;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
	float inputX;
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	float inputY;
	
	UPROPERTY(EditAnywhere)
	float lerpValue;

	float lerpCamera=0;
	

};


