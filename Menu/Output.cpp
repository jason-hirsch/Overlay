#include "Output.h"


Output::Output() { //Constructor
	debug = false;
}


void Output::printReg(std::string msg) { //Print white text
	if (debug) {
		std::lock_guard<std::mutex> lock(printLock);
		std::cout << msg << "\n";
	}
}


void Output::printSucc(std::string msg) { //Print green text
	if (debug) {
		std::lock_guard<std::mutex> lock(printLock);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
		std::cout << msg << "\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	}
}


void Output::printErr(std::string msg) { //Print red text
	if (debug) {
		std::lock_guard<std::mutex> lock(printLock);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0C);
		std::cout << msg << "\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	}
}


void Output::enable() { //Enable console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	debug = true;
}



void Output::disable() { //Disable console
	debug = false;
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
}


Output::~Output() { //Clean up
	if (debug) disable();
}

