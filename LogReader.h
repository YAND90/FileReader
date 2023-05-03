#ifndef _FILE_READER_LOG_READER_
#define _FILE_READER_LOG_READER_

class CFileWrapper;

class CLogReader
{
public:
	CLogReader(CFileWrapper& fileWrapper, const unsigned int& readBufferSize);
	~CLogReader();

	/*
	* Open file for scan
	*/
	bool Open(const char* fileName);
	/*
	* Close file
	*/
	bool Close();
	/*
	* Set pattern for scan
	*/
	bool SetFilter(const char* filter);
	/*
	* Get next line from the file which matches the pattern
	*/
	bool GetNextLine(char* buf, const int bufSize);

private:
	bool GetNextChar(char& c, bool& eof);
	bool CheckBufferBounds(const unsigned int write, const unsigned int bufSize);
	bool GetNextLine(char* buf, const int bufSize, bool& found);

	CFileWrapper* m_FileWrapper;
	unsigned int m_FileOffset;

	char* m_Filter;

	bool m_Remain;
	unsigned int m_RemainOffset;
	unsigned int m_RemainLen;

	bool m_FileOpened;

	char* m_ReaBuffer;
	unsigned int m_ReadBufferSize;
};

#endif


