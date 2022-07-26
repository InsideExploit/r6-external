#include "includes.h"
#include <atlsecurity.h> 

std::string get_hwid() {
	ATL::CAccessToken accessToken;
	ATL::CSid currentUserSid;
	if (accessToken.GetProcessToken(TOKEN_READ | TOKEN_QUERY) &&
		accessToken.GetUser(&currentUserSid))
		return std::string(CT2A(currentUserSid.Sid()));
	return xorstr_("");
}

bool query()
{
	hwid = get_hwid();
	return true;
}