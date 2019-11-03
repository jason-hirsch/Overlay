#include "MainThread.h"


BOOL APIENTRY DllMain(HMODULE hDll, DWORD ul_reason_for_call, LPVOID lpReserved) { //Entry point
	static MainThread *mainThread = new MainThread;
    switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: { //Dll attached
			DisableThreadLibraryCalls(hDll); //Disable thread cases
			mainThread->start(hDll, (LPCTSTR)DllMain); //pass pointer instead later (im not sure if mem gets cleared or not)
			break;
		}
		case DLL_PROCESS_DETACH: //Dll detached
			delete mainThread; //Clean up
			break;
	}
	return TRUE;
}

