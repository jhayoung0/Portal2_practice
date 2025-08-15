// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

namespace ComponentHelper
{
	template<typename T>
	static T* FindComponentByName(AActor* Owner, const FName& ComponentName)
	{
		if (!IsValid((Owner)))
		{
			return nullptr;
		}

		TArray<T*> FoundComponents;
		Owner->GetComponents(FoundComponents);

		for (T* Component : FoundComponents)
		{
			if (Component && Component->GetFName() == ComponentName)
			{
				return Component;
			}
		}
		return nullptr;
	}
	template<typename T>
	static T* FindComponentByNameRecursive(AActor* Owner, const FName& ComponentName)
	{
		if (!IsValid((Owner)))
			return nullptr;

		TArray<UActorComponent*> Components;
		Owner->GetComponents(Components, true);

		for (UActorComponent* Component : Components)
		{
			if (Component && Component->GetFName() == ComponentName)
			{
				return Cast<T>(Component);
			}
		}
		return nullptr;
	}
};