#include "pch.h"

NTSTATUS NTAPI CRedirector::hook_NtCreateFile(
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
)
{
    if (ObjectAttributes->ObjectName)
    {
        std::wstring obj_name( ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length / sizeof( wchar_t ) );
        auto new_path = instance().relative( obj_name );

        if (!new_path.empty())
        {
            UNICODE_STRING unicode_str{};
            unicode_str.Buffer = new_path.data();
            unicode_str.Length = (USHORT)(new_path.size() * sizeof( wchar_t ));
            unicode_str.MaximumLength = unicode_str.Length + sizeof( wchar_t );

            OBJECT_ATTRIBUTES obj_attributes = *ObjectAttributes;
            obj_attributes.ObjectName = &unicode_str;

            return instance().m_NtCreateFile( FileHandle, DesiredAccess, &obj_attributes, IoStatusBlock, AllocationSize, FileAttributes,
                ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength );
        }
    }

    return instance().m_NtCreateFile( FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes,
        ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength );
}


NTSTATUS NTAPI CRedirector::hook_NtOpenFile(
  PHANDLE FileHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  PIO_STATUS_BLOCK IoStatusBlock,
  ULONG ShareAccess,
  ULONG OpenOptions
)
{
    if (ObjectAttributes->ObjectName)
    {
        std::wstring obj_name( ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length / sizeof( wchar_t ) );
        auto new_path = instance().relative( obj_name );

        if (!new_path.empty())
        {
            UNICODE_STRING unicode_str{};
            unicode_str.Buffer = new_path.data();
            unicode_str.Length = (USHORT)(new_path.size() * sizeof( wchar_t ));
            unicode_str.MaximumLength = unicode_str.Length + sizeof( wchar_t );

            OBJECT_ATTRIBUTES obj_attributes = *ObjectAttributes;
            obj_attributes.ObjectName = &unicode_str;

            return instance().m_NtOpenFile( FileHandle, DesiredAccess, &obj_attributes, IoStatusBlock,
                ShareAccess, OpenOptions );
        }
    }

    return instance().m_NtOpenFile( FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock,
        ShareAccess, OpenOptions );
}


NTSTATUS NTAPI CRedirector::hook_NtQueryFullAttributesFile(
  _In_ POBJECT_ATTRIBUTES ObjectAttributes,
  _Out_ PFILE_NETWORK_OPEN_INFORMATION FileInformation
)
{
    if (ObjectAttributes->ObjectName)
    {
        std::wstring obj_name( ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length / sizeof( wchar_t ) );
        auto new_path = instance().relative( obj_name );

        if (!new_path.empty())
        {
            UNICODE_STRING unicode_str{};
            unicode_str.Buffer = new_path.data();
            unicode_str.Length = (USHORT)(new_path.size() * sizeof( wchar_t ));
            unicode_str.MaximumLength = unicode_str.Length + sizeof( wchar_t );

            OBJECT_ATTRIBUTES obj_attributes = *ObjectAttributes;
            obj_attributes.ObjectName = &unicode_str;

            return instance().m_NtQueryFullAttributesFile( &obj_attributes, FileInformation );
        }
    }

    return instance().m_NtQueryFullAttributesFile( ObjectAttributes, FileInformation );
}