// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.generated.h"

UCLASS()
class DREAMPLACE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// Bullet collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent *SphereComponent;

	// Bullet movement component
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* BulletMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Bullet initial speed and direction
	void FireInDirection(const FVector& ShootDirection);

	// Whether hit the target
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
