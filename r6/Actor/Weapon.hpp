#pragma once

struct Weapon
{
	void setRecoil(float value) {
		if (!this) return;

		wpm<std::uint32_t>((uintptr_t)this + 0x1A8, ((*(uint32_t*)&value - 0x7866FCFB) ^ 0x2196A6B6u) + 0x33);
	}

	void setSpread(float value) {
		if (!this) return;

		wpm<std::uint32_t>((uintptr_t)this + 0x44, _rotr((*(uint32_t*)&value) + 0x32, 0xB) - 0x1E181AB8);
		wpm<std::uint32_t>((uintptr_t)this + 0x64, (_rotr(*(uint32_t*)&value, 2) ^ 0x82A80848) + 0x7D2F52CF);
	}
};