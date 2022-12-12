#include "sha256.hpp"
#include <sstream>
#include <iomanip>
SHA256::SHA256(std::string message)
{
    Pad(message);
    Update(message);
}

unsigned int SHA256::RightRotate(unsigned int word, unsigned int times)
{
    return (word >> times) | (word << (32 - times));
}

unsigned int SHA256::Sigma0(unsigned int word)
{
    return RightRotate(word, 7) ^ RightRotate(word, 18) ^ (word >> 3);
}

unsigned int SHA256::Sigma1(unsigned int word)
{
    return RightRotate(word, 17) ^ RightRotate(word, 19) ^ (word >> 10);
}

unsigned int SHA256::CapitalSigma0(unsigned int hash)
{
    return RightRotate(hash, 2) ^ RightRotate(hash, 13) ^ RightRotate(hash, 22);
}

unsigned int SHA256::CapitalSigma1(unsigned int hash)
{
    return RightRotate(hash, 6) ^ RightRotate(hash, 11) ^ RightRotate(hash, 25);
}

unsigned int SHA256::Choice(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ ((~hash1) & hash3);
}

unsigned int SHA256::Majority(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ (hash1 & hash3) ^ (hash2 & hash3);
}

void SHA256::Pad(std::string& message) {
    unsigned long long length = message.length() * 8;
    message += (uint8_t)128; //10000000 in binary
    while (message.length() % 64 != 56) {
        message += (char)0; //00000000 in binary
    }
    std::string bigEndianLength = "";
    for (int i = 0; i < 8; i++) {
        bigEndianLength += (uint8_t)(length >> (8 * (7 - i)));
    }
    message += bigEndianLength;
}

void SHA256::Transform(unsigned int Words[64])
{
    //Transform the message W[0..15] into 64 words using the SHA256 algorithm
    for (int index = 16; index < 64; index++)
    {
        Words[index] = Words[index - 16] + Sigma0(Words[index - 15]) + Words[index - 7] + Sigma1(Words[index - 2]);
    }
    //Working variables
    unsigned int a = this->Hash[0];
    unsigned int b = Hash[1];
    unsigned int c = Hash[2];
    unsigned int d = Hash[3];
    unsigned int e = Hash[4];
    unsigned int f = Hash[5];
    unsigned int g = Hash[6];
    unsigned int h = Hash[7];

    //Compression function main loop
    for (int i = 0; i < 64; i++)
    {
        unsigned int temp1 = h + CapitalSigma1(e) + Choice(e, f, g) + K[i] + Words[i];
        unsigned int temp2 = CapitalSigma0(a) + Majority(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    //Update the hash values with the compressed chunk
    Hash[0] += a;
    Hash[1] += b;
    Hash[2] += c;
    Hash[3] += d;
    Hash[4] += e;
    Hash[5] += f;
    Hash[6] += g;
    Hash[7] += h;

}

void SHA256::Update(std::string& message)
{
    for (int chunk = 0; chunk < message.size() / 64; chunk++)
    {
        unsigned int Words[64];
        for (int index = 0; index < 16; index++)
        {
            Words[index] = 0;
            //Words[index] will become |aaaaaaaabbbbbbbbccccccccdddddddd|
            //Where a,b,c,d are the bits of chunk[index], chunk[index + 1], chunk[index + 2], chunk[index + 3]
            //Where chunk is the current 64 byte chunk of the message
            for (int j = 0; j < 4; j++) {
                Words[index] = (Words[index] << 8) + (uint8_t)message[chunk * 64 + index * 4 + j];
                //By casting to uint8_t(unsigned char), we ensure that the value is exactly 1 unsigned byte
            }
        }
        Transform(Words);
    }
}

std::string SHA256::Digest()
{
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    for (int i = 0; i < 8; i++)
    {
        ss << std::setfill('0') << std::setw(8) << std::hex << (Hash[i]);
    }
    return ss.str();
}