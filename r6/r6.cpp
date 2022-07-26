// Others
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "../protection/lazy_importer.hpp"
#include <thread>

// Sdk Instances
#include "Managers/R6GameManager.hpp"
#include "Managers/R6ProfileManager.hpp"
#include "Managers/R6RoundManager.hpp"




inline GameManager* gameManager = nullptr;
inline ProfileManager* profileManager = nullptr;
inline RoundManager* roundManager = nullptr;

namespace settings {
	bool wallhack = false;
	bool norecoil = false;
	bool nospread = false;
}

void unlockAll();

void hotkeys() {
	while (true) {
		if (LI_FN(GetAsyncKeyState).safe_cached()(VK_F6) & 1) {
			settings::wallhack = !settings::wallhack;
		}
		if (LI_FN(GetAsyncKeyState).safe_cached()(VK_F7) & 1) {
			settings::norecoil = !settings::norecoil;
		}
		if (LI_FN(GetAsyncKeyState).safe_cached()(VK_F8) & 1) {
			settings::nospread = !settings::nospread;
		}
		if (LI_FN(GetAsyncKeyState).safe_cached()(VK_F9) & 1) {
			unlockAll();
		}
		LI_FN(Sleep).safe_cached()(1);
	}
}

inline uint32_t originalSpread1;
inline uint32_t originalSpread2;

inline uint32_t originalRecoil;

void loop()
{
	//printf("before roundManager->isPlaying()\n");
	if (!roundManager->isPlaying()) return;
	//printf("after roundManager->isPlaying()\n");

	const auto localPlayer = profileManager->localController();
	const auto localPawn = localPlayer->pawn();
	const auto localActor = localPawn->actor();
	const auto currentWeapon = localActor->currentWeapon();

	//printf("localPlayer: %llx\n", (uintptr_t)localPlayer);
	//printf("localPawn: %llx\n", (uintptr_t)localPawn);
	//printf("localActor: %llx\n", (uintptr_t)localActor);
	//printf("currentWeapon: %llx\n", (uintptr_t)currentWeapon);

	if (settings::norecoil) {
		if (currentWeapon) {
			if (!originalRecoil)
				originalRecoil = rpm<uint32_t>((uintptr_t)currentWeapon + 0x1A8);

			currentWeapon->setRecoil(0.f);
		}
	}
	else {
		if (currentWeapon) {
			if (originalRecoil) {
				wpm<uint32_t>((uintptr_t)currentWeapon + 0x1A8, originalRecoil);
			}
		}
	}

	if (settings::nospread) {
		if (currentWeapon) {
			if (!originalSpread1 || !originalSpread2) {
				originalSpread1 = rpm<uint32_t>((uintptr_t)currentWeapon + 0x44);
				originalSpread2 = rpm<uint32_t>((uintptr_t)currentWeapon + 0x64);
			}

			currentWeapon->setSpread(0.f);
		}
	}
	else {
		if (currentWeapon) {
			if (originalSpread1 && originalSpread2) {
				wpm<uint32_t>((uintptr_t)currentWeapon + 0x44, originalSpread1);
				wpm<uint32_t>((uintptr_t)currentWeapon + 0x64, originalSpread2);
			}
		}
	}

	const auto localreplication = localPlayer->playerReplicationInfo();

	for (int i = 0; i < gameManager->controllerCount(); i++)
	{
		const auto player = rpm<PlayerController*>((uintptr_t)gameManager->controllerList() + (i * sizeof(void*)));
		if (!player || player == localPlayer) continue;
		//printf("(%i) player: %llx\n", (uintptr_t)player);

		const auto replication = player->playerReplicationInfo();
		if (replication->teamId() == localreplication->teamId()) continue;

		const auto pawn = player->pawn();
		if (!pawn) continue;
		//printf("(%i) pawn: %llx\n", (uintptr_t)pawn);

		const auto actor = pawn->actor();
		if (!actor) continue;
		//printf("(%i) actor: %llx\n", (uintptr_t)actor);

		const auto component = _rotl64(rpm<uint64_t>((uintptr_t)actor + 0x1C8) ^ 0xAE43C62F812A63B0ui64, 41) - 0x5E838FF784FCE781i64;
		if (!component) continue;
		//printf("(%i) component: %llx\n", (uintptr_t)component);

		// 0x748000 -> disabled
		// 0x20748000 -> enabled

		if (settings::wallhack)
			wpm<uint32_t>(component + 0xB0, 0x20748000);
		else
			wpm<uint32_t>(component + 0xB0, 0x748000);
	}
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
{
	driver->ProcessId = procId;
	return driver->GetModuleBase(xorstr_(L"RainbowSix.exe"));
}

unsigned char UnlockAllShellcode[59] = {
	0x65, 0x48, 0x8B, 0x04, 0x25, 0x60, 0x00, 0x00, 0x00,
	0x4C, 0x8B, 0x40, 0x10,
	0x48, 0x8B, 0x04, 0x24, // 17
	0x49, 0x8D, 0x90, 0xB6, 0x7B, 0x0A, 0x02,
	0x48, 0x3B, 0xC2,
	0x75, 0x11, // 28
	0x49, 0x8D, 0x80, 0x90, 0x7D, 0x0A, 0x02,
	0x48, 0x89, 0x04, 0x24,
	0xB8, 0x01, 0x00, 0x00, 0x00, // mov     eax, 1
	0xC3,
	0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x48, 0xFF, 0xE0
};

static void vmt_hook(std::uintptr_t address, std::uintptr_t function) {
	DWORD old_protect{};
	driver->ProtectMemory((LPVOID)address, PAGE_EXECUTE_READWRITE, 8);
	wpm<uintptr_t>(address, function);
	driver->ProtectMemory((LPVOID)address, PAGE_EXECUTE_READ, 8);
}

void unlockAll() {

	uintptr_t pref_code_cave{};
	driver->ScanCodeCave(&pref_code_cave);

	DWORD old_protect{};
	driver->ProtectMemory((LPVOID)pref_code_cave, PAGE_EXECUTE_READWRITE, 8);

	*(unsigned long long*)(&UnlockAllShellcode[48]) = (g_moduleBase + 0x104EA00);

	for (int i = 0; i < sizeof(UnlockAllShellcode); i++) {
		wpm<uintptr_t>(pref_code_cave + i, UnlockAllShellcode[i]);
	}

	std::uintptr_t VTable = rpm<uintptr_t>(g_moduleBase + 0x75BCEB0);
	if (!VTable) return;

	vmt_hook(rpm<uintptr_t>(VTable) + 0x100, (uintptr_t)pref_code_cave);
	driver->ProtectMemory((LPVOID)pref_code_cave, PAGE_EXECUTE_READ, 8);
}

void r6()
{
	LI_FN(printf).safe_cached()(xorstr_(" (!) im going to hide now, enjoy :)\n"));
	LI_FN(Sleep).safe_cached()(2250);

	std::thread(hotkeys).detach();
	LI_FN(system).safe_cached()(xorstr_("cls"));

	g_moduleBase = GetModuleBaseAddress(driver->ProcessId, xorstr_("RainbowSix.exe"));
	if (!g_moduleBase) {
		LI_FN(printf).safe_cached()(xorstr_(" (-) error getting information, please create a ticket\n"));
		LI_FN(Sleep).safe_cached()(2250);
		*(void**)(0) = 0;
	}

	gameManager = GameManager::global();
	profileManager = ProfileManager::global();
	roundManager = RoundManager::global();

	while (true) {
		loop();
		LI_FN(Sleep).safe_cached()(1);
	}
}