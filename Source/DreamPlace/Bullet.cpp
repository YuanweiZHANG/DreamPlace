// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "DreamPlacePlayerState.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the bullet as a sphere
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	SphereComponent->InitSphereRadius(5.0f);
	SphereComponent->SetupAttachment(RootComponent);

	// Set bullet movement component
	BulletMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovementComponent"));
	BulletMovementComponent->SetUpdatedComponent(SphereComponent);
	BulletMovementComponent->InitialSpeed = 3000.0f;
	BulletMovementComponent->MaxSpeed = 3000.0f;
	BulletMovementComponent->bRotationFollowsVelocity = true;
	BulletMovementComponent->bShouldBounce = true;
	BulletMovementComponent->Bounciness = 0.3f;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Set the initial direction of a bullet
void ABullet::FireInDirection(const FVector& ShootDirection)
{
	BulletMovementComponent->Velocity = ShootDirection * BulletMovementComponent->InitialSpeed;
}

// Every time hit the target, add 1 to the score
void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherActor->GetName().StartsWith("Target"))
	{
		
	}
}