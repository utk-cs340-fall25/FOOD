#include "../headers/food.hpp"
#include "../headers/file_manager.hpp"

#include <map>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>

namespace file = std::filesystem;

struct Ingredient
{
    std::string name;
    std::string amount_s;
    std::string unit;
    double amount_d;
};
struct Recipe
{
    std::string name;
    std::vector<Ingredient> ingredients;
    std::vector<std::string> instructions;
};

std::map<int64_t, std::string> ERROR_MSGS;

uint64_t ERROR_PRINTER(int64_t status, std::string prefix)
{
    std::cerr << prefix << ": ";
    if (ERROR_MSGS.find(status) == ERROR_MSGS.end())
    {
        std::cerr << "Invalid or undefined error code";
        return 0;
    }

    std::cerr << ERROR_MSGS[status];
    return 0;
}
void STATUS_LOADER()
{
    ERROR_MSGS[STATUS_SUCCESS] = "Successfuly performed";
    ERROR_MSGS[STATUS_OPEN_FAILED] = "Invalid permissions";
    ERROR_MSGS[STATUS_FILE_NOT_FOUND] = "No file found at path";
    ERROR_MSGS[STATUS_INVALID_DATA] = "Invalid data in file";
    ERROR_MSGS[STATUS_BAD_ARGUMENTS] = "Invalid arguments were passed";
    ERROR_MSGS[STATUS_FILE_EXISTS] = "File already exists";
    ERROR_MSGS[STATUS_BAD_EXTENSION] = "File has an improper extension";

    return;
}
void INIT(std::list<Recipe> &saved_recipes)
{
    STATUS_LOADER();

    File_Manager loader;
    std::list<Recipe> loaded_recipes;
    uint64_t status;

    file::directory_iterator dir_it("../recipes");
    for (dir_it; dir_it != std::filesystem::end(dir_it); dir_it++)
    {
        struct Recipe recipe;
        status = loader.Read_Recipe(((*dir_it).path()).u8string(), recipe);
        if (status != STATUS_SUCCESS) continue;

        loaded_recipes.push_back(recipe);
    }

    saved_recipes = loaded_recipes;
    return;
}
void DEINIT()
{

}

std::string to_lower(std::string string)
{
    std::string output = string;

    for (uint64_t i = 0; i < string.length(); i += 1)
    {
        output.at(i) = static_cast<char>(tolower(output.at(i)));
    }

    return output;
};