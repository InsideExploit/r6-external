#pragma once
#include "..\..\driver\driver.h"
#include "..\Actor\Actor.hpp"

struct BasePawn {};

struct Pawn : BasePawn
{
	Actor* actor() {
		if (!this) return {};

		const auto unk = rpm<std::uintptr_t>((uintptr_t)this + 0x18);
		if (!unk) return 0;

		return reinterpret_cast<Actor*>(_rotl64(_rotl64(unk, 0x1C) ^ 0x9E600726EA5DC460, 0x37));
	}
};