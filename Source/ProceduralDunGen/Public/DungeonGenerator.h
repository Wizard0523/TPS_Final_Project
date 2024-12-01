// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RoomBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

UCLASS()
class PROCEDURALDUNGEN_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ARoomBase* initDungeon();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> actorToSpawn;
	
	void rewrite();

};
