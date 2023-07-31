// Personal Copyright
#pragma once
#include "GenericTeamAgentInterface.h"
#include "WarTeamAgentInterface.generated.h"

UINTERFACE(meta=(CannotImplementInterfaceInBlueprint))
class UWarTeamAgentInterface : public UGenericTeamAgentInterface
{
	GENERATED_UINTERFACE_BODY()
};

class WARFIRSTPERSON_API IWarTeamAgentInterface : public IGenericTeamAgentInterface
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual int32 GenericTeamIdToInteger(const FGenericTeamId& GenericTeamID) const;
	virtual FGenericTeamId IntegerToGenericTeamId(const int32& IntegerID) const;
};