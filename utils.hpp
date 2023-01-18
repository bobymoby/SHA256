#pragma once
namespace utils
{
    void DecToHex(unsigned int num, char hex[8]);
    int CeilDiv(int num, int den);

    int StrLen(char* str);
    char* Substring(char* str, int start, int length);
    bool StrComp(char* str1, char* str2);

    bool DoesFileExist(char* filename);
    char* ReadFile(char* filename);
    void WriteFile(char* filename, char* data);
}