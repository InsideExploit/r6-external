#include "includes.h"
#include "driver/driver.h"
#include "r6/r6.hpp"

void program()
{
	LI_FN(printf).safe_cached()(xorstr_(" (*) loading spoofer\n"));
	if (map_driver(spoofer) != 0)
		LI_FN(printf).safe_cached()(xorstr_(" (-) failed spoofing\n"));
	else
		LI_FN(printf).safe_cached()(xorstr_(" (+) spoofed with success\n"));

	// initialize driver
	if (!driver->Init(false)) {
		LI_FN(printf).safe_cached()(xorstr_(" (-) couldn't initialize cheeze, retry\n"));
		LI_FN(Sleep).safe_cached()(2000);
		*(void**)(0) = 0;
	}

	LI_FN(printf).safe_cached()(xorstr_("\n"));
	LI_FN(printf).safe_cached()(xorstr_(" (*) start the game, after press F4 in menu\n"));

	while (!GetAsyncKeyState(VK_F4)) { LI_FN(Sleep).safe_cached()(1); };

	if (!driver->Attach(xorstr_("RainbowSix.exe"))) {

		LI_FN(printf).safe_cached()(xorstr_(" (-) couldn't attach to game, retry\n"));
		LI_FN(Sleep).safe_cached()(2000);

		*(void**)(0) = 0;
	}

	LI_FN(printf).safe_cached()(xorstr_(" (+) game attached\n"));

	r6();
}