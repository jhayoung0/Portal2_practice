// Copyright (c) 2025 Doppleddiggong. All rights reserved.

#include "UCoffeeCommonUtil.h"
#include "Misc/DateTime.h"
//
// void UCoffeeCommonUtil::TestULog()
// {
// 	int32 Number = 100;
// 	float Pi = 3.14f;
// 	int64 LongValue = 10000;
// 	double DoubleValue = 100000;
// 	bool IsGood = false;
// 	FString MyName = "MyName";
// 	
// 	// --- ULOG 기능 테스트 ---
// 	UE_LOG( LogTemp,  Warning, TEXT("Hello World"));
// 	UE_LOG( LogTemp, Warning, TEXT("Number : %d"), Number);
// 	// 결과: "9,999" (en-US 기준)
// 	FText LocalizedNumber = FText::AsNumber(Number);
// 	UE_LOG( LogTemp, Warning, TEXT("#,##0 : %s"), *LocalizedNumber.ToString());
// 	UE_LOG( LogTemp, Warning, TEXT("pi : %f"), Pi);
// 	UE_LOG( LogTemp, Warning, TEXT("longValue : %lld"), LongValue);
// 	UE_LOG( LogTemp, Warning, TEXT("doubleValue : %f"), DoubleValue);
// 	UE_LOG( LogTemp, Warning, TEXT("isGood : %d"), IsGood);
// 	UE_LOG( LogTemp, Warning, TEXT("myName : %s"), *MyName);
// 	UE_LOG( LogTemp, Warning, TEXT("myName : %s"), TEXT("배주백"));
// }
//
// void UCoffeeCommonUtil::TestInBound()
// {
// 	// --- CoffeeLibrary 기능 테스트 ---
// 	UE_LOG(LogTemp, Warning, TEXT("--- Testing CoffeeLibrary::CommonUtil::InBounds ---"));
//
// 	// MyArrayCount는 'InBounds' 함수를 테스트하기 위한 가상의 배열 크기입니다.
// 	constexpr int32 MyArrayCount = 5;
// 	constexpr int32 TestIndex1 = 3;  // 유효한 인덱스
// 	constexpr int32 TestIndex2 = 5;  // 유효하지 않은 인덱스
//
// 	const bool bIsIndex1InBounds = InBounds(TestIndex1, MyArrayCount);
// 	UE_LOG(LogTemp, Warning, TEXT("Is index %d in bounds [0..%d)? -> %s"),
// 		TestIndex1, MyArrayCount, bIsIndex1InBounds ? TEXT("True") : TEXT("False"));
//
// 	const bool bIsIndex2InBounds = InBounds(TestIndex2, MyArrayCount);
// 	UE_LOG(LogTemp, Warning, TEXT("Is index %d in bounds [0..%d)? -> %s"),
// 		TestIndex2, MyArrayCount, bIsIndex2InBounds ? TEXT("True") : TEXT("False"));
// }

bool UCoffeeCommonUtil::InBounds(const int32 Index, const int32 Count)
{
	return (Index >= 0) && (Index < Count);
}

int64 UCoffeeCommonUtil::GetNowTimestamp()
{
	const auto DataTime = FDateTime::UtcNow();
	return DataTime.ToUnixTimestamp();
}

int32 UCoffeeCommonUtil::GetFirstNumberInActorLabel(const AActor* Actor)
{
	for (const auto& Char : Actor->GetActorLabel())
	{
		if (FChar::IsDigit(Char))
			return Char - '0';
	}
	return 0;
}