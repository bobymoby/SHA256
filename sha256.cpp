#include <iostream>
#include <string>

using namespace std;

unsigned int K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

unsigned int RightRotate(unsigned int word, unsigned int times)
{
    return (word >> times) | (word << (32 - times));
}

unsigned int Sigma0(unsigned int word)
{
    return RightRotate(word, 7) ^ RightRotate(word, 18) ^ (word >> 3);
}

unsigned int Sigma1(unsigned int word)
{
    return RightRotate(word, 17) ^ RightRotate(word, 19) ^ (word >> 10);
}

unsigned int CapitalSigma0(unsigned int hash)
{
    return RightRotate(hash, 2) ^ RightRotate(hash, 13) ^ RightRotate(hash, 22);
}

unsigned int CapitalSigma1(unsigned int hash)
{
    return RightRotate(hash, 6) ^ RightRotate(hash, 11) ^ RightRotate(hash, 25);
}

unsigned int Choice(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ ((~hash1) & hash3);
}

unsigned int Majority(unsigned int hash1, unsigned int hash2, unsigned int hash3)
{
    return (hash1 & hash2) ^ (hash1 & hash3) ^ (hash2 & hash3);
}

void Pad(string& message) {
    unsigned long long length = message.length() * 8;
    message += (char)0x80;
    while (message.length() % 64 != 56) {
        message += (char)0x00;
    }
    string bigEndianLength = "";
    for (int i = 0; i < 8; i++) {
        bigEndianLength += (char)((length >> (8 * (7 - i))) & 0xFF);
    }
    message += bigEndianLength;
}

void Transform(unsigned int Words[64], unsigned int Hash[8])
{
    for (int index = 16; index < 64; index++)
    {
        Words[index] = Words[index - 16] + Sigma0(Words[index - 15]) + Words[index - 7] + Sigma1(Words[index - 2]);
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

    Hash[0] += a;
    Hash[1] += b;
    Hash[2] += c;
    Hash[3] += d;
    Hash[4] += e;
    Hash[5] += f;
    Hash[6] += g;
    Hash[7] += h;

}

void Update(string& message, unsigned int Hash[8])
{
    for (int iterations = 0; iterations < message.size() / 64; iterations++)
    {
        unsigned int Words[64];
        for (int index = 0; index < 16; index++)
        {
            Words[index] = 0;
            for (int j = 0; j < 4; j++) {
                Words[index] = (Words[index] << 8) + (unsigned char)message[iterations * 64 + index * 4 + j];
            }
        }
        Transform(Words, Hash);
    }
}

void PrintHash(unsigned int Hash[8])
{
    for (int i = 0; i < 8; i++)
    {
        cout << hex << (Hash[i]);
    }
    cout << endl;
}

void SHA256(string input)
{
    unsigned int Hash[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
    Pad(input);
    Update(input, Hash);
    PrintHash(Hash);
}

int main()
{
    string in;
    getline(cin, in);

    SHA256(in);
    return 0;
}