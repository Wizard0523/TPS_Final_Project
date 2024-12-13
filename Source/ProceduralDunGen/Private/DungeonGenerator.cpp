// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
#include "RoomBase.h"
#include "MonsterMarker.h"
#include "Logging/StructuredLog.h"
#include "ChestActor.h"
#include "KeyActor.h"
#include "PuzzleActor.h"


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
	ARoomBase* rootRoom = initDungeon();
	rewriteContent(rootRoom);
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ARoomBase* ADungeonGenerator::initDungeon()
{
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
	int roomBudget = 100; // num of rooms created between start and boss rooms
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
					int randContent = FMath::RandRange(1, 12);
					switch (randContent) {
					case 1:
						roomContent = ARoomBase::contentType::PUZZLE;
						GetWorld()->SpawnActor<APuzzleActor>(puzzle, currentLocation, spawnRotation, spawnInfo);
						break;

					case 2:
						roomContent = ARoomBase::contentType::MONSTER;
						GetWorld()->SpawnActor<AMonsterMarker>(monsterMarker, currentLocation, spawnRotation, spawnInfo);
						break;

					case 3:
						roomContent = ARoomBase::contentType::KEY;
						GetWorld()->SpawnActor<AKeyActor>(treasureKey, currentLocation, spawnRotation, spawnInfo);
						break;

					case 4:
						roomContent = ARoomBase::contentType::LOOT;
						GetWorld()->SpawnActor<AChestActor>(treasureChest, currentLocation, spawnRotation, spawnInfo);
						break;

					default:
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
void ADungeonGenerator::rewriteContent(ARoomBase* rootNode)
{
	//traverse graph using depth first search: https://www.youtube.com/watch?v=ymlzHmRN4To

	// Stack used to return to previous rooms:
	// Pop elements from the stack when they are fully explored,
	// i.e. they are connected to no unexplored rooms
	TArray<ARoomBase*> recentRooms;
	ARoomBase* currentRoom = rootNode;
	if (currentRoom != nullptr) {
		currentRoom->isStart = true;
		UE_LOG(LogTemp, Display, TEXT("currentRoom is not null"));
	}
	
	UE_LOG(LogTemp, Display, TEXT("--- Initiate DFS ---"));
	recentRooms.Add(rootNode);
	UE_LOG(LogTemp, Display, TEXT("room 1 added to recentRooms"));
	int loopCount = 0;
	
	while (recentRooms.Num() > 0) {
		loopCount += 1;
		UE_LOGFMT(LogTemp, Display, "loop {number}:", loopCount);
		UE_LOG(LogTemp, Display, TEXT(""));
		if (currentRoom != nullptr) {
			//first visit:
			if (!currentRoom->beenVisited) {
				UE_LOGFMT(LogTemp, Display, "room {number} has NOT been visited before", currentRoom->roomID);
				getUnexploredRooms(currentRoom);
				UE_LOGFMT(LogTemp, Display, "created stack for room {number}", currentRoom->roomID);
				currentRoom->beenVisited = true;
			}

			//each visit:
			printStack(currentRoom);

			//no more rooms to explore:
			if (currentRoom->unexploredRooms.Num() == 0) {
				if (recentRooms.Num() > 0) {
					currentRoom = recentRooms.Pop();
					UE_LOGFMT(LogTemp, Display, "room {number} popped from recent rooms' stack", currentRoom->roomID);
					continue;
				}
				else {
					UE_LOG(LogTemp, Display, TEXT("No more recent rooms. Exiting.."));
					break;
				}
			}

			if (currentRoom->unexploredRooms.Num() == 1) {
				currentRoom = currentRoom->unexploredRooms.Pop();
				UE_LOGFMT(LogTemp, Display, "exploring room {number}", currentRoom->roomID);
				continue;
			}

			if (currentRoom->unexploredRooms.Num() > 1) {
				if (!recentRooms.Contains(currentRoom)) {
					recentRooms.Add(currentRoom);
				}
				UE_LOGFMT(LogTemp, Display, "added room {number} to recent rooms", currentRoom->roomID);
				currentRoom = currentRoom->unexploredRooms.Pop();
				UE_LOGFMT(LogTemp, Display, "exploring room {number}", currentRoom->roomID);
				continue;
			}
		}
		
		if (loopCount > 1000) {
			UE_LOG(LogTemp, Display, TEXT("Exceeded Loop Count. Exiting.."));
			break;
		}
	}
	

	/*
	while (recentRooms.Num() > 0) {
		if (currentRoom != nullptr) {
			if (currentRoom->beenVisited) {
				UE_LOGFMT(LogTemp, Display, "room {number} has been visited before", currentRoom->roomID);

				//if current room has no more rooms to explore, remove it from recent rooms,
				//and move current room to next room on top of recentRooms stack
				if (currentRoom->unexploredRooms.Num() == 0) {
					if (recentRooms.Contains(currentRoom)) {
						recentRooms.Remove(currentRoom);
						UE_LOGFMT(LogTemp, Display, "room {number} has been explored, and removed from recent rooms stack", currentRoom->roomID);
					}
					if (recentRooms.Num() > 0) {
						currentRoom = recentRooms.Last();
					}
					else {
						UE_LOG(LogTemp, Display, TEXT("there are no more recent rooms, exiting.."));
						continue;
					} 
					UE_LOGFMT(LogTemp, Display, "switching to room {number} from recentRooms stack", currentRoom->roomID);
				}
				//else, currrent room has more rooms to explore, so it should remain on the stack
				//move to next unexplored room off of current room
				else {
					UE_LOGFMT(LogTemp, Display, "room {number} has more rooms to explore, so it'll remain in recent rooms stack", currentRoom->roomID);
					currentRoom = currentRoom->unexploredRooms.Pop();
					UE_LOGFMT(LogTemp, Display, "exploring room {number}", currentRoom->roomID);
				}
			}
			//current room has not been visited -- a stack, to track unexplored rooms, needs to be created
			else {
				UE_LOGFMT(LogTemp, Display, "first time visiting room {number}", currentRoom->roomID);
				currentRoom->beenVisited = true;
				//for each connected room:
				for (auto It = currentRoom->connectedRooms.CreateConstIterator(); It; ++It)
				{
					//add unexplored rooms to a stack
					if (!It.Value()->beenVisited) {
						currentRoom->unexploredRooms.Add(It.Value());
						UE_LOGFMT(LogTemp, Display, "unexplored room ({number}) added to stack", It.Value()->roomID);

						//display for development purposes
						for (int i = 0; i < currentRoom->unexploredRooms.Num(); ++i) {
							UE_LOGFMT(LogTemp, Display, "room {number} still on the stack", currentRoom->unexploredRooms[i]->roomID);
						}
					}
					
				}
				//if currentRoom has no rooms to explore, then go back to recent room
				if (currentRoom->unexploredRooms.Num() == 0) {
					UE_LOGFMT(LogTemp, Display, "current room ({number}) ", currentRoom->roomID);
					currentRoom = recentRooms.Pop();
					UE_LOGFMT(LogTemp, Display, "going back to room {number}", currentRoom->roomID);

				}
				//else current room has more rooms to explore
				else {
					//if it has only 1 unexploredRoom, that room will be explored, so it doesn't need to be saved
					//how do I avoid adding the starting room to recent rooms twice?
					if (currentRoom->unexploredRooms.Num() > 1) {
						if (currentRoom->isStart != true) {
							UE_LOGFMT(LogTemp, Display, "adding room {number} to recent rooms stack", currentRoom->roomID);
							recentRooms.Add(currentRoom);
						}
					}
					currentRoom = currentRoom->unexploredRooms.Pop();
					UE_LOGFMT(LogTemp, Display, "exploring room {number}", currentRoom->roomID);
				}
				
			}
			loopCount += 1;
			if (loopCount >= 10000) {
				break;
			}
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("current room has null ptr"));
			break;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("All done!"));
	*/
}

void ADungeonGenerator::getUnexploredRooms(ARoomBase* currentRoom)
{
	for (auto It = currentRoom->connectedRooms.CreateConstIterator(); It; ++It)
	{
		//add unexplored rooms to a stack
		if (!It.Value()->beenVisited) {
			currentRoom->unexploredRooms.Add(It.Value());
			UE_LOGFMT(LogTemp, Display, "unexplored room ({number}) added to stack", It.Value()->roomID);
		}
	}
}

void ADungeonGenerator::printStack(ARoomBase* currentRoom)
{
	//for each connected room:
	for (int i = 0; i < currentRoom->unexploredRooms.Num(); ++i) {
		UE_LOGFMT(LogTemp, Display, "room {number} still on the stack", currentRoom->unexploredRooms[i]->roomID);
	}
}





