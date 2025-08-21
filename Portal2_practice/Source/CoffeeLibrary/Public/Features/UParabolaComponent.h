// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Shared/FMathHelper.h"
#include "Components/ActorComponent.h"
#include "UParabolaComponent.generated.h"



enum class EParabolaType : uint8
{
	// 초기속도·중력 사용하는 물리식임이 즉시 읽힘.
	Ballistic		UMETA(DisplayName="Physics (Ballistic)",		ToolTip="Start + V0*t + 0.5*g*t^2"),
	// 도착점 고정 + 정점높이로 기하적 아치 생성.
	Geometric		UMETA(DisplayName="Geometric (Apex Height)",	ToolTip="Lerp(Start,Target,a) + UpAxis * 4*H*a*(1-a)")
};

UENUM(BlueprintType)
enum class EForwardAxis : uint8 { X, Y, Z };

USTRUCT(BlueprintType)
struct FParabolaBallisticTrack
{
	GENERATED_BODY()

	// 입력 파라미터
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Start = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.001")) float Duration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GravityZ = -980.f; // UE 기본 cm/s^2
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Direction = FVector(1,0,0); // 자동 정규화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0")) float Power = 1200.f; // cm/s

	// 평면 고정(선택)
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bLockZ = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bLockZ")) float LockedZ = 0.f;

	// 런타임 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient) float CurrentTime = 0.f;

	// 타임 유틸
	FORCEINLINE void ResetTime() { CurrentTime = 0.f; }
	FORCEINLINE float GetAlpha() const { return FMath::Clamp(CurrentTime / FMath::Max(0.001f, Duration), 0.f, 1.f); }
	FORCEINLINE void Advance(float DeltaTime)
	{
		CurrentTime = FMath::Min(CurrentTime + FMath::Max(0.f, DeltaTime), FMath::Max(0.001f, Duration));
	}

	// 속도/평가
	FORCEINLINE FVector GetInitialVelocity() const
	{
		return Direction.GetSafeNormal() * Power;
	}

	FVector EvaluateAtTime(const AActor* /*Owner*/, float TimeSec) const
	{
		const float t = FMath::Clamp(TimeSec, 0.f, FMath::Max(0.001f, Duration));
		const FVector g(0,0,GravityZ);
		const FVector V0 = GetInitialVelocity();

		FVector P = Start + V0 * t + 0.5f * g * t * t;
		if (bLockZ)
			P.Z = LockedZ;
		return P;
	}

	FORCEINLINE FVector EvaluateAtAlpha(const AActor* Owner, float Alpha) const
	{
		return EvaluateAtTime(Owner, Alpha * FMath::Max(0.001f, Duration));
	}

	FORCEINLINE FVector EvaluateAtCurrent(const AActor* Owner) const
	{
		return EvaluateAtTime(Owner, CurrentTime);
	}

	// 목표점+시간으로 V0 역산 → Direction/Power 세팅
	static FVector SolveV0ForArc(const FVector& InStart, const FVector& InEnd, float t, float InGravityZ=-980.f)
	{
		return FMathHelper::SolveV0ForProjectile( InStart, InEnd, t, InGravityZ );
	}

	static void SplitVelocity(const FVector& V0, FVector& OutDir, float& OutPow)
	{
		OutPow = V0.Length();
		OutDir = (OutPow > SMALL_NUMBER) ? V0 / OutPow : FVector(1,0,0);
	}

	void ApplyArcSolution(const FVector& InStart, const FVector& InEnd, float t)
	{
		Start = InStart;
		Duration = FMath::Max(0.001f, t);
		const FVector V0 = SolveV0ForArc(InStart, InEnd, Duration, GravityZ);
		SplitVelocity(V0, Direction, Power);
		ResetTime();
	}
};

USTRUCT(BlueprintType)
struct FParabolaGeometricTrack
{
	GENERATED_BODY()

