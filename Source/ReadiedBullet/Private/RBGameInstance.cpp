// Fill out your copyright notice in the Description page of Project Settings.


#include "RBGameInstance.h"

void URBGameInstance::SetInstanceBoxSlot1(TArray<FTransform> BoxSlot)
{
	InstanceBoxSlot1 = BoxSlot;
}

void URBGameInstance::SetInstanceCylinderSlot1(TArray<FTransform> CylinderSlot)
{
	InstanceCylinderSlot1 = CylinderSlot;
}

void URBGameInstance::SetInstanceSphereSlot1(TArray<FTransform> SphereSlot)
{
	InstanceSphereSlot1 = SphereSlot;
}

TArray<FTransform> URBGameInstance::GetInstanceBoxSlot1()
{
	return InstanceBoxSlot1;
}

TArray<FTransform> URBGameInstance::GetInstanceCylinderSlot1()
{
	return InstanceCylinderSlot1;
}

TArray<FTransform> URBGameInstance::GetInstanceSphereSlot1()
{
	return InstanceSphereSlot1;
}
