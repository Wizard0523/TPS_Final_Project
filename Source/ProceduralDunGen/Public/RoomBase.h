// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.generated.h"

UCLASS()
class PROCEDURALDUNGEN_API ARoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomBase();

protected:
	virtual void BeginPlay() override;
	

public:	
	virtual void Tick(float DeltaTime) override;
	
	enum direction {
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	enum contentType {
		START,
		MONSTER,
		LOOT,
		PUZZLE,
		KEY,
		BOSS,
	};

	enum contentType content;

	TMap<direction, ARoomBase*> connectedRooms;
	//direction getOpposite(enum direction);
	static direction getOpposite(direction);
};
