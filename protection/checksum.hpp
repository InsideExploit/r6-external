#pragma once

inline std::vector<uint8_t> crc32_function(PVOID pFunc);

inline bool detect_changes(PVOID pFunc, std::vector<uint8_t> original) {
    if (crc32_function(pFunc) != original) return true;
    return false;
}

inline size_t detect_function_size(PVOID pFunc) {
    PBYTE pMem = (PBYTE)pFunc;
    size_t nFuncSize = 0;
    do
    {
        ++nFuncSize;
    } while (*(pMem++) != 0xC3);
    return nFuncSize;
}

inline std::vector<uint8_t> crc32_function(PVOID pFunc) {
    
    std::vector<uint8_t> temp;
    for (auto i = 0; i < detect_function_size(pFunc); i++) {
        temp.push_back(*(BYTE*)((uint64_t)pFunc + i));
    }
    return temp;
}