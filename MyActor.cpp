// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"

#include "Kismet/KismetSystemLibrary.h"

AMyActor::AMyActor() {
  MyControlRigComponent = CreateDefaultSubobject<UControlRigComponent>(
      TEXT("MyControlRigComponent"));
  MyControlRigComponent->OnPreInitializeDelegate.AddDynamic(
      this, &AMyActor::CleanAndMapSkeletalMesh);
}

void AMyActor::Tick(float DeltaSeconds) {
  Super::Tick(DeltaSeconds);

  CurrentTime += DeltaSeconds;
  float Value = (FMath::Sin(CurrentTime) + 1.f) * 0.5f;

  MyControlRigComponent->SetControlFloat(FName("CTRL_L_eye_blink"), Value);
}

void AMyActor::BeginPlay() {
  Super::BeginPlay();

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

  UObject *FaceControlRigObject = StaticLoadObject(
      UClass::StaticClass(), nullptr,
      TEXT("/Game/MetaHumans/Common/Face/"
           "Face_ControlBoard_CtrlRig.Face_ControlBoard_CtrlRig_C"));
  checkf(FaceControlRigObject, TEXT("FaceControlRigObject not valid."));

  UClass *FaceControlRigClass = Cast<UClass>(FaceControlRigObject);
  checkf(FaceControlRigClass, TEXT("FaceControlRigClass not valid."));

  MyControlRigComponent->ControlRigClass = FaceControlRigClass;

  checkf(MyFaceSkeletalMeshComponent,
         TEXT("Face's skeletal mesh component is nullptr."));
  MyControlRigComponent->AddMappedCompleteSkeletalMesh(
      MyFaceSkeletalMeshComponent);
  MyControlRigComponent->SetupAttachment(MyFaceSkeletalMeshComponent);
}

bool AMyActor::IsMetaHuman(const AActor *Actor) {
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
      MyFaceSkeletalMeshComponent = Element;
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

void AMyActor::CleanAndMapSkeletalMesh(UControlRigComponent *Component) {
  UE_LOG(LogTemp, Warning, TEXT("CleanAndMap"));
  Component->ClearMappedElements();
  Component->AddMappedCompleteSkeletalMesh(MyFaceSkeletalMeshComponent);
}