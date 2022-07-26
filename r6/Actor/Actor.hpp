#pragma once
#include "Weapon.hpp"

struct Actor
{
	Weapon* currentWeapon() {
		if (!this) return {};

		const auto component = rpm<std::uintptr_t>((uintptr_t)this + 0xD8);
		if (!component) return 0;

		const auto index = rpm<BYTE>((uintptr_t)this + 0x228);

		const auto weapon_array = rpm<std::uintptr_t>(component + (index * sizeof(uintptr_t)));
		if (!weapon_array) return 0;

		auto entry = rpm<std::uintptr_t>(weapon_array + 0xE0);
		if (!entry) return 0;

		entry = ((entry - 0x2C0768632F2C7564) ^ 0xDDB76692420BF881) - 0x5FEBC83F4293B191;

		entry = rpm<std::uintptr_t>(entry);
		if (!entry) return 0;

		const auto array_entry = rpm<std::uintptr_t>(entry + 0x1B0);
		if (!array_entry) return 0;

		const auto current_weapon = rpm<std::uintptr_t>(array_entry + 0x208);
		if (!current_weapon) return 0;

		return reinterpret_cast<Weapon*>(_rotl64(_rotl64(current_weapon, 0x35) - 0x59, 0xC));
	}
};