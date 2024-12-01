// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"




// Sets default values
ARoomBase::ARoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ARoomBase::direction ARoomBase::getOpposite(direction dir)
{
	ARoomBase::direction oppositeDirection;

	switch (dir) {
	case NORTH:
		oppositeDirection = SOUTH;
		break;

	case EAST:
		oppositeDirection = WEST;
		break;

	case SOUTH:
		oppositeDirection = NORTH;
		break;

	case WEST:
		oppositeDirection = EAST;
		break;
	}
	return oppositeDirection;
}

/*

{	
	enum direction oppositeDirection;

	
	return oppositeDirection;
}
*/

