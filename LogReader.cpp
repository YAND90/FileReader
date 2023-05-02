#include "LogReader.h"

#include "FileWrapper.h"
#include "wildcard.h"

#include <stdio.h>
#include <string.h>

CLogReader::CLogReader(CFileWrapper& fileWrapper)
    : m_Filter("*"), m_FileOffset(0), m_Remain(false), m_RemainLen(0), m_RemainOffset(0), m_Line(new char[LINE_BUFFER_SIZE]), m_LineSize(LINE_BUFFER_SIZE), m_FileOpened(false)
{
	m_FileWrapper = &fileWrapper;
}

CLogReader::~CLogReader()
{
    Close();
    delete[] m_Line;
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
	return true;
}

bool CLogReader::SetFilter(const char* filter)
{
	this->m_Filter = filter;
	return false;
}

bool CLogReader::GetNextChar(char* buf, const int size, char& c, bool& eof) {
    while (true) {
        if (m_Remain) {
            if (m_RemainOffset < m_RemainLen) {
                c = buf[m_RemainOffset];
                m_RemainOffset++;
                return true;
            }
        }

        unsigned int bytesRead = 0;

        if (!m_FileWrapper->read(buf, size, bytesRead, m_FileOffset)) {
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
        if (!GetNextChar(buf, bufSize, c, eof)) {
            return false;
        }

        if (eof) {
            break;
        }

        if (c == '\r' || c == '\n') {
            break;
        }

        ExtendLineBufferIfNeeded(write);

        m_Line[write] = c;
        ++write;
    }

    if (c == '\r') {
        GetNextChar(buf, bufSize, c, eof);
    }

    ExtendLineBufferIfNeeded(write);

    m_Line[write] = '\0';

    if (wildcard::match(m_Line, m_Filter)) {
        printf("Line: %s \n", m_Line);
        found = true;
    }

    if (eof) {
        return false;
    }

    return true;
}

bool CLogReader::GetNextLine(char* buf, const int bufSize)
{
    bool found = false;
    bool res = false;
    while (!found) {
        if (!GetNextLine(buf, bufSize, found)) {
            return false;
        }
    }

    return true;
}

void CLogReader::ExtendLineBufferIfNeeded(const unsigned int write) {
    if (write + 1 > m_LineSize) {
        char* tmp = new char[m_LineSize + LINE_BUFFER_SIZE];
        memcpy(tmp, m_Line, m_LineSize);
        m_LineSize += LINE_BUFFER_SIZE;
        delete[] m_Line;
        m_Line = tmp;
    }
}