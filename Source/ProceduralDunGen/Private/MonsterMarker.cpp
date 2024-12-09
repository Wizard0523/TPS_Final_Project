// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterMarker.h"

// Sets default values
AMonsterMarker::AMonsterMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterMarker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

