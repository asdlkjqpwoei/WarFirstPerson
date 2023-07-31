// Personal Copyright
#pragma once
#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"

class UClass;

class FWarContextEffectsLibraryAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
};
