#pragma once

// MinHook (https://github.com/TsudaKageyu/minhook)
#include <minhook/include/MinHook.h>

typedef struct _FILE_BASIC_INFORMATION
{
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER AllocationSize;
	LARGE_INTEGER EndOfFile;
	ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

class CRedirector
{
private:

    // A directory to check for
    std::wstring m_mods_dir{ L"Mods" };

	// A new path to the mods
	fs::path m_mods_path{ fs::current_path() / m_mods_dir };

    // Resolve typedefs
    __typedef_func( NtCreateFile, NTSTATUS, PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG );
    __typedef_func( NtOpenFile, NTSTATUS, PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, ULONG, ULONG );
    __typedef_func( NtQueryFullAttributesFile, NTSTATUS, POBJECT_ATTRIBUTES, PFILE_NETWORK_OPEN_INFORMATION );

    // Get the relative path to the file/directory
    std::wstring relative( const std::wstring& obj_path )
    {
        if (!std::wcsstr( obj_path.c_str(), m_mods_dir.c_str() ))
            return std::wstring();

        return std::wstring( L"\\??\\") += 
            m_mods_path.wstring() += 
            obj_path.substr( obj_path.find( m_mods_dir ) + m_mods_dir.size() );
    }

    // 'NtCreateFile' API hook
    static NTSTATUS NTAPI hook_NtCreateFile(
      PHANDLE FileHandle,
      ACCESS_MASK DesiredAccess,
      POBJECT_ATTRIBUTES ObjectAttributes,
      PIO_STATUS_BLOCK IoStatusBlock,
      PLARGE_INTEGER AllocationSize,
      ULONG FileAttributes,
      ULONG ShareAccess,
      ULONG CreateDisposition,
      ULONG CreateOptions,
      PVOID EaBuffer,
      ULONG EaLength
    );

    // 'NtOpenFile' API hook
    static NTSTATUS NTAPI hook_NtOpenFile(
      PHANDLE FileHandle,
      ACCESS_MASK DesiredAccess,
      POBJECT_ATTRIBUTES ObjectAttributes,
      PIO_STATUS_BLOCK IoStatusBlock,
      ULONG ShareAccess,
      ULONG OpenOptions
    );

    // 'NtQueryFullAttributesFile' API hook
    static NTSTATUS NTAPI hook_NtQueryFullAttributesFile(
      _In_ POBJECT_ATTRIBUTES ObjectAttributes,
      _Out_ PFILE_NETWORK_OPEN_INFORMATION FileInformation
    );

public:

    CRedirector()
    {
        // Try reading the 'mods_path.txt' to obtain the full path to the mods directory
        std::wfstream stream( fs::current_path() / "mods_path.txt" );

        if (stream.good() || !stream.fail())
        {
            if (stream.tellg() > 0x0)
            {
                std::wstring mods_path;
                std::getline( stream, mods_path );

                if (!mods_path.empty())
                    m_mods_path = mods_path;
            }
        }
    }

    ~CRedirector()
    {
        __clear_proc( NtCreateFile );
        __clear_proc( NtOpenFile );
        __clear_proc( NtQueryFullAttributesFile );
    }

    /// <summary>
    /// Gets the current 'CRedirector' class instance
    /// </summary>
    static CRedirector& instance()
    {
        static CRedirector instance;
        return instance;
    }

    /// <summary>
    /// Setup all the required API hooks for redirection
    /// </summary>
    /// <returns>Returns "true" upon success, "false" otherwise</returns>
    bool setup_hooks()
    {
        // Check if the target exists
        if (!fs::exists( m_mods_path ))
            return false;

        // Check if the target is a real directory
        if (!fs::is_directory( m_mods_path ))
            return false;

        // Check if directory is not empty
        if (fs::is_empty( m_mods_path ))
            return false;

        MH_STATUS status{ MH_ERROR_NOT_INITIALIZED };

        status = MH_Initialize();

        if (status != MH_STATUS::MH_OK)
            return false;

        LPVOID target;

        // Create and enable 'NtCreateFile' API hook
        status = MH_CreateHookApiEx(
            L"ntdll.dll",
            "NtCreateFile",
            &hook_NtCreateFile,
            reinterpret_cast<void**>(&m_NtCreateFile), &target );

        if (status != MH_STATUS::MH_OK)
            return false;

        status = MH_QueueEnableHook( target );
        if (status != MH_STATUS::MH_OK)
            return false;


        // Create and enable 'NtOpenFile' API hook
        status = MH_CreateHookApiEx(
            L"ntdll.dll",
            "NtOpenFile",
            &hook_NtOpenFile,
            reinterpret_cast<void**>(&m_NtOpenFile), &target );

        if (status != MH_STATUS::MH_OK)
            return false;

        status = MH_QueueEnableHook( target );
        if (status != MH_STATUS::MH_OK)
            return false;


        // Create and enable 'NtQueryFullAttributesFile' API hook
        status = MH_CreateHookApiEx(
           L"ntdll.dll",
           "NtQueryFullAttributesFile",
           &hook_NtQueryFullAttributesFile,
           reinterpret_cast<void**>(&m_NtQueryFullAttributesFile), &target );

        if (status != MH_STATUS::MH_OK)
            return false;

        status = MH_QueueEnableHook( target );
        if (status != MH_STATUS::MH_OK)
            return false;


        // Apply all the previously created hooks
        status = MH_ApplyQueued();
        if (status != MH_STATUS::MH_OK)
            return false;

        return true;
    }
};