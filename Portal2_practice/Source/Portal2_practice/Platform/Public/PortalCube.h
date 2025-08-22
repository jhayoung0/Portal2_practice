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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FLinearColor ActivateColor = FLinearColor::Yellow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FLinearColor DeactivateColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	FName ColorParam = FName("EmissiveFactor");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	class UStaticMeshComponent* SymbolMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cube")
	class UMaterialInstanceDynamic* SymbolMaterial;
};
