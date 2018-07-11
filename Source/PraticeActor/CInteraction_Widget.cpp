// Fill out your copyright notice in the Description page of Project Settings.

#include "CInteraction_Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

void UCInteraction_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	Interaction = this->WidgetTree->FindWidget<UTextBlock>(FName("Interaction"));
}

void UCInteraction_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Interaction)
		Interaction->SetText(FText::FromString(FoundFieldItemName));
}
