#pragma once
#include "..\..\driver\driver.h"
#include "..\Pawn\Pawn.hpp"
#include "PlayerReplicationInfo.hpp"

struct Controller {};

struct PlayerController : Controller
{
	Pawn* pawn() {
		if (!this) return {};

		const auto unk = rpm<std::uintptr_t>((uintptr_t)this + 0x38);
		if (!unk) return 0;

		return reinterpret_cast<Pawn*>((_rotl64(unk, 2) ^ 0x6E2C20B9BADACF18) - 0x2C);
	}

	PlayerReplicationInfo* playerReplicationInfo() {
		if (!this) return {};

		const auto unk = rpm<std::uintptr_t>((uintptr_t)this + 0xA0);
		if (!unk) return 0;

		return reinterpret_cast<PlayerReplicationInfo*>((_rotl64(unk + 0xFFFFFFFFFFFFFFC1, 0x24)) + 0xFFFFFFFFFFFFFF93);
	}
};