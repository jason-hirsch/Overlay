#include "Hook.h"


void Hook::trampoline() {
	auto *const origBytesFunc = reinterpret_cast<__int32(*)()>(origBytes);
	origBytesFunc();
}


bool Hook::install() {
	DWORD origProt, tempProt;
	origBytes = VirtualAlloc(NULL, len+5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!origBytes) return false;

	if(!VirtualProtect((LPVOID)pos, len, PAGE_EXECUTE_READWRITE, &origProt)) {
		VirtualFree(origBytes, 0, MEM_RELEASE);
		return false;
	}

	memcpy(origBytes, (LPVOID)pos, len);
	*(DWORD*)((DWORD)origBytes + len) = 0xE9;
	*(DWORD*)((DWORD)origBytes + len + 1) = retPos - (DWORD)origBytes + len - 5;

	memset((LPVOID)pos, 0x90, len);
	*(DWORD*)pos = 0xE9;
	*(DWORD*)(pos + 1) = func - pos - 5;

	if(!VirtualProtect((LPVOID)pos, len, origProt, &tempProt)) {
		memcpy((LPVOID)pos, origBytes, len);
		VirtualFree(origBytes, 0, MEM_RELEASE);
		return false;
	}
	return true;
}


bool Hook::uninstall() {
	DWORD origProt, tempProt;
	if (!VirtualProtect((void*)pos, len, PAGE_EXECUTE_READWRITE, &origProt)) {
		return false;
	}
	memcpy((LPVOID)pos, origBytes, len);
	if (!VirtualProtect((LPVOID)pos, len, origProt, &tempProt)) {
		memcpy(origBytes, (LPVOID)pos, len);
		return false;
	}

	if (!VirtualFree(origBytes, 0, MEM_RELEASE)) {
		return false;
	}
	return true;
}


Hook::Hook(DWORD pos, DWORD len, DWORD func) : pos(pos), len(len), func(func) {
	retPos = pos + len;
	origBytes = nullptr;
}
