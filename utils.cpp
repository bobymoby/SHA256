#include "utils.hpp"
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