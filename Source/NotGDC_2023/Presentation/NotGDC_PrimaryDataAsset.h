// Copyright 2023 Nicholas Helish. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NotGDC_PrimaryDataAsset.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNotGDC_PrimaryDataAsset, Log, All);

/**
 * 
 */
UCLASS(ClassGroup = "NotGDC")
class NOTGDC_2023_API UNotGDC_PrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

#if WITH_EDITORONLY_DATA

	// UPrimaryDataAsset interface
	virtual void UpdateAssetBundleData() override;
	// ~UPrimaryDataAsset interface

	virtual void UpdateBundleDataWithInstancedObjectsMetaData(FAssetBundleData& BundleData) { }

	void CollectSoftPathsFromBlueprints(const UObject* Object, const FName& BundleName, FAssetBundleData& BundleData)
	{
		CollectSoftPathsFromBlueprints(Object->GetClass(), Object, BundleData, BundleName, Object->GetFName());
	}

	void CollectSoftPathsFromBlueprints(const UStruct* Struct, const void* StructValue, FAssetBundleData& BundleData, const FName& BundleName, FName DebugName = NAME_None)
	{
		TSet<const void*> ALlVisitedStructValues;
		CollectSoftPathsFromBlueprints_Recursive(Struct, StructValue, BundleData, BundleName, DebugName, ALlVisitedStructValues);
	}

#if WITH_EDITOR

	virtual void CollectSoftPathsFromBlueprints_Recursive(const UStruct* Struct, const void* StructValue, FAssetBundleData& AssetBundle, const FName& BundleName, FName DebugName, TSet<const void*>& AllVisitedStructValues) const;

#endif

#endif
};
