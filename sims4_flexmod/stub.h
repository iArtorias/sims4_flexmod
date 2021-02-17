#pragma once

namespace fs = std::filesystem;

class CProxyStub
{
private:

    // Original 'version.dll' module handle
    HMODULE m_versiondll{ nullptr };

    // A full path to the original 'version.dll'
    fs::path m_version_path{};

public:

    CProxyStub()
    {
        wchar_t sys_path[MAX_PATH]{};
        GetSystemDirectoryW( sys_path, MAX_PATH );
        m_version_path = fs::path( sys_path ) / "version.dll";
    }

    ~CProxyStub()
    {
        __clear_proc( GetFileVersionInfoA );
        __clear_proc( GetFileVersionInfoByHandle );
        __clear_proc( GetFileVersionInfoExA );
        __clear_proc( GetFileVersionInfoExW );
        __clear_proc( GetFileVersionInfoSizeA );
        __clear_proc( GetFileVersionInfoSizeExA );
        __clear_proc( GetFileVersionInfoSizeExW );
        __clear_proc( GetFileVersionInfoSizeW );
        __clear_proc( GetFileVersionInfoW );
        __clear_proc( VerFindFileA );
        __clear_proc( VerFindFileW );
        __clear_proc( VerInstallFileA );
        __clear_proc( VerInstallFileW );
        __clear_proc( VerLanguageNameA );
        __clear_proc( VerLanguageNameW );
        __clear_proc( VerQueryValueA );
        __clear_proc( VerQueryValueW );
    }

    /// <summary>
    /// Gets the current 'CProxyStub' class instance
    /// </summary>
    static CProxyStub& instance()
    {
        static CProxyStub instance;
        return instance;
    }

    // Resolve typedefs
    __typedef_func( GetFileVersionInfoA, BOOL, LPCSTR, DWORD, DWORD, LPVOID );
    __typedef_func( GetFileVersionInfoByHandle, BOOL, size_t, HANDLE, size_t, size_t );
    __typedef_func( GetFileVersionInfoExA, BOOL, DWORD, LPCSTR, DWORD, DWORD, LPVOID );
    __typedef_func( GetFileVersionInfoExW, BOOL, DWORD, LPCWSTR, DWORD, DWORD, LPVOID );
    __typedef_func( GetFileVersionInfoSizeA, DWORD, LPCSTR, LPDWORD );
    __typedef_func( GetFileVersionInfoSizeExA, DWORD, DWORD, LPCSTR, LPDWORD );
    __typedef_func( GetFileVersionInfoSizeExW, DWORD, DWORD, LPCWSTR, LPDWORD );
    __typedef_func( GetFileVersionInfoSizeW, DWORD, LPCWSTR, LPDWORD );
    __typedef_func( GetFileVersionInfoW, BOOL, LPCWSTR, DWORD, DWORD, LPVOID );
    __typedef_func( VerFindFileA, DWORD, DWORD, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT, LPSTR, PUINT );
    __typedef_func( VerFindFileW, DWORD, DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT, LPWSTR, PUINT );
    __typedef_func( VerInstallFileA, DWORD, DWORD, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT );
    __typedef_func( VerInstallFileW, DWORD, DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT );
    __typedef_func( VerLanguageNameA, DWORD, DWORD, LPSTR, DWORD );
    __typedef_func( VerLanguageNameW, DWORD, DWORD, LPWSTR, DWORD );
    __typedef_func( VerQueryValueA, BOOL, LPCVOID, LPCSTR, LPVOID*, PUINT );
    __typedef_func( VerQueryValueW, BOOL, LPCVOID, LPCWSTR, LPVOID*, PUINT );

    /// <summary>
    /// Loads an original system library and resolves all the function procs
    /// </summary>
    /// <returns>Returns "true" upon success, "false" otherwise</returns>
    bool resolve()
    {
        m_versiondll = LoadLibraryW( m_version_path.c_str() );

        if (m_versiondll == nullptr)
            return false;

        __setup_proc( GetFileVersionInfoA );
        __setup_proc( GetFileVersionInfoByHandle );
        __setup_proc( GetFileVersionInfoExA );
        __setup_proc( GetFileVersionInfoExW );
        __setup_proc( GetFileVersionInfoSizeA );
        __setup_proc( GetFileVersionInfoSizeExA );
        __setup_proc( GetFileVersionInfoSizeExW );
        __setup_proc( GetFileVersionInfoSizeW );
        __setup_proc( GetFileVersionInfoW );
        __setup_proc( VerFindFileA );
        __setup_proc( VerFindFileW );
        __setup_proc( VerInstallFileA );
        __setup_proc( VerInstallFileW );
        __setup_proc( VerLanguageNameA );
        __setup_proc( VerLanguageNameW );
        __setup_proc( VerQueryValueA );
        __setup_proc( VerQueryValueW );

        return true;
    }
};