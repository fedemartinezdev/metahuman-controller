// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControlRigComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySceneComponent.h"
// clang-format off
#include "MyActor.generated.h"
// clang-format on

UCLASS()
class MYPROJECTCPP_API AMyActor : public AActor {
  GENERATED_BODY()

public:
  AMyActor();
  void Tick(float DeltaSeconds) override;

protected:
  void BeginPlay() override;

private:
  bool IsMetaHuman(const AActor *Actor);

  UFUNCTION()
  void CleanAndMapSkeletalMesh(UControlRigComponent *Component);

  float CurrentTime{0.f};
  AActor *MetaHuman{nullptr};
  USkeletalMeshComponent *MyFaceSkeletalMeshComponent{nullptr};
  UControlRigComponent *MyControlRigComponent{nullptr};
};
