// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once


// 공통 선언-구현 매크로
#define DEFINE_SUBSYSTEM_GETTER_INLINE(ClassName) \
UFUNCTION(BlueprintPure, Category="CoffeeLibrary|Subsystem", meta=(WorldContext="WorldContextObject")) \
static ClassName* Get(UObject* WorldContext) \
{ \
if (!WorldContext) \
return nullptr; \
if (auto* GI = WorldContext->GetWorld()->GetGameInstance()) \
return GI->GetSubsystem<ClassName>(); \
return nullptr; \
}

// 공통 선언 매크로
#define DECLARE_SUBSYSTEM_GETTER(ClassName) \
UFUNCTION(BlueprintPure, Category="CoffeeLibrary|Subsystem", meta=(WorldContext="WorldContextObject")) \
static ClassName* Get(UObject* WorldContext);

// 공통 구현 매크로
#define DEFINE_SUBSYSTEM_GETTER_IMPL(ClassName) \
static ClassName* Get(UObject* WorldContext) \
{ \
	if (!WorldContext) \
		return nullptr; \
	if (auto* GI = WorldContext->GetWorld()->GetGameInstance()) \
		return GI->GetSubsystem<ClassName>(); \
	return nullptr; \
}
