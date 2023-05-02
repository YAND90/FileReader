#ifndef _FILE_READER_FILE_WRAPPER_
#define _FILE_READER_FILE_WRAPPER_

/*
* File wrapper for WinApi functionality which helps to open/read/close files
*/
class CFileWrapper
{
public:
	CFileWrapper();
	~CFileWrapper();
	/*
	* Opens existing file on FS for reading
	*/
	bool open(const char* fileName);
	/*
	* Opens file on FS
	*/
	bool open(const char* fileName, const unsigned int desiredAccess, const unsigned int sharedMode, const unsigned int creationDisposition, const unsigned int flags);
	/*
	* Closes file
	*/
	bool close();
	/*
	* Reads file from specified offset
	*/
	bool read(void* buffer, const unsigned int size, unsigned int& bytesRead, const unsigned int offset);
	/*
	* Reads the last error from OS
	*/
	unsigned int getError();
	/*
	* Checks if error is EOF
	*/
	bool isEOF(const unsigned int error);
private:
	void* m_Handle;
};

#endif // !_FILE_READER_FILE_WRAPPER_

