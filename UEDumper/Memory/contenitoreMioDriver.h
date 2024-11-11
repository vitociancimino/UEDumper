#pragma once
#include <Windows.h>
#include <tlhelp32.h>
#include "HandleMioDriver.h"


inline void init()
{
	if (!driver::ready())
	{
		exit(0);
	}
}

//qua bisogna restituire il pid del processo e il base address
inline void loadData(std::string& processName, uint64_t& baseAddress, int& processID)
{
	processID = (int)driver::attach(processName.c_str());
	baseAddress = driver::get_module_base_address(processName.c_str());
}

uint64_t _getBaseAddress(const wchar_t* processName, int& pid)
{
	char convertedProcessName[256];
	wcstombs(convertedProcessName, processName, sizeof(convertedProcessName));
	return driver::get_module_base_address(convertedProcessName);
}

void attachToProcess(const int& pid)
{
	driver::target_process = (HANDLE)pid;
}

inline void _read(const void* address, void* buffer, const DWORD64 size)
{
	driver::read_size((uint64_t)address, buffer, size);
}

inline void _write(void* address, const void* buffer, const DWORD64 size)
{
	driver::write_size((uint64_t)address, &buffer, size);

}
