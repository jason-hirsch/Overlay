#include "MainThread.h"


void MainThread::start(HMODULE hanDll, LPCTSTR dllMain) { //Start main thread
	std::thread mainThread(mainThread, this, hanDll, dllMain);
	mainThread.detach();
}


void MainThread::mainThread(MainThread *self, HMODULE hanDll, LPCTSTR dllMain) { //Main thread
	self->hDll = hanDll;
	self->stopThreads.store(false);
	self->out = new Output;
	self->out->enable(); //TEMP

	bool ret = DxHook::runHook(self->out);
	//if (ret) self->out->printSucc("Successfully hooked DirectX");
	//else self->out->printErr("Failed to hook DirectX");

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	FreeLibrary(self->hDll);
	FreeLibraryAndExitThread(self->hDll, 0); //Free library and exit thread
}


MainThread::~MainThread() { //Deconstructor
	if (out) delete out;
}

