#pragma once
#include <Windows.h>


class Hook {
private:
	static DWORD pos, retPos, func;
	static DWORD len;
public:
	static void trampoline();
	static bool install();
	static bool uninstall();
	static void setPos(DWORD p) { pos = p; retPos = pos + len; }
	static void setFunc(DWORD f) { func = f; }
	static void setLen(DWORD l) { len = l; retPos = pos + len; }
};

