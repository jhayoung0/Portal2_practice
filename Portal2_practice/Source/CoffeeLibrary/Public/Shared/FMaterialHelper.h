#pragma once

struct FMaterialHelper
{
	static FLinearColor GetVectorParameterValueSafe(
		UMaterialInstanceDynamic* MaterialInstance, 
		const FName& ParamName)
	{
		if (MaterialInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetVectorParameterValueSafe: MaterialInstance is null"));
			return FLinearColor::Black;
		}
		return MaterialInstance->K2_GetVectorParameterValue(ParamName);
	}
};