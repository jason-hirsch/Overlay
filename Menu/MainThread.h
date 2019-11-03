#pragma once
#include "DxHook.h"


class MainThread {
private:
	std::atomic<bool> stopThreads;
	Output *out;
	HMODULE hDll;
public:
	void start(HMODULE hanDll, LPCTSTR dllMain);
	static void mainThread(MainThread *self, HMODULE hanDll, LPCTSTR dllMain);
	~MainThread();
};

