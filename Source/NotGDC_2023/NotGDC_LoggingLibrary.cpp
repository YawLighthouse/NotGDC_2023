// Copyright 2023 Nicholas Helish. All rights reserved.


#include "NotGDC_LoggingLibrary.h"

#include "NotGDC_Logging.h"

DEFINE_LOG_CATEGORY(LogNotGDCLoggingLibrary);


/**
 * Prints a string onscreen. 	 
 *
 * @param Message The string to print onscreen 	 
 * @param TextColor The color of the text to display
 * @param Duration The display duration. Using negative number will result in loading the duration time from the config. 	 
 */
void UNotGDC_LoggingLibrary::OnScreenPrint(const FString& Message, const FLinearColor TextColor, const float Duration)
{
	if(GEngine == nullptr)
	{
		LOG_NOTGDC(LogNotGDCLoggingLibrary, Error, "NULL GEngine");
		return;
	}
	if(!GAreScreenMessagesEnabled)
	{
		LOG_NOTGDC(LogNotGDCLoggingLibrary, Error, "GAreScreenMessagesEnabled is false. Cannot print to screen.");
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, Duration, TextColor.ToFColor(true), Message);
}
