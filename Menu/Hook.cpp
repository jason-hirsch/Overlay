#include "Hook.h"


DWORD Hook::pos;
DWORD Hook::retPos;
DWORD Hook::func;
DWORD Hook::len;


void __declspec(naked) Hook::trampoline() {
	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		jmp retPos
	};
}


bool Hook::install() {
	DWORD prot, prot2, tempProt;
	VirtualProtect(trampoline, len, PAGE_EXECUTE_READWRITE, &prot);
	VirtualProtect((void*)pos, len, PAGE_EXECUTE_READWRITE, &prot2);
	memcpy(trampoline, (void*)pos, len);
	VirtualProtect(trampoline, len, prot, &tempProt);
	memset((void*)pos, 0x90, len);
	*(DWORD*)pos = 0xE9;
	*(DWORD*)(pos + 1) = func - pos - 5;
	VirtualProtect((void*)pos, len, prot2, &tempProt);
	return true;
}


bool Hook::uninstall() {
	DWORD prot, prot2, tempProt;
	VirtualProtect(trampoline, len, PAGE_EXECUTE_READWRITE, &prot);
	VirtualProtect((void*)pos, len, PAGE_EXECUTE_READWRITE, &prot2);
	memcpy((void*)pos, trampoline, len);
	VirtualProtect(trampoline, len, prot, &tempProt);
	VirtualProtect((void*)pos, len, prot2, &tempProt);
	return true;
}

