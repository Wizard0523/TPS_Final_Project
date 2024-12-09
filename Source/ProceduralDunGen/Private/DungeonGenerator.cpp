// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RoomBase.h"
#include "MonsterMarker.h"
#include "Logging/StructuredLog.h"
#include "ChestActor.h"


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
	int roomNumber = 1;

	//spawn starting room
	FVector currentLocation = FVector(0.0f, 0.0f, 0.0f);
	FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawnInfo;
	ARoomBase* startingRoom = GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, currentLocation, spawnRotation, spawnInfo);
	if (startingRoom) {
		startingRoom->roomID = roomNumber;
		UE_LOGFMT(LogTemp, Display, "Room {number}", startingRoom->roomID);
	}
	roomNumber += 1;
	rooms.Add(currentLocation, startingRoom);

	const float offset = 1500.f;
	int roomBudget = 9; // num of rooms created between start and boss rooms
	int roomCount = 0;
	int wastedMoves = 0; //for debugging purposes
	
	//spawn 9 rooms (including boss room) randomly
	//get random direction and prevent overlap
	while (roomCount < roomBudget) {

		//pick random direction and get nextLocation
		FVector nextLocation;
		FVector farLocation;
		ARoomBase::direction roomDirection;
		int randDirection = FMath::RandRange(1, 4);

		switch (randDirection) {
			//north
		case 1:
			nextLocation = currentLocation + FVector(0.f, offset, 0.f);
			farLocation = nextLocation + FVector(0.f, offset, 0.f);
			roomDirection = ARoomBase::direction::NORTH;
			break;

			//east
		case 2:
			nextLocation = currentLocation + FVector(offset, 0.f, 0.f);
			farLocation = nextLocation + FVector(offset, 0.f, 0.f);
			roomDirection = ARoomBase::direction::EAST;
			break;

			//south
		case 3:
			nextLocation = currentLocation + FVector(0.f, -offset, 0.f);
			farLocation = nextLocation + FVector(0.f, -offset, 0.f);
			roomDirection = ARoomBase::direction::SOUTH;
			break;

			//west
		case 4:
			nextLocation = currentLocation + FVector(-offset, 0.f, 0.f);
			farLocation = nextLocation + FVector(-offset, 0.f, 0.f);
			roomDirection = ARoomBase::direction::WEST;
			break;
		}

		//if next location is occupied
		if (rooms.Contains(nextLocation) && rooms.Contains(currentLocation)) {
			ARoomBase* nextRoom = *rooms.Find(nextLocation);
			if (nextRoom) {
				UE_LOGFMT(LogTemp, Display, "space is occupied by room {number}", nextRoom->roomID);
			}

			/*
			ARoomBase* nextRoom = *rooms.Find(nextLocation);
			ARoomBase* currentRoom = *rooms.Find(currentLocation);
			if (currentRoom && nextRoom) {
				currentRoom->connectedRooms.Add(roomDirection, nextRoom);
				ARoomBase::direction oppositeDirection = ARoomBase::getOpposite(roomDirection);
				nextRoom->connectedRooms.Add(oppositeDirection, currentRoom);
			}
			*/
			currentLocation = nextLocation;
			wastedMoves += 1;
			continue;
		}

		//else next location is not occupied
		else {
			//if connected rooms map, in room at currentLocation, does NOT contain room at next location
			//then add room at next location to connected rooms map of room at current location
			ARoomBase* currentRoom;
			ARoomBase* newRoom;

			if (rooms.Contains(currentLocation)) {
				//get current room
				currentRoom = *rooms.Find(currentLocation);
				//get next room
				currentLocation = nextLocation;
				newRoom = GetWorld()->SpawnActor<ARoomBase>(actorToSpawn, currentLocation, spawnRotation, spawnInfo);
				if (newRoom) {
					newRoom->roomID = roomNumber;
					roomNumber += 1;
				}
				rooms.Add(currentLocation, newRoom);
				if (newRoom) {
					UE_LOGFMT(LogTemp, Display, "Room {number}", newRoom->roomID);
				}

				//add connections to each room surrounding the next room -- including current room
				if (currentRoom && newRoom) {
					/*
					currentRoom->connectedRooms.Add(roomDirection, newRoom);
					ARoomBase::direction oppositeDirection = ARoomBase::getOpposite(roomDirection);
					newRoom->connectedRooms.Add(oppositeDirection, currentRoom);
					UE_LOGFMT(LogTemp, Display, "test: is connected to {number}", currentRoom->roomID);

					if (rooms.Contains(farLocation)) {
					ARoomBase* farRoom = *rooms.Find(farLocation);
					newRoom->connectedRooms.Add(roomDirection, farRoom);
					farRoom->connectedRooms.Add(oppositeDirection, newRoom);
					UE_LOGFMT(LogTemp, Display, "far: is connected to {number}", farRoom->roomID);
					*/

					//NORTH
					FVector northLocation = currentLocation + FVector(0.f, offset, 0.f);
					if (rooms.Contains(northLocation)) {
						ARoomBase* northRoom = *rooms.Find(northLocation);
						newRoom->connectedRooms.Add(ARoomBase::NORTH, northRoom);
						northRoom->connectedRooms.Add(ARoomBase::SOUTH, newRoom);
						UE_LOGFMT(LogTemp, Display, "is connected to {number} (NORTH)", northRoom->roomID);
					}


					//SOUTH
					FVector southLocation = currentLocation + FVector(0.f, -offset, 0.f);
					if (rooms.Contains(southLocation)) {
						ARoomBase* southRoom = *rooms.Find(southLocation);
						newRoom->connectedRooms.Add(ARoomBase::SOUTH, southRoom);
						southRoom->connectedRooms.Add(ARoomBase::NORTH, newRoom);
						UE_LOGFMT(LogTemp, Display, "is connected to {number} (SOUTH)", southRoom->roomID);
					}


					//WEST -- positive on the x axis
					FVector westLocation = currentLocation + FVector(offset, 0.f, 0.f);
					if (rooms.Contains(westLocation)) {
						ARoomBase* westRoom = *rooms.Find(westLocation);
						newRoom->connectedRooms.Add(ARoomBase::EAST, westRoom);
						westRoom->connectedRooms.Add(ARoomBase::WEST, newRoom);
						UE_LOGFMT(LogTemp, Display, "is connected to {number} (WEST)", westRoom->roomID);
					}

					//EAST -- negative on the x axis
					FVector eastLocation = currentLocation + FVector(-offset, 0.f, 0.f);
					if (rooms.Contains(eastLocation)) {
						ARoomBase* eastRoom = *rooms.Find(eastLocation);
						newRoom->connectedRooms.Add(ARoomBase::direction::WEST, eastRoom);
						eastRoom->connectedRooms.Add(ARoomBase::direction::EAST, newRoom);
						UE_LOGFMT(LogTemp, Display, "is connected to {number} (EAST)", eastRoom->roomID);
					}

					//randomly select content type for each room:
					ARoomBase::contentType roomContent = ARoomBase::contentType::EMPTY;
					int randContent = FMath::RandRange(1, 4);
					switch (randContent) {
					case 1:
						roomContent = ARoomBase::contentType::PUZZLE;
						break;

					case 2:
						roomContent = ARoomBase::contentType::MONSTER;
						GetWorld()->SpawnActor<AMonsterMarker>(monsterMarker, currentLocation, spawnRotation, spawnInfo);
						break;

					case 3:
						roomContent = ARoomBase::contentType::KEY;
						break;

					case 4:
						roomContent = ARoomBase::contentType::LOOT;
						GetWorld()->SpawnActor<AChestActor>(treasureChest, currentLocation, spawnRotation, spawnInfo);
						break;

					case 5:
						break;
					}
					newRoom->content = roomContent;

					
					//print room name, content type, and all of its connections to log
					//newRoom

					
				}
			}
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Added Room"));
			roomCount += 1;
		}

		UE_LOGFMT(LogTemp, Display, "Number of wasted moves: {number}", wastedMoves);


		//for each room in rooms

		
	}
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

