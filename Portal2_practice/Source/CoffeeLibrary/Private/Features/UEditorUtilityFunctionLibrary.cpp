// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Features/UEditorUtilityFunctionLibrary.h"
#include "Styling/SlateIconFinder.h"
#include "Styling/AppStyle.h"
#include "UObject/UObjectGlobals.h"
#include "ScopedTransaction.h"

static FString PadInt(int32 Value, int32 ZeroPad)
{
	FString S = FString::FromInt(Value);

	while (S.Len() < ZeroPad)
		S = TEXT("0") + S;
	return S;
}

FSlateBrush UEditorUtilityFunctionLibrary::GetClassIconBrush(const UClass* InClass, const bool bSmallIcon)
{
	FSlateBrush Result;

#if WITH_EDITOR
	if (InClass)
	{
		const FSlateIcon Icon = FSlateIconFinder::FindIconForClass(InClass);
		const FSlateBrush* Brush = bSmallIcon ? Icon.GetSmallIcon() : Icon.GetIcon();

		if (Brush)
		{
			Result = *Brush;
			if (Result.ImageSize.IsNearlyZero())
			{
				if ( bSmallIcon )
					Result.ImageSize = FVector2D(16.f,16.f);	
				else
					Result.ImageSize = FVector2D(32.f, 32.f);
			}
		}
	}
#endif

	return Result;
}

bool UEditorUtilityFunctionLibrary::RemoveActor(TArray<AActor*>& Array, AActor* Target)
{
	if (!IsValid(Target))
		return false;

	const int32 Idx = Array.IndexOfByKey(Target);
	if (Idx == INDEX_NONE)
		return false;
	
	Array.RemoveAt(Idx);
	return true;
}

bool UEditorUtilityFunctionLibrary::MoveActor(TArray<AActor*>& Array, AActor* Target, int32 Delta, int32& OutNewIndex)
{
	OutNewIndex = -1;
	if (!IsValid(Target) || Delta == 0)
		return false;

	const int32 From = Array.IndexOfByKey(Target);
	if (!Array.IsValidIndex(From))
		return false;

	const int32 To = From + Delta;
	if (!Array.IsValidIndex(To))
		return false;

	Array.Swap(From, To);
	OutNewIndex = To;
	return true;
}

void UEditorUtilityFunctionLibrary::ArrangeActorsLinear(TArray<AActor*>& Actors, FVector StepOffset, bool bKeepZ)
{
#if WITH_EDITOR
    Actors.RemoveAll([](AActor* A){ return !IsValid(A); });
    if (Actors.Num() == 0)
    	return;

	const FScopedTransaction Tx(FText::FromString(TEXT("Arrange Actors Linear")));
    const FVector Anchor = Actors[0]->GetActorLocation();

    for (int32 i=0; i<Actors.Num(); ++i)
    {
        AActor* A = Actors[i];
        if (!IsValid(A))
        	continue;
        A->Modify();

        FVector NewLoc = Anchor + StepOffset * i;
        if (bKeepZ)
        	NewLoc.Z = A->GetActorLocation().Z;

        A->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);
    }
#endif
}

void UEditorUtilityFunctionLibrary::ArrangeAndRename(TArray<AActor*>& Actors, FVector StepOffset,
    const FString& Prefix, int32 StartIndex, int32 ZeroPad, bool bKeepZ)
{
#if WITH_EDITOR
    Actors.RemoveAll([](AActor* A){ return !IsValid(A); });
    if (Actors.Num() == 0)
    	return;

	const FScopedTransaction Tx(FText::FromString(TEXT("Arrange & Rename Actors")));
    const FVector Anchor = Actors[0]->GetActorLocation();

    for (int32 i=0; i<Actors.Num(); ++i)
    {
        AActor* A = Actors[i];
        if (!IsValid(A))
        	continue;
        A->Modify();

        FVector NewLoc = Anchor + StepOffset * i;
        if (bKeepZ) NewLoc.Z = A->GetActorLocation().Z;
        A->SetActorLocation(NewLoc, false, nullptr, ETeleportType::TeleportPhysics);

        const FString Label = Prefix + PadInt(StartIndex + i, ZeroPad);
        A->SetActorLabel(Label, true); // bMarkDirty
    }
#endif
}

void UEditorUtilityFunctionLibrary::ArrangeByFirstForward(TArray<AActor*>& Actors, float Spacing, bool bKeepZ)
{
#if WITH_EDITOR
    Actors.RemoveAll([](AActor* A){ return !IsValid(A); });
    if (Actors.Num() == 0)
    	return;

    const FVector Forward = Actors[0]->GetActorForwardVector().GetSafeNormal();
    ArrangeActorsLinear(Actors, Forward * Spacing, bKeepZ);
#endif
}