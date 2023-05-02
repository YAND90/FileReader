#include "FileWrapper.h"

#include <Windows.h>
#include <stdio.h>

CFileWrapper::CFileWrapper()
    :m_Handle(INVALID_HANDLE_VALUE)
{

}

CFileWrapper::~CFileWrapper() 
{
    close();
}

bool CFileWrapper::open(const char* fileName) {
    return open(fileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);
}

bool CFileWrapper::open(const char* fileName, const unsigned int desiredAccess, const unsigned int sharedMode, const unsigned int creationDisposition, const unsigned int flags)
{
    if (m_Handle != INVALID_HANDLE_VALUE) {
        return false;
    }
    m_Handle = CreateFileA(fileName, desiredAccess, sharedMode, nullptr, creationDisposition, flags, nullptr);
    if (m_Handle == INVALID_HANDLE_VALUE) {
        unsigned int error = getError();
        printf("Failed to open file. Error code: %d", error);
        return false;
    }

    return true;
}

bool CFileWrapper::close()
{
    if (m_Handle == INVALID_HANDLE_VALUE) {
        return true;
    }
    BOOL result = CloseHandle(m_Handle);
    m_Handle = INVALID_HANDLE_VALUE;

    return result != FALSE;
}

bool CFileWrapper::read(void* buffer, const unsigned int size, unsigned int& bytesRead, const unsigned int offset)
{
    if (m_Handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    OVERLAPPED ov = {};
    ov.Offset = offset;

    return ReadFile(m_Handle, buffer, size, (LPDWORD)&bytesRead, &ov) != FALSE;
}


unsigned int CFileWrapper::getError() {
    return GetLastError();
}

bool CFileWrapper::isEOF(const unsigned int error) {
    return error == ERROR_HANDLE_EOF;
}
