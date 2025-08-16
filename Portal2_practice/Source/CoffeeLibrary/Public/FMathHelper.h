#pragma once

struct FMathHelper
{
	static FVector CalcParabola(const FVector& Start, const FVector& End, float Height, float Alpha)
	{
		// 선형 보간으로 기본 경로
		FVector Base = FMath::Lerp(Start, End, Alpha);

		// 아치 성분 (0→1→0)
		float OffsetZ = FMath::Sin(Alpha * PI) * Height;

		return Base + FVector(0, 0, OffsetZ);
	}

	static void DrawParabolaDebug(
		UWorld* World,
		const FVector& Start,
		const FVector& End,
		float Height,
		int32 NumSegments = 20,
		float LifeTime = 2.0f,
		FColor Color = FColor::Green)
	{
		if (!World) return;

		FVector PrevPos = Start;
		for (int32 i = 1; i <= NumSegments; ++i)
		{
			float Alpha = (float)i / (float)NumSegments;
			FVector CurrPos = CalcParabola(Start, End, Height, Alpha);

			DrawDebugLine(World, PrevPos, CurrPos, Color, false, LifeTime, 0, 1.5f);
			PrevPos = CurrPos;
		}
	}
};
