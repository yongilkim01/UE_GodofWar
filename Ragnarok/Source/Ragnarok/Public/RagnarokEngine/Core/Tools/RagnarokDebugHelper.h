#pragma once

#include "../Ragnarok.h"

namespace Debug
{
	static void Print(const FString& DebugMessage, const FColor& DisplayColor = FColor::White, bool bAssert = false, int32 InKey = -1, float TimeToDisplay = 7.f )
	{
		if (nullptr != GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, TimeToDisplay, DisplayColor, DebugMessage);

			if (FColor::White == DisplayColor)
			{
				UE_LOG(RAGNAROK_LOG, Log, TEXT("%s"), *DebugMessage);
			}
			else if (FColor::Yellow == DisplayColor)
			{
				UE_LOG(RAGNAROK_LOG, Warning, TEXT("%s"), *DebugMessage);
			}
			else if (FColor::Red == DisplayColor)
			{
				if (true == bAssert)
				{
					UE_LOG(RAGNAROK_LOG, Error, TEXT("%s"), *DebugMessage);
				}
				else
				{
					UE_LOG(RAGNAROK_LOG, Fatal, TEXT("%s"), *DebugMessage);
				}
			}
		}
	}
}