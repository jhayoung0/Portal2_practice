// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalCube.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API APortalCube : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalCube();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cube")
	void ActivateCube();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cube")
	void DeactivateCube();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cube")
	void ReturnPool();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Cube")
	void StartDisolve();
	
private:
	void UpdateDissolve();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FLinearColor ActivateColor = FLinearColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FLinearColor DeactivateColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FName ColorParam = FName("EmissiveFactor");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FName DisolveParam = FName("Disolve");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UStaticMeshComponent* CoverMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UMaterialInstanceDynamic* CoverMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UStaticMeshComponent* SymbolMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	class UMaterialInstanceDynamic* SymbolMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	float DissolveDuration = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	float DissolveBegin = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	float DissolveEnd = 1.0f;
	
private:
	FTimerHandle DissolveTimerHandle;
	float DissolveElapsed = 0.0f;
};
