#include "PCH.h"
#include "Util/General.h"
#include "SaveLoadListener.h"
#include "Animation/GraphManager.h"
#include "Animation/Face/Manager.h"
#include "Papyrus/EventManager.h"
#include <memory>

namespace Tasks::SaveLoadListener
{
	// Forward declaration hook function
	static void* DropAllRunningDataHook(RE::BSScript::Internal::VirtualMachine* a_this);

	// Lazy-initialized hook pointer
	static std::unique_ptr<Util::VFuncHook<void*(RE::BSScript::Internal::VirtualMachine*)>> g_revertHook;
	static std::atomic<bool> g_hookInstalled{ false };

	// Hook function
	static void* DropAllRunningDataHook(RE::BSScript::Internal::VirtualMachine* a_this)
	{
		SAF_LOG_INFO("SaveLoadListener: Game loading/reverting - setting save-load flag and resetting managers");

		// Ustaw flagę aby GraphUpdateHook pomijał logikę SAF
		Animation::GraphManager::GetSingleton()->SetSaveLoadInProgress(true);

		try {
			Animation::GraphManager::GetSingleton()->Reset();
			Animation::Face::Manager::GetSingleton()->Reset();
			Papyrus::EventManager::GetSingleton()->Reset();
		} catch (...) {
			SAF_LOG_ERROR("SaveLoadListener: Exception during reset");
		}

		// Call original
		void* result = nullptr;
		if (g_revertHook) {
			result = (*g_revertHook)(a_this);
		}

		// NIE resetuj flagi tutaj - zrobi to OnPostLoadGame / case 6
		return result;
	}

	// Stub functions - nie używamy już g_pendingRebind
	bool HasPendingRebind()
	{
		return false;
	}

	bool ConsumePendingRebind()
	{
		return false;
	}

	void Install()
	{
		if (g_hookInstalled.load()) {
			return;
		}

		try {
			g_revertHook = std::make_unique<Util::VFuncHook<void*(RE::BSScript::Internal::VirtualMachine*)>>(
				481117,
				0x7,
				"BSScript::Internal::VirtualMachine::DropAllRunningData",
				&DropAllRunningDataHook
			);
			g_hookInstalled.store(true);
			SAF_LOG_INFO("SaveLoadListener: DropAllRunningData hook registered (ID 481117, slot 0x7)");
		} catch (const std::exception& e) {
			SAF_LOG_ERROR("SaveLoadListener: Failed to install hook - {}", e.what());
		} catch (...) {
			SAF_LOG_ERROR("SaveLoadListener: Failed to install hook - unknown exception");
		}
	}
}
