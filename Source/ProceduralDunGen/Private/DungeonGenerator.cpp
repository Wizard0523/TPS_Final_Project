// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RoomBase.h"

enum contentType {
	START,
	MONSTER,
	LOOT,
	PUZZLE,
	KEY,
	BOSS,

};

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
	
	initDungeon();
	
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 

}


ARoomBase* ADungeonGenerator::initDungeon()
{
	TMap<FVector, contentType> rooms;

	//spawn starting room
	FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;
	ARoomBase* startingRoom = GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, spawnLocation, spawnRotation, spawnInfo);
	
	const float offset = 1500.f;
	FVector currentLocation = spawnLocation;
	
	int roomBudget = 9; // num of rooms created between start and boss rooms
	int roomCount = 0;

	//spawn 9 rooms (including boss room) randomly
	//get random direction and prevent overlap
	while (roomCount < roomBudget) {

		//pick random direction and get nextLocation
		FVector nextLocation;
		int randDirection = FMath::RandRange(1, 4);

		switch (randDirection) {
			//north
		case 1:
			nextLocation = currentLocation + FVector(0.f, offset, 0.f);
			break;

			//east
		case 2:
			nextLocation = currentLocation + FVector(offset, 0.f, 0.f);
			break;

			//south
		case 3:
			nextLocation = currentLocation + FVector(0.f, -offset, 0.f);
			break;

			//west
		case 4:
			nextLocation = currentLocation + FVector(-offset, 0.f, 0.f);
			break;
		}

		//if nextLocation is occupied, i.e. rooms.contains next location: update current location to next location, break;
		if (rooms.Contains(nextLocation)) {
			currentLocation = nextLocation;
			continue;
		}
		//else next loction is not occupied: add next location to rooms, update current location to next location
		else {
			currentLocation = nextLocation;
			GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, currentLocation, spawnRotation, spawnInfo);
			

			//randomly select content type for each room:
			enum contentType content;
			int randContent = FMath::RandRange(1, 4);

			switch (randContent) {
			case 1:
				content = contentType::PUZZLE;
				break;

			case 2:
				content = contentType::MONSTER;
				break;

			case 3:
				content = contentType::KEY;
				break;

			case 4:
				content = contentType::LOOT;
				break;
			}
			rooms.Add(currentLocation, content);
		}
		roomCount += 1;
	}
	/*
	int randNum = FMath::RandRange(1, 4);
	switch (1) {
	
	//up
	case 1:
		//prevent overlap
		spawnLocation = (currentLocation + FVector(0.f, 1500.f, 0.f));
		
		//if location isn't occupied, then 
		if (!roomLocations.Contains(spawnLocation)) {
			currentLocation = spawnLocation;
			GetWorld()->SpawnActor<AActor>(actorToSpawn, spawnLocation, spawnRotation, spawnInfo);
		}
		//else location is occupied
		else {

		}

		
		break;

	//right
	case 2:

		break;


	case 3:

		break;

	case 4:

		break;

	}
	*/
	

	//add loop:
	//loop becomes infinite if the currentLocation becomes surrounded
	//maybe each time the room takes a step no matter if the space is occupied.
	//Yes.
	/*
	while (numOfRooms < 1000) {
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
				numOfRooms += 1;
				//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Magenta, TEXT("up %i"), );
			}
			break;

		case 2:
			//right (+offset, 0)
			if (!roomLocations.Contains(currentLocation + (offset, 0.f))) {
				currentLocation = currentLocation + (offset, 0.f);
				roomLocations.Add(currentLocation);
				numOfRooms += 1;
			}
			break;

		case 3:
			//down (0, -offset)
			if (!roomLocations.Contains(currentLocation - (0.f, offset))) {
				currentLocation = currentLocation - (0.f, offset);
				roomLocations.Add(currentLocation);
				numOfRooms += 1;
			}
			break;

		case 4:
			//left (-offset, 0)
			if (!roomLocations.Contains(currentLocation - (offset, 0.f))) {
				currentLocation = currentLocation - (offset, 0.f);
				roomLocations.Add(currentLocation);
				numOfRooms += 1;
			}
			break;
		}
	}
	*/
	return startingRoom;
}

//Rules:
//1. for each key room in a section, there is exactly one treasure room that section
//2. there can't be 3 monster/boss rooms in a row
//3. 
void ADungeonGenerator::rewrite()
{
	//traverse graph using depth first search: https://www.youtube.com/watch?v=ymlzHmRN4To
	
	//stack used to return to previous rooms. Pop elements from the stack when they are fully explored, i.e. are connected to no more unexplored nodes
	TArray<ARoomBase> rooms;

	//how do I keep track of explored rooms?




	//check for rules along the way:
	//i.e. keep count of the num of keys and treasures


}

