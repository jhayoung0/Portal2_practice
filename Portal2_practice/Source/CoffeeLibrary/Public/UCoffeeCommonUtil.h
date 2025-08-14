// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UCoffeeCommonUtil.generated.h"

UCLASS()
class COFFEELIBRARY_API UCoffeeCommonUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 인덱스가 유효한 범위 (0, Count) 내에 있는지 확인합니다.
	 * @param Index 검사할 인덱스
	 * @param Count 배열 또는 리스트의 총 길이 (상한값, 미포함)
	 */
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "In Bounds"))
	static bool InBounds(const int32 Index, const int32 Count);

	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "NowTimestamp"))
	static int64 GetNowTimestamp();	


	static void TestULog();
	static void TestInBound();
	
	/**
	 * @brief 액터에 부착된 특정 이름의 컴포넌트를 찾습니다.
	 * @tparam T 찾고자 하는 컴포넌트의 타입 (예: USceneComponent, UStaticMeshComponent 등)
	 * @param Owner 컴포넌트를 소유하는 액터.
	 * @param ComponentName 찾고자 하는 컴포넌트의 이름.
	 * @return 찾은 컴포넌트의 포인터. 찾지 못하면 nullptr을 반환합니다.
	 */
	template<typename T>
	static T* FindComponentByName(AActor* Owner, const FName& ComponentName)
	{
		if (Owner == nullptr)
			return nullptr;

		TArray<T*> Components;
		Owner->GetComponents<T>(Components);

		for (T* Comp : Components)
		{
			if (Comp && Comp->GetFName() == ComponentName)
			{
				return Comp;
			}
		}
	
		return nullptr;
	}

	template<typename T>
	static T* FindComponentByNameRecursive(AActor* Owner, const FName& ComponentName)
	{
		if (Owner == nullptr)
			return nullptr;

		// 1. 현재 Actor에서 먼저 검색
		TArray<T*> Components;
		Owner->GetComponents<T>(Components);

		for (T* Comp : Components)
		{
			if (Comp && Comp->GetFName() == ComponentName)
			{
				return Comp;
			}
		}

		TArray<AActor*> AttachedActors;
		Owner->GetAttachedActors(AttachedActors);

		for (AActor* ChildActor : AttachedActors)
		{
			if (ChildActor)
			{
				if (T* Found = FindComponentByNameRecursive<T>(ChildActor, ComponentName))
				{
					return Found;
				}
			}
		}

		return nullptr;
	};
};

