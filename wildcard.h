#ifndef _FILE_READER_WILDCARD_
#define _FILE_READER_WILDCARD_

namespace wildcard {
	/*
	* Function checks that string matches the pattern. 
	* Supports the following wildcard operators: ?, *.
	* If you need to use ?, * symbols then you need to escape them using '\\'(for example: *abs\*?)
	* Args:
	* - string - string for matching
	* - pattern - wildcard string
	* Return:
	* true if string matches the pattern
	*/
	bool match(const char* string, const char* pattern);
}

#endif