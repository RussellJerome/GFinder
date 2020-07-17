#include <cstdint>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <psapi.h>
#include "Main.h"
class FUObjectArray{};
class TNameEntryArray{};
class UWorld{};
FUObjectArray* GlobalObjects;
TNameEntryArray* GlobalNames;
UWorld* GlobalWorld;

uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask)
{
	MODULEINFO info = { };
	GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

	return FindPattern(reinterpret_cast<uintptr_t>(module), info.SizeOfImage, pattern, mask);
}

uintptr_t FindPattern(uintptr_t start, size_t length, const unsigned char* pattern, const char* mask)
{
	size_t pos = 0;
	auto maskLength = std::strlen(mask) - 1;

	auto startAdress = start;
	for (auto it = startAdress; it < startAdress + length; ++it)
	{
		if (*reinterpret_cast<unsigned char*>(it) == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return it - maskLength;
			}

			pos++;
		}
		else
		{
			pos = 0;
		}
	}

	return -1;
}


bool InitializeOBJ()
{
	std::cout << "ObjectsStore Initialize" << std::endl;
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xD6", "xxx????x????x????x????xxx");
	GlobalObjects = (FUObjectArray*)(Address + *(DWORD*)(Address + 0x3) + 0x7);
	if (!GlobalObjects)
	{
		std::cout << "ObjectsStore pattern not found!" << std::endl;
	}
	else
	{
		std::cout << GlobalObjects << std::endl;
	}
	return true;
}

bool InitializeName()
{
	std::cout << "NamesStore Initialize" << std::endl;
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x50\xB9\x00\x00\x00\x00\x48\x89\x5C\x24", "xxx????xxxxxx????xxxx");
	GlobalNames = *(TNameEntryArray**)(Address + *(DWORD*)(Address + 0x3) + 0x7);
	if (!GlobalNames)
	{
		std::cout << "NamesStore pattern not found!" << std::endl;
	}
	else
	{
		std::cout << GlobalNames << std::endl;
	}
	return true;
}

bool InitializeGWorld()
{
	std::cout << "GlobalWorld Initialize" << std::endl;
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x48\x8B\x1D\x00\x00\x00\x00\x00\x00\x00\x10\x4C\x8D\x4D\x00\x4C", "xxx???????xxxx?x");
	GlobalWorld = *(UWorld**)(Address + *(DWORD*)(Address + 0x3) + 0x7);
	if (!GlobalWorld)
	{
		std::cout << "GlobalWorld pattern not found!" << std::endl;
	}
	else
	{
		std::cout << GlobalWorld << std::endl;
	}
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  _reason, LPVOID lpReserved)
{
	if (AllocConsole())
	{
		freopen_s((FILE**)stdin, "CONOIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		std::cout << "Starting\n";
		InitializeOBJ();
		InitializeName();
		InitializeGWorld();
		return true;
	}
	return TRUE;

}