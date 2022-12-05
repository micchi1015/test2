// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USphereComponentを生成
	OurSpereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OurSphere"));

	//半径を15に設定
	OurSpereComponent->InitSphereRadius(15.0f);

	//ルートコンポーネントにUSphereComponentを設定
	RootComponent = OurSpereComponent;

	//プロファイルを設定
	OurSpereComponent->BodyInstance.SetCollisionProfileName(TEXT("BulletProfile"));

	OurSpereComponent->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);

	//弾の生存時間
	InitialLifeSpan = 2.0f;

	//UProjectileMovementComponentを生成
	OurMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("OurMovement"));

	//作成したUSphereComponentを設定
	OurMovementComponent->SetUpdatedComponent(OurSpereComponent);

	//InitialSpeed(初期速度),MaxSpeed(最大速度)を設定
	OurMovementComponent->InitialSpeed = BulletSpeed;
	OurMovementComponent->MaxSpeed = MaxSpeed;

	//bRotationFollowsVelocity,bShouldBounceのフラグをONにする
	OurMovementComponent->bRotationFollowsVelocity = true;
	OurMovementComponent->bShouldBounce = true;

	//Bouncinessを設定
	OurMovementComponent->Bounciness = 0.3f;

	//ProjectileGravityScaleを設定
	OurMovementComponent->ProjectileGravityScale = 0.0f;

	//UStaticMeshComponentを生成
	OurMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurMesh"));

	//インポートしたSphereメッシュを検索
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("'/Game/Mesh/Bullet/EnemyBullet.EnemyBullet'"));

	if (SphereVisualAsset.Succeeded())
	{
		OurMeshComponent->SetStaticMesh(SphereVisualAsset.Object);
		OurMeshComponent->SetupAttachment(RootComponent);
	}

	//インポートしたMaterialを検索
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Mesh/Bullet/Enemy_Fbx_Default_Material.Enemy_Fbx_Default_Material'"));

	if (Material.Succeeded())
	{
		//UMaterialInstanceDynamicを生成
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