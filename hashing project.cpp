#include "utils.hpp"
#include "sha256.hpp"

#include <iostream>

const int MAX_INPUT_SIZE = 260; // longest path length on Windows

void HashFileScreen()
{
    std::cout << "Enter the path of the source file:" << std::endl;
    char source[MAX_INPUT_SIZE];
    std::cin.getline(source, MAX_INPUT_SIZE);

    char hash[sha256::DIGEST_SIZE];
    char* message = utils::ReadFile(source);

    if (message == nullptr)
    {
        std::cout << "File not found" << std::endl;
        return;
    }

    std::cout << "Message to be hashed: " << message << std::endl;

    sha256::SHA256(message, hash);

    std::cout << "Hashed message: " << hash << std::endl;
    std::cout << "Enter the path of the destination file: ";

    char destination[MAX_INPUT_SIZE];
    std::cin.getline(destination, MAX_INPUT_SIZE);

    utils::WriteFile(destination, hash);
}

void VerifyHashScreen()
{
    std::cout << "Enter the path of the hash file:" << std::endl;
    char source[MAX_INPUT_SIZE];
    std::cin.getline(source, MAX_INPUT_SIZE);

    char* hash = utils::ReadFile(source);

    std::cout << "Enter the original message:" << std::endl;
    char message[MAX_INPUT_SIZE];

    std::cin.getline(message, MAX_INPUT_SIZE);

    if (sha256::Verify(message, hash))
    {
        std::cout << "The hash is valid" << std::endl;
    }
    else
    {
        std::cout << "The hash is invalid" << std::endl;
    }
}

void MainScreen()
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
        HashFileScreen();
    }
    if (choice == 2)
    {
        VerifyHashScreen();
    }
}

int main()
{
    MainScreen();

    return 0;
}