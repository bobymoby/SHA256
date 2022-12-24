#include <iostream>
#include "utils.hpp"

const int DIGEST_SIZE = 65;
const int MAX_INPUT_SIZE = 100;

void Pad(const char* message, unsigned char*& binMessage, unsigned long long& length) {
    unsigned long long msgLength = 0;
    while (message[msgLength] != '\0') msgLength++;
    msgLength *= 8;
    length = ((msgLength + 65) / 512 + 1) * 64;
    int index = 0;
    binMessage = new unsigned char[length];
    while (message[index] != '\0')
    {
        binMessage[index] = message[index];
        index++;
    }
    binMessage[index] = (unsigned char)128;
    index++;
    while (index % 64 != 56)
    {
        binMessage[index] = (unsigned char)0;
        index++;
    }
    for (int i = 0; i < 8; i++)
    {
        binMessage[index + i] = (unsigned char)(msgLength >> (8 * (7 - i)));
    }
}

void Transform(unsigned int Words[64], unsigned int Hash[8])
{
    for (int index = 16; index < 64; index++)
    {
        Words[index] = Words[index - 16] + utils::Sigma0(Words[index - 15])
            + Words[index - 7] + utils::Sigma1(Words[index - 2]);
    }
    unsigned int a = Hash[0];
    unsigned int b = Hash[1];
    unsigned int c = Hash[2];
    unsigned int d = Hash[3];
    unsigned int e = Hash[4];
    unsigned int f = Hash[5];
    unsigned int g = Hash[6];
    unsigned int h = Hash[7];

    for (int i = 0; i < 64; i++)
    {
        unsigned int temp1 = h + utils::CapitalSigma1(e) + utils::Choice(e, f, g) + utils::K[i] + Words[i];
        unsigned int temp2 = utils::CapitalSigma0(a) + utils::Majority(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    Hash[0] += a;
    Hash[1] += b;
    Hash[2] += c;
    Hash[3] += d;
    Hash[4] += e;
    Hash[5] += f;
    Hash[6] += g;
    Hash[7] += h;

}

void Update(unsigned char* binMessage, unsigned long long length, unsigned int Hash[8])
{
    for (int chunk = 0; chunk < length / 64; chunk++)
    {
        unsigned int Words[64];
        for (int index = 0; index < 16; index++)
        {
            Words[index] = 0;

            for (int j = 0; j < 4; j++) {
                Words[index] = (Words[index] << 8) + binMessage[chunk * 64 + index * 4 + j];
            }
        }
        Transform(Words, Hash);
    }
}

void SHA256(char* message, char digest[DIGEST_SIZE])
{
    unsigned int Hash[8];
    for (int i = 0; i < 8; i++)
    {
        Hash[i] = utils::Hash[i];
    }
    unsigned char* binMessage;
    unsigned long long length;
    Pad(message, binMessage, length);
    Update(binMessage, length, Hash);

    for (int i = 0; i < 8; i++)
    {
        char hex[8];
        utils::DecToHex(Hash[i], hex);
        for (int j = 0; j < 8; j++)
        {
            digest[i * 8 + j] = hex[j];
        }
    }
    digest[64] = '\0';
}

bool Verify(char* message, char* hash)
{
    char digest[DIGEST_SIZE];
    SHA256(message, digest);
    for (int i = 0; i < 64; i++)
    {
        if (digest[i] != hash[i])
        {
            return false;
        }
    }
    return true;
}
int main()
{
    std::cout << "Do you wish to:" << std::endl;
    std::cout << "1. Hash a file" << std::endl;
    std::cout << "2. Verify hash" << std::endl;
    int choice = 0;
    std::cin >> choice;
    std::cin.ignore();
    if (choice < 1 || choice > 2)
    {
        std::cout << "Invalid choice" << std::endl;
    }
    if (choice == 1)
    {
        std::cout << "Enter the path of the source file:" << std::endl;
        char source[MAX_INPUT_SIZE];
        std::cin.getline(source, MAX_INPUT_SIZE);
        char hash[DIGEST_SIZE];
        char* message = utils::ReadFile(source);
        std::cout << "Message to be hashed: " << message << std::endl;
        SHA256(message, hash);
        std::cout << "Hashed message: " << hash << std::endl;
        std::cout << "Enter the path of the destination file:" << std::endl;
        char destination[MAX_INPUT_SIZE];
        std::cin.getline(destination, MAX_INPUT_SIZE);
        utils::WriteFile(destination, hash);
    }
    if (choice == 2)
    {
        std::cout << "Enter the path of the hash file:" << std::endl;
        char source[MAX_INPUT_SIZE];
        std::cin.getline(source, MAX_INPUT_SIZE);
        char* hash = utils::ReadFile(source);
        std::cout << "Enter the original message:" << std::endl;
        char message[MAX_INPUT_SIZE];
        std::cin.getline(message, MAX_INPUT_SIZE);
        if (Verify(message, hash))
        {
            std::cout << "The hash is valid" << std::endl;
        }
        else
        {
            std::cout << "The hash is invalid" << std::endl;
        }
    }
    return 0;
}