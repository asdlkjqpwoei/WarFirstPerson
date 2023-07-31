// Personal Copyright
#include "Team/WarTeamDefinition.h"
#include "Components/MeshComponent.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Team/WarTeamManagerSubsystem.h"

void UWarTeamDefinition::SetTeamID(int32 NewTeamID)
{
	TeamID = NewTeamID;
}

int32 UWarTeamDefinition::GetTeamID() const
{
	return TeamID;
}

void UWarTeamDefinition::ApplyToMaterial(UMaterialInstanceDynamic* MaterialInstanceDynamic)
{
	if (MaterialInstanceDynamic)
	{
		for (const TTuple<FName, float>& Scalar : Scalars)
		{
			MaterialInstanceDynamic->SetScalarParameterValue(Scalar.Key, Scalar.Value);
		}
		for (const TTuple<FName, FLinearColor>& Color : Colors)
		{
			MaterialInstanceDynamic->SetVectorParameterValue(Color.Key, Color.Value);
		}
		for (const TTuple<FName, TObjectPtr<UTexture>>& Texture : Textures)
		{
			MaterialInstanceDynamic->SetTextureParameterValue(Texture.Key, Texture.Value);
		}
	}
}

void UWarTeamDefinition::ApplyToMeshComponent(UMeshComponent* MeshComponent)
{
	if (MeshComponent)
	{
		for (const TTuple<FName, float>& Scalar : Scalars)
		{
			MeshComponent->SetScalarParameterValueOnMaterials(Scalar.Key, Scalar.Value);
		}
		for (const TTuple<FName, FLinearColor>& Color : Colors)
		{
			MeshComponent->SetVectorParameterValueOnMaterials(Color.Key, FVector(Color.Value));
		}
		const TArray<UMaterialInterface*> MaterialInterfaces = MeshComponent->GetMaterials();
		for (int32 MaterialIndex = 0; MaterialIndex < MaterialInterfaces.Num(); MaterialIndex++)
		{
			if (UMaterialInterface* MaterialInterface = MaterialInterfaces[MaterialIndex])
			{
				UMaterialInstanceDynamic* DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(MaterialInterface);
				if (!DynamicMaterialInstance)
				{
					DynamicMaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
				}
				for (const TTuple<FName, TObjectPtr<UTexture>>& Texture : Textures)
				{
					DynamicMaterialInstance->SetTextureParameterValue(Texture.Key, Texture.Value);
				}
			}
		}
	}
}

void UWarTeamDefinition::ApplyToNiagaraComponent(UNiagaraComponent* NiagaraComponent)
{
	if (NiagaraComponent)
	{
		for (const TTuple<FName, float>& Scalar : Scalars)
		{
			NiagaraComponent->SetVariableFloat(Scalar.Key, Scalar.Value);
		}
		for (const TTuple<FName, FLinearColor>& Color : Colors)
		{
			NiagaraComponent->SetVariableLinearColor(Color.Key, Color.Value);
		}
		for (const TTuple<FName, TObjectPtr<UTexture>>& Texture : Textures)
		{
			NiagaraComponent->SetVariableTexture(Texture.Key, Texture.Value);
		}
	}
}

void UWarTeamDefinition::ApplyToActor(AActor* Actor, bool bIncludeChildActors)
{
	if (Actor)
	{
		Actor->ForEachComponent(bIncludeChildActors, [=](UActorComponent* ActorComponent)
		{
			if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(ActorComponent))
			{
				ApplyToMeshComponent(MeshComponent);
			}
			else if (UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(ActorComponent))
			{
				ApplyToNiagaraComponent(NiagaraComponent);
			}
		});
	}
}