	// 입력 파라미터
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Start = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Target = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.001")) float Duration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0")) float ApexHeight = 200.f; // 정점에서 +H
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector UpAxis = FVector::UpVector;

	// 평면 고정(선택)
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bLockZ = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="bLockZ")) float LockedZ = 0.f;

	// 런타임 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient) float CurrentTime = 0.f;

	// 타임 유틸
	FORCEINLINE void ResetTime() { CurrentTime = 0.f; }
	FORCEINLINE float GetAlpha() const { return FMath::Clamp(CurrentTime / FMath::Max(0.001f, Duration), 0.f, 1.f); }
	FORCEINLINE void Advance(float DeltaTime)
	{
		CurrentTime = FMath::Min(CurrentTime + FMath::Max(0.f, DeltaTime), FMath::Max(0.001f, Duration));
	}

	// 평가(직선보간 + 4Ha(1-a) 버프)
	FVector EvaluateAtTime(const AActor* /*Owner*/, float TimeSec) const
	{
		const float D = FMath::Max(0.001f, Duration);
		const float t = FMath::Clamp(TimeSec, 0.f, D);
		const float a = t / D;

		return FMathHelper::InterpArcSin(Start, Target, ApexHeight, a);
	}

	FORCEINLINE FVector EvaluateAtAlpha(const AActor* Owner, float Alpha) const
	{
		return EvaluateAtTime(Owner, Alpha * FMath::Max(0.001f, Duration));
	}

	FORCEINLINE FVector EvaluateAtCurrent(const AActor* Owner) const
	{
		return EvaluateAtTime(Owner, CurrentTime);
	}

	void ApplyArcSolution(const FVector& InStart, const FVector& InTarget, const float InApexHeight, float t, const FVector& InUpAxis = FVector::UpVector)
	{
		Start = InStart;
		Target = InTarget;
		ApexHeight = FMath::Max(0.f, InApexHeight);
		UpAxis = InUpAxis;
		Duration = FMath::Max(0.001f, t);
		ResetTime();
	}
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COFFEELIBRARY_API UParabolaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UParabolaComponent();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateTracks(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Parabola")
	FRotator GetParabolaFacing(FName TrackName, bool bYawOnly, EForwardAxis ForwardAxis) const;

	UFUNCTION(BlueprintCallable, Category="Parabola")
	FRotator MakeFacingFromDir(const FVector& Direction, const bool bYawOnly, const EForwardAxis ForwardAxis) const;
	
	// --- Ballistic ---
	UFUNCTION(BlueprintCallable, Category="Parabola|Ballistic")
	void SetBallisticParabolaTrack(FName TrackName, const FParabolaBallisticTrack& Track);

	UFUNCTION(BlueprintPure, Category="Parabola|Ballistic")
	FVector GetBallisticParabolaVectorTrack(FName TrackName) const;

	UFUNCTION(BlueprintPure, Category="Parabola|Ballistic")
	FVector GetBallisticVectorAtAlphaFromTrack(FName TrackName, float Alpha) const;

	UFUNCTION(BlueprintCallable, Category="Parabola|Ballistic")
	void DrawBallisticPath(FName TrackName, int32 NumSegments, FColor Color, float LifeTime) const;

	// --- Geometric ---
	UFUNCTION(BlueprintCallable, Category="Parabola|Geometric")
	void SetGeometricParabolaTrack(FName TrackName, const FParabolaGeometricTrack& Track);

	UFUNCTION(BlueprintPure, Category="Parabola|Geometric")
	FVector GetGeometricParabolaVectorTrack(FName TrackName) const;

	UFUNCTION(BlueprintPure, Category="Parabola|Geometric")
	FVector GetGeometricVectorAtAlphaFromTrack(FName TrackName, float Alpha) const;

	UFUNCTION(BlueprintCallable, Category="Parabola|Geometric")
	void DrawGeometricPath(FName TrackName, int32 NumSegments, FColor Color, float LifeTime) const;

private:
	TMap<FName, FParabolaBallisticTrack> BallisticTracks;
	TMap<FName, FParabolaGeometricTrack> GeometricTracks;
};
