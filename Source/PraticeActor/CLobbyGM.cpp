// Fill out your copyright notice in the Description page of Project Settings.

#include "CLobbyGM.h"
#include "CLobbyPC.h"

ACLobbyGM::ACLobbyGM()
{
	PlayerControllerClass = ACLobbyPC::StaticClass();

	DefaultPawnClass = NULL;
}

