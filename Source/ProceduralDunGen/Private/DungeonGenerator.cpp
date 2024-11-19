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
	
	//spawn starting room
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

void ADungeonGenerator::preventOverlap()
{

}

void ADungeonGenerator::initDungeon()
{
	//TSet documentation: https://dev.epicgames.com/documentation/en-us/unreal-engine/tset?application_version=4.27 
	TSet<FVector2f> roomLocations;
	const float offset = 1500.f;
	FVector2f currentLocation = FVector2f(0.f, 0.f);
	
	roomLocations.Add(currentLocation);

	//spawn room randomly off of starting room
	int randomNum = FMath::RandRange(1, 4);
	switch (randomNum) {
	case 1:
		//up (0, +offset)
		if (!roomLocations.Contains(currentLocation + (0.f, offset))) {
			//update current location
			currentLocation = currentLocation + (0.f, offset);
			//add current location to roomLocations
			roomLocations.Add(currentLocation);
		}
		break;

	case 2:
		//right (+offset, 0)
		if (!roomLocations.Contains(currentLocation + (offset, 0.f))) {
			currentLocation = currentLocation + (offset, 0.f);
			roomLocations.Add(currentLocation);
		}
		break;

	case 3:
		//down (0, -offset)
		if (!roomLocations.Contains(currentLocation - (0.f, offset))) {
			currentLocation = currentLocation - (0.f, offset);
			roomLocations.Add(currentLocation);
		}

		break;

	case 4:
		//left (-offset, 0)
		if (!roomLocations.Contains(currentLocation - (offset, 0.f))) {
			currentLocation = currentLocation - (offset, 0.f);
			roomLocations.Add(currentLocation);
		}
		break;
	}




}

