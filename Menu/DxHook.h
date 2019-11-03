#pragma once
#include "Output.h"
#include "Hook.h"
#include "Render.h"


class DxHook {
private:
	static void* vTable[119];
	static Output* out;
	static Render r;
	static Hook* h;
	static bool getVTable();
	static bool hookEndScene();
	static bool hookBeginScene();
	static bool hookPresent();
	static bool getDxObj();
	static bool startRender();
public:
	static bool runHook(Output* out);
};