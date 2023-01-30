// Fill out your copyright notice in the Description page of Project Settings.


#include "BL_GameModeBase.h"

ABL_GameModeBase::ABL_GameModeBase()
	: Super()
{
	// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BL_Content/Character/BL_BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
