#include "FileWrapper.h"
#include "LogReader.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("Incorrect usage of tool. Please, specify 2 args: 1 - file path, 2 - wildcard\n");
        return -1;
    }

    char* fileName = argv[1];
    char* pattern = argv[2];

    printf("Started to scan file '%s' with pattern '%s'\n\n", fileName, pattern);


    CFileWrapper fileWrapper;
    CLogReader logReader(fileWrapper);


    if (!logReader.Open(fileName)) {
        return -1;
    }

    logReader.SetFilter(pattern);

    char buf[1024];
    unsigned int size = 1024;

    while (logReader.GetNextLine(buf, size)) {
        continue;
    }

    printf("\nFinished scan");

    return 0;
}
