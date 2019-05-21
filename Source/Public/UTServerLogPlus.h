#pragma once

#include "Core.h"
#include "Engine.h"
#include "UTMutator.h"

#include "UTServerLogPlus.generated.h"

DEFINE_LOG_CATEGORY_STATIC(UTServerLogPlus, Log, All);

#define DEBUGLOG Verbose
//#define DEBUGLOG Log

UCLASS()
class AUTServerLogPlus : public AUTMutator
{
	GENERATED_UCLASS_BODY()

	void PostPlayerInit_Implementation(AController* C) override;
	void NotifyLogout_Implementation(AController* C) override;

	bool AllowTextMessage_Implementation(FString& Msg, bool bIsTeamMessage, AUTBasePlayerController* Sender) override;
};
