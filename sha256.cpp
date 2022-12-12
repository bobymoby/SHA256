#include "sha256.hpp"
#include <sstream>
#include <iomanip>
SHA256::SHA256(std::string message)
{
    Pad(message);
    Update(message);
}

uint32_t SHA256::RightRotate(uint32_t word, uint32_t times)
{
    return (word >> times) | (word << (32 - times));
}

uint32_t SHA256::Sigma0(uint32_t word)
{
    return RightRotate(word, 7) ^ RightRotate(word, 18) ^ (word >> 3);
}

uint32_t SHA256::Sigma1(uint32_t word)
{
    return RightRotate(word, 17) ^ RightRotate(word, 19) ^ (word >> 10);
}

uint32_t SHA256::CapitalSigma0(uint32_t hash)
{
    return RightRotate(hash, 2) ^ RightRotate(hash, 13) ^ RightRotate(hash, 22);
}

uint32_t SHA256::CapitalSigma1(uint32_t hash)
{
    return RightRotate(hash, 6) ^ RightRotate(hash, 11) ^ RightRotate(hash, 25);
}

uint32_t SHA256::Choice(uint32_t hash1, uint32_t hash2, uint32_t hash3)
{
    return (hash1 & hash2) ^ ((~hash1) & hash3);
}

uint32_t SHA256::Majority(uint32_t hash1, uint32_t hash2, uint32_t hash3)
{
    return (hash1 & hash2) ^ (hash1 & hash3) ^ (hash2 & hash3);
}

//Add a single 1 bit to the end of the message and then
//0 bits until (the message length) % 64 = 56
//Then add the length of the message as a 8 byte big endian integer(with leading zeros if necessary)

//uint8_t(unsigned char) is used because it is the only type that is guaranteed to be 1 byte
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

//Compress the chunk of 64 bytes and update the hash values with the compressed chunk
void SHA256::Transform(uint32_t Words[64])
{
    //Transform the message W[0..15] into 64 words using the SHA256 algorithm
    for (int index = 16; index < 64; index++)
    {
        Words[index] = Words[index - 16] + Sigma0(Words[index - 15]) + Words[index - 7] + Sigma1(Words[index - 2]);
    }
    //Working variables
    uint32_t a = Hash[0];
    uint32_t b = Hash[1];
    uint32_t c = Hash[2];
    uint32_t d = Hash[3];
    uint32_t e = Hash[4];
    uint32_t f = Hash[5];
    uint32_t g = Hash[6];
    uint32_t h = Hash[7];

    //Compression function main loop
    for (int i = 0; i < 64; i++)
    {
        uint32_t temp1 = h + CapitalSigma1(e) + Choice(e, f, g) + K[i] + Words[i];
        uint32_t temp2 = CapitalSigma0(a) + Majority(a, b, c);
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

//Convert the message into 64 byte chunks and Transform with each chunk
void SHA256::Update(std::string& message)
{
    for (int chunk = 0; chunk < message.size() / 64; chunk++)
    {
        uint32_t Words[64];
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

//Convert to string
std::string SHA256::Digest()
{
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    for (int i = 0; i < 8; i++)
    {
        ss << std::setfill('0') << std::setw(8) << std::hex << (Hash[i]);
    }
    return ss.str();
}