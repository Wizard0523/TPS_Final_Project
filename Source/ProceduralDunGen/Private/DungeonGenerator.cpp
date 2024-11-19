// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RoomBase.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;


	GetWorld()->SpawnActor<AActor>(actorToSpawn, spawnLocation, spawnRotation, spawnInfo);
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

