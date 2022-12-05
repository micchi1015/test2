// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Bullet.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//UCameraComponentを生成
	OurCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	//カプセルコンポーネントをシーンコンポーネントにキャスト
	USceneComponent* OurSceneComponent = CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent());

	//シーンコンポーネントをカプセルコンポーネントに親子付け
	OurCameraComponent->SetupAttachment(OurSceneComponent);

	//位置を調整
	OurCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	OurCameraComponent->bUsePawnControlRotation = true;

	//USkeletalMeshComponentを生成
	OurSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OurSkeletalMesh"));

	//OurCameraComponentに親子付け
	OurSkeletalMeshComponent->SetupAttachment(OurCameraComponent);

	//親(PlayerController)からのみ視認できるように設定
	OurSkeletalMeshComponent->SetOnlyOwnerSee(true);

	//影表示フラグをオフ
	OurSkeletalMeshComponent->bCastDynamicShadow = false;
	OurSkeletalMeshComponent->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);

	//インポートしたSoundメッシュを検索
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
	
	////デバッグ文字の表示
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

	//MoveXの入力値をMoveForward関数へバインドする
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);

	//MoveYの入力値をMoveForward関数へバインドする
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);

	InputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::MoveForward(float AxisValue)
{
	//Controller->GetControlRotation()でcharacterのRotatorで取得
	FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);

	//FRotatorを元に回転行列を作成し、行列の軸(X,Y,Z)を取得
	AddMovementInput(vector, AxisValue);
}

void AMyCharacter::MoveRight(float AxisValue)
{
	//Controller->GetControlRotation()でcharacterのRotatorで取得
	FVector vector = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

	//FRotatorを元に回転行列を作成し、行列の軸(X,Y,Z)を取得
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
	//視点位置と視点回転を取得
	FVector CameraLocation;
	FRotator CameraRotation;

	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	//変数に値を設定
	BulletVelocity.Set(100.0f, 0.0f, 0.0f);

	//生成位置の設定
	FVector Location = CameraLocation + FTransform(CameraRotation).TransformVector(BulletVelocity);

	//ワールド情報を取得
	UWorld* World = GetWorld();

	FActorSpawnParameters Spawnprm;
	Spawnprm.Owner = this;
	Spawnprm.Instigator = GetInstigator();
	
	ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, Location, CameraRotation, Spawnprm);

	Projectile->FireInDirection(CameraRotation.Vector());

	AudioCmp->Play(0.0f);
}