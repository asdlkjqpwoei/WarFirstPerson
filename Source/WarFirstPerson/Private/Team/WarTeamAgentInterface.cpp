// Personal Copyright
#include "Team/WarTeamAgentInterface.h"
#include "WarFirstPerson/WarLogChannels.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WarTeamAgentInterface)

UWarTeamAgentInterface::UWarTeamAgentInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

int32 IWarTeamAgentInterface::GenericTeamIdToInteger(const FGenericTeamId& GenericTeamID) const
{
	return (GenericTeamID == FGenericTeamId::NoTeam) ? INDEX_NONE : (int32)GenericTeamID;
}

FGenericTeamId IWarTeamAgentInterface::IntegerToGenericTeamId(const int32& IntegerID) const
{
	return (IntegerID == INDEX_NONE) ? FGenericTeamId::NoTeam : FGenericTeamId((uint8)IntegerID);
}
