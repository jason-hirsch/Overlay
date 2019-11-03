#pragma once
#include <Windows.h>


class Hook {
private:
	DWORD pos, len, retPos, func;
	LPVOID origBytes;
public:
	void trampoline();
	bool install();
	bool uninstall();
	Hook(DWORD pos, DWORD len, DWORD func);
};

