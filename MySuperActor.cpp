// Fill out your copyright notice in the Description page of Project Settings.

#include "MySuperActor.h"

// Sets default values
AMySuperActor::AMySuperActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

void AMySuperActor::CreateMetaHuman() {
  UWorld *World = GetWorld();
  UObject *MetaHumanObject = StaticLoadObject(
      UClass::StaticClass(), nullptr,
      TEXT("/Game/MetaHumans/Bernice/BP_Bernice.BP_Bernice_C"));
  checkf(MetaHumanObject, TEXT("MetaHumanObject not valid."));

  UClass *MetaHumanClass = Cast<UClass>(MetaHumanObject);
  checkf(MetaHumanClass, TEXT("MetaHumanClass not valid."));

  FActorSpawnParameters SpawnParameters;
  MetaHuman = World->SpawnActor<AActor>(MetaHumanClass, FVector::ZeroVector,
                                        FRotator::ZeroRotator, SpawnParameters);
  checkf(IsMetaHuman(MetaHuman), TEXT("Actor is not a MetaHuman!"));

  // Attach MySceneComponent to MetaHuman
  MySceneComponent =
      NewObject<UMySceneComponent>(MetaHuman, "MySceneComponent");
  MySceneComponent->RegisterComponent();
  MySceneComponent->AttachToComponent(
      MetaHuman->GetRootComponent(),
      FAttachmentTransformRules::KeepRelativeTransform);
  MySceneComponent->InitializeComponent();
}

bool AMySuperActor::IsMetaHuman(const AActor *Actor) {
  if (!Actor) {
    return false;
  }

  // Get all skeletal mesh components
  TArray<USkeletalMeshComponent *> SkeletalMeshComponents;
  Actor->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

  bool HasBody = false;
  bool HasFace = false;
  bool HasTorso = false;
  bool HasLegs = false;
  bool HasFeet = false;

  for (auto &Element : SkeletalMeshComponents) {
    if (Element->GetFName().ToString().Compare(TEXT("Body")) == 0) {
      HasBody = true;
    } else if (Element->GetFName().ToString().Compare(TEXT("Face")) == 0) {
      HasFace = true;
      FaceSkeletalMeshComponent = Element;
    } else if (Element->GetFName().ToString().Compare(TEXT("Torso")) == 0) {
      HasTorso = true;
    } else if (Element->GetFName().ToString().Compare(TEXT("Legs")) == 0) {
      HasLegs = true;
    } else if (Element->GetFName().ToString().Compare(TEXT("Feet")) == 0) {
      HasFeet = true;
    }

    if (HasBody && HasFace && HasTorso && HasLegs && HasFeet) {
      return true;
    }
  }

  return false;
}