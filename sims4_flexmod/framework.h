#pragma once

#ifdef _WIN64
#define _AMD64_
#else
#define _X86_
#endif

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define EXTERN_API extern "C" _declspec(dllexport)

// Function typedef
#define __typedef_func(func, ret, ...) using func##_t = ret(WINAPI*)(##__VA_ARGS__); \
                                           func##_t m_##func = nullptr;

// Resolve the specified function proc
#define __setup_proc(func) m_##func = reinterpret_cast<func##_t>(::GetProcAddress(m_versiondll, #func));

// Set the proc back to nullptr
#define __clear_proc(func) if(m_##func) m_##func = nullptr;

// Windows Header Files
#include <minwindef.h>
#include <libloaderapi.h>
#include <sysinfoapi.h>
#include <winternl.h>
#include <string>
#include <fstream>
#include <filesystem>

// Proxy stub
#include "stub.h"

// APIs redirector
#include "redirector.h"
