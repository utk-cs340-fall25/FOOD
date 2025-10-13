#pragma once

#include <cstdint>
#include <string>
#include <list>

enum STATUS
{
    STATUS_SUCCESS, // Success Status
    STATUS_OPEN_FAILED,
    STATUS_FILE_NOT_FOUND,
    STATUS_INVALID_DATA,
    STATUS_BAD_ARGUMENTS,
    STATUS_FILE_EXISTS,
    STATUS_BAD_EXTENSION
};

struct Ingredient;
struct Recipe;

void STATUS_LOADER();
uint64_t ERROR_PRINTER(int64_t status, std::string prefix);
void INIT(std::list<Recipe>&);
void DEINIT();

std::string to_lower(std::string);

