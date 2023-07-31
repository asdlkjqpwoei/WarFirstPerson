// Personal Copyright
#pragma once
#include "Engine/DataAsset.h"
#include "WarTeamDefinition.generated.h"

class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class WARFIRSTPERSON_API UWarTeamDefinition : public UDataAsset
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "War|Team")
	void SetTeamID(int32 NewTeamID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "War|Team")
	int32 GetTeamID() const;

	UFUNCTION(BlueprintCallable, Category = "War|Team")
	void ApplyToMaterial(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	UFUNCTION(BlueprintCallable, Category = "War|Team")
	void ApplyToMeshComponent(UMeshComponent* MeshComponent);

	UFUNCTION(BlueprintCallable, Category = "War|Team")
	void ApplyToNiagaraComponent(UNiagaraComponent* NiagaraComponent);

	UFUNCTION(BlueprintCallable, Category = "War|Team")
	void ApplyToActor(AActor* Actor, bool bIncludeChildActors = true);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Team")
	TMap<FName, float> Scalars;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Team")
	TMap<FName, FLinearColor> Colors;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Team")
	TMap<FName, TObjectPtr<UTexture>> Textures;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Team")
	FText Name;

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "War|Team", Meta = (AllowPrivateAccess = "true"))
	int32 TeamID;
};
