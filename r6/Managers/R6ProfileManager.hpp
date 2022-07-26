#pragma once
#include "..\..\driver\driver.h"
#include "..\Controller\Controller.hpp"

struct R6ProfileManager {};

struct ProfileManager : R6ProfileManager
{
	static ProfileManager* global() {
		auto unk_1 = rpm<std::uintptr_t>(g_moduleBase + /*0x7611980*/0x75BB600); // 0x75BB600
		if (!unk_1) return 0;

		unk_1 ^= 0x7806877225AD852C;

		auto unk_2 = rpm<std::uintptr_t>(unk_1 + 0x18);
		if (!unk_2) return 0;

		return reinterpret_cast<ProfileManager*>(_rotl64(unk_2 ^ 0x2E0E1E821F7D24CF, 0x3C));
	}

	PlayerController* localController() {
		if (!this) return {};

		auto unk_1 = rpm<std::uintptr_t>((uintptr_t)this + 0xEC97021BA78D76AF);
		if (!unk_1) return 0;

		auto unk_2 = rpm<std::uintptr_t>(unk_1 + 0x30);
		if (!unk_2) return 0;

		return reinterpret_cast<PlayerController*>(_rotl64(unk_2 ^ 0xA4DC8E444583FA72, 0xC) + 0x3451A281F57858CC);
	}
};