#pragma once
bool InitializeName();
bool InitializeOBJ();
bool InitializeGWorld();
uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
uintptr_t FindPattern(uintptr_t start, size_t length, const unsigned char* pattern, const char* mask);



