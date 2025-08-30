// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AElevator.generated.h"

UENUM(BlueprintType)
enum class EElevatorMoveDirectionType : uint8
{
	Start_To_End           UMETA(DisplayName="Start To End"),
	End_To_Start				UMETA(DisplayName="End To Start")
};


UCLASS()
class PORTAL2_PRACTICE_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	AElevator();

protected:
	virtual void BeginPlay() override;

	void FindWall();
	void FindStartEndPos();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Elevator")
	FORCEINLINE void SetElevatorDirection( const EElevatorMoveDirectionType InMoveDirection)
	{
		this->MoveDirection = InMoveDirection;
	}

	UFUNCTION(BlueprintCallable, Category="Elevator")
	FORCEINLINE void SetMoveState( const bool InMoveState )
	{
		this->MoveState = InMoveState;
		this->SetWallCollision(MoveState);
	}

	UFUNCTION(BlueprintCallable, Category="Elevator")
	FORCEINLINE bool IsSameGroup(int32 InGroup) const
	{
		return Group == InGroup;
	}

	UFUNCTION(CallInEditor, Category="Elevator", meta=(DevelopmentOnly, DisplayName="Refresh"))
	void Refresh()
	{
		this->SetElevatorDirection(MoveDirection);
		this->SetMoveState(MoveState);
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cube")
	void MoveDone();

	UFUNCTION( BlueprintCallable, Category="Elevator")
	void SetWallCollision(const bool State);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Elevator")
	int32 Group;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Elevator")
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Elevator")
	bool MoveState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Elevator")
	EElevatorMoveDirectionType MoveDirection = EElevatorMoveDirectionType::Start_To_End;

private:
	UPROPERTY(VisibleAnywhere, Category="Elevator")
	class AActor* StartPos;

	UPROPERTY(VisibleAnywhere, Category="Elevator")
	class AActor* EndPos;

	UPROPERTY(VisibleAnywhere, Category="Elevator");
	class TArray<UPrimitiveComponent*> WallArray;

	UPROPERTY(VisibleAnywhere, Category="Elevator");
	UPrimitiveComponent* ElevatorCollision;
};
