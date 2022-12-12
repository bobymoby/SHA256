#include <iostream>
#include "sha256.hpp"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <windows.h>

const std::string banner = "   _____ _    _          ___  _____   __  \n  / ____| |  | |   /\\   |__ \\| ____| / /  \n | (___ | |__| |  /  \\     ) | |__  / /_  \n  \\___ \\|  __  | / /\\ \\   / /|___ \\| '_ \\ \n  ____) | |  | |/ ____ \\ / /_ ___) | (_) |\n |_____/|_|  |_/_/    \\_\\____|____/ \\___/\n                       By Borimir Georgiev";

std::string ReadFile(std::string src)
{
    std::ifstream file(src);
    std::string msg((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return msg;
}

void WriteFile(std::string dest, std::string msg)
{
    std::ofstream file(dest);
    file << msg;
    file.close();
}

inline void ClearScreen()
{
    system("cls");
    std::cout << banner << std::endl;
}

void SavePrompt(std::string digest)
{
    std::cout << "Do you want to save the hash? (y/n): ";
    char choice = 'a';
    while (choice != 'y' && choice != 'n')
    {
        std::cin >> choice;
    }
    if (choice == 'y')
    {
        std::string dest;
        std::cout << "Enter the destination file: ";
        std::cin.ignore();
        std::getline(std::cin, dest);

        WriteFile(dest, digest);
    }
}

void HashStringScreen()
{
    ClearScreen();
    std::string msg;
    std::cout << "Enter the string to hash: ";
    std::cin.ignore();
    std::getline(std::cin, msg);
    SHA256 sha(msg);
    std::string digest = sha.Digest();
    std::cout << "Hash: " << digest << std::endl;
    SavePrompt(digest);
}

void HashFileString()
{
    ClearScreen();
    std::string src;
    std::cout << "Enter the source file: ";
    std::cin.ignore();
    std::getline(std::cin, src);
    std::string msg = ReadFile(src);
    SHA256 sha(msg);
    std::string digest = sha.Digest();
    std::cout << "Hash: " << digest << std::endl;
    SavePrompt(digest);
}

void ReadHashScreen()
{
    ClearScreen();
    std::string src;
    std::cout << "Enter the source file: ";
    std::cin.ignore();
    std::getline(std::cin, src);
    std::string msg = ReadFile(src);
    std::cout << "Hash: " << msg << std::endl;
}

void LandingScreen()
{
    ClearScreen();
    std::cout << "1. Hash a string" << std::endl;
    std::cout << "2. Hash a file" << std::endl;
    std::cout << "3. Read saved hash" << std::endl;
    std::cout << "4. Exit" << std::endl;

    int action = 0;
    while (action < 1 || action > 4)
    {
        std::cout << "Enter your choice: ";
        std::cin >> action;
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
    LandingScreen();
    return 0;
}