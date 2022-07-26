#pragma once
#include "..\..\driver\driver.h"

struct R6RoundManager {};

struct RoundManager : R6RoundManager
{
	static RoundManager* global() {
		auto unk = rpm<std::uintptr_t>(g_moduleBase + /*0x77FB0B8*/0x77A4D38); // 0x77A4D38
		if (!unk) return 0;

		return reinterpret_cast<RoundManager*>((unk ^ 0x20) - 0x6BCAB792AF644E44);
	}

	int32_t roundState() {
		if (!this) return {};

		const auto unk = rpm<std::int32_t>((uintptr_t)this);
		if (!unk) return 0;

		return ((unk - 0x7A) ^ 0xC7A6E52D) + 0x36557EBE;
	}

	bool isPlaying() {
		if (!this) return {};

		return roundState() == 2 || roundState() == 3;
	}
};