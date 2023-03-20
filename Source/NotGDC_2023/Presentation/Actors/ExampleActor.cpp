// Copyright 2023 Nicholas Helish. All rights reserved.


#include "ExampleActor.h"

DEFINE_LOG_CATEGORY(LogExampleActor);

AExampleActor::AExampleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

