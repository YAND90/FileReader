# FileReader
Simple file reader for Windows which extracts lines from the file only which matches the pattern

# Usage
FileReader.exe <file name> <pattern>

# Example

C:\Users\ilyayandaev\source\repos\FileReader\Debug>FileReader.exe test1.txt *
Started to scan file 'test1.txt' with pattern '*'

Line: Hello

Line: World

Line: My
Line: Name
Line: Is
Line: Ilya

Finished scan
C:\Users\ilyayandaev\source\repos\FileReader\Debug>FileReader.exe test1.txt H*
Started to scan file 'test1.txt' with pattern 'H*'

Line: Hello

Finished scan
C:\Users\ilyayandaev\source\repos\FileReader\Debug>FileReader.exe test1.txt *O*
Started to scan file 'test1.txt' with pattern '*O*'


Finished scan
C:\Users\ilyayandaev\source\repos\FileReader\Debug>FileReader.exe test1.txt *o*
Started to scan file 'test1.txt' with pattern '*o*'

Line: Hello
Line: World

Finished scan
C:\Users\ilyayandaev\source\repos\FileReader\Debug>FileReader.exe test1.txt ?lya
Started to scan file 'test1.txt' with pattern '?lya'

Line: Ilya

Finished scan

# Note
CFileWrapper could be replaced with another one for required OS for example and project could be run on that OS
