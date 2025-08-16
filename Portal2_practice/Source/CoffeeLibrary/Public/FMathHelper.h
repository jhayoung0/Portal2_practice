#pragma once

struct FMathHelper
{

	/**
	 * @brief 주어진 시간 안에 목표 지점에 도달하기 위한 초기 속도를 계산합니다. (운동 방정식 기반)
	 *
	 * 공식:
	 *   P(t) = Start + V0 * t + 0.5 * g * t^2
	 *   → V0 = (End - Start - 0.5 * g * t^2) / t
	 *
	 * 특징:
	 * - 뉴턴 역학 기반의 실제 투사체/점프 궤적
	 * - 중력 값(GravityZ)을 반영
	 *
	 * 용도:
	 * - 점프 궤적, 포물선 투사체, 실제 물리 시뮬레이션
	 */
	static FVector SolveV0ForProjectile(
		const FVector& Start,
		const FVector& End,
		float Time,
		float GravityZ = -980.f)
	{
		if (Time <= KINDA_SMALL_NUMBER)
			return FVector::ZeroVector;

		const FVector g(0, 0, GravityZ);
		return (End - Start - 0.5f * g * Time * Time) / Time;
	}
	
	/**
	 * @brief 선형 보간 경로에 사인 함수를 더해 아치 형태의 곡선을 생성합니다. (시각 효과 기반)
	 *
	 * 공식:
	 *   Base(α)   = (1-α) * Start + α * End
	 *   Offset(α) = sin(π * α) * Height
	 *   Result    = Base + (0,0,Offset)
	 *
	 * 특징:
	 * - 실제 물리와 무관, 단순히 아치 형태를 연출
	 * - α=0과 α=1에서 0, α=0.5에서 최대 Height
	 *
	 * 용도:
	 * - 스킬 이펙트, UI 이동, 단순한 점프 연출 등
	 */
	static FVector InterpArcSin(
		const FVector& Start,
		const FVector& End,
		float Height,
		float Alpha)
	{
		FVector Base = FMath::Lerp(Start, End, Alpha);
		float OffsetZ = FMath::Sin(Alpha * PI) * Height; // (0→Height→0)
		return Base + FVector(0, 0, OffsetZ);
	}

	/**
	 * @brief 초기 속도 V0를 이용해 물리 기반 포물선 궤적을 디버그 라인으로 그립니다.
	 *
	 * 운동 방정식:
	 *   P(t) = Start + V0 * t + 0.5 * g * t^2
	 *
	 * 특징:
	 * - SolveV0ForProjectile()로 계산된 V0를 입력받아, 실제 물리 포물선을 시각화
	 * - 중력 가속도(GravityZ)를 직접 지정 가능
	 * - Segment 수와 StepTime을 통해 해상도 조절
	 *
	 * 매개변수:
	 * - World      : UWorld 포인터 (필수, 없으면 반환)
	 * - Start      : 시작 좌표
	 * - V0         : 초기 속도 (SolveV0ForProjectile로 얻을 수 있음)
	 * - GravityZ   : 중력 값 (언리얼 기본: -980.f)
	 * - NumSegments: 선분 개수 (값이 클수록 매끄러움)
	 * - StepTime   : 시뮬레이션 시간 단위 (초 단위, Segments와 Duration 균형 필요)
	 * - LifeTime   : 디버그 라인 유지 시간 (초 단위)
	 * - Color      : 라인 색상
	 */
	static void DrawProjectileArcDebug(
		UWorld* World,
		const FVector& Start,
		const FVector& V0,
		float GravityZ = -980.f,
		int32 NumSegments = 20,
		float StepTime = 0.1f,
		float LifeTime = 2.0f,
		FColor Color = FColor::Red)
	{
		if (!World) return;

		const FVector g(0, 0, GravityZ);

		FVector PrevPos = Start;
		for (int32 i = 1; i <= NumSegments; ++i)
		{
			float t = StepTime * i;
			FVector CurrPos = Start + V0 * t + 0.5f * g * t * t;

			DrawDebugLine(World, PrevPos, CurrPos, Color, false, LifeTime, 0, 1.5f);
			PrevPos = CurrPos;
		}
	}
	
	/**
	 * @brief 두 점(Start → End)을 잇는 곡선을 디버그 라인으로 그립니다. (보간 기반 아치)
	 *
	 * 원리:
	 *   - 구간을 NumSegments로 나누어 일정 간격의 α 값을 계산
	 *   - 각 α에서 CalcParabola(Start, End, Height, α)로 좌표를 얻음
	 *   - 이전 점(PrevPos)과 현재 점(CurrPos)을 연결하는 선분을 DrawDebugLine으로 그림
	 *
	 * 특징:
	 * - 단순히 디버깅/시각화를 위한 함수 (실제 렌더링 아님)
	 * - CalcParabola() 구현체에 따라 사인 아치, 베지에 아치 등 원하는 형태 표시 가능
	 *
	 * 매개변수:
	 * - World      : UWorld 포인터 (필수, 없으면 반환)
	 * - Start/End  : 시작/끝 좌표
	 * - Height     : 아치 높이
	 * - NumSegments: 곡선을 나눌 선분 개수 (값이 클수록 곡선이 부드러움)
	 * - LifeTime   : 디버그 라인이 화면에 유지되는 시간 (초 단위)
	 * - Color      : 라인 색상
	 */
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
			FVector CurrPos = InterpArcSin(Start, End, Height, Alpha);

			DrawDebugLine(World, PrevPos, CurrPos, Color, false, LifeTime, 0, 1.5f);
			PrevPos = CurrPos;
		}
	}
};
