// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ExampleActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExampleActor, Log, All);

UCLASS(ClassGroup = "NotGDC")
class NOTGDC_2023_API AExampleActor : public AStaticMeshActor
{
	GENERATED_BODY()
public:

	AExampleActor();
	
};
