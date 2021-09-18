// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger() 
{
	//UE_LOG(LogTemp, Warning, TEXT("You've been shot!"));
	
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	
	FHitResult HitResult;
	FVector ShotDirection;

	if (GetTrace(HitResult, ShotDirection))
	{
		// 着弾点をデバッグ表示する
		//DrawDebugPoint(GetWorld(), Hit.Location, 20.0f, FColor::Red, true);

		// 着弾点にパーティクルを出す
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactEffect,
				HitResult.Location,
				ShotDirection.Rotation()
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ImpactEffect is nullptr."));
		}

		// 着弾点でサウンドを再生する
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
		}

		// ダメージを与える
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			const FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GetTrace(FHitResult& outHit, FVector& outShotDirection) const
{
	outHit = FHitResult();
	outShotDirection = FVector::ZeroVector;


	// カメラ視線方向を取得する
	AController* OwnerController = GetOwnerController();
	if (!OwnerController)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);


	// 衝突判定
	FHitResult Hit;
	FVector Start = Location;
	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECollisionChannel::ECC_GameTraceChannel1, // Bullet
		Params,
		FCollisionResponseParams::DefaultResponseParam
	);


	outHit = Hit;
	outShotDirection = -Rotation.Vector();
	return bSuccess;
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}
