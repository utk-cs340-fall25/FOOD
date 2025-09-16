#include <iostream>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

// For letter tracking in the map
struct letter_count
{
    uint64_t count = 0;
    std::vector<uint64_t> positions;
};

// This is for the output for loop
char letter_reference[] = {'H', 'h', 'E', 'e', 'L', 'l', 'L', 'l', 'O', 'o', 
                           'W', 'w', 'O', 'o', 'R', 'r', 'L', 'l', 'D', 'd'};

int main(void)
{
    // The map of what letters are in the input
    std::map<char, letter_count> letters;

    // Input
    std::string in;
    std::cout << "Enter any string: " << std::endl;
    std::getline(std::cin, in, '\n'); // Reading the full string with spaces, punctuation, etc.

    // Adding all the letters on the input to the map
    for (uint64_t i = 0; i < in.length(); i += 1)
    {
        letters[in.at(i)].count += 1;
        letters[in.at(i)].positions.push_back(i);
    }

    // Checking if hello world can be made
    if (
        (letters['H'].count + letters['h'].count == 0) ||
        (letters['E'].count + letters['e'].count == 0) ||
        (letters['L'].count + letters['l'].count < 3) ||
        (letters['O'].count + letters['o'].count < 2) ||
        (letters['W'].count + letters['w'].count == 0) ||
        (letters['R'].count + letters['r'].count == 0) ||
        (letters['D'].count + letters['d'].count == 0))
    {
        std::cout << "Hello World cannot be written with the given string." << std::endl;
        exit(0);
    }

    // Outputting all the positions where each letter occurs
    for (uint64_t i = 0; i < 10; i += 1)
    {
        std::cout << letter_reference[i*2] << " or " << letter_reference[(i*2) + 1] << ':';
        for (uint64_t j = 0; j < 2; j += 1)
        {
            for (uint64_t k = 0; k < letters[letter_reference[(i*2) + j]].positions.size(); k += 1)
            {
                std::cout << ' ' << letters[letter_reference[(i*2) + j]].positions.at(k);
            }
        }
        std::cout << std::endl;
    }
    std::cout << &main << std::endl;
    exit(0);
}
