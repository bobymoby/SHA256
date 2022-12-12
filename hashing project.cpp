#include <iostream>
#include "sha256.hpp"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

const string banner = "   _____ _    _          ___  _____   __  \n  / ____| |  | |   /\\   |__ \\| ____| / /  \n | (___ | |__| |  /  \\     ) | |__  / /_  \n  \\___ \\|  __  | / /\\ \\   / /|___ \\| '_ \\ \n  ____) | |  | |/ ____ \\ / /_ ___) | (_) |\n |_____/|_|  |_/_/    \\_\\____|____/ \\___/";

string ReadFile(string src)
{
    std::ifstream file(src);
    std::string msg((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return msg;
}

void WriteFile(string dest, string msg)
{
    std::ofstream file(dest);
    file << msg;
    file.close();
}

inline void ClearScreen()
{
    system("cls");
    cout << banner << endl;
}

void HashStringScreen()
{
    ClearScreen();
    string msg;
    cout << "Enter the string to hash: ";
    cin.ignore();
    getline(cin, msg);
    SHA256 sha(msg);
    string digest = sha.Digest();
    cout << "Hash: " << digest << endl;
    cout << "Do you want to save the hash? (y/n): ";
    char choice = 'a';
    while (choice != 'y' && choice != 'n')
    {
        cin >> choice;
    }
    if (choice == 'y')
    {
        string dest;
        cout << "Enter the destination file: ";
        cin.ignore();
        getline(cin, dest);

        WriteFile(dest, digest);
    }
}

void HashFileString()
{
    ClearScreen();
    string src;
    cout << "Enter the source file: ";
    cin.ignore();
    getline(cin, src);
    string msg = ReadFile(src);
    SHA256 sha(msg);
    string digest = sha.Digest();
    cout << "Hash: " << digest << endl;
    cout << "Do you want to save the hash? (y/n): ";
    char choice = 'a';
    while (choice != 'y' && choice != 'n')
    {
        cin >> choice;
    }
    if (choice == 'y')
    {
        string dest;
        cout << "Enter the destination file: ";
        cin.ignore();
        getline(cin, dest);

        WriteFile(dest, digest);
    }
}

void ReadHashScreen()
{
    ClearScreen();
    string src;
    cout << "Enter the source file: ";
    cin.ignore();
    getline(cin, src);
    string msg = ReadFile(src);
    cout << "Hash: " << msg << endl;
}

void LandingScreen()
{
    ClearScreen();
    cout << "1. Hash a string" << endl;
    cout << "2. Hash a file" << endl;
    cout << "3. Read saved hash" << endl;
    cout << "4. Exit" << endl;

    int action = 0;
    while (action < 1 || action > 4)
    {
        cout << "Enter your choice: ";
        cin >> action;
    }
    switch (action)
    {
    case 1:
        HashStringScreen();
        break;
    case 2:
        HashFileString();
        break;
    case 3:
        ReadHashScreen();
        break;
    }
}

int main()
{
    // // cout << banner << endl;
    // SHA256 sha("as");
    // cout << sha.Digest() << endl;
    LandingScreen();
    return 0;
}