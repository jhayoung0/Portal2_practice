// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "UPortalEventManager.h"

void UPortalEventManager::SendMessage(const FString& InMsg)
{
	OnMessage.Broadcast(InMsg);
}

void UPortalEventManager::SendLight(const int& InGroup, bool InState)
{
	OnLight.Broadcast(InGroup, InState);
}

void UPortalEventManager::SendMovie(const int& InGroup, bool InState)
{
	OnMovie.Broadcast(InGroup, InState);
}