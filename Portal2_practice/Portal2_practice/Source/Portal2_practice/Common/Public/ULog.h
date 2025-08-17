// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ULog, Log, All);

#define ULOG(LogLevel, Format, ...) \
{ \
FString TimeStr = FDateTime::Now().ToString(TEXT("%H:%M:%S")); \
UE_LOG(ULog, LogLevel, TEXT("[%s] %s"), *TimeStr, *FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
}
