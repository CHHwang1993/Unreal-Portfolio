// Fill out your copyright notice in the Description page of Project Settings.

#include "CLoadingGM.h"
#include "CLoadingPC.h"

ACLoadingGM::ACLoadingGM()
{
	PlayerControllerClass = ACLoadingPC::StaticClass();
}


