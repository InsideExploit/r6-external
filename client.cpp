#include "includes.h"

void debugger_thread() {
	while (true) {
		if (is_debugger_present() || is_debugger_present_patched() || is_hardware_breakpoint_present() || is_pageguard_breakpoint() || is_debugger_running()) {
			// debugger detected
			*(void**)(0) = 0;
			//LI_FN(abort).safe_cached()();
		}
		LI_FN(Sleep).safe_cached()(1);
	}
}

void checksum_thread() {
	while (true) {

		static std::vector<uint8_t> ochecksum_thread = crc32_function((PBYTE)&checksum_thread);
		static std::vector<uint8_t> odebugger_thread = crc32_function((PBYTE)&debugger_thread);
		static std::vector<uint8_t> ois_debugger_present = crc32_function((PBYTE)&is_debugger_present);
		static std::vector<uint8_t> ois_debugger_present_patched = crc32_function((PBYTE)&is_debugger_present_patched);
		static std::vector<uint8_t> ois_hardware_breakpoint_present = crc32_function((PBYTE)&is_hardware_breakpoint_present);
		static std::vector<uint8_t> ois_pageguard_breakpoint = crc32_function((PBYTE)&is_pageguard_breakpoint);
		static std::vector<uint8_t> ois_debugger_running = crc32_function((PBYTE)&is_debugger_running);

		// patch functions
		static std::vector<uint8_t> odbg_breakpoint_patch = crc32_function((PBYTE)&dbg_breakpoint_patch);
		static std::vector<uint8_t> odbg_ui_remote_patch = crc32_function((PBYTE)&dbg_ui_remote_patch);

		// winapi
		static std::vector<uint8_t> oGetModuleHandleA = crc32_function((PBYTE)LI_FN(GetModuleHandleA).safe_cached());
		static std::vector<uint8_t> oGetProcAddress = crc32_function((PBYTE)LI_FN(GetProcAddress).safe_cached());
		static std::vector<uint8_t> oCreateToolhelp32Snapshot = crc32_function((PBYTE)LI_FN(CreateToolhelp32Snapshot).safe_cached());
		static std::vector<uint8_t> oProcess32FirstW = crc32_function((PBYTE)LI_FN(Process32FirstW).safe_cached());
		static std::vector<uint8_t> oGetCurrentProcessId = crc32_function((PBYTE)LI_FN(GetCurrentProcessId).safe_cached());
		static std::vector<uint8_t> oOpenProcess = crc32_function((PBYTE)LI_FN(OpenProcess).safe_cached());
		static std::vector<uint8_t> oReadProcessMemory = crc32_function((PBYTE)LI_FN(ReadProcessMemory).safe_cached());
		static std::vector<uint8_t> oCloseHandle = crc32_function((PBYTE)LI_FN(CloseHandle).safe_cached());
		static std::vector<uint8_t> oProcess32NextW = crc32_function((PBYTE)LI_FN(Process32NextW).safe_cached());
		static std::vector<uint8_t> oVirtualAlloc = crc32_function((PBYTE)LI_FN(VirtualAlloc).safe_cached());
		static std::vector<uint8_t> oGetThreadContext = crc32_function((PBYTE)LI_FN(GetThreadContext).safe_cached());
		static std::vector<uint8_t> oGetCurrentThread = crc32_function((PBYTE)LI_FN(GetCurrentThread).safe_cached());
		static std::vector<uint8_t> oVirtualFree = crc32_function((PBYTE)LI_FN(VirtualFree).safe_cached());
		static std::vector<uint8_t> oGetSystemInfo = crc32_function((PBYTE)LI_FN(GetSystemInfo).safe_cached());
		static std::vector<uint8_t> oVirtualProtect = crc32_function((PBYTE)LI_FN(VirtualProtect).safe_cached());
		static std::vector<uint8_t> oDebugBreak = crc32_function((PBYTE)LI_FN(DebugBreak).safe_cached());

		// security functions
		static std::vector<uint8_t> osecurityencrypt = crc32_function((PBYTE)&security::encrypt);
		static std::vector<uint8_t> osecuritydecrypt = crc32_function((PBYTE)&security::decrypt);
		static std::vector<uint8_t> osecurityencrypt_decrypt_data = crc32_function((PBYTE)&security::encrypt_decrypt_data);

		// encryption
		static std::vector<uint8_t> oencsendEnc = crc32_function((PBYTE)&sendEnc);
		static std::vector<uint8_t> oencrecvDec = crc32_function((PBYTE)&recvDec);

		// random
		static std::vector<uint8_t> ogen_random = crc32_function((PBYTE)&gen_random);

		// checksum
		static std::vector<uint8_t> odetect_changes = crc32_function((PBYTE)&detect_changes);
		static std::vector<uint8_t> odetect_function_size = crc32_function((PBYTE)&detect_function_size);
		static std::vector<uint8_t> ocrc32_function = crc32_function((PBYTE)&crc32_function);

		if (
			// check threads
			detect_changes((PBYTE)&checksum_thread, ochecksum_thread) ||
			detect_changes((PBYTE)&debugger_thread, odebugger_thread) ||

			// debuggers function
			detect_changes((PBYTE)&is_debugger_present, ois_debugger_present) ||
			detect_changes((PBYTE)&is_debugger_present_patched, ois_debugger_present_patched) ||
			detect_changes((PBYTE)&is_hardware_breakpoint_present, ois_hardware_breakpoint_present) ||
			detect_changes((PBYTE)&is_pageguard_breakpoint, ois_pageguard_breakpoint) ||
			detect_changes((PBYTE)&is_debugger_running, ois_debugger_running) ||
			detect_changes((PBYTE)&dbg_breakpoint_patch, odbg_breakpoint_patch) ||
			detect_changes((PBYTE)&dbg_ui_remote_patch, odbg_ui_remote_patch) ||

			// apis
			detect_changes((PBYTE)LI_FN(GetModuleHandleA).safe_cached(), oGetModuleHandleA) ||
			detect_changes((PBYTE)LI_FN(GetProcAddress).safe_cached(), oGetProcAddress) ||
			detect_changes((PBYTE)LI_FN(CreateToolhelp32Snapshot).safe_cached(), oCreateToolhelp32Snapshot) ||
			detect_changes((PBYTE)LI_FN(Process32FirstW).safe_cached(), oProcess32FirstW) ||
			detect_changes((PBYTE)LI_FN(GetCurrentProcessId).safe_cached(), oGetCurrentProcessId) ||
			detect_changes((PBYTE)LI_FN(OpenProcess).safe_cached(), oOpenProcess) ||
			detect_changes((PBYTE)LI_FN(ReadProcessMemory).safe_cached(), oReadProcessMemory) ||
			detect_changes((PBYTE)LI_FN(CloseHandle).safe_cached(), oCloseHandle) ||
			detect_changes((PBYTE)LI_FN(Process32NextW).safe_cached(), oProcess32NextW) ||
			detect_changes((PBYTE)LI_FN(VirtualAlloc).safe_cached(), oVirtualAlloc) ||
			detect_changes((PBYTE)LI_FN(GetThreadContext).safe_cached(), oGetThreadContext) ||
			detect_changes((PBYTE)LI_FN(GetCurrentThread).safe_cached(), oGetCurrentThread) ||
			detect_changes((PBYTE)LI_FN(VirtualFree).safe_cached(), oVirtualFree) ||
			detect_changes((PBYTE)LI_FN(GetSystemInfo).safe_cached(), oGetSystemInfo) ||
			detect_changes((PBYTE)LI_FN(VirtualProtect).safe_cached(), oVirtualProtect) ||
			detect_changes((PBYTE)LI_FN(DebugBreak).safe_cached(), oDebugBreak) ||

			// security
			detect_changes((PBYTE)&security::encrypt, osecurityencrypt) ||
			detect_changes((PBYTE)&security::decrypt, osecuritydecrypt) ||
			detect_changes((PBYTE)&security::encrypt_decrypt_data, osecurityencrypt_decrypt_data) ||

			// enc
			detect_changes((PBYTE)&sendEnc, oencsendEnc) ||
			detect_changes((PBYTE)&recvDec, oencrecvDec) ||

			// random
			detect_changes((PBYTE)&gen_random, ogen_random) ||

			// checksum
			detect_changes((PBYTE)&detect_changes, odetect_changes) ||
			detect_changes((PBYTE)&detect_function_size, odetect_function_size) ||
			detect_changes((PBYTE)&crc32_function, ocrc32_function)
			)
		{
			// checksum corrupted
			*(void**)(0) = 0;
			//LI_FN(abort).safe_cached()();
		}

		LI_FN(Sleep).safe_cached()(1);
	}
}

