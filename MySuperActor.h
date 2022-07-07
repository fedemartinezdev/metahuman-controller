// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySceneComponent.h"
// clang-format off
#include "MySuperActor.generated.h"
// clang-format on

UCLASS()
class MYPROJECTCPP_API AMySuperActor : public AActor {
  GENERATED_BODY()

public:
  AMySuperActor();

  UFUNCTION(BlueprintCallable)
  void CreateMetaHuman();

private:
  bool IsMetaHuman(const AActor *Actor);

  AActor *MetaHuman;
  USkeletalMeshComponent *FaceSkeletalMeshComponent;
  UMySceneComponent *MySceneComponent;
};
