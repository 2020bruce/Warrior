#pragma once

namespace Debug
{
	void LogWarning(const FString& Message);
	void LogError(const FString& Message);

	inline void Log(const FString& Message, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 5.f, Color, Message);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}
}