void fuckMyAssLGBTQUIA(uintptr_t function){
	DWORD old_protect{};
	LI_FN(VirtualProtect).safe_cached()((LPVOID)function, 8, PAGE_EXECUTE_READWRITE, &old_protect);
	*(void**)(function) = 0;
	LI_FN(VirtualProtect).safe_cached()((LPVOID)function, 8, old_protect, &old_protect);
}

int main()
{
	// map driver
	if (map_driver(pe_auth) != 0) {
		LI_FN(printf).safe_cached()(xorstr_(" (-) failed initialise something, better if you restart.\n"));
	}

	// fuck
	DWORD old_protect{};
	fuckMyAssLGBTQUIA((uintptr_t)LI_FN(GetProcAddress).safe_cached()(LI_FN(GetModuleHandleA).safe_cached()(xorstr_("kernel32.dll")), xorstr_("LoadLibraryW")));
	fuckMyAssLGBTQUIA((uintptr_t)LI_FN(GetProcAddress).safe_cached()(LI_FN(GetModuleHandleA).safe_cached()(xorstr_("kernel32.dll")), xorstr_("LoadLibraryA")));

	// debugging
	dbg_breakpoint_patch();
	dbg_ui_remote_patch();

	// threading
	std::thread(debugger_thread).detach();
	std::thread(checksum_thread).detach();
	WSADATA wsa_data;
	SOCKADDR_IN addr;

	int wsa = LI_FN(WSAStartup).safe_cached()(MAKEWORD(2, 2), &wsa_data);
	if (wsa != 0)
	{
		LI_FN(WSACleanup).safe_cached()();
		return 0;
	}

	const auto server = LI_FN(socket).safe_cached()(AF_INET, SOCK_STREAM, 0);

	LI_FN(inet_pton).safe_cached()(AF_INET, xorstr_("IP_HERE"), &addr.sin_addr.s_addr);

	addr.sin_family = AF_INET;
	addr.sin_port = LI_FN(htons).safe_cached()(1337);

	int con = LI_FN(connect).safe_cached()(server, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	if (con != 0)
	{
		LI_FN(closesocket).safe_cached()(server);
		LI_FN(WSACleanup).safe_cached()();
		return 0;
	}

	LI_FN(printf).safe_cached()(xorstr_(" (+) connected\n"));

	//program
	int pid = getpid();
	string license, buffer;

	//get license
	LI_FN(printf).safe_cached()(xorstr_(" (>) enter your license: "));
	cin >> license;

	//get hwid
	query();
	hwid += m_sid;
	hwid.erase(remove_if(hwid.begin(), hwid.end(), isspace), hwid.end());

	//prepare and send initialization data and make encryption key
	string preInit = xorstr_("initcheck__") + hwid + xorstr_("__") + version + xorstr_("__") + license;

	string akey = gen_random(32, pid);
	string aiv = gen_random(16, pid);

	string initData = security::encrypt(preInit, akey, aiv);
	buffer = initData + xorstr_("__") + to_string(pid * 3 - 796);

	LI_FN(send).safe_cached()(server, buffer.c_str(), 2048, 0);

	//receive version check and validate
	buffer = recvDec(server, akey, aiv);
	if (buffer == xorstr_("badinitcheck"))
	{
		LI_FN(printf).safe_cached()(xorstr_(" (-) request took too long\n"));
		LI_FN(closesocket).safe_cached()(server);
		LI_FN(WSACleanup).safe_cached()();
		LI_FN(system).safe_cached()(xorstr_("pause"));
	}
	else if (buffer == xorstr_("goodver"))
	{
		//receive license and ban check
		buffer = recvDec(server, akey, aiv);
		if (buffer == xorstr_("hwidban"))
		{
			LI_FN(printf).safe_cached()(xorstr_(" (-) you have been banned\n"));
			LI_FN(closesocket).safe_cached()(server);
			LI_FN(WSACleanup).safe_cached()();
			LI_FN(Sleep).safe_cached()(2200);
		}
		else if (buffer == xorstr_("invalid") || buffer == xorstr_("expired"))
		{
			LI_FN(printf).safe_cached()(xorstr_(" (-) your license is invalid or expired\n"));
			LI_FN(closesocket).safe_cached()(server);
			LI_FN(WSACleanup).safe_cached()();
			LI_FN(Sleep).safe_cached()(2200);
		}
		else if (buffer == xorstr_("badhwid"))
		{
			LI_FN(printf).safe_cached()(xorstr_(" (-) hwid doesn't match\n"));
			LI_FN(closesocket).safe_cached()(server);
			LI_FN(WSACleanup).safe_cached()();
			LI_FN(Sleep).safe_cached()(2200);
		}
		else
		{
			LI_FN(printf).safe_cached()(xorstr_(" (+) success\n"));
			LI_FN(Sleep).safe_cached()(2200);

			LI_FN(system).safe_cached()(xorstr_("cls"));
			program();

			LI_FN(closesocket).safe_cached()(server);
			LI_FN(WSACleanup).safe_cached()();
		}
	}
	else
	{
		LI_FN(printf).safe_cached()(xorstr_(" (-) invalid version\n"));
		LI_FN(closesocket).safe_cached()(server);
		LI_FN(WSACleanup).safe_cached()();
		LI_FN(system).safe_cached()(xorstr_("pause"));
	}

	LI_FN(closesocket).safe_cached()(server);
	LI_FN(WSACleanup).safe_cached()();
	return 0;
}