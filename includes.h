#pragma once

//defines
#define _CRT_NONSTDC_NO_WARNINGS

//main
#include <iostream>
#include <sstream>

//socket
#include <winsock2.h>
#include <ws2tcpip.h>

//crypto
#include "../shared/crypto/crypto.h"
#include "protection/xorstr.hpp"

// shellcodes
#include "shellcode.hpp"

//extra
#pragma comment(lib,"WS2_32")

//namespaces
using namespace std;

/******************************************************/
inline string version = xorstr_("0.1.0");	//version of your program
/******************************************************/

//declarations
void sendEnc(SOCKET s, string data, string akey, string aiv);
string recvDec(SOCKET s, string akey, string aiv);

string gen_random(const int len, int pid);

bool query();

void program();

inline string m_sid;
inline string hwid;

#pragma once
#define WIN32_LEAN_AND_MEAN

// etc
#include <windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <vector>
#include <thread>

// imports
extern "C" NTSTATUS map_driver(std::vector<uint8_t> raw_image);

// obfuscation
#include "protection/lazy_importer.hpp"
#include "protection/xorstr.hpp"

// protection
#include "protection/debugging.hpp"
#include "protection/checksum.hpp"