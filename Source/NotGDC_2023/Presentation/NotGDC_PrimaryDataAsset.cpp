// Copyright 2023 Nicholas Helish. All rights reserved.


#include "NotGDC_PrimaryDataAsset.h"

#include "NotGDC_Logging.h"
#include "Engine/AssetManager.h"

DEFINE_LOG_CATEGORY(LogNotGDC_PrimaryDataAsset);

#if WITH_EDITORONLY_DATA

void UNotGDC_PrimaryDataAsset::UpdateAssetBundleData()
{
	if(!UAssetManager::IsValid())
	{
		return;
	}

	AssetBundleData.Reset();
	UpdateBundleDataWithInstancedObjectsMetaData(AssetBundleData);
	UAssetManager::Get().InitializeAssetBundlesFromMetadata(this, AssetBundleData);
}

#if WITH_EDITOR

void UNotGDC_PrimaryDataAsset::CollectSoftPathsFromBlueprints_Recursive(const UStruct* Struct, const void* StructValue,
	FAssetBundleData& AssetBundle, const FName& BundleName, FName DebugName, TSet<const void*>& AllVisitedStructValues) const
{
	if (!ensure(Struct && StructValue))
	{
		return;
	}

	if (AllVisitedStructValues.Contains(StructValue))
	{
		return;
	}

	AllVisitedStructValues.Add(StructValue);

	for (TPropertyValueIterator<const FProperty> It(Struct, StructValue); It; ++It)
	{
		const FProperty* Property = It.Key();
		const void* PropertyValue = It.Value();

		FSoftObjectPath FoundRef;
		if (const FSoftClassProperty* AssetClassProp = CastField<FSoftClassProperty>(Property))
		{
			const FSoftObjectPtr& AssetClassPtr = AssetClassProp->GetPropertyValue(PropertyValue);
			FoundRef = AssetClassPtr.ToSoftObjectPath();
		}
		else if (const FSoftObjectProperty* AssetProp = CastField<FSoftObjectProperty>(Property))
		{
			const FSoftObjectPtr& AssetClassPtr = AssetProp->GetPropertyValue(PropertyValue);
			FoundRef = AssetClassPtr.ToSoftObjectPath();
		}
		else if (const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			// SoftClassPath is binary identical with SoftObjectPath
			if (StructProperty->Struct == TBaseStructure<FSoftObjectPath>::Get() || StructProperty->Struct == TBaseStructure<FSoftClassPath>::Get())
			{
				if (const FSoftObjectPath* const AssetRefPtr = reinterpret_cast<const FSoftObjectPath*>(PropertyValue))
				{
					FoundRef = *AssetRefPtr;
				}
				// Skip recursion, we don't care about the raw string property
				It.SkipRecursiveProperty();
			}
		}
		else if (const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			if (ObjectProperty->PropertyFlags & CPF_InstancedReference)
			{
				const UObject* Object = ObjectProperty->GetObjectPropertyValue(PropertyValue);
				if (Object != nullptr)
				{
					CollectSoftPathsFromBlueprints_Recursive(Object->GetClass(), Object, AssetBundle, BundleName, Object->GetFName(), AllVisitedStructValues);
				}
			}
		}

		if (FoundRef.IsNull())
		{
			continue;
		}
		if(FoundRef.GetLongPackageName().IsEmpty())
		{
			LOG_NOTGDC(LogNotGDC_PrimaryDataAsset, Error, "Asset bundle reference with invalid package(%s) | Property(%s)", *DebugName.ToString(), *GetNameSafe(Property));
			continue;
		}

		AssetBundle.AddBundleAsset(BundleName, FoundRef.GetAssetPath());
	}
}

#endif

#endif

