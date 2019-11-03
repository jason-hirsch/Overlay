#include "DxHook.h"


void* DxHook::vTable[119];
Output* DxHook::out;
Render DxHook::r;
Hook DxHook::h;


BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam) {
	DWORD pid;
	GetWindowThreadProcessId(handle, &pid);
	if (GetCurrentProcessId() == pid) {
		HWND& hWnd = *reinterpret_cast<HWND*>(lParam);
		hWnd = handle;
		return FALSE;
	}
	return TRUE;
}


bool DxHook::getVTable() {
	LPDIRECT3D9 d3d; //pointer to d3d interface
	LPDIRECT3DDEVICE9 d3dev; //pointer to d3d device class
	D3DPRESENT_PARAMETERS devInfo; //device info
	HWND hWnd = NULL; //process window

	out->printReg("Creating D3D Interface...");
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) {
		out->printErr("Failed to create D3D Interface");
		d3d->Release();
		return false;
	}

	out->printReg("Finding process window...");
	EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&hWnd));
	if (!hWnd) {
		out->printErr("Failed to find process window");
		d3d->Release();
		return false;
	}

	out->printReg("Setting Device Information...");
	ZeroMemory(&devInfo, sizeof(devInfo));
	devInfo.Windowed = true;
	devInfo.SwapEffect = D3DSWAPEFFECT_DISCARD;
	devInfo.hDeviceWindow = hWnd;

	out->printReg("Creating D3D Device...");
	HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, devInfo.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &devInfo, &d3dev);
	if (hr != S_OK) {
		out->printErr("Failed to create D3D Device as windowed, trying not windowed...");
		devInfo.Windowed = false;
		HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, devInfo.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &devInfo, &d3dev);
		if (hr != S_OK) {
			out->printErr("Failed to create D3D Device");
			d3d->Release();
			return false;
		}
	}

	out->printReg("Saving address of vtable...");
	memcpy(vTable, *reinterpret_cast<void***>(d3dev), sizeof(vTable));
	d3d->Release();
	d3dev->Release();
	return true;
}


bool DxHook::hookEndScene() {
	out->printReg("Hooking EndScene...");
	char s[100];
	sprintf_s(s, "%p", h.trampoline);
	out->printReg(std::string(s));
	h.setPos((DWORD)vTable[42]);
	h.setLen(7);
	h.setFunc((DWORD)DxHook::startRender);
	return h.install();
}


bool DxHook::hookBeginScene() {
	out->printReg("Failed to hook EndScene, trying to hook BeginScene...");
	h.setPos((DWORD)vTable[41]);
	h.setLen(7);
	h.setFunc((DWORD)DxHook::startRender);
	return h.install();
}


bool DxHook::hookPresent() {
	out->printReg("Failed to hook BeginScene, trying to hook Present...");
	h.setPos((DWORD)vTable[17]);
	h.setLen(5);
	h.setFunc((DWORD)DxHook::startRender);
	return h.install();
}


bool DxHook::getDxObj() {
	return true;
}


bool DxHook::startRender() {
	out->printReg("Rendered");
	h.trampoline();
	return true;
}


bool DxHook::runHook(Output* o) {
	out = o;
	bool ret = true;
	if (ret = getVTable()) {
		if (ret = (hookEndScene() || hookBeginScene() || hookPresent())) {
			ret = getDxObj();
		}
	}
	return ret;
}

