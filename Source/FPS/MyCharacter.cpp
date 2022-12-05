// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Bullet.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UCameraComponent�𐶐�
	OurCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	//�J�v�Z���R���|�[�l���g���V�[���R���|�[�l���g�ɃL���X�g
	USceneComponent* OurSceneComponent = CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent());

	//�V�[���R���|�[�l���g���J�v�Z���R���|�[�l���g�ɐe�q�t��
	OurCameraComponent->SetupAttachment(OurSceneComponent);

	//�ʒu�𒲐�
	OurCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	OurCameraComponent->bUsePawnControlRotation = true;

	//USkeletalMeshComponent�𐶐�
	OurSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OurSkeletalMesh"));

	//OurCameraComponent�ɐe�q�t��
	OurSkeletalMeshComponent->SetupAttachment(OurCameraComponent);

	//�e(PlayerController)����̂ݎ��F�ł���悤�ɐݒ�
	OurSkeletalMeshComponent->SetOnlyOwnerSee(true);

	//�e�\���t���O���I�t
	OurSkeletalMeshComponent->bCastDynamicShadow = false;
	OurSkeletalMeshComponent->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	//�C���|�[�g����Sound���b�V��������
	static ConstructorHelpers::FObjectFinder<USoundCue>Sound(TEXT("'/Game/Sound/SE_Shot_Cue.SE_Shot_Cue'"));

	if (Sound.Succeeded())
	{
		ShotSound = Sound.Object;
	}

	AudioCmp = CreateDefaultSubobject<UAudioComponent>(TEXT("OurSound"));
	AudioCmp->SetupAttachment(RootComponent);
	AudioCmp->SetSound(ShotSound);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	////�f�o�b�O�����̕\��
	//GEngine->AddOnScreenDebugMessage((uint64)-1,	//Key
	//	2.0f,										//TimeToDisplay
	//	FLinearColor::Red.ToFColor(true),			//DisplayColor
	//	TEXT("MyFPSCharactor!"));					//DebugString
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//MoveX�̓��͒l��MoveForward�֐��փo�C���h����
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);

	//MoveY�̓��͒l��MoveForward�֐��փo�C���h����
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);

	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::MoveForward(float AxisValue)
{
	//Controller->GetControlRotation()��character��Rotator�Ŏ擾
	FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	//FRotator�����ɉ�]�s����쐬���A�s��̎�(X,Y,Z)���擾
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::MoveRight(float AxisValue)
{
	//Controller->GetControlRotation()��character��Rotator�Ŏ擾
	FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	//FRotator�����ɉ�]�s����쐬���A�s��̎�(X,Y,Z)���擾
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AMyCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AMyCharacter::Jump()
{
	bPressedJump = true;
}

void AMyCharacter::Fire()
{
	//���_�ʒu�Ǝ��_��]���擾
	FVector CameraLocation;
	FRotator CameraRotation;

	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	//�ϐ��ɒl��ݒ�
	BulletVelocity.Set(100.0f, 0.0f, 0.0f);

	//�����ʒu�̐ݒ�
	FVector Location = CameraLocation + FTransform(CameraRotation).TransformVector(BulletVelocity);

	//���[���h�����擾
	UWorld* World = GetWorld();

	FActorSpawnParameters Spawnprm;
	Spawnprm.Owner = this;
	Spawnprm.Instigator = GetInstigator();
	
	ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, Location, CameraRotation, Spawnprm);

	Projectile->FireInDirection(CameraRotation.Vector());

	AudioCmp->Play(0.0f);
}