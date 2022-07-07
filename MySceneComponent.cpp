// Fill out your copyright notice in the Description page of Project Settings.

#include "MySceneComponent.h"

// Sets default values for this component's properties
UMySceneComponent::UMySceneComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  UE_LOG(LogTemp, Warning, TEXT("UMySceneComponent Constructor"));
  FaceControlRigComponent = CreateDefaultSubobject<UControlRigComponent>(
      TEXT("FaceControlRigComponent"));
  FaceControlRigComponent->RegisterComponent();
  FaceControlRigComponent->SetupAttachment(this);
  FaceControlRigComponent->OnPreInitializeDelegate.AddDynamic(
      this, &UMySceneComponent::CleanAndMap);
}

void UMySceneComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  CurrentTime += DeltaTime;
  float Value = (FMath::Sin(CurrentTime) + 1.f) / 2.f;
  FaceControlRigComponent->SetControlFloat(FName("CTRL_L_eye_blink"), Value);
}

void UMySceneComponent::InitializeComponent() {
  Super::InitializeComponent();
  UE_LOG(LogTemp, Warning, TEXT("UMySceneComponent InitializeComponent"));

  UObject *FaceControlRigObject = StaticLoadObject(
      UClass::StaticClass(), nullptr,
      TEXT("/Game/MetaHumans/Common/Face/"
           "Face_ControlBoard_CtrlRig.Face_ControlBoard_CtrlRig_C"));
  checkf(FaceControlRigObject, TEXT("FaceControlRigObject not valid."));

  UClass *FaceControlRigClass = Cast<UClass>(FaceControlRigObject);
  checkf(FaceControlRigClass, TEXT("FaceControlRigClass not valid."));

  FaceControlRigComponent->ControlRigClass = FaceControlRigClass;
  if (!FaceSkeletalMeshComponent) {
    FaceSkeletalMeshComponent = GetSkeletalMeshComponent(TEXT("Face"));
  }

  // TSoftObjectPtr<UObject> DefaultAnimatingRig =
  //     FaceSkeletalMeshComponent->SkeletalMesh->GetDefaultAnimatingRig();
  // FaceControlRigComponent->SetControlRig(
  //     Cast<UControlRig>(DefaultAnimatingRig.Get()));

  CleanAndMap(FaceControlRigComponent);
}

void UMySceneComponent::CleanAndMap(UControlRigComponent *Component) {
  UE_LOG(LogTemp, Warning, TEXT("UMySceneComponent CleanAndMap"));
  Component->ClearMappedElements();

  if (!FaceSkeletalMeshComponent) {
    FaceSkeletalMeshComponent = GetSkeletalMeshComponent(TEXT("Face"));
  }

  Component->AddMappedCompleteSkeletalMesh(FaceSkeletalMeshComponent);
}

USkeletalMeshComponent *
UMySceneComponent::GetSkeletalMeshComponent(const FString &Name) {
  TArray<USkeletalMeshComponent *> SkeletalMeshComponents;
  GetOwner()->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
  for (auto Element : SkeletalMeshComponents) {
    if (Element->GetFName().ToString().Compare("Face") == 0) {
      return Element;
    }
  }
  return nullptr;
}