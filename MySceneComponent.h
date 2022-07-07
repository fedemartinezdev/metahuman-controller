// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ControlRigComponent.h"
#include "CoreMinimal.h"
// clang-format off
#include "MySceneComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECTCPP_API UMySceneComponent : public USceneComponent {
  GENERATED_BODY()

public:
  UMySceneComponent();

  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;

  void InitializeComponent() override;

private:
  UFUNCTION()
  void CleanAndMap(UControlRigComponent *Component);

  USkeletalMeshComponent *GetSkeletalMeshComponent(const FString &Name);

  float CurrentTime = 0.f;
  UControlRigComponent *FaceControlRigComponent{nullptr};
  USkeletalMeshComponent *FaceSkeletalMeshComponent{nullptr};
};
