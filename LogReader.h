#ifndef _FILE_READER_LOG_READER_
#define _FILE_READER_LOG_READER_

class CFileWrapper;

class CLogReader
{
public:
	CLogReader(CFileWrapper& fileWrapper);
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
	static const int LINE_BUFFER_SIZE = 1024;

	bool GetNextChar(char* buf, const int size, char& c, bool& eof);
	void ExtendLineBufferIfNeeded(const unsigned int write);
	bool GetNextLine(char* buf, const int bufSize, bool& found);

	CFileWrapper* m_FileWrapper;
	unsigned int m_FileOffset;

	const char* m_Filter;

	bool m_Remain;
	unsigned int m_RemainOffset;
	unsigned int m_RemainLen;

	char* m_Line;
	unsigned int m_LineSize;

	bool m_FileOpened;
};

#endif


