#pragma once

#pragma pack(push, 1)
struct DbgUiRemoteBreakinPatch
{
    WORD  push_0;
    BYTE  push;
    DWORD CurrentPorcessHandle;
    BYTE  mov_eax;
    UINT64 TerminateProcess;
    WORD  call_eax;
};
#pragma pack(pop)

inline bool is_debugger_present() {
    return *reinterpret_cast<bool*>(__readgsqword(0x60) + 0x2) != 0;
}

inline bool is_debugger_present_patched() {
    HMODULE hKernel32 = LI_FN(GetModuleHandleA).safe_cached()(xorstr_("kernel32.dll"));
    if (!hKernel32)
        return false;

    FARPROC pIsDebuggerPresent = LI_FN(GetProcAddress).safe_cached()(hKernel32, xorstr_("IsDebuggerPresent"));
    if (!pIsDebuggerPresent)
        return false;

    HANDLE hSnapshot = LI_FN(CreateToolhelp32Snapshot).safe_cached()(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
        return false;

    PROCESSENTRY32W ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (!LI_FN(Process32FirstW).safe_cached()(hSnapshot, &ProcessEntry))
        return false;

    bool bDebuggerPresent = false;
    HANDLE hProcess = NULL;
    DWORD dwFuncBytes = 0;
    const DWORD dwCurrentPID = LI_FN(GetCurrentProcessId).safe_cached()();
    do
    {
        __try
        {
            if (dwCurrentPID == ProcessEntry.th32ProcessID)
                __leave;

            hProcess = LI_FN(OpenProcess).safe_cached()(PROCESS_ALL_ACCESS, FALSE, ProcessEntry.th32ProcessID);
            if (NULL == hProcess)
                __leave;

            if (!LI_FN(ReadProcessMemory).safe_cached()(hProcess, pIsDebuggerPresent, &dwFuncBytes, sizeof(DWORD), NULL))
                __leave;

            if (dwFuncBytes != *(PDWORD)pIsDebuggerPresent)
            {
                bDebuggerPresent = true;
                __leave;
            }
        }
        __finally
        {
            if (hProcess)
                LI_FN(CloseHandle).safe_cached()(hProcess);
        }
    } while (LI_FN(Process32NextW).safe_cached()(hSnapshot, &ProcessEntry));

    if (hSnapshot)
        LI_FN(CloseHandle).safe_cached()(hSnapshot);
    return bDebuggerPresent;
}

inline bool is_hardware_breakpoint_present() {
	PCONTEXT context = (PCONTEXT)LI_FN(VirtualAlloc).safe_cached()(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE);
	if (context) {
		RtlSecureZeroMemory(context, sizeof(CONTEXT));
		context->ContextFlags = CONTEXT_DEBUG_REGISTERS;

		if (LI_FN(GetThreadContext).safe_cached()(LI_FN(GetCurrentThread).safe_cached()(), context))
			if (context->Dr0 != 0 || context->Dr1 != 0 || context->Dr2 != 0 || context->Dr3 != 0)
				return true;

		LI_FN(VirtualFree).safe_cached()(context, 0, MEM_RELEASE);
	}

	return false;
}

inline bool is_pageguard_breakpoint() {
	SYSTEM_INFO system_info{};
	DWORD old_protection{};

	LI_FN(GetSystemInfo).safe_cached()(&system_info);

	PVOID allocation = LI_FN(VirtualAlloc).safe_cached()(NULL, system_info.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!allocation) return false;

	PBYTE pMem = (PBYTE)allocation;
	*pMem = 0xC3;

	if (LI_FN(VirtualProtect).safe_cached()(allocation, system_info.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &old_protection) == 0) return false;

	__try {
		((void(*)())allocation)();
	}
	__except (_exception_code() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		if(LI_FN(VirtualFree).safe_cached()(allocation, 0, MEM_RELEASE))
			return false;
	}

	LI_FN(VirtualFree).safe_cached()(allocation, 0, MEM_RELEASE);
	return true;
}

inline bool is_debugger_running() {
	__try {
		DebugBreak();
	}
	__except (_exception_code() == EXCEPTION_BREAKPOINT ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		return false;
	}

	return true;
}

inline void dbg_breakpoint_patch()
{
    HMODULE hNtdll = LI_FN(GetModuleHandleA).safe_cached()(xorstr_("ntdll.dll"));
    if (!hNtdll)
        return;

    FARPROC pDbgBreakPoint = LI_FN(GetProcAddress).safe_cached()(hNtdll, xorstr_("DbgBreakPoint"));
    if (!pDbgBreakPoint)
        return;

    DWORD dwOldProtect;
    if (!LI_FN(VirtualProtect).safe_cached()(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
        return;

    *(PBYTE)pDbgBreakPoint = (BYTE)0xC3;
}

inline void dbg_ui_remote_patch()
{
    HMODULE hNtdll = LI_FN(GetModuleHandleA).safe_cached()(xorstr_("ntdll.dll"));
    if (!hNtdll)
        return;

    FARPROC pDbgUiRemoteBreakin = LI_FN(GetProcAddress).safe_cached()(hNtdll, xorstr_("DbgUiRemoteBreakin"));
    if (!pDbgUiRemoteBreakin)
        return;

    HMODULE hKernel32 = LI_FN(GetModuleHandleA).safe_cached()("kernel32.dll");
    if (!hKernel32)
        return;

    FARPROC pTerminateProcess = LI_FN(GetProcAddress).safe_cached()(hKernel32, xorstr_("TerminateProcess"));
    if (!pTerminateProcess)
        return;

    DbgUiRemoteBreakinPatch patch = { 0 };
    patch.push_0 = '\x6A\x00';
    patch.push = '\x68';
    patch.CurrentPorcessHandle = 0xFFFFFFFF;
    patch.mov_eax = '\xB8';
    patch.TerminateProcess = (UINT64)pTerminateProcess;
    patch.call_eax = '\xFF\xD0';

    DWORD dwOldProtect;
    if (!LI_FN(VirtualProtect).safe_cached()(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), PAGE_READWRITE, &dwOldProtect))
        return;

    LI_FN(memcpy_s).safe_cached()(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), &patch, sizeof(DbgUiRemoteBreakinPatch));
    LI_FN(VirtualProtect).safe_cached()(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), dwOldProtect, &dwOldProtect);
}