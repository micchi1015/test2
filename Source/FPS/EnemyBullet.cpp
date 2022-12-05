// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USphereComponent�𐶐�
	OurSpereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OurSphere"));

	//���a��15�ɐݒ�
	OurSpereComponent->InitSphereRadius(15.0f);

	//���[�g�R���|�[�l���g��USphereComponent��ݒ�
	RootComponent = OurSpereComponent;

	//�v���t�@�C����ݒ�
	OurSpereComponent->BodyInstance.SetCollisionProfileName(TEXT("BulletProfile"));

	OurSpereComponent->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);

	//�e�̐�������
	InitialLifeSpan = 2.0f;

	//UProjectileMovementComponent�𐶐�
	OurMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("OurMovement"));

	//�쐬����USphereComponent��ݒ�
	OurMovementComponent->SetUpdatedComponent(OurSpereComponent);

	//InitialSpeed(�������x),MaxSpeed(�ő呬�x)��ݒ�
	OurMovementComponent->InitialSpeed = BulletSpeed;
	OurMovementComponent->MaxSpeed = MaxSpeed;

	//bRotationFollowsVelocity,bShouldBounce�̃t���O��ON�ɂ���
	OurMovementComponent->bRotationFollowsVelocity = true;
	OurMovementComponent->bShouldBounce = true;

	//Bounciness��ݒ�
	OurMovementComponent->Bounciness = 0.3f;

	//ProjectileGravityScale��ݒ�
	OurMovementComponent->ProjectileGravityScale = 0.0f;

	//UStaticMeshComponent�𐶐�
	OurMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurMesh"));

	//�C���|�[�g����Sphere���b�V��������
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("'/Game/Mesh/Bullet/EnemyBullet.EnemyBullet'"));

	if (SphereVisualAsset.Succeeded())
	{
		OurMeshComponent->SetStaticMesh(SphereVisualAsset.Object);
		OurMeshComponent->SetupAttachment(RootComponent);
	}

	//�C���|�[�g����Material������
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Mesh/Bullet/Enemy_Fbx_Default_Material.Enemy_Fbx_Default_Material'"));

	if (Material.Succeeded())
	{
		//UMaterialInstanceDynamic�𐶐�
		OurMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, OurMeshComponent);
		OurMeshComponent->SetMaterial(0, OurMaterialInstance);
		OurMeshComponent->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	}
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet::FireInDirection(const FVector& ShootDirection)
{
	OurMovementComponent->Velocity = ShootDirection * OurMovementComponent->InitialSpeed;
}

void AEnemyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(OurMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}