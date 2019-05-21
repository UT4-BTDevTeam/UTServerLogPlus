#include "UTServerLogPlus.h"

#include "UnrealTournament.h"

AUTServerLogPlus::AUTServerLogPlus(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AUTServerLogPlus::PostPlayerInit_Implementation(AController* C)
{
	Super::PostPlayerInit_Implementation(C);

	// ignore bots
	if ( IsValid(C) && C->IsA(AUTPlayerController::StaticClass()) )
	{
		AUTPlayerState* PS = Cast<AUTPlayerState>(C->PlayerState);
		if (PS != nullptr)
		{
			UE_LOG(UTServerLogPlus, Log, TEXT("[player-join] %s (%s) (%s) (Spec=%u)"), *PS->PlayerName, *PS->UniqueId.ToString(), *PS->SavedNetworkAddress, PS->bOnlySpectator ? 1 : 0);
		}
	}
}

void AUTServerLogPlus::NotifyLogout_Implementation(AController* C)
{
	Super::NotifyLogout_Implementation(C);

	// ignore bots
	if ( IsValid(C) && C->IsA(AUTPlayerController::StaticClass()) )
	{
		AUTPlayerState* PS = Cast<AUTPlayerState>(C->PlayerState);
		if ( PS != nullptr )
		{
			UE_LOG(UTServerLogPlus, Log, TEXT("[player-left] %s (%s) (%s)"), *PS->PlayerName, *PS->UniqueId.ToString(), *PS->SavedNetworkAddress);
		}
	}
}

bool AUTServerLogPlus::AllowTextMessage_Implementation(FString& Msg, bool bIsTeamMessage, AUTBasePlayerController* Sender)
{
	if (!Super::AllowTextMessage_Implementation(Msg, bIsTeamMessage, Sender))
		return false;

	if (IsValid(Sender) && IsValid(Sender->PlayerState))
	{
		if (Sender->PlayerState->bOnlySpectator)
		{
			UE_LOG(UTServerLogPlus, Log, TEXT("[chat] (spec) %s: %s"), *Sender->PlayerState->PlayerName, *Msg.Trim());
		}
		else if (bIsTeamMessage)
		{
			UE_LOG(UTServerLogPlus, Log, TEXT("[chat] (team) %s: %s"), *Sender->PlayerState->PlayerName, *Msg.Trim());
		}
		else
		{
			UE_LOG(UTServerLogPlus, Log, TEXT("[chat] (all) %s: %s"), *Sender->PlayerState->PlayerName, *Msg.Trim());
		}
	}
	else
	{
		UE_LOG(UTServerLogPlus, Log, TEXT("[chat] (system) %s"), *Msg.Trim());
	}

	return true;
}
