#include "Features/UGameEventManager.h"

void UGameEventManager::SendMessage(const FString& InMsg)
{
	OnMessage.Broadcast(InMsg);
}