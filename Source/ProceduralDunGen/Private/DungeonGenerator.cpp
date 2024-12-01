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
	initDungeon();
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ARoomBase* ADungeonGenerator::initDungeon()
{
	TMap<FVector, ARoomBase*> rooms;

	//spawn starting room
	FVector currentLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;
	ARoomBase* startingRoom = GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, currentLocation, spawnRotation, spawnInfo);
	rooms.Add(currentLocation, startingRoom);
	
	const float offset = 1500.f;
	int roomBudget = 9; // num of rooms created between start and boss rooms
	int roomCount = 0;

	//spawn 9 rooms (including boss room) randomly
	//get random direction and prevent overlap
	while (roomCount < roomBudget) {

		//pick random direction and get nextLocation
		FVector nextLocation;
		int randDirection = FMath::RandRange(1, 4);
		ARoomBase::direction roomDirection;

		switch (randDirection) {
			//north
		case 1:
			nextLocation = currentLocation + FVector(0.f, offset, 0.f);
			roomDirection = ARoomBase::direction::NORTH;
			break;

			//east
		case 2:
			nextLocation = currentLocation + FVector(offset, 0.f, 0.f);
			roomDirection = ARoomBase::direction::EAST;
			break;

			//south
		case 3:
			nextLocation = currentLocation + FVector(0.f, -offset, 0.f);
			roomDirection = ARoomBase::direction::SOUTH;
			break;

			//west
		case 4:
			nextLocation = currentLocation + FVector(-offset, 0.f, 0.f);
			roomDirection = ARoomBase::direction::WEST;
			break;
		}

		//if nextLocation is occupied, i.e. rooms.contains next location: update current location to next location, break;
		if (rooms.Contains(nextLocation) && rooms.Contains(currentLocation)) {
			
			ARoomBase* nextRoom = *rooms.Find(nextLocation);
			ARoomBase* currentRoom = *rooms.Find(currentLocation);
			if (currentRoom && nextRoom) {
				currentRoom->connectedRooms.Add(roomDirection, nextRoom);
				ARoomBase::direction oppositeDirection = ARoomBase::getOpposite(roomDirection);
				nextRoom->connectedRooms.Add(oppositeDirection, currentRoom);
			}
			currentLocation = nextLocation;
			continue;
		}

		//else next location is not occupied: add next location to rooms, update current location to next location
		else {
			//if connected rooms map, in room at currentLocation, does NOT contain room at next location
			//then add room at next location to connected rooms map of room at current location
			ARoomBase* currentRoom;
			ARoomBase* newRoom;

			if (rooms.Contains(currentLocation)) {
				currentRoom = *rooms.Find(currentLocation);

				currentLocation = nextLocation;
				newRoom = GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, currentLocation, spawnRotation, spawnInfo);
				rooms.Add(currentLocation, newRoom);
				if (currentRoom && newRoom) {
					currentRoom->connectedRooms.Add(roomDirection, newRoom);
					ARoomBase::direction oppositeDirection = ARoomBase::getOpposite(roomDirection);
					newRoom->connectedRooms.Add(oppositeDirection, currentRoom);

					//randomly select content type for each room:
					ARoomBase::contentType roomContent;
					int randContent = FMath::RandRange(1, 4);

					switch (randContent) {
					case 1:
						roomContent = ARoomBase::contentType::PUZZLE;
						break;

					case 2:
						roomContent = ARoomBase::contentType::MONSTER;
						break;

					case 3:
						roomContent = ARoomBase::contentType::KEY;
						break;

					case 4:
						roomContent = ARoomBase::contentType::LOOT;
						break;
					}
					newRoom->content = roomContent;

				}
			}
		}
		roomCount += 1;
	}



	//for each room in rooms

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

