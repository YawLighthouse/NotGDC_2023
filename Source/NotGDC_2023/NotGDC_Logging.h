// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NotGDC_LoggingLibrary.h"

// Outputs CLASS::FUNCTION for FUNC_NAME as const TCHAR*
#ifdef _MSC_VER
#define FUNC_WIDEN2(x) L ## x
#define FUNC_WIDEN(x) FUNC_WIDEN2(x)
#define FUNC_NAME FUNC_WIDEN(__FUNCTION__)
#else
#define FUNC_NAME (*(FString(__func__)))
#endif

#define UTC_TIME (FDateTime::UtcNow().ToUnixTimestamp())

#if NO_LOGGING
#define LOG_NOTGDC_INTERNAL(LogCategory, Verbosity, Format, ...) do { } while(false)
#else
#define LOG_NOTGDC_INTERNAL(LogCategory, Verbosity, Format, ...) \
do { \
	const FString Internal_Prefixed_Message = FString::Printf(TEXT("%s %lld: %s"), FUNC_NAME, UTC_TIME, *Format); \
	UE_LOG(LogCategory, Verbosity, TEXT("%s"), *Internal_Prefixed_Message); \
} while(false)
#endif

#define ON_SCREEN_PRINT_NOTGDC(Duration, Verbosity, Format, ...) \
{ \
	FLinearColor TextColor = FLinearColor(0.0f, 0.66f, 1.0f); \
	switch(ELogVerbosity::Verbosity) \
	{ \
		case ELogVerbosity::Warning: \
		{ \
			TextColor = FLinearColor::Yellow; \
			break; \
		} \
		case ELogVerbosity::Error: \
		{ \
			TextColor = FLinearColor::Red; \
			break; \
		} \
		default: \
			break; \
	} \
	const FString FormattedMessage = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	const FString PrefixedMessage = FString::Printf(TEXT("[%s] %s"), FUNC_NAME, *FormattedMessage); \
	UNotGDC_LoggingLibrary::OnScreenPrint(PrefixedMessage, TextColor, Duration); \
}

#define LOG_NOTGDC(LogCategory, Verbosity, Format, ...) \
{ \
	const FString FormattedMessage = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	LOG_NOTGDC_INTERNAL(LogCategory, Verbosity, FormattedMessage); \
}

#define LOG_SCREEN_NOTGDC(LogCategory, Duration, Verbosity, Format, ...) \
{ \
	LOG_NOTGDC(LogCategory, Verbosity, Format, ##__VA_ARGS__); \
	ON_SCREEN_PRINT_NOTGDC(Duration, Verbosity, Format, ##__VA_ARGS__); \
}

