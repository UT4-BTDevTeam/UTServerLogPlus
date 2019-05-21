
#include "UTServerLogPlus.h"

#include "ModuleManager.h"
#include "ModuleInterface.h"

#include "UnrealTournament.h"

class FUTServerLogPlusPlugin : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FDelegateHandle OnActorSpawnedDelegateHandle;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);
	void OnActorSpawned(AActor* InActor);
};

IMPLEMENT_MODULE(FUTServerLogPlusPlugin, UTServerLogPlus)


//================================================
// Startup
//================================================

void FUTServerLogPlusPlugin::StartupModule()
{
	UE_LOG(LogLoad, Log, TEXT("[UTServerLogPlus] StartupModule"));

	// Log PID once
	UE_LOG(UTServerLogPlus, Log, TEXT("[pid] %u"), FPlatformProcess::GetCurrentProcessId());

	// Wait for World
	auto deleg = FWorldDelegates::FWorldInitializationEvent::FDelegate::CreateRaw(this, &FUTServerLogPlusPlugin::OnPostWorldInitialization);
	FWorldDelegates::OnPostWorldInitialization.Add(deleg);
}


//================================================
// World
//================================================

void FUTServerLogPlusPlugin::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	UE_LOG(UTServerLogPlus, DEBUGLOG, TEXT("[D] OnPostWorldInitialization"));
	if (World != nullptr)
	{
		UE_LOG(UTServerLogPlus, DEBUGLOG, TEXT("[D] World: %s"), *World->GetName());

		// Wait for Gamemode
		auto deleg = FOnActorSpawned::FDelegate::CreateRaw(this, &FUTServerLogPlusPlugin::OnActorSpawned);
		OnActorSpawnedDelegateHandle = World->AddOnActorSpawnedHandler(deleg);
	}
	else
	{
		UE_LOG(UTServerLogPlus, Warning, TEXT("World is null"));
	}
}

void FUTServerLogPlusPlugin::OnActorSpawned(AActor* InActor)
{
	UE_LOG(UTServerLogPlus, DEBUGLOG, TEXT("[D] OnActorSpawned: %s"), *InActor->GetName());

	AGameMode* Gamemode = Cast<AGameMode>(InActor);
	if ( Gamemode != nullptr )
	{
		UE_LOG(UTServerLogPlus, DEBUGLOG, TEXT("[D] Gamemode: %s"), *Gamemode->GetName());

		// Stop waiting
		InActor->GetWorld()->RemoveOnActorSpawnedHandler(OnActorSpawnedDelegateHandle);

		AUTGameMode* UTGamemode = Cast<AUTGameMode>(Gamemode);
		if (UTGamemode != nullptr)
		{
			UE_LOG(UTServerLogPlus, DEBUGLOG, TEXT("[D] Gamemode is a UTGamemode!"));

			// Add mutator
			// AddMutatorClass doesn't work here (too early?)
			// We can use BuiltInMutators or ConfigMutators to let gamemode load it appropriately
			UTGamemode->BuiltInMutators.Add(AUTServerLogPlus::StaticClass());
		}
		else
		{
			UE_LOG(UTServerLogPlus, Warning, TEXT("Gamemode is not a UTGamemode (%s)"), *Gamemode->GetName());
		}
	}
}


//================================================
// Shutdown
//================================================

void FUTServerLogPlusPlugin::ShutdownModule()
{
	UE_LOG(LogLoad, Log, TEXT("[UTServerLogPlus] ShutdownModule"));
}
