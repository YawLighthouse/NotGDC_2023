// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NotGDC_LoggingLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNotGDCLoggingLibrary, Display, All);

/**
 * 
 */
UCLASS(ClassGroup = "NotGDC")
class NOTGDC_2023_API UNotGDC_LoggingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
 	 * Prints a string onscreen. 	 
 	 *
 	 * @param Message The string to print onscreen 	 
 	 * @param TextColor The color of the text to display
 	 * @param Duration The display duration. Using negative number will result in loading the duration time from the config. 	 
 	 */
	UFUNCTION(BlueprintCallable, Category = "notGDC Logging Library", meta = (CallableWithoutWorldContext, AdvancedDisplay = "1", DevelopmentOnly))
	static void OnScreenPrint(const FString& Message = FString(TEXT("Hello")), const FLinearColor TextColor = FLinearColor(0.0f, 0.66f, 1.0f), const float Duration = 2.0f);
	
};
