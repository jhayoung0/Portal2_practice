// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Actor/AListActorManager.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Engine/Selection.h"
#include "ScopedTransaction.h"
#include "Algo/Unique.h"
#endif

#if WITH_EDITOR
int32 AListActorManager::GatherSelectedItem(TArray<AActor*>& Out) const
{
	Out.Reset();
	if (!GEditor)
		return 0;

	TArray<AActor*> Selected;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(Selected);

	for (AActor* A : Selected)
	{
		if (!IsValid(A))
			continue;

		if (ClassFilter && !A->IsA(ClassFilter))
			continue;
		Out.Add(A);
	}

	// 일관성: 이름 정렬 + 중복 제거
	Out.Sort([](const AActor& L, const AActor& R){
		return L.GetFName().LexicalLess(R.GetFName());
	});

	Out.SetNum(Algo::Unique(Out));

	return Out.Num();
}

void AListActorManager::AssignInternal(const bool bAppend)
{
	TArray<AActor*> Picked;
	const int32 Count = GatherSelectedItem(Picked);
	if (Count <= 0)
		return;

	const FScopedTransaction Tx(NSLOCTEXT("ListActorManager", "AssignFromSelection", "Assign Items From Selection"));
	Modify();

	if (!bAppend)
	{
		ArrayActors.Reset();
	}

	for (AActor* A : Picked)
	{
		if (!IsValid(A))
			continue;
		
		ArrayActors.AddUnique(A);
	}

	if (UPackage* Pkg = GetOutermost())
	{
		Pkg->SetDirtyFlag(true);
	}
}
#endif // WITH_EDITOR

void AListActorManager::AssignItemReplace()
{
#if WITH_EDITOR
	AssignInternal(false);
#endif
}

void AListActorManager::AssignItemAppend()
{
#if WITH_EDITOR
	AssignInternal(true);
#endif
}

#if WITH_EDITOR
void AListActorManager::SortByNameAsc()
{
	const FScopedTransaction Tx(
		NSLOCTEXT("ListActorManager", "SortByNameAsc", "Sort ArrayActors by Name Asc")
	);
	Modify();

	ArrayActors.Sort([](const AActor& L, const AActor& R) {
		return L.GetActorLabel() < R.GetActorLabel();
	});

	if (UPackage* Pkg = GetOutermost())
	{
		Pkg->SetDirtyFlag(true);
	}
}

void AListActorManager::SortByNameDesc()
{
	const FScopedTransaction Tx(
		NSLOCTEXT("ListActorManager", "SortByNameDesc", "Sort ArrayActors by Name Desc")
	);
	Modify();

	ArrayActors.Sort([](const AActor& L, const AActor& R) {
		return L.GetActorLabel() > R.GetActorLabel();
	});
	
	if (UPackage* Pkg = GetOutermost())
	{
		Pkg->SetDirtyFlag(true);
	}
}
#endif