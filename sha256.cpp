#include "sha256.hpp"
#include "utils.hpp"

//Functions defined by the SHA256 algorithm
unsigned int sha256::RightRotate(unsigned int word, unsigned int times)
{
    return (word >> times) | (word << (32 - times));
}

unsigned int sha256::Sigma0(unsigned int word)
{
    return RightRotate(word, 7) ^ RightRotate(word, 18) ^ (word >> 3);
}

unsigned int sha256::Sigma1(unsigned int word)
{
    return RightRotate(word, 17) ^ RightRotate(word, 19) ^ (word >> 10);
}

unsigned int sha256::CapitalSigma0(unsigned int hash)
{
    return RightRotate(hash, 2) ^ RightRotate(hash, 13) ^ RightRotate(hash, 22);
}

unsigned int sha256::CapitalSigma1(unsigned int hash)
{
    return RightRotate(hash, 6) ^ RightRotate(hash, 11) ^ RightRotate(hash, 25);
}

unsigned int sha256::Choice(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ ((~hash1) & hash3);
}

unsigned int sha256::Majority(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ (hash1 & hash3) ^ (hash2 & hash3);
}

//The sha256 algorithm requires the messenge to be padded to a multiple of 512 bits
//The padding is done by adding a 1 bit, followed by as many 0 bits as necessary
//The message length is appended to the end of the message in 64 bits
unsigned char* sha256::Pad(char* message, unsigned int& length)
{

    unsigned int msgLength = utils::StrLen(message);
    unsigned long long bitLength = msgLength * 8;

    length = utils::CeilDiv(bitLength + 64 + 8, 512) * 64;
    //bitLength + 64(for the length) + 8(for the 1 bit)
    //512 - chunk size
    //*64 - convert to bytes 

    unsigned char* paddedMessage = new unsigned char[length];
    int index = 0;

    //Copy the message into the padded message
    while (index < msgLength)
    {
        paddedMessage[index] = (unsigned char)message[index];
        index++;
    }

    //Add the 1 bit
    paddedMessage[index] = (unsigned char)128; //10000000 in binary
    index++;

    //Add the 0 bits
    while (index % 64 != 56)
    {
        paddedMessage[index] = (unsigned char)0;
        index++;
    }

    //Add the length
    for (int i = 0; i < 8; i++)
    {
        paddedMessage[index + i] = (unsigned char)(bitLength >> (8 * (7 - i)));
    }


    return paddedMessage;
}

//Using the sha256 algorithm update the hash with a 64 byte chunk of the message
void sha256::Update(unsigned int Words[64], unsigned int Hash[8])
{
    for (int index = 16; index < 64; index++)
    {
        Words[index] = Words[index - 16];
        Words[index] += sha256::Sigma0(Words[index - 15]);
        Words[index] += Words[index - 7];
        Words[index] += sha256::Sigma1(Words[index - 2]);
    }
    unsigned int a = Hash[0];
    unsigned int b = Hash[1];
    unsigned int c = Hash[2];
    unsigned int d = Hash[3];
    unsigned int e = Hash[4];
    unsigned int f = Hash[5];
    unsigned int g = Hash[6];
    unsigned int h = Hash[7];

    //Main hashing loop
    for (int i = 0; i < 64; i++)
    {
        unsigned int temp1, temp2;
        temp1 = h;
        temp1 += sha256::CapitalSigma1(e);
        temp1 += sha256::Choice(e, f, g);
        temp1 += sha256::K[i];
        temp1 += Words[i];

        temp2 = sha256::CapitalSigma0(a) + sha256::Majority(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    //Update the current hash values
    Hash[0] += a;
    Hash[1] += b;
    Hash[2] += c;
    Hash[3] += d;
    Hash[4] += e;
    Hash[5] += f;
    Hash[6] += g;
    Hash[7] += h;

}

//Transform the padded message into 64 byte chunks and update the hash with each chunk
//An array with size of 64 is used to store the Words for each chunk
//A Word is 4 bytes long and there are 16 Words in each chunk
//The rest of the Words are calculated from the previous Words using the sha256 algorithm
void sha256::Transform(unsigned char* paddedMessage, unsigned int length, unsigned int Hash[8])
{
    for (int chunk = 0; chunk < length / 64; chunk++)
    {
        unsigned int Words[64];
        for (int wordIndex = 0; wordIndex < 16; wordIndex++)
        {
            Words[wordIndex] = 0;

            for (int j = 0; j < 4; j++)
            {
                Words[wordIndex] = (Words[wordIndex] << 8) + paddedMessage[chunk * 64 + wordIndex * 4 + j];
            }
        }
        sha256::Update(Words, Hash);
    }
}

void sha256::SHA256(char* message, char digest[DIGEST_SIZE])
{
    unsigned int Hash[8];
    //Set the initial hash values
    for (int i = 0; i < 8; i++)
    {
        Hash[i] = sha256::Hash[i];
    }

    unsigned int paddedLength;
    unsigned char* paddedMessage = sha256::Pad(message, paddedLength);

    sha256::Transform(paddedMessage, paddedLength, Hash);

    for (int i = 0; i < 8; i++)
    {
        char hex[8];
        utils::DecToHex(Hash[i], hex);

        for (int j = 0; j < 8; j++)
        {
            digest[i * 8 + j] = hex[j];
        }
    }
    digest[DIGEST_SIZE - 1] = '\0';
}

//Verify that a hash has came from a given message
bool sha256::Verify(char* message, char* hash)
{
    char digest[DIGEST_SIZE];
    sha256::SHA256(message, digest);

    return utils::StrComp(digest, hash);
}