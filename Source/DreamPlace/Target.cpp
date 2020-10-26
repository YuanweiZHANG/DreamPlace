// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(RootComponent);

	// Load the target's skeletal mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TargetAsset(TEXT("/Game/MS_PlasticCon/3D_Asset/01_Dirty_Plastic_Oil_Container_tk4lbfkiw/tk4lbfkiw_LOD0"));
	if (TargetAsset.Succeeded())
	{
		TargetMesh->SetStaticMesh(TargetAsset.Object);
		TargetMesh->SetRelativeLocation(FVector(-60.0f, 350.0f, 250.0f));
	}
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATarget::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

