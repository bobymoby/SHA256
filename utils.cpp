#include "utils.hpp"
#include <fstream>

void SetHexToZero(char hex[8])
{
    for (int i = 0; i < 8; i++)
    {
        hex[i] = '0';
    }
}

void utils::DecToHex(unsigned int dec, char hex[8])
{
    SetHexToZero(hex);

    int index = 0;
    while (dec > 0)
    {
        int temp = dec % 16;
        if (temp < 10)
        {
            hex[7 - index] = temp + '0';
        }
        else
        {
            hex[7 - index] = temp - 10 + 'a';
        }
        index++;
        dec = dec / 16;
    }
}

int utils::CeilDiv(int num, int den)
{
    return num / den + (num % den != 0);
}

int utils::StrLen(char* str)
{
    int len = 0;
    while (str[len] != '\0') len++;

    return len;
}

char* utils::Substring(char* str, int start, int length)
{
    char* substr = new char[length + 1]; // length + 1 for null terminator

    for (int i = 0; i < length; i++)
    {
        substr[i] = str[start + i];
    }
    substr[length] = '\0';

    return substr;
}

bool utils::StrComp(char* str1, char* str2)
{
    int len1 = utils::StrLen(str1);
    int len2 = utils::StrLen(str2);

    // If the lengths are not equal, the strings are not equal
    if (len1 != len2)
    {
        return false;
    }

    // Compares every character in the strings
    for (int i = 0; i < len1; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }

    return true;
}

// Adds .txt to the end of the filename if it does not already have it
char* ConvertFileName(char* filename)
{
    int len = utils::StrLen(filename);
    char* substr = utils::Substring(filename, len - 4, 4);

    bool shouldAddTxt = !utils::StrComp(substr, (char*)".txt");
    delete[] substr;

    if (shouldAddTxt)
    {
        char* newfilename = new char[len + 5]; // + 4 for .txt + 1 for null terminator

        for (int i = 0; i < len; i++)
        {
            newfilename[i] = filename[i];
        }
        newfilename[len] = '.';
        newfilename[len + 1] = 't';
        newfilename[len + 2] = 'x';
        newfilename[len + 3] = 't';
        newfilename[len + 4] = '\0';

        return newfilename;
    }
    else
    {
        //If the file already ends with .txt, cast it to dynamic memory
        //It will be easier to work with it later
        char* dynamicFileName = new char[len + 1]; // + 1 for null terminator

        for (int i = 0; i < len; i++)
        {
            dynamicFileName[i] = filename[i];
        }
        dynamicFileName[len] = '\0';

        return dynamicFileName;
    }
}

bool utils::DoesFileExist(char* filename)
{
    char* newfilename = ConvertFileName(filename);
    std::ifstream file(newfilename);

    bool isGood = file.good();

    file.close();
    delete[] newfilename;

    return isGood;
}

char* utils::ReadFile(char* filename)
{
    char* convertedFileName = ConvertFileName(filename);

    if (!utils::DoesFileExist(convertedFileName))
    {
        delete[] convertedFileName;
        return nullptr;
    }

    std::ifstream file(convertedFileName);

    delete[] convertedFileName;

    file.seekg(0, std::ios::end); // Moves the 'cursor' to the end of the file
    int length = file.tellg(); // Gets the position of the 'cursor'

    char* result = new char[length + 1];

    file.seekg(0, std::ios::beg); // Moves the 'cursor' to the beginning of the file
    file.read(result, length);

    result[length] = '\0';

    file.close();
    return result;
}

void utils::WriteFile(char* filename, char* data)
{
    char* newfilename = ConvertFileName(filename);
    std::ofstream file(newfilename);

    file << data;

    delete[] newfilename;
    file.close();
}