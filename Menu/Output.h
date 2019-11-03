#pragma once
#pragma comment(lib, "d3d9.lib")

#include <iostream>
#include <mutex>
#include <atomic>
#include <Windows.h>
#include <cstdio>
#include <d3d9.h>


class Output {
private:
	std::mutex printLock;
	bool debug;
public:
	Output();
	void printReg(std::string msg);
	void printSucc(std::string msg);
	void printErr(std::string msg);
	void enable();
	void disable();
	~Output();
};

