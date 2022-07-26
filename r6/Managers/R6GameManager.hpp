#pragma once
#include "..\..\driver\driver.h"
#include "..\Controller\Controller.hpp"

struct R6GameManager {};

struct GameManager : R6GameManager
{
	static GameManager* global() {
		const auto unk = rpm<std::uintptr_t>(g_moduleBase + /*0x7C81658*/0x7C2ACD8); // 0x7C2ACD8
		if (!unk) return 0;

		return reinterpret_cast<GameManager*>(_rotl64((unk ^ 0x6B) - 0x56, 0x1B));
	}

	PlayerController* controllerList() {
		if (!this) return {};

		const auto unk = rpm<std::uintptr_t>((uintptr_t)this + 0x138);
		if (!unk) return 0;

		return reinterpret_cast<PlayerController*>(((unk ^ 0x1C) + 0xD5DC38CA549FDCBF) ^ 0xC46AA449993A7864);
	}

	int controllerCount() {
		if (!this) return {};

		const auto unk = rpm<std::uintptr_t>((uintptr_t)this + 0x140);
		if (!unk) return 0;

		return static_cast<int>((((unk ^ 0x1C) + 0xD5DC38CA549FDCBF) ^ 0xC46AA449993A7864) & 0x3FFFFFFF);
	}
};