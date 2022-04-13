// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"






// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraSpring"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	//anim = this->GetMesh()->GetAnimInstance();
	CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("FllowCamera"));
	CameraCom->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraCom->bUsePawnControlRotation = false;

	
	

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorRotation(LookAtTarget(this->GetActorLocation(), targetActor));
	CameraCom->SetWorldRotation(LookAtTarget(CameraCom->GetComponentLocation(), this));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("move",this.)
	
}

void AMainCharacter::MoveForward(float value)
{

	//int dir = value > 0 ? 1 : -1;
	if (value > 0)
	{
		inputY = FMath::Lerp(inputY, 1.f, lerpValue);
	}
	else if (value < 0)
	{
		inputY = FMath::Lerp(inputY, -1.f, lerpValue);
	}
	else
	{
		if (inputY < 0.3 && inputY > -0.3) inputY = 0;
		else
		{
			inputY = FMath::Lerp(inputY, 0.f, lerpValue);

		}
	}
	AddMovementInput(GetActorForwardVector(), inputY*0.5f);
}


void AMainCharacter::MoveRight(float value)
{
	//int dir = value > 0 ? 1 : -1;
	if (value > 0)
	{
		lerpCamera = FMath::Lerp(lerpCamera, 1.f, 0.01);
		inputX = FMath::Lerp(inputX, 1.f, lerpValue);
	}
	else if (value < 0)
	{
		lerpCamera = FMath::Lerp(lerpCamera, -1.f, 0.01);

		inputX = FMath::Lerp(inputX, -1.f, lerpValue);
	}
	else
	{
		if (inputX < 0.3 && inputX > -0.3) inputX = 0;
		else
		{
			inputX = FMath::Lerp(inputX, 0.f, lerpValue);

		}

		//lerpCamera = FMath::Lerp(lerpCamera, 0.f, 0.05);

	}

	//AddMovementInput(GetActorRightVector(), inputX * 0.5f);
	FVector targetPos = RotateAxis(targetActor->GetActorLocation(), GetActorLocation(), -inputX * 2, FVector(0, 0, 1));
	SetActorLocation(targetPos);

	FVector targetCamPos = RotateAxis(targetPos, targetPos - GetActorForwardVector() * 200 + GetActorUpVector() * 40, lerpCamera * 80, FVector(0, 0, 1));

	CameraCom->SetWorldLocation(targetCamPos);
	
}

FRotator AMainCharacter::LookAtTarget(FVector location, AActor* target)
{
	FVector targetLocation = target->GetActorLocation();
	FVector rotateForward = FVector(targetLocation.X - location.X, targetLocation.Y - location.Y, 0);
	return rotateForward.Rotation();
	//CameraCom->SetActorRotation(rotateForward.Rotation());
	//CameraCom->SetRelativeRotation(FRotator(0, 0, 0));
	//CameraCom->SetWorldRotation((targetLocation-CameraCom->GetComponentLocation()).Rotation()+ FRotator(0, 20 * lerpCamera, 0));
	//DrawDebugLine(GetWorld(),FVector(1000,1000,1000), targetLocation, FColor::Red);
			//UKismetSystemLibrary::DrawDebugLine(this, CameraCom->GetComponentLocation(), targetLocation, FColor::Red);
}

FVector AMainCharacter::GetCameraTargetPos(FVector target,UCameraComponent* camera,float offset)
{
	return FVector(0, 0, 0);

}

FVector AMainCharacter::RotateAxis(FVector centerPos, FVector aroundPos, float moveSpeed, FVector Axis)
{

	FVector radius = aroundPos - centerPos;
	FVector changeRadius = ProjectOnPlane(radius, Axis);
	
	
	float S, C;
	FMath::SinCos(&S, &C, moveSpeed / changeRadius.Size());

	const float XX = Axis.X * Axis.X;
	const float YY = Axis.Y * Axis.Y;
	const float ZZ = Axis.Z * Axis.Z;

	const float XY = Axis.X * Axis.Y;
	const float YZ = Axis.Y * Axis.Z;
	const float ZX = Axis.Z * Axis.X;

	const float XS = Axis.X * S;
	const float YS = Axis.Y * S;
	const float ZS = Axis.Z * S;

	const float OMC = 1.f - C;

	return FVector(
		(OMC * XX + C) * radius.X + (OMC * XY - ZS) * radius.Y + (OMC * ZX + YS) * radius.Z+ centerPos.X,
		(OMC * XY + ZS) * radius.X + (OMC * YY + C) * radius.Y + (OMC * YZ - XS) * radius.Z+centerPos.Y,
		(OMC * ZX - YS) * radius.X + (OMC * YZ + XS) * radius.Y + (OMC * ZZ + C) * radius.Z+centerPos.Z
	);
}

FVector AMainCharacter::ProjectOnPlane(FVector target, FVector noraml)
{
	return target - noraml * FVector::DotProduct(target, noraml) / FVector::DotProduct(noraml, noraml);
}
