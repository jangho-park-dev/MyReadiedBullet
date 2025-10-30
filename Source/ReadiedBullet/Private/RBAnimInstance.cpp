// Fill out your copyright notice in the Description page of Project Settings.


#include "RBAnimInstance.h"

URBAnimInstance::URBAnimInstance()
{
	IsDead = false;
}

void URBAnimInstance::setDead()
{
	IsDead = true;
}