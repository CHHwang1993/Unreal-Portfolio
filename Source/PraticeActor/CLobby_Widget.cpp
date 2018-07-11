// Fill out your copyright notice in the Description page of Project Settings.

#include "CLobby_Widget.h"
#include "CLobbyPC.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/WidgetTree.h"
#include "Engine.h"

void UCLobby_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	JoinServerButton = WidgetTree->FindWidget<UButton>(FName("JoinServer"));
	JoinServerButton->OnClicked.AddDynamic(this, &UCLobby_Widget::OnJoinServer);

	IPAdressTextBox = WidgetTree->FindWidget<UEditableTextBox>(FName("IpAdress"));
}

void UCLobby_Widget::OnJoinServer()
{
	ACLobbyPC* pc = Cast<ACLobbyPC>(GetOwningPlayer());

	if (pc)
	{
		pc->JoinServer(IPAdressTextBox->GetText().ToString());
	}
}

