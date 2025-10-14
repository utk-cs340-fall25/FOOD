#pragma once

#include <cstdint>
#include <string>
#include <list>

// Status Codes
enum STATUS : uint64_t
{
    STATUS_SUCCESS, // Success Status
    STATUS_OPEN_FAILED,
    STATUS_FILE_NOT_FOUND,
    STATUS_INVALID_DATA,
    STATUS_BAD_ARGUMENTS,
    STATUS_FILE_EXISTS,
    STATUS_BAD_EXTENSION
};

typedef uint64_t STATUS;

struct Ingredient; // Structure for an ingredient
struct Recipe; // Structure for a recipe

void STATUS_LOADER(); // Loads status messages
void ERROR_PRINTER(int64_t status, std::string prefix); // Prints status messages
void INIT(std::list<Recipe>&); // Initializes basic data for the app
void DEINIT();

std::string to_lower(std::string); // Converts a string to all lowercase
STATUS fraction_to_double(std::string, double&); // Converts a string-fraction into a double

STATUS Read_Recipe(const std::string, struct Recipe&); // This will read a .rcp file
STATUS Write_Recipe(const std::string, const struct Recipe&, bool); // This will write to a .rcp file

