#include "LogReader.h"

#include "FileWrapper.h"
#include "wildcard.h"

#include <stdio.h>
#include <string.h>

CLogReader::CLogReader(CFileWrapper& fileWrapper, const unsigned int& readBufferSize)
    : m_Filter(nullptr), m_FileOffset(0), m_Remain(false), m_RemainLen(0), m_RemainOffset(0), m_FileOpened(false)
{
    m_ReadBufferSize = readBufferSize;
    m_ReaBuffer = new char[m_ReadBufferSize];
	m_FileWrapper = &fileWrapper;
}

CLogReader::~CLogReader()
{
    Close();
    delete[] m_Filter;
    delete[] m_ReaBuffer;
}

bool CLogReader::Open(const char* fileName)
{
    if (m_FileOpened) {
        printf("Trying to open new file, when another one is in use\n");
        return false;
    }

	if (!m_FileWrapper->open(fileName)) {
		return false;
	}

    m_FileOpened = true;
	return true;
}

bool CLogReader::Close()
{
	if (!m_FileWrapper->close()) {
		return false;
	}
    m_FileOpened = false;
	return true;
}

bool CLogReader::SetFilter(const char* filter)
{
    unsigned int len = strlen(filter);

    if (this->m_Filter) {
        delete[] this->m_Filter;
    }

    this->m_Filter = new char[len + 1];
    strcpy_s(this->m_Filter, len + 1, filter);
	return true;
}

bool CLogReader::GetNextChar(char& c, bool& eof) {
    while (true) {
        if (m_Remain) {
            if (m_RemainOffset < m_RemainLen) {
                c = m_ReaBuffer[m_RemainOffset];
                m_RemainOffset++;
                return true;
            }
        }

        unsigned int bytesRead = 0;

        if (!m_FileWrapper->read(m_ReaBuffer, m_ReadBufferSize, bytesRead, m_FileOffset)) {
            unsigned int errorCode = m_FileWrapper->getError();

            if (m_FileWrapper->isEOF(errorCode)) {
                eof = true;
                return true;
            }

            printf("Failed to get next line. Error: %d\n", errorCode);
            return false;
        }

        if (bytesRead == 0) {
            eof = true;
            return true;
        }

        m_FileOffset += bytesRead;

        m_Remain = true;
        m_RemainOffset = 0;
        m_RemainLen = bytesRead;
    }
}

bool CLogReader::GetNextLine(char* buf, const int bufSize, bool& found) {
    char c = 0;
    unsigned int write = 0;
    bool eof = false;
    found = false;

    while (true) {
        if (!GetNextChar(c, eof)) {
            return false;
        }

        if (eof) {
            break;
        }

        if (c == '\r' || c == '\n') {
            break;
        }

        if (!CheckBufferBounds(write, bufSize)) {
            return false;
        }

        buf[write] = c;
        ++write;
    }

    if (c == '\r') {
        GetNextChar(c, eof);
    }

    if (!CheckBufferBounds(write, bufSize)) {
        return false;
    }

    buf[write] = '\0';

    if (write > 0 && wildcard::match(buf, m_Filter)) {
        found = true;
    }

    if (eof) {
        return false;
    }

    return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufSize)
{
    if (!m_Filter) {
        printf("Filter was not set for reading\n");
        return false;
    }

    if (!m_FileOpened) {
        printf("File was not set for reading\n");
        return false;
    }

    bool found = false;
    bool res = false;
    while (!found) {
        if (!GetNextLine(buf, bufSize, found)) {
            if (found) {
                return true;
            }
            return false;
        }
    }

    return true;
}

bool CLogReader::CheckBufferBounds(const unsigned int write, const unsigned int bufSize) {
    if (write + 1 > bufSize) {
        printf("Input buffer to short for returning the line. Line size is greater then %d bytes", bufSize);
        return false;
    }

    return true;
}