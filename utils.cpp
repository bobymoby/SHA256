#include "utils.hpp"
#include <fstream>

//debug
#include <iostream>

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
        int temp = 0;
        temp = dec % 16;
        if (temp < 10)
        {
            hex[7 - index] = temp + '0';
            index++;
        }
        else
        {
            hex[7 - index] = temp + 'a' - 10;
            index++;
        }
        dec = dec / 16;
    }
}
unsigned int utils::RightRotate(unsigned int word, unsigned int times)
{
    return (word >> times) | (word << (32 - times));
}

unsigned int utils::Sigma0(unsigned int word)
{
    return RightRotate(word, 7) ^ RightRotate(word, 18) ^ (word >> 3);
}

unsigned int utils::Sigma1(unsigned int word)
{
    return RightRotate(word, 17) ^ RightRotate(word, 19) ^ (word >> 10);
}

unsigned int utils::CapitalSigma0(unsigned int hash)
{
    return RightRotate(hash, 2) ^ RightRotate(hash, 13) ^ RightRotate(hash, 22);
}

unsigned int utils::CapitalSigma1(unsigned int hash)
{
    return RightRotate(hash, 6) ^ RightRotate(hash, 11) ^ RightRotate(hash, 25);
}

unsigned int utils::Choice(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ ((~hash1) & hash3);
}

unsigned int utils::Majority(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ (hash1 & hash3) ^ (hash2 & hash3);
}

int utils::StrLen(char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

char* utils::Substring(char* str, int start, int length)
{
    char* substr = new char[length + 1];
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
    if (len1 != len2)
    {
        return false;
    }
    for (int i = 0; i < len1; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

char* ConvertFileName(char* filename)
{
    int len = utils::StrLen(filename);
    char* substr = utils::Substring(filename, len - 4, 4);
    if (utils::StrComp(substr, (char*)".txt"))
    {
        delete[] substr;
        char* dynamicFileName = new char[len + 1]; //dynamic cast
        for (int i = 0; i < len; i++)
        {
            dynamicFileName[i] = filename[i];
        }
        dynamicFileName[len] = '\0';
        return dynamicFileName;
    }
    else
    {
        delete[] substr;
        char* newfilename = new char[len + 5];
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
}

bool utils::DoesFileExist(char* filename)
{
    char* newfilename = ConvertFileName(filename);
    // std::cout << "Debug fn:" << newfilename << std::endl;
    std::ifstream file(newfilename);
    bool isGood = file.good();
    file.close();
    delete[] newfilename;
    return isGood;
}

char* utils::ReadFile(char* filename)
{
    char* newfilename = ConvertFileName(filename);
    // std::cout << "Debug fn:" << newfilename << std::endl;
    if (!utils::DoesFileExist(newfilename))
    {
        return nullptr;
    }
    // std::cout << "Debug file is good" << std::endl;
    std::ifstream file(newfilename);
    delete[] newfilename;
    // std::cout << "Debug file is open" << std::endl;
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    // std::cout << "Debug length:" << length << std::endl;
    char* result = new char[length + 1];
    file.seekg(0, std::ios::beg);
    file.read(result, length);
    file.close();
    result[length] = '\0';
    return result;
}

void utils::WriteFile(char* filename, char* data)
{
    //overwrite file
    char* newfilename = ConvertFileName(filename);
    std::ofstream file(newfilename);
    delete[] newfilename;
    file << data;
    file.close();
}