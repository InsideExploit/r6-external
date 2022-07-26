#pragma once
#include "..\..\driver\driver.h"

struct PlayerReplicationInfo
{
	const char* name() {
		if (!this) return ("Terrorist");

		return rpm<const char*>((uintptr_t)this + 0x9F0);
	}

	int teamId() {
		if (!this) return {};

		auto unk_1 = rpm<std::uintptr_t>((uintptr_t)this + 0x6D8);
		if (!unk_1) return 0;

		unk_1 = _rotl64(unk_1, 0x15);
		
		uintptr_t rcx = 0xF91791C3840711C9;
		rcx += unk_1;
		rcx = _rotl64(rcx, 0x5);

		auto unk_2 = rpm<std::uint32_t>(rcx + 0xA0);
		if (!unk_2) return 0;

		return _rotl(unk_2 + 0xDCA87FEA, 0x1D) + 0xD1B08CDD;
	}
};